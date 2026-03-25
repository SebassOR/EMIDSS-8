/*
 * Init_DRV.h
 *
 *  Created on: 24 jun 2023
 *      Author: Daniel Diaz Platon
 */

/**
 * @brief The SGP40 is a digital gas sensor designed for easy integration into air purifiers or demand-controlled ventilation systems. 
 *        Sensirion’s CMOSens® technology offers a complete, easy-to-use sensor system on a single chip featuring a digital I2C interface 
 *        and a temperatur controlled micro hotplate, providing a humidit compensated VOC based indoor air quality signal. The output signal 
 *        can be directly processed by Sensirion’s powerful Gas Index Algorithm to translate the raw signal into a VOC Index as a robust measure 
 *        for indoor air quality. The Gas Index Algorithm automatically adapts to the environment the sensor is exposed to.
* 
*         IMU address 0x59
*         Protocol I2C
* 
*/
#ifndef SGP40_AIR_QUALITY_DRV_H_
#define SGP40_AIR_QUALITY_DRV_H_

#include "Std_types.h"
#include "lpi2c_driver.h"

/* LPI2C instance */
#define INST_LPI2C0  0U



uint16 bytes_to_uint16(const uint8_t* bytes);

/**
 * add_command_to_buffer() - This command starts/continues the VOC
 * measurement mode
 *
 * @param buffer Leaves humidity compensation disabled by sending the
 * default value 0x8000 (50%RH) or enables humidity compensation when sending
 * the relative humidity in ticks (ticks = %RH * 65535 / 100)
 *
 * @param offset Leaves humidity compensation disabled by sending the
 * default value 0x6666 (25 degC) or enables humidity compensation when sending
 * the temperature in ticks (ticks = (degC + 45) * 65535 / 175)
 *
 * @param command u16 unsigned integer directly provides the raw signal
 * SRAW_VOC in ticks which is proportional to the logarithm of the resistance of
 * the sensing element.
 *
 * @return 0 on success, an error code otherwise
 */
uint16 add_command_to_buffer(uint8* buffer, uint16 offset,
                                             uint16 command);

/**
 * sgp40_measure_raw_signal() - This command starts/continues the VOC
 * measurement mode
 *
 * @param relative_humidity Leaves humidity compensation disabled by sending the
 * default value 0x8000 (50%RH) or enables humidity compensation when sending
 * the relative humidity in ticks (ticks = %RH * 65535 / 100)
 *
 * @param temperature Leaves humidity compensation disabled by sending the
 * default value 0x6666 (25 degC) or enables humidity compensation when sending
 * the temperature in ticks (ticks = (degC + 45) * 65535 / 175)
 *
 * @param sraw_voc u16 unsigned integer directly provides the raw signal
 * SRAW_VOC in ticks which is proportional to the logarithm of the resistance of
 * the sensing element.
 *
 * @return 0 on success, an error code otherwise
 */
sint16 sgp40_measure_raw_signal(uint16 relative_humidity,
                                 uint16 temperature, uint16* sraw_voc);

/**
 * sgp40_execute_self_test() - This command triggers the built-in self-test
 * checking for integrity of the hotplate and MOX material and returns the
 * result of this test as 2 bytes
 *
 * @param test_result 0xD4 00: all tests passed successfully or 0x4B 00: one or
 * more tests have failed
 *
 * @return 0 on success, an error code otherwise
 */
sint16 sgp40_execute_self_test(uint16* test_result);

/**
 * sgp40_turn_heater_off() - This command turns the hotplate off and stops the
 * measurement. Subsequently, the sensor enters the idle mode.
 *
 * @return 0 on success, an error code otherwise
 */
sint16 sgp40_turn_heater_off(void);

/**
 * sgp40_get_serial_number() - This command provides the decimal serial number
 * of the SGP40 chip by returning 3x2 bytes.
 *
 * @param serial_number 48-bit unique serial number
 *
 * @return 0 on success, an error code otherwise
 */
sint16 sgp40_get_serial_number(uint16* serial_number,
                                uint8_t serial_number_size);


#endif  // SGP40_AIR_QUALITY_DRV_H_