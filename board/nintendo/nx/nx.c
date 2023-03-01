/*
 * (C) Copyright 2022-2023, CTCaer.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <env.h>
#include <i2c.h>
#include <version.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/arch/gpio.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/tegra.h>
#include <asm/arch-tegra/ap.h>
#include <asm/arch-tegra/gp_padctrl.h>
#include <asm/arch-tegra/pmc.h>
#include "../../nvidia/p2571/max77620_init.h"
#include "pinmux-config-nx.h"

#define FUSE_BASE                   0x7000F800
#define FUSE_RESERVED_ODMX(x)       (0x1C8 + 4 * (x))

#define FUSE_OPT_LOT_CODE_0         0x208
#define FUSE_OPT_WAFER_ID           0x210
#define FUSE_OPT_X_COORDINATE       0x214
#define FUSE_OPT_Y_COORDINATE       0x218
#define FUSE_RESERVED_ODM28_T210B01 0x240

void pin_mux_mmc(void)
{
	struct pmc_ctlr *const pmc = (struct pmc_ctlr *)NV_PA_PMC_BASE;
	struct udevice *dev;
	u32 reg_val;
	uchar val;
	int ret;

	/* Make sure the SDMMC1 controller is powered */
	reg_val = readl(&pmc->pmc_no_iopower);
	reg_val |= BIT(12);
	writel(reg_val, &pmc->pmc_no_iopower);
	(void)readl(&pmc->pmc_no_iopower);
	udelay(1000);
	reg_val &= ~(BIT(12));
	writel(reg_val, &pmc->pmc_no_iopower);
	(void)readl(&pmc->pmc_no_iopower);

	/* Inform IO pads that voltage is gonna be 3.3V */
	reg_val = readl(&pmc->pmc_pwr_det_val);
	reg_val |= BIT(12);
	writel(reg_val, &pmc->pmc_pwr_det_val);
	(void)readl(&pmc->pmc_pwr_det_val);

	/* Turn on MAX77620 LDO2 to 3.3V for SD card power */
	ret = i2c_get_chip_for_busnum(5, MAX77620_I2C_ADDR_7BIT, 1, &dev);
	if (ret) {
		printf("%s: Cannot find MAX77620 I2C chip\n", __func__);
		return;
	}
	/* 0xF2 for 3.3v, enabled: bit7:6 = 11 = enable, bit5:0 = voltage */
	val = 0xF2;
	ret = dm_i2c_write(dev, MAX77620_CNFG1_L2_REG, &val, 1);
	if (ret)
		printf("Failed to enable 3.3V LDO for SD Card IO: %d\n", ret);

	/* Disable LDO4 discharge for RTC power. Already disabled on T210B01. */
	ret = dm_i2c_read(dev, MAX77620_CNFG2_L4_REG, &val, 1);
	if (ret) {
		printf("Failed to read LDO4 register: %d\n", ret);
	} else {
		val &= ~BIT(1); /* ADE */
		ret = dm_i2c_write(dev, MAX77620_CNFG2_L4_REG, &val, 1);
		if (ret)
			printf("Failed to disable ADE in LDO4: %d\n", ret);
	}
}

enum {
	NX_HW_TYPE_ODIN,
	NX_HW_TYPE_MODIN,
	NX_HW_TYPE_VALI,
	NX_HW_TYPE_FRIG
};

static int get_sku(bool t210b01)
{
	const volatile void __iomem *odm4 = (void *)(NV_PA_FUSE_BASE + FUSE_RESERVED_ODMX(4));

	if (t210b01)
	{
		switch ((readl(odm4) & 0xF0000) >> 16)
		{
			case 2:
				return NX_HW_TYPE_VALI;

			case 4:
				return NX_HW_TYPE_FRIG;

			case 1:
			default:
				return NX_HW_TYPE_MODIN;
		}
	}

	return NX_HW_TYPE_ODIN;
}

static void pmic_power_off_reset(void)
{
	struct udevice *dev;
	uchar val;
	int ret;

	ret = i2c_get_chip_for_busnum(5, MAX77620_I2C_ADDR_7BIT, 1, &dev);
	if (ret) {
		debug("%s: Cannot find MAX77620 I2C chip\n", __func__);
		return;
	}

	/* Set soft reset wake up reason */
	ret = dm_i2c_read(dev, MAX77620_REG_ONOFF_CFG2, &val, 1);
	if (ret)
		debug("Failed to read ONOFF_CNFG2 register: %d\n", ret);

	val |= BIT(7); /* SFT_RST_WK */
	ret = dm_i2c_write(dev, MAX77620_REG_ONOFF_CFG2, &val, 1);
	if (ret)
		debug("Failed to write ONOFF_CNFG2: %d\n", ret);

	/* Initiate power down sequence and generate a reset */
	val = BIT(7);
	ret = dm_i2c_write(dev, MAX77620_REG_ONOFF_CFG1, &val, 1);
	if (ret)
		debug("Failed to write ONOFF_CNFG1: %d\n", ret);
}

void reset_misc(void)
{
	/* r2p is not possible on T210B01, so do a full power off reboot */
	if (tegra_get_chip_rev() == MAJORPREV_TEGRA210B01) {
		pmic_power_off_reset();

		mdelay(100);

		/* If failed try a complete power off */
		psci_system_off();
	}

	/* r2p reboot */
	psci_system_reset();
}

static void set_pmic_type(void)
{
	const volatile void __iomem *rsvd_odm28 =
				    (void *)(FUSE_BASE + FUSE_RESERVED_ODM28_T210B01);

	u32 odm28 = readl(rsvd_odm28);
	if (odm28 & 1)
		env_set("pmic_type", "0"); /* 0x33 211 phase config (retail) */
	else
		env_set("pmic_type", "1"); /* 0x31 31  phase config (devboard) */
}

static void generate_and_set_serial(bool t210b01)
{
	const volatile void __iomem *opt_lot0 =
				    (void *)(FUSE_BASE + FUSE_OPT_LOT_CODE_0);
	const volatile void __iomem *opt_wafer =
				    (void *)(FUSE_BASE + FUSE_OPT_WAFER_ID);
	const volatile void __iomem *opt_x =
				    (void *)(FUSE_BASE + FUSE_OPT_X_COORDINATE);
	const volatile void __iomem *opt_y =
				    (void *)(FUSE_BASE + FUSE_OPT_Y_COORDINATE);
	u32 lot0 = readl(opt_lot0);
	u32 wfxy = (readl(opt_wafer) << 18) | (readl(opt_x) << 9) | readl(opt_y);
	char buf[32];

	/* Generate serial number */
	switch (get_sku(t210b01)) {
	case NX_HW_TYPE_MODIN:
		sprintf(buf, "%04X%08X%06X", 0x494F, (~lot0) & 0x3FFFFFFF, wfxy);
		break;

	case NX_HW_TYPE_VALI:
		sprintf(buf, "%04X%08X%06X", 0x4C49, (~lot0) & 0x3FFFFFFF, wfxy);
		break;

	case NX_HW_TYPE_FRIG:
		sprintf(buf, "%04X%08X%06X", 0x4947, (~lot0) & 0x3FFFFFFF, wfxy);
		break;

	case NX_HW_TYPE_ODIN:
	default:
		sprintf(buf, "%04X%08X%06X", 0x494E, (~lot0) & 0x3FFFFFFF, wfxy);
		break;
	}

	/* Set serial number to env */
	env_set("device_serial", buf);

	/* Generate default bluetooth mac address and set it to env */
	sprintf(buf, "98:B6:E9:%02X:%02X:%02X",
		(lot0 >> 16) & 0xFF, (lot0 >> 8) & 0xFF, (lot0 + 1) & 0xFF);
	env_set("device_bt_mac", buf);

	/* Generate default wifi mac address and set it to env */
	sprintf(buf, "98:B6:E9:%02X:%02X:%02X",
		(lot0 >> 16) & 0xFF, (lot0 >> 8) & 0xFF, (lot0 + 2) & 0xFF);
	env_set("device_wifi_mac", buf);
}

#define SECURE_SCRATCH112_SETUP_DONE		0x34313254
#define PMC_SCRATCH0_RECOVERY_MODE		(1 << 31)
#define PMC_SCRATCH0_FASTBOOT_MODE		(1 << 30)
#define PMC_SCRATCH0_MASK (PMC_SCRATCH0_RECOVERY_MODE | PMC_SCRATCH0_FASTBOOT_MODE)
static void board_env_setup(void)
{
	u32 scratch0   = tegra_pmc_readl(offsetof(struct pmc_ctlr, pmc_scratch0));
	u32 scratch113 = tegra_pmc_readl(offsetof(struct pmc_ctlr, pmc_secure_scratch113));
	bool t210b01 = tegra_get_chip_rev() == MAJORPREV_TEGRA210B01;
	char sku[2] = { get_sku(t210b01) + '0', 0 };
    u32 in_volt_lim = scratch113 >> 16;

    /* Generate device serial and set it to env */
    generate_and_set_serial(t210b01);

	// If the mariko modchip is not initialized in a specific manner, things will hang.
	// Scratch112 gets set to this specific value on known good modchip payloads.
	if (t210b01 &&
	    tegra_pmc_readl(offsetof(struct pmc_ctlr, pmc_secure_scratch112))
			!= SECURE_SCRATCH112_SETUP_DONE) {
		puts("Board was not initialized properly! Hang prevented.\n"
			"Board will reboot in 10s..\n");
		mdelay(10000);

		/* r2p is not possible so do a full power off reboot */
		pmic_power_off_reset();

        mdelay(100);

		return;
	}

	// Set soc type
	env_set("t210b01", t210b01 ? "1" : "0");

	// Set nx sku
	env_set("sku", sku);

	// Set Display ID
	env_set_hex("display_id", scratch113 & 0xFFFF);

    // Set pmic type for T210b01
	if (t210b01)
		set_pmic_type();

	// Set charging limits for lite
	if (get_sku(t210b01) == NX_HW_TYPE_VALI) {
		env_set_hex("VLIM", in_volt_lim);

        /* Set translated SOC */
		switch (in_volt_lim) {
		case 4192:
			env_set_hex("SOCLIM", 90);
			break;
		case 4224:
			env_set_hex("SOCLIM", 93);
			break;
		case 4240:
			env_set_hex("SOCLIM", 94);
			break;
		case 4256:
			env_set_hex("SOCLIM", 95);
			break;
		case 4272:
			env_set_hex("SOCLIM", 97);
			break;
		case 4288:
			env_set_hex("SOCLIM", 98);
			break;
		case 4304:
			env_set_hex("SOCLIM", 99);
			break;
		default:
			env_set_hex("SOCLIM", 100);
			break;
		}
	}

	// If kernel set reboot to recovery, then set a var for scripts to action on
	env_set("recovery", scratch0 & PMC_SCRATCH0_RECOVERY_MODE ? "1" : "0");

	// Clear out scratch0 mode select bits
	tegra_pmc_writel(scratch0 & ~PMC_SCRATCH0_MASK, offsetof(struct pmc_ctlr, pmc_scratch0));

	env_set("blver", PLAIN_VERSION);
}

// This has nothing to do with a fan, but it gets called at the right time
void start_cpu_fan(void)
{
	board_env_setup();
}

/*
 * Routine: pinmux_init
 * Description: Do individual peripheral pinmux configs
 */
void pinmux_init(void)
{
	gpio_config_table(nx_gpio_inits,
			  ARRAY_SIZE(nx_gpio_inits));

	pinmux_config_pingrp_table(nx_pingrps,
				   ARRAY_SIZE(nx_pingrps));

    if (tegra_get_chip_rev() == MAJORPREV_TEGRA210) {
		pinmux_config_pingrp_table(nx_sd_t210_pingrps,
				ARRAY_SIZE(nx_sd_t210_pingrps));
	} else {
		pinmux_config_pingrp_table(nx_sd_t210b01_pingrps,
				ARRAY_SIZE(nx_sd_t210b01_pingrps));
	}
}
