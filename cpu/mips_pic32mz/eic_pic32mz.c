/*
 * Copyright(C) 2016,2017, Imagination Technologies Limited and/or its
 *                 affiliated group companies.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 */
#include <mips/cpu.h>
#include <mips/hal.h>
#include <assert.h>
#include "../mips32r2_common/include/irq.h"
#include "../mips32r2_common/include/uthash.h"

#include "board.h"

typedef struct {
    int irq_num;
    irq_fn_t *fn;
    UT_hash_handle hh;
}irq_table_t;

static irq_table_t* irq_table[IRQ_PRIO_MAX] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
static volatile int spurious_irqs = 0;

void irq_initialise(void)
{
#ifdef MIPS_MICROMIPS
    /* Ensure exceptions are encoded correctly! */
    mips32_set_c0(C0_CONFIG3, mips32_getconfig3() | CFG3_IOE);
#endif

    /* Disable all interrupts on controller */
    IEC0CLR = 0xffffffff;
    ((uint32_t *) & IEC0CLR)[4] = 0xffffffff;
    ((uint32_t *) & IEC0CLR)[8] = 0xffffffff;
    ((uint32_t *) & IEC0CLR)[12] = 0xffffffff;
    ((uint32_t *) & IEC0CLR)[16] = 0xffffffff;
    ((uint32_t *) & IEC0CLR)[20] = 0xffffffff;
    ((uint32_t *) & IEC0CLR)[24] = 0xffffffff;
    ((uint32_t *) & IEC0CLR)[28] = 0xffffffff;

    /* Enable but mask interrupts */
    // mips_bcssr(SR_BEV | SR_IMASK, SR_IE); ???

    /* Enable IRQ0 CPU Timer Interrupt */
    IEC0SET = _IEC0_CTIE_MASK;

    /* Set IRQ 0 to priority 1.0 */
    IPC0SET = 1 << _IPC0_CTIP_POSITION | 0 << _IPC0_CTIS_POSITION;
}

static int translate_irq(int irq_num)
{
    /* convert generic MIPS internal IRQs to PIC32IRQ NUMS */
    if (irq_num >= 0)             return irq_num;
    if (irq_num == EIC_IRQ_TIMER) return _CORE_TIMER_VECTOR;
    if (irq_num == EIC_IRQ_FDC)   return _CORE_FAST_DEBUG_CHAN_VECTOR;
    if (irq_num == EIC_IRQ_PC)    return _CORE_PERF_COUNT_VECTOR;
    return irq_num;
}

/**
 * @brief   Route an interrupt handle to the supplied IRQ number and enable it
 */
void irq_route(int irq_num, int priority, irq_fn_t* fn)
{
    /* assert(priority_grp < IRQ_PRIO_MAX);for now only support 1 prio level */
    assert(priority == 1);

    irq_num = translate_irq(irq_num);

    irq_disable(irq_num);

    irq_table_t* irq;
    HASH_FIND_INT(irq_table[priority], &irq_num, irq);
    if(irq == NULL)
    {
        irq = (irq_table_t*)malloc(sizeof(irq_table_t));
        irq->irq_num = irq_num;
        HASH_ADD_INT(irq_table[priority], irq_num, irq);
    }
    irq->fn = fn;

    /* Set IRQ priority */
    IPC0SET = (priority & 0x7) << _IPC0_CTIP_POSITION | 0 << _IPC0_CTIS_POSITION;

    irq_enable(irq_num);
}

/**
 * @brief   Enable an interrupt
 */
void irq_enable(int irq_num)
{
    irq_num = translate_irq(irq_num);
    uint32_t wordNum = irq_num >> 5;
    ((uint32_t *) & IEC0SET)[wordNum << 2] = 1 << (irq_num & 31);
}

/**
 * @brief   Disable an interrupt
 */
void irq_disable(int irq_num)
{
    irq_num = translate_irq(irq_num);
    uint32_t wordNum = irq_num >> 5;
    ((uint32_t *) & IEC0CLR)[wordNum << 2] = 1 << (irq_num & 31);
}

static void irq_ack(int irq_num)
{
    uint32_t wordNum = irq_num >> 5;
    ((uint32_t *) & IFS0CLR)[wordNum << 2] = 1 << (irq_num & 31);
}

static void irq_dispatch(int priority, int irq_num)
{
    irq_table_t* irq;

    HASH_FIND_INT(irq_table[priority], &irq_num, irq);
    if(irq == NULL) {
        spurious_irqs++;
    } else {
        irq->fn(irq_num);
    }
}

/*
 * This is a hack - currently the toolchain does not support correct placement
 * of EIC mode vectors (it is coming though) But we can support non-vectored EIC
 * mode and note the default PIC32 interrupt controller (which uses EIC +
 * MCU-ASE) defaults to non vectored mode anyway with all interrupts coming via
 * vector 0 which is equivalent to 'sw0' in 'VI' mode.
 *
 * Thus all EIC interrupts should be decoded here
 */
void __attribute__ ((interrupt("vector=sw0"), keep_interrupts_masked)) _mips_isr_sw0(void)
{
    register int cr = mips_getcr();
    register int irq_num;

    if (cr & CR_TI) {
        irq_num = _CORE_TIMER_VECTOR;
    } else if (cr & CR_RIPL) {
        /* Don't need to check RIPL number as we are using non vectored mode */
        irq_num = INTSTAT & _INTSTAT_SIRQ_MASK;
    } else {
        spurious_irqs++;
        return;
    }

    irq_dispatch(0,irq_num);
    irq_ack(irq_num);
}
