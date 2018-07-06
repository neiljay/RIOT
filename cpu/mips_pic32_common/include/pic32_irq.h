/*
 * Copyright(C) 2017, Imagination Technologies Limited and/or its
 *                 affiliated group companies.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 */

/**
 * @ingroup         cpu_mips_pic32_common
 * @{
 *
 * @file
 * @brief           PIC32 interrupt handling API
 *
 * @author          Neil Jones <neil.jones@imgtec.com>
 */

#ifndef PIC32_IRQ_H
#define PIC32_IRQ_H

#ifdef __cplusplus
extern "C" {
#endif

//typedef void(*irq_fn_t)() ;

typedef struct irq_table {
    int irq_num;
    irq_fn_t fn;
    UT_hash_handle hh;
} irq_table_t;

/**
 * @brief   initialise the interrupt subsystem.
 */
//void irq_initialise();

/**
 * @brief   Setup a handler
 */
//void irq_route(int num, int priority, irq_fn_t* fn);

/**
 * @brief   Enable an interrupt
 */
//void irq_enable(int irq_num);

/**
 * @brief   Disable an interrupt
 */
//void irq_disable(int irq_num);

/**
 * @brief   Acknowledge an interrupt
 */
//void irq_ack(int irq_num);

#ifdef __cplusplus
}
#endif

#endif /* PIC32_IRQ_H */
/** @} */
