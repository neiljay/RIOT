/*
 * Copyright 2016, Imagination Technologies Limited and/or its
 *                 affiliated group companies.
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <mips/m32c0.h>
#include "arch/irq_arch.h"


unsigned int irq_arch_enable(void)
{
	unsigned int status;
	asm volatile ("ei %0" : "=r"(status));
	return status;
}

unsigned int irq_arch_disable(void)
{
	unsigned int status;
	asm volatile ("di %0" : "=r"(status));
	return status;
}

void irq_arch_restore(unsigned int state)
{
	if (state & SR_IE) {
		mips32_bs_c0(C0_STATUS, SR_IE);
	} else {
		mips32_bc_c0(C0_STATUS, SR_IE);
	}
}

int irq_arch_in(void)
{
	return (mips32_get_c0(C0_STATUS) & SR_EXL) != 0;
}
