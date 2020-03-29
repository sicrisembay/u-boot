// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2020
 * Author(s): Sicris Rey Embay <sicris.embay@gmail.com>
 */

#include <common.h>
#include <dm.h>
#include <ram.h>
#include <spl.h>
#include <asm/io.h>
#include <asm/armv7m.h>

DECLARE_GLOBAL_DATA_PTR;

static void BOARD_WriteU32(uint32_t addr, uint32_t val)
{
    *((volatile uint32_t*)addr) = val;
}

static uint32_t BOARD_ReadU32(uint32_t addr)
{
    return((uint32_t)(*((volatile uint32_t*)addr)));
}

int board_early_init_f(void)
{
    /*
     * Fix: Explicitly disable sysTick
     * I'm not sure why it's sometimes enabled and later causes
     * __invalid_entry exception.
     */
    BOARD_WriteU32(0xE000E010, 0x00000004);

    return 0;
}


int dram_init(void)
{
#ifndef CONFIG_SUPPORT_SPL
	int rv;
	struct udevice *dev;

	rv = uclass_get_device(UCLASS_RAM, 0, &dev);
	if (rv) {
		debug("DRAM init failed: %d\n", rv);
		return rv;
	}

#endif
	return fdtdec_setup_mem_size_base();
}

int dram_init_banksize(void)
{
	return fdtdec_setup_memory_banksize();
}

#ifdef CONFIG_SPL_BUILD
#ifdef CONFIG_SPL_OS_BOOT
int spl_start_uboot(void)
{
	debug("SPL: booting kernel\n");
	/* break into full u-boot on 'c' */
	return serial_tstc() && serial_getc() == 'c';
}
#endif

int spl_dram_init(void)
{
	struct udevice *dev;
	int rv;

	rv = uclass_get_device(UCLASS_RAM, 0, &dev);
	if (rv)
		debug("DRAM init failed: %d\n", rv);
	return rv;
}

void spl_board_init(void)
{
	spl_dram_init();
	preloader_console_init();
	arch_cpu_init(); /* to configure mpu for sdram rw permissions */
}

u32 spl_boot_device(void)
{
	return BOOT_DEVICE_MMC1;
}
#endif

u32 get_board_rev(void)
{
	return 0;
}

int board_init(void)
{
	gd->bd->bi_boot_params = gd->bd->bi_dram[0].start + 0x100;

	return 0;
}
