/*
 * Sensor_Data.h
 *
 *  Created on: 16 jun. 2026
 *      Author: grtre
 */

#ifndef COMMON_SENSOR_DATA_H_
#define COMMON_SENSOR_DATA_H_

#include "Platform_Types.h"

typedef uint8 Std_ReturnTypes;


#define OK   		(Std_ReturnTypes)0U
#define NOT_OK		(Std_ReturnTypes)1U
#define FLOAT_SIZE	4

typedef struct {
	float32 temperature;
    float32 pressure;
    float32 humidity;
} SensorData_t;



#endif /* COMMON_SENSOR_DATA_H_ */
