/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (c) 2017-2019, The Linux Foundation. All rights reserved.
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

#ifndef _CAM_CDM_UTIL_H_
#define _CAM_CDM_UTIL_H_

/* Max len for tag name for header while dumping cmd buffer*/
#define CAM_CDM_CMD_TAG_MAX_LEN 32

enum cam_cdm_command {
	CAM_CDM_CMD_UNUSED = 0x0,
	CAM_CDM_CMD_DMI = 0x1,
	CAM_CDM_CMD_NOT_DEFINED = 0x2,
	CAM_CDM_CMD_REG_CONT = 0x3,
	CAM_CDM_CMD_REG_RANDOM = 0x4,
	CAM_CDM_CMD_BUFF_INDIRECT = 0x5,
	CAM_CDM_CMD_GEN_IRQ = 0x6,
	CAM_CDM_CMD_WAIT_EVENT = 0x7,
	CAM_CDM_CMD_CHANGE_BASE = 0x8,
	CAM_CDM_CMD_PERF_CTRL = 0x9,
	CAM_CDM_CMD_DMI_32 = 0xa,
	CAM_CDM_CMD_DMI_64 = 0xb,
	CAM_CDM_CMD_PRIVATE_BASE = 0xc,
	CAM_CDM_CMD_SWD_DMI_32 = (CAM_CDM_CMD_PRIVATE_BASE + 0x64),
	CAM_CDM_CMD_SWD_DMI_64 = (CAM_CDM_CMD_PRIVATE_BASE + 0x65),
	CAM_CDM_CMD_PRIVATE_BASE_MAX = 0x7F
};

/**
 * struct cam_cdm_utils_ops - Camera CDM util ops
 *
 * @cdm_required_size_reg_random: Calculates the size of a reg-random command
 *                                in dwords.
 *      @numRegVals  Number of register/value pairs
 *      @return Size in dwords
 *
 * @cdm_required_size_genirq: Calculates size of a Genirq command in dwords.
 *      @return Size in dwords
 *
 * @cdm_required_size_changebase: Calculates the size of a change-base command
 *                                in dwords.
 *      @return Size in dwords
 *
 * @cdm_write_regrandom: Writes a command into the command buffer in
 *                       register/value pairs.
 *      @pCmdBuffer: Pointer to command buffer
 *      @numRegVals: Number of register/value pairs that will be written
 *      @pRegVals: An array of register/value pairs that will be written
 *                 The even indices are registers and the odd indices
 *                 arevalues, e.g., {reg1, val1, reg2, val2, ...}.
 *      @return Pointer in command buffer pointing past the written commands
 *
 * @cdm_write_changebase: Writes a changing CDM (address) base command into
 *                        the command buffer.
 *      @pCmdBuffer: Pointer to command buffer
 *      @base: New base (device) address
 *      @return Pointer in command buffer pointing past the written commands
 *
 * @cdm_write_genirq:  Writes a gen irq command into the command buffer.
 *      @pCmdBuffer: Pointer to command buffer
 *      @userdata: userdata or cookie return by hardware during irq.
 */
struct cam_cdm_utils_ops {
uint32_t (*cdm_required_size_reg_random)(uint32_t numRegVals);
uint32_t (*cdm_required_size_genirq)(void);
uint32_t (*cdm_required_size_changebase)(void);
uint32_t *(*cdm_write_regrandom)(
	uint32_t *pCmdBuffer,
	uint32_t numRegVals,
	uint32_t *pRegVals);
uint32_t *(*cdm_write_changebase)(
	uint32_t *pCmdBuffer,
	uint32_t base);
void (*cdm_write_genirq)(
	uint32_t *pCmdBuffer,
	uint32_t  userdata);
};

/**
 * struct cam_cdm_cmd_buf_dump_info; - Camera CDM dump info
 * @src_start:       source start address
 * @src_end:         source end   address
 * @dst_start:       dst start address
 * @dst_offset:      dst offset
 * @dst_max_size     max size of destination buffer
 */
struct cam_cdm_cmd_buf_dump_info {
	uint32_t *src_start;
	uint32_t *src_end;
	uintptr_t dst_start;
	uint32_t  dst_offset;
	uint32_t  dst_max_size;
};

/**
 * struct cam_cdm_cmd_dump_header- Camera CDM dump header
 * @tag:       tag name for header
 * @size:      size of data
 * @word_size: size of each word
 */
struct cam_cdm_cmd_dump_header {
	char      tag[CAM_CDM_CMD_TAG_MAX_LEN];
	uint64_t  size;
	uint32_t  word_size;
};

/**
 * cam_cdm_util_log_cmd_bufs()
 *
 * @brief:            Util function to log cdm command buffers
 *
 * @cmd_buffer_start: Pointer to start of cmd buffer
 * @cmd_buffer_end:   Pointer to end of cmd buffer
 *
 */
void cam_cdm_util_dump_cmd_buf(
	uint32_t *cmd_buffer_start, uint32_t *cmd_buffer_end);

/**
 * cam_cdm_util_dump_cmd_bufs_v2()
 *
 * @brief:        Util function to log cdm command buffers
 *                to a buffer
 *
 * @dump_info:    Information about source and destination buffers
 *
 */
void cam_cdm_util_dump_cmd_bufs_v2(
	struct cam_cdm_cmd_buf_dump_info *dump_info);

const struct cam_cdm_utils_ops *cam_cdm_util_get_cmd170_ops(void);

#endif /* _CAM_CDM_UTIL_H_ */
