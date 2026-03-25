/*
* Iridium9603_def.h
*
*  Created on: 29 abr. 2025
*      Author: Sergio Ramirez
*/

#ifndef BSP_ABS_DRIVER_IRIDIUM_INC_IRIDIUM9603_DEF_H_
#define BSP_ABS_DRIVER_IRIDIUM_INC_IRIDIUM9603_DEF_H_

#define IRIDIUM_I2C_ADDRESS 0x63
#define IO_REG  0x10
#define LEN_REG 0xFD
#define DATA_REG 0xFF

// Bits del IO_REGISTER
#define IO_SHDN    (1 << 0)
#define IO_PWR_EN  (1 << 1)
#define IO_ON_OFF  (1 << 2)
#define IO_RI      (1 << 3)
#define IO_NA      (1 << 4)
#define IO_PGOOD   (1 << 5)
#define IO_LOW_PWR (1 << 6)

/* Status info */
#define IRIDIUM_FAIL_CONN		  (0)
#define IRIDIUM_SUCCESS			  (1)
#define IRIDIUM_FAIL_TIMEOUT	  (2)
#define IRIDIUM_NO_MODEM_DETECTED (3)

#endif /* BSP_ABS_DRIVER_IRIDIUM_INC_IRIDIUM9603_DEF_H_ */
