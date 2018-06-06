/*
 * Copyright 2018, Imagination Technologies Limited and/or its
 *                 affiliated group companies.
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup        cpu_mips32r2_common Imagination Technologies MIPS32R2 Common
 * @ingroup         cpu
 * @brief           Imagination Technologies MIPS32R2 Common implementation
 * @{
 *
 * @file
 * @brief       API for supporting Interrupt Controllers
 *
 * @author      Neil Jones <neil.jones@imgtec.com>
 */

#ifndef IRQ_H
#define IRQ_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ brief Internal Interrupt numbers
 *
 * MIPS cores have a few internally generated interrupts from the Timer,
 * Performance Counters and Fast Debug Channel hardware, in EIC mode these
 * become outputs from the core and are connected to the external controller,
 * the external control then loops these back at whichever IPL it decides
 *
 * We use negative numbers to represent these, leaving positive numbers free for
 * the SoC specific interrupts
 * @{
 */
#define EIC_IRQ_TIMER (-1)
#define EIC_IRQ_FDC   (-2)
#define EIC_IRQ_PC    (-3)

#define IRQ_PRIO_1 1
#define IRQ_PRIO_MAX 8

typedef void(irq_fn_t)(int);
/** @} */

/**
 * @brief   initialise the interrupt module
 */
void irq_initialise(void);

/**
 * @brief   Route (+ enable) an interrupt
 */
void irq_route(int irq_num, int priority, irq_fn_t* fn);

/**
 * @brief   Enable an interrupt
 */
void irq_enable(int irq_num);

/**
 * @brief   Disable an interrupt
 */
void irq_disable(int irq_num);

#ifdef __cplusplus
}
#endif

#endif /* IRQ_H */
/** @} */
