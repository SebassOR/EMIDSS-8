/*
 * PORT.c
 *
 *  Created on: Mar 15, 2024
 *  Author: JARS
 *  Version: 1.0v
 *  Ultima modif.: -
 *  Author: -
 *
 */

/* System Headers*/
#include "S32K144W.h"
#include "Platform_Types.h"
#include "Std_Types.h"

/*	Own headers */
#include "PORT_cfg.h"
#include "PORT_def.h"
#include "PORT.h"

/*	Macros	*/

/*	Defines	*/

/*	Types	*/

/*	Private functions */

static void PORT_s_vEnableClocks(void){

	/* Se inicializan los relojes de los perifericos de puerto */

	/* Habilita el reloj del puerto A */
	PCC->PCCn[PCC_PORTA_INDEX] |= PCC_PCCn_CGC_MASK;

	/* Habilita el reloj del puerto B */
	PCC->PCCn[PCC_PORTB_INDEX] |= PCC_PCCn_CGC_MASK;

	/* Habilita el reloj del puerto C */
	PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;

	/* Habilita el reloj del puerto D */
	PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;

	/* Habilita el reloj del puerto E */
	PCC->PCCn[PCC_PORTE_INDEX] |= PCC_PCCn_CGC_MASK;

}

static void PORT_s_vInitIOs(void){

	/* Se configura los pines de salidas y entrada del micro */

	/* Inputs */
	PORTC->PCR[PORT_enPIN3] |= (1 << 4);/* PTC3Passive Filter Enable */
	PORTC->PCR[PORT_enPIN3] |= PORT_PCR_MUX(PORT_enPINMUX1);  /* PTC3 PinMux Control 1 USER_PB */

	/* Outputs */
	//PORTC->PCR[PORT_enPIN2] |= PORT_PCR_MUX(PORT_enPINMUX1);  /* PTC2 PinMux Control 1 USER_LED */
	PORTC->PCR[PORT_enPIN14] |= PORT_PCR_MUX(PORT_enPINMUX1);  /* PTC14 PinMux Control 1 LPSPI2_PCS0 */
	PORTE->PCR[PORT_enPIN10] |= PORT_PCR_MUX(PORT_enPINMUX1);  /* PTE10 PinMux Control 1 LPSPI2_PCS1 */
}
static void PORT_s_vInitUARTs(void){

	/* Se configura los pines de UARTs */

	/* UART0 */
//	PORTB->PCR[PORT_enPIN1] |= PORT_PCR_MUX(PORT_enPINMUX2);  /* PTB1 PinMux Control 2 UART0 TX */
//	PORTB->PCR[PORT_enPIN0] |= PORT_PCR_MUX(PORT_enPINMUX2);  /* PTB0 PinMux Control 2 UART0 RX */

	/* UART1 */
	//PORTC->PCR[PORT_enPIN6] |= PORT_PCR_MUX(PORT_enPINMUX2);  /* PTC6 PinMux Control 2 UART1 RX */
	//PORTC->PCR[PORT_enPIN7] |= PORT_PCR_MUX(PORT_enPINMUX2);  /* PTC6 PinMux Control 2 UART1 TX */

}

static void PORT_s_vInitSPIs(void){

	/* Se configura los pines de SPI */

	/* Config. SPI0 */
	//PORTD->PCR[PORT_enPIN15] |= PORT_PCR_MUX(PORT_enPINMUX4); /* PTD16 PinMux Control 4 LPSPI0_SCK reloj */
	//PORTD->PCR[PORT_enPIN16] |= PORT_PCR_MUX(PORT_enPINMUX4); /* PTD16 PinMux Control 4 LPSPI0_SIN/MISO */
	//PORTB->PCR[PORT_enPIN4] |= PORT_PCR_MUX(PORT_enPINMUX3);  /* PTB4 PinMux Control 3 LPSPI0_SOUT/MOSI */
	//PORTB->PCR[PORT_enPIN5] |= PORT_PCR_MUX(PORT_enPINMUX4);  /* PTB5 PinMux Control 4 LPSPI0_PCS0 */
//
	///*	Config. SPI2	*/
	//PORTC->PCR[PORT_enPIN15]|=PORT_PCR_MUX(PORT_enPINMUX3); /* PTC15 PinMux Control 3 LPSPI2_SCK reloj */
	//PORTC->PCR[PORT_enPIN0]|=PORT_PCR_MUX(PORT_enPINMUX3);  /* PTC0 PinMux Control 3 LPSPI2_SIN/MISO */
	//PORTC->PCR[PORT_enPIN1]|=PORT_PCR_MUX(PORT_enPINMUX3);  /* PTC1 PinMux Control 3 LPSPI2_SOUT/MOSI */
//	PORTC->PCR[PORT_enPIN14]|=PORT_PCR_MUX(PORT_enPINMUX3); /* PTC14 PinMux Control 3 LPSPI2_PCS0 */

}

static void PORT_s_vInitI2C(void){

	/* Se configura los pines de I2C */

	/* Config. I2C0 */
	PORTA->PCR[PORT_enPIN2] |= PORT_PCR_MUX(PORT_enPINMUX3); /* PTA2 PinMux Control 3 I2C_SDA */
	PORTA->PCR[PORT_enPIN3] |= PORT_PCR_MUX(PORT_enPINMUX3); /* PTA3 PinMux Control 3 I2C_SCL */
}

static void PORT_s_vInitFlexIO(void)
{
	PORTA->PCR[PORT_enPIN0] |= PORT_PCR_MUX(PORT_enPINMUX4); /* PTA0 PinMux Control 4 FlexIO_I2C_SCL */
	PORTA->PCR[PORT_enPIN1] |= PORT_PCR_MUX(PORT_enPINMUX4); /* PTA1 PinMux Control 4 FlexIO_I2C_SDA */
}

/*	Public functions	*/

void PORT_vInitPorts(void){

	PORT_s_vEnableClocks();
	PORT_s_vInitIOs();
	PORT_s_vInitUARTs();
	PORT_s_vInitSPIs();
	PORT_s_vInitI2C();
	PORT_s_vInitFlexIO();
}

/* EOF */
