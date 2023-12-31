/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2018 Linaro Ltd.
 * Sam Protsenko <semen.protsenko@linaro.org>
 */

#ifndef IMAGE_ANDROID_DT_H
#define IMAGE_ANDROID_DT_H

#include <linux/types.h>
#include <dt_table.h>

bool android_dt_check_header(ulong hdr_addr);
bool android_dt_get_fdt_by_index(ulong hdr_addr, u32 index, ulong *addr,
				 u32 *size);
bool android_dt_get_fdt_by_field(ulong hdr_addr, struct dt_table_entry *dte,
				 ulong *addr, u32 *size, u32 *index);

#if !defined(CONFIG_SPL_BUILD)
void android_dt_print_contents(ulong hdr_addr);
#endif

#endif /* IMAGE_ANDROID_DT_H */
