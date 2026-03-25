
/*	System Headers	*/
//#include "S32K144.h"
#include "S32K144W.h"
#include "S32K144W_features.h"
#include "Platform_Types.h"
#include "Std_Types.h"
#include "interrupt_manager.h"

/*	Own Headers	*/
#include "UART_MCAL_cfg.h"
#include "UART_MCAL_def.h"
#include "UART_MCAL.h"
#include "UserInterface.h"

static void vDelayMs(uint32 u32Time);

/*	Macros	*/

/*	Types	*/

typedef struct {
	uint32 VERID;                             /**< Version ID Register, offset: 0x0 */
	uint32 PARAM;                             /**< Parameter Register, offset: 0x4 */
	uint32 GLOBAL;                            /**< LPUART Global Register, offset: 0x8 */
	uint32 PINCFG;                            /**< LPUART Pin Configuration Register, offset: 0xC */
	uint32 BAUD;                              /**< LPUART Baud Rate Register, offset: 0x10 */
	uint32 STAT;                              /**< LPUART Status Register, offset: 0x14 */
	uint32 CTRL;                              /**< LPUART Control Register, offset: 0x18 */
	uint32 DATA;                              /**< LPUART Data Register, offset: 0x1C */
	uint32 MATCH;                             /**< LPUART Match Address Register, offset: 0x20 */
	uint32 MODIR;                             /**< LPUART Modem IrDA Register, offset: 0x24 */
	uint32 FIFO;                              /**< LPUART FIFO Register, offset: 0x28 */
	uint32 WATER;                             /**< LPUART Watermark Register, offset: 0x2C */
} UART_s_tstMemRegister;


#if (LPUART_INSTANCE_COUNT > 0U)
/* Implementation of LPUART0 handler named in startup code. */
void LPUART0_IrqHandler(void)
{
    //LPUART_DRV_IRQHandler(0);
}
#endif

#if (LPUART_INSTANCE_COUNT > 1U)
/* Implementation of LPUART1 handler named in startup code. */
void LPUART1_IrqHandler(void)
{
    //LPUART_DRV_IRQHandler(1);
	LPUART1_RxTx_IR();
}
#endif

#if (LPUART_INSTANCE_COUNT > 2U)
/* Implementation of LPUART2 handler named in startup code. */
void LPUART2_IrqHandler(void)
{
    //LPUART_DRV_IRQHandler(2);
}
#endif

#if (LPUART_INSTANCE_COUNT > 3U)
/* Implementation of LPUART3 handler named in startup code. */
void LPUART3_IrqHandler(void)
{
    //LPUART_DRV_IRQHandler(3);
}
#endif

/* Array storing references to LPUART irq handlers */
isr_t g_lpuartIsr[LPUART_INSTANCE_COUNT] =
{
#if (LPUART_INSTANCE_COUNT > 0U)
    LPUART0_IrqHandler,
#endif
#if (LPUART_INSTANCE_COUNT > 1U)
    LPUART1_IrqHandler,
#endif
#if (LPUART_INSTANCE_COUNT > 2U)
    LPUART2_IrqHandler,
#endif
#if (LPUART_INSTANCE_COUNT > 3U)
    LPUART3_IrqHandler,
#endif
};

/* Table to save LPUART enum numbers defined in CMSIS files. */
static const IRQn_Type s_lpuartRxTxIrqId[LPUART_INSTANCE_COUNT] = LPUART_RX_TX_IRQS;


/*	Private fuctions	*/

static void* UART_s_vSelectModule(uint8 u8Module){

	void *pAddress = (void*)NULL;

	if(u8Module < UART_enTotalUarts){
		switch (u8Module) {
			case UART_enUART0:
				pAddress = (void*)UART0_BASE_ADDR_cfg;
				break;
			case UART_enUART1:
				pAddress = (void*)UART1_BASE_ADDR_cfg;
				break;
			default:
				pAddress = (void*)UART2_BASE_ADDR_cfg;
				break;
		}
	}
	else{
		/*	 Nothing to do	*/
	}

	return pAddress;
}

static void UART_s_vInitPorts(void){

	/* Se inicializa el reloj del puerto y selecion de la funcion del pin */
	/* UART 0 */
	//PCC->PCCn[PCC_PORTC_INDEX ]|=PCC_PCCn_CGC_MASK; /* Habilita el reloj del puerto C */
	//PORTC->PCR[3] |= PORT_PCR_MUX(4);  /* PTC3 Pin Mux Control 4 UART TX */
	//PORTC->PCR[2] |= PORT_PCR_MUX(4);  /* PTC2 Pin Mux Control 4 UART RX */

	/* UART 1 */
	PCC->PCCn[PCC_PORTC_INDEX]|=PCC_PCCn_CGC_MASK; /* Habilita el reloj del puerto C */
	PORTC->PCR[7] |= PORT_PCR_MUX(2);  /* PTC7 Pin Mux Control 2 UART TX */
	PORTC->PCR[6] |= PORT_PCR_MUX(2);  /* PTC6 Pin Mux Control 2 UART RX */

	/* UART 2 */
	PCC->PCCn[PCC_PORTD_INDEX]|=PCC_PCCn_CGC_MASK; /* Habilita el reloj del puerto D */
	PORTD->PCR[7] |= PORT_PCR_MUX(2);  /* PTD7 Pin Mux Control 2 UART TX */
	PORTD->PCR[6] |= PORT_PCR_MUX(2);  /* PTD6 Pin Mux Control 2 UART RX */

}

/*	Public fuctions	*/

/*
* Name: void UART_vInit( void)
* Description: La funcion inicializa los relojes, registros y la funcion de los pines
				a usar para los puertos de UART.
* Parameters:
*
*/
void UART_vInit(void) /* Init. summary: 9600 baud, 1 stop bit, 8 bit format, no parity */
{
	UART_s_vInitPorts();

	/* UART 0 */
	//PCC->PCCn[PCC_LPUART0_INDEX] &= ~PCC_PCCn_CGC_MASK; /* Ensure clk disabled for config */
	//PCC->PCCn[PCC_LPUART0_INDEX] |= PCC_PCCn_PCS(0b001) /* Clock Src= 1 (SOSCDIV2_CLK) */
	//							 | PCC_PCCn_CGC_MASK; /* Enable clock for LPUART1 regs */

	//LPUART0->BAUD = 0x0F000034;  /* Baudios 9600 */
	//LPUART0->CTRL=0x000C0000;    /* Habilitacion de trasmision y recepcion, sin paridad, 8 bits*/

	/* UART 1 */
	PCC->PCCn[PCC_LPUART1_INDEX] &= ~PCC_PCCn_CGC_MASK; /* Ensure clk disabled for config */
	PCC->PCCn[PCC_LPUART1_INDEX] |= PCC_PCCn_PCS(0b001) /* Clock Src= 1 (SOSCDIV2_CLK) */
								 | PCC_PCCn_CGC_MASK; /* Enable clock for LPUART1 regs */

	LPUART1->BAUD = 0x0F000034;
	LPUART1->CTRL = 0x002C0000;


	/* UART 2 */
	PCC->PCCn[PCC_LPUART2_INDEX] &= ~PCC_PCCn_CGC_MASK; /* Ensure clk disabled for config */
	PCC->PCCn[PCC_LPUART2_INDEX] |= PCC_PCCn_PCS(0b001) /* Clock Src= 1 (SOSCDIV2_CLK) */
								 | PCC_PCCn_CGC_MASK; /* Enable clock for LPUART2 regs */

	LPUART2->BAUD = 0x0F000034;
	LPUART2->CTRL=0x000C0000;

}

void UART_vInitIRQs(void)
{
	INT_SYS_InstallHandler(s_lpuartRxTxIrqId[1], g_lpuartIsr[1], (isr_t*) 0);
	INT_SYS_EnableIRQ(s_lpuartRxTxIrqId[1]);
}

void UART_vPauseIRQs(void)
{
	LPUART1->CTRL |= 0x000C0000;
}

void UART_vResumeIRQs(void)
{
	LPUART1->CTRL |= 0x002C0000;
}
/*
* Name: uint8 UART_u8SendData(uint8 u8Module, uint8 u8Data)
* Description: La funcion se encarga de mandar un dato de tamano uint8 mediante
*			   el puerto UART selecionado.
* Parameters:
*			uint8 u8Module : [UART_enUART0 , UART_enTotalUarts) del enum UART_tenUartsPorts.
*			uint8 u8Data : Entrada que trasporta la informaicon de tama�o uint8.
* Return:
* 			uint8 u8Return	:	[E_OK, E_NOT_OK].
*
*/
uint8 UART_u8SendData(uint8 u8Module, uint8 u8Data){

	UART_s_tstMemRegister *pstUart;
	uint8 u8Return = (uint8)NOT_OK;

	if(u8Module < UART_enTotalUarts){

		pstUart = (UART_s_tstMemRegister*)UART_s_vSelectModule(u8Module);

		if(pstUart != NULL){
			/* Espera que el buffer este vacio */
			while((LPUART1->STAT & LPUART_STAT_TDRE_MASK)>>LPUART_STAT_TDRE_SHIFT == 0);
			/* Envia el dato */
			pstUart->DATA = u8Data;

			u8Return = (uint8)OK;
		}
		else{
			/*	 Nothing to do	*/
		}

	}
	else{
		/*	 Nothing to do	*/
	}

	return u8Return;

}

/*
* Name: uint8 UART_vSendBuffer(uint8 u8Module, uint8 u8Buffer[], uint8 lenght)
* Description: La funcion se encarga de mandar un buffer de datos determinado mediante
*			   el puerto UART selecionado.
* Parameters:
*			uint8 u8Module : [UART_enUart0 , UART_enTotalUarts) del enum UART_enTotalUartPorts.
*			uint8 u8Buffer[] :  Buffer que trasporta la informaicon de tama�o uint8.
*			uint8 lenght   : Tama�o del buffer [0, 255].
* Return:
* 			uint8 u8Return	:	[E_OK, E_NOT_OK].
*
*/
static void vDelayMs(uint32 u32Time){
	for(uint32 u32x = 0; u32x < u32Time; u32x++){

		for(uint16 u16x = 0; u16x < 7000; u16x++){
			/*	Nothing to do	*/
		}

	}
}
uint8 UART_vSendBuffer(uint8 u8Module, uint8 u8Buffer[], uint32 lenght)
{

	uint8 u8Return = (uint8)OK;
	uint32 u8i = (uint8)0U;

	if(u8Module < UART_enTotalUarts)
	{

		while((u8i < lenght) && (u8Return == OK ))
		{

			u8Return = (uint8)UART_u8SendData(u8Module, u8Buffer[u8i]);
			u8i++;
			//vDelayMs(200);
		}
	}
	else
	{

		u8Return = (uint8)NOT_OK;
	}
}

/*
* Name: uint8 UART_u8SendString(uint8 u8Module, uint8* pu8DataBuffer[])
* Description: La funcion se encarga de mandar un string.
* Parameters:
*			uint8 u8Module : [UART_enUART0 , UART_enTotalUarts) del enum UART_tenUartsPorts.
*			uint8 u8DataBuffer : String.
* Return:
* 			uint8 u8Return	:	[E_OK, E_NOT_OK].
*
*/

uint8 UART_u8SendString(uint8 u8Module, uint8 u8DataBuffer[]){

	UART_s_tstMemRegister *pstUart;
	uint32 u32i = (uint32)0;
	uint8 u8Return = (uint8)NOT_OK;

	if(u8Module < UART_enTotalUarts){

		pstUart = (UART_s_tstMemRegister*)UART_s_vSelectModule(u8Module);

		if(pstUart != NULL){
			while(u8DataBuffer[u32i] != '\0') { /* Send chars one at a time */
				u8Return = UART_u8SendData(u8Module, u8DataBuffer[u32i]);
				u32i++;
			}
		}
		else{
			/*	 Nothing to do	*/
			}
	}
	else{
		/*	 Nothing to do	*/
	}

	return u8Return;

}

/*
* Name: uint8 UART_vReceiveData(uint8 u8Module, uint8* u8Data)
* Description: La funcion se encarga de recibir un dato de tamano uint8 mediante
*			   el puerto UART selecionado.
* Parameters:
*			uint8 u8Module : [UART_enUART0 , UART_enTotalUarts) del enum UART_tenUartsPorts.
*			uint8* pu8Data : Puntero que almacena la informacion recivida.
* Return:
* 			uint8 u8Return	:	[E_OK, E_NOT_OK].
*
*/
void UART_u8ReceiveData(uint8 instance, uint8 *pu8Data)
{

	LPUART_Type * base = s_lpuartBase[instance];

	*pu8Data = (uint8_t)base->DATA;

	LPUART1->STAT |= LPUART_STAT_RDRF_MASK;

}

/* EOF*/
