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
#include "Sensor_Data.h"
#include "Std_Types.h"

#define MS8607_ADDRESS (0x76)
#define MS8607_RESET (0x1E)



/* Define Instances */
#define I2C_INSTANCE_0    (0U) /* LPI2C1 Hw Channel for I2C0  Sensor Is connected to I2C0*/
#define I2C_INSTANCE_1    (1U) /* LPI2C0 Hw Channel for I2C1  Iridium modem is connected to I2C1 */
#define I2C_TIMEOUT       (10000U) /* 100ms */



extern void I2C0_vInit(void);
extern void I2C1_vInit(void);

extern Std_ReturnType I2C0_u8WriteCmd(uint8_t u8SlaveAddr, uint8_t* u8Cmd);

extern Std_ReturnType I2C0_u8Write(uint8_t u8SlaveAddr, uint8_t u8Register, uint8_t u8Data);

extern Std_ReturnType I2C0_u8Read(uint8_t u8SlaveAddr, uint8_t *pu8Data, uint8_t u8NumData);



extern Std_ReturnType I2C1_u8WriteCmd(uint8_t u8SlaveAddr, uint8_t* u8Cmd,  uint8_t len);

extern Std_ReturnType I2C1_u8Write(uint8_t u8SlaveAddr, uint8_t* Buffer, uint8_t bytes);


extern Std_ReturnType I2C1_u8Read(uint8_t u8SlaveAddr, uint8_t *pu8Data, uint8_t u8NumData);



#endif /* I2C_H_ */
