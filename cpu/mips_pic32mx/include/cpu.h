/*
 * Copyright 2016, Imagination Technologies Limited and/or its
 *                 affiliated group companies.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    cpu_mips_pic32mx MIPS PIC32MX
 * @ingroup     cpu
 * @brief       main CPU definitions for pic32mx devices.
 * @{
 *
 * @file
 * @brief       main CPU definitions for pic32mx devices.
 *
 * @author      Imagination Technologies
 */

#ifndef _CPU_H_
#define _CPU_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include "irq.h"

/**
 * @brief We run from flash on PIC32
 */
#define FLASH_XIP (1)

/**
 * @brief Stop the linker from throwing away the PIC32 config register settings
 * @{
 */
extern void dummy(void);
static inline void cpu_init_early(void)
{
	dummy();
}
/** @} */


/**
 * @brief   Print the last instruction's address
 *
 * @todo:   Not supported
 */
static inline void cpu_print_last_instruction(void)
{
    /* This function must exist else RIOT won't compile */
}

#ifdef __cplusplus
}
#endif

#endif
/** @} */

