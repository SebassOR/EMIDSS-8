/*
 * UART_MCAL.h
 *
 *  Created on: Feb 1, 2024
 *      Author: jarsr
 */

#ifndef UART_MCAL_H_
#define UART_MCAL_H_

static LPUART_Type * const s_lpuartBase[LPUART_INSTANCE_COUNT] = LPUART_BASE_PTRS;

extern void UART_vInit(void);

extern uint8 UART_u8SendData(uint8 u8Module, uint8 u8Data);

extern uint8 UART_vSendBuffer(uint8 u8Module, uint8 u8Buffer[], uint32 lenght);

extern uint8 UART_u8SendString(uint8 u8Module, uint8 u8DataBuffer[]);

void UART_u8ReceiveData(uint8 , uint8 *);

void UART_vInitIRQs(void);

#endif /* UART_MCAL_UART_MCAL_H_ */
