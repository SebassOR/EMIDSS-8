/*
 * Std_types.h
 *
 *  Created on: 19 abr 2023
 *      Author: daniel
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_

#include "Platform_Types.h"
#include "pin_maping.h"

typedef uint8 Std_ReturnTypes;

#define OK   		(Std_ReturnTypes)0U
#define NOT_OK		(Std_ReturnTypes)1U
#define FLOAT_SIZE	4

typedef struct {
	float32 temperature;
    float32 pressure;
    float32 humidity;
} SensorData_t;

#endif /* STD_TYPES_H_ */
