/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2020
 * Author(s): Sicris Rey Embay <sicris.embay@gmail.com>
 */

#ifndef __IMXRT105X_ZB_H
#define __IMXRT105X_ZB_H

#include <asm/arch/imx-regs.h>

#define DEBUG                       (1)
#define CONFIG_SYS_INIT_SP_ADDR     0x80800000

/*
 * To get Image data right at the 'Load Address' (0x80008000), and thus avoid
 * additional uImage relocation:
 * - 0x80007fc0 reserve place for uImage header; two separate image/dtb files
 * - 0x80007fb4 reserve place for 2-files multi-image header; one image+dtb file
 *
 * Note, that unaligned address can't be used when load from FAT as this results
 * to the slow copy path (and 'FAT: Misaligned buffer address') in fs/fat/fat.c.
 */
#define CONFIG_SYS_LOAD_ADDR        0x80007fc0
#define CONFIG_LOADADDR             0x80007fc0

#define CONFIG_SYS_FSL_ERRATUM_ESDHC135		1
#define ESDHCI_QUIRK_BROKEN_TIMEOUT_VALUE	1

#define PHYS_SDRAM                  0x80000000
#define PHYS_SDRAM_SIZE             (16 * 1024 * 1024)

#define DMAMEM_SZ_ALL               (1 * 1024 * 1024)
#define DMAMEM_BASE                 (PHYS_SDRAM + PHYS_SDRAM_SIZE - \
                                        DMAMEM_SZ_ALL)

#define CONFIG_SYS_MMC_ENV_DEV		0   /* USDHC1 */

/*
 * Configuration of the external SDRAM memory
 */
#define CONFIG_SYS_MALLOC_LEN		(1 * 1024 * 1024)

#endif /* __IMXRT105X_ZB_H */
