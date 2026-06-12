/*
 * pressure.h
 *
 *  Created on: 19 abr 2023
 *      Author: luisf
 */

#ifndef DRIVER_PRES_IF_INC_PRESSURE_H_
#define DRIVER_PRES_IF_INC_PRESSURE_H_

#include "Std_types.h"


/**
 * @brief The MS8607 converts both analog output voltages to a 24-bit digital value for the pressure and temperature measurements,
 *        and a 12-bit digital value for the relative humidity measurement. The external microcontroller clocks in the data through
 *        the input SCL (Serial CLock) and SDA (Serial DAta). Both sensors respond on the same pin SDA which is bidirectional for the
 *        I2C bus interface. Two distinct I2C addresses are used (one for pressure and temperature, the other for relative humidity)
 *
 *
 * @return Std_ReturnTypes
 */
Std_ReturnTypes request_data_humidity(void);

#endif /* DRIVER_PRES_IF_INC_PRESSURE_H_ */
