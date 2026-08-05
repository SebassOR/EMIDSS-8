/*==================================================================================================
* Project : RTD AUTOSAR 4.9
* Platform : CORTEXM
* Peripheral : S32K3XX
* Dependencies : none
*
* Autosar Version : 4.9.0
* Autosar Revision : ASR_REL_4_9_REV_0000
* Autosar Conf.Variant :
* SW Version : 7.0.0
* Build Version : S32K3_RTD_7_0_0_QLP03_D2512_ASR_REL_4_9_REV_0000_20251210
*
* Copyright 2020 - 2025 NXP
*
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

/**
*   @file main.c
*
*   @addtogroup main_module main module documentation
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif


/* Including necessary configuration files. */
#include "Mcal.h"
#include "Clock_Ip.h"
#include "Lpi2c_Ip.h"
#include "Siul2_Port_Ip.h"
#include "OsIf.h"

/* User includes */
#include "swc_os.h"
#include "swc_tem_pre_hum.h"
#include "mem.h"
#include "timeCounter.h"
#include "Delay.h"
#include "GPIO.h"
#include "S32K312.h"
#include <stdio.h>
#include <string.h>
#include "Iridium9603.h"
#include "bme280.h"

#define TP_SENSOR_ADDR										0x76 //0b1110110, D1=Pressure, D2=Temperature
#define H_SENSOR_ADDR										0x40 //0b1000000

volatile int exit_code = 0;


/* SysTick Registers for Cortex-M7 */
#define SYST_CSR (*(volatile uint32*)0xE000E010)
#define SYST_RVR (*(volatile uint32*)0xE000E014)
#define SYST_CVR (*(volatile uint32*)0xE000E018)

/*******************************************************************************
 * Function Name : SysTick_Conf
 * Notes         : Configures SysTick for 1ms interrupt using a fixed frequency
 *******************************************************************************/
void SysTick_Conf(void)
{
    /* 
     * The Clock_Ip_GetClockFrequency API is disabled in the generated configuration.
     * The S32K312 is configured to run at 80 MHz or 160 MHz typically. 
     * We will use 160MHz / 1000 = 160,000 ticks for a 1ms interrupt.
     * If your core is 80MHz, change this value to 80000000U.
     */
    uint32 core_freq = 160000000U; /* 160 MHz */
    uint32 ticks = core_freq / 1000U;

    SYST_RVR = (ticks - 1U); /* Set reload register */
    SYST_CVR = 0U;           /* Clear current value */
    SYST_CSR = 0x07U;        /* Enable SysTick, Enable Interrupt (TICKINT), Use Core Clock */
}

/*!
  \brief The main function for the project.
*/
int main(void)
{
    /* Initialize RTD Clock Driver */
    Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);


    GPIO_u32SetOutputState(0);

    /* Initialize OS Tick (Hardware Timer mapped via OsIf) */
    OsIf_Init(NULL_PTR);

    /* Force enable the SysTick Interrupt natively */
    SysTick_Conf();

    // Initialize RTD Port Driver (SIUL2)
    Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS_PortContainer_0_BOARD_InitPeripherals,
           		g_pin_mux_InitConfigArr_PortContainer_0_BOARD_InitPeripherals);


    // Initialiaze all system modules, Sensors, Modem, MEMORY, first system checkup.

 //   I2C0_vInit();
    I2C1_vInit();

    SPI_vInitMaster();
    // Sensor MS8607 INIT
	//sensor_vInit();

    float32 temp_degC = 0.0f;
    float32 press_hPa = 0.0f;
    float32 hum_pct   = 0.0f;

    if (BME280_vInit(SPI_enSPI2) == OK)
       {
          /* Read Temperature, Pressure, and Humidity */
          BME280_read_temperature_pressure_humidity(SPI_enSPI2, &temp_degC, &press_hPa, &hum_pct);
       }



    /*****************************/

    /* App and Sensor Init */

    Time_init_time(23, 30);

    // MEM_InitMemory();

    test_iridium();

    while(1)
    {


        if(Delay_count_1_minute())//Called every 1m
       {
           Time_increase_minute();
            Task_1m();
       }

        if(Delay_count_1_second()) //Called every 1s
       {
            Task_1s();
       }
    }
}


#ifdef __cplusplus
}
#endif

/** @} */
