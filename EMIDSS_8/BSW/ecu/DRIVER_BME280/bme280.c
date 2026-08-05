/*
 * bme280.c
 *  Created on: Jul 22, 2026
 *      Author: Gerardo Trejo
 * Driver implementation for BME280 Environmental Sensor via SPI (SPI2).
 * Target Architecture: S32K312 / RTD AUTOSAR 4.9.0
 */

#include "bme280.h"
#include "SPI_MCAL.h"
#include "Std_Types.h"
#include "Sensor_Data.h"
#include "SPI_MCAL_def.h"
#include "Delay.h"



static BME280_CalibData_t g_bme280_calib;
static int32_t g_t_fine = 0;

/* Private compensation formulas as defined in Bosch BME280 specification */

static int32_t BME280_compensate_temp(int32_t adc_T)
{
    int32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((int32_t)g_bme280_calib.dig_T1 << 1))) * ((int32_t)g_bme280_calib.dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)g_bme280_calib.dig_T1)) * ((adc_T >> 4) - ((int32_t)g_bme280_calib.dig_T1))) >> 12) * ((int32_t)g_bme280_calib.dig_T3)) >> 14;

    g_t_fine = var1 + var2;
    T = (g_t_fine * 5 + 128) >> 8;
    return T; /* Temperature in 0.01 degC */
}

static uint32_t BME280_compensate_pressure(int32_t adc_P)
{
    int32_t var1, var2;
    uint32_t p;

    var1 = (((int32_t)g_t_fine) >> 1) - (int32_t)64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t)g_bme280_calib.dig_P6);
    var2 = var2 + ((var1 * ((int32_t)g_bme280_calib.dig_P5)) << 1);
    var2 = (var2 >> 2) + (((int32_t)g_bme280_calib.dig_P4) << 16);
    var1 = (((g_bme280_calib.dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((int32_t)g_bme280_calib.dig_P2) * var1) >> 1)) >> 18;
    var1 = ((((32768 + var1)) * ((int32_t)g_bme280_calib.dig_P1)) >> 15);

    if (var1 == 0)
    {
        return 0U; /* Avoid division by zero */
    }

    p = (((uint32_t)(((int32_t)1048576) - adc_P) - (uint32_t)(var2 >> 12))) * 3125U;
    if (p < 0x80000000U)
    {
        p = (p << 1) / ((uint32_t)var1);
    }
    else
    {
        p = (p / (uint32_t)var1) * 2U;
    }

    var1 = (((int32_t)g_bme280_calib.dig_P9) * ((int32_t)(((p >> 3) * (p >> 3)) >> 13))) >> 12;
    var2 = (((int32_t)(p >> 2)) * ((int32_t)g_bme280_calib.dig_P8)) >> 13;
    p = (uint32_t)((int32_t)p + ((var1 + var2 + g_bme280_calib.dig_P7) >> 4));

    return p; /* Pressure in Pa */
}

static uint32_t BME280_compensate_humidity(int32_t adc_H)
{
    int32_t v_x1_u32r;

    v_x1_u32r = (g_t_fine - ((int32_t)76800));
    v_x1_u32r = (((((adc_H << 14) - (((int32_t)g_bme280_calib.dig_H4) << 20) - (((int32_t)g_bme280_calib.dig_H5) * v_x1_u32r)) +
                   ((int32_t)16384)) >> 15) * (((((((v_x1_u32r * ((int32_t)g_bme280_calib.dig_H6)) >> 10) *
                                                   (((v_x1_u32r * ((int32_t)g_bme280_calib.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
                                                 ((int32_t)2097152)) * ((int32_t)g_bme280_calib.dig_H2) + 8192) >> 14));
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)g_bme280_calib.dig_H1)) >> 4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);

    return (uint32_t)(v_x1_u32r >> 12); /* Humidity in Q22.10 format */
}

Std_ReturnTypes BME280_is_connected(uint8_t u8SpiModule)
{
    uint8_t chip_id = 0U;
    Std_ReturnTypes status;

    status = SPI_u8ReadRegister(u8SpiModule, BME280_REG_CHIP_ID, &chip_id, 1U);
    if ((status == OK) && (chip_id == BME280_CHIP_ID_VAL))
    {
        return OK;
    }
    return NOT_OK;
}

Std_ReturnTypes BME280_reset(uint8_t u8SpiModule)
{
    Std_ReturnTypes status;

    status = SPI_u8WriteRegister(u8SpiModule, BME280_REG_RESET, BME280_RESET_CMD);
    Delay_vMs(10U);
    return status;
}

Std_ReturnTypes BME280_read_compensation_parameters(uint8_t u8SpiModule)
{
    uint8_t buffer[26];
    uint8_t buffer_h[7];
    Std_ReturnTypes status;

    /* Read 26 bytes starting from 0x88 (dig_T1..T3, dig_P1..P9, dig_H1 at 0xA1) */
    status = SPI_u8ReadRegister(u8SpiModule, BME280_REG_DIG_T1, buffer, 26U);
    if (status != OK)
    {
        return NOT_OK;
    }

    g_bme280_calib.dig_T1 = (uint16_t)(buffer[0] | (buffer[1] << 8));
    g_bme280_calib.dig_T2 = (int16_t)(buffer[2] | (buffer[3] << 8));
    g_bme280_calib.dig_T3 = (int16_t)(buffer[4] | (buffer[5] << 8));

    g_bme280_calib.dig_P1 = (uint16_t)(buffer[6] | (buffer[7] << 8));
    g_bme280_calib.dig_P2 = (int16_t)(buffer[8] | (buffer[9] << 8));
    g_bme280_calib.dig_P3 = (int16_t)(buffer[10] | (buffer[11] << 8));
    g_bme280_calib.dig_P4 = (int16_t)(buffer[12] | (buffer[13] << 8));
    g_bme280_calib.dig_P5 = (int16_t)(buffer[14] | (buffer[15] << 8));
    g_bme280_calib.dig_P6 = (int16_t)(buffer[16] | (buffer[17] << 8));
    g_bme280_calib.dig_P7 = (int16_t)(buffer[18] | (buffer[19] << 8));
    g_bme280_calib.dig_P8 = (int16_t)(buffer[20] | (buffer[21] << 8));
    g_bme280_calib.dig_P9 = (int16_t)(buffer[22] | (buffer[23] << 8));

    g_bme280_calib.dig_H1 = buffer[25];

    /* Read 7 bytes starting from 0xE1 (dig_H2..dig_H6) */
    status = SPI_u8ReadRegister(u8SpiModule, BME280_REG_DIG_H2, buffer_h, 7U);
    if (status != OK)
    {
        return NOT_OK;
    }

    g_bme280_calib.dig_H2 = (int16_t)(buffer_h[0] | (buffer_h[1] << 8));
    g_bme280_calib.dig_H3 = buffer_h[2];
    g_bme280_calib.dig_H4 = (int16_t)((buffer_h[3] << 4) | (buffer_h[4] & 0x0FU));
    g_bme280_calib.dig_H5 = (int16_t)((buffer_h[4] >> 4) | (buffer_h[5] << 4));
    g_bme280_calib.dig_H6 = (int8_t)buffer_h[6];

    return OK;
}

Std_ReturnTypes BME280_vInit(uint8_t u8SpiModule)
{
    Std_ReturnTypes status;
    uint8_t status_reg = 0U;
    uint16_t timeout_cnt = 0U;

    /* Verify device connectivity */
    status = BME280_is_connected(u8SpiModule);
    if (status != OK)
    {
        return NOT_OK;
    }

    /* Soft reset device */
    BME280_reset(u8SpiModule);

    /* Wait for NVM calibration copy to complete (BME280 status reg 0xF3 bit 0 im_update) */
    do
    {
        Delay_vMs(2U);
        SPI_u8ReadRegister(u8SpiModule, BME280_REG_STATUS, &status_reg, 1U);
        timeout_cnt++;
    } while (((status_reg & 0x01U) != 0U) && (timeout_cnt < 50U));

    /* Read factory compensation parameters */
    status = BME280_read_compensation_parameters(u8SpiModule);
    if (status != OK)
    {
        return NOT_OK;
    }

    /* Ensure sensor is in SLEEP mode before configuring ctrl_hum and config (Bosch DS requirement) */
    SPI_u8WriteRegister(u8SpiModule, BME280_REG_CTRL_MEAS, 0x00U);

    /* Configure Humidity oversampling (0xF2) = x1 (0x01) */
    SPI_u8WriteRegister(u8SpiModule, BME280_REG_CTRL_HUM, 0x01U);

    /* Configure Config (0xF5) = t_standby 0.5ms, filter off -> 0x00 */
    SPI_u8WriteRegister(u8SpiModule, BME280_REG_CONFIG, 0x00U);

    /* Configure Ctrl Meas (0xF4) LAST = Temp x1, Press x1, Normal Mode -> 0x27 */
    SPI_u8WriteRegister(u8SpiModule, BME280_REG_CTRL_MEAS, 0x27U);

    return OK;
}

Std_ReturnTypes BME280_read_raw(uint8_t u8SpiModule, int32_t *humidity, int32_t *pressure, int32_t *temperature)
{
    uint8_t buffer[8];
    Std_ReturnTypes status;

    status = SPI_u8ReadRegister(u8SpiModule, BME280_REG_PRESS_MSB, buffer, 8U);
    if (status != OK)
    {
        return NOT_OK;
    }

    *pressure    = ((int32_t)buffer[0] << 12) | ((int32_t)buffer[1] << 4) | ((int32_t)buffer[2] >> 4);
    *temperature = ((int32_t)buffer[3] << 12) | ((int32_t)buffer[4] << 4) | ((int32_t)buffer[5] >> 4);
    *humidity    = ((int32_t)buffer[6] << 8)  | (int32_t)buffer[7];

    return OK;
}

Std_ReturnTypes BME280_read_temperature_pressure_humidity(uint8_t u8SpiModule, float32 *temp_degC, float32 *press_hPa, float32 *hum_pct)
{
    int32_t raw_hum = 0, raw_press = 0, raw_temp = 0;
    int32_t temp_comp;
    uint32_t press_comp, hum_comp;
    Std_ReturnTypes status;

    status = BME280_read_raw(u8SpiModule, &raw_hum, &raw_press, &raw_temp);
    if (status != OK)
    {
        return NOT_OK;
    }

    temp_comp  = BME280_compensate_temp(raw_temp);
    press_comp = BME280_compensate_pressure(raw_press);
    hum_comp   = BME280_compensate_humidity(raw_hum);

    if (temp_degC != NULL_PTR)
    {
        *temp_degC = (float32)temp_comp / 100.0f;
    }
    if (press_hPa != NULL_PTR)
    {
        *press_hPa = (float32)press_comp / 100.0f; /* Convert Pa to hPa / mbar */
    }
    if (hum_pct != NULL_PTR)
    {
        *hum_pct = (float32)hum_comp / 1024.0f; /* Convert Q22.10 to %RH */
    }

    return OK;
}
