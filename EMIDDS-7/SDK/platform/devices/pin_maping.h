/*
 * pin_maping.h
 *
 *  Created on: 19 abr 2023
 *      Author: Daniel
 */

#ifndef PIN_MAPING_H_
#define PIN_MAPING_H_

#include "S32K144W.h"



/***********************************    PIN CONFIGURATION     *******************************/


/* 		PortA		*/
#define PortA_IO	(7)			//Pin Inputs - Outputs

#define PTA0	(0U)	//PortA Pin 0
#define PTA1	(1U)	//PortA Pin 1
#define PTA2	(2U)	//PortA Pin 2	//SDA  I2C0
#define PTA3	(3U)	//PortA Pin 3	//SCL  I2C0
#define PTA4	(4U)	//PortA Pin 4	//JTAG_TMS/SWD_DIO
#define PTA5	(5U)	//PortA Pin 5	//PB_RESET
#define PTA6	(6U)	//PortA Pin 6
#define PTA7	(7U)	//PortA Pin 7
#define PTA10	(10U)	//PortA Pin 10	//JTAG_TDO
#define PTA11	(11U)	//PortA Pin 11
#define PTA12	(12U)	//PortA Pin 12
#define PTA13	(13U)	//PortA Pin 13

/*	PortA IO table	*/


/* 		PortB		*/
#define PortB_IO	(3)			//Pin Inputs - Outputs

#define PTB0	(0U)	//PortB Pin 0
#define PTB1	(1U)	//PortB Pin 1
#define PTB2	(2U)	//PortB Pin 2	//SCK  		SPI0
#define PTB3	(3U)	//PortB Pin 3	//SIN/MISO  SPI0
#define PTB4	(4U)	//PortB Pin 4	//SOUT/MOSI SPI0
#define PTB5	(5U)	//PortB Pin 5	//CE
#define PTB6	(6U)	//PortB Pin 6	//XTAL
#define PTB7	(7U)	//PortB Pin 7	//EXTAL
#define PTB12	(12U)	//PortB Pin 12	//AIR_QUALITY_EN
#define PTB13	(13U)	//PortB Pin 13

/*	PortB IO table	*/

/* 		PortC		*/
#define PortC_IO	(11)			//Pin Inputs - Outputs

#define PTC0	(0U)	//PortC Pin 0
#define PTC1	(1U)	//PortC Pin 1  	//LED_USER
#define PTC2	(2U)	//PortC Pin 2
#define PTC3	(3U)	//PortC Pin 3
#define PTC4	(4U)	//PortC Pin 4	//JTAG_TCLK/SWD_CLK
#define PTC5	(5U)	//PortC Pin 5	//JTAG_TDI
#define PTC6	(6U)	//PortC Pin 6
#define PTC7	(7U)	//PortC Pin 7
#define PTC8	(8U)	//PortC Pin 8
#define PTC9	(9U)	//PortC Pin 9
#define PTC14	(14U)	//PortC Pin 14
#define PTC15	(15U)	//PortC Pin 15
#define PTC16	(16U)	//PortC Pin 16
#define PTC17	(17U)	//PortC Pin 17

/*	PortC IO table	*/

/* 		PortD		*/
#define PortD_IO	(4)			//Pin Inputs - Outputs

#define PTD0	(0U)	//PortD Pin 0
#define PTD1	(1U)	//PortD Pin 1	//PB_USER
#define PTD2	(2U)	//PortD Pin 2	//HUM_TEMP_EN
#define PTD3	(3U)	//PortD Pin 3	//PRESSURE_EN
#define PTD4	(4U)	//PortD Pin 4	//IMU_EN
#define PTD5	(5U)	//PortD Pin 5
#define PTD6	(6U)	//PortD Pin 6	//UART2_TX
#define PTD7	(7U)	//PortD Pin 7	//UART2_RX
#define PTD15	(15U)	//PortD Pin 15
#define PTD16	(16U)	//PortD Pin 16

/*	PortD IO table	*/


/* 		PortE		*/
#define PortE_IO	(10)			//Pin Inputs - Outputs

#define PTE0	(0U)	//PortE Pin 0
#define PTE1	(1U)	//PortE Pin 1
#define PTE2	(2U)	//PortE Pin 2	//UART2_RTS
#define PTE3	(3U)	//PortE Pin 3
#define PTE4	(4U)	//PortE Pin 4
#define PTE5	(5U)	//PortE Pin 5
#define PTE6	(6U)	//PortE Pin 6
#define PTE7	(7U)	//PortE Pin 7
#define PTE8	(8U)	//PortE Pin 8
#define PTE9	(9U)	//PortE Pin 9	//UART2_CTS
#define PTE10	(10U)	//PortE Pin 10
#define PTE11	(11U)	//PortE Pin 11

/*	PortE IO table	*/

#define LED_USER        (PTC1)          //LED_USER Macro
#define PB_USER         (PTD1)          //PB_USER Macro


/** AIR SENSOR MAPPING*/
#define AIR_SENSOR_EN                    PTB12               // C02 Y VOC
#define AIR_SENSOR_EN_PORT               PTB                 // C02 Y VOC
#define AIR_SENSOR_MOSI                  PTB4
#define AIR_SENSOR_MOSI_PORT             PTB
#define AIR_SENSOR_MISO                  PTB3
#define AIR_SENSOR_MISO_PORT             PTB
#define AIR_SENSOR_SCK                   PTB2
#define AIR_SENSOR_SCK_PORT              PTB





/*================ The MS8607 ============================================================*/
/** HUM AND TEMP SENSOR MAPPING*/
#define HUM_TEMP_SENSOR_EN                PTD2            // TEMPERATURA Y HUMEDAD
#define HUM_TEMP_SENSOR_EN_PORT           PTD             // TEMPERATURA Y HUMEDAD
#define HUM_TEMP_SENSOR_SDA               PTA2
#define HUM_TEMP_SENSOR_SDA_PORT          PTA
#define HUM_TEMP_SENSOR_SCL               PTA3
#define HUM_TEMP_SENSOR_SCL_PORT          PTA



/** IMU SENSOR MAPPING*/
#define IMU_SENSOR_PORT                           // GIROSCOPIO O ACELEROMETRO
#define IMU_SENSOR_SDA
#define IMU_SENSOR_SDL



/** ATM SENSOR MAPPING*/
#define PRESSURE_SENSOR_PORT                  // ATM SENSOR
#define PRESSURE_SENSOR_SDA
#define PRESSURE_SENSOR_SDL




/** SDCARD MAPPING*/
#define FLASH_MEMORY_EN
#define FLASH_MEMORY_SDA
#define FLASH_MEMORY_SDL


#endif /* PIN_MAPING_H_ */
