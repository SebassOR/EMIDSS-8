/*
 * UART_MCAL_cfg.h
 *
 *  Created on: Feb 1, 2024
 *      Author: jarsr
 */

#ifndef UART_MCAL_CFG_H_
#define UART_MCAL_CFG_H_

/* System Headers*/
#include "Std_Types.h"

/*
 * Revisar la seccion de Referencial Manual 53.3.1.1 LPUART memory map.
 */

/** Peripheral LPUART0 base address */
#define UART0_BASE_ADDR_cfg				(0x4006A000U)

/** Peripheral LPUART1 base address */
#define UART1_BASE_ADDR_cfg				(0x4006B000U)

/** Peripheral LPUART2 base address */
#define UART2_BASE_ADDR_cfg				(0x4006C000U)

void UART_vPauseIRQs(void);
void UART_vResumeIRQs(void);
#endif /* UART_MCAL_CFG_H_ */
