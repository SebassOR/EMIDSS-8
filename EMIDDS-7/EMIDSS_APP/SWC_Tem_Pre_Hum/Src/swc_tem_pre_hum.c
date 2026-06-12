/*
 * swc_temp_pre_hum.c
 *
 *  Created on: Jun 10, 2024
 *      Author: Noe Ortiz Terrones
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "string.h"
#include "swc_tem_pre_hum.h"

/*******************************************************************************
 * Functions
 ******************************************************************************/
/**
 * \brief Init configuration of MS8607 sensor
 */
Std_ReturnTypes sensor_vInit(void){
	ms8607_vInit();
	return ms8607_reset();
}

/**
 * \brief Get temperature, pressure and humidity from MS8607 sensor
 */
Std_ReturnTypes getTemperaturePressureHumidity(SensorData_t *sensorData){
	return ms8607_read_temperature_pressure_humidity(&sensorData->temperature, &sensorData->pressure, &sensorData->humidity);
}
