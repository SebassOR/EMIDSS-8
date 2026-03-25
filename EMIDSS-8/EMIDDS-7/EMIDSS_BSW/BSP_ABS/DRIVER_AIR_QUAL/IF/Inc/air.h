/*
 * air.h
 *
 *  Created on: 15 abr 2023
 *      Author: USER
 */

#ifndef AIR_DRIVER_IF_AIR_H_
#define AIR_DRIVER_IF_AIR_H_

//typedef unsigned int* co2_data_t;
//typedef unsigned int* voc_data_t;

#include "Std_types.h"

Std_ReturnTypes request_data_voc(void);
Std_ReturnTypes request_data_co2(void);

#endif /* AIR_DRIVER_IF_AIR_H_ */
