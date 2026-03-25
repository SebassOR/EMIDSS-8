/*
 * GPIO.c
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

/* Headers */
//#include "PORT_def.h"

/*	Own headers */
#include "GPIO_cfg.h"
#include "GPIO_def.h"
#include "GPIO.h"

/*	Macros	*/

/*	Defines	*/

#define GPIO_OneMask	(1U)
/*Desired PIN is PTC2*/
/*	Types	*/

/* Variables */

static GPIO_stGPIOPin astGpioOuts[GPIO_enTotalOutputs] = GPIO_Outputs_table_cfg;
static GPIO_stGPIOPin astGpioInputs[GPIO_enTotalInputs] = GPIO_Inputs_table_cfg;

/*	Private fuctions */

static void* GPIO_s_vPORT(uint8 u8Port){

	void *pAddress = (void*)NULL;

	switch (u8Port) {
		case GPIO_enPortA:
			pAddress = (void*)PTA;
			break;
		case GPIO_enPortB:
			pAddress = (void*)PTB;
			break;
		case GPIO_enPortC:
			pAddress = (void*)PTC;
			break;
		case GPIO_enPortD:
			pAddress = (void*)PTD;
			break;
		case GPIO_enPortE:
			pAddress = (void*)PTE;
			break;
		default:
			/*	Nothing to do	*/
			break;
	}

	return pAddress;
}

/*	Public fuctions	*/

void GPIO_vInit(void)
{
	/*INITIALIZE PTC2 AS OUTPUT*/
	PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;
	PORTC->PCR[PORT_enPIN2] = PORT_PCR_MUX(PORT_enPINMUX1);  /* PTC2 PinMux Control 1 USER_LED */
	PTC -> PDDR |= GPIO_OneMask<<PORT_enPIN2;

}

void GPIO_u32SetOutputState(uint8 u8State)
{
	if(u8State == (uint8)STD_HIGH)
	{
		/*Set HIGH PTC2*/
		PTC-> PCOR |= GPIO_OneMask<<PORT_enPIN2;
	}
	else
	{
		/*Set LOW PTC2*/
		PTC-> PSOR |= GPIO_OneMask<<PORT_enPIN2;
	}
}

void GPIO_ToggleOutputState(void)
{
	/*This function have its own sequece to change the output. If the output is changed by other source. 
	This will be altered. It will take 2 more cycles to return to its normal cycle						*/

	static uint8_t Flag_GPIO = 0;

	if(Flag_GPIO == 0)/*ToogleLed*/
	{
		GPIO_u32SetOutputState(STD_LOW);
		Flag_GPIO = 1;
	}
	else
	{
		GPIO_u32SetOutputState(STD_HIGH);
		Flag_GPIO = 0;
	}
}

/* EOF */
