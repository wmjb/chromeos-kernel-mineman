// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2018 MediaTek Inc.
 * Author: Jitao Shi <jitao.shi@mediatek.com>
 */

#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/regulator/consumer.h>

#include <drm/drm_connector.h>
#include <drm/drm_crtc.h>
#include <drm/drm_mipi_dsi.h>
#include <drm/drm_panel.h>

#include <video/mipi_display.h>

struct panel_desc {
	const struct drm_display_mode *modes;
	unsigned int bpc;

	/**
	 * @width_mm: width of the panel's active display area
	 * @height_mm: height of the panel's active display area
	 */
	struct {
		unsigned int width_mm;
		unsigned int height_mm;
	} size;

	unsigned long mode_flags;
	enum mipi_dsi_pixel_format format;
	int (*init)(struct mipi_dsi_device *dsi);
	unsigned int lanes;
	bool discharge_on_disable;
	bool lp11_before_reset;
};

struct boe_panel {
	struct drm_panel base;
	struct mipi_dsi_device *dsi;

	const struct panel_desc *desc;

	enum drm_panel_orientation orientation;
	struct regulator *pp3300;
	struct regulator *pp1800;
	struct regulator *avee;
	struct regulator *avdd;
	struct gpio_desc *enable_gpio;
};

static int boe_tv110c9m_init(struct mipi_dsi_device *dsi)
{
	int ret;

	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x20);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0x05, 0xD9);
	mipi_dsi_dcs_write_seq(dsi, 0x07, 0x78);
	mipi_dsi_dcs_write_seq(dsi, 0x08, 0x5A);
	mipi_dsi_dcs_write_seq(dsi, 0x0D, 0x63);
	mipi_dsi_dcs_write_seq(dsi, 0x0E, 0x91);
	mipi_dsi_dcs_write_seq(dsi, 0x0F, 0x73);
	mipi_dsi_dcs_write_seq(dsi, 0x95, 0xE6);
	mipi_dsi_dcs_write_seq(dsi, 0x96, 0xF0);
	mipi_dsi_dcs_write_seq(dsi, 0x30, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x6D, 0x66);
	mipi_dsi_dcs_write_seq(dsi, 0x75, 0xA2);
	mipi_dsi_dcs_write_seq(dsi, 0x77, 0x3B);

	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x00, 0x08, 0x00, 0x23, 0x00, 0x4D, 0x00, 0x6D, 0x00, 0x89, 0x00, 0xA1, 0x00, 0xB6, 0x00, 0xC9);
	mipi_dsi_dcs_write_seq(dsi, 0xB1, 0x00, 0xDA, 0x01, 0x13, 0x01, 0x3C, 0x01, 0x7E, 0x01, 0xAB, 0x01, 0xF7, 0x02, 0x2F, 0x02, 0x31);
	mipi_dsi_dcs_write_seq(dsi, 0xB2, 0x02, 0x67, 0x02, 0xA6, 0x02, 0xD1, 0x03, 0x08, 0x03, 0x2E, 0x03, 0x5B, 0x03, 0x6B, 0x03, 0x7B);
	mipi_dsi_dcs_write_seq(dsi, 0xB3, 0x03, 0x8E, 0x03, 0xA2, 0x03, 0xB7, 0x03, 0xE7, 0x03, 0xFD, 0x03, 0xFF);

	mipi_dsi_dcs_write_seq(dsi, 0xB4, 0x00, 0x08, 0x00, 0x23, 0x00, 0x4D, 0x00, 0x6D, 0x00, 0x89, 0x00, 0xA1, 0x00, 0xB6, 0x00, 0xC9);
	mipi_dsi_dcs_write_seq(dsi, 0xB5, 0x00, 0xDA, 0x01, 0x13, 0x01, 0x3C, 0x01, 0x7E, 0x01, 0xAB, 0x01, 0xF7, 0x02, 0x2F, 0x02, 0x31);
	mipi_dsi_dcs_write_seq(dsi, 0xB6, 0x02, 0x67, 0x02, 0xA6, 0x02, 0xD1, 0x03, 0x08, 0x03, 0x2E, 0x03, 0x5B, 0x03, 0x6B, 0x03, 0x7B);
	mipi_dsi_dcs_write_seq(dsi, 0xB7, 0x03, 0x8E, 0x03, 0xA2, 0x03, 0xB7, 0x03, 0xE7, 0x03, 0xFD, 0x03, 0xFF);
	mipi_dsi_dcs_write_seq(dsi, 0xB8, 0x00, 0x08, 0x00, 0x23, 0x00, 0x4D, 0x00, 0x6D, 0x00, 0x89, 0x00, 0xA1, 0x00, 0xB6, 0x00, 0xC9);
	mipi_dsi_dcs_write_seq(dsi, 0xB9, 0x00, 0xDA, 0x01, 0x13, 0x01, 0x3C, 0x01, 0x7E, 0x01, 0xAB, 0x01, 0xF7, 0x02, 0x2F, 0x02, 0x31);
	mipi_dsi_dcs_write_seq(dsi, 0xBA, 0x02, 0x67, 0x02, 0xA6, 0x02, 0xD1, 0x03, 0x08, 0x03, 0x2E, 0x03, 0x5B, 0x03, 0x6B, 0x03, 0x7B);
	mipi_dsi_dcs_write_seq(dsi, 0xBB, 0x03, 0x8E, 0x03, 0xA2, 0x03, 0xB7, 0x03, 0xE7, 0x03, 0xFD, 0x03, 0xFF);

	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x21);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);

	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x45, 0x00, 0x65, 0x00, 0x81, 0x00, 0x99, 0x00, 0xAE, 0x00, 0xC1);
	mipi_dsi_dcs_write_seq(dsi, 0xB1, 0x00, 0xD2, 0x01, 0x0B, 0x01, 0x34, 0x01, 0x76, 0x01, 0xA3, 0x01, 0xEF, 0x02, 0x27, 0x02, 0x29);
	mipi_dsi_dcs_write_seq(dsi, 0xB2, 0x02, 0x5F, 0x02, 0x9E, 0x02, 0xC9, 0x03, 0x00, 0x03, 0x26, 0x03, 0x53, 0x03, 0x63, 0x03, 0x73);

	mipi_dsi_dcs_write_seq(dsi, 0xB3, 0x03, 0x86, 0x03, 0x9A, 0x03, 0xAF, 0x03, 0xDF, 0x03, 0xF5, 0x03, 0xE0);
	mipi_dsi_dcs_write_seq(dsi, 0xB4, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x45, 0x00, 0x65, 0x00, 0x81, 0x00, 0x99, 0x00, 0xAE, 0x00, 0xC1);
	mipi_dsi_dcs_write_seq(dsi, 0xB5, 0x00, 0xD2, 0x01, 0x0B, 0x01, 0x34, 0x01, 0x76, 0x01, 0xA3, 0x01, 0xEF, 0x02, 0x27, 0x02, 0x29);
	mipi_dsi_dcs_write_seq(dsi, 0xB6, 0x02, 0x5F, 0x02, 0x9E, 0x02, 0xC9, 0x03, 0x00, 0x03, 0x26, 0x03, 0x53, 0x03, 0x63, 0x03, 0x73);
	mipi_dsi_dcs_write_seq(dsi, 0xB7, 0x03, 0x86, 0x03, 0x9A, 0x03, 0xAF, 0x03, 0xDF, 0x03, 0xF5, 0x03, 0xE0);

	mipi_dsi_dcs_write_seq(dsi, 0xB8, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x45, 0x00, 0x65, 0x00, 0x81, 0x00, 0x99, 0x00, 0xAE, 0x00, 0xC1);
	mipi_dsi_dcs_write_seq(dsi, 0xB9, 0x00, 0xD2, 0x01, 0x0B, 0x01, 0x34, 0x01, 0x76, 0x01, 0xA3, 0x01, 0xEF, 0x02, 0x27, 0x02, 0x29);
	mipi_dsi_dcs_write_seq(dsi, 0xBA, 0x02, 0x5F, 0x02, 0x9E, 0x02, 0xC9, 0x03, 0x00, 0x03, 0x26, 0x03, 0x53, 0x03, 0x63, 0x03, 0x73);

	mipi_dsi_dcs_write_seq(dsi, 0xBB, 0x03, 0x86, 0x03, 0x9A, 0x03, 0xAF, 0x03, 0xDF, 0x03, 0xF5, 0x03, 0xE0);
	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x24);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);

	mipi_dsi_dcs_write_seq(dsi, 0x00, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x01, 0x00);

	mipi_dsi_dcs_write_seq(dsi, 0x02, 0x1C);
	mipi_dsi_dcs_write_seq(dsi, 0x03, 0x1C);

	mipi_dsi_dcs_write_seq(dsi, 0x04, 0x1D);
	mipi_dsi_dcs_write_seq(dsi, 0x05, 0x1D);

	mipi_dsi_dcs_write_seq(dsi, 0x06, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0x07, 0x04);

	mipi_dsi_dcs_write_seq(dsi, 0x08, 0x0F);
	mipi_dsi_dcs_write_seq(dsi, 0x09, 0x0F);

	mipi_dsi_dcs_write_seq(dsi, 0x0A, 0x0E);
	mipi_dsi_dcs_write_seq(dsi, 0x0B, 0x0E);

	mipi_dsi_dcs_write_seq(dsi, 0x0C, 0x0D);
	mipi_dsi_dcs_write_seq(dsi, 0x0D, 0x0D);

	mipi_dsi_dcs_write_seq(dsi, 0x0E, 0x0C);
	mipi_dsi_dcs_write_seq(dsi, 0x0F, 0x0C);

	mipi_dsi_dcs_write_seq(dsi, 0x10, 0x08);
	mipi_dsi_dcs_write_seq(dsi, 0x11, 0x08);

	mipi_dsi_dcs_write_seq(dsi, 0x12, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x13, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x14, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x15, 0x00);

	mipi_dsi_dcs_write_seq(dsi, 0x16, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x17, 0x00);

	mipi_dsi_dcs_write_seq(dsi, 0x18, 0x1C);
	mipi_dsi_dcs_write_seq(dsi, 0x19, 0x1C);

	mipi_dsi_dcs_write_seq(dsi, 0x1A, 0x1D);
	mipi_dsi_dcs_write_seq(dsi, 0x1B, 0x1D);

	mipi_dsi_dcs_write_seq(dsi, 0x1C, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0x1D, 0x04);

	mipi_dsi_dcs_write_seq(dsi, 0x1E, 0x0F);
	mipi_dsi_dcs_write_seq(dsi, 0x1F, 0x0F);

	mipi_dsi_dcs_write_seq(dsi, 0x20, 0x0E);
	mipi_dsi_dcs_write_seq(dsi, 0x21, 0x0E);

	mipi_dsi_dcs_write_seq(dsi, 0x22, 0x0D);
	mipi_dsi_dcs_write_seq(dsi, 0x23, 0x0D);

	mipi_dsi_dcs_write_seq(dsi, 0x24, 0x0C);
	mipi_dsi_dcs_write_seq(dsi, 0x25, 0x0C);

	mipi_dsi_dcs_write_seq(dsi, 0x26, 0x08);
	mipi_dsi_dcs_write_seq(dsi, 0x27, 0x08);

	mipi_dsi_dcs_write_seq(dsi, 0x28, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x29, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x2A, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x2B, 0x00);

	mipi_dsi_dcs_write_seq(dsi, 0x2D, 0x20);
	mipi_dsi_dcs_write_seq(dsi, 0x2F, 0x0A);
	mipi_dsi_dcs_write_seq(dsi, 0x30, 0x44);
	mipi_dsi_dcs_write_seq(dsi, 0x33, 0x0C);
	mipi_dsi_dcs_write_seq(dsi, 0x34, 0x32);

	mipi_dsi_dcs_write_seq(dsi, 0x37, 0x44);
	mipi_dsi_dcs_write_seq(dsi, 0x38, 0x40);
	mipi_dsi_dcs_write_seq(dsi, 0x39, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x3A, 0x5D);
	mipi_dsi_dcs_write_seq(dsi, 0x3B, 0x60);
	mipi_dsi_dcs_write_seq(dsi, 0x3D, 0x42);
	mipi_dsi_dcs_write_seq(dsi, 0x3F, 0x06);
	mipi_dsi_dcs_write_seq(dsi, 0x43, 0x06);
	mipi_dsi_dcs_write_seq(dsi, 0x47, 0x66);
	mipi_dsi_dcs_write_seq(dsi, 0x4A, 0x5D);
	mipi_dsi_dcs_write_seq(dsi, 0x4B, 0x60);
	mipi_dsi_dcs_write_seq(dsi, 0x4C, 0x91);
	mipi_dsi_dcs_write_seq(dsi, 0x4D, 0x21);
	mipi_dsi_dcs_write_seq(dsi, 0x4E, 0x43);
	mipi_dsi_dcs_write_seq(dsi, 0x51, 0x12);
	mipi_dsi_dcs_write_seq(dsi, 0x52, 0x34);
	mipi_dsi_dcs_write_seq(dsi, 0x55, 0x82, 0x02);
	mipi_dsi_dcs_write_seq(dsi, 0x56, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0x58, 0x21);
	mipi_dsi_dcs_write_seq(dsi, 0x59, 0x30);
	mipi_dsi_dcs_write_seq(dsi, 0x5A, 0x60);
	mipi_dsi_dcs_write_seq(dsi, 0x5B, 0x50);
	mipi_dsi_dcs_write_seq(dsi, 0x5E, 0x00, 0x06);
	mipi_dsi_dcs_write_seq(dsi, 0x5F, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x65, 0x82);
	mipi_dsi_dcs_write_seq(dsi, 0x7E, 0x20);
	mipi_dsi_dcs_write_seq(dsi, 0x7F, 0x3C);
	mipi_dsi_dcs_write_seq(dsi, 0x82, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0x97, 0xC0);

	mipi_dsi_dcs_write_seq(dsi, 0xB6, 0x05, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x05, 0x00, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x91, 0x44);
	mipi_dsi_dcs_write_seq(dsi, 0x92, 0xA9);
	mipi_dsi_dcs_write_seq(dsi, 0x93, 0x1A);
	mipi_dsi_dcs_write_seq(dsi, 0x94, 0x96);
	mipi_dsi_dcs_write_seq(dsi, 0xD7, 0x55);
	mipi_dsi_dcs_write_seq(dsi, 0xDA, 0x0A);
	mipi_dsi_dcs_write_seq(dsi, 0xDE, 0x08);
	mipi_dsi_dcs_write_seq(dsi, 0xDB, 0x05);
	mipi_dsi_dcs_write_seq(dsi, 0xDC, 0xA9);
	mipi_dsi_dcs_write_seq(dsi, 0xDD, 0x22);

	mipi_dsi_dcs_write_seq(dsi, 0xDF, 0x05);
	mipi_dsi_dcs_write_seq(dsi, 0xE0, 0xA9);
	mipi_dsi_dcs_write_seq(dsi, 0xE1, 0x05);
	mipi_dsi_dcs_write_seq(dsi, 0xE2, 0xA9);
	mipi_dsi_dcs_write_seq(dsi, 0xE3, 0x05);
	mipi_dsi_dcs_write_seq(dsi, 0xE4, 0xA9);
	mipi_dsi_dcs_write_seq(dsi, 0xE5, 0x05);
	mipi_dsi_dcs_write_seq(dsi, 0xE6, 0xA9);
	mipi_dsi_dcs_write_seq(dsi, 0x5C, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x5D, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x8D, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x8E, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xB5, 0x90);
	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x25);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0x05, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x19, 0x07);
	mipi_dsi_dcs_write_seq(dsi, 0x1F, 0x60);
	mipi_dsi_dcs_write_seq(dsi, 0x20, 0x50);
	mipi_dsi_dcs_write_seq(dsi, 0x26, 0x60);
	mipi_dsi_dcs_write_seq(dsi, 0x27, 0x50);
	mipi_dsi_dcs_write_seq(dsi, 0x33, 0x60);
	mipi_dsi_dcs_write_seq(dsi, 0x34, 0x50);
	mipi_dsi_dcs_write_seq(dsi, 0x3F, 0xE0);
	mipi_dsi_dcs_write_seq(dsi, 0x40, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x44, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x45, 0x40);
	mipi_dsi_dcs_write_seq(dsi, 0x48, 0x60);
	mipi_dsi_dcs_write_seq(dsi, 0x49, 0x50);
	mipi_dsi_dcs_write_seq(dsi, 0x5B, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x5C, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x5D, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x5E, 0xD0);
	mipi_dsi_dcs_write_seq(dsi, 0x61, 0x60);
	mipi_dsi_dcs_write_seq(dsi, 0x62, 0x50);
	mipi_dsi_dcs_write_seq(dsi, 0xF1, 0x10);
	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x2A);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);

	mipi_dsi_dcs_write_seq(dsi, 0x64, 0x16);
	mipi_dsi_dcs_write_seq(dsi, 0x67, 0x16);
	mipi_dsi_dcs_write_seq(dsi, 0x6A, 0x16);

	mipi_dsi_dcs_write_seq(dsi, 0x70, 0x30);

	mipi_dsi_dcs_write_seq(dsi, 0xA2, 0xF3);
	mipi_dsi_dcs_write_seq(dsi, 0xA3, 0xFF);
	mipi_dsi_dcs_write_seq(dsi, 0xA4, 0xFF);
	mipi_dsi_dcs_write_seq(dsi, 0xA5, 0xFF);

	mipi_dsi_dcs_write_seq(dsi, 0xD6, 0x08);

	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x26);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0x00, 0xA1);

	mipi_dsi_dcs_write_seq(dsi, 0x02, 0x31);
	mipi_dsi_dcs_write_seq(dsi, 0x04, 0x28);
	mipi_dsi_dcs_write_seq(dsi, 0x06, 0x30);
	mipi_dsi_dcs_write_seq(dsi, 0x0C, 0x16);
	mipi_dsi_dcs_write_seq(dsi, 0x0D, 0x0D);
	mipi_dsi_dcs_write_seq(dsi, 0x0F, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x11, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x12, 0x50);
	mipi_dsi_dcs_write_seq(dsi, 0x13, 0x56);
	mipi_dsi_dcs_write_seq(dsi, 0x14, 0x57);
	mipi_dsi_dcs_write_seq(dsi, 0x15, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x16, 0x10);
	mipi_dsi_dcs_write_seq(dsi, 0x17, 0xA0);
	mipi_dsi_dcs_write_seq(dsi, 0x18, 0x86);
	mipi_dsi_dcs_write_seq(dsi, 0x19, 0x0D);
	mipi_dsi_dcs_write_seq(dsi, 0x1A, 0x7F);
	mipi_dsi_dcs_write_seq(dsi, 0x1B, 0x0C);
	mipi_dsi_dcs_write_seq(dsi, 0x1C, 0xBF);
	mipi_dsi_dcs_write_seq(dsi, 0x22, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x23, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x2A, 0x0D);
	mipi_dsi_dcs_write_seq(dsi, 0x2B, 0x7F);

	mipi_dsi_dcs_write_seq(dsi, 0x1D, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x1E, 0x65);
	mipi_dsi_dcs_write_seq(dsi, 0x1F, 0x65);
	mipi_dsi_dcs_write_seq(dsi, 0x24, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x25, 0x65);
	mipi_dsi_dcs_write_seq(dsi, 0x2F, 0x05);
	mipi_dsi_dcs_write_seq(dsi, 0x30, 0x65);
	mipi_dsi_dcs_write_seq(dsi, 0x31, 0x05);
	mipi_dsi_dcs_write_seq(dsi, 0x32, 0x7D);
	mipi_dsi_dcs_write_seq(dsi, 0x39, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x3A, 0x65);
	mipi_dsi_dcs_write_seq(dsi, 0x20, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0x33, 0x11);
	mipi_dsi_dcs_write_seq(dsi, 0x34, 0x78);
	mipi_dsi_dcs_write_seq(dsi, 0x35, 0x16);
	mipi_dsi_dcs_write_seq(dsi, 0xC8, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0xC9, 0x9E);
	mipi_dsi_dcs_write_seq(dsi, 0xCA, 0x4E);
	mipi_dsi_dcs_write_seq(dsi, 0xCB, 0x00);

	mipi_dsi_dcs_write_seq(dsi, 0xA9, 0x49);
	mipi_dsi_dcs_write_seq(dsi, 0xAA, 0x4B);
	mipi_dsi_dcs_write_seq(dsi, 0xAB, 0x48);
	mipi_dsi_dcs_write_seq(dsi, 0xAC, 0x43);
	mipi_dsi_dcs_write_seq(dsi, 0xAD, 0x40);
	mipi_dsi_dcs_write_seq(dsi, 0xAE, 0x50);
	mipi_dsi_dcs_write_seq(dsi, 0xAF, 0x44);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x54);
	mipi_dsi_dcs_write_seq(dsi, 0xB1, 0x4E);
	mipi_dsi_dcs_write_seq(dsi, 0xB2, 0x4D);
	mipi_dsi_dcs_write_seq(dsi, 0xB3, 0x4C);
	mipi_dsi_dcs_write_seq(dsi, 0xB4, 0x41);
	mipi_dsi_dcs_write_seq(dsi, 0xB5, 0x47);
	mipi_dsi_dcs_write_seq(dsi, 0xB6, 0x53);
	mipi_dsi_dcs_write_seq(dsi, 0xB7, 0x3E);
	mipi_dsi_dcs_write_seq(dsi, 0xB8, 0x51);
	mipi_dsi_dcs_write_seq(dsi, 0xB9, 0x3C);
	mipi_dsi_dcs_write_seq(dsi, 0xBA, 0x3B);
	mipi_dsi_dcs_write_seq(dsi, 0xBB, 0x46);
	mipi_dsi_dcs_write_seq(dsi, 0xBC, 0x45);
	mipi_dsi_dcs_write_seq(dsi, 0xBD, 0x55);
	mipi_dsi_dcs_write_seq(dsi, 0xBE, 0x3D);
	mipi_dsi_dcs_write_seq(dsi, 0xBF, 0x3F);
	mipi_dsi_dcs_write_seq(dsi, 0xC0, 0x52);
	mipi_dsi_dcs_write_seq(dsi, 0xC1, 0x4A);
	mipi_dsi_dcs_write_seq(dsi, 0xC2, 0x39);
	mipi_dsi_dcs_write_seq(dsi, 0xC3, 0x4F);
	mipi_dsi_dcs_write_seq(dsi, 0xC4, 0x3A);
	mipi_dsi_dcs_write_seq(dsi, 0xC5, 0x42);
	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x27);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);

	mipi_dsi_dcs_write_seq(dsi, 0x56, 0x06);
	mipi_dsi_dcs_write_seq(dsi, 0x58, 0x80);
	mipi_dsi_dcs_write_seq(dsi, 0x59, 0x75);
	mipi_dsi_dcs_write_seq(dsi, 0x5A, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x5B, 0x02);
	mipi_dsi_dcs_write_seq(dsi, 0x5C, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x5D, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x5E, 0x20);
	mipi_dsi_dcs_write_seq(dsi, 0x5F, 0x10);
	mipi_dsi_dcs_write_seq(dsi, 0x60, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x61, 0x2E);
	mipi_dsi_dcs_write_seq(dsi, 0x62, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x63, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0x64, 0x43);
	mipi_dsi_dcs_write_seq(dsi, 0x65, 0x2D);
	mipi_dsi_dcs_write_seq(dsi, 0x66, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x67, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0x68, 0x44);

	mipi_dsi_dcs_write_seq(dsi, 0x00, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x78, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xC3, 0x00);

	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x2A);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);

	mipi_dsi_dcs_write_seq(dsi, 0x22, 0x2F);
	mipi_dsi_dcs_write_seq(dsi, 0x23, 0x08);

	mipi_dsi_dcs_write_seq(dsi, 0x24, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x25, 0x65);
	mipi_dsi_dcs_write_seq(dsi, 0x26, 0xF8);
	mipi_dsi_dcs_write_seq(dsi, 0x27, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x28, 0x1A);
	mipi_dsi_dcs_write_seq(dsi, 0x29, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x2A, 0x1A);
	mipi_dsi_dcs_write_seq(dsi, 0x2B, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x2D, 0x1A);

	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x23);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);

	mipi_dsi_dcs_write_seq(dsi, 0x00, 0x80);
	mipi_dsi_dcs_write_seq(dsi, 0x07, 0x00);

	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0xE0);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0x14, 0x60);
	mipi_dsi_dcs_write_seq(dsi, 0x16, 0xC0);

	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0xF0);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0x3A, 0x08);

	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x10);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xB9, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x20);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0x18, 0x40);

	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x10);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xB9, 0x02);
	mipi_dsi_dcs_write_seq(dsi, 0x35, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x51, 0x00, 0xFF);
	mipi_dsi_dcs_write_seq(dsi, 0x53, 0x24);
	mipi_dsi_dcs_write_seq(dsi, 0x55, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xBB, 0x13);
	mipi_dsi_dcs_write_seq(dsi, 0x3B, 0x03, 0x96, 0x1A, 0x04, 0x04);
	msleep(100);
	ret = mipi_dsi_dcs_exit_sleep_mode(dsi);
	if (ret)
		return ret;
	msleep(200);
	ret = mipi_dsi_dcs_set_display_on(dsi);
	if (ret)
		return ret;
	msleep(100);

	return 0;
}

static int inx_hj110iz_init(struct mipi_dsi_device *dsi)
{
	int ret;

	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x20);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0x05, 0xD1);
	mipi_dsi_dcs_write_seq(dsi, 0x0D, 0x63);
	mipi_dsi_dcs_write_seq(dsi, 0x07, 0x8C);
	mipi_dsi_dcs_write_seq(dsi, 0x08, 0x4B);
	mipi_dsi_dcs_write_seq(dsi, 0x0E, 0x91);
	mipi_dsi_dcs_write_seq(dsi, 0x0F, 0x69);
	mipi_dsi_dcs_write_seq(dsi, 0x95, 0xF5);
	mipi_dsi_dcs_write_seq(dsi, 0x96, 0xF5);
	mipi_dsi_dcs_write_seq(dsi, 0x9D, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x9E, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x69, 0x98);
	mipi_dsi_dcs_write_seq(dsi, 0x75, 0xA2);
	mipi_dsi_dcs_write_seq(dsi, 0x77, 0xB3);
	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x24);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0x91, 0x44);
	mipi_dsi_dcs_write_seq(dsi, 0x92, 0x7A);
	mipi_dsi_dcs_write_seq(dsi, 0x93, 0x1A);
	mipi_dsi_dcs_write_seq(dsi, 0x94, 0x40);
	mipi_dsi_dcs_write_seq(dsi, 0x9A, 0x08);
	mipi_dsi_dcs_write_seq(dsi, 0x60, 0x96);
	mipi_dsi_dcs_write_seq(dsi, 0x61, 0xD0);
	mipi_dsi_dcs_write_seq(dsi, 0x63, 0x70);
	mipi_dsi_dcs_write_seq(dsi, 0xC2, 0xCF);
	mipi_dsi_dcs_write_seq(dsi, 0x9B, 0x0F);
	mipi_dsi_dcs_write_seq(dsi, 0x9A, 0x08);
	mipi_dsi_dcs_write_seq(dsi, 0x00, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0x01, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0x02, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0x03, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0x04, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0x05, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0x06, 0x22);
	mipi_dsi_dcs_write_seq(dsi, 0x07, 0x06);
	mipi_dsi_dcs_write_seq(dsi, 0x08, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x09, 0x1D);
	mipi_dsi_dcs_write_seq(dsi, 0x0A, 0x1C);
	mipi_dsi_dcs_write_seq(dsi, 0x0B, 0x13);
	mipi_dsi_dcs_write_seq(dsi, 0x0C, 0x12);
	mipi_dsi_dcs_write_seq(dsi, 0x0D, 0x11);
	mipi_dsi_dcs_write_seq(dsi, 0x0E, 0x10);
	mipi_dsi_dcs_write_seq(dsi, 0x0F, 0x0F);
	mipi_dsi_dcs_write_seq(dsi, 0x10, 0x0E);
	mipi_dsi_dcs_write_seq(dsi, 0x11, 0x0D);
	mipi_dsi_dcs_write_seq(dsi, 0x12, 0x0C);
	mipi_dsi_dcs_write_seq(dsi, 0x13, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0x14, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0x15, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0x16, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0x17, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0x18, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0x19, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0x1A, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0x1B, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0x1C, 0x22);
	mipi_dsi_dcs_write_seq(dsi, 0x1D, 0x06);
	mipi_dsi_dcs_write_seq(dsi, 0x1E, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x1F, 0x1D);
	mipi_dsi_dcs_write_seq(dsi, 0x20, 0x1C);
	mipi_dsi_dcs_write_seq(dsi, 0x21, 0x13);
	mipi_dsi_dcs_write_seq(dsi, 0x22, 0x12);
	mipi_dsi_dcs_write_seq(dsi, 0x23, 0x11);
	mipi_dsi_dcs_write_seq(dsi, 0x24, 0x10);
	mipi_dsi_dcs_write_seq(dsi, 0x25, 0x0F);
	mipi_dsi_dcs_write_seq(dsi, 0x26, 0x0E);
	mipi_dsi_dcs_write_seq(dsi, 0x27, 0x0D);
	mipi_dsi_dcs_write_seq(dsi, 0x28, 0x0C);
	mipi_dsi_dcs_write_seq(dsi, 0x29, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0x2A, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0x2B, 0x03);

	mipi_dsi_dcs_write_seq(dsi, 0x2F, 0x05);
	mipi_dsi_dcs_write_seq(dsi, 0x30, 0x32);
	mipi_dsi_dcs_write_seq(dsi, 0x31, 0x43);
	mipi_dsi_dcs_write_seq(dsi, 0x33, 0x05);
	mipi_dsi_dcs_write_seq(dsi, 0x34, 0x32);
	mipi_dsi_dcs_write_seq(dsi, 0x35, 0x43);
	mipi_dsi_dcs_write_seq(dsi, 0x37, 0x44);
	mipi_dsi_dcs_write_seq(dsi, 0x38, 0x40);
	mipi_dsi_dcs_write_seq(dsi, 0x39, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x3A, 0x18);
	mipi_dsi_dcs_write_seq(dsi, 0x3B, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x3D, 0x93);
	mipi_dsi_dcs_write_seq(dsi, 0xAB, 0x44);
	mipi_dsi_dcs_write_seq(dsi, 0xAC, 0x40);

	mipi_dsi_dcs_write_seq(dsi, 0x4D, 0x21);
	mipi_dsi_dcs_write_seq(dsi, 0x4E, 0x43);
	mipi_dsi_dcs_write_seq(dsi, 0x4F, 0x65);
	mipi_dsi_dcs_write_seq(dsi, 0x50, 0x87);
	mipi_dsi_dcs_write_seq(dsi, 0x51, 0x78);
	mipi_dsi_dcs_write_seq(dsi, 0x52, 0x56);
	mipi_dsi_dcs_write_seq(dsi, 0x53, 0x34);
	mipi_dsi_dcs_write_seq(dsi, 0x54, 0x21);
	mipi_dsi_dcs_write_seq(dsi, 0x55, 0x83);
	mipi_dsi_dcs_write_seq(dsi, 0x56, 0x08);
	mipi_dsi_dcs_write_seq(dsi, 0x58, 0x21);
	mipi_dsi_dcs_write_seq(dsi, 0x59, 0x40);
	mipi_dsi_dcs_write_seq(dsi, 0x5A, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x5B, 0x2C);
	mipi_dsi_dcs_write_seq(dsi, 0x5E, 0x00, 0x10);
	mipi_dsi_dcs_write_seq(dsi, 0x5F, 0x00);

	mipi_dsi_dcs_write_seq(dsi, 0x7A, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x7B, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x7C, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x7D, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x7E, 0x20);
	mipi_dsi_dcs_write_seq(dsi, 0x7F, 0x3C);
	mipi_dsi_dcs_write_seq(dsi, 0x80, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x81, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x82, 0x08);
	mipi_dsi_dcs_write_seq(dsi, 0x97, 0x02);
	mipi_dsi_dcs_write_seq(dsi, 0xC5, 0x10);
	mipi_dsi_dcs_write_seq(dsi, 0xDA, 0x05);
	mipi_dsi_dcs_write_seq(dsi, 0xDB, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xDC, 0x7A);
	mipi_dsi_dcs_write_seq(dsi, 0xDD, 0x55);
	mipi_dsi_dcs_write_seq(dsi, 0xDE, 0x27);
	mipi_dsi_dcs_write_seq(dsi, 0xDF, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xE0, 0x7A);
	mipi_dsi_dcs_write_seq(dsi, 0xE1, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xE2, 0x7A);
	mipi_dsi_dcs_write_seq(dsi, 0xE3, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xE4, 0x7A);
	mipi_dsi_dcs_write_seq(dsi, 0xE5, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xE6, 0x7A);
	mipi_dsi_dcs_write_seq(dsi, 0xE7, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xE8, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xE9, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xEA, 0x7A);
	mipi_dsi_dcs_write_seq(dsi, 0xEB, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xEE, 0x7A);
	mipi_dsi_dcs_write_seq(dsi, 0xEF, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xF0, 0x7A);

	mipi_dsi_dcs_write_seq(dsi, 0xB6, 0x05, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x05, 0x00, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x25);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);

	mipi_dsi_dcs_write_seq(dsi, 0x05, 0x00);

	mipi_dsi_dcs_write_seq(dsi, 0x13, 0x02);
	mipi_dsi_dcs_write_seq(dsi, 0x14, 0xDF);
	mipi_dsi_dcs_write_seq(dsi, 0xF1, 0x10);
	mipi_dsi_dcs_write_seq(dsi, 0x1E, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x1F, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x20, 0x2C);
	mipi_dsi_dcs_write_seq(dsi, 0x25, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x26, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x27, 0x2C);
	mipi_dsi_dcs_write_seq(dsi, 0x3F, 0x80);
	mipi_dsi_dcs_write_seq(dsi, 0x40, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x43, 0x00);

	mipi_dsi_dcs_write_seq(dsi, 0x44, 0x18);
	mipi_dsi_dcs_write_seq(dsi, 0x45, 0x00);

	mipi_dsi_dcs_write_seq(dsi, 0x48, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x49, 0x2C);
	mipi_dsi_dcs_write_seq(dsi, 0x5B, 0x80);
	mipi_dsi_dcs_write_seq(dsi, 0x5C, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x5D, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x5E, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x61, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x62, 0x2C);
	mipi_dsi_dcs_write_seq(dsi, 0x68, 0x10);
	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x26);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);

	mipi_dsi_dcs_write_seq(dsi, 0x00, 0xA1);
	mipi_dsi_dcs_write_seq(dsi, 0x02, 0x31);
	mipi_dsi_dcs_write_seq(dsi, 0x0A, 0xF2);
	mipi_dsi_dcs_write_seq(dsi, 0x04, 0x28);
	mipi_dsi_dcs_write_seq(dsi, 0x06, 0x30);
	mipi_dsi_dcs_write_seq(dsi, 0x0C, 0x16);
	mipi_dsi_dcs_write_seq(dsi, 0x0D, 0x0D);
	mipi_dsi_dcs_write_seq(dsi, 0x0F, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x11, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x12, 0x50);
	mipi_dsi_dcs_write_seq(dsi, 0x13, 0x56);
	mipi_dsi_dcs_write_seq(dsi, 0x14, 0x57);
	mipi_dsi_dcs_write_seq(dsi, 0x15, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x16, 0x10);
	mipi_dsi_dcs_write_seq(dsi, 0x17, 0xA0);
	mipi_dsi_dcs_write_seq(dsi, 0x18, 0x86);
	mipi_dsi_dcs_write_seq(dsi, 0x22, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x23, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x19, 0x0D);
	mipi_dsi_dcs_write_seq(dsi, 0x1A, 0x7F);
	mipi_dsi_dcs_write_seq(dsi, 0x1B, 0x0C);
	mipi_dsi_dcs_write_seq(dsi, 0x1C, 0xBF);
	mipi_dsi_dcs_write_seq(dsi, 0x2A, 0x0D);
	mipi_dsi_dcs_write_seq(dsi, 0x2B, 0x7F);
	mipi_dsi_dcs_write_seq(dsi, 0x20, 0x00);

	mipi_dsi_dcs_write_seq(dsi, 0x1D, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x1E, 0x78);
	mipi_dsi_dcs_write_seq(dsi, 0x1F, 0x78);

	mipi_dsi_dcs_write_seq(dsi, 0x2F, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0x30, 0x78);
	mipi_dsi_dcs_write_seq(dsi, 0x33, 0x78);
	mipi_dsi_dcs_write_seq(dsi, 0x34, 0x66);
	mipi_dsi_dcs_write_seq(dsi, 0x35, 0x11);

	mipi_dsi_dcs_write_seq(dsi, 0x39, 0x10);
	mipi_dsi_dcs_write_seq(dsi, 0x3A, 0x78);
	mipi_dsi_dcs_write_seq(dsi, 0x3B, 0x06);

	mipi_dsi_dcs_write_seq(dsi, 0xC8, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0xC9, 0x84);
	mipi_dsi_dcs_write_seq(dsi, 0xCA, 0x4E);
	mipi_dsi_dcs_write_seq(dsi, 0xCB, 0x00);

	mipi_dsi_dcs_write_seq(dsi, 0xA9, 0x50);
	mipi_dsi_dcs_write_seq(dsi, 0xAA, 0x4F);
	mipi_dsi_dcs_write_seq(dsi, 0xAB, 0x4D);
	mipi_dsi_dcs_write_seq(dsi, 0xAC, 0x4A);
	mipi_dsi_dcs_write_seq(dsi, 0xAD, 0x48);
	mipi_dsi_dcs_write_seq(dsi, 0xAE, 0x46);
	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x27);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xC0, 0x18);
	mipi_dsi_dcs_write_seq(dsi, 0xC1, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xC2, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x56, 0x06);
	mipi_dsi_dcs_write_seq(dsi, 0x58, 0x80);
	mipi_dsi_dcs_write_seq(dsi, 0x59, 0x75);
	mipi_dsi_dcs_write_seq(dsi, 0x5A, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x5B, 0x02);
	mipi_dsi_dcs_write_seq(dsi, 0x5C, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x5D, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x5E, 0x20);
	mipi_dsi_dcs_write_seq(dsi, 0x5F, 0x10);
	mipi_dsi_dcs_write_seq(dsi, 0x60, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x61, 0x2E);
	mipi_dsi_dcs_write_seq(dsi, 0x62, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x63, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0x64, 0x43);
	mipi_dsi_dcs_write_seq(dsi, 0x65, 0x2D);
	mipi_dsi_dcs_write_seq(dsi, 0x66, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x67, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0x68, 0x43);
	mipi_dsi_dcs_write_seq(dsi, 0x98, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xB4, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0x9B, 0xBD);
	mipi_dsi_dcs_write_seq(dsi, 0xA0, 0x90);
	mipi_dsi_dcs_write_seq(dsi, 0xAB, 0x1B);
	mipi_dsi_dcs_write_seq(dsi, 0xBC, 0x0C);
	mipi_dsi_dcs_write_seq(dsi, 0xBD, 0x28);

	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x2A);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);

	mipi_dsi_dcs_write_seq(dsi, 0x22, 0x2F);
	mipi_dsi_dcs_write_seq(dsi, 0x23, 0x08);

	mipi_dsi_dcs_write_seq(dsi, 0x24, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x25, 0x65);
	mipi_dsi_dcs_write_seq(dsi, 0x26, 0xF8);
	mipi_dsi_dcs_write_seq(dsi, 0x27, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x28, 0x1A);
	mipi_dsi_dcs_write_seq(dsi, 0x29, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x2A, 0x1A);
	mipi_dsi_dcs_write_seq(dsi, 0x2B, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x2D, 0x1A);

	mipi_dsi_dcs_write_seq(dsi, 0x64, 0x96);
	mipi_dsi_dcs_write_seq(dsi, 0x65, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x66, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x6A, 0x96);
	mipi_dsi_dcs_write_seq(dsi, 0x6B, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x6C, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x70, 0x92);
	mipi_dsi_dcs_write_seq(dsi, 0x71, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x72, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xA2, 0x33);
	mipi_dsi_dcs_write_seq(dsi, 0xA3, 0x30);
	mipi_dsi_dcs_write_seq(dsi, 0xA4, 0xC0);
	mipi_dsi_dcs_write_seq(dsi, 0xE8, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x97, 0x3C);
	mipi_dsi_dcs_write_seq(dsi, 0x98, 0x02);
	mipi_dsi_dcs_write_seq(dsi, 0x99, 0x95);
	mipi_dsi_dcs_write_seq(dsi, 0x9A, 0x06);
	mipi_dsi_dcs_write_seq(dsi, 0x9B, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x9C, 0x0B);
	mipi_dsi_dcs_write_seq(dsi, 0x9D, 0x0A);
	mipi_dsi_dcs_write_seq(dsi, 0x9E, 0x90);
	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0xF0);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0x3A, 0x08);
	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0xD0);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0x00, 0x33);
	mipi_dsi_dcs_write_seq(dsi, 0x08, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0x09, 0xBF);
	mipi_dsi_dcs_write_seq(dsi, 0x2F, 0x33);
	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x23);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0x00, 0x80);
	mipi_dsi_dcs_write_seq(dsi, 0x07, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x20);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0x30, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x24);
	mipi_dsi_dcs_write_seq(dsi, 0x5C, 0x88);
	mipi_dsi_dcs_write_seq(dsi, 0x5D, 0x08);
	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x10);
	mipi_dsi_dcs_write_seq(dsi, 0xB9, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x20);
	mipi_dsi_dcs_write_seq(dsi, 0x18, 0x40);
	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x10);
	mipi_dsi_dcs_write_seq(dsi, 0xB9, 0x02);
	mipi_dsi_dcs_write_seq(dsi, 0xFF, 0x10);
	mipi_dsi_dcs_write_seq(dsi, 0xFB, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xBB, 0x13);
	mipi_dsi_dcs_write_seq(dsi, 0x3B, 0x03, 0x96, 0x1A, 0x04, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0x35, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0x51, 0x0F, 0xFF);
	mipi_dsi_dcs_write_seq(dsi, 0x53, 0x24);
	msleep(100);
	ret = mipi_dsi_dcs_exit_sleep_mode(dsi);
	if (ret)
		return ret;
	msleep(200);
	ret = mipi_dsi_dcs_set_display_on(dsi);
	if (ret)
		return ret;
	msleep(100);

	return 0;
}

static int boe_init(struct mipi_dsi_device *dsi)
{
	msleep(24);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x05);
	mipi_dsi_dcs_write_seq(dsi, 0xB1, 0xE5);
	mipi_dsi_dcs_write_seq(dsi, 0xB3, 0x52);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xB3, 0x88);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0xB8, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xB6, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0xBA, 0x8B);
	mipi_dsi_dcs_write_seq(dsi, 0xBF, 0x1A);
	mipi_dsi_dcs_write_seq(dsi, 0xC0, 0x0F);
	mipi_dsi_dcs_write_seq(dsi, 0xC2, 0x0C);
	mipi_dsi_dcs_write_seq(dsi, 0xC3, 0x02);
	mipi_dsi_dcs_write_seq(dsi, 0xC4, 0x0C);
	mipi_dsi_dcs_write_seq(dsi, 0xC5, 0x02);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xE0, 0x26);
	mipi_dsi_dcs_write_seq(dsi, 0xE1, 0x26);
	mipi_dsi_dcs_write_seq(dsi, 0xDC, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xDD, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xCC, 0x26);
	mipi_dsi_dcs_write_seq(dsi, 0xCD, 0x26);
	mipi_dsi_dcs_write_seq(dsi, 0xC8, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xC9, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xD2, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0xD3, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0xE6, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0xE7, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0xC4, 0x09);
	mipi_dsi_dcs_write_seq(dsi, 0xC5, 0x09);
	mipi_dsi_dcs_write_seq(dsi, 0xD8, 0x0A);
	mipi_dsi_dcs_write_seq(dsi, 0xD9, 0x0A);
	mipi_dsi_dcs_write_seq(dsi, 0xC2, 0x0B);
	mipi_dsi_dcs_write_seq(dsi, 0xC3, 0x0B);
	mipi_dsi_dcs_write_seq(dsi, 0xD6, 0x0C);
	mipi_dsi_dcs_write_seq(dsi, 0xD7, 0x0C);
	mipi_dsi_dcs_write_seq(dsi, 0xC0, 0x05);
	mipi_dsi_dcs_write_seq(dsi, 0xC1, 0x05);
	mipi_dsi_dcs_write_seq(dsi, 0xD4, 0x06);
	mipi_dsi_dcs_write_seq(dsi, 0xD5, 0x06);
	mipi_dsi_dcs_write_seq(dsi, 0xCA, 0x07);
	mipi_dsi_dcs_write_seq(dsi, 0xCB, 0x07);
	mipi_dsi_dcs_write_seq(dsi, 0xDE, 0x08);
	mipi_dsi_dcs_write_seq(dsi, 0xDF, 0x08);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x02);
	mipi_dsi_dcs_write_seq(dsi, 0xC0, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xC1, 0x0D);
	mipi_dsi_dcs_write_seq(dsi, 0xC2, 0x17);
	mipi_dsi_dcs_write_seq(dsi, 0xC3, 0x26);
	mipi_dsi_dcs_write_seq(dsi, 0xC4, 0x31);
	mipi_dsi_dcs_write_seq(dsi, 0xC5, 0x1C);
	mipi_dsi_dcs_write_seq(dsi, 0xC6, 0x2C);
	mipi_dsi_dcs_write_seq(dsi, 0xC7, 0x33);
	mipi_dsi_dcs_write_seq(dsi, 0xC8, 0x31);
	mipi_dsi_dcs_write_seq(dsi, 0xC9, 0x37);
	mipi_dsi_dcs_write_seq(dsi, 0xCA, 0x37);
	mipi_dsi_dcs_write_seq(dsi, 0xCB, 0x37);
	mipi_dsi_dcs_write_seq(dsi, 0xCC, 0x39);
	mipi_dsi_dcs_write_seq(dsi, 0xCD, 0x2E);
	mipi_dsi_dcs_write_seq(dsi, 0xCE, 0x2F);
	mipi_dsi_dcs_write_seq(dsi, 0xCF, 0x2F);
	mipi_dsi_dcs_write_seq(dsi, 0xD0, 0x07);
	mipi_dsi_dcs_write_seq(dsi, 0xD2, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xD3, 0x0D);
	mipi_dsi_dcs_write_seq(dsi, 0xD4, 0x17);
	mipi_dsi_dcs_write_seq(dsi, 0xD5, 0x26);
	mipi_dsi_dcs_write_seq(dsi, 0xD6, 0x31);
	mipi_dsi_dcs_write_seq(dsi, 0xD7, 0x3F);
	mipi_dsi_dcs_write_seq(dsi, 0xD8, 0x3F);
	mipi_dsi_dcs_write_seq(dsi, 0xD9, 0x3F);
	mipi_dsi_dcs_write_seq(dsi, 0xDA, 0x3F);
	mipi_dsi_dcs_write_seq(dsi, 0xDB, 0x37);
	mipi_dsi_dcs_write_seq(dsi, 0xDC, 0x37);
	mipi_dsi_dcs_write_seq(dsi, 0xDD, 0x37);
	mipi_dsi_dcs_write_seq(dsi, 0xDE, 0x39);
	mipi_dsi_dcs_write_seq(dsi, 0xDF, 0x2E);
	mipi_dsi_dcs_write_seq(dsi, 0xE0, 0x2F);
	mipi_dsi_dcs_write_seq(dsi, 0xE1, 0x2F);
	mipi_dsi_dcs_write_seq(dsi, 0xE2, 0x07);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0xC8, 0x0B);
	mipi_dsi_dcs_write_seq(dsi, 0xC9, 0x07);
	mipi_dsi_dcs_write_seq(dsi, 0xC3, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xE7, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xC5, 0x2A);
	mipi_dsi_dcs_write_seq(dsi, 0xDE, 0x2A);
	mipi_dsi_dcs_write_seq(dsi, 0xCA, 0x43);
	mipi_dsi_dcs_write_seq(dsi, 0xC9, 0x07);
	mipi_dsi_dcs_write_seq(dsi, 0xE4, 0xC0);
	mipi_dsi_dcs_write_seq(dsi, 0xE5, 0x0D);
	mipi_dsi_dcs_write_seq(dsi, 0xCB, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x06);
	mipi_dsi_dcs_write_seq(dsi, 0xB8, 0xA5);
	mipi_dsi_dcs_write_seq(dsi, 0xC0, 0xA5);
	mipi_dsi_dcs_write_seq(dsi, 0xC7, 0x0F);
	mipi_dsi_dcs_write_seq(dsi, 0xD5, 0x32);
	mipi_dsi_dcs_write_seq(dsi, 0xB8, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xC0, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xBC, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x07);
	mipi_dsi_dcs_write_seq(dsi, 0xB1, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xB2, 0x02);
	mipi_dsi_dcs_write_seq(dsi, 0xB3, 0x0F);
	mipi_dsi_dcs_write_seq(dsi, 0xB4, 0x25);
	mipi_dsi_dcs_write_seq(dsi, 0xB5, 0x39);
	mipi_dsi_dcs_write_seq(dsi, 0xB6, 0x4E);
	mipi_dsi_dcs_write_seq(dsi, 0xB7, 0x72);
	mipi_dsi_dcs_write_seq(dsi, 0xB8, 0x97);
	mipi_dsi_dcs_write_seq(dsi, 0xB9, 0xDC);
	mipi_dsi_dcs_write_seq(dsi, 0xBA, 0x22);
	mipi_dsi_dcs_write_seq(dsi, 0xBB, 0xA4);
	mipi_dsi_dcs_write_seq(dsi, 0xBC, 0x2B);
	mipi_dsi_dcs_write_seq(dsi, 0xBD, 0x2F);
	mipi_dsi_dcs_write_seq(dsi, 0xBE, 0xA9);
	mipi_dsi_dcs_write_seq(dsi, 0xBF, 0x25);
	mipi_dsi_dcs_write_seq(dsi, 0xC0, 0x61);
	mipi_dsi_dcs_write_seq(dsi, 0xC1, 0x97);
	mipi_dsi_dcs_write_seq(dsi, 0xC2, 0xB2);
	mipi_dsi_dcs_write_seq(dsi, 0xC3, 0xCD);
	mipi_dsi_dcs_write_seq(dsi, 0xC4, 0xD9);
	mipi_dsi_dcs_write_seq(dsi, 0xC5, 0xE7);
	mipi_dsi_dcs_write_seq(dsi, 0xC6, 0xF4);
	mipi_dsi_dcs_write_seq(dsi, 0xC7, 0xFA);
	mipi_dsi_dcs_write_seq(dsi, 0xC8, 0xFC);
	mipi_dsi_dcs_write_seq(dsi, 0xC9, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xCA, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xCB, 0x16);
	mipi_dsi_dcs_write_seq(dsi, 0xCC, 0xAF);
	mipi_dsi_dcs_write_seq(dsi, 0xCD, 0xFF);
	mipi_dsi_dcs_write_seq(dsi, 0xCE, 0xFF);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x08);
	mipi_dsi_dcs_write_seq(dsi, 0xB1, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0xB2, 0x05);
	mipi_dsi_dcs_write_seq(dsi, 0xB3, 0x11);
	mipi_dsi_dcs_write_seq(dsi, 0xB4, 0x24);
	mipi_dsi_dcs_write_seq(dsi, 0xB5, 0x39);
	mipi_dsi_dcs_write_seq(dsi, 0xB6, 0x4F);
	mipi_dsi_dcs_write_seq(dsi, 0xB7, 0x72);
	mipi_dsi_dcs_write_seq(dsi, 0xB8, 0x98);
	mipi_dsi_dcs_write_seq(dsi, 0xB9, 0xDC);
	mipi_dsi_dcs_write_seq(dsi, 0xBA, 0x23);
	mipi_dsi_dcs_write_seq(dsi, 0xBB, 0xA6);
	mipi_dsi_dcs_write_seq(dsi, 0xBC, 0x2C);
	mipi_dsi_dcs_write_seq(dsi, 0xBD, 0x30);
	mipi_dsi_dcs_write_seq(dsi, 0xBE, 0xAA);
	mipi_dsi_dcs_write_seq(dsi, 0xBF, 0x26);
	mipi_dsi_dcs_write_seq(dsi, 0xC0, 0x62);
	mipi_dsi_dcs_write_seq(dsi, 0xC1, 0x9B);
	mipi_dsi_dcs_write_seq(dsi, 0xC2, 0xB5);
	mipi_dsi_dcs_write_seq(dsi, 0xC3, 0xCF);
	mipi_dsi_dcs_write_seq(dsi, 0xC4, 0xDB);
	mipi_dsi_dcs_write_seq(dsi, 0xC5, 0xE8);
	mipi_dsi_dcs_write_seq(dsi, 0xC6, 0xF5);
	mipi_dsi_dcs_write_seq(dsi, 0xC7, 0xFA);
	mipi_dsi_dcs_write_seq(dsi, 0xC8, 0xFC);
	mipi_dsi_dcs_write_seq(dsi, 0xC9, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xCA, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xCB, 0x16);
	mipi_dsi_dcs_write_seq(dsi, 0xCC, 0xAF);
	mipi_dsi_dcs_write_seq(dsi, 0xCD, 0xFF);
	mipi_dsi_dcs_write_seq(dsi, 0xCE, 0xFF);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x09);
	mipi_dsi_dcs_write_seq(dsi, 0xB1, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0xB2, 0x02);
	mipi_dsi_dcs_write_seq(dsi, 0xB3, 0x16);
	mipi_dsi_dcs_write_seq(dsi, 0xB4, 0x24);
	mipi_dsi_dcs_write_seq(dsi, 0xB5, 0x3B);
	mipi_dsi_dcs_write_seq(dsi, 0xB6, 0x4F);
	mipi_dsi_dcs_write_seq(dsi, 0xB7, 0x73);
	mipi_dsi_dcs_write_seq(dsi, 0xB8, 0x99);
	mipi_dsi_dcs_write_seq(dsi, 0xB9, 0xE0);
	mipi_dsi_dcs_write_seq(dsi, 0xBA, 0x26);
	mipi_dsi_dcs_write_seq(dsi, 0xBB, 0xAD);
	mipi_dsi_dcs_write_seq(dsi, 0xBC, 0x36);
	mipi_dsi_dcs_write_seq(dsi, 0xBD, 0x3A);
	mipi_dsi_dcs_write_seq(dsi, 0xBE, 0xAE);
	mipi_dsi_dcs_write_seq(dsi, 0xBF, 0x2A);
	mipi_dsi_dcs_write_seq(dsi, 0xC0, 0x66);
	mipi_dsi_dcs_write_seq(dsi, 0xC1, 0x9E);
	mipi_dsi_dcs_write_seq(dsi, 0xC2, 0xB8);
	mipi_dsi_dcs_write_seq(dsi, 0xC3, 0xD1);
	mipi_dsi_dcs_write_seq(dsi, 0xC4, 0xDD);
	mipi_dsi_dcs_write_seq(dsi, 0xC5, 0xE9);
	mipi_dsi_dcs_write_seq(dsi, 0xC6, 0xF6);
	mipi_dsi_dcs_write_seq(dsi, 0xC7, 0xFA);
	mipi_dsi_dcs_write_seq(dsi, 0xC8, 0xFC);
	mipi_dsi_dcs_write_seq(dsi, 0xC9, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xCA, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xCB, 0x16);
	mipi_dsi_dcs_write_seq(dsi, 0xCC, 0xAF);
	mipi_dsi_dcs_write_seq(dsi, 0xCD, 0xFF);
	mipi_dsi_dcs_write_seq(dsi, 0xCE, 0xFF);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x0A);
	mipi_dsi_dcs_write_seq(dsi, 0xB1, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xB2, 0x02);
	mipi_dsi_dcs_write_seq(dsi, 0xB3, 0x0F);
	mipi_dsi_dcs_write_seq(dsi, 0xB4, 0x25);
	mipi_dsi_dcs_write_seq(dsi, 0xB5, 0x39);
	mipi_dsi_dcs_write_seq(dsi, 0xB6, 0x4E);
	mipi_dsi_dcs_write_seq(dsi, 0xB7, 0x72);
	mipi_dsi_dcs_write_seq(dsi, 0xB8, 0x97);
	mipi_dsi_dcs_write_seq(dsi, 0xB9, 0xDC);
	mipi_dsi_dcs_write_seq(dsi, 0xBA, 0x22);
	mipi_dsi_dcs_write_seq(dsi, 0xBB, 0xA4);
	mipi_dsi_dcs_write_seq(dsi, 0xBC, 0x2B);
	mipi_dsi_dcs_write_seq(dsi, 0xBD, 0x2F);
	mipi_dsi_dcs_write_seq(dsi, 0xBE, 0xA9);
	mipi_dsi_dcs_write_seq(dsi, 0xBF, 0x25);
	mipi_dsi_dcs_write_seq(dsi, 0xC0, 0x61);
	mipi_dsi_dcs_write_seq(dsi, 0xC1, 0x97);
	mipi_dsi_dcs_write_seq(dsi, 0xC2, 0xB2);
	mipi_dsi_dcs_write_seq(dsi, 0xC3, 0xCD);
	mipi_dsi_dcs_write_seq(dsi, 0xC4, 0xD9);
	mipi_dsi_dcs_write_seq(dsi, 0xC5, 0xE7);
	mipi_dsi_dcs_write_seq(dsi, 0xC6, 0xF4);
	mipi_dsi_dcs_write_seq(dsi, 0xC7, 0xFA);
	mipi_dsi_dcs_write_seq(dsi, 0xC8, 0xFC);
	mipi_dsi_dcs_write_seq(dsi, 0xC9, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xCA, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xCB, 0x16);
	mipi_dsi_dcs_write_seq(dsi, 0xCC, 0xAF);
	mipi_dsi_dcs_write_seq(dsi, 0xCD, 0xFF);
	mipi_dsi_dcs_write_seq(dsi, 0xCE, 0xFF);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x0B);
	mipi_dsi_dcs_write_seq(dsi, 0xB1, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0xB2, 0x05);
	mipi_dsi_dcs_write_seq(dsi, 0xB3, 0x11);
	mipi_dsi_dcs_write_seq(dsi, 0xB4, 0x24);
	mipi_dsi_dcs_write_seq(dsi, 0xB5, 0x39);
	mipi_dsi_dcs_write_seq(dsi, 0xB6, 0x4F);
	mipi_dsi_dcs_write_seq(dsi, 0xB7, 0x72);
	mipi_dsi_dcs_write_seq(dsi, 0xB8, 0x98);
	mipi_dsi_dcs_write_seq(dsi, 0xB9, 0xDC);
	mipi_dsi_dcs_write_seq(dsi, 0xBA, 0x23);
	mipi_dsi_dcs_write_seq(dsi, 0xBB, 0xA6);
	mipi_dsi_dcs_write_seq(dsi, 0xBC, 0x2C);
	mipi_dsi_dcs_write_seq(dsi, 0xBD, 0x30);
	mipi_dsi_dcs_write_seq(dsi, 0xBE, 0xAA);
	mipi_dsi_dcs_write_seq(dsi, 0xBF, 0x26);
	mipi_dsi_dcs_write_seq(dsi, 0xC0, 0x62);
	mipi_dsi_dcs_write_seq(dsi, 0xC1, 0x9B);
	mipi_dsi_dcs_write_seq(dsi, 0xC2, 0xB5);
	mipi_dsi_dcs_write_seq(dsi, 0xC3, 0xCF);
	mipi_dsi_dcs_write_seq(dsi, 0xC4, 0xDB);
	mipi_dsi_dcs_write_seq(dsi, 0xC5, 0xE8);
	mipi_dsi_dcs_write_seq(dsi, 0xC6, 0xF5);
	mipi_dsi_dcs_write_seq(dsi, 0xC7, 0xFA);
	mipi_dsi_dcs_write_seq(dsi, 0xC8, 0xFC);
	mipi_dsi_dcs_write_seq(dsi, 0xC9, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xCA, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xCB, 0x16);
	mipi_dsi_dcs_write_seq(dsi, 0xCC, 0xAF);
	mipi_dsi_dcs_write_seq(dsi, 0xCD, 0xFF);
	mipi_dsi_dcs_write_seq(dsi, 0xCE, 0xFF);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x0C);
	mipi_dsi_dcs_write_seq(dsi, 0xB1, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0xB2, 0x02);
	mipi_dsi_dcs_write_seq(dsi, 0xB3, 0x16);
	mipi_dsi_dcs_write_seq(dsi, 0xB4, 0x24);
	mipi_dsi_dcs_write_seq(dsi, 0xB5, 0x3B);
	mipi_dsi_dcs_write_seq(dsi, 0xB6, 0x4F);
	mipi_dsi_dcs_write_seq(dsi, 0xB7, 0x73);
	mipi_dsi_dcs_write_seq(dsi, 0xB8, 0x99);
	mipi_dsi_dcs_write_seq(dsi, 0xB9, 0xE0);
	mipi_dsi_dcs_write_seq(dsi, 0xBA, 0x26);
	mipi_dsi_dcs_write_seq(dsi, 0xBB, 0xAD);
	mipi_dsi_dcs_write_seq(dsi, 0xBC, 0x36);
	mipi_dsi_dcs_write_seq(dsi, 0xBD, 0x3A);
	mipi_dsi_dcs_write_seq(dsi, 0xBE, 0xAE);
	mipi_dsi_dcs_write_seq(dsi, 0xBF, 0x2A);
	mipi_dsi_dcs_write_seq(dsi, 0xC0, 0x66);
	mipi_dsi_dcs_write_seq(dsi, 0xC1, 0x9E);
	mipi_dsi_dcs_write_seq(dsi, 0xC2, 0xB8);
	mipi_dsi_dcs_write_seq(dsi, 0xC3, 0xD1);
	mipi_dsi_dcs_write_seq(dsi, 0xC4, 0xDD);
	mipi_dsi_dcs_write_seq(dsi, 0xC5, 0xE9);
	mipi_dsi_dcs_write_seq(dsi, 0xC6, 0xF6);
	mipi_dsi_dcs_write_seq(dsi, 0xC7, 0xFA);
	mipi_dsi_dcs_write_seq(dsi, 0xC8, 0xFC);
	mipi_dsi_dcs_write_seq(dsi, 0xC9, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xCA, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xCB, 0x16);
	mipi_dsi_dcs_write_seq(dsi, 0xCC, 0xAF);
	mipi_dsi_dcs_write_seq(dsi, 0xCD, 0xFF);
	mipi_dsi_dcs_write_seq(dsi, 0xCE, 0xFF);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xB3, 0x08);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0xB8, 0x68);
	msleep(150);

	return 0;
}

static int auo_kd101n80_45na_init(struct mipi_dsi_device *dsi)
{
	int ret;

	msleep(24);
	ret = mipi_dsi_dcs_exit_sleep_mode(dsi);
	if (ret)
		return ret;
	msleep(120);
	ret = mipi_dsi_dcs_set_display_on(dsi);
	if (ret)
		return ret;
	msleep(120);

	return 0;
}

static int auo_b101uan08_3_init(struct mipi_dsi_device *dsi)
{
	msleep(24);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xC0, 0x48);
	mipi_dsi_dcs_write_seq(dsi, 0xC1, 0x48);
	mipi_dsi_dcs_write_seq(dsi, 0xC2, 0x47);
	mipi_dsi_dcs_write_seq(dsi, 0xC3, 0x47);
	mipi_dsi_dcs_write_seq(dsi, 0xC4, 0x46);
	mipi_dsi_dcs_write_seq(dsi, 0xC5, 0x46);
	mipi_dsi_dcs_write_seq(dsi, 0xC6, 0x45);
	mipi_dsi_dcs_write_seq(dsi, 0xC7, 0x45);
	mipi_dsi_dcs_write_seq(dsi, 0xC8, 0x64);
	mipi_dsi_dcs_write_seq(dsi, 0xC9, 0x64);
	mipi_dsi_dcs_write_seq(dsi, 0xCA, 0x4F);
	mipi_dsi_dcs_write_seq(dsi, 0xCB, 0x4F);
	mipi_dsi_dcs_write_seq(dsi, 0xCC, 0x40);
	mipi_dsi_dcs_write_seq(dsi, 0xCD, 0x40);
	mipi_dsi_dcs_write_seq(dsi, 0xCE, 0x66);
	mipi_dsi_dcs_write_seq(dsi, 0xCF, 0x66);
	mipi_dsi_dcs_write_seq(dsi, 0xD0, 0x4F);
	mipi_dsi_dcs_write_seq(dsi, 0xD1, 0x4F);
	mipi_dsi_dcs_write_seq(dsi, 0xD2, 0x41);
	mipi_dsi_dcs_write_seq(dsi, 0xD3, 0x41);
	mipi_dsi_dcs_write_seq(dsi, 0xD4, 0x48);
	mipi_dsi_dcs_write_seq(dsi, 0xD5, 0x48);
	mipi_dsi_dcs_write_seq(dsi, 0xD6, 0x47);
	mipi_dsi_dcs_write_seq(dsi, 0xD7, 0x47);
	mipi_dsi_dcs_write_seq(dsi, 0xD8, 0x46);
	mipi_dsi_dcs_write_seq(dsi, 0xD9, 0x46);
	mipi_dsi_dcs_write_seq(dsi, 0xDA, 0x45);
	mipi_dsi_dcs_write_seq(dsi, 0xDB, 0x45);
	mipi_dsi_dcs_write_seq(dsi, 0xDC, 0x64);
	mipi_dsi_dcs_write_seq(dsi, 0xDD, 0x64);
	mipi_dsi_dcs_write_seq(dsi, 0xDE, 0x4F);
	mipi_dsi_dcs_write_seq(dsi, 0xDF, 0x4F);
	mipi_dsi_dcs_write_seq(dsi, 0xE0, 0x40);
	mipi_dsi_dcs_write_seq(dsi, 0xE1, 0x40);
	mipi_dsi_dcs_write_seq(dsi, 0xE2, 0x66);
	mipi_dsi_dcs_write_seq(dsi, 0xE3, 0x66);
	mipi_dsi_dcs_write_seq(dsi, 0xE4, 0x4F);
	mipi_dsi_dcs_write_seq(dsi, 0xE5, 0x4F);
	mipi_dsi_dcs_write_seq(dsi, 0xE6, 0x41);
	mipi_dsi_dcs_write_seq(dsi, 0xE7, 0x41);
	msleep(150);

	return 0;
}

static int starry_qfh032011_53g_init(struct mipi_dsi_device *dsi)
{
	int ret;

	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xC3, 0x4F);
	mipi_dsi_dcs_write_seq(dsi, 0xC4, 0x40);
	mipi_dsi_dcs_write_seq(dsi, 0xC5, 0x40);
	mipi_dsi_dcs_write_seq(dsi, 0xC6, 0x40);
	mipi_dsi_dcs_write_seq(dsi, 0xC7, 0x40);
	mipi_dsi_dcs_write_seq(dsi, 0xC8, 0x4D);
	mipi_dsi_dcs_write_seq(dsi, 0xC9, 0x52);
	mipi_dsi_dcs_write_seq(dsi, 0xCA, 0x51);
	mipi_dsi_dcs_write_seq(dsi, 0xCD, 0x5D);
	mipi_dsi_dcs_write_seq(dsi, 0xCE, 0x5B);
	mipi_dsi_dcs_write_seq(dsi, 0xCF, 0x4B);
	mipi_dsi_dcs_write_seq(dsi, 0xD0, 0x49);
	mipi_dsi_dcs_write_seq(dsi, 0xD1, 0x47);
	mipi_dsi_dcs_write_seq(dsi, 0xD2, 0x45);
	mipi_dsi_dcs_write_seq(dsi, 0xD3, 0x41);
	mipi_dsi_dcs_write_seq(dsi, 0xD7, 0x50);
	mipi_dsi_dcs_write_seq(dsi, 0xD8, 0x40);
	mipi_dsi_dcs_write_seq(dsi, 0xD9, 0x40);
	mipi_dsi_dcs_write_seq(dsi, 0xDA, 0x40);
	mipi_dsi_dcs_write_seq(dsi, 0xDB, 0x40);
	mipi_dsi_dcs_write_seq(dsi, 0xDC, 0x4E);
	mipi_dsi_dcs_write_seq(dsi, 0xDD, 0x52);
	mipi_dsi_dcs_write_seq(dsi, 0xDE, 0x51);
	mipi_dsi_dcs_write_seq(dsi, 0xE1, 0x5E);
	mipi_dsi_dcs_write_seq(dsi, 0xE2, 0x5C);
	mipi_dsi_dcs_write_seq(dsi, 0xE3, 0x4C);
	mipi_dsi_dcs_write_seq(dsi, 0xE4, 0x4A);
	mipi_dsi_dcs_write_seq(dsi, 0xE5, 0x48);
	mipi_dsi_dcs_write_seq(dsi, 0xE6, 0x46);
	mipi_dsi_dcs_write_seq(dsi, 0xE7, 0x42);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0xBE, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0xCC, 0x44);
	mipi_dsi_dcs_write_seq(dsi, 0xC8, 0x07);
	mipi_dsi_dcs_write_seq(dsi, 0xC9, 0x05);
	mipi_dsi_dcs_write_seq(dsi, 0xCA, 0x42);
	mipi_dsi_dcs_write_seq(dsi, 0xCD, 0x3E);
	mipi_dsi_dcs_write_seq(dsi, 0xCF, 0x60);
	mipi_dsi_dcs_write_seq(dsi, 0xD2, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0xD3, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0xD4, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xD5, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xD6, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0xD7, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0xD9, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xDB, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xE4, 0xF0);
	mipi_dsi_dcs_write_seq(dsi, 0xE5, 0x0A);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xCC, 0x08);
	mipi_dsi_dcs_write_seq(dsi, 0xC2, 0x08);
	mipi_dsi_dcs_write_seq(dsi, 0xC4, 0x10);
	mipi_dsi_dcs_write_seq(dsi, 0xB0, 0x02);
	mipi_dsi_dcs_write_seq(dsi, 0xC0, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xC1, 0x0A);
	mipi_dsi_dcs_write_seq(dsi, 0xC2, 0x20);
	mipi_dsi_dcs_write_seq(dsi, 0xC3, 0x24);
	mipi_dsi_dcs_write_seq(dsi, 0xC4, 0x23);
	mipi_dsi_dcs_write_seq(dsi, 0xC5, 0x29);
	mipi_dsi_dcs_write_seq(dsi, 0xC6, 0x23);
	mipi_dsi_dcs_write_seq(dsi, 0xC7, 0x1C);
	mipi_dsi_dcs_write_seq(dsi, 0xC8, 0x19);
	mipi_dsi_dcs_write_seq(dsi, 0xC9, 0x17);
	mipi_dsi_dcs_write_seq(dsi, 0xCA, 0x17);
	mipi_dsi_dcs_write_seq(dsi, 0xCB, 0x18);
	mipi_dsi_dcs_write_seq(dsi, 0xCC, 0x1A);
	mipi_dsi_dcs_write_seq(dsi, 0xCD, 0x1E);
	mipi_dsi_dcs_write_seq(dsi, 0xCE, 0x20);
	mipi_dsi_dcs_write_seq(dsi, 0xCF, 0x23);
	mipi_dsi_dcs_write_seq(dsi, 0xD0, 0x07);
	mipi_dsi_dcs_write_seq(dsi, 0xD1, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xD2, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xD3, 0x0A);
	mipi_dsi_dcs_write_seq(dsi, 0xD4, 0x13);
	mipi_dsi_dcs_write_seq(dsi, 0xD5, 0x1C);
	mipi_dsi_dcs_write_seq(dsi, 0xD6, 0x1A);
	mipi_dsi_dcs_write_seq(dsi, 0xD7, 0x13);
	mipi_dsi_dcs_write_seq(dsi, 0xD8, 0x17);
	mipi_dsi_dcs_write_seq(dsi, 0xD9, 0x1C);
	mipi_dsi_dcs_write_seq(dsi, 0xDA, 0x19);
	mipi_dsi_dcs_write_seq(dsi, 0xDB, 0x17);
	mipi_dsi_dcs_write_seq(dsi, 0xDC, 0x17);
	mipi_dsi_dcs_write_seq(dsi, 0xDD, 0x18);
	mipi_dsi_dcs_write_seq(dsi, 0xDE, 0x1A);
	mipi_dsi_dcs_write_seq(dsi, 0xDF, 0x1E);
	mipi_dsi_dcs_write_seq(dsi, 0xE0, 0x20);
	mipi_dsi_dcs_write_seq(dsi, 0xE1, 0x23);
	mipi_dsi_dcs_write_seq(dsi, 0xE2, 0x07);

	ret = mipi_dsi_dcs_exit_sleep_mode(dsi);
	if (ret)
		return ret;
	msleep(120);
	ret = mipi_dsi_dcs_set_display_on(dsi);
	if (ret)
		return ret;
	msleep(80);

	return 0;
}


static int starry_himax83102_j02_init(struct mipi_dsi_device *dsi)
{
	int ret;

	mipi_dsi_dcs_write_seq(dsi, 0xB9, 0x83, 0x10, 0x21, 0x55, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xB1, 0x2C, 0xB5, 0xB5, 0x31, 0xF1, 0x31, 0xD7, 0x2F, 0x36, 0x36,
			       0x36, 0x36, 0x1A, 0x8B, 0x11, 0x65, 0x00, 0x88, 0xFA, 0xFF, 0xFF, 0x8F,
			       0xFF, 0x08, 0x74, 0x33);
	mipi_dsi_dcs_write_seq(dsi, 0xB2, 0x00, 0x47, 0xB0, 0x80, 0x00, 0x12, 0x72, 0x3C, 0xA3, 0x03,
			       0x03, 0x00, 0x00, 0x88, 0xF5);
	mipi_dsi_dcs_write_seq(dsi, 0xB4, 0x76, 0x76, 0x76, 0x76, 0x76, 0x76, 0x63, 0x5C, 0x63, 0x5C,
			       0x01, 0x9E);
	mipi_dsi_dcs_write_seq(dsi, 0xE9, 0xCD);
	mipi_dsi_dcs_write_seq(dsi, 0xBA, 0x84);
	mipi_dsi_dcs_write_seq(dsi, 0xE9, 0x3F);
	mipi_dsi_dcs_write_seq(dsi, 0xBC, 0x1B, 0x04);
	mipi_dsi_dcs_write_seq(dsi, 0xBE, 0x20);
	mipi_dsi_dcs_write_seq(dsi, 0xBF, 0xFC, 0xC4);
	mipi_dsi_dcs_write_seq(dsi, 0xC0, 0x36, 0x36, 0x22, 0x11, 0x22, 0xA0, 0x61, 0x08, 0xF5, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0xE9, 0xCC);
	mipi_dsi_dcs_write_seq(dsi, 0xC7, 0x80);
	mipi_dsi_dcs_write_seq(dsi, 0xE9, 0x3F);
	mipi_dsi_dcs_write_seq(dsi, 0xE9, 0xC6);
	mipi_dsi_dcs_write_seq(dsi, 0xC8, 0x97);
	mipi_dsi_dcs_write_seq(dsi, 0xE9, 0x3F);
	mipi_dsi_dcs_write_seq(dsi, 0xC9, 0x00, 0x1E, 0x13, 0x88, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xCB, 0x08, 0x13, 0x07, 0x00, 0x0F, 0x33);
	mipi_dsi_dcs_write_seq(dsi, 0xCC, 0x02);
	mipi_dsi_dcs_write_seq(dsi, 0xE9, 0xC4);
	mipi_dsi_dcs_write_seq(dsi, 0xD0, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0xE9, 0x3F);
	mipi_dsi_dcs_write_seq(dsi, 0xD1, 0x37, 0x06, 0x00, 0x02, 0x04, 0x0C, 0xFF);
	mipi_dsi_dcs_write_seq(dsi, 0xD2, 0x1F, 0x11, 0x1F);
	mipi_dsi_dcs_write_seq(dsi, 0xD3, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x08,
			       0x37, 0x47, 0x34, 0x3B, 0x12, 0x12, 0x03, 0x03, 0x32, 0x10, 0x10,
			       0x00, 0x10, 0x32, 0x10, 0x08, 0x00, 0x08, 0x32, 0x17, 0x94, 0x07,
			       0x94, 0x00, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xD5, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
			       0x18, 0x19, 0x19, 0x40, 0x40, 0x1A, 0x1A, 0x1B, 0x1B, 0x00, 0x01,
			       0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x20, 0x21, 0x28, 0x29, 0x18,
			       0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
			       0x18, 0x18);
	mipi_dsi_dcs_write_seq(dsi, 0xD6, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
			       0x18, 0x40, 0x40, 0x19, 0x19, 0x1A, 0x1A, 0x1B, 0x1B, 0x07, 0x06,
			       0x05, 0x04, 0x03, 0x02, 0x01, 0x00, 0x29, 0x28, 0x21, 0x20, 0x18,
			       0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
			       0x18, 0x18);
	mipi_dsi_dcs_write_seq(dsi, 0xD8, 0xAA, 0xBA, 0xEA, 0xAA, 0xAA, 0xA0, 0xAA, 0xBA, 0xEA,
			       0xAA, 0xAA, 0xA0, 0xAA, 0xBA, 0xEA, 0xAA, 0xAA, 0xA0, 0xAA, 0xBA,
			       0xEA, 0xAA, 0xAA, 0xA0, 0xAA, 0xBA, 0xEA, 0xAA, 0xAA, 0xA0, 0xAA,
			       0xBA, 0xEA, 0xAA, 0xAA, 0xA0);
	mipi_dsi_dcs_write_seq(dsi, 0xE0, 0x00, 0x09, 0x14, 0x1E, 0x26, 0x48, 0x61, 0x67, 0x6C,
			       0x67, 0x7D, 0x7F, 0x80, 0x8B, 0x87, 0x8F, 0x98, 0xAB, 0xAB, 0x55,
			       0x5C, 0x68, 0x73, 0x00, 0x09, 0x14, 0x1E, 0x26, 0x48, 0x61, 0x67,
			       0x6C, 0x67, 0x7D, 0x7F, 0x80, 0x8B, 0x87, 0x8F, 0x98, 0xAB, 0xAB,
			       0x55, 0x5C, 0x68, 0x73);
	mipi_dsi_dcs_write_seq(dsi, 0xE7, 0x0E, 0x10, 0x10, 0x21, 0x2B, 0x9A, 0x02, 0x54, 0x9A,
			       0x14, 0x14, 0x00, 0x00, 0x00, 0x00, 0x12, 0x05, 0x02, 0x02, 0x10);
	mipi_dsi_dcs_write_seq(dsi, 0xBD, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xB1, 0x01, 0xBF, 0x11);
	mipi_dsi_dcs_write_seq(dsi, 0xCB, 0x86);
	mipi_dsi_dcs_write_seq(dsi, 0xD2, 0x3C, 0xFA);
	mipi_dsi_dcs_write_seq(dsi, 0xE9, 0xC5);
	mipi_dsi_dcs_write_seq(dsi, 0xD3, 0x00, 0x00, 0x00, 0x00, 0x80, 0x0C, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xE9, 0x3F);
	mipi_dsi_dcs_write_seq(dsi, 0xE7, 0x02, 0x00, 0x28, 0x01, 0x7E, 0x0F, 0x7E, 0x10, 0xA0,
			       0x00, 0x00, 0x20, 0x40, 0x50, 0x40);
	mipi_dsi_dcs_write_seq(dsi, 0xBD, 0x02);
	mipi_dsi_dcs_write_seq(dsi, 0xD8, 0xFF, 0xFF, 0xBF, 0xFE, 0xAA, 0xA0, 0xFF, 0xFF, 0xBF,
			       0xFE, 0xAA, 0xA0);
	mipi_dsi_dcs_write_seq(dsi, 0xE7, 0xFE, 0x04, 0xFE, 0x04, 0xFE, 0x04, 0x03, 0x03, 0x03,
			       0x26, 0x00, 0x26, 0x81, 0x02, 0x40, 0x00, 0x20, 0x9E, 0x04, 0x03,
			       0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xBD, 0x03);
	mipi_dsi_dcs_write_seq(dsi, 0xE9, 0xC6);
	mipi_dsi_dcs_write_seq(dsi, 0xB4, 0x03, 0xFF, 0xF8);
	mipi_dsi_dcs_write_seq(dsi, 0xE9, 0x3F);
	mipi_dsi_dcs_write_seq(dsi, 0xD8, 0x00, 0x2A, 0xAA, 0xA8, 0x00, 0x00, 0x00, 0x2A, 0xAA,
			       0xA8, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x3F,
			       0xFF, 0xFC, 0x00, 0x00, 0x00, 0x2A, 0xAA, 0xA8, 0x00, 0x00, 0x00,
			       0x2A, 0xAA, 0xA8, 0x00, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xBD, 0x00);
	mipi_dsi_dcs_write_seq(dsi, 0xE9, 0xC4);
	mipi_dsi_dcs_write_seq(dsi, 0xBA, 0x96);
	mipi_dsi_dcs_write_seq(dsi, 0xE9, 0x3F);
	mipi_dsi_dcs_write_seq(dsi, 0xBD, 0x01);
	mipi_dsi_dcs_write_seq(dsi, 0xE9, 0xC5);
	mipi_dsi_dcs_write_seq(dsi, 0xBA, 0x4F);
	mipi_dsi_dcs_write_seq(dsi, 0xE9, 0x3F);
	mipi_dsi_dcs_write_seq(dsi, 0xBD, 0x00);


	ret = mipi_dsi_dcs_exit_sleep_mode(dsi);
	if (ret)
		return ret;
	msleep(120);
	ret = mipi_dsi_dcs_set_display_on(dsi);
	if (ret)
		return ret;
	msleep(80);

	return 0;
};

static inline struct boe_panel *to_boe_panel(struct drm_panel *panel)
{
	return container_of(panel, struct boe_panel, base);
}

static int boe_panel_enter_sleep_mode(struct boe_panel *boe)
{
	struct mipi_dsi_device *dsi = boe->dsi;
	int ret;

	dsi->mode_flags &= ~MIPI_DSI_MODE_LPM;

	ret = mipi_dsi_dcs_set_display_off(dsi);
	if (ret < 0)
		return ret;

	ret = mipi_dsi_dcs_enter_sleep_mode(dsi);
	if (ret < 0)
		return ret;

	return 0;
}

static int boe_panel_disable(struct drm_panel *panel)
{
	struct boe_panel *boe = to_boe_panel(panel);
	int ret;

	ret = boe_panel_enter_sleep_mode(boe);
	if (ret < 0) {
		dev_err(panel->dev, "failed to set panel off: %d\n", ret);
		return ret;
	}

	msleep(150);

	return 0;
}

static int boe_panel_unprepare(struct drm_panel *panel)
{
	struct boe_panel *boe = to_boe_panel(panel);

	if (boe->desc->discharge_on_disable) {
		regulator_disable(boe->avee);
		regulator_disable(boe->avdd);
		usleep_range(5000, 7000);
		gpiod_set_value(boe->enable_gpio, 0);
		usleep_range(5000, 7000);
		regulator_disable(boe->pp1800);
		regulator_disable(boe->pp3300);
	} else {
		gpiod_set_value(boe->enable_gpio, 0);
		usleep_range(1000, 2000);
		regulator_disable(boe->avee);
		regulator_disable(boe->avdd);
		usleep_range(5000, 7000);
		regulator_disable(boe->pp1800);
		regulator_disable(boe->pp3300);
	}

	return 0;
}

static int boe_panel_prepare(struct drm_panel *panel)
{
	struct boe_panel *boe = to_boe_panel(panel);
	int ret;

	gpiod_set_value(boe->enable_gpio, 0);
	usleep_range(1000, 1500);

	ret = regulator_enable(boe->pp3300);
	if (ret < 0)
		return ret;

	ret = regulator_enable(boe->pp1800);
	if (ret < 0)
		return ret;

	usleep_range(3000, 5000);

	ret = regulator_enable(boe->avdd);
	if (ret < 0)
		goto poweroff1v8;
	ret = regulator_enable(boe->avee);
	if (ret < 0)
		goto poweroffavdd;

	usleep_range(10000, 11000);

	if (boe->desc->lp11_before_reset) {
		mipi_dsi_dcs_nop(boe->dsi);
		usleep_range(1000, 2000);
	}
	gpiod_set_value(boe->enable_gpio, 1);
	usleep_range(1000, 2000);
	gpiod_set_value(boe->enable_gpio, 0);
	usleep_range(1000, 2000);
	gpiod_set_value(boe->enable_gpio, 1);
	usleep_range(6000, 10000);

	if (boe->desc->init) {
		ret = boe->desc->init(boe->dsi);
		if (ret < 0) {
			dev_err(panel->dev, "failed to init panel: %d\n", ret);
			goto poweroff;
		}
	}

	return 0;

poweroff:
	regulator_disable(boe->avee);
poweroffavdd:
	regulator_disable(boe->avdd);
poweroff1v8:
	usleep_range(5000, 7000);
	regulator_disable(boe->pp1800);
	gpiod_set_value(boe->enable_gpio, 0);

	return ret;
}

static int boe_panel_enable(struct drm_panel *panel)
{
	msleep(130);
	return 0;
}

static const struct drm_display_mode boe_tv110c9m_default_mode = {
	.clock = 166594,
	.hdisplay = 1200,
	.hsync_start = 1200 + 40,
	.hsync_end = 1200 + 40 + 8,
	.htotal = 1200 + 40 + 8 + 28,
	.vdisplay = 2000,
	.vsync_start = 2000 + 26,
	.vsync_end = 2000 + 26 + 2,
	.vtotal = 2000 + 26 + 2 + 148,
	.type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED,
};

static const struct panel_desc boe_tv110c9m_desc = {
	.modes = &boe_tv110c9m_default_mode,
	.bpc = 8,
	.size = {
		.width_mm = 143,
		.height_mm = 238,
	},
	.lanes = 4,
	.format = MIPI_DSI_FMT_RGB888,
	.mode_flags = MIPI_DSI_MODE_LPM | MIPI_DSI_MODE_VIDEO
			| MIPI_DSI_MODE_VIDEO_HSE
			| MIPI_DSI_CLOCK_NON_CONTINUOUS
			| MIPI_DSI_MODE_VIDEO_BURST,
	.init = boe_tv110c9m_init,
};

static const struct drm_display_mode inx_hj110iz_default_mode = {
	.clock = 168432,
	.hdisplay = 1200,
	.hsync_start = 1200 + 40,
	.hsync_end = 1200 + 40 + 8,
	.htotal = 1200 + 40 + 8 + 28,
	.vdisplay = 2000,
	.vsync_start = 2000 + 26,
	.vsync_end = 2000 + 26 + 2,
	.vtotal = 2000 + 26 + 2 + 172,
	.type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED,
};

static const struct panel_desc inx_hj110iz_desc = {
	.modes = &inx_hj110iz_default_mode,
	.bpc = 8,
	.size = {
		.width_mm = 143,
		.height_mm = 238,
	},
	.lanes = 4,
	.format = MIPI_DSI_FMT_RGB888,
	.mode_flags = MIPI_DSI_MODE_LPM | MIPI_DSI_MODE_VIDEO
			| MIPI_DSI_MODE_VIDEO_HSE
			| MIPI_DSI_CLOCK_NON_CONTINUOUS
			| MIPI_DSI_MODE_VIDEO_BURST,
	.init = inx_hj110iz_init,
};

static const struct drm_display_mode boe_tv101wum_nl6_default_mode = {
	.clock = 159425,
	.hdisplay = 1200,
	.hsync_start = 1200 + 100,
	.hsync_end = 1200 + 100 + 40,
	.htotal = 1200 + 100 + 40 + 24,
	.vdisplay = 1920,
	.vsync_start = 1920 + 10,
	.vsync_end = 1920 + 10 + 14,
	.vtotal = 1920 + 10 + 14 + 4,
};

static const struct panel_desc boe_tv101wum_nl6_desc = {
	.modes = &boe_tv101wum_nl6_default_mode,
	.bpc = 8,
	.size = {
		.width_mm = 135,
		.height_mm = 216,
	},
	.lanes = 4,
	.format = MIPI_DSI_FMT_RGB888,
	.mode_flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_SYNC_PULSE |
		      MIPI_DSI_MODE_LPM,
	.init = boe_init,
	.discharge_on_disable = false,
};

static const struct drm_display_mode auo_kd101n80_45na_default_mode = {
	.clock = 157000,
	.hdisplay = 1200,
	.hsync_start = 1200 + 60,
	.hsync_end = 1200 + 60 + 24,
	.htotal = 1200 + 60 + 24 + 56,
	.vdisplay = 1920,
	.vsync_start = 1920 + 16,
	.vsync_end = 1920 + 16 + 4,
	.vtotal = 1920 + 16 + 4 + 16,
};

static const struct panel_desc auo_kd101n80_45na_desc = {
	.modes = &auo_kd101n80_45na_default_mode,
	.bpc = 8,
	.size = {
		.width_mm = 135,
		.height_mm = 216,
	},
	.lanes = 4,
	.format = MIPI_DSI_FMT_RGB888,
	.mode_flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_SYNC_PULSE |
		      MIPI_DSI_MODE_LPM,
	.init = auo_kd101n80_45na_init,
	.discharge_on_disable = true,
};

static const struct drm_display_mode boe_tv101wum_n53_default_mode = {
	.clock = 159916,
	.hdisplay = 1200,
	.hsync_start = 1200 + 80,
	.hsync_end = 1200 + 80 + 24,
	.htotal = 1200 + 80 + 24 + 60,
	.vdisplay = 1920,
	.vsync_start = 1920 + 20,
	.vsync_end = 1920 + 20 + 4,
	.vtotal = 1920 + 20 + 4 + 10,
	.type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED,
};

static const struct panel_desc boe_tv101wum_n53_desc = {
	.modes = &boe_tv101wum_n53_default_mode,
	.bpc = 8,
	.size = {
		.width_mm = 135,
		.height_mm = 216,
	},
	.lanes = 4,
	.format = MIPI_DSI_FMT_RGB888,
	.mode_flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_SYNC_PULSE |
		      MIPI_DSI_MODE_LPM,
	.init = boe_init,
};

static const struct drm_display_mode auo_b101uan08_3_default_mode = {
	.clock = 159667,
	.hdisplay = 1200,
	.hsync_start = 1200 + 60,
	.hsync_end = 1200 + 60 + 4,
	.htotal = 1200 + 60 + 4 + 80,
	.vdisplay = 1920,
	.vsync_start = 1920 + 34,
	.vsync_end = 1920 + 34 + 2,
	.vtotal = 1920 + 34 + 2 + 24,
	.type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED,
};

static const struct panel_desc auo_b101uan08_3_desc = {
	.modes = &auo_b101uan08_3_default_mode,
	.bpc = 8,
	.size = {
		.width_mm = 135,
		.height_mm = 216,
	},
	.lanes = 4,
	.format = MIPI_DSI_FMT_RGB888,
	.mode_flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_SYNC_PULSE |
		      MIPI_DSI_MODE_LPM,
	.init = auo_b101uan08_3_init,
};

static const struct drm_display_mode boe_tv105wum_nw0_default_mode = {
	.clock = 159916,
	.hdisplay = 1200,
	.hsync_start = 1200 + 80,
	.hsync_end = 1200 + 80 + 24,
	.htotal = 1200 + 80 + 24 + 60,
	.vdisplay = 1920,
	.vsync_start = 1920 + 20,
	.vsync_end = 1920 + 20 + 4,
	.vtotal = 1920 + 20 + 4 + 10,
	.type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED,
};

static const struct panel_desc boe_tv105wum_nw0_desc = {
	.modes = &boe_tv105wum_nw0_default_mode,
	.bpc = 8,
	.size = {
		.width_mm = 141,
		.height_mm = 226,
	},
	.lanes = 4,
	.format = MIPI_DSI_FMT_RGB888,
	.mode_flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_SYNC_PULSE |
		      MIPI_DSI_MODE_LPM,
	.init = boe_init,
	.lp11_before_reset = true,
};

static const struct drm_display_mode starry_qfh032011_53g_default_mode = {
	.clock = 165731,
	.hdisplay = 1200,
	.hsync_start = 1200 + 100,
	.hsync_end = 1200 + 100 + 10,
	.htotal = 1200 + 100 + 10 + 100,
	.vdisplay = 1920,
	.vsync_start = 1920 + 14,
	.vsync_end = 1920 + 14 + 10,
	.vtotal = 1920 + 14 + 10 + 15,
};

static const struct panel_desc starry_qfh032011_53g_desc = {
	.modes = &starry_qfh032011_53g_default_mode,
	.bpc = 8,
	.size = {
		.width_mm = 135,
		.height_mm = 216,
	},
	.lanes = 4,
	.format = MIPI_DSI_FMT_RGB888,
	.mode_flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_SYNC_PULSE |
		      MIPI_DSI_MODE_LPM,
	.init = starry_qfh032011_53g_init,
};

static const struct drm_display_mode starry_himax83102_j02_default_mode = {
	.clock = 161600,
	.hdisplay = 1200,
	.hsync_start = 1200 + 40,
	.hsync_end = 1200 + 40 + 20,
	.htotal = 1200 + 40 + 20 + 40,
	.vdisplay = 1920,
	.vsync_start = 1920 + 116,
	.vsync_end = 1920 + 116 + 8,
	.vtotal = 1920 + 116 + 8 + 12,
	.type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED,
};

static const struct panel_desc starry_himax83102_j02_desc = {
	.modes = &starry_himax83102_j02_default_mode,
	.bpc = 8,
	.size = {
		.width_mm = 141,
		.height_mm = 226,
	},
	.lanes = 4,
	.format = MIPI_DSI_FMT_RGB888,
	.mode_flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_SYNC_PULSE |
		      MIPI_DSI_MODE_LPM,
	.init = starry_himax83102_j02_init,
	.lp11_before_reset = true,
};

static int boe_panel_get_modes(struct drm_panel *panel,
			       struct drm_connector *connector)
{
	struct boe_panel *boe = to_boe_panel(panel);
	const struct drm_display_mode *m = boe->desc->modes;
	struct drm_display_mode *mode;

	mode = drm_mode_duplicate(connector->dev, m);
	if (!mode) {
		dev_err(panel->dev, "failed to add mode %ux%u@%u\n",
			m->hdisplay, m->vdisplay, drm_mode_vrefresh(m));
		return -ENOMEM;
	}

	mode->type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED;
	drm_mode_set_name(mode);
	drm_mode_probed_add(connector, mode);

	connector->display_info.width_mm = boe->desc->size.width_mm;
	connector->display_info.height_mm = boe->desc->size.height_mm;
	connector->display_info.bpc = boe->desc->bpc;
	/*
	 * TODO: Remove once all drm drivers call
	 * drm_connector_set_orientation_from_panel()
	 */
	drm_connector_set_panel_orientation(connector, boe->orientation);

	return 1;
}

static enum drm_panel_orientation boe_panel_get_orientation(struct drm_panel *panel)
{
	struct boe_panel *boe = to_boe_panel(panel);

	return boe->orientation;
}

static const struct drm_panel_funcs boe_panel_funcs = {
	.disable = boe_panel_disable,
	.unprepare = boe_panel_unprepare,
	.prepare = boe_panel_prepare,
	.enable = boe_panel_enable,
	.get_modes = boe_panel_get_modes,
	.get_orientation = boe_panel_get_orientation,
};

static int boe_panel_add(struct boe_panel *boe)
{
	struct device *dev = &boe->dsi->dev;
	int err;

	boe->avdd = devm_regulator_get(dev, "avdd");
	if (IS_ERR(boe->avdd))
		return PTR_ERR(boe->avdd);

	boe->avee = devm_regulator_get(dev, "avee");
	if (IS_ERR(boe->avee))
		return PTR_ERR(boe->avee);

	boe->pp3300 = devm_regulator_get(dev, "pp3300");
	if (IS_ERR(boe->pp3300))
		return PTR_ERR(boe->pp3300);

	boe->pp1800 = devm_regulator_get(dev, "pp1800");
	if (IS_ERR(boe->pp1800))
		return PTR_ERR(boe->pp1800);

	boe->enable_gpio = devm_gpiod_get(dev, "enable", GPIOD_OUT_LOW);
	if (IS_ERR(boe->enable_gpio)) {
		dev_err(dev, "cannot get reset-gpios %ld\n",
			PTR_ERR(boe->enable_gpio));
		return PTR_ERR(boe->enable_gpio);
	}

	gpiod_set_value(boe->enable_gpio, 0);

	drm_panel_init(&boe->base, dev, &boe_panel_funcs,
		       DRM_MODE_CONNECTOR_DSI);
	err = of_drm_get_panel_orientation(dev->of_node, &boe->orientation);
	if (err < 0) {
		dev_err(dev, "%pOF: failed to get orientation %d\n", dev->of_node, err);
		return err;
	}

	err = drm_panel_of_backlight(&boe->base);
	if (err)
		return err;

	boe->base.funcs = &boe_panel_funcs;
	boe->base.dev = &boe->dsi->dev;

	drm_panel_add(&boe->base);

	return 0;
}

static int boe_panel_probe(struct mipi_dsi_device *dsi)
{
	struct boe_panel *boe;
	int ret;
	const struct panel_desc *desc;

	boe = devm_kzalloc(&dsi->dev, sizeof(*boe), GFP_KERNEL);
	if (!boe)
		return -ENOMEM;

	desc = of_device_get_match_data(&dsi->dev);
	dsi->lanes = desc->lanes;
	dsi->format = desc->format;
	dsi->mode_flags = desc->mode_flags;
	boe->desc = desc;
	boe->dsi = dsi;
	ret = boe_panel_add(boe);
	if (ret < 0)
		return ret;

	mipi_dsi_set_drvdata(dsi, boe);

	ret = mipi_dsi_attach(dsi);
	if (ret)
		drm_panel_remove(&boe->base);

	return ret;
}

static void boe_panel_shutdown(struct mipi_dsi_device *dsi)
{
	struct boe_panel *boe = mipi_dsi_get_drvdata(dsi);

	drm_panel_disable(&boe->base);
	drm_panel_unprepare(&boe->base);
}

static void boe_panel_remove(struct mipi_dsi_device *dsi)
{
	struct boe_panel *boe = mipi_dsi_get_drvdata(dsi);
	int ret;

	boe_panel_shutdown(dsi);

	ret = mipi_dsi_detach(dsi);
	if (ret < 0)
		dev_err(&dsi->dev, "failed to detach from DSI host: %d\n", ret);

	if (boe->base.dev)
		drm_panel_remove(&boe->base);
}

static const struct of_device_id boe_of_match[] = {
	{ .compatible = "boe,tv101wum-nl6",
	  .data = &boe_tv101wum_nl6_desc
	},
	{ .compatible = "auo,kd101n80-45na",
	  .data = &auo_kd101n80_45na_desc
	},
	{ .compatible = "boe,tv101wum-n53",
	  .data = &boe_tv101wum_n53_desc
	},
	{ .compatible = "auo,b101uan08.3",
	  .data = &auo_b101uan08_3_desc
	},
	{ .compatible = "boe,tv105wum-nw0",
	  .data = &boe_tv105wum_nw0_desc
	},
	{ .compatible = "boe,tv110c9m-ll3",
	  .data = &boe_tv110c9m_desc
	},
	{ .compatible = "innolux,hj110iz-01a",
	  .data = &inx_hj110iz_desc
	},
	{ .compatible = "starry,2081101qfh032011-53g",
	  .data = &starry_qfh032011_53g_desc
	},
	{ .compatible = "starry,himax83102-j02",
	  .data = &starry_himax83102_j02_desc
	},
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, boe_of_match);

static struct mipi_dsi_driver boe_panel_driver = {
	.driver = {
		.name = "panel-boe-tv101wum-nl6",
		.of_match_table = boe_of_match,
	},
	.probe = boe_panel_probe,
	.remove = boe_panel_remove,
	.shutdown = boe_panel_shutdown,
};
module_mipi_dsi_driver(boe_panel_driver);

MODULE_AUTHOR("Jitao Shi <jitao.shi@mediatek.com>");
MODULE_DESCRIPTION("BOE tv101wum-nl6 1200x1920 video mode panel driver");
MODULE_LICENSE("GPL v2");
