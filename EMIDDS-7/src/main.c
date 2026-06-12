/*	System Headers	*/
//#include "S32K144.h"
#include <DRIVER_MS8607/SER/Inc/MS8607_PTH.h>
#include "S32K144W.h"
#include "Platform_Types.h"
#include "Std_Types.h"
#include "system_S32K144W.h"

/*	Own Headers	*/
#include "Std_types.h"
#include "clocks_and_modes.h"
#include "UART_MCAL_cfg.h"
#include "UART_MCAL_def.h"
#include "UART_MCAL.h"
#include "SPI_MCAL_cfg.h"
#include "SPI_MCAL_def.h"
#include "SPI_MCAL.h"
#include "UserInterface.h"
#include "mem.h"
#include "Delay.h"

#include "swc_tem_pre_hum.h"
#include "I2C.h"
#include "FlexIO_I2C.h"
#include "PORT.h"

#include "time.h"
#include "swc_os.h"
#include "GPIO.h"

extern UART_tenUartsPorts UARTPort;

/*
 * Sensor debug
 */

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
/*	Prototypes	*/

/*******************************************************************************
Function Name : SysTick_Conf
Notes         :
*******************************************************************************/
void SysTick_Conf(void){

#ifdef Systick200ms
    /* SysTick Reload Value Register */
    S32_SysTick->RVR |= 0x00B927C00;        // 200ms (48Mhz Core clock)

    /* SysTick Control and Status Register */
    S32_SysTick->CSR |= 0x00000007;
    					// [2] CLKSOURCE =  1 Core clock
    					// [1] TICKINT   =  1 Counting down to zero asserts the SysTick exception request
    					// [0] ENABLE    =  1 Counter enabled
#else
    /* SysTick Reload Value Register */

    //S32_SysTick->RVR = (uint32)0x00249F00;        // x = 50ms * 48Mhz, x = 0x249F00
    S32_SysTick->RVR = (uint32)0x0001370F;        // S32_SysTick->LOAD = (uint32)0x0001370F = 1ms

    INT_SYS_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);

    S32_SysTick->CVR = 0;

    /* SysTick Control and Status Register */
    S32_SysTick->CSR |= 0x00000007;
    					// [2] CLKSOURCE =  1 Core clock
    					// [1] TICKINT   =  1 Counting down to zero asserts the SysTick exception request
    					// [0] ENABLE    =  1 Counter enabled

#endif
}


int main(void)
{
	PORT_vInitPorts();
	WDOG_Disable();
	SOSC_init_8MHz(); /* Initialize system oscillator for 8 MHz xtal */
	SPLL_init_160MHz(); /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
	NormalRUNmode_80MHz(); /* Init clocks: 80 MHz SPLL & core, 40 MHz bus, 20 MHz flash */
	/*Drivers Init*/
	Uart_Start();
	SPI_vInitMaster();
	GPIO_vInit();
	Flexio_Init_I2C();

	/*OS Init*/
	SystemCoreClockUpdate();
	SysTick_Conf();
	sensor_vInit();

	/*Init Time*/
	Time_init_time(23, 30);

	// Initialize the memory
	MEM_InitMemory();

	while(1)
	{
		if(Delay_count_1_minute())/*Called every 1m*/
		{
			Time_increase_minute();
			Task_1m();
		}

		if(Delay_count_1_second())/*Called every 1s*/
		{
			Task_1s();
		}
	}
}

/* END main */
