/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (c) 2018-2019, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _CAM_CSIPHY_2_0_HWREG_H_
#define _CAM_CSIPHY_2_0_HWREG_H_

#include "../cam_csiphy_dev.h"

static const struct csiphy_reg_parms_t csiphy_v2_0 = {
	.mipi_csiphy_interrupt_status0_addr = 0x8B0,
	.mipi_csiphy_interrupt_clear0_addr = 0x858,
	.mipi_csiphy_glbl_irq_cmd_addr = 0x828,
	.csiphy_common_array_size = 8,
	.csiphy_reset_array_size = 5,
	.csiphy_2ph_config_array_size = 15,
	.csiphy_3ph_config_array_size = 17,
	.csiphy_2ph_clock_lane = 0x1,
	.csiphy_2ph_combo_ck_ln = 0x10,
};

static const struct csiphy_reg_t csiphy_common_reg_2_0[] = {
	{0x0814, 0x00, 0x00, CSIPHY_LANE_ENABLE},
	{0x0818, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
	{0x081C, 0x06, 0x00, CSIPHY_3PH_REGS},
	{0x0800, 0x01, 0x01, CSIPHY_DEFAULT_PARAMS},
	{0x0800, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
	{0x0164, 0x00, 0x00, CSIPHY_2PH_REGS},
	{0x0364, 0x00, 0x00, CSIPHY_2PH_REGS},
	{0x0564, 0x00, 0x00, CSIPHY_2PH_REGS},
};

static const struct csiphy_reg_t csiphy_reset_reg_2_0[] = {
	{0x0814, 0x00, 0x05, CSIPHY_LANE_ENABLE},
	{0x0818, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
	{0x081C, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
	{0x0800, 0x01, 0x01, CSIPHY_DEFAULT_PARAMS},
	{0x0800, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
};

static const struct csiphy_reg_t csiphy_irq_reg_2_0[] = {
	{0x082c, 0xff, 0x00, CSIPHY_DEFAULT_PARAMS},
	{0x0830, 0xff, 0x01, CSIPHY_DEFAULT_PARAMS},
	{0x0834, 0xfb, 0x00, CSIPHY_DEFAULT_PARAMS},
	{0x0838, 0xff, 0x00, CSIPHY_DEFAULT_PARAMS},
	{0x083c, 0x7f, 0x00, CSIPHY_DEFAULT_PARAMS},
	{0x0840, 0xff, 0x00, CSIPHY_DEFAULT_PARAMS},
	{0x0844, 0xff, 0x00, CSIPHY_DEFAULT_PARAMS},
	{0x0848, 0xef, 0x00, CSIPHY_DEFAULT_PARAMS},
	{0x084c, 0xff, 0x00, CSIPHY_DEFAULT_PARAMS},
	{0x0850, 0xff, 0x00, CSIPHY_DEFAULT_PARAMS},
	{0x0854, 0xff, 0x00, CSIPHY_DEFAULT_PARAMS},
};

static const
struct csiphy_reg_t csiphy_2ph_v2_0_reg[MAX_LANES][MAX_SETTINGS_PER_LANE] = {
	{
		{0x0030, 0x02, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x002C, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0034, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0028, 0x04, 0x00, CSIPHY_DNP_PARAMS},
		{0x003C, 0xB8, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x001C, 0x0A, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0000, 0xD7, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0004, 0x08, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0020, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0008, 0x04, 0x00, CSIPHY_SETTLE_CNT_LOWER_BYTE},
		{0x000C, 0xFF, 0x00, CSIPHY_DNP_PARAMS},
		{0x0010, 0x50, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0038, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0060, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0064, 0x3F, 0x00, CSIPHY_DEFAULT_PARAMS},
	},
	{
		{0x0730, 0x02, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x072C, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0734, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0728, 0x04, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x073C, 0xB8, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x071C, 0x0A, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0700, 0xC0, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0704, 0x08, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0720, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0708, 0x04, 0x00, CSIPHY_SETTLE_CNT_LOWER_BYTE},
		{0x070C, 0xFF, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0710, 0x50, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0738, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0760, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0764, 0x3F, 0x00, CSIPHY_DEFAULT_PARAMS},
	},
	{
		{0x0230, 0x02, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x022C, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0234, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0228, 0x04, 0x00, CSIPHY_DNP_PARAMS},
		{0x023C, 0xB8, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x021C, 0x0A, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0200, 0xD7, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0204, 0x08, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0220, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0208, 0x04, 0x00, CSIPHY_SETTLE_CNT_LOWER_BYTE},
		{0x020C, 0xFF, 0x00, CSIPHY_DNP_PARAMS},
		{0x0210, 0x50, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0238, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0260, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0264, 0x3F, 0x00, CSIPHY_DEFAULT_PARAMS},
	},
	{
		{0x0430, 0x02, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x042C, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0434, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0428, 0x04, 0x00, CSIPHY_DNP_PARAMS},
		{0x043C, 0xB8, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x041C, 0x0A, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0400, 0xD7, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0404, 0x08, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0420, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0408, 0x04, 0x00, CSIPHY_SETTLE_CNT_LOWER_BYTE},
		{0x040C, 0xFF, 0x00, CSIPHY_DNP_PARAMS},
		{0x0410, 0x50, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0438, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0460, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0464, 0x3F, 0x00, CSIPHY_DEFAULT_PARAMS},
	},
	{
		{0x0630, 0x02, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x062C, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0634, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0628, 0x04, 0x00, CSIPHY_DNP_PARAMS},
		{0x063C, 0xB8, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x061C, 0x0A, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0600, 0xD7, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0604, 0x08, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0620, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0608, 0x04, 0x00, CSIPHY_SETTLE_CNT_LOWER_BYTE},
		{0x060C, 0xFF, 0x00, CSIPHY_DNP_PARAMS},
		{0x0610, 0x50, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0638, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0660, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0664, 0x3F, 0x00, CSIPHY_DEFAULT_PARAMS},
	},
};

static const struct csiphy_reg_t
	csiphy_2ph_v2_0_combo_mode_reg[MAX_LANES][MAX_SETTINGS_PER_LANE] = {
	{
		{0x0030, 0x02, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x002C, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0034, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0028, 0x04, 0x00, CSIPHY_DNP_PARAMS},
		{0x003C, 0xB8, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x001C, 0x0A, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0000, 0xD7, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0004, 0x08, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0020, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0008, 0x04, 0x00, CSIPHY_SETTLE_CNT_LOWER_BYTE},
		{0x000C, 0xFF, 0x00, CSIPHY_DNP_PARAMS},
		{0x0010, 0x50, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0038, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0060, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0064, 0x3F, 0x00, CSIPHY_DEFAULT_PARAMS},
	},
	{
		{0x0730, 0x02, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x072C, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0734, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0728, 0x04, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x073C, 0xB8, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x071C, 0x0A, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0700, 0xC0, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0704, 0x08, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0720, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0708, 0x04, 0x00, CSIPHY_SETTLE_CNT_LOWER_BYTE},
		{0x070C, 0xFF, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0710, 0x50, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0738, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0760, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0764, 0x3F, 0x00, CSIPHY_DEFAULT_PARAMS},
	},
	{
		{0x0230, 0x02, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x022C, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0234, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0228, 0x04, 0x00, CSIPHY_DNP_PARAMS},
		{0x023C, 0xB8, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x021C, 0x0A, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0200, 0xD7, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0204, 0x08, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0220, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0208, 0x04, 0x00, CSIPHY_SETTLE_CNT_LOWER_BYTE},
		{0x020C, 0xFF, 0x00, CSIPHY_DNP_PARAMS},
		{0x0210, 0x50, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0238, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0260, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0264, 0x3F, 0x00, CSIPHY_DEFAULT_PARAMS},
	},
	{
		{0x0430, 0x02, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x042C, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0434, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0428, 0x0A, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x043C, 0xB8, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x041C, 0x0A, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0400, 0xD7, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0404, 0x08, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0420, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0408, 0x04, 0x00, CSIPHY_SETTLE_CNT_LOWER_BYTE},
		{0x040C, 0xFF, 0x00, CSIPHY_DNP_PARAMS},
		{0x0410, 0x50, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0438, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0460, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0464, 0x3F, 0x00, CSIPHY_DEFAULT_PARAMS},
	},
	{
		{0x0630, 0x02, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x062C, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0634, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0628, 0x0E, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x063C, 0xB8, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x061C, 0x0A, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0600, 0xC0, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0604, 0x08, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0620, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0608, 0x04, 0x00, CSIPHY_SETTLE_CNT_LOWER_BYTE},
		{0x060C, 0xFF, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0610, 0x50, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0638, 0x01, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0660, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0664, 0x3F, 0x00, CSIPHY_DEFAULT_PARAMS},
	},
};

static const
struct csiphy_reg_t csiphy_3ph_v2_0_reg[MAX_LANES][MAX_SETTINGS_PER_LANE] = {
	{
		{0x015C, 0x23, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0104, 0x06, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x010C, 0x12, 0x00, CSIPHY_SETTLE_CNT_LOWER_BYTE},
		{0x0108, 0x00, 0x00, CSIPHY_SETTLE_CNT_HIGHER_BYTE},
		{0x0114, 0x20, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0168, 0x70, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x016C, 0x17, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0118, 0x3e, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x011C, 0x41, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0120, 0x41, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0124, 0x7F, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0128, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x012C, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0144, 0x32, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0160, 0x02, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x01CC, 0x41, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0164, 0x40, 0x00, CSIPHY_DEFAULT_PARAMS},
	},
	{
		{0x035C, 0x23, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0304, 0x06, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x030C, 0x12, 0x00, CSIPHY_SETTLE_CNT_LOWER_BYTE},
		{0x0308, 0x00, 0x00, CSIPHY_SETTLE_CNT_HIGHER_BYTE},
		{0x0314, 0x20, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0368, 0x70, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x036C, 0x17, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0318, 0x3e, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x031C, 0x41, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0320, 0x41, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0324, 0x7F, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0328, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x032C, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0344, 0x32, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0360, 0x02, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x03CC, 0x41, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0364, 0x40, 0x00, CSIPHY_DEFAULT_PARAMS},
	},
		{
		{0x055C, 0x23, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0504, 0x06, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x050C, 0x12, 0x00, CSIPHY_SETTLE_CNT_LOWER_BYTE},
		{0x0508, 0x00, 0x00, CSIPHY_SETTLE_CNT_HIGHER_BYTE},
		{0x0514, 0x20, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0568, 0x70, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x056C, 0x17, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0518, 0x3e, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x051C, 0x41, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0520, 0x41, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0524, 0x7F, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0528, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x052C, 0x00, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0544, 0x32, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0560, 0x02, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x05CC, 0x41, 0x00, CSIPHY_DEFAULT_PARAMS},
		{0x0564, 0x40, 0x00, CSIPHY_DEFAULT_PARAMS},
	},
};

static const struct data_rate_settings_t data_rate_delta_table_2_0 = {
	.num_data_rate_settings = 2,
	.data_rate_settings = {
		{
			// data rate <= 2 Gsps
			// max bandwidth = 2 * 2.28 * (10**3) Mbps
			.bandwidth = 4560000000,
			.data_rate_reg_array_size = 3,
			.csiphy_data_rate_regs = {
				{0x0164, 0x50, 0x00, CSIPHY_DEFAULT_PARAMS},
				{0x0364, 0x50, 0x00, CSIPHY_DEFAULT_PARAMS},
				{0x0564, 0x50, 0x00, CSIPHY_DEFAULT_PARAMS}
			}
		},
		{
			// 2 Gsps <= data rate <= 2.5 Gsps
			// max bandwidth = 2.5 * 2.28 * (10**3) Mbps
			.bandwidth = 5700000000,
			.data_rate_reg_array_size = 3,
			.csiphy_data_rate_regs = {
				{0x0164, 0x40, 0x00, CSIPHY_DEFAULT_PARAMS},
				{0x0364, 0x40, 0x00, CSIPHY_DEFAULT_PARAMS},
				{0x0564, 0x40, 0x00, CSIPHY_DEFAULT_PARAMS}
			}
		}
	}
};

#endif /* _CAM_CSIPHY_2_0_HWREG_H_ */
