/*
 * Init_Modules.c
 *
 *  Created on: 24 jun 2023
 *      Author: USER
 */


#include "Init_Modules.h"
#include "Sensor_Data.h"


Std_ReturnTypes modules_init(void){

    Std_ReturnTypes std_result = E_NOT_OK;

    std_result = Drivers_init();

    return std_result;
}

