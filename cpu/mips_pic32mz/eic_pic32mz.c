/*
 * Copyright(C) 2016,2017, Imagination Technologies Limited and/or its
 *                 affiliated group companies.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 */
#include <assert.h>

#include "board.h"
#include "eic_irq.h"


extern char __isr_vec_space __attribute__((section("data")));

static irq_table_t* irq_table[IRQ_PRIO_MAX] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};




void irq_initialise()
{

#ifdef MIPS_MICROMIPS
    /* Ensure exceptions are encoded correctly! */
    _m32c0_mtc0(C0_CONFIG, 3, mips32_getconfig3() | CFG3_IOE);
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
}


void irq_route(int num, int priority_grp, irq_fn_t* fn)
{
    assert(priority_grp < IRQ_PRIO_MAX);

    irq_disable(num);

    irq_table_t* irq;
    HASH_FIND_INT(irq_table[priority_grp], &num, irq);
    if(irq == NULL)
    {
        irq = (irq_table_t*)malloc(sizeof(irq_table_t));
        irq->irq_num = num;
        HASH_ADD_INT(irq_table[priority], irq_num, irq);
    }
    irq->fn = fn;

    irq_enable(num);
}

/**
 * @brief   Enable an interrupt
 */
void irq_enable(int irq_num)
{
    uint32_t wordNum = irq_num >> 5;
    ((uint32_t *) & IEC0SET)[wordNum << 2] = 1 << (irq_num & 31);
}

/**
 * @brief   Disable an interrupt
 */
void irq_disable(int irq_num)
{
    uint32_t wordNum = irq_num >> 5;
    ((uint32_t *) & IEC0CLR)[wordNum << 2] = 1 << (irq_num & 31);
}

/**
 * @brief   Acknowledge an interrupt
 */
void irq_ack(int irq_num)
{

}

void irq_dispatch(int priority_grp)
{
    irq_table_t* irq;

    HASH_FIND_INT(irq_table[priority_grp], &num, irq);
    if(irq != NULL)
        irq->fn();
}



void eic_irq_configure(int irq_num)
{
    (void)irq_num;
    /* Only timer interrupt supported currently */
    assert(irq_num == EIC_IRQ_TIMER);

    /* Enable IRQ0 CPU Timer Interrupt */
    IEC0SET = _IEC0_CTIE_MASK;

    /* Set IRQ 0 to priority 1.0 */
    IPC0SET = 1 << _IPC0_CTIP_POSITION | 0 << _IPC0_CTIS_POSITION;
}

void eic_irq_enable(int irq_num)
{
    (void)irq_num;
    /* Only timer interrupt supported currently */
    assert(irq_num == EIC_IRQ_TIMER);

    /* Enable IRQ0 CPU Timer Interrupt */
    IEC0SET = _IEC0_CTIE_MASK;
}

void eic_irq_disable(int irq_num)
{
    (void)irq_num;
    /* Only timer interrupt supported currently */
    assert(irq_num == EIC_IRQ_TIMER);

    /* Disable IRQ0 CPU Timer Interrupt */
    IEC0CLR = _IEC0_CTIE_MASK;
}

void eic_irq_ack(int irq_num)
{
    (void)irq_num;
    /* Only timer interrupt supported currently */
    assert(irq_num == EIC_IRQ_TIMER);

    /* Ack the timer interrupt */
    IFS0CLR =_IFS0_CTIF_MASK;
}
