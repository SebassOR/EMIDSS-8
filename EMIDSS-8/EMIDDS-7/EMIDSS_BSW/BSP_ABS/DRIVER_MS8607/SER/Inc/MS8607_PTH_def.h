/*
 * MS8607_PTH_def.h
 *
 *  Created on: Jun 9, 2024
 *      Author: Noe Ortiz Terrones
 */

#ifndef MS8607_PTH_DEF_H
#define MS8607_PTH_DEF_H

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define INST_LPI2C0  										0U
#define TP_SENSOR_ADDR										0x76 //0b1110110, D1=Pressure, D2=Temperature
#define H_SENSOR_ADDR										0x40 //0b1000000

#define BUFF_SIZE (64u)

/*						Humidity sensor								*/
// Commands
#define H_SENSOR_RESET_COMMAND								0xFE
#define H_SENSOR_READ_HUMIDITY_W_HOLD_COMMAND				0xE5
#define H_SENSOR_READ_HUMIDITY_WO_HOLD_COMMAND				0xF5
#define H_SENSOR_READ_SERIAL_FIRST_8BYTES_COMMAND			0xFA0F
#define H_SENSOR_READ_SERIAL_LAST_6BYTES_COMMAND				0xFCC9
#define H_SENSOR_WRITE_USER_REG_COMMAND						0xE6
#define H_SENSOR_READ_USER_REG_COMMAND						0xE7

// User register
#define H_SENSOR_USER_REG_DATA_MASK  						0x7E
#define H_SENSOR_USER_REG_RESOLUTION_MASK					0x81
#define H_SENSOR_USER_REG_END_OF_BATTERY_MASK				0x40
#define H_SENSOR_USER_REG_ENABLE_ONCHIP_HEATER_MASK			0x4
#define H_SENSOR_USER_REG_DISABLE_OTP_RELOAD_MASK			0x2

// Resolution
#define H_SENSOR_USER_REG_RESOLUTION_12b					0x00
#define H_SENSOR_USER_REG_RESOLUTION_11b					0x01
#define H_SENSOR_USER_REG_RESOLUTION_10b					0x80
#define H_SENSOR_USER_REG_RESOLUTION_8b						0x81

// Battery status
#define H_SENSOR_USER_REG_BATTERY_STATUS_MASK				0x40
#define H_SENSOR_USER_REG_BATTERY_STATUS_ABOVE_2_25V		0x00
#define H_SENSOR_USER_REG_BATTERY_STATUS_BELOW_2_25V		0x40

// Enable on chip heater
#define H_SENSOR_USER_REG_HEATER_MASK						0x04
#define H_SENSOR_USER_REG_NOT_HEATER_MASK					0xFB
#define H_SENSOR_USER_REG_ONCHIP_HEATER_ENABLE				0x04
#define H_SENSOR_USER_REG_ONCHIP_HEATER_DISABLE				0x0

// Conversion timings
#define H_SENSOR_CONVERSION_TIME_12b						16000
#define H_SENSOR_CONVERSION_TIME_10b						5000
#define H_SENSOR_CONVERSION_TIME_8b							3000
#define H_SENSOR_CONVERSION_TIME_11b						9000
#define H_SENSOR_CONVERSION_TIME							7U

// Coefficients for relative humidity computation
#define HUMIDITY_COEFF_MUL									(125)
#define HUMIDITY_COEFF_ADD									(-6)

// Processing constants
#define H_SENSOR_TEMPERATURE_COEFFICIENT					(float32)(-0.18)
#define H_SENSOR_CONSTANT_A									(float32)(8.1332)
#define H_SENSOR_CONSTANT_B									(float32)(1762.39)
#define H_SENSOR_CONSTANT_C									(float32)(235.66)

/*						Temperature and pressure sensor				*/
// Commands
#define TP_SENSOR_RESET_COMMAND								0x1E
#define TP_SENSOR_START_PRESSURE_ADC_CONVERSION				0x40
#define TP_SENSOR_START_TEMPERATURE_ADC_CONVERSION			0x50
#define TP_SENSOR_READ_ADC									0x00
#define PROM_ADDRESS_READ_ADDRESS_0							0xA0
#define PROM_ADDRESS_READ_ADDRESS_1							0xA2
#define PROM_ADDRESS_READ_ADDRESS_2							0xA4
#define PROM_ADDRESS_READ_ADDRESS_3							0xA6
#define PROM_ADDRESS_READ_ADDRESS_4							0xA8
#define PROM_ADDRESS_READ_ADDRESS_5							0xAA
#define PROM_ADDRESS_READ_ADDRESS_6							0xAC
#define PROM_ADDRESS_READ_ADDRESS_7							0xAE

// Coefficients indexes for computation
#define CRC_INDEX											0
#define C1_PRESSURE_SENSITIVITY_INDEX						1
#define C2_PRESSURE_OFFSET_INDEX							2
#define C3_TEMP_COEFF_OF_PRESSURE_SENSITIVITY_INDEX			3
#define C4_TEMP_COEFF_OF_PRESSURE_OFFSET_INDEX				4
#define C5_REFERENCE_TEMPERATURE_INDEX						5
#define C6_TEMP_COEFF_OF_TEMPERATURE_INDEX					6
#define COEFFICIENT_NUMBERS									7

// Coefficients for temperature computation
#define TEMPERATURE_COEFF_MUL								(175.72)
#define TEMPERATURE_COEFF_ADD								(-46.85)

// Temperature and pressure resolutions
#define TP_SENSOR_CONVERSION_TIME_OSR_256					1000
#define TP_SENSOR_CONVERSION_TIME_OSR_512					2000
#define TP_SENSOR_CONVERSION_TIME_OSR_1024					3000
#define TP_SENSOR_CONVERSION_TIME_OSR_2048					5000
#define TP_SENSOR_CONVERSION_TIME_OSR_4096					9000
#define TP_SENSOR_CONVERSION_TIME_OSR_8192					18000

#define TP_SENSOR_TIME_CONVERSION_ORS_256					1
#define TP_SENSOR_TIME_CONVERSION_ORS_512					1
#define TP_SENSOR_TIME_CONVERSION_ORS_1024					2
#define TP_SENSOR_TIME_CONVERSION_ORS_2048					3
#define TP_SENSOR_TIME_CONVERSION_ORS_4096					6
#define TP_SENSOR_TIME_CONVERSION_ORS_8192					13

#endif /* MS8607_PTH_DEF_H_ */
