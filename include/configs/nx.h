/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2022
 * The LineageOS Project
 */

#ifndef _NX_H
#define _NX_H

#include <linux/sizes.h>

#include "tegra210-common.h"

/* High-level configuration options */
#define CONFIG_TEGRA_BOARD_STRING	"Nintendo Switch"

/* Board-specific serial config */
/* #define CONFIG_TEGRA_ENABLE_UARTA */
/* #define CONFIG_TEGRA_ENABLE_UARTB */
/* #define CONFIG_TEGRA_ENABLE_UARTC */

#define BOARD_EXTRA_ENV_SETTINGS \
    /* Defines for inline script */ \
    "defines=" \
        /* FDT IDS FOR VERIFICATION */ \
        "setenv odin_id               0x4F44494E; " \
        "setenv modin_id              0x4F44494F; " \
        "setenv vali_id               0x56414C49; " \
        "setenv frig_id               0x46524947; " \
        /* STAGING ADDRESSES */ \
        "setenv fdt_staging_addr      0x94000000; " \
        "setenv boot_staging_addr     0x98000000; " \
        "setenv recovery_staging_addr 0x98000000; " \
        "setenv env_staging_addr      0x90000000; " \
        /* BOOTARGS FOR UART TYPES */ \
        "setenv uarta_args            \"no_console_suspend console=ttyS0,115200,8n1 androidboot.console=ttyS0\"; " \
        "setenv uartb_args            \"no_console_suspend console=ttyS1,115200,8n1 androidboot.console=ttyS1\"; " \
        "setenv uartc_args            \"no_console_suspend console=ttyS2,115200,8n1 androidboot.console=ttyS2\"; " \
        "setenv usblg_args            \"usb_logging androidboot.console=ttyGS0\"; " \
        "setenv uarta_early           \"earlycon=uart,mmio32,0x70006000\"; " \
        "setenv uartb_early           \"earlycon=uart,mmio32,0x70006040\"; " \
        "setenv uartc_early           \"earlycon=uart,mmio32,0x70006200\"; " \
        "setenv no_args               \"console=null\";\0" \
    "setup_env=" \
        "setenv boot_dir ${prefix}; " \
        "test -n ${loader_rev}           || setenv loader_rev 0; " \
        "test -n ${id}                   || setenv id SWANDR; " \
        "test -n ${emmc}                 || setenv emmc 0; " \
        "test -n ${device_serial}        || mmc info device_serial; " \
        "test -n ${fbconsole}            || setenv fbconsole 9; " \
        "test -n ${uart_port}            || setenv uart_port 0; " \
        "test -n ${earlycon}             || setenv earlycon 0; " \
        "test -n ${r2p_action}           || setenv r2p_action bootloader; " \
        "test -n ${autoboot}             || setenv autoboot 0; " \
        "test -n ${autoboot_list}        || setenv autoboot_list 0; " \
        "test -n ${usb3_enable}          || setenv usb3_enable 0; " \
        "test -n ${4k60_disable}         || setenv 4k60_disable 0; " \
        "test -n ${dvfsb}                || setenv dvfsb 0; " \
        "test -n ${gpu_dvfsc}            || setenv gpu_dvfsc 0; " \
        "test -n ${limit_gpu_clk}        || setenv limit_gpu_clk 0; " \
        "test -n ${touch_skip_tuning}    || setenv touch_skip_tuning 0; " \
        "test -n ${jc_rail_disable}      || setenv jc_rail_disable 0; " \
        "test -n ${wifi_disable_vht80}   || setenv wifi_disable_vht80 0; " \
        "test -n ${alarms_enable}        || setenv alarms_enable 0; " \
        "test -n ${sio_wakeup_disable}   || setenv sio_wakeup_disable 0; " \
        "test -n ${ddr200_enable}        || setenv ddr200_enable 0;" \
        "test -n ${cec_disable}          || setenv cec_disable 0;\0" \
    "setup_calib=" \
        /* IMU */ \
        "test -n ${acc_cal_off_x}        || setenv acc_cal_off_x 0x0; " \
        "test -n ${acc_cal_off_y}        || setenv acc_cal_off_y 0x0; " \
        "test -n ${acc_cal_off_z}        || setenv acc_cal_off_z 0x0; " \
        "test -n ${acc_cal_scl_x}        || setenv acc_cal_scl_x 0x0; " \
        "test -n ${acc_cal_scl_y}        || setenv acc_cal_scl_y 0x0; " \
        "test -n ${acc_cal_scl_z}        || setenv acc_cal_scl_z 0x0; " \
        "test -n ${gyr_cal_off_x}        || setenv gyr_cal_off_x 0x0; " \
        "test -n ${gyr_cal_off_y}        || setenv gyr_cal_off_y 0x0; " \
        "test -n ${gyr_cal_off_z}        || setenv gyr_cal_off_z 0x0; " \
        "test -n ${gyr_cal_scl_x}        || setenv gyr_cal_scl_x 0x0; " \
        "test -n ${gyr_cal_scl_y}        || setenv gyr_cal_scl_y 0x0; " \
        "test -n ${gyr_cal_scl_z}        || setenv gyr_cal_scl_z 0x0; " \
        /* Sio Sticks */ \
        "test -n ${lite_cal_lx_lof}      || setenv lite_cal_lx_lof 0x0; " \
        "test -n ${lite_cal_lx_cnt}      || setenv lite_cal_lx_cnt 0x0; " \
        "test -n ${lite_cal_lx_rof}      || setenv lite_cal_lx_rof 0x0; " \
        "test -n ${lite_cal_ly_dof}      || setenv lite_cal_ly_dof 0x0; " \
        "test -n ${lite_cal_ly_cnt}      || setenv lite_cal_ly_cnt 0x0; " \
        "test -n ${lite_cal_ly_uof}      || setenv lite_cal_ly_uof 0x0; " \
        "test -n ${lite_cal_rx_lof}      || setenv lite_cal_rx_lof 0x0; " \
        "test -n ${lite_cal_rx_cnt}      || setenv lite_cal_rx_cnt 0x0; " \
        "test -n ${lite_cal_rx_rof}      || setenv lite_cal_rx_rof 0x0; " \
        "test -n ${lite_cal_ry_dof}      || setenv lite_cal_ry_dof 0x0; " \
        "test -n ${lite_cal_ry_cnt}      || setenv lite_cal_ry_cnt 0x0; " \
        "test -n ${lite_cal_ry_uof}      || setenv lite_cal_ry_uof 0x0;\0" \
    "address_parse=" \
        "host_mac_addr=0xff; " \
        /* load mac address info file from sd */ \
        "if fatload mmc 1:1 0x90000000 switchroot/joycon_mac.ini; then " \
            "echo Parsing JoyCon pairing info; " \
            "ini \"joycon_00\" 0x90000000; " \
        "fi;\0" \
    "preboot=" \
        "if itest.l *0xA9FBFFFC == 0x33334C42; then " \
            "env import -t 0xA9FC0000 0x20000; " \
        "fi;" \
        "run defines; " \
        "run setup_env; " \
        "run setup_calib;\0" \
    "set_variant=" \
        "if test ${t210b01} = 1; then setenv plat_info T210B01; else setenv plat_info T210; fi; " \
        /* V1 SWITCH */ \
        "if   test ${sku} = 0; then " \
            "setenv dtid ${odin_id}; " \
            "setenv dtrev 0x0; " \
            "setenv variant odin; " \
        /* V2 SWITCH */ \
        "elif test ${sku} = 1; then " \
            "setenv dtid ${modin_id}; " \
            "setenv dtrev 0xb01; " \
            "setenv variant modin; " \
        /* SWITCH LITE */ \
        "elif test ${sku} = 2; then " \
            "setenv dtid ${vali_id}; " \
            "setenv dtrev 0x0; " \
            "setenv variant vali; " \
        /* SWITCH OLED */ \
        "elif test ${sku} = 3; then " \
            "setenv dtid ${frig_id}; " \
            "setenv dtrev 0x0; " \
            "setenv variant frig; " \
        "fi; " \
        "echo SKU: ${variant} REV: ${dtrev}; " \
        "echo Serial: ${device_serial};\0" \
    "default_target=" \
        "setenv mmcdev 1; " \
        "setenv bootpart 0; " \
        "mmc dev ${mmcdev};\0" \
    "emmc_target=" \
        "setenv mmcdev 0; " \
        "mmc dev ${mmcdev};\0" \
    "emmc_overlay=" \
        "fdt set /sdhci@700b0600 status okay; " \
        "fdt set /firmware/android boot_devices sdhci-tegra.3; " \
        "if test \"${mmc_1bit}\" = 1; then " \
            "echo -e eMMC is initialized in 1-bit mode!; " \
            "fdt set /sdhci@700b0600 bus-width <0x1>; " \
            "fdt set /sdhci@700b0600 uhs-mask <0x7F>; " \
        "fi; " \
        "echo -e using eMMC;\0" \
    "sd_overlay=" \
        "fdt set /firmware/android boot_devices sdhci-tegra.0;\0" \
    "touch_overlay=" \
        "setenv bootargs ${bootargs} \"ftm4.skip_tuning=1\";\0" \
    "vht80_overlay=" \
        "setenv bootargs ${bootargs} \"brcmfmac.vht_mask=12\";\0" \
    "ddr200_overlay=" \
        "echo SD DDR200 mode enabled; " \
        "fdt set /sdhci@700b0000 enable-ddr200;\0" \
    "cec_disable_overlay=" \
        "echo HDMI-CEC disabled; " \
        "setenv bootargs ${bootargs} \"cec_disable=1 androidboot.cec_disable=1\";\0" \
    "usb3_overlay=" \
        "echo USB3 disabled; " \
        "fdt get value DHANDLE_USB2 /xusb_padctl@7009f000/pads/usb2/lanes/usb2-0 phandle; " \
        "fdt set /xusb@70090000 phys <$DHANDLE_USB2>; " \
        "fdt set /xusb@70090000 phy-names usb2-0; " \
        "fdt set /xudc@700d0000 phys <$DHANDLE_USB2>; " \
        "fdt set /xudc@700d0000 phy-names usb2; " \
        "fdt set /xusb_padctl@7009f000 usb3-no-mapping-war <0x1>; " \
        "fdt set /xusb_padctl@7009f000/ports/usb2-0 nvidia,usb3-port-fake <0xffffffff>; " \
        "fdt set /xusb_padctl@7009f000/ports/usb3-0 status disabled;\0" \
    "4k60_overlay=" \
        "fdt set /i2c@7000c000/bm92t@18 rohm,dp-lanes <2>; " \
        "echo -e 4K60 disabled;\0" \
    "1bit_overlay=" \
        "echo -e SD Card is initialized in 1-bit mode!; " \
        "fdt set /sdhci@700b0000 bus-width <0x1>; " \
        "fdt set /sdhci@700b0000 uhs-mask <0x7F>;\0" \
    "dvfs_enable=" \
        "echo -e DVFS B-Side enabled; " \
        "setenv bootargs ${bootargs} speedo_tegra210.cspd_id=2 speedo_tegra210.cspd_id=2 speedo_tegra210.gspd_id=2 androidboot.dvfsb=1; " \
        "if test ${gpu_dvfsc} != 1; then " \
		    "setenv bootargs ${bootargs} \"speedo_tegra210.gspd_id=2\"; " \
	    "fi; " \
        "if test ${sku} != 2; then; " \
            /* 2397 MHz CPU and 1075 MHz GPU hard limit */ \
            "fdt set /cpufreq/cpu-scaling-data max-frequency <0x249348>; " \
            "fdt set /dvfs nvidia,gpu-max-freq-khz <0x106800>; " \
        "else; " \
            /* 2091 MHz CPU and 844 MHz GPU hard limit. Vali */ \
            "fdt set /cpufreq/cpu-scaling-data max-frequency <0x1FE7F8>; " \
            "fdt set /dvfs nvidia,gpu-max-freq-khz <0xCE400>; " \
        "fi;\0" \
    "dvfsc_enable=" \
        "echo -e DVFS C-Side GPU enabled; " \
        "setenv bootargs ${bootargs} \"androidboot.dvfsc=1\"; " \
        "if test ${dvfsb} != 1; then " \
            "setenv bootargs ${bootargs} \"speedo_tegra210.sku_id=0x83 speedo_tegra210.gspd_id=3\"; " \
        "else; " \
            "setenv bootargs ${bootargs} \"speedo_tegra210.gspd_id=3\"; " \
        "fi;\0" \
    "gpu_limit_overlay=" \
        /* If not Vali set GPU hard limit to 1075 MHz. */ \
        "if test ${sku} != 2; then " \
            "setenv bootargs ${bootargs} androidboot.gpulim=1; " \
            "echo -e GPU clock limit enabled; " \
            "fdt set /dvfs nvidia,gpu-max-freq-khz <0x106800>; " \
	    "fi;\0" \
    "jc_rail_overlay=" \
        "echo -e Joycon Rails disabled; " \
        "fdt set /serial@70006040 status disabled; " \
        "fdt set /serial@70006040/joyconr status disabled; " \
        "fdt set /serial@70006200 status disabled; " \
        "fdt set /serial@70006200/joyconl status disabled;\0" \
    "alarms_enable_overlay=" \
        "echo -e Wakeup alarms enabled; " \
        "fdt get value DHANDLE_PMC /pmc@7000e400 phandle; " \
        "fdt get value DHANDLE_GPIO /gpio@6000d000 phandle; " \
        /* PMC_WAKE_TYPE_EVENT = 0x1 */ \
        /* PMC_TRIGGER_TYPE_HIGH = 0x100*/ \
        "fdt set /rtc nvidia,pmc-wakeup <$DHANDLE_PMC 0x1 0x10000 0x100>; " \
        /* TEGRA_GPIO(H, 5) --> 7*8 + 5 */ \
        "fdt set /bluedroid_pm bluedroid_pm,host-wake-gpio <$DHANDLE_GPIO 61 0>; " \
        "fdt set /bluedroid_pm interrupt-parent <$DHANDLE_GPIO>; " \
        "fdt set /bluedroid_pm interrupts <61 0x01>; " \
        /* TEGRA_GPIO(A, 2) --> 0*8 + 2 */ \
        "fdt set /pcie@1003000 nvidia,wake-gpio <$DHANDLE_GPIO 2 0>; " \
        "fdt set /brcmfmac_pcie_wlan nvidia,pmc-wakeup <$DHANDLE_PMC 0x1 0x1000 0x100>;\0" \
    "vali_vlim_overlay=" \
        "echo VALI: voltage limits [${VLIM}, ${SOCLIM}]; " \
        "if test \"${VLIM}\" != 1070; then " \
            /* Newer Lite. 4320 mV / 1664 mA. */ \
            "fdt set /i2c@7000c000/battery-charger@6b/charger ti,charge-voltage-limit-millivolt <0x$VLIM>; " \
            "fdt set /i2c@7000c000/battery-charger@6b/charger ti,charge-thermal-voltage-limit <0x$VLIM 0x$VLIM 0x$VLIM 0xFF0>; " \
            "fdt set /i2c@7000c000/battery-gauge@36 maxim,kernel-maximum-soc <0x$SOCLIM>; " \
        "else; " \
            /* Old Lite. 4208 mV / 1536 mA. (Unreleased?) */ \
            "fdt set /i2c@7000c000/battery-charger@6b/charger ti,charge-voltage-limit-millivolt <0x1070>; " \
            "fdt set /i2c@7000c000/battery-charger@6b/charger ti,charge-thermal-voltage-limit <0x1070 0x1070 0x1070 0xF70>; " \
            "fdt set /i2c@7000c000/battery-charger@6b/charger ti,fast-charge-current-limit-milliamp <0x600>; " \
            "fdt set /i2c@7000c000/battery-charger@6b/charger ti,charge-current-limit <0x200 0x240 0x600 0x600>; " \
        "fi;\0" \
    "sio_calib_overlay=" \
        "if load mmc 1:1 ${env_staging_addr} /switchroot/switch.cal; then; " \
            "env import -t -r ${env_staging_addr} ${filesize}; " \
            "echo Sio Calibration set; " \
            "fdt set /serial@70006200/sio sio-stick-cal-l <$lite_cal_lx_lof $lite_cal_lx_cnt $lite_cal_lx_rof $lite_cal_ly_dof $lite_cal_ly_cnt $lite_cal_ly_uof>; " \
            "fdt set /serial@70006200/sio sio-stick-cal-r <$lite_cal_rx_lof $lite_cal_rx_cnt $lite_cal_rx_rof $lite_cal_ry_dof $lite_cal_ry_cnt $lite_cal_ry_uof>; " \
            "fdt set /serial@70006200/sio sio-acc-cal <$acc_cal_off_x $acc_cal_off_y $acc_cal_off_z $acc_cal_scl_x $acc_cal_scl_y $acc_cal_scl_z>; " \
            "fdt set /serial@70006200/sio sio-gyr-cal <$gyr_cal_off_x $gyr_cal_off_y $gyr_cal_off_z $gyr_cal_scl_x $gyr_cal_scl_y $gyr_cal_scl_z>; " \
        "fi;\0" \
    "devboard_overlay=" \
        "echo -e GPU 15A Regulator enabled; " \
        "fdt set /i2c@7000d000/max77812@33 reg <0x31>; " \
        "fdt set /i2c@7000d000/max77812@33/m3vout status disabled; " \
        "fdt set /i2c@7000d000/fan53528@52 status okay; " \
        "fdt set /dvfs nvidia,gpu-max-volt-mv <0x3B6>;\0" \
    "display_overlay=" \
        "if   test ${display_id} = f20;  then echo Display is INN 6.2; fdt get value DHANDLE /host1x@50000000/dsi/panel-i-720p-6-2 phandle; " \
        "elif test ${display_id} = f30;  then echo Display is AUO 6.2; fdt get value DHANDLE /host1x@50000000/dsi/panel-a-720p-6-2 phandle; " \
        "elif test ${display_id} = 10;   then echo Display is JDI 6.2; fdt get value DHANDLE /host1x@50000000/dsi/panel-j-720p-6-2 phandle; " \
        "elif test ${display_id} = 1020; then echo Display is INN 5.5; fdt get value DHANDLE /host1x@50000000/dsi/panel-i-720p-5-5 phandle; " \
        "elif test ${display_id} = 1030; then echo Display is AUO 5.5; fdt get value DHANDLE /host1x@50000000/dsi/panel-a-720p-5-5 phandle; " \
        "elif test ${display_id} = 1040; then echo Display is SHP 5.5; fdt get value DHANDLE /host1x@50000000/dsi/panel-s-720p-5-5 phandle; " \
        "else echo -e Unknown Display ID: ${display_id}!; fi; " \
        "if test -n ${DHANDLE}; then echo Setting Display panel; fdt set /host1x/dsi nvidia,active-panel <$DHANDLE>; fi\0" \
    "get_fdt=" \
        "part start mmc ${mmcdev} dtb dtb_start; " \
        "part size mmc ${mmcdev} dtb dtb_size; " \
        "mmc read ${fdt_staging_addr} ${dtb_start} ${dtb_size}; " \
        "adtimg addr ${fdt_staging_addr}; " \
        "adtimg get dt --id=${dtid} dtaddr dtsize dtidx; " \
        "cp.b ${dtaddr} ${fdt_addr_r} ${dtsize};" \
        "fdt addr ${fdt_addr_r} ${dtsize};" \
        "fdt resize 16384\0" \
    "bootcmd_common=" \
        "run set_variant; " \
        "setenv bootargs init=/init nvdec_enabled=0 pcie_aspm=off vpr_resize tegra_fbmem=0x800000@0xf5a00000 loglevel=8; " \
        "setenv bootargs ${bootargs} firmware_class.path=/vendor/firmware fbcon=map:${fbconsole} consoleblank=0; " \
        "setenv bootargs ${bootargs} pmc_r2p.action=${r2p_action} pmc_r2p.enabled=1 pmc_r2p.param1=${autoboot} pmc_r2p.param2=${autoboot_list}; " \
        "if test $emmc = 1; then run emmc_target; fi; " \
        "run get_fdt; " \
        "if test $emmc = 1; then run emmc_overlay; else run sd_overlay; fi; " \
        "echo uart port (debug): ${uart_port}; " \
        /* UART-A (Onboard UART Port) */ \
        "if test ${uart_port} = 1; then " \
            "setenv bootargs \"${uarta_args} ${bootargs}\"; echo -e Enabled UART-A logging; " \
            "fdt set /serial@70006000 compatible nvidia,tegra20-uart; " \
            "fdt set /serial@70006000 status okay; " \
            "if test ${earlycon} = 1; then; " \
                "echo -e Early logging enabled; " \
                "setenv bootargs ${bootargs} ${uarta_early}; " \
                "fdt set /serial@70006000 reset-names noreset; " \
            "fi; " \
        /* UART-B (Right JoyCon Rail) */ \
        "elif test ${uart_port} = 2; then " \
            "setenv bootargs \"${uartb_args} ${bootargs}\"; echo -e Enabled UART-B logging; " \
            "fdt set /serial@70006040 compatible nvidia,tegra20-uart; " \
            "fdt set /serial@70006040/joyconr status disabled; " \
            "if test ${earlycon} = 1; then; " \
                "echo -e Early logging enabled; " \
                "setenv bootargs ${bootargs} ${uartb_early}; " \
                "fdt set /serial@70006040 reset-names noreset; " \
            "fi; " \
        /* UART-C (Left JoyCon Rail) */ \
        "elif test ${uart_port} = 3; then " \
            "setenv bootargs \"${uartc_args} ${bootargs}\"; echo -e Enabled UART-C logging; " \
            "fdt set /serial@70006200 compatible nvidia,tegra20-uart; " \
            "fdt set /serial@70006200/joyconl status disabled; " \
            "if test ${earlycon} = 1; then; " \
                "echo -e Early logging enabled; " \
                "setenv bootargs ${bootargs} ${uartc_early}; " \
                "fdt set /serial@70006200 reset-names noreset; " \
            "fi; " \
        /* USB Serial */ \
        "elif test ${uart_port} = 4; then " \
            "setenv bootargs \"${usblg_args} ${bootargs}\"; echo -e Enabled USB Serial logging; " \
        /* No serial console */ \
        "else; " \
            "setenv bootargs \"${bootargs} ${no_args}\"; " \
        "fi; " \
        "if test ${4k60_disable} = 1; then run 4k60_overlay; fi; " \
        "if test \"${sd_1bit}\" = 1; then run 1bit_overlay; fi; " \
        "if test ${sku} != 3; then run display_overlay; fi; " \
        "if test ${t210b01} = 1 -a ${dvfsb} = 1; then run dvfs_enable; else setenv bootargs ${bootargs} androidboot.dvfsb=0; fi; " \
        "if test ${t210b01} = 1 -a ${gpu_dvfsc} = 1; then run dvfsc_enable; else setenv bootargs ${bootargs} androidboot.dvfsc=0; fi; " \
        "if test ${t210b01} = 1 -a ${limit_gpu_clk} = 1; then run gpu_limit_overlay; else setenv bootargs ${bootargs} androidboot.gpulim=0; fi; " \
        "if test ${t210b01} = 1 -a -n \"${pmic_type}\" -a ${pmic_type} = 1; then run devboard_overlay; fi; " \
        "if test ${t210b01} = 1 -a \"${loader_rev}\" > 1; then setenv bootargs ${bootargs} \"latency_allowance.ptsa_rework\"; fi; " \
        "if test ${sku} = 2 -a -n \"${VLIM}\"; then run vali_vlim_overlay; fi; " \
        "if test ${sku} = 2; then run sio_calib_overlay; fi; " \
        "if test ${sku} = 2 -a ${sio_wakeup_disable} = 1; then fdt set /serial@70006200/sio nvidia,pmc-wakeup <0>; fi; " \
        "if test ${sku} = 2 -o ${cec_disable} = 1; then run cec_disable_overlay; fi; " \
        "if test ${jc_rail_disable} = 1; then run jc_rail_overlay; fi; " \
        "if test ${touch_skip_tuning} = 1; then run touch_overlay; fi; " \
        "if test ${wifi_disable_vht80} = 1; then run vht80_overlay; fi; " \
        "if test ${alarms_enable} = 1; then run alarms_enable_overlay; fi; " \
        "if test \"${loader_rev}\" != 0 -a ${ddr200_enable} = 1; then run ddr200_overlay; fi; " \
        "if test ${usb3_enable} = 0; then run usb3_overlay; else echo -e USB3 enabled; fi; " \
        /* Try to grab address from joycons, otherwise use defaults */ \
        "run address_parse; " \
        /* We only get BT MAC from JoyCons, so use BT MAC with FF as the last byte for Wi-Fi */ \
        "if test ! -n ${wifi_mac}; then " \
            "if test -n ${host}; then setexpr wifi_mac sub \"\\:..$\" \":FF\" ${host}; else setenv wifi_mac ${device_wifi_mac}; fi;" \
        "fi; " \
        "if test ! -n ${bt_mac}; then " \
            "if test -n ${host}; then setenv bt_mac ${host}; else setenv bt_mac ${device_bt_mac}; fi;" \
        "fi; " \
        "echo BT MAC: ${bt_mac}; " \
        "echo WF MAC: ${wifi_mac}; " \
        /* insert mac address dtb node */ \
        "fdt set /chosen nvidia,wifi-mac ${wifi_mac}; " \
        "fdt set /chosen nvidia,bluetooth-mac ${bt_mac}; " \
        "fdt set / serial-number ${device_serial}; " \
        "setenv bootargs ${bootargs} androidboot.bootloader=${blver} androidboot.hardware=nx androidboot.hardware.sku=${variant} androidboot.serialno=${device_serial} androidboot.modem=none androidboot.dtb_idx=${dtidx};\0" \
    "bootcmd_android=" \
        "part start mmc ${mmcdev} boot boot_start; " \
        "part size mmc ${mmcdev} boot boot_size; " \
        "mmc read ${boot_staging_addr} ${boot_start} ${boot_size}; " \
        "setenv bootargs ro ${bootargs} bluetooth.disable_ertm=1; " \
        "bootm ${boot_staging_addr} ${boot_staging_addr} ${fdt_addr_r};\0" \
    "bootcmd_recovery=" \
        "part start mmc ${mmcdev} recovery recovery_start; " \
        "part size mmc ${mmcdev} recovery recovery_size; " \
        "mmc read ${recovery_staging_addr} ${recovery_start} ${recovery_size}; " \
        "bootm ${recovery_staging_addr} ${recovery_staging_addr} ${fdt_addr_r};\0" \
    "bootcmd=" \
        "run default_target; " \
        "if test -n ${uenvcmd}; then " \
            "echo Running uenvcmd ...; " \
            "run uenvcmd; " \
        "else " \
            "run bootcmd_common; " \
            "if gpio input 190 || test ${recovery} = \"1\"; then " \
                "run bootcmd_recovery; " \
            "else " \
                "run bootcmd_android; " \
            "fi; " \
        "fi;\0"

#include "tegra-common-post.h"

/* Crystal is 38.4MHz. clk_m runs at half that rate */
#define COUNTER_FREQUENCY	19200000

#define CONFIG_SYS_MEM_RESERVE_SECURE (4 * SZ_1M)

#endif /* _NX_H */
