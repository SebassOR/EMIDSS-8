/*
 * MS8607_PTH.h
 *
 *  Created on: Jun 9, 2024
 *      Author: Noe Ortiz Terrones
 */

/*
 * Driver for MS8607 that gets variables such as temperature, pressure and relative humidity from environment.
 * Ranges and resolutions
 * Temperature	- [-40° to 85°]	Resolution: 0.01°C
 * Pressure		- [10 to 2000]	Resolution: 0.01mbar
 * Humidity		- [-6% to 118%]	Resolution: 0.01%RH
 */

#ifndef MS8607_PTH_H
#define MS8607_PTH_H

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "Std_types.h"
#include "I2C.h"
#include "Delay.h"
#include "MS8607_PTH_cfg.h"
#include "MS8607_PTH_def.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/**
 * \brief Configures the LPI2C0 master to be used with the MS8607 device.
 */
void ms8607_vInit(void);

/**
 * \brief Check whether MS8607 device is connected
 *
 * \return Std_ReturnTypes : status of MS8607
 *       - OK 		: Device is present
 *       - NOT_OK 	: Device is not acknowledging I2C address
  */
Std_ReturnTypes ms8607_is_connected(void);

/**
 * \brief Reset the MS8607 device
 *
 * \return ms8607_status : status of MS8607
 *       - OK 		: Device has been reseted
 *       - NOT_OK 	: Device is not acknowledging I2C address
 */
Std_ReturnTypes ms8607_reset(void);

/**
 * \brief Set Humidity sensor ADC resolution.
 *
 * \param res : Resolution used for humidity sensor
 * 		 - ms8607_humidity_resolution_12b 	: 12 bits
 * 		 - ms8607_humidity_resolution_8b	: 8 bits
 * 		 - ms8607_humidity_resolution_10b	: 10 bits
 * 		 - ms8607_humidity_resolution_11b	: 11 bits
 *
 * \return ms8607_status : status of MS8607
 *       - OK 		: Humidity resolution set
 *       - NOT_OK 	: Device is not acknowledging I2C address
 */
Std_ReturnTypes ms8607_set_humidity_resolution(enum ms8607_humidity_resolution resolution);

/**
 * \brief Set I2C master mode.
 *
 * \param[in] ms8607_i2c_master_mode : Measure humidity with master hold or without
 *		- ms8607_i2c_hold 		: Measure humidity with master on hold
 * 		- ms8607_i2c_no_hold	: Measure humidity without master on hold
 */
void ms8607_set_humidity_i2c_master_mode(enum ms8607_humidity_i2c_master_mode);

/**
 * \brief Reads the temperature, pressure and relative humidity value.
 *
 * \param[out] float32* : degC temperature value
 * \param[out] float32* : mbar pressure value
 * \param[out] float32* : %RH Relative Humidity value
 *
 * \return ms8607_status : status of MS8607
 *       - OK 		: Operation done
 *       - NOT_OK 	: Device is not acknowledging I2C address
 */
Std_ReturnTypes ms8607_read_temperature_pressure_humidity(float32 *t, float32 *p, float32 *h);

/**
 * \brief Provide battery status
 *
 * \param[out] ms8607_battery_status* : Battery status
 *                      - ms8607_battery_ok,
 *                      - ms8607_battery_low
 *
 * \return status of MS8607
 *       - OK 		: Operation done
 *       - NOT_OK 	: Device is not acknowledging I2C address
 */
Std_ReturnTypes ms8607_get_humidity_battery_status(uint8_t *battStatus);

/**
 * \brief Enable heater
 *
 * \return ms8607_status : status of MS8607
 *       - OK 		: Heater on
 *       - NOT_OK 	: Device is not acknowledging I2C address
 */
Std_ReturnTypes ms8607_enable_humidity_heater(void);

/**
 * \brief Disable heater
 *
 * \return ms8607_status : status of MS8607
 *       - OK 		: Heater off
 *       - NOT_OK 	: Device is not acknowledging I2C address
 */
Std_ReturnTypes ms8607_disable_humidity_heater(void);

/**
 * \brief Get heater status
 *
 * \param[in] *heaterStatus : Return heater status (above or below 2.5V)
 *	                    - ms8607_heater_off,
 *                      - ms8607_heater_on
 *
 * \return ms8607_status : status of MS8607
 *       - OK 		: Operation done
 *       - NOT_OK 	: Device is not acknowledging I2C address
 */
Std_ReturnTypes ms8607_get_heater_status(uint8_t *heaterStatus);

/**
 * \brief Returns result of compensated humidity
 *
 * \param[in] float32 - Actual temperature measured (degC)
 * \param[in] float32 - Actual relative humidity measured (%RH)
 * \param[out] float32 *- Compensated humidity (%RH).
 */
void ms8607_get_compensated_humidity(float32 temperature, float32 relative_humidity, float32 *compensated_humidity);

/**
 * \brief Set Pressure sensor ADC resolution.
 *
 * \param[in] ms8607_pressure_resolution :
 * 						- ms8607_pressure_resolution_osr_256 = 0,
 *						- ms8607_pressure_resolution_osr_512,
 *						- ms8607_pressure_resolution_osr_1024,
 *						- ms8607_pressure_resolution_osr_2048,
 *						- ms8607_pressure_resolution_osr_4096,
 *						- ms8607_pressure_resolution_osr_8192
 */
void ms8607_set_pressure_resolution(enum ms8607_pressure_resolution);

#endif /* MS8607_PTH_H_ */
