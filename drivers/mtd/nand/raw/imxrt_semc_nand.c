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

#define DEBUG_IMXRT_SEMC_NAND       0
#if (DEBUG_IMXRT_SEMC_NAND == 1)
#define dbgImxrtNand(fmt, ...) \
    printf(fmt, ##__VA_ARGS__)
#else
#define dbgImxrtNand(fmt, ...)
#endif

#define SEMC_MCR_REG_OFFSET         0x00
#define SEMC_IOCR_REG_OFFSET        0x04
#define SEMC_BR8_REG_OFFSET         0x30
#define SEMC_INTR_REG_OFFSET        0x3C
#define SEMC_NANDCR_REG_OFFSET      0x50
#define SEMC_IP_REG_OFFSET          0x90
#define SEMC_STS_REG_OFFSET         0xC0

/* Define macros for SEMC driver. */
#define SEMC_IPCOMMANDDATASIZEBYTEMAX   (4U)
#define SEMC_IPCOMMANDMAGICKEY          (0xA55A)


/* INTR - Interrupt Enable Register */
#define SEMC_INTR_IPCMDDONE_MASK                 (0x1U)
#define SEMC_INTR_IPCMDDONE_SHIFT                (0U)
#define SEMC_INTR_IPCMDDONE(x)                   (((uint32_t)(((uint32_t)(x)) << SEMC_INTR_IPCMDDONE_SHIFT)) & SEMC_INTR_IPCMDDONE_MASK)
#define SEMC_INTR_IPCMDERR_MASK                  (0x2U)
#define SEMC_INTR_IPCMDERR_SHIFT                 (1U)
#define SEMC_INTR_IPCMDERR(x)                    (((uint32_t)(((uint32_t)(x)) << SEMC_INTR_IPCMDERR_SHIFT)) & SEMC_INTR_IPCMDERR_MASK)
#define SEMC_INTR_AXICMDERR_MASK                 (0x4U)
#define SEMC_INTR_AXICMDERR_SHIFT                (2U)
#define SEMC_INTR_AXICMDERR(x)                   (((uint32_t)(((uint32_t)(x)) << SEMC_INTR_AXICMDERR_SHIFT)) & SEMC_INTR_AXICMDERR_MASK)
#define SEMC_INTR_AXIBUSERR_MASK                 (0x8U)
#define SEMC_INTR_AXIBUSERR_SHIFT                (3U)
#define SEMC_INTR_AXIBUSERR(x)                   (((uint32_t)(((uint32_t)(x)) << SEMC_INTR_AXIBUSERR_SHIFT)) & SEMC_INTR_AXIBUSERR_MASK)
#define SEMC_INTR_NDPAGEEND_MASK                 (0x10U)
#define SEMC_INTR_NDPAGEEND_SHIFT                (4U)
#define SEMC_INTR_NDPAGEEND(x)                   (((uint32_t)(((uint32_t)(x)) << SEMC_INTR_NDPAGEEND_SHIFT)) & SEMC_INTR_NDPAGEEND_MASK)
#define SEMC_INTR_NDNOPEND_MASK                  (0x20U)
#define SEMC_INTR_NDNOPEND_SHIFT                 (5U)
#define SEMC_INTR_NDNOPEND(x)                    (((uint32_t)(((uint32_t)(x)) << SEMC_INTR_NDNOPEND_SHIFT)) & SEMC_INTR_NDNOPEND_MASK)

/* NANDCR0 - NAND control register 0 */
#define SEMC_NANDCR0_PS_MASK                     (0x1U)
#define SEMC_NANDCR0_PS_SHIFT                    (0U)
#define SEMC_NANDCR0_PS(x)                       (((uint32_t)(((uint32_t)(x)) << SEMC_NANDCR0_PS_SHIFT)) & SEMC_NANDCR0_PS_MASK)
#define SEMC_NANDCR0_BL_MASK                     (0x70U)
#define SEMC_NANDCR0_BL_SHIFT                    (4U)
#define SEMC_NANDCR0_BL(x)                       (((uint32_t)(((uint32_t)(x)) << SEMC_NANDCR0_BL_SHIFT)) & SEMC_NANDCR0_BL_MASK)
#define SEMC_NANDCR0_EDO_MASK                    (0x80U)
#define SEMC_NANDCR0_EDO_SHIFT                   (7U)
#define SEMC_NANDCR0_EDO(x)                      (((uint32_t)(((uint32_t)(x)) << SEMC_NANDCR0_EDO_SHIFT)) & SEMC_NANDCR0_EDO_MASK)
#define SEMC_NANDCR0_COL_MASK                    (0x700U)
#define SEMC_NANDCR0_COL_SHIFT                   (8U)
#define SEMC_NANDCR0_COL(x)                      (((uint32_t)(((uint32_t)(x)) << SEMC_NANDCR0_COL_SHIFT)) & SEMC_NANDCR0_COL_MASK)

/* NANDCR1 - NAND control register 1 */
#define SEMC_NANDCR1_CES_MASK                    (0xFU)
#define SEMC_NANDCR1_CES_SHIFT                   (0U)
#define SEMC_NANDCR1_CES(x)                      (((uint32_t)(((uint32_t)(x)) << SEMC_NANDCR1_CES_SHIFT)) & SEMC_NANDCR1_CES_MASK)
#define SEMC_NANDCR1_CEH_MASK                    (0xF0U)
#define SEMC_NANDCR1_CEH_SHIFT                   (4U)
#define SEMC_NANDCR1_CEH(x)                      (((uint32_t)(((uint32_t)(x)) << SEMC_NANDCR1_CEH_SHIFT)) & SEMC_NANDCR1_CEH_MASK)
#define SEMC_NANDCR1_WEL_MASK                    (0xF00U)
#define SEMC_NANDCR1_WEL_SHIFT                   (8U)
#define SEMC_NANDCR1_WEL(x)                      (((uint32_t)(((uint32_t)(x)) << SEMC_NANDCR1_WEL_SHIFT)) & SEMC_NANDCR1_WEL_MASK)
#define SEMC_NANDCR1_WEH_MASK                    (0xF000U)
#define SEMC_NANDCR1_WEH_SHIFT                   (12U)
#define SEMC_NANDCR1_WEH(x)                      (((uint32_t)(((uint32_t)(x)) << SEMC_NANDCR1_WEH_SHIFT)) & SEMC_NANDCR1_WEH_MASK)
#define SEMC_NANDCR1_REL_MASK                    (0xF0000U)
#define SEMC_NANDCR1_REL_SHIFT                   (16U)
#define SEMC_NANDCR1_REL(x)                      (((uint32_t)(((uint32_t)(x)) << SEMC_NANDCR1_REL_SHIFT)) & SEMC_NANDCR1_REL_MASK)
#define SEMC_NANDCR1_REH_MASK                    (0xF00000U)
#define SEMC_NANDCR1_REH_SHIFT                   (20U)
#define SEMC_NANDCR1_REH(x)                      (((uint32_t)(((uint32_t)(x)) << SEMC_NANDCR1_REH_SHIFT)) & SEMC_NANDCR1_REH_MASK)
#define SEMC_NANDCR1_TA_MASK                     (0xF000000U)
#define SEMC_NANDCR1_TA_SHIFT                    (24U)
#define SEMC_NANDCR1_TA(x)                       (((uint32_t)(((uint32_t)(x)) << SEMC_NANDCR1_TA_SHIFT)) & SEMC_NANDCR1_TA_MASK)
#define SEMC_NANDCR1_CEITV_MASK                  (0xF0000000U)
#define SEMC_NANDCR1_CEITV_SHIFT                 (28U)
#define SEMC_NANDCR1_CEITV(x)                    (((uint32_t)(((uint32_t)(x)) << SEMC_NANDCR1_CEITV_SHIFT)) & SEMC_NANDCR1_CEITV_MASK)

/* NANDCR2 - NAND control register 2 */
#define SEMC_NANDCR2_TWHR_MASK                   (0x3FU)
#define SEMC_NANDCR2_TWHR_SHIFT                  (0U)
#define SEMC_NANDCR2_TWHR(x)                     (((uint32_t)(((uint32_t)(x)) << SEMC_NANDCR2_TWHR_SHIFT)) & SEMC_NANDCR2_TWHR_MASK)
#define SEMC_NANDCR2_TRHW_MASK                   (0xFC0U)
#define SEMC_NANDCR2_TRHW_SHIFT                  (6U)
#define SEMC_NANDCR2_TRHW(x)                     (((uint32_t)(((uint32_t)(x)) << SEMC_NANDCR2_TRHW_SHIFT)) & SEMC_NANDCR2_TRHW_MASK)
#define SEMC_NANDCR2_TADL_MASK                   (0x3F000U)
#define SEMC_NANDCR2_TADL_SHIFT                  (12U)
#define SEMC_NANDCR2_TADL(x)                     (((uint32_t)(((uint32_t)(x)) << SEMC_NANDCR2_TADL_SHIFT)) & SEMC_NANDCR2_TADL_MASK)
#define SEMC_NANDCR2_TRR_MASK                    (0xFC0000U)
#define SEMC_NANDCR2_TRR_SHIFT                   (18U)
#define SEMC_NANDCR2_TRR(x)                      (((uint32_t)(((uint32_t)(x)) << SEMC_NANDCR2_TRR_SHIFT)) & SEMC_NANDCR2_TRR_MASK)
#define SEMC_NANDCR2_TWB_MASK                    (0x3F000000U)
#define SEMC_NANDCR2_TWB_SHIFT                   (24U)
#define SEMC_NANDCR2_TWB(x)                      (((uint32_t)(((uint32_t)(x)) << SEMC_NANDCR2_TWB_SHIFT)) & SEMC_NANDCR2_TWB_MASK)

/* NANDCR3 - NAND control register 3 */
#define SEMC_NANDCR3_NDOPT1_MASK                 (0x1U)
#define SEMC_NANDCR3_NDOPT1_SHIFT                (0U)
#define SEMC_NANDCR3_NDOPT1(x)                   (((uint32_t)(((uint32_t)(x)) << SEMC_NANDCR3_NDOPT1_SHIFT)) & SEMC_NANDCR3_NDOPT1_MASK)
#define SEMC_NANDCR3_NDOPT2_MASK                 (0x2U)
#define SEMC_NANDCR3_NDOPT2_SHIFT                (1U)
#define SEMC_NANDCR3_NDOPT2(x)                   (((uint32_t)(((uint32_t)(x)) << SEMC_NANDCR3_NDOPT2_SHIFT)) & SEMC_NANDCR3_NDOPT2_MASK)
#define SEMC_NANDCR3_NDOPT3_MASK                 (0x4U)
#define SEMC_NANDCR3_NDOPT3_SHIFT                (2U)
#define SEMC_NANDCR3_NDOPT3(x)                   (((uint32_t)(((uint32_t)(x)) << SEMC_NANDCR3_NDOPT3_SHIFT)) & SEMC_NANDCR3_NDOPT3_MASK)

/* IPCR0 - IP Command control register 0 */
#define SEMC_IPCR0_SA_MASK                       (0xFFFFFFFFU)
#define SEMC_IPCR0_SA_SHIFT                      (0U)
#define SEMC_IPCR0_SA(x)                         (((uint32_t)(((uint32_t)(x)) << SEMC_IPCR0_SA_SHIFT)) & SEMC_IPCR0_SA_MASK)

/* IPCR1 - IP Command control register 1 */
#define SEMC_IPCR1_DATSZ_MASK                    (0x7U)
#define SEMC_IPCR1_DATSZ_SHIFT                   (0U)
#define SEMC_IPCR1_DATSZ(x)                      (((uint32_t)(((uint32_t)(x)) << SEMC_IPCR1_DATSZ_SHIFT)) & SEMC_IPCR1_DATSZ_MASK)

/* IPCR2 - IP Command control register 2 */
#define SEMC_IPCR2_BM0_MASK                      (0x1U)
#define SEMC_IPCR2_BM0_SHIFT                     (0U)
#define SEMC_IPCR2_BM0(x)                        (((uint32_t)(((uint32_t)(x)) << SEMC_IPCR2_BM0_SHIFT)) & SEMC_IPCR2_BM0_MASK)
#define SEMC_IPCR2_BM1_MASK                      (0x2U)
#define SEMC_IPCR2_BM1_SHIFT                     (1U)
#define SEMC_IPCR2_BM1(x)                        (((uint32_t)(((uint32_t)(x)) << SEMC_IPCR2_BM1_SHIFT)) & SEMC_IPCR2_BM1_MASK)
#define SEMC_IPCR2_BM2_MASK                      (0x4U)
#define SEMC_IPCR2_BM2_SHIFT                     (2U)
#define SEMC_IPCR2_BM2(x)                        (((uint32_t)(((uint32_t)(x)) << SEMC_IPCR2_BM2_SHIFT)) & SEMC_IPCR2_BM2_MASK)
#define SEMC_IPCR2_BM3_MASK                      (0x8U)
#define SEMC_IPCR2_BM3_SHIFT                     (3U)
#define SEMC_IPCR2_BM3(x)                        (((uint32_t)(((uint32_t)(x)) << SEMC_IPCR2_BM3_SHIFT)) & SEMC_IPCR2_BM3_MASK)

/* IPCMD - IP Command register */
#define SEMC_IPCMD_CMD_MASK                      (0xFFFFU)
#define SEMC_IPCMD_CMD_SHIFT                     (0U)
#define SEMC_IPCMD_CMD(x)                        (((uint32_t)(((uint32_t)(x)) << SEMC_IPCMD_CMD_SHIFT)) & SEMC_IPCMD_CMD_MASK)
#define SEMC_IPCMD_KEY_MASK                      (0xFFFF0000U)
#define SEMC_IPCMD_KEY_SHIFT                     (16U)
#define SEMC_IPCMD_KEY(x)                        (((uint32_t)(((uint32_t)(x)) << SEMC_IPCMD_KEY_SHIFT)) & SEMC_IPCMD_KEY_MASK)

/* IPTXDAT - TX DATA register (for IP Command) */
#define SEMC_IPTXDAT_DAT_MASK                    (0xFFFFFFFFU)
#define SEMC_IPTXDAT_DAT_SHIFT                   (0U)
#define SEMC_IPTXDAT_DAT(x)                      (((uint32_t)(((uint32_t)(x)) << SEMC_IPTXDAT_DAT_SHIFT)) & SEMC_IPTXDAT_DAT_MASK)

/* IPRXDAT - RX DATA register (for IP Command) */
#define SEMC_IPRXDAT_DAT_MASK                    (0xFFFFFFFFU)
#define SEMC_IPRXDAT_DAT_SHIFT                   (0U)
#define SEMC_IPRXDAT_DAT(x)                      (((uint32_t)(((uint32_t)(x)) << SEMC_IPRXDAT_DAT_SHIFT)) & SEMC_IPRXDAT_DAT_MASK)

/* Status register 0 */
#define SEMC_STS0_IDLE_MASK         (0x1U)
#define SEMC_STS0_IDLE_SHIFT        (0U)
#define SEMC_STS0_IDLE(x)           (((uint32_t)(((uint32_t)(x)) << SEMC_STS0_IDLE_SHIFT)) & SEMC_STS0_IDLE_MASK)
#define SEMC_STS0_NARDY_MASK        (0x2U)
#define SEMC_STS0_NARDY_SHIFT       (1U)
#define SEMC_STS0_NARDY(x)          (((uint32_t)(((uint32_t)(x)) << SEMC_STS0_NARDY_SHIFT)) & SEMC_STS0_NARDY_MASK)

/* SEMC memory device type. */
typedef enum _semc_mem_type {
    kSEMC_MemType_SDRAM = 0, /*!< SDRAM */
    kSEMC_MemType_SRAM,      /*!< SRAM */
    kSEMC_MemType_NOR,       /*!< NOR */
    kSEMC_MemType_NAND,      /*!< NAND */
    kSEMC_MemType_8080       /*!< 8080. */
} semc_mem_type_t;

/* SEMC IP command for NAND: address mode. */
typedef enum _semc_ipcmd_nand_addrmode {
    kSEMC_NANDAM_ColumnRow = 0x0U, /*!< Address mode: column and row address(5Byte-CA0/CA1/RA0/RA1/RA2). */
    kSEMC_NANDAM_ColumnCA0,        /*!< Address mode: column address only(1 Byte-CA0).  */
    kSEMC_NANDAM_ColumnCA0CA1,     /*!< Address mode: column address only(2 Byte-CA0/CA1). */
    kSEMC_NANDAM_RawRA0,           /*!< Address mode: row address only(1 Byte-RA0). */
    kSEMC_NANDAM_RawRA0RA1,        /*!< Address mode: row address only(2 Byte-RA0/RA1). */
    kSEMC_NANDAM_RawRA0RA1RA2      /*!< Address mode: row address only(3 Byte-RA0).  */
} semc_ipcmd_nand_addrmode_t;

/* SEMC IP command for NAND： command mode. */
typedef enum _semc_ipcmd_nand_cmdmode {
    kSEMC_NANDCM_AXICmdAddrRead = 0x0U, /*!< For AXI read. */
    kSEMC_NANDCM_AXICmdAddrWrite,       /*!< For AXI write.  */
    kSEMC_NANDCM_Command,               /*!< command. */
    kSEMC_NANDCM_CommandHold,           /*!< Command hold. */
    kSEMC_NANDCM_CommandAddress,        /*!< Command address. */
    kSEMC_NANDCM_CommandAddressHold,    /*!< Command address hold.  */
    kSEMC_NANDCM_CommandAddressRead,    /*!< Command address read.  */
    kSEMC_NANDCM_CommandAddressWrite,   /*!< Command address write.  */
    kSEMC_NANDCM_CommandRead,           /*!< Command read.  */
    kSEMC_NANDCM_CommandWrite,          /*!< Command write.  */
    kSEMC_NANDCM_Read,                  /*!< Read.  */
    kSEMC_NANDCM_Write                  /*!< Write.  */
} semc_ipcmd_nand_cmdmode_t;

struct imxrt_semc_nand_reg {
    volatile u32 * semc_mcr;
    volatile u32 * semc_iocr;
    volatile u32 * semc_br8;
    volatile u32 * semc_intr;
    volatile u32 * semc_nandcr0;
    volatile u32 * semc_nandcr1;
    volatile u32 * semc_nandcr2;
    volatile u32 * semc_nandcr3;
    volatile u32 * semc_ipcr0;
    volatile u32 * semc_ipcr1;
    volatile u32 * semc_ipcr2;
    volatile u32 * semc_ipcmd;
    volatile u32 * semc_iptxdat;
    volatile u32 * semc_iprxdat;
    volatile u32 * semc_sts0;
};

struct imxrt_semc_nand_param {
    u32 nandcr0;
    u32 nandcr1;
    u32 nandcr2;
    u32 nandcr3;
    u32 br8;
};

struct nand_chip_info {
    u32 page_size;
    u32 oob_size;
    u32 page_per_block;
    u32 ecc_size;
    u32 ecc_bytes;
    u32 base_address;
};

struct imxrt_semc_nand_info {
    struct udevice *dev;
    struct nand_chip nand;
    struct imxrt_semc_nand_reg semc_nand_regs;
    struct imxrt_semc_nand_param semc_nand_param;
    struct nand_chip_info chip_info;

    u8 *data_buf;
    u8 *oob_buf;
    bool spare_only;
    bool status_request;
    u16 col_addr;
    u32 page_addr;
    u8 status;
};

static int imxrt_semc_configure_ip_cmd(struct udevice * dev, u8 size_bytes)
{
    struct imxrt_semc_nand_info *platData = (struct imxrt_semc_nand_info *)dev_get_platdata(dev);
    if((size_bytes > SEMC_IPCOMMANDDATASIZEBYTEMAX) || (!size_bytes)) {
        return -EPERM;
    }

    *(platData->semc_nand_regs.semc_ipcr1) = SEMC_IPCR1_DATSZ(size_bytes);
    if (size_bytes < 4)
    {
        *(platData->semc_nand_regs.semc_ipcr2) |= SEMC_IPCR2_BM3_MASK;
    }
    if (size_bytes < 3)
    {
        *(platData->semc_nand_regs.semc_ipcr2) |= SEMC_IPCR2_BM2_MASK;
    }
    if (size_bytes < 2)
    {
        *(platData->semc_nand_regs.semc_ipcr2) |= SEMC_IPCR2_BM1_MASK;
    }
    return 0;
}

static int imxrt_semc_is_ip_cmd_done(struct udevice * dev)
{
    struct imxrt_semc_nand_info *platData = (struct imxrt_semc_nand_info *)dev_get_platdata(dev);

    /* Poll status bit till command is done*/
    while (!((*(platData->semc_nand_regs.semc_intr)) & SEMC_INTR_IPCMDDONE_MASK));

    /* Clear status bit */
    *(platData->semc_nand_regs.semc_intr) |= SEMC_INTR_IPCMDDONE_MASK;

    /* Check error status */
    if ((*(platData->semc_nand_regs.semc_intr)) & SEMC_INTR_IPCMDERR_MASK)
    {
        (*(platData->semc_nand_regs.semc_intr)) |= SEMC_INTR_IPCMDERR_MASK;
        return -EIO;
    }

    return 0;
}

static int imxrt_semc_send_ip_cmd(struct udevice * dev, semc_mem_type_t type,
        u32 address, u16 command, u32 write, u32 * read)
{
    struct imxrt_semc_nand_info *platData = (struct imxrt_semc_nand_info *)dev_get_platdata(dev);
    u32 cmdMode;
    bool readCmd;
    bool writeCmd;
    int ret;

    /* Clear status bit */
    *(platData->semc_nand_regs.semc_intr) |= SEMC_INTR_IPCMDDONE_MASK;
    /* Set address. */
    *(platData->semc_nand_regs.semc_ipcr0) = address;

    /* Check command mode. */
    cmdMode = command & 0xFU;
    readCmd = (cmdMode == kSEMC_NANDCM_AXICmdAddrRead) || (cmdMode == kSEMC_NANDCM_CommandAddressRead) ||
              (cmdMode == kSEMC_NANDCM_CommandRead) || (cmdMode == kSEMC_NANDCM_Read);
    writeCmd = (cmdMode == kSEMC_NANDCM_AXICmdAddrWrite) || (cmdMode == kSEMC_NANDCM_CommandAddressWrite) ||
               (cmdMode == kSEMC_NANDCM_CommandWrite) || (cmdMode == kSEMC_NANDCM_Write);

    if (writeCmd)
    {
        /* Set data. */
        *(platData->semc_nand_regs.semc_iptxdat) = write;
    }

    /* Set command code. */
    *(platData->semc_nand_regs.semc_ipcmd) = command | SEMC_IPCMD_KEY(SEMC_IPCOMMANDMAGICKEY);

    /* Wait for command done. */
    ret = imxrt_semc_is_ip_cmd_done(dev);
    if (ret)
    {
        return ret;
    }

    if (readCmd)
    {
        /* Get the read data */
        *read = *(platData->semc_nand_regs.semc_iprxdat);
    }

    return 0;
}

static inline u16 imxrt_semc_build_nand_ip_cmd(u8 userCommand,
                semc_ipcmd_nand_addrmode_t addrMode,
                semc_ipcmd_nand_cmdmode_t cmdMode)
{
    return ((u16)userCommand << 8) | ((u8)(addrMode << 4)) | ((u8)cmdMode & 0x0Fu);
}

static int imxrt_semc_ip_cmd_nand_write(struct udevice * dev,
        u32 address, u8 * data, u32 size_bytes)
{
    struct imxrt_semc_nand_info *platData = (struct imxrt_semc_nand_info *)dev_get_platdata(dev);
    int ret = 0;
    u16 ipCmd;
    u8 dataSize = (*(platData->semc_nand_regs.semc_nandcr0)) & SEMC_NANDCR0_PS_MASK;

    if(data == (u8 *)0) {
        return -EINVAL;
    }

    /* Configure IP command data size. */
    ret = imxrt_semc_configure_ip_cmd(dev, 1);
    if(ret) {
        dbgImxrtNand("%s: imxrt_semc_configure_ip_cmd(): %d\n", __func__, ret);
        return ret;
    }

    /* Write command built */
    ipCmd = imxrt_semc_build_nand_ip_cmd(0,
            kSEMC_NANDAM_ColumnRow,
            kSEMC_NANDCM_Write);
    while (size_bytes) {
        ret = imxrt_semc_send_ip_cmd(dev, kSEMC_MemType_NAND, address, ipCmd, (uint32_t)*data, NULL);
        if (ret) {
            break;
        }
        size_bytes--;
        data++;
    }

    ret = imxrt_semc_configure_ip_cmd(dev, dataSize);
    if(ret) {
        dbgImxrtNand("%s: imxrt_semc_configure_ip_cmd(): %d\n", __func__, ret);
        return ret;
    }

    return ret;
}

static int imxrt_semc_ip_cmd_nand_read(struct udevice *dev,
            u32 address, u8 * data, u32 size_bytes)
{
    struct imxrt_semc_nand_info *platData = (struct imxrt_semc_nand_info *)dev_get_platdata(dev);
    u8 dataSize = (*(platData->semc_nand_regs.semc_nandcr0)) & SEMC_NANDCR0_PS_MASK;
    u32 getData = 0;
    u16 ipCmd;
    u32 i = 0;
    int ret;

    if(data == (u8 *)0) {
        return -EINVAL;
    }
    /* Configure IP command data size */
    ret = imxrt_semc_configure_ip_cmd(dev, SEMC_IPCOMMANDDATASIZEBYTEMAX);
    if(ret) {
        dbgImxrtNand("%s (%d): imxrt_semc_configure_ip_cmd(ret=%d)\n", __func__, __LINE__, ret);
    }

    /* Read command built */
    ipCmd = imxrt_semc_build_nand_ip_cmd(0, kSEMC_NANDAM_ColumnRow, kSEMC_NANDCM_Read);

    while (size_bytes >= SEMC_IPCOMMANDDATASIZEBYTEMAX) {
        ret = imxrt_semc_send_ip_cmd(dev, kSEMC_MemType_NAND, address, ipCmd, 0, &getData);
        if (!ret) {
            for (i = 0; i < sizeof(getData); i++) {
                *data = getData & 0xFFU;
                getData >>= 8U;
                data++;
                size_bytes--;
            }
        } else {
            dbgImxrtNand("%s (%d): imxrt_semc_send_ip_cmd(ret=%d)\n", __func__, __LINE__, ret);
            break;
        }
    }

    if ((ret == 0) && size_bytes) {
        ret = imxrt_semc_configure_ip_cmd(dev, size_bytes);
        if(ret) {
            dbgImxrtNand("%s(%d): imxrt_semc_configure_ip_cmd(): %d\n", __func__, __LINE__, ret);
            return ret;
        }
        ret = imxrt_semc_send_ip_cmd(dev, kSEMC_MemType_NAND, address, ipCmd, 0, &getData);
        if (ret == 0) {
            for (i = 0; i < sizeof(getData); i++) {
                *data = getData & 0xFFU;
                getData >>= 8U;
                data++;
                size_bytes--;
            }
        } else {
            dbgImxrtNand("%s(%d): imxrt_semc_send_ip_cmd(): %d\n", __func__, __LINE__, ret);
        }
    }

    imxrt_semc_configure_ip_cmd(dev, dataSize);

    return 0;
}

static bool imxrt_semc_is_nand_ready(struct udevice * dev)
{
    struct imxrt_semc_nand_info *platData = (struct imxrt_semc_nand_info *)dev_get_platdata(dev);
    if(*(platData->semc_nand_regs.semc_sts0) & SEMC_STS0_NARDY_MASK) {
        return true;
    } else {
        return false;
    }
}

static int imxrt_semc_nand_read_buf(struct udevice * dev, u8 * buf, u32 len)
{
    return(imxrt_semc_ip_cmd_nand_read(dev, 0, buf, len));
}

static int imxrt_semc_nand_read_status(struct udevice * dev, u8 * status)
{
    int ret = 0;
    u32 readoutData;
    u16 commandCode;
    u32 slaveAddress;

    // Note: If there is only one plane per target, the READ STATUS (70h) command can be used
    //   to return status following any NAND command.
    // Note: In devices that have more than one plane per target, during and following interleaved
    //  die (multi-plane) operations, the READ STATUS ENHANCED command must be used to select the
    //  die (LUN) that should report status.

    // READ STATUS command is accepted by device even when it is busy (RDY = 0).
    commandCode = imxrt_semc_build_nand_ip_cmd(0x70, kSEMC_NANDAM_ColumnRow, kSEMC_NANDCM_CommandRead);
    // Note: For those command without address, the address should be valid as well,
    //  it shouldn't be out of IPG memory space, or SEMC IP will ignore this command.
    slaveAddress = CONFIG_SYS_NAND_BASE;

    ret = imxrt_semc_send_ip_cmd(dev, kSEMC_MemType_NAND, slaveAddress, commandCode, 0, &readoutData);
    if(ret) {
        *status = (u8)(readoutData & 0xFF);
    } else {
        *status = 0;
    }
    return(ret);
}

static int imxrt_semc_nand_reset(struct udevice *dev)
{
    int ret = 0;
    u32 dummyData = 0;

    dbgImxrtNand("Enter: %s\n", __func__);

    // The RESET command may be executed with the target in any state.
    u16 commandCode = imxrt_semc_build_nand_ip_cmd(0xFF, kSEMC_NANDAM_ColumnRow, kSEMC_NANDCM_CommandHold);
    ret = imxrt_semc_send_ip_cmd(dev, kSEMC_MemType_NAND, CONFIG_SYS_NAND_BASE, commandCode, 0, &dummyData);
    if(ret) {
        return ret;
    }
    // wait
    while(imxrt_semc_is_nand_ready(dev) != true);

    return 0;
}

static int imxrt_semc_nand_read_id(struct udevice * dev, u8 * buf)
{
    int ret = 0;
    u32 dummyData = 0;
    u32 slaveAddress;
    u16 commandCode;

    dbgImxrtNand("Enter: %s\n", __func__);

    // READ PAGE command is accepted by the device when it is ready (RDY = 1, ARDY = 1).
    while(imxrt_semc_is_nand_ready(dev) != true);

    commandCode = imxrt_semc_build_nand_ip_cmd(0x90U, kSEMC_NANDAM_ColumnCA0, kSEMC_NANDCM_CommandAddressHold);

    // Note: For those command without address, the address should be valid as well,
    //  it shouldn't be out of IPG memory space, or SEMC IP will ignore this command.
    slaveAddress = CONFIG_SYS_NAND_BASE;
    ret = imxrt_semc_send_ip_cmd(dev, kSEMC_MemType_NAND, slaveAddress, commandCode, 0, &dummyData);

    if (ret) {
        return ret;
    }

    while(imxrt_semc_is_nand_ready(dev) != true);

    // Get ID Bytes
    ret = imxrt_semc_ip_cmd_nand_read(dev, slaveAddress, buf, 5);
    if (ret) {
        return ret;
    }
#if(DEBUG_IMXRT_SEMC_NAND == 1)
    int i;
    dbgImxrtNand("%s: ", __func__);
    for(i = 0; i < 5; i++) {
        dbgImxrtNand("0x%02X ", buf[i]);
    }
    dbgImxrtNand("\n");
#endif
    return 0;
}

static int imxrt_semc_nand_read_page_data_oob(struct udevice * dev,
                u32 pageAddress, u8 * buf)
{
    u32 slaveAddress;
    u32 dummyData = 0;
    u16 commandCode;
    int ret = 0;
    struct imxrt_semc_nand_info *platData = (struct imxrt_semc_nand_info *)dev_get_platdata(dev);

    while(imxrt_semc_is_nand_ready(dev) != true);

    /* Load Page to Buffer */
    commandCode = imxrt_semc_build_nand_ip_cmd(
                    0x00U,
                    kSEMC_NANDAM_ColumnRow,
                    kSEMC_NANDCM_CommandAddress);
    slaveAddress = CONFIG_SYS_NAND_BASE +
            (pageAddress * platData->chip_info.page_size);
    ret = imxrt_semc_send_ip_cmd(dev, kSEMC_MemType_NAND, slaveAddress, commandCode, 0, &dummyData);
    if(ret) {
        return ret;
    }
    commandCode = imxrt_semc_build_nand_ip_cmd(
                    0x30U,
                    kSEMC_NANDAM_ColumnRow,
                    kSEMC_NANDCM_CommandHold);
    ret = imxrt_semc_send_ip_cmd(dev, kSEMC_MemType_NAND, slaveAddress, commandCode, 0, &dummyData);
    if(ret) {
        return ret;
    }

    while(imxrt_semc_is_nand_ready(dev) != true);
    ret = imxrt_semc_ip_cmd_nand_read(dev, slaveAddress,
            buf, platData->chip_info.page_size + platData->chip_info.oob_size);
    if(ret) {
        return ret;
    }

    return 0;
}

static int imxrt_semc_nand_erase(struct udevice * dev, u8 command, u32 page_addr)
{
    u16 commandCode;
    u32 slaveAddress;
    u32 dummyData = 0;
    int ret = 0;
    struct imxrt_semc_nand_info *platData = (struct imxrt_semc_nand_info *)dev_get_platdata(dev);

    if(page_addr >= 0) {
        commandCode = imxrt_semc_build_nand_ip_cmd(
                            command,
                            kSEMC_NANDAM_RawRA0RA1,
                            kSEMC_NANDCM_CommandAddress);
        slaveAddress = page_addr / platData->chip_info.page_per_block; // block
        slaveAddress = slaveAddress * platData->chip_info.page_per_block *
                platData->chip_info.page_size;
    } else {
        commandCode = imxrt_semc_build_nand_ip_cmd(
                            command,
                            kSEMC_NANDAM_ColumnRow,
                            kSEMC_NANDCM_CommandHold);
        slaveAddress = CONFIG_SYS_NAND_BASE; // dummy
    }
    ret = imxrt_semc_send_ip_cmd(dev, kSEMC_MemType_NAND, slaveAddress, commandCode, 0, &dummyData);
    if(ret) {
        return ret;
    }
    while(imxrt_semc_is_nand_ready(dev) != true);

    return 0;
}

static int imxrt_semc_nand_prog_page(struct udevice * dev,
                u32 page_addr, u32 column, u32 len, u8 * buf)
{
    u32 slaveAddress;
    u32 dummyData = 0;
    u16 commandCode;
    int ret = 0;
    struct imxrt_semc_nand_info *platData = (struct imxrt_semc_nand_info *)dev_get_platdata(dev);

    if((page_addr < 0) || (column < 0) || (buf == (uint8_t *)0)) {
        return -EINVAL;
    }

    while(imxrt_semc_is_nand_ready(dev) != true);

    commandCode = imxrt_semc_build_nand_ip_cmd(
                        0x80,
                        kSEMC_NANDAM_ColumnRow,
                        kSEMC_NANDCM_CommandAddressHold);
    slaveAddress = (page_addr * platData->chip_info.page_size) + CONFIG_SYS_NAND_BASE;
    ret = imxrt_semc_send_ip_cmd(dev, kSEMC_MemType_NAND, slaveAddress, commandCode, 0, &dummyData);
    if(ret) {
        return ret;
    }

    ret = imxrt_semc_ip_cmd_nand_write(dev, slaveAddress, buf, len);
    if(ret) {
        dbgImxrtNand("%s: imxrt_semc_ip_cmd_nand_write(): %d\n", __func__, ret);
        return ret;
    }
    commandCode = imxrt_semc_build_nand_ip_cmd(
                        0x10,
                        kSEMC_NANDAM_ColumnRow,
                        kSEMC_NANDCM_CommandHold);
    ret = imxrt_semc_send_ip_cmd(dev, kSEMC_MemType_NAND, slaveAddress, commandCode, 0, &dummyData);

    if(ret) {
        return ret;
    }

    while(imxrt_semc_is_nand_ready(dev) != true);

    return 0;
}

static void imxrt_nand_command(struct mtd_info *mtd, u32 command,
        int column, int page_addr)
{
    struct nand_chip * nand = mtd_to_nand(mtd);
    struct udevice * dev = (struct udevice *)nand_get_controller_data(nand);
    struct imxrt_semc_nand_info * platData = (struct imxrt_semc_nand_info *)dev_get_platdata(dev);
    int ret = 0;

    /* Reset command state information */
    platData->status_request = false;

    switch (command) {
    case NAND_CMD_RESET:
        ret = imxrt_semc_nand_reset(dev);
        if(ret) {
            dbgImxrtNand("%s(%d): NAND_CMD_RESET (%d)\n", __func__, __LINE__, ret);
        }
        break;

    case NAND_CMD_STATUS:
        platData->col_addr = 0;
        platData->status_request = true;
        ret = imxrt_semc_nand_read_status(dev, &(platData->status));
        if(ret) {
            dbgImxrtNand("%s(%d): NAND_CMD_STATUS (%d)\n", __func__, __LINE__, ret);
        }
        break;

    case NAND_CMD_READ0:
        platData->page_addr = page_addr;
        platData->col_addr = column;
        platData->spare_only = false;
        ret = imxrt_semc_nand_read_page_data_oob(dev, page_addr, platData->data_buf);
        if(ret) {
            dbgImxrtNand("%s(%d): NAND_CMD_READ0 (%d)\n", __func__, __LINE__, ret);
        }
        break;

    case NAND_CMD_READOOB:
        platData->page_addr = page_addr;
        platData->col_addr = column;
        platData->spare_only = true;
        ret = imxrt_semc_nand_read_page_data_oob(dev, page_addr, platData->data_buf);
        if(ret) {
            dbgImxrtNand("%s(%d): NAND_CMD_READOOB (%d)\n", __func__, __LINE__, ret);
        }
        break;

    case NAND_CMD_RNDOUT:
        if(column >= mtd->writesize) {
            /* Page OOB region */
            platData->col_addr = column - mtd->writesize;
            platData->spare_only = true;
        } else {
            /* Page Data region */
            platData->spare_only = false;
            platData->col_addr = column;
        }
        break;

    case NAND_CMD_SEQIN:
        // Read Page and OOB
        platData->page_addr = page_addr;
        ret = imxrt_semc_nand_read_page_data_oob(dev, page_addr, platData->data_buf);
        if(ret) {
            dbgImxrtNand("%s(%d): NAND_CMD_SEQIN (%d)\n", __func__, __LINE__, ret);
        }
        if (column >= mtd->writesize) {
            /* Page OOB region */
            platData->col_addr = column - mtd->writesize;
            platData->spare_only = true;
        } else {
            /* Page Data region */
            platData->spare_only = false;
            platData->col_addr = column;
        }
        break;

    case NAND_CMD_PAGEPROG:
        /* Program Page Data and OOB */
        ret = imxrt_semc_nand_prog_page(dev, platData->page_addr, 0, mtd->oobsize + mtd->writesize, platData->data_buf);
        if(ret) {
            dbgImxrtNand("%s(%d): NAND_CMD_PAGEPROG (%d)\n", __func__, __LINE__, ret);
        }
        break;

    case NAND_CMD_READID:
        platData->col_addr = 0;
        ret = imxrt_semc_nand_read_id(dev, platData->data_buf);
        if(ret) {
            dbgImxrtNand("%s(%d): NAND_CMD_READID (%d)\n", __func__, __LINE__, ret);
        }
        break;

    case NAND_CMD_ERASE1:
    case NAND_CMD_ERASE2:
        ret = imxrt_semc_nand_erase(dev, command, page_addr);
        if(ret) {
            dbgImxrtNand("%s(%d): NAND_CMD_ERASE1/2 (%d)\n", __func__, __LINE__, ret);
        }
        break;
    default:
        printf("imxrt_nand_command, unhandled command %X\n", command);
        break;
    }
}

static int imxrt_nand_dev_ready(struct mtd_info *mtd)
{
    struct nand_chip * nand = mtd_to_nand(mtd);
    struct udevice * dev = nand_get_controller_data(nand);
    return(imxrt_semc_is_nand_ready(dev));
}

static void imxrt_nand_select_chip(struct mtd_info *mtd, int chipnr)
{
    /// TODO:
    /// not used as this board uses one NAND chip
}

static u8 imxrt_read_byte(struct mtd_info *mtd)
{
    struct nand_chip * nand = mtd_to_nand(mtd);
    struct udevice * dev = (struct udevice *)nand_get_controller_data(nand);
    struct imxrt_semc_nand_info * platData = (struct imxrt_semc_nand_info *)dev_get_platdata(dev);
    u8 ret = 0;

    /* Check for status request */
    if(platData->status_request) {
        return(platData->status);
    }
    /* If we are accessing the spare region */
    if (platData->spare_only) {
        ret = platData->oob_buf[platData->col_addr];
    } else {
        ret = platData->data_buf[platData->col_addr];
    }

    dbgImxrtNand("%s: 0x%02X\n", __func__, ret);
    /* Update saved column address */
    platData->col_addr++;

    return ret;
}

static void imxrt_write_byte(struct mtd_info *mtd, uint8_t byte)
{
    struct nand_chip * nand = mtd_to_nand(mtd);
    struct udevice * dev = (struct udevice *)nand_get_controller_data(nand);
    struct imxrt_semc_nand_info *platData = (struct imxrt_semc_nand_info *)dev_get_platdata(dev);

    dbgImxrtNand("%s: 0x%02X\n", __func__, byte);
    platData->data_buf[platData->col_addr] = byte;

    /* Update saved column address */
    platData->col_addr++;

}

static void imxrt_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
    struct nand_chip * nand = mtd_to_nand(mtd);
    struct udevice * dev = (struct udevice *)nand_get_controller_data(nand);
    struct imxrt_semc_nand_info *platData = (struct imxrt_semc_nand_info *)dev_get_platdata(dev);
    uint32_t col = 0;
    uint8_t *p;

    col = platData->col_addr;

    if(platData->spare_only) {
        p = platData->oob_buf;
    } else {
        p = platData->data_buf;
    }

    dbgImxrtNand("%s: ", __func__);
    while(len > 0) {
        dbgImxrtNand("0x%02X ", p[col]);
        *buf = p[col];
        buf++;
        col++;
        len--;
    }
    dbgImxrtNand("\n");

    /* Update saved column address */
    platData->col_addr = col;
}

static void imxrt_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
    struct nand_chip * nand = mtd_to_nand(mtd);
    struct udevice * dev = (struct udevice *)nand_get_controller_data(nand);
    struct imxrt_semc_nand_info *platData = (struct imxrt_semc_nand_info *)dev_get_platdata(dev);
    int col = 0;

    col = platData->col_addr;

    dbgImxrtNand("%s: ", __func__);
    while(len > 0) {
        dbgImxrtNand("0x%02X ", *buf);
        platData->data_buf[col] = *buf;
        buf++;
        col++;
        len--;
    }
    dbgImxrtNand("\n");

    /* Update saved column address */
    platData->col_addr = col;
}

static int imxrt_semc_nand_init(struct udevice *dev)
{
    struct imxrt_semc_nand_info *platData = (struct imxrt_semc_nand_info *)dev_get_platdata(dev);
    u32 tempRegVal = 0;
    struct nand_chip * nand;

    /* Initialize SEMC NAND */
    *(platData->semc_nand_regs.semc_br8) = platData->semc_nand_param.br8;
    *(platData->semc_nand_regs.semc_nandcr0) = platData->semc_nand_param.nandcr0;
    *(platData->semc_nand_regs.semc_nandcr1) = platData->semc_nand_param.nandcr1;
    *(platData->semc_nand_regs.semc_nandcr2) = platData->semc_nand_param.nandcr2;
    *(platData->semc_nand_regs.semc_nandcr3) = platData->semc_nand_param.nandcr3;
    tempRegVal = *(platData->semc_nand_regs.semc_iocr);
    tempRegVal &= ~(0x38);  // Clear CS0X field
    tempRegVal |= 0x20;     // Set CS0X field to NAND CE#
    *(platData->semc_nand_regs.semc_iocr) = tempRegVal;

    nand = &(platData->nand);

    /* Initialize buffer */
    platData->data_buf = (u8 *)malloc(platData->chip_info.page_size + platData->chip_info.oob_size);
    if(platData->data_buf == (u8*)0) {
        return -ENOMEM;
    }
    platData->oob_buf = (u8 *)(platData->data_buf + platData->chip_info.page_size);

#ifdef CONFIG_SYS_NAND_USE_FLASH_BBT
    nand->bbt_options |= NAND_BBT_USE_FLASH | NAND_BBT_NO_OOB;
#endif
    nand_set_controller_data(nand, dev);

    if(platData->dev) {
        nand->flash_node = dev_of_offset(platData->dev);
    }

    nand->cmdfunc = imxrt_nand_command;
    nand->dev_ready = imxrt_nand_dev_ready;
    nand->select_chip = imxrt_nand_select_chip;
    nand->read_byte  = imxrt_read_byte;
    nand->write_byte = imxrt_write_byte;
    nand->read_buf   = imxrt_read_buf;
    nand->write_buf  = imxrt_write_buf;

    nand->ecc.mode = NAND_ECC_SOFT;
    nand->ecc.size = platData->chip_info.ecc_size;
    nand->ecc.layout = NULL; /* uses default ecc oob layout assigned on scan_tail */

    return 0;
}

static int imxrt_nand_ofdata_to_platdata(struct udevice *dev)
{
    struct imxrt_semc_nand_info *platData = dev_get_platdata(dev);
    fdt_addr_t addr;
    ofnode partition_node;
    int ret;

    dbgImxrtNand("Enter: %s\n\r", __func__);

    platData->dev = dev;
    addr = dev_read_addr(dev);
    if(addr == FDT_ADDR_T_NONE) {
        return -EINVAL;
    }
    /* SEMC MCR */
    platData->semc_nand_regs.semc_mcr = (u32 *)(addr + SEMC_MCR_REG_OFFSET);
    dbgImxrtNand("SEMC MCR addr: %08X\n\r", (u32)(platData->semc_nand_regs.semc_mcr));
    /* SEMC IOCR */
    platData->semc_nand_regs.semc_iocr = (u32 *)(addr + SEMC_IOCR_REG_OFFSET);
    dbgImxrtNand("SEMC IOCR addr: %08X\n\r", (u32)(platData->semc_nand_regs.semc_iocr));
    /* SEMC BR8 */
    platData->semc_nand_regs.semc_br8 =  (u32 *)(addr + SEMC_BR8_REG_OFFSET);
    dbgImxrtNand("SEMC BR8 addr: %08X\n\r", (u32)(platData->semc_nand_regs.semc_br8));
    /* SEMC INTR */
    platData->semc_nand_regs.semc_intr = (u32 *)(addr + SEMC_INTR_REG_OFFSET);
    dbgImxrtNand("SEMC INTR addr: %08X\n\r", (u32)(platData->semc_nand_regs.semc_intr));
    /* SEMC NANDCR */
    platData->semc_nand_regs.semc_nandcr0 = (u32 *)(addr + SEMC_NANDCR_REG_OFFSET + 0x0);
    dbgImxrtNand("SEMC NANDCR0 addr: %08X\n\r", (u32)(platData->semc_nand_regs.semc_nandcr0));
    platData->semc_nand_regs.semc_nandcr1 = (u32 *)(addr + SEMC_NANDCR_REG_OFFSET + 0x4);
    dbgImxrtNand("SEMC NANDCR1 addr: %08X\n\r", (u32)(platData->semc_nand_regs.semc_nandcr1));
    platData->semc_nand_regs.semc_nandcr2 = (u32 *)(addr + SEMC_NANDCR_REG_OFFSET + 0x8);
    dbgImxrtNand("SEMC NANDCR2 addr: %08X\n\r", (u32)(platData->semc_nand_regs.semc_nandcr2));
    platData->semc_nand_regs.semc_nandcr3 = (u32 *)(addr + SEMC_NANDCR_REG_OFFSET + 0xC);
    dbgImxrtNand("SEMC NANDCR3 addr: %08X\n\r", (u32)(platData->semc_nand_regs.semc_nandcr3));
    /* SEMC IP */
    platData->semc_nand_regs.semc_ipcr0 =  (u32 *)(addr + SEMC_IP_REG_OFFSET + 0x0);
    platData->semc_nand_regs.semc_ipcr1 =  (u32 *)(addr + SEMC_IP_REG_OFFSET + 0x4);
    platData->semc_nand_regs.semc_ipcr2 =  (u32 *)(addr + SEMC_IP_REG_OFFSET + 0x8);
    platData->semc_nand_regs.semc_ipcmd =  (u32 *)(addr + SEMC_IP_REG_OFFSET + 0xC);
    platData->semc_nand_regs.semc_iptxdat =  (u32 *)(addr + SEMC_IP_REG_OFFSET + 0x10);
    platData->semc_nand_regs.semc_iprxdat =  (u32 *)(addr + SEMC_IP_REG_OFFSET + 0x20);
    /* SEMC STS */
    platData->semc_nand_regs.semc_sts0 =  (u32 *)(addr + SEMC_STS_REG_OFFSET);

    /* NAND CHIP INFO */
    platData->chip_info.page_size = dev_read_u32_default(dev, "page-size", 2048);
    platData->chip_info.oob_size = dev_read_u32_default(dev, "oob-size", 64);
    platData->chip_info.page_per_block = dev_read_u32_default(dev, "page-per-block", 64);
    platData->chip_info.ecc_size = dev_read_u32_default(dev, "ecc-size", 512);
    platData->chip_info.ecc_bytes = dev_read_u32_default(dev, "ecc-bytes", 5);
    platData->chip_info.base_address = dev_read_u32_default(dev, "base-address", 0);
    dbgImxrtNand("NAND Layout: Page: %d, OOB: %d, PageCount: %d, ECC: %d %d, Base: %d\n\r",
            platData->chip_info.page_size,
            platData->chip_info.oob_size,
            platData->chip_info.page_per_block,
            platData->chip_info.ecc_size,
            platData->chip_info.ecc_bytes,
            platData->chip_info.base_address);

    /* Timing Parameter */
    ret = dev_read_u32_array(dev, "fsl,imxrt-nand-ctrl",
            &(platData->semc_nand_param.nandcr0), 5);
    if(ret) {
        return ret;
    }
    dbgImxrtNand("Timing parameter: %08X %08X %08X %08X %08X\n\r",
            platData->semc_nand_param.nandcr0,
            platData->semc_nand_param.nandcr1,
            platData->semc_nand_param.nandcr2,
            platData->semc_nand_param.nandcr3,
            platData->semc_nand_param.br8);

    return 0;
}

static int imxrt_nand_probe(struct udevice *dev)
{
    struct imxrt_semc_nand_info *platData = (struct imxrt_semc_nand_info *)dev_get_platdata(dev);
    struct nand_chip * nand = &(platData->nand);
    struct mtd_info * mtd = nand_to_mtd(nand);
    int ret = 0;
    dbgImxrtNand("Enter: %s\n\r", __func__);

    ret = imxrt_semc_nand_init(dev);
    if(ret) {
        return ret;
    }

    nand->IO_ADDR_R = (void __iomem *)(platData->chip_info.base_address);
    nand->IO_ADDR_W = (void __iomem *)(platData->chip_info.base_address);

    ret = nand_scan(mtd, 1);
    if(ret) {
        return ret;
    }

    ret = nand_register(0, mtd);
    if(ret) {
        return ret;
    }

    return 0;
}

static int imxrt_nand_remove(struct udevice * dev)
{
    struct imxrt_semc_nand_info * platData = (struct imxrt_semc_nand_info *)dev_get_platdata(dev);

    dbgImxrtNand("Enter: %s\n\r", __func__);

    if(platData->data_buf != (u8 *)0) {
        free((void *)(platData->data_buf));
    }

    return 0;
}

static const struct udevice_id imxrt_semc_nand_ids[] = {
    {
        .compatible = "fsl,imxrt-semc-nand",
        .data = 0,
    },
    { /* sentinel */ }
};

U_BOOT_DRIVER(imxrt_semc_nand) = {
    .name = "imxrt_semc_nand",
    .id = UCLASS_MTD,
    .of_match = imxrt_semc_nand_ids,
    .ofdata_to_platdata = imxrt_nand_ofdata_to_platdata,
    .probe = imxrt_nand_probe,
    .remove = imxrt_nand_remove,
    .platdata_auto_alloc_size = sizeof(struct imxrt_semc_nand_info),
};

void board_nand_init(void)
{
    struct udevice *dev;
    int ret;

    ret = uclass_get_device_by_driver(UCLASS_MTD,
                    DM_GET_DRIVER(imxrt_semc_nand),
                    &dev);
    if (ret && ret != -ENODEV) {
        pr_err("Failed to initialize iMXRT NAND controller. (error %d)\n", ret);
    }
}
