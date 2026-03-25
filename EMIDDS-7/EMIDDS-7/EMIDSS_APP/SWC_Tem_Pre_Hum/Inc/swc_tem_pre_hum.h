/*
 * swc_tem_pre_hum.h
 *
 *  Created on: Jun 10, 2024
 *      Author: Noe Ortiz Terrones
 */

#ifndef SWC_TEMP_PRE_HUM_H_
#define SWC_TEMP_PRE_HUM_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "MS8607_PTH.h"
#include "swc_tem_pre_hum_cfg.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/**
 * \brief Init configuration of MS8607 sensor
 *
 * \return status of MS8607
 *       - OK 		: Operation done
 *       - NOT_OK 	: Device is not acknowledging I2C address
 */
Std_ReturnTypes sensor_vInit(void);

/**
 * \brief Get temperature, pressure and humidity from MS8607 sensor
 *
 * \param[out] SensorData_t* : Struct of sensor variables
 *                      - float32 temperature,
 *                      - float32 pressure,
 *                      - float32 humidity
 *
 * \return status of MS8607
 *       - OK 		: Operation done
 *       - NOT_OK 	: Device is not acknowledging I2C address
 */
Std_ReturnTypes getTemperaturePressureHumidity(SensorData_t *);

#endif /* SWC_TEMP_PRE_HUM_H_ */
