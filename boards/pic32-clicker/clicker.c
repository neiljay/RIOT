/*
 * Copyright 2016, Imagination Technologies Limited and/or its
 *                 affiliated group companies.
 */
#include <stdio.h>
#include <stdint.h>
#include "bitarithm.h"
#include "board.h"


void board_init(void)
{
    /*
     * Setup pin mux for UART3 this is the one connected
     * to the mickroBUS
     */
    U3RXREG = 0x2;           /*connect pin RPF5 to UART3 RX*/
    RPF4R =   0x1;           /*connect pin RPF4 to UART3 TX*/
    PORTFCLR = BIT5 | BIT4;  /*set '0' on Porf F pins 4 and 5 */
    TRISFCLR = BIT4;         /*set PortF pin 4 for output */
    TRISFSET = BIT5;         /*set PortF pin 5 for input */
    ODCFCLR =  BIT5 | BIT4;  /*set PortF pin 4 and 5 as not open-drain */

    /* intialise UART used for debug (printf) */
#ifdef DEBUG_VIA_UART
    uart_init(DEBUG_VIA_UART, DEBUG_UART_BAUD, NULL, 0);
#endif
}

void reboot(void)
{
    /* TODO, note this is needed to get 'default' example to build */
}
