/*
 * SPI_MCAL.h
 *
 *  Created on: Feb 5, 2024
 *      Author: jarsr
 */

#ifndef SPI_MCAL_H_
#define SPI_MCAL_H_

#include "S32K144W.h"

extern void SPI_vInitMaster(void);

extern uint16_t SPI_u16SendData(uint8_t u8SpiModule, uint16_t u16Data);
uint16_t SPI_u8Transmit(uint8_t u8SpiModule, uint8_t *data, uint16_t size);
uint16_t SPI_u8ReceiveData(uint8_t u8SpiModule, uint8_t* data, uint16_t size);
extern uint16_t SPI_u16ReciveData(uint8_t u8SpiModule, uint16_t* u16Data);
uint16_t SPI_CS(uint8_t u8SpiModule, uint8_t state);


#endif /* SPI_MCAL_H_ */
