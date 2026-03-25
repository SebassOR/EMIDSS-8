/*
 * FlexIO_I2C.h
 *
 *  Created on: 25 abr. 2025
 *      Author: hecto
 */

#ifndef BSP_MCAL_FLEXIO_I2C_MCAL_FLEXIO_I2C_H_
#define BSP_MCAL_FLEXIO_I2C_MCAL_FLEXIO_I2C_H_

#include "Std_types.h"

#include "FlexIO_I2C_def.h"
#include "FlexIO_I2C_cfg.h"

extern void Flexio_Init_I2C (void);
extern void FlexIO_I2C_Transmit(uint8_t address, uint8_t *TXDataBuffer,uint8_t num_bytes);
extern void FlexIO_I2C_Receive(uint8_t address,uint8_t *RXDataBuffer, uint8_t num_bytes, bool Relbus);
extern void FlexIO_I2C_startCondition(void);
extern void FlexIO_I2C_stopCondition(void);

#endif /* BSP_MCAL_FLEXIO_I2C_MCAL_FLEXIO_I2C_H_ */
