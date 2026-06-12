/*
 * MS8607_PTH_cfg.h
 *
 *  Created on: Jun 9, 2024
 *      Author: Noe Ortiz Terrones
 */

#ifndef MS8607_PTH_CFG_H
#define MS8607_PTH_CFG_H

enum ms8607_humidity_i2c_master_mode{
	ms8607_i2c_hold,
	ms8607_i2c_no_hold
};

enum ms8607_status{
	ms8607_status_ok,
	ms8607_status_no_i2c_acknowledge,
	ms8607_status_i2c_transfer_error,
	ms8607_status_crc_error,
	ms8607_status_heater_on_error
};

enum ms8607_humidity_resolution{
	ms8607_humidity_resolution_12b = 0,
	ms8607_humidity_resolution_8b,
	ms8607_humidity_resolution_10b,
	ms8607_humidity_resolution_11b
};

enum ms8607_battery_status{
	ms8607_battery_ok,
	ms8607_battery_low
};

enum ms8607_heater_status{
	ms8607_heater_off,
	ms8607_heater_on
};

enum ms8607_pressure_resolution{
	ms8607_pressure_resolution_osr_256 = 0,
	ms8607_pressure_resolution_osr_512,
	ms8607_pressure_resolution_osr_1024,
	ms8607_pressure_resolution_osr_2048,
	ms8607_pressure_resolution_osr_4096,
	ms8607_pressure_resolution_osr_8192
};

typedef struct{
	uint8_t i2c_instance;					/* Instance of I2C */
	uint8_t humidityResolution;				/* Humidity resolution. Enum ms8607_humidity_resolution */
	uint8_t tempPressResolution;			/* Temperature, pressure resolution. Enum ms8607_pressure_resolution */
	uint8_t humidityMeasureMode;			/* Measurement of humidity with master hold or without. Enum ms8607_humidity_i2c_master_mode */
	uint8_t humiditySensorBatteryStatus;	/* State of battery from humidity sensor. Enum ms8607_battery_status */
	uint8_t heaterStatus;					/* State of heater embedded on humidity sensor. Enum ms8607_heater_status */
	uint8_t tempPressAdcDelay;				/* Time to wait until ADC command has been finished */
	bool getCompensatedHumidity;			/* Flag to indicate if calculate compensated humidity or not */
} ms8607_instance_t;


#endif /* MS8607_PTH_CFG_H_ */
