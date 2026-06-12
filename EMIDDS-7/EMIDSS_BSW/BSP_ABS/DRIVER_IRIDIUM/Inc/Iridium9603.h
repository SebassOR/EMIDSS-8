/*
* Iridium9603.h
*
*  Created on: 29 abr. 2025
*      Author: Sergio Ramirez
*/
#ifndef BSP_ABS_DRIVER_IRIDIUM_INC_IRIDIUM9603_H_
#define BSP_ABS_DRIVER_IRIDIUM_INC_IRIDIUM9603_H_

/*******************************************************************************
* Includes
******************************************************************************/
#include "Platform_Types.h"

/*******************************************************************************
* API
******************************************************************************/
extern uint8_t Iridium9603_EnableModem(void);
extern uint8_t Iridium9603_GetSignalQuality(void);
extern bool Iridium9603_SendSBDText(uint8_t* buffer, uint16_t len);
extern bool Iridium9603_ClearMOBuffer(void);
extern void Iridium9603_DisableModem(void);


#endif /* BSP_ABS_DRIVER_IRIDIUM_INC_IRIDIUM9603_H_ */


