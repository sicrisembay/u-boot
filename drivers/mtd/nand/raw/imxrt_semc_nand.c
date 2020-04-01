// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2020
 * Author(s): Sicris Rey Embay <sicris.embay@gmail.com>
 */

#include <dm.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/printk.h>

#include <mxs_nand.h>

struct imxrt_nand_info_struct {
    unsigned char *data_buf;
    unsigned char *oob_buf;
    bool spare_only;
    bool status_request;
    unsigned short col_addr;
    unsigned long page_addr;
    unsigned char status;
};
struct imxrt_semc_nand_data_struct {
    unsigned int max_ecc_strength_supported;
    struct imxrt_nand_info_struct imxrt_nand_info;
};

static struct imxrt_semc_nand_data_struct imxrt_semc_nand_data = {
    .max_ecc_strength_supported = 1,
};

static int imxrt_nand_probe(struct udevice *dev)
{
    struct mxs_nand_info *info = dev_get_priv(dev);
    struct imxrt_semc_nand_data_struct *data;

    data = (void *)dev_get_driver_data(dev);

    return 0;
}

static const struct udevice_id imxrt_semc_nand_ids[] = {
    {
        .compatible = "fsl,imxrt-semc-nand",
        .data = (unsigned long)&imxrt_semc_nand_data,
    },
    { /* sentinel */ }
};

U_BOOT_DRIVER(imxrt_semc_nand) = {
    .name = "imxrt_semc_nand",
    .id = UCLASS_MTD,
    .of_match = imxrt_semc_nand_ids,
    .probe = imxrt_nand_probe,
    .priv_auto_alloc_size = sizeof(struct mxs_nand_info),
};
