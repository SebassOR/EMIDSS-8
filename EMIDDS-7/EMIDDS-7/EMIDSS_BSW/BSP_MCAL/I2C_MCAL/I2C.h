/*
 * I2C.h
 *
 *  Created on: Mar 15, 2024
 *  Author: JARS
 *  Version: 1.0v
 *  Ultima modif.: Jun 9, 2024
 *  Author: Noe Ortiz Terrones
 *
 */

#ifndef I2C_H_
#define I2C_H_

#include "I2C_cfg.h"
#include "I2C_def.h"
#include "GPIO_def.h"

#define MS8607_ADDRESS (0x76)
#define MS8607_RESET (0x1E)

extern void I2C_vInit(void);

extern Std_ReturnType I2C_u8WriteCmd(uint8_t u8SlaveAddr, uint8_t u8Cmd);

extern Std_ReturnType I2C0_u8Write(uint8_t u8SlaveAddr, uint8_t u8Register, uint8_t u8Data);

extern Std_ReturnType I2C0_u8Read(uint8_t u8SlaveAddr, uint8_t *pu8Data, uint8_t u8NumData);


#endif /* I2C_H_ */
