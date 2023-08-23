// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2016 MediaTek Inc.
 * Author: Ming Hsiu Tsai <minghsiu.tsai@mediatek.com>
 */

#include <linux/clk.h>
#include <linux/component.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <soc/mediatek/smi.h>
#include <linux/pm_runtime.h>

#include "mtk_mdp_comp.h"
#include "mtk_mdp_core.h"

/**
 * enum mtk_mdp_comp_type - the MDP component
 * @MTK_MDP_RDMA:		Read DMA
 * @MTK_MDP_RSZ:		Reszer
 * @MTK_MDP_WDMA:		Write DMA
 * @MTK_MDP_WROT:		Write DMA with rotation
 * @MTK_MDP_COMP_TYPE_MAX:	Placeholder for num elems in this enum
 */
enum mtk_mdp_comp_type {
	MTK_MDP_RDMA,
	MTK_MDP_RSZ,
	MTK_MDP_WDMA,
	MTK_MDP_WROT,
	MTK_MDP_COMP_TYPE_MAX,
};

static const struct of_device_id mtk_mdp_comp_driver_dt_match[] = {
	{
		.compatible = "mediatek,mt8173-mdp-rdma",
		.data = (void *)MTK_MDP_RDMA
	}, {
		.compatible = "mediatek,mt8173-mdp-rsz",
		.data = (void *)MTK_MDP_RSZ
	}, {
		.compatible = "mediatek,mt8173-mdp-wdma",
		.data = (void *)MTK_MDP_WDMA
	}, {
		.compatible = "mediatek,mt8173-mdp-wrot",
		.data = (void *)MTK_MDP_WROT
	},
	{ }
};
MODULE_DEVICE_TABLE(of, mtk_mdp_comp_driver_dt_match);

int mtk_mdp_comp_power_on(struct mtk_mdp_comp *comp)

{
	int err;

	err = pm_runtime_get_sync(comp->dev);
	if (err < 0) {
		dev_err(comp->dev, "failed to runtime get, err %d.\n", err);
		return err;
	}

	err = mtk_mdp_comp_clock_on(comp);
	if (err) {
		dev_err(comp->dev, "failed to turn on clock. err=%d", err);
		goto err_mtk_mdp_comp_clock_on;
	}

	return 0;

err_mtk_mdp_comp_clock_on:
	err = pm_runtime_put_sync(comp->dev);
	if (err)
		dev_err(comp->dev, "failed to runtime put in cleanup. err=%d", err);

	return err;
}

int mtk_mdp_comp_power_off(struct mtk_mdp_comp *comp)
{
	int status, err;

	mtk_mdp_comp_clock_off(comp);

	err = pm_runtime_put_sync(comp->dev);
	if (err < 0) {
		dev_err(comp->dev, "failed to runtime put, err %d.\n", err);
		status = err;
		goto err_pm_runtime_put_sync;
	}

	return 0;

err_pm_runtime_put_sync:
	err = mtk_mdp_comp_clock_on(comp);
	if (err)
		dev_err(comp->dev, "failed to turn on clock in cleanup. err=%d", err);

	return status;
}

int mtk_mdp_comp_clock_on(struct mtk_mdp_comp *comp)
{
	int i, err;

	for (i = 0; i < ARRAY_SIZE(comp->clk); i++) {
		if (IS_ERR(comp->clk[i]))
			continue;
		err = clk_prepare_enable(comp->clk[i]);
		if (err) {
			dev_err(comp->dev, "failed to enable clock, err %d. i:%d\n", err, i);
			goto err_clk_prepare_enable;
		}
	}

	return 0;

err_clk_prepare_enable:
	for (--i; i >= 0; i--) {
		if (IS_ERR(comp->clk[i]))
			continue;
		clk_disable_unprepare(comp->clk[i]);
	}

	pm_runtime_put_sync(comp->dev);

	return err;
}

void mtk_mdp_comp_clock_off(struct mtk_mdp_comp *comp)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(comp->clk); i++) {
		if (IS_ERR(comp->clk[i]))
			continue;
		clk_disable_unprepare(comp->clk[i]);
	}
}

static int mtk_mdp_comp_bind(struct device *dev, struct device *master, void *data)
{
	struct mtk_mdp_comp *comp = dev_get_drvdata(dev);
	struct mtk_mdp_dev *mdp = data;

	mtk_mdp_register_component(mdp, comp);
	pm_runtime_enable(dev);

	return 0;
}

static void mtk_mdp_comp_unbind(struct device *dev, struct device *master,
			   void *data)
{
	struct mtk_mdp_comp *comp = dev_get_drvdata(dev);
	struct mtk_mdp_dev *mdp = data;

	pm_runtime_disable(dev);
	mtk_mdp_unregister_component(mdp, comp);
}

static const struct component_ops mtk_mdp_component_ops = {
	.bind   = mtk_mdp_comp_bind,
	.unbind = mtk_mdp_comp_unbind,
};

int mtk_mdp_comp_init(struct mtk_mdp_comp *comp, struct device *dev)
{
	int ret;
	int i;
	struct device_node *node = dev->of_node;
	enum mtk_mdp_comp_type comp_type =
		(long)of_device_get_match_data(dev);

	INIT_LIST_HEAD(&comp->node);
	comp->dev = dev;

	for (i = 0; i < ARRAY_SIZE(comp->clk); i++) {
		comp->clk[i] = of_clk_get(node, i);
		if (IS_ERR(comp->clk[i])) {
			if (PTR_ERR(comp->clk[i]) != -EPROBE_DEFER)
				dev_err(dev, "Failed to get clock\n");
			ret = PTR_ERR(comp->clk[i]);
			goto err;
		}

		/* Only RDMA needs two clocks */
		if (comp_type != MTK_MDP_RDMA)
			break;
	}

	return 0;

err:
	return ret;
}

static int mtk_mdp_comp_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *vpu_node;
	int status;
	struct mtk_mdp_comp *comp;

	vpu_node = of_parse_phandle(dev->of_node, "mediatek,vpu", 0);
	if (vpu_node) {
		of_node_put(vpu_node);
		/*
		 * The device tree node with a mediatek,vpu property is deemed
		 * the MDP "master" device, we don't want to add a component
		 * for it in this function because the initialization for the
		 * master is done elsewhere.
		 */
		dev_info(dev, "vpu node found, not probing\n");
		return -ENODEV;
	}

	comp = devm_kzalloc(dev, sizeof(*comp), GFP_KERNEL);
	if (!comp)
		return -ENOMEM;

	status = mtk_mdp_comp_init(comp, dev);
	if (status) {
		dev_err(dev, "Failed to initialize component: %d\n", status);
		return status;
	}

	dev_set_drvdata(dev, comp);

	return component_add(dev, &mtk_mdp_component_ops);
}

static int mtk_mdp_comp_remove(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;

	component_del(dev, &mtk_mdp_component_ops);
	return 0;
}

struct platform_driver mtk_mdp_component_driver = {
	.probe          = mtk_mdp_comp_probe,
	.remove         = mtk_mdp_comp_remove,
	.driver         = {
		.name   = "mediatek-mdp-comp",
		.owner  = THIS_MODULE,
		.of_match_table = mtk_mdp_comp_driver_dt_match,
	},
};
