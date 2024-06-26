// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2007-2009  Freescale Semiconductor, Inc.
 * Copyright (C) 2008-2009  MontaVista Software, Inc.
 *
 * Authors: Tony Li <tony.li@freescale.com>
 *          Anton Vorontsov <avorontsov@ru.mvista.com>
 */

#include <clock_legacy.h>
#include <pci.h>
#include <mpc83xx.h>
#include <asm/global_data.h>
#include <asm/io.h>
#include <linux/delay.h>

DECLARE_GLOBAL_DATA_PTR;

#define PCIE_MAX_BUSES 2

static struct {
	u32 base;
	u32 size;
} mpc83xx_pcie_cfg_space[] = {
	{
		.base = CFG_SYS_PCIE1_CFG_BASE,
		.size = CFG_SYS_PCIE1_CFG_SIZE,
	},
#if defined(CFG_SYS_PCIE2_CFG_BASE) && defined(CFG_SYS_PCIE2_CFG_SIZE)
	{
		.base = CFG_SYS_PCIE2_CFG_BASE,
		.size = CFG_SYS_PCIE2_CFG_SIZE,
	},
#endif
};

int get_pcie_clk(int index)
{
	volatile immap_t *im = (immap_t *) CONFIG_SYS_IMMR;
	u32 pci_sync_in;
	u8 spmf;
	u8 clkin_div;
	u32 sccr;
	u32 csb_clk;
	u32 testval;

	clkin_div = ((im->clk.spmr & SPMR_CKID) >> SPMR_CKID_SHIFT);
	sccr = im->clk.sccr;
	pci_sync_in = get_board_sys_clk() / (1 + clkin_div);
	spmf = (im->clk.spmr & SPMR_SPMF) >> SPMR_SPMF_SHIFT;
	csb_clk = pci_sync_in * (1 + clkin_div) * spmf;

	if (index)
		testval = (sccr & SCCR_PCIEXP2CM) >> SCCR_PCIEXP2CM_SHIFT;
	else
		testval = (sccr & SCCR_PCIEXP1CM) >> SCCR_PCIEXP1CM_SHIFT;

	switch (testval) {
	case 0:
		return 0;
	case 1:
		return csb_clk;
	case 2:
		return csb_clk / 2;
	case 3:
		return csb_clk / 3;
	}

	return 0;
}

static void mpc83xx_pcie_init_bus(int bus, struct pci_region *reg)
{
	immap_t *immr = (immap_t *)CONFIG_SYS_IMMR;
	pex83xx_t *pex = &immr->pciexp[bus];
	struct pex_outbound_window *out_win;
	struct pex_inbound_window *in_win;
	void *hose_cfg_base;
	unsigned int ram_sz;
	unsigned int barl;
	unsigned int tar;
	u16 reg16;
	int i;

	/* Enable pex csb bridge inbound & outbound transactions */
	out_le32(&pex->bridge.pex_csb_ctrl,
		in_le32(&pex->bridge.pex_csb_ctrl) | PEX_CSB_CTRL_OBPIOE |
		PEX_CSB_CTRL_IBPIOE);

	/* Enable bridge outbound */
	out_le32(&pex->bridge.pex_csb_obctrl, PEX_CSB_OBCTRL_PIOE |
		PEX_CSB_OBCTRL_MEMWE | PEX_CSB_OBCTRL_IOWE |
		PEX_CSB_OBCTRL_CFGWE);

	out_win = &pex->bridge.pex_outbound_win[0];
	out_le32(&out_win->ar, PEX_OWAR_EN | PEX_OWAR_TYPE_CFG |
			mpc83xx_pcie_cfg_space[bus].size);
	out_le32(&out_win->bar, mpc83xx_pcie_cfg_space[bus].base);
	out_le32(&out_win->tarl, 0);
	out_le32(&out_win->tarh, 0);

	for (i = 0; i < 2; i++) {
		u32 ar;

		if (reg[i].size == 0)
			break;

		out_win = &pex->bridge.pex_outbound_win[i + 1];
		out_le32(&out_win->bar, reg[i].phys_start);
		out_le32(&out_win->tarl, reg[i].bus_start);
		out_le32(&out_win->tarh, 0);
		ar = PEX_OWAR_EN | (reg[i].size & PEX_OWAR_SIZE);
		if (reg[i].flags & PCI_REGION_IO)
			ar |= PEX_OWAR_TYPE_IO;
		else
			ar |= PEX_OWAR_TYPE_MEM;
		out_le32(&out_win->ar, ar);
	}

	out_le32(&pex->bridge.pex_csb_ibctrl, PEX_CSB_IBCTRL_PIOE);

	ram_sz = gd->ram_size;
	barl = 0;
	tar = 0;
	i = 0;
	while (ram_sz > 0) {
		in_win = &pex->bridge.pex_inbound_win[i];
		out_le32(&in_win->barl, barl);
		out_le32(&in_win->barh, 0x0);
		out_le32(&in_win->tar, tar);
		if (ram_sz >= 0x10000000) {
			/* The maxium windows size is 256M */
			out_le32(&in_win->ar, PEX_IWAR_EN | PEX_IWAR_NSOV |
				PEX_IWAR_TYPE_PF | 0x0FFFF000);
			barl += 0x10000000;
			tar += 0x10000000;
			ram_sz -= 0x10000000;
		} else {
			/* The UM  is not clear here.
			 * So, round up to even Mb boundary */

			ram_sz = ram_sz >> (20 +
					((ram_sz & 0xFFFFF) ? 1 : 0));
			if (!(ram_sz % 2))
				ram_sz -= 1;
			out_le32(&in_win->ar, PEX_IWAR_EN | PEX_IWAR_NSOV |
				PEX_IWAR_TYPE_PF | (ram_sz << 20) | 0xFF000);
			ram_sz = 0;
		}
		i++;
	}

	in_win = &pex->bridge.pex_inbound_win[i];
	out_le32(&in_win->barl, CONFIG_SYS_IMMR);
	out_le32(&in_win->barh, 0);
	out_le32(&in_win->tar, CONFIG_SYS_IMMR);
	out_le32(&in_win->ar, PEX_IWAR_EN |
		PEX_IWAR_TYPE_NO_PF | PEX_IWAR_SIZE_1M);

	/* Enable the host virtual INTX interrupts */
	out_le32(&pex->bridge.pex_int_axi_misc_enb,
		in_le32(&pex->bridge.pex_int_axi_misc_enb) | 0x1E0);

	/* Hose configure header is memory-mapped */
	hose_cfg_base = (void *)pex;

	/* Configure the PCIE controller core clock ratio */
	out_le32(hose_cfg_base + PEX_GCLK_RATIO,
		((get_pcie_clk(bus) / 1000000) * 16) / 333);
	udelay(1000000);

	/* Do Type 1 bridge configuration */
	out_8(hose_cfg_base + PCI_PRIMARY_BUS, 0);
	out_8(hose_cfg_base + PCI_SECONDARY_BUS, 1);
	out_8(hose_cfg_base + PCI_SUBORDINATE_BUS, 255);

	/*
	 * Write to Command register
	 */
	reg16 = in_le16(hose_cfg_base + PCI_COMMAND);
	reg16 |= PCI_COMMAND_MASTER | PCI_COMMAND_MEMORY | PCI_COMMAND_IO |
			PCI_COMMAND_SERR | PCI_COMMAND_PARITY;
	out_le16(hose_cfg_base + PCI_COMMAND, reg16);

	/*
	 * Clear non-reserved bits in status register.
	 */
	out_le16(hose_cfg_base + PCI_STATUS, 0xffff);
	out_8(hose_cfg_base + PCI_LATENCY_TIMER, 0x80);
	out_8(hose_cfg_base + PCI_CACHE_LINE_SIZE, 0x08);

	printf("PCIE%d: ", bus);

#define PCI_LTSSM	0x404 /* PCIe Link Training, Status State Machine */
#define PCI_LTSSM_L0	0x16 /* L0 state */
	reg16 = in_le16(hose_cfg_base + PCI_LTSSM);
	if (reg16 >= PCI_LTSSM_L0)
		printf("link\n");
	else
		printf("No link\n");
}

/*
 * The caller must have already set SCCR, SERDES and the PCIE_LAW BARs
 * must have been set to cover all of the requested regions.
 */
void mpc83xx_pcie_init(int num_buses, struct pci_region **reg)
{
	int i;

	/*
	 * Release PCI RST Output signal.
	 * Power on to RST high must be at least 100 ms as per PCI spec.
	 * On warm boots only 1 ms is required, but we play it safe.
	 */
	udelay(100000);

	if (num_buses > ARRAY_SIZE(mpc83xx_pcie_cfg_space)) {
		printf("Second PCIE host contoller not configured!\n");
		num_buses = ARRAY_SIZE(mpc83xx_pcie_cfg_space);
	}

	for (i = 0; i < num_buses; i++)
		mpc83xx_pcie_init_bus(i, reg[i]);
}
