/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2018 - 2020 Intel Corporation */

#ifndef IPU_PLATFORM_REGS_H
#define IPU_PLATFORM_REGS_H

/*
 * IPU6 uses uniform address within IPU, therefore all subsystem registers
 * locates in one signle space starts from 0 but in different sctions with
 * different addresses, the subsystem offsets are defined to 0 as the
 * register definition will have the address offset to 0.
 */
#define IPU_UNIFIED_OFFSET			0

#define IPU_ISYS_IOMMU0_OFFSET		0x2E0000
#define IPU_ISYS_IOMMU1_OFFSET		0x2E0500
#define IPU_ISYS_IOMMUI_OFFSET		0x2E0A00

#define IPU_PSYS_IOMMU0_OFFSET		0x1B0000
#define IPU_PSYS_IOMMU1_OFFSET		0x1B0700
#define IPU_PSYS_IOMMU1R_OFFSET		0x1B0E00
#define IPU_PSYS_IOMMUI_OFFSET		0x1B1500

/* the offset from IOMMU base register */
#define IPU_MMU_L1_STREAM_ID_REG_OFFSET	0x0c
#define IPU_MMU_L2_STREAM_ID_REG_OFFSET	0x4c
#define IPU_PSYS_MMU1W_L2_STREAM_ID_REG_OFFSET	0x8c

#define IPU_MMU_INFO_OFFSET		0x8

#define IPU_ISYS_SPC_OFFSET		0x210000

#define IPU6SE_PSYS_SPC_OFFSET		0x110000
#define IPU6_PSYS_SPC_OFFSET		0x118000

#define IPU_ISYS_DMEM_OFFSET		0x200000
#define IPU_PSYS_DMEM_OFFSET		0x100000

#define IPU_REG_ISYS_CSI_TOP_CTRL0_IRQ_EDGE		0x238200
#define IPU_REG_ISYS_CSI_TOP_CTRL0_IRQ_MASK		0x238204
#define IPU_REG_ISYS_CSI_TOP_CTRL0_IRQ_STATUS		0x238208
#define IPU_REG_ISYS_CSI_TOP_CTRL0_IRQ_CLEAR		0x23820c
#define IPU_REG_ISYS_CSI_TOP_CTRL0_IRQ_ENABLE		0x238210
#define IPU_REG_ISYS_CSI_TOP_CTRL0_IRQ_LEVEL_NOT_PULSE	0x238214

#define IPU_REG_ISYS_CSI_TOP_CTRL1_IRQ_EDGE		0x238220
#define IPU_REG_ISYS_CSI_TOP_CTRL1_IRQ_MASK		0x238224
#define IPU_REG_ISYS_CSI_TOP_CTRL1_IRQ_STATUS		0x238228
#define IPU_REG_ISYS_CSI_TOP_CTRL1_IRQ_CLEAR		0x23822c
#define IPU_REG_ISYS_CSI_TOP_CTRL1_IRQ_ENABLE		0x238230
#define IPU_REG_ISYS_CSI_TOP_CTRL1_IRQ_LEVEL_NOT_PULSE	0x238234

/* MTL IPU6V6 irq ctrl0 & ctrl1 */
#define IPU6V6_REG_ISYS_CSI_TOP_CTRL0_IRQ_EDGE			0x238700
#define IPU6V6_REG_ISYS_CSI_TOP_CTRL0_IRQ_MASK			0x238704
#define IPU6V6_REG_ISYS_CSI_TOP_CTRL0_IRQ_STATUS		0x238708
#define IPU6V6_REG_ISYS_CSI_TOP_CTRL0_IRQ_CLEAR			0x23870c
#define IPU6V6_REG_ISYS_CSI_TOP_CTRL0_IRQ_ENABLE		0x238710
#define IPU6V6_REG_ISYS_CSI_TOP_CTRL0_IRQ_LEVEL_NOT_PULSE	0x238714

#define IPU6V6_REG_ISYS_CSI_TOP_CTRL1_IRQ_EDGE			0x238720
#define IPU6V6_REG_ISYS_CSI_TOP_CTRL1_IRQ_MASK			0x238724
#define IPU6V6_REG_ISYS_CSI_TOP_CTRL1_IRQ_STATUS		0x238728
#define IPU6V6_REG_ISYS_CSI_TOP_CTRL1_IRQ_CLEAR			0x23872c
#define IPU6V6_REG_ISYS_CSI_TOP_CTRL1_IRQ_ENABLE		0x238730
#define IPU6V6_REG_ISYS_CSI_TOP_CTRL1_IRQ_LEVEL_NOT_PULSE	0x238734

#define IPU_REG_ISYS_UNISPART_IRQ_EDGE			0x27c000
#define IPU_REG_ISYS_UNISPART_IRQ_MASK			0x27c004
#define IPU_REG_ISYS_UNISPART_IRQ_STATUS		0x27c008
#define IPU_REG_ISYS_UNISPART_IRQ_CLEAR			0x27c00c
#define IPU_REG_ISYS_UNISPART_IRQ_ENABLE			0x27c010
#define IPU_REG_ISYS_UNISPART_IRQ_LEVEL_NOT_PULSE	0x27c014
#define IPU_REG_ISYS_UNISPART_SW_IRQ_REG			0x27c414
#define IPU_REG_ISYS_UNISPART_SW_IRQ_MUX_REG		0x27c418
#define IPU_ISYS_UNISPART_IRQ_CSI0			BIT(2)
#define IPU_ISYS_UNISPART_IRQ_CSI1			BIT(3)
#define IPU_ISYS_UNISPART_IRQ_SW			BIT(22)

#define IPU_REG_ISYS_ISL_TOP_IRQ_EDGE			0x2b0200
#define IPU_REG_ISYS_ISL_TOP_IRQ_MASK			0x2b0204
#define IPU_REG_ISYS_ISL_TOP_IRQ_STATUS			0x2b0208
#define IPU_REG_ISYS_ISL_TOP_IRQ_CLEAR			0x2b020c
#define IPU_REG_ISYS_ISL_TOP_IRQ_ENABLE			0x2b0210
#define IPU_REG_ISYS_ISL_TOP_IRQ_LEVEL_NOT_PULSE	0x2b0214

#define IPU_REG_ISYS_CMPR_TOP_IRQ_EDGE			0x2d2100
#define IPU_REG_ISYS_CMPR_TOP_IRQ_MASK			0x2d2104
#define IPU_REG_ISYS_CMPR_TOP_IRQ_STATUS		0x2d2108
#define IPU_REG_ISYS_CMPR_TOP_IRQ_CLEAR			0x2d210c
#define IPU_REG_ISYS_CMPR_TOP_IRQ_ENABLE		0x2d2110
#define IPU_REG_ISYS_CMPR_TOP_IRQ_LEVEL_NOT_PULSE	0x2d2114

/* CDC Burst collector thresholds for isys - 3 FIFOs i = 0..2 */
#define IPU_REG_ISYS_CDC_THRESHOLD(i)		(0x27c400 + ((i) * 4))

#define IPU_ISYS_CSI_PHY_NUM				2
#define IPU_CSI_IRQ_NUM_PER_PIPE			4
#define IPU6SE_ISYS_CSI_PORT_NUM			4
#define IPU6_ISYS_CSI_PORT_NUM				8

#define IPU_ISYS_CSI_PORT_IRQ(irq_num)		(1 << (irq_num))

/* PSYS Info bits*/
#define IPU_REG_PSYS_INFO_SEG_CMEM_MASTER(a)	(0x2C + ((a) * 12))
#define IPU_REG_PSYS_INFO_SEG_XMEM_MASTER(a)	(0x5C + ((a) * 12))

/* PKG DIR OFFSET in IMR in secure mode */
#define IPU_PKG_DIR_IMR_OFFSET			0x40

#define IPU_ISYS_REG_SPC_STATUS_CTRL		0x0

#define IPU_ISYS_SPC_STATUS_START			BIT(1)
#define IPU_ISYS_SPC_STATUS_RUN				BIT(3)
#define IPU_ISYS_SPC_STATUS_READY			BIT(5)
#define IPU_ISYS_SPC_STATUS_CTRL_ICACHE_INVALIDATE	BIT(12)
#define IPU_ISYS_SPC_STATUS_ICACHE_PREFETCH		BIT(13)

#define IPU_PSYS_REG_SPC_STATUS_CTRL		0x0
#define IPU_PSYS_REG_SPC_START_PC		0x4
#define IPU_PSYS_REG_SPC_ICACHE_BASE		0x10
#define IPU_REG_PSYS_INFO_SEG_0_CONFIG_ICACHE_MASTER	0x14

#define IPU_PSYS_SPC_STATUS_START			BIT(1)
#define IPU_PSYS_SPC_STATUS_RUN				BIT(3)
#define IPU_PSYS_SPC_STATUS_READY			BIT(5)
#define IPU_PSYS_SPC_STATUS_CTRL_ICACHE_INVALIDATE	BIT(12)
#define IPU_PSYS_SPC_STATUS_ICACHE_PREFETCH		BIT(13)

#define IPU_PSYS_REG_SPP0_STATUS_CTRL			0x20000

#define IPU_INFO_ENABLE_SNOOP			BIT(0)
#define IPU_INFO_DEC_FORCE_FLUSH		BIT(1)
#define IPU_INFO_DEC_PASS_THRU			BIT(2)
#define IPU_INFO_ZLW                            BIT(3)
#define IPU_INFO_STREAM_ID_SET(id)		(((id) & 0x1F) << 4)
#define IPU_INFO_REQUEST_DESTINATION_IOSF	BIT(9)
#define IPU_INFO_IMR_BASE			BIT(10)
#define IPU_INFO_IMR_DESTINED			BIT(11)

#define IPU_INFO_REQUEST_DESTINATION_PRIMARY IPU_INFO_REQUEST_DESTINATION_IOSF

/* Trace unit related register definitions */
#define TRACE_REG_MAX_ISYS_OFFSET	0xfffff
#define TRACE_REG_MAX_PSYS_OFFSET	0xfffff
#define IPU_ISYS_OFFSET			IPU_ISYS_DMEM_OFFSET
#define IPU_PSYS_OFFSET			IPU_PSYS_DMEM_OFFSET
/* ISYS trace unit registers */
/* Trace unit base offset */
#define IPU_TRACE_REG_IS_TRACE_UNIT_BASE		0x27d000
/* Trace monitors */
#define IPU_TRACE_REG_IS_SP_EVQ_BASE		0x211000
/* GPC blocks */
#define IPU_TRACE_REG_IS_SP_GPC_BASE		0x210800
#define IPU_TRACE_REG_IS_ISL_GPC_BASE		0x2b0a00
#define IPU_TRACE_REG_IS_MMU_GPC_BASE		0x2e0f00
/* each CSI2 port has a dedicated trace monitor, index 0..7 */
#define IPU_TRACE_REG_CSI2_TM_BASE(port)	(0x220400 + 0x1000 * (port))

/* Trace timers */
#define IPU_TRACE_REG_IS_GPREG_TRACE_TIMER_RST_N		0x27c410
#define TRACE_REG_GPREG_TRACE_TIMER_RST_OFF		BIT(0)

/* SIG2CIO */
#define IPU_TRACE_REG_CSI2_PORT_SIG2SIO_GR_BASE(port)		\
			(0x220e00 + (port) * 0x1000)

/* PSYS trace unit registers */
/* Trace unit base offset */
#define IPU_TRACE_REG_PS_TRACE_UNIT_BASE		0x1b4000
/* Trace monitors */
#define IPU_TRACE_REG_PS_SPC_EVQ_BASE			0x119000
#define IPU_TRACE_REG_PS_SPP0_EVQ_BASE			0x139000

/* GPC blocks */
#define IPU_TRACE_REG_PS_SPC_GPC_BASE			0x118800
#define IPU_TRACE_REG_PS_SPP0_GPC_BASE			0x138800
#define IPU_TRACE_REG_PS_MMU_GPC_BASE			0x1b1b00

/* Trace timers */
#define IPU_TRACE_REG_PS_GPREG_TRACE_TIMER_RST_N	0x1aa714

/*
 * s2m_pixel_soc_pixel_remapping is dedicated for the enableing of the
 * pixel s2m remp ability.Remap here  means that s2m rearange the order
 * of the pixels in each 4 pixels group.
 * For examle, mirroring remping means that if input's 4 first pixels
 * are 1 2 3 4 then in output we should see 4 3 2 1 in this 4 first pixels.
 * 0xE4 is from s2m MAS document. It means no remaping.
 */
#define S2M_PIXEL_SOC_PIXEL_REMAPPING_FLAG_NO_REMAPPING 0xE4
/*
 * csi_be_soc_pixel_remapping is for the enabling of the csi\mipi be pixel
 * remapping feature. This remapping is exactly like the stream2mmio remapping.
 */
#define CSI_BE_SOC_PIXEL_REMAPPING_FLAG_NO_REMAPPING    0xE4

#define IPU_REG_DMA_TOP_AB_GROUP1_BASE_ADDR         0x1AE000
#define IPU_REG_DMA_TOP_AB_GROUP2_BASE_ADDR         0x1AF000
#define IPU_REG_DMA_TOP_AB_RING_MIN_OFFSET(n)       (0x4 + (n) * 0xC)
#define IPU_REG_DMA_TOP_AB_RING_MAX_OFFSET(n)       (0x8 + (n) * 0xC)
#define IPU_REG_DMA_TOP_AB_RING_ACCESS_OFFSET(n)    (0xC + (n) * 0xC)

enum ipu_device_ab_group1_target_id {
	IPU_DEVICE_AB_GROUP1_TARGET_ID_R0_SPC_DMEM,
	IPU_DEVICE_AB_GROUP1_TARGET_ID_R1_SPC_DMEM,
	IPU_DEVICE_AB_GROUP1_TARGET_ID_R2_SPC_DMEM,
	IPU_DEVICE_AB_GROUP1_TARGET_ID_R3_SPC_STATUS_REG,
	IPU_DEVICE_AB_GROUP1_TARGET_ID_R4_SPC_MASTER_BASE_ADDR,
	IPU_DEVICE_AB_GROUP1_TARGET_ID_R5_SPC_PC_STALL,
	IPU_DEVICE_AB_GROUP1_TARGET_ID_R6_SPC_EQ,
	IPU_DEVICE_AB_GROUP1_TARGET_ID_R7_SPC_RESERVED,
	IPU_DEVICE_AB_GROUP1_TARGET_ID_R8_SPC_RESERVED,
	IPU_DEVICE_AB_GROUP1_TARGET_ID_R9_SPP0,
	IPU_DEVICE_AB_GROUP1_TARGET_ID_R10_SPP1,
	IPU_DEVICE_AB_GROUP1_TARGET_ID_R11_CENTRAL_R1,
	IPU_DEVICE_AB_GROUP1_TARGET_ID_R12_IRQ,
	IPU_DEVICE_AB_GROUP1_TARGET_ID_R13_CENTRAL_R2,
	IPU_DEVICE_AB_GROUP1_TARGET_ID_R14_DMA,
	IPU_DEVICE_AB_GROUP1_TARGET_ID_R15_DMA,
	IPU_DEVICE_AB_GROUP1_TARGET_ID_R16_GP,
	IPU_DEVICE_AB_GROUP1_TARGET_ID_R17_ZLW_INSERTER,
	IPU_DEVICE_AB_GROUP1_TARGET_ID_R18_AB,
};

enum nci_ab_access_mode {
	NCI_AB_ACCESS_MODE_RW,	/* read & write */
	NCI_AB_ACCESS_MODE_RO,	/* read only */
	NCI_AB_ACCESS_MODE_WO,	/* write only */
	NCI_AB_ACCESS_MODE_NA	/* No access at all */
};

/*
 * 3:0 CSI_PORT.irq_out[3:0] CSI_PORT_CTRL0 IRQ outputs (4bits)
 * [0] CSI_PORT.IRQ_CTRL0_csi
 * [1] CSI_PORT.IRQ_CTRL1_csi_sync
 * [2] CSI_PORT.IRQ_CTRL2_s2m_sids0to7
 * [3] CSI_PORT.IRQ_CTRL3_s2m_sids8to15
 */
#define IPU_ISYS_UNISPART_IRQ_CSI2(port)		\
				   (0x3 << ((port) * IPU_CSI_IRQ_NUM_PER_PIPE))

/* IRQ-related registers in PSYS */
#define IPU_REG_PSYS_GPDEV_IRQ_EDGE		0x1aa200
#define IPU_REG_PSYS_GPDEV_IRQ_MASK		0x1aa204
#define IPU_REG_PSYS_GPDEV_IRQ_STATUS		0x1aa208
#define IPU_REG_PSYS_GPDEV_IRQ_CLEAR		0x1aa20c
#define IPU_REG_PSYS_GPDEV_IRQ_ENABLE		0x1aa210
#define IPU_REG_PSYS_GPDEV_IRQ_LEVEL_NOT_PULSE	0x1aa214
/* There are 8 FW interrupts, n = 0..7 */
#define IPU_PSYS_GPDEV_FWIRQ0			5
#define IPU_PSYS_GPDEV_FWIRQ1			6
#define IPU_PSYS_GPDEV_FWIRQ2			7
#define IPU_PSYS_GPDEV_FWIRQ3			8
#define IPU_PSYS_GPDEV_FWIRQ4			9
#define IPU_PSYS_GPDEV_FWIRQ5			10
#define IPU_PSYS_GPDEV_FWIRQ6			11
#define IPU_PSYS_GPDEV_FWIRQ7			12
#define IPU_PSYS_GPDEV_IRQ_FWIRQ(n)		(1 << (n))
#define IPU_REG_PSYS_GPDEV_FWIRQ(n)		(4 * (n) + 0x1aa100)

/*
 * ISYS GPC (Gerneral Performance Counters) Registers
 */
#define IPU_ISYS_GPC_BASE			0x2E0000
#define IPU_ISYS_GPREG_TRACE_TIMER_RST		0x27C410
enum ipu_isf_cdc_mmu_gpc_registers {
	IPU_ISF_CDC_MMU_GPC_SOFT_RESET  = 0x0F00,
	IPU_ISF_CDC_MMU_GPC_OVERALL_ENABLE  = 0x0F04,
	IPU_ISF_CDC_MMU_GPC_ENABLE0  = 0x0F20,
	IPU_ISF_CDC_MMU_GPC_VALUE0  = 0x0F60,
	IPU_ISF_CDC_MMU_GPC_CNT_SEL0 = 0x0FA0,
};

/*
 * GPC (Gerneral Performance Counters) Registers
 */
#define IPU_GPC_BASE 0x1B0000
#define IPU_GPREG_TRACE_TIMER_RST	0x1AA714
enum ipu_cdc_mmu_gpc_registers {
	IPU_CDC_MMU_GPC_SOFT_RESET  = 0x1B00,
	IPU_CDC_MMU_GPC_OVERALL_ENABLE  = 0x1B04,
	IPU_CDC_MMU_GPC_TRACE_HEADER  = 0x1B08,
	IPU_CDC_MMU_GPC_TRACE_ADDR  = 0x1B0C,
	IPU_CDC_MMU_GPC_TRACE_ENABLE_NPK  = 0x1B10,
	IPU_CDC_MMU_GPC_TRACE_ENABLE_DDR  = 0x1B14,
	IPU_CDC_MMU_GPC_LP_CLEAR  = 0x1B18,
	IPU_CDC_MMU_GPC_LOST_PACKET  = 0x1B1C,
	IPU_CDC_MMU_GPC_ENABLE0  = 0x1B20,
	IPU_CDC_MMU_GPC_VALUE0  = 0x1B60,
	IPU_CDC_MMU_GPC_CNT_SEL0 = 0x1BA0,
	IPU_CDC_MMU_GPC_START_SEL0 = 0x1BE0,
	IPU_CDC_MMU_GPC_STOP_SEL0 = 0x1C20,
	IPU_CDC_MMU_GPC_MSG_SEL0 = 0x1C60,
	IPU_CDC_MMU_GPC_PLOAD_SEL0 = 0x1CA0,
	IPU_CDC_MMU_GPC_IRQ_TRIGGER_VALUE0 = 0x1CE0,
	IPU_CDC_MMU_GPC_IRQ_TIMER_SEL0 = 0x1D20,
	IPU_CDC_MMU_GPC_IRQ_ENABLE0 = 0x1D60,
	IPU_CDC_MMU_GPC_END = 0x1D9C
};

#define IPU_GPC_SENSE_OFFSET		7
#define IPU_GPC_ROUTE_OFFSET		5
#define IPU_GPC_SOURCE_OFFSET		0

/*
 * Signals monitored by GPC
 */
#define IPU_GPC_TRACE_TLB_MISS_MMU_LB_IDX		0
#define IPU_GPC_TRACE_FULL_WRITE_LB_IDX			1
#define IPU_GPC_TRACE_NOFULL_WRITE_LB_IDX		2
#define IPU_GPC_TRACE_FULL_READ_LB_IDX			3
#define IPU_GPC_TRACE_NOFULL_READ_LB_IDX		4
#define IPU_GPC_TRACE_STALL_LB_IDX			5
#define IPU_GPC_TRACE_ZLW_LB_IDX			6
#define IPU_GPC_TRACE_TLB_MISS_MMU_HBTX_IDX		7
#define IPU_GPC_TRACE_FULL_WRITE_HBTX_IDX		8
#define IPU_GPC_TRACE_NOFULL_WRITE_HBTX_IDX		9
#define IPU_GPC_TRACE_FULL_READ_HBTX_IDX		10
#define IPU_GPC_TRACE_STALL_HBTX_IDX			11
#define IPU_GPC_TRACE_ZLW_HBTX_IDX			12
#define IPU_GPC_TRACE_TLB_MISS_MMU_HBFRX_IDX		13
#define IPU_GPC_TRACE_FULL_READ_HBFRX_IDX		14
#define IPU_GPC_TRACE_NOFULL_READ_HBFRX_IDX		15
#define IPU_GPC_TRACE_STALL_HBFRX_IDX			16
#define IPU_GPC_TRACE_ZLW_HBFRX_IDX			17
#define IPU_GPC_TRACE_TLB_MISS_ICACHE_IDX		18
#define IPU_GPC_TRACE_FULL_READ_ICACHE_IDX		19
#define IPU_GPC_TRACE_STALL_ICACHE_IDX			20
/*
 * psys subdomains power request regs
 */
enum ipu_device_buttress_psys_domain_pos {
	IPU_PSYS_SUBDOMAIN_ISA		= 0,
	IPU_PSYS_SUBDOMAIN_PSA		= 1,
	IPU_PSYS_SUBDOMAIN_BB		= 2,
	IPU_PSYS_SUBDOMAIN_IDSP1	= 3, /* only in IPU6M */
	IPU_PSYS_SUBDOMAIN_IDSP2	= 4, /* only in IPU6M */
};

#define IPU_PSYS_SUBDOMAINS_POWER_MASK  (BIT(IPU_PSYS_SUBDOMAIN_ISA) | \
					 BIT(IPU_PSYS_SUBDOMAIN_PSA) | \
					 BIT(IPU_PSYS_SUBDOMAIN_BB))

#define IPU_PSYS_SUBDOMAINS_POWER_REQ                   0xa0
#define IPU_PSYS_SUBDOMAINS_POWER_STATUS                0xa4

#endif /* IPU_PLATFORM_REGS_H */
