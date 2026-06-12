/*
 * ReadServices.h
 *
 *  Created on: 15 mar. 2024
 *      Author: Maximiliano Mancilla
 */

#ifndef SWC_USER_INTERFACE_INC_READSERVICES_H_
#define SWC_USER_INTERFACE_INC_READSERVICES_H_

#include "Std_Types.h"

void ResetService(uint8_t);
void ReadServiceSelect(uint8_t);
void WriteServiceSelect(uint8_t, uint8_t, uint8_t);

void ReadService_01(void);
void ReadService_02(void);
void ReadService_03(void);
void WriteService_01(uint8_t, uint8_t);
void ResetService_01(void);
void ResetService_02(void);
void AliveUart(void);

#endif /* SWC_USER_INTERFACE_INC_READSERVICES_H_ */
