// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2022 StarFive Technology Co., Ltd.
 * Author: Yanhong Wang<yanhong.wang@starfivetech.com>
 */

#include <linux/kernel.h>
#include <asm/io.h>

static const  u32 ddr_phy_data[] = {
	0x4f0,
	0x0,
	0x1030200,
	0x0,
	0x0,
	0x3000000,
	0x1000001,
	0x3000400,
	0x1000001,
	0x0,
	0x0,
	0x1000001,
	0x0,
	0xc00004,
	0xcc0008,
	0x660601,
	0x3,
	0x0,
	0x1,
	0xaaaa,
	0x5555,
	0xb5b5,
	0x4a4a,
	0x5656,
	0xa9a9,
	0xa9a9,
	0xb5b5,
	0x0,
	0x0,
	0x8000000,
	0x4000008,
	0x408,
	0xe4e400,
	0x71020,
	0xc0020,
	0x620,
	0x100,
	0x55555555,
	0xaaaaaaaa,
	0x55555555,
	0xaaaaaaaa,
	0x5555,
	0x1000100,
	0x800180,
	0x1,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x4,
	0x20,
	0x0,
	0x0,
	0x0,
	0x0,
	0x7ff0000,
	0x20008008,
	0x810,
	0x40100,
	0x0,
	0x1880c01,
	0x2003880c,
	0x20000125,
	0x7ff0200,
	0x101,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x20000,
	0x51515052,
	0x31c06000,
	0x11f0004,
	0xc0c001,
	0x3000000,
	0x30202,
	0x42100010,
	0x10c053e,
	0xf0c20,
	0x1000140,
	0xa30120,
	0xc00,
	0x210,
	0x200,
	0x2800000,
	0x80800101,
	0x3,
	0x76543210,
	0x8,
	0x2800280,
	0x2800280,
	0x2800280,
	0x2800280,
	0x280,
	0x8000,
	0x800080,
	0x800080,
	0x800080,
	0x800080,
	0x800080,
	0x800080,
	0x800080,
	0x800080,
	0x6e0080,
	0x1a00003,
	0x0,
	0x30000,
	0x80200,
	0x0,
	0x20202020,
	0x20202020,
	0x2020,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x4f0,
	0x0,
	0x1030200,
	0x0,
	0x0,
	0x3000000,
	0x1000001,
	0x3000400,
	0x1000001,
	0x0,
	0x0,
	0x1000001,
	0x0,
	0xc00004,
	0xcc0008,
	0x660601,
	0x3,
	0x0,
	0x1,
	0xaaaa,
	0x5555,
	0xb5b5,
	0x4a4a,
	0x5656,
	0xa9a9,
	0xa9a9,
	0xb5b5,
	0x0,
	0x0,
	0x8000000,
	0x4000008,
	0x408,
	0xe4e400,
	0x71020,
	0xc0020,
	0x620,
	0x100,
	0x55555555,
	0xaaaaaaaa,
	0x55555555,
	0xaaaaaaaa,
	0x5555,
	0x1000100,
	0x800180,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x4,
	0x20,
	0x0,
	0x0,
	0x0,
	0x0,
	0x7ff0000,
	0x20008008,
	0x810,
	0x40100,
	0x0,
	0x1880c01,
	0x2003880c,
	0x20000125,
	0x7ff0200,
	0x101,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x20000,
	0x51515052,
	0x31c06000,
	0x11f0004,
	0xc0c001,
	0x3000000,
	0x30202,
	0x42100010,
	0x10c053e,
	0xf0c20,
	0x1000140,
	0xa30120,
	0xc00,
	0x210,
	0x200,
	0x2800000,
	0x80800101,
	0x3,
	0x76543210,
	0x8,
	0x2800280,
	0x2800280,
	0x2800280,
	0x2800280,
	0x280,
	0x8000,
	0x800080,
	0x800080,
	0x800080,
	0x800080,
	0x800080,
	0x800080,
	0x800080,
	0x800080,
	0x6e0080,
	0x1a00003,
	0x0,
	0x30000,
	0x80200,
	0x0,
	0x20202020,
	0x20202020,
	0x2020,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x4f0,
	0x0,
	0x1030200,
	0x0,
	0x0,
	0x3000000,
	0x1000001,
	0x3000400,
	0x1000001,
	0x0,
	0x0,
	0x1000001,
	0x0,
	0xc00004,
	0xcc0008,
	0x660601,
	0x3,
	0x0,
	0x1,
	0xaaaa,
	0x5555,
	0xb5b5,
	0x4a4a,
	0x5656,
	0xa9a9,
	0xa9a9,
	0xb5b5,
	0x0,
	0x0,
	0x8000000,
	0x4000008,
	0x408,
	0xe4e400,
	0x71020,
	0xc0020,
	0x620,
	0x100,
	0x55555555,
	0xaaaaaaaa,
	0x55555555,
	0xaaaaaaaa,
	0x5555,
	0x1000100,
	0x800180,
	0x1,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x4,
	0x20,
	0x0,
	0x0,
	0x0,
	0x0,
	0x7ff0000,
	0x20008008,
	0x810,
	0x40100,
	0x0,
	0x1880c01,
	0x2003880c,
	0x20000125,
	0x7ff0200,
	0x101,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x20000,
	0x51515052,
	0x31c06000,
	0x11f0004,
	0xc0c001,
	0x3000000,
	0x30202,
	0x42100010,
	0x10c053e,
	0xf0c20,
	0x1000140,
	0xa30120,
	0xc00,
	0x210,
	0x200,
	0x2800000,
	0x80800101,
	0x3,
	0x76543210,
	0x8,
	0x2800280,
	0x2800280,
	0x2800280,
	0x2800280,
	0x280,
	0x8000,
	0x800080,
	0x800080,
	0x800080,
	0x800080,
	0x800080,
	0x800080,
	0x800080,
	0x800080,
	0x6e0080,
	0x1a00003,
	0x0,
	0x30000,
	0x80200,
	0x0,
	0x20202020,
	0x20202020,
	0x2020,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x4f0,
	0x0,
	0x1030200,
	0x0,
	0x0,
	0x3000000,
	0x1000001,
	0x3000400,
	0x1000001,
	0x0,
	0x0,
	0x1000001,
	0x0,
	0xc00004,
	0xcc0008,
	0x660601,
	0x3,
	0x0,
	0x1,
	0xaaaa,
	0x5555,
	0xb5b5,
	0x4a4a,
	0x5656,
	0xa9a9,
	0xa9a9,
	0xb5b5,
	0x0,
	0x0,
	0x8000000,
	0x4000008,
	0x408,
	0xe4e400,
	0x71020,
	0xc0020,
	0x620,
	0x100,
	0x55555555,
	0xaaaaaaaa,
	0x55555555,
	0xaaaaaaaa,
	0x5555,
	0x1000100,
	0x800180,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x4,
	0x20,
	0x0,
	0x0,
	0x0,
	0x0,
	0x7ff0000,
	0x20008008,
	0x810,
	0x40100,
	0x0,
	0x1880c01,
	0x2003880c,
	0x20000125,
	0x7ff0200,
	0x101,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x20000,
	0x51515052,
	0x31c06000,
	0x11f0004,
	0xc0c001,
	0x3000000,
	0x30202,
	0x42100010,
	0x10c053e,
	0xf0c20,
	0x1000140,
	0xa30120,
	0xc00,
	0x210,
	0x200,
	0x2800000,
	0x80800101,
	0x3,
	0x76543210,
	0x8,
	0x2800280,
	0x2800280,
	0x2800280,
	0x2800280,
	0x280,
	0x8000,
	0x800080,
	0x800080,
	0x800080,
	0x800080,
	0x800080,
	0x800080,
	0x800080,
	0x800080,
	0x6e0080,
	0x1a00003,
	0x0,
	0x30000,
	0x80200,
	0x0,
	0x20202020,
	0x20202020,
	0x2020,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x100,
	0x200,
	0x0,
	0x0,
	0x0,
	0x0,
	0x400000,
	0x80,
	0xdcba98,
	0x3000000,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x2a,
	0x15,
	0x15,
	0x2a,
	0x33,
	0xc,
	0xc,
	0x33,
	0xa418820,
	0x3f0000,
	0x13f,
	0x20202000,
	0x202020,
	0x20008008,
	0x810,
	0x0,
	0x255,
	0x30000,
	0x300,
	0x300,
	0x300,
	0x300,
	0x300,
	0x42080010,
	0x33e,
	0x1010002,
	0x80,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x100,
	0x200,
	0x0,
	0x0,
	0x0,
	0x0,
	0x400000,
	0x80,
	0xdcba98,
	0x3000000,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x2a,
	0x15,
	0x15,
	0x2a,
	0x33,
	0xc,
	0xc,
	0x33,
	0x0,
	0x0,
	0x0,
	0x20202000,
	0x202020,
	0x20008008,
	0x810,
	0x0,
	0x255,
	0x30000,
	0x300,
	0x300,
	0x300,
	0x300,
	0x300,
	0x42080010,
	0x33e,
	0x1010002,
	0x80,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x100,
	0x200,
	0x0,
	0x0,
	0x0,
	0x0,
	0x400000,
	0x80,
	0xdcba98,
	0x3000000,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x2a,
	0x15,
	0x15,
	0x2a,
	0x33,
	0xc,
	0xc,
	0x33,
	0x0,
	0x10000000,
	0x0,
	0x20202000,
	0x202020,
	0x20008008,
	0x810,
	0x0,
	0x255,
	0x30000,
	0x300,
	0x300,
	0x300,
	0x300,
	0x300,
	0x42080010,
	0x33e,
	0x1010002,
	0x80,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x100,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x50000,
	0x4000000,
	0x55,
	0x0,
	0x0,
	0x0,
	0xf0001,
	0x280040,
	0x5002,
	0x10101,
	0x8008,
	0x81020,
	0x0,
	0x0,
	0x1000000,
	0x1,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x64,
	0x0,
	0x0,
	0x1010000,
	0x2020101,
	0x4040202,
	0x8080404,
	0xf0f0808,
	0xf0f0f0f,
	0x20200f0f,
	0x1b428000,
	0x4,
	0x1010000,
	0x1070501,
	0x54,
	0x4410,
	0x4410,
	0x4410,
	0x4410,
	0x4410,
	0x4410,
	0x4410,
	0x4410,
	0x4410,
	0x4410,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x64,
	0x0,
	0x108,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x3000000,
	0x0,
	0x0,
	0x0,
	0x4102035,
	0x41020,
	0x1c98c98,
	0x3f400000,
	0x3f3f1f3f,
	0x1f3f3f1f,
	0x1f3f3f,
	0x0,
	0x0,
	0x1,
	0x0,
	0x0,
	0x0,
	0x0,
	0x76543210,
	0x6010198,
	0x0,
	0x0,
	0x0,
	0x40700,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x2,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x1142,
	0x3020100,
	0x3000300,
	0x3000300,
	0x3000300,
	0x3000300,
	0x3000300,
	0x3000300,
	0x3000300,
	0x3000300,
	0x3000300,
	0x3000300,
	0x300,
	0x300,
	0x300,
	0x300,
	0x2,
	0x4011,
	0x4011,
	0x40,
	0x40,
	0x4011,
	0x1fff00,
	0x4011,
	0x4011,
	0x4011,
	0x4011,
	0x4011,
	0x4011,
	0x4011,
	0x4011,
	0x4011,
	0x4011,
	0x4011,
	0x1004011,
	0x200400,

};

void ddr_phy_util(u32 *phyreg)
{
	u32 i, len;

	len = ARRAY_SIZE(ddr_phy_data);
	for (i = 1792; i < len; i++)
		out_le32(phyreg + i, ddr_phy_data[i]);

	for (i = 0; i < 1792; i++)
		out_le32(phyreg + i, ddr_phy_data[i]);
}
