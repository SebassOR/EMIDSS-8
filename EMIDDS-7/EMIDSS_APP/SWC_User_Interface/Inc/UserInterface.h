#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

#include "UART_MCAL_cfg.h"
#include "UART_MCAL_def.h"
#include "UART_MCAL.h"
#include "ReadServices.h"

void Uart_Start(void);
void LPUART1_RxTx_IR(void);
void SendCommand(uint8_t Command[4]);

#endif /* UserInterface */
