/*
 * Init_DRV.c
 *
 *  Created on: 24 jun 2023
 *      Author: USER
 */

#include "Init_DRV.h"
#include "clock_config.h"
#include "pin_mux.h"
//#include "temp_press.h"



/*!
  \brief Function that enable all drivers to use. .e.g. I2C, SPI, UART
  \details The startup initialization sequence is the following:
 * - Init ports configurations, port_init()
 * - Init pins condifurations, pin_init()
 * - Init drivers 
 *  - CLOCK  
 *  - I2C 
 *  - SPI
 *  - UART
*/
Std_ReturnTypes Drivers_init(void){

    Std_ReturnTypes std_result = OK;

    /*=========== CLOCK INIT ============================*/
    if(OK != CLOCK_DRV_Init(&clockMan1_InitConfig0))
    {
        return NOT_OK;
    }

    /*=========== PIN INIT ============================*/
    if(OK != PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0))
    {
        return NOT_OK;
    }

    if(OK != temp_pres_init())
    {
        return NOT_OK;
    }

    SPIF_Init();

    return std_result;
}

