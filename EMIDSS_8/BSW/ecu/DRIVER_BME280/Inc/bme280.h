/*
 * bme280.h
 *
 *  Created on: 4 ago. 2026
 *      Author: grtre
 */

#ifndef ECU_DRIVER_BME280_INC_BME280_H_

#define ECU_DRIVER_BME280_INC_BME280_H_


#include "Std_Types.h"
#include "Sensor_Data.h"
#include "SPI_MCAL.h"
#include "SPI_MCAL_def.h"
#include "Delay.h"

#ifndef OK
#define OK         (0U)
#endif

#ifndef NOT_OK
#define NOT_OK     (1U)
#endif

/* BME280 Register Map */
#define BME280_REG_DIG_T1        0x88U
#define BME280_REG_DIG_H1        0xA1U
#define BME280_REG_CHIP_ID       0xD0U
#define BME280_REG_RESET         0xE0U
#define BME280_REG_DIG_H2        0xE1U
#define BME280_REG_CTRL_HUM      0xF2U
#define BME280_REG_STATUS        0xF3U
#define BME280_REG_CTRL_MEAS     0xF4U
#define BME280_REG_CONFIG        0xF5U
#define BME280_REG_PRESS_MSB     0xF7U

#define BME280_CHIP_ID_VAL       0x60U
#define BME280_RESET_CMD         0xB6U







/* BME280 Factory Calibration Parameters */
typedef struct {
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;
    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;
    uint8_t  dig_H1;
    int16_t  dig_H2;
    uint8_t  dig_H3;
    int16_t  dig_H4;
    int16_t  dig_H5;
    int8_t   dig_H6;
} BME280_CalibData_t;

/* Function Prototypes */

/**
 * \brief Initialize the BME280 sensor over SPI.
 * \param[in] u8SpiModule SPI bus module (e.g. SPI_enSPI2)
 * \return OK if initialization successful, NOT_OK otherwise
 */
Std_ReturnTypes BME280_vInit(uint8_t u8SpiModule);

/**
 * \brief Verify BME280 connectivity by reading chip ID.
 * \param[in] u8SpiModule SPI bus module
 * \return OK if device connected (ID = 0x60), NOT_OK otherwise
 */
Std_ReturnTypes BME280_is_connected(uint8_t u8SpiModule);

/**
 * \brief Soft reset the BME280 sensor.
 * \param[in] u8SpiModule SPI bus module
 * \return OK if reset command sent successfully
 */
Std_ReturnTypes BME280_reset(uint8_t u8SpiModule);

/**
 * \brief Read factory compensation parameters from BME280 EEPROM registers.
 * \param[in] u8SpiModule SPI bus module
 * \return OK if calibration parameters read successfully
 */
Std_ReturnTypes BME280_read_compensation_parameters(uint8_t u8SpiModule);

/**
 * \brief Read raw sensor readings (humidity, pressure, temperature).
 * \param[in] u8SpiModule SPI bus module
 * \param[out] humidity Raw humidity ADC value
 * \param[out] pressure Raw pressure ADC value
 * \param[out] temperature Raw temperature ADC value
 * \return OK if successful
 */
Std_ReturnTypes BME280_read_raw(uint8_t u8SpiModule, int32_t *humidity, int32_t *pressure, int32_t *temperature);

/**
 * \brief Read and compensate Temperature (degC), Pressure (hPa), and Humidity (%RH).
 * \param[in] u8SpiModule SPI bus module
 * \param[out] temp_degC Pointer to store temperature in Celsius
 * \param[out] press_hPa Pointer to store pressure in hPa/mbar
 * \param[out] hum_pct Pointer to store relative humidity in %
 * \return OK if successful
 */
Std_ReturnTypes BME280_read_temperature_pressure_humidity(uint8_t u8SpiModule, float32 *temp_degC, float32 *press_hPa, float32 *hum_pct);


#endif /* ECU_DRIVER_BME280_INC_BME280_H_ */
