/*
 * I2C.c
 *
 *  Created on: Mar 15, 2024
 *  Author: JARS
 *  Version: 1.0v
 *  Ultima modif.: Jun 9, 2024
 *  Author: Noe Ortiz Terrones
 *
 */

/* System Headers*/
#include "S32K144W.h"
#include "Platform_Types.h"
#include "Std_Types.h"

/* Headers */
#include "PORT_cfg.h"
#include "Delay.h"

/*	Own headers */
#include "I2C.h"

/*	Macros	*/

/*	Defines	*/

/*	Types	*/

/* Variables */

/*	Private fuctions */
static Std_ReturnType I2C_s_vStartACK(uint8_t u8SlaveAddr, uint8_t u8Operation){
	uint32_t command;
	Std_ReturnType error = OK;

	//Check Bus Arbitration Lost Flag ALF
	if(LPI2C0->MSR & LPI2C_MSR_ALF_MASK){
		error = NOT_OK;
	}
	else{
		// Read
		if(u8Operation){
			command = LPI2C_MTDR_ADDRESS_R(u8SlaveAddr);
		}
		//Write
		else{
			command = LPI2C_MTDR_ADDRESS_W(u8SlaveAddr);
		}

		command |= LPI2C_MTDR_CMD(I2C_GEN_START_CMD);
		LPI2C0->MTDR = command;
	}

	return error;
}

static Std_ReturnType I2C_s_u8DetectNack(void){
	Std_ReturnType result;

	//Wait until bus is free
	Delay_vMs(10);
	//Detect an NACK
	if(LPI2C0->MSR & LPI2C_MSR_NDF_MASK){
		result = NOT_OK;
	}
	else{
		result = OK;
	}

	return result;
}

static void I2C_vStop(void){
	LPI2C0->MTDR = LPI2C_MTDR_CMD(I2C_GEN_STOP_CMD);
}

static void I2C_vResetBus(void){
	//Software reset (MCR->RST)
	LPI2C0->MCR |= 0x2;
	LPI2C0->MCR &= 0xFFFFFFFD;
	I2C_vInit();
}

/*	Public fuctions	*/

void I2C_vInit(void){

	/* Se hablita el reloj del periferico */
	PCC->PCCn[PCC_LPI2C0_INDEX] |= PCC_PCCn_PCS(2);   /* SIRCDIV2_CLK (8 MHz) */
	PCC->PCCn[PCC_LPI2C0_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clock for LPI2C0 regs */

    uint32_t regValue = (uint32_t)LPI2C0->MCR;
    regValue &= (uint32_t)(~(LPI2C_MCR_MEN_MASK));
    regValue |= LPI2C_MCR_MEN(0);
    LPI2C0->MCR = (uint32_t)regValue;

    LPI2C0->MCCR0 = 0x04090909; /* Deshabilitacion de trasmision y recepcion en inicio */
      	  	  	  	  	  	  	/* DATAVD = 4, 0x04, numero minimo de ciclos  */
    							/* SETHOLD = 9, 0x09, minimum number of cycles (minus one) that is used by the master  */
    							/* CLKHI = 9, 0x09, Clock High Period  */
								/* CLKLO = 9, 0x09, Clock Low Period  */

     /* Master Interrupt Enable Register (MIER) */
    LPI2C0->MIER = 0x00000000;
      	  	  	  	  	  	  // [14] DMIE = 0  (Data match interrupt disabled)
      	  	  	  	  	  	  // [13] PLTIE = 0 (Pin low timeout interrupt disabled)
      	  	  	  	  	  	  // [12] FEIE = 0  (FIFO error interrupt disabled)
      	  	  	  	  	  	  // [11] ALIE = 0  (Arbitration lost interrupt disabled)
      	  	  	  	  	  	  // [10] NDIE = 0  (NACK detect interrupt disabled)
      	  	  	  	  	  	  // [9]  SDIE = 0  (STOP detect interrupt disabled)
      	  	  	  	  	  	  // [8]  EPIE = 0  (End packet interrupt disabled)
    						  // [1]  RDIE = 0  (Receive Data Interrupt disabled)
    						  // [0]  TDIE = 0  (Transmit Data Interrupt disabled)

    /* Master Configuration Register 0 (MCFGR0) */
    LPI2C0->MCFGR0 = 0x00000000;
    							// [9] RDMO = 0    (Received data is stored in the receive FIFO as normal)
     							// [8] CIRFIFO = 0 (Circular FIFO is disabled)
    							// [2] HRSEL = 0   (Host request input is pin HREQ)
    							// [1] HRPOL = 0   (Active low)
    							// [0] HREN = 0    (Host request input is disabled)

    /* Master Configuration Register 1 (MCFGR1) */
    LPI2C0->MCFGR1 = 0x00000800;
      	  	  	  	  	  	  	  // [26-14] PINCFG     = 0 (LPI2C configured for 2-pin open drain mode)
      	  	  	  	  	  	  	  // [16-18] MATCFG     = 0 (Match disabled)
      	  	  	  	  	  	  	  // [10]    TIMECFG    = 0     (Pin Low Timeout Flag will set if either SCL or SDA is low for longer than the configured timeout)
      	  	  	  	  	  	  	  // [9]     IGNACK     = 0     (LPI2C Master will receive ACK and NACK normally)
      	  	  	  	  	  	  	  // [8]     AUTOSTOP   = 0     (Without autostop generation)
      	  	  	  	  	  	  	  // [2-0]   PRESCALE   = 0 (Divided by 1)

    /* Master Configuration Register 2 (MCFGR2) */
    LPI2C0->MCFGR2 = 0x00000000;
      	  	  	  	  	  	  	  // [27-24] FILTSDA = 0  (Disable the glitch filter)
      	  	  	  	  	  	  	  // [19-16] FILTSDA = 0  (Disable the glitch filter)
      	  	  	  	  	  	  	  // [11-0]  BUSIDLE = 0  (Bus idle timeout period in clock cycles)

    /* Master Configuration Register 3 (MCFGR3) */
    LPI2C0->MCFGR3 = 0x00000000;
      	  	  	  	  	  	  // [19-8] PINLOW = 0 (Pin Low Timeout enabled)

    /* Master FIFO Control Register (MFCR) */
    //LPI2C0->MFCR = 0x00030000;
    LPI2C0->MFCR = 0x00000000;
      	  	  	  	  	  	  // [17-16] RXWATER = 3, 0x03, (Receive FIFO watermark)
      	  	  	  	  	  	  // [1-0]   TXWATER = 0 (Transmit FIFO watermark)

     // Master Control Register
    LPI2C0->MCR = 0x00000009;
      	  	  	  	  	  // [9] RRF = 0   (Receive FIFO is reset)
      	  	  	  	  	  // [8] RTF = 0   (Transmit FIFO is reset)
      	  	  	  	  	  // [3] DBGEN = 1 (Master is enabled in debug mode)
      	  	  	  	  	  // [2] DOZEN = 0 (Master is disabled in Doze mode)
      	  	  	  	  	  // [1] RST = 0   (Master logic is not reset)
      	  	  	  	  	  // [0] MEN = 1   (Master logic is enabled)


    regValue = (uint32_t)LPI2C0->MCR;
    regValue &= (uint32_t)(~(LPI2C_MCR_MEN_MASK));
    regValue |= LPI2C_MCR_MEN(1);
    LPI2C0->MCR = (uint32_t)regValue;
}

Std_ReturnType I2C_u8WriteCmd(uint8_t u8SlaveAddr, uint8_t u8Cmd){
	Std_ReturnType error = NOT_OK;

	//Stop detect flag
	if(LPI2C0->MSR & (LPI2C_MSR_SDF_WIDTH << LPI2C_MSR_SDF_SHIFT)){
		LPI2C0->MSR |= (LPI2C_MSR_SDF_WIDTH << LPI2C_MSR_SDF_SHIFT); // Limpia la bandera de Stop
	}

	error = I2C_s_vStartACK(u8SlaveAddr, WRITE);

	//Arbitration error
	if(!error){
		LPI2C0->MTDR = u8Cmd;
		error = I2C_s_u8DetectNack();

		//NACK error
		if(!error){
			I2C_vStop();
		}
		else{
			//Do nothing
		}
	}
	else{
		//Do nothing
	}

	if(error){
		I2C_vResetBus();
	}


	return error;
}

Std_ReturnType I2C0_u8Write(uint8_t u8SlaveAddr, uint8_t u8Register, uint8_t u8Data){
	Std_ReturnType error = OK;
	uint8_t u8Return = (uint8_t)E_NOT_OK;
	uint16_t u16TimeOut = (uint16_t)0;

	/* Revisa que el puerto no este ocupado */
    while ((LPI2C0->MSR & (LPI2C_MSR_BBF_WIDTH << LPI2C_MSR_BBF_SHIFT)) && (u16TimeOut < TimeOutI2C)){
    	u16TimeOut++;
    }

    if(u16TimeOut <= TimeOutI2C){
    	/* Generate (repeated) START and transmit address in DATA[7:0], in write mode */
    	error = I2C_s_vStartACK(u8SlaveAddr, WRITE);

    	//Arbitration error
    	if(!error){
			/* Se envia el registro */
			LPI2C0->MTDR = (uint8_t)u8Register;
			/* Se envia el byte o dato a enviar */
			LPI2C0->MTDR = (uint8_t)u8Data;
			error = I2C_s_u8DetectNack();

			//NACK error
			if(!error){
				u16TimeOut = (uint16_t)0;
				I2C_vStop();

				/* Espera a que se genere la señal de stop */
				while ((LPI2C0->MSR & (LPI2C_MSR_SDF_WIDTH << LPI2C_MSR_SDF_SHIFT)) && (u16TimeOut < StopTimeI2C)){
					u16TimeOut++;
				}

				if(u16TimeOut <= StopTimeI2C){
					u8Return = (uint8_t)OK;
				}
				else{
					u8Return = (uint8_t)NOT_OK;
				}

				/* Si la bandera esta prendida */
				if(LPI2C0->MSR & (LPI2C_MSR_SDF_WIDTH << LPI2C_MSR_SDF_SHIFT)){

					LPI2C0->MSR |= (LPI2C_MSR_SDF_WIDTH << LPI2C_MSR_SDF_SHIFT); // Limpia la bandera de Stop

				}
				else{
					/* Nothing to do */
				}
			}
			else{
				//NACK detected
			}
    	}
    	else{
    		u8Return = error;
    	}
    }
    else{
    	//Timeout exceeded
    }

    if(error){
		I2C_vResetBus();
	}


    return u8Return;
}

Std_ReturnType I2C0_u8Read(uint8_t u8SlaveAddr, uint8_t *pu8Data, uint8_t u8NumData){
	Std_ReturnType error = OK;
	uint16_t u16TimeOut = (uint16_t)0;
	uint16_t cmd = LPI2C_MTDR_CMD(I2C_RECEIVE_CMD);

	/* Revisa que el puerto no este ocupado */
	while ((LPI2C0->MSR & (LPI2C_MSR_BBF_WIDTH << LPI2C_MSR_BBF_SHIFT)) && (u16TimeOut < TimeOutI2C)){
		u16TimeOut++;
	}

	if(u16TimeOut <= TimeOutI2C){
		error = I2C_s_vStartACK(u8SlaveAddr, READ);

		//Arbitration error
		if(!error){
			error = I2C_s_u8DetectNack();
			u16TimeOut = 0;

			//NACK error
			if(!error){
				//Receive (DATA[7:0] + n) bytes
				cmd |= u8NumData - 1;
				LPI2C0->MTDR = cmd;

				for(uint8_t i=0;i<u8NumData;i++){
					Delay_vMs(1);
					pu8Data[i] = LPI2C0->MRDR;
				}
			}
			else{
				//NACK detected
			}
			I2C_vStop();
		}
	}
	else{
		//Timeout exceeded
	}

	if(error){
		I2C_vResetBus();
	}


	return error;
}

/* EOF */
