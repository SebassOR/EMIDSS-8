
/*
 * Init_DRV.c
 *
 *  Created on: 24 jun 2023
 *      Author: USER
 */

#include "Init_DRV.h"
#include "I2C.h"
#include "SPI_MCAL.h"
#include "swc_tem_pre_hum.h"

Std_ReturnTypes Drivers_init(void){

    Std_ReturnTypes std_result = OK;

    /* Clocks and Pins are already initialized in main.c via RTD */

    /* Init I2C Wrapper (Instances 0 and 1) */
    I2C0_vInit();
    I2C1_vInit();

    // Sensor MS8607 INIT
	sensor_vInit();

    /* Init SPI Wrapper (Instance 1) */
    SPI_vInitMaster();

    /* Init Sensors */
    if (OK != sensor_vInit())
    {
        std_result = NOT_OK;
    }

    return std_result;
}


