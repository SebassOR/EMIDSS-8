/*
 * SPI_MCAL.c
 *
 *  Created on: Feb 5, 2024
 *      Author: jarsr
 */

/* System Headers*/
//#include "S32K144.h"
#include "S32K144W.h"

/*	Headers */
#include "SPI_MCAL_cfg.h"
#include "SPI_MCAL_def.h"
#include "SPI_MCAL.h"

/*	Macros	*/

/*	Types	*/

/*	Private fuctions	*/


static void* SPI_s_vSelectModule(uint8 u8SpiModule){

	void *pAddress = (void*)NULL;

	if(u8SpiModule == (uint8)SPI_enSPI0){

		pAddress = (void*)LPSPI0_BASE_ADDR_cfg;

	}
	else if(u8SpiModule == (uint8)SPI_enSPI2){

		pAddress = (void*)LPSPI2_BASE_ADDR_cfg;

	}
	else{
		/*	 Nothing to do	*/
	}

	return pAddress;
}

void SPI_s_vInitPorts(void){

	/*	Enable clocks	*/
	PCC->PCCn[PCC_PORTB_INDEX]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTB */
	PCC->PCCn[PCC_PORTC_INDEX]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTC */
	PCC->PCCn[PCC_PORTD_INDEX]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTD */

	/*	Config. SPI0	*/
	PORTD->PCR[15]|=PORT_PCR_MUX(4); /* Port D15: MUX = ALT4, LPSPI0_SCK reloj */
	PORTD->PCR[16]|=PORT_PCR_MUX(4); /* Port D16: MUX = ALT4, LPSPI0_SIN/MISO */
	PORTB->PCR[4]|=PORT_PCR_MUX(3); /* Port B4: MUX = ALT3, LPSPI0_SOUT/MOSI */
	//PORTB->PCR[5]|=PORT_PCR_MUX(4); /* Port B5: MUX = ALT4, LPSPI0_PCS0 */
	PTB->PDDR |= 1 << PTB5;
	PORTB->PCR[5]|=PORT_PCR_MUX(1);
	PTB->PSOR |= 1 << PTB5;			// Set initial state to high

	/*	Config. SPI2	*/
	PORTC->PCR[15]|=PORT_PCR_MUX(3); /* Port C15: MUX = ALT3, LPSPI2_SCK reloj */
	PORTC->PCR[0]|=PORT_PCR_MUX(3); /* Port C0: MUX = ALT3, LPSPI2_SIN/MISO */
	PORTC->PCR[1]|=PORT_PCR_MUX(3); /* Port C1: MUX = ALT3, LPSPI2_SOUT/MOSI */
	PORTC->PCR[14]|=PORT_PCR_MUX(3); /* Port C14: MUX = ALT3, LPSPI2_PCS0 */

}

/*	Public fuctions	*/

void SPI_vInitMaster(void){

	LPSPI_Type *pstSPI;

	for(uint8 u8x = 0; u8x < SPI_enTotalSPIs; u8x++){

		if(u8x == (uint8)SPI_enSPI0){
			PCC->PCCn[PCC_LPSPI0_INDEX] = 0; 			/* Disable clocks to modify PCS ( default) */
			PCC->PCCn[PCC_LPSPI0_INDEX] = 0xC6000000; 	/* Enable PCS=SPLL_DIV2 (40 MHz func'l clock) */
			pstSPI = (LPSPI_Type*)SPI_s_vSelectModule(u8x);
		}
		else if(u8x == (uint8)SPI_enSPI2){
			PCC->PCCn[PCC_LPSPI2_INDEX] = 0; 			/* Disable clocks to modify PCS ( default) */
			PCC->PCCn[PCC_LPSPI2_INDEX] = 0xC6000000; 	/* Enable PCS=SPLL_DIV2 (40 MHz func'l clock) */
			pstSPI = (LPSPI_Type*)SPI_s_vSelectModule(u8x);
		}
		else{
			/*	Nothing to do	*/
		}

		pstSPI->CR = 0x00000000; 	/* Disable module for configuration */

		pstSPI->IER = 0x00000000; 	/* Interrupts not used */

		pstSPI->DER = 0x00000000; 	/* DMA not used */

		pstSPI->CFGR0 = 0x00000000; /* Defaults: */
									/* RDM0=0: rec'd data to FIFO as normal */
									/* CIRFIFO=0; Circular FIFO is disabled */
									/* HRSEL, HRPOL, HREN=0: Host request disabled */

		pstSPI->CFGR1 = 0x00000001; /* Configurations: master mode*/
									/* PCSCFG=0: PCS[3:2] are enabled */
									/* OUTCFG=0: Output data retains last value when CS negated */
									/* PINCFG=0: SIN is input, SOUT is output */
									/* MATCFG=0: Match disabled */
									/* PCSPOL=0: PCS is active low */
									/* NOSTALL=0: Stall if Tx FIFO empty or Rx FIFO full */
									/* AUTOPCS=0: does not apply for master mode */
									/* SAMPLE=0: input data sampled on SCK edge */
									/* MASTER=1: Master mode */

		pstSPI->TCR = 0x5000001F; 	/* Transmit cmd: PCS0, 32bits, prescale func'l clk by 4. */
		//pstSPI->TCR = 0x50000007; 	/* Transmit cmd: PCS0, 32bits, prescale func'l clk by 4. */
									/* CPOL=0: SCK inactive state is low */
									/* CPHA=1: Change data on SCK lead'g, capture on trail'g edge*/
									/* PRESCALE=2: Functional clock divided by 2**2 = 4 */
									/* PCS=0: Transfer using PCS0 */
									/* LSBF=0: Data is transferred MSB first */
	/*pstSPI->TCR = 0x5080001F*/	/* LSBF=1: Data is transferred LSB first */
									/* BYSW=0: Byte swap disabled */
									/* CONT, CONTC=0: Continuous transfer disabled */
									/* RXMSK=0: Normal transfer: rx data stored in rx FIFO */
									/* TXMSK=0: Normal transfer: data loaded from tx FIFO */
									/* WIDTH=0: Single bit transfer */
									/* FRAMESZ=15: # bits in frame = 15+1=16 */
		/*pstSPI->TCR = 0x5000001F;*/ /* FRAMESZ=31: # bits in frame = 31+1=32 */

		pstSPI->CCR = 0x04090808; 	/* Clk dividers based on prescaled func'l clk of 100 nsec */
									/* SCKPCS=4: SCK to PCS delay = 4+1 = 5 (500 nsec) */
									/* PCSSCK=4: PCS to SCK delay = 9+1 = 10 (1 usec) */
									/* DBT=8: Delay between Transfers = 8+2 = 10 (1 usec) */
									/* SCKDIV=8: SCK divider =8+2 = 10 (1 usec: 1 MHz baud rate) */

		pstSPI->FCR = 0x00000003; 	/* RXWATER=0: Rx flags set when Rx FIFO >0 */
									/* TXWATER=3: Tx flags set when Tx FIFO <= 3 */

		pstSPI->CR = 0x00000009; 	/* Enable module for operation */
									/* DBGEN=1: module enabled in debug mode */
									/* DOZEN=0: module enabled in Doze mode */
									/* RST=0: Master logic not reset */
									/* MEN=1: Module is enabled */

	}

	SPI_s_vInitPorts();

//	PCC->PCCn[PCC_LPSPI0_INDEX] = 0; 			/* Disable clocks to modify PCS ( default) */
//	PCC->PCCn[PCC_LPSPI0_INDEX] = 0xC6000000; 	/* Enable PCS=SPLL_DIV2 (40 MHz func'l clock) */
//
//	LPSPI0->CR = 0x00000000; 	/* Disable module for configuration */
//
//	LPSPI0->IER = 0x00000000; 	/* Interrupts not used */
//
//	LPSPI0->DER = 0x00000000; 	/* DMA not used */
//
//	LPSPI0->CFGR0 = 0x00000000; /* Defaults: */
//								/* RDM0=0: rec'd data to FIFO as normal */
//								/* CIRFIFO=0; Circular FIFO is disabled */
//								/* HRSEL, HRPOL, HREN=0: Host request disabled */
//
//	LPSPI0->CFGR1 = 0x00000001; /* Configurations: master mode*/
//								/* PCSCFG=0: PCS[3:2] are enabled */
//								/* OUTCFG=0: Output data retains last value when CS negated */
//								/* PINCFG=0: SIN is input, SOUT is output */
//								/* MATCFG=0: Match disabled */
//								/* PCSPOL=0: PCS is active low */
//								/* NOSTALL=0: Stall if Tx FIFO empty or Rx FIFO full */
//								/* AUTOPCS=0: does not apply for master mode */
//								/* SAMPLE=0: input data sampled on SCK edge */
//								/* MASTER=1: Master mode */
//
//	LPSPI0->TCR = 0x5000001F; 	/* Transmit cmd: PCS0, 32bits, prescale func'l clk by 4. */
//								/* CPOL=0: SCK inactive state is low */
//								/* CPHA=1: Change data on SCK lead'g, capture on trail'g edge*/
//								/* PRESCALE=2: Functional clock divided by 2**2 = 4 */
//								/* PCS=0: Transfer using PCS0 */
//								/* LSBF=0: Data is transferred MSB first */
//								/* BYSW=0: Byte swap disabled */
//								/* CONT, CONTC=0: Continuous transfer disabled */
//								/* RXMSK=0: Normal transfer: rx data stored in rx FIFO */
//								/* TXMSK=0: Normal transfer: data loaded from tx FIFO */
//								/* WIDTH=0: Single bit transfer */
//								/* FRAMESZ=15: # bits in frame = 15+1=16 */
//	/*LPSPI0->TCR = 0x5000001F;*/ /* FRAMESZ=31: # bits in frame = 31+1=32 */
//
//	LPSPI0->CCR = 0x04090808; 	/* Clk dividers based on prescaled func'l clk of 100 nsec */
//								/* SCKPCS=4: SCK to PCS delay = 4+1 = 5 (500 nsec) */
//								/* PCSSCK=4: PCS to SCK delay = 9+1 = 10 (1 usec) */
//								/* DBT=8: Delay between Transfers = 8+2 = 10 (1 usec) */
//								/* SCKDIV=8: SCK divider =8+2 = 10 (1 usec: 1 MHz baud rate) */
//
//	LPSPI0->FCR = 0x00000003; 	/* RXWATER=0: Rx flags set when Rx FIFO >0 */
//								/* TXWATER=3: Tx flags set when Tx FIFO <= 3 */
//
//	LPSPI0->CR = 0x00000009; 	/* Enable module for operation */
//								/* DBGEN=1: module enabled in debug mode */
//								/* DOZEN=0: module enabled in Doze mode */
//								/* RST=0: Master logic not reset */
//								/* MEN=1: Module is enabled */


//	SPI_s_vInitPorts();

}


uint16 SPI_CS(uint8 u8SpiModule, uint8 state){

	uint16 u16Return = (uint16)NOT_OK;
	LPSPI_Type *pstSPI;

	if(u8SpiModule < (uint8)SPI_enTotalSPIs){
		pstSPI = (LPSPI_Type*)SPI_s_vSelectModule(u8SpiModule);
		if(pstSPI != NULL)
		{
			if(u8SpiModule == (uint8)SPI_enSPI0)
			{
				if(state == 1)
				{
					// Wait for module to be idle
					while((pstSPI->SR & LPSPI_SR_MBF_MASK) >> LPSPI_SR_MBF_SHIFT == 1);
					// Put pin to high
					PTB->PSOR |= 1 << PTB5;
				}
				else if(state == 0)
				{
					// Put pin to low
					PTB->PCOR |= 1 << PTB5;
				}
			}
			else if(u8SpiModule == (uint8)SPI_enSPI2)
			{

			}
			else{
				/*	Nothing to do	*/
			}
		}
		else{
			/*	Nothing to do	*/
		}
	}
	else{
		/*	Nothing to do	*/
	}

	return u16Return;
}




uint16 SPI_u8Transmit(uint8 u8SpiModule, uint8 *data, uint16 size)
{
	uint16 u16Return = (uint16)NOT_OK;
	LPSPI_Type *pstSPI;

	if(u8SpiModule < (uint8)SPI_enTotalSPIs){

		pstSPI = (LPSPI_Type*)SPI_s_vSelectModule(u8SpiModule);

		if(pstSPI != NULL){

			// Clean Tx buffer
			pstSPI->CR |= 1 << LPSPI_CR_RTF_SHIFT;

			// Configure the FRAMESZ based on the input size
			pstSPI->TCR = 0x10080000 | (8 - 1);

			// Send all 8-bit frames that we can handle
			for(int i=0; i<size; i++)
			{
				//	Wait for the FIFO to be available
				while((pstSPI->SR & LPSPI_SR_TDF_MASK)>>LPSPI_SR_TDF_SHIFT == 0);
				// Transmit data
				pstSPI->TDR = data[i];
				// Clean TDF flag
				pstSPI->SR |= LPSPI_SR_TDF_MASK;
			}

			// Wait for all transfers to complete
			while((pstSPI->FSR & LPSPI_FSR_TXCOUNT_MASK) >> LPSPI_FSR_TXCOUNT_SHIFT != 0);

			u16Return = (uint16)OK;
		}
		else{
			/*	Nothing to do	*/
		}
	}
	else{
		/*	Nothing to do	*/
	}

	return u16Return;
}


uint16 SPI_u8ReceiveData(uint8 u8SpiModule, uint8* data, uint16 size)
{

	uint16 u16Return = (uint16)NOT_OK;
	LPSPI_Type *pstSPI;
	uint32 buffer=0;

	if(u8SpiModule < (uint8)SPI_enTotalSPIs){

		pstSPI = (LPSPI_Type*)SPI_s_vSelectModule(u8SpiModule);

		if(pstSPI != NULL){

			// Clean Tx and Rx buffer
			pstSPI->CR |= 1 << LPSPI_CR_RTF_SHIFT;
			pstSPI->CR |= 1 << LPSPI_CR_RRF_SHIFT;

			// Read all 32-bit frames that we can handle
			for(int i=0; i<size; i++)
			{
				// Configure the FRAMESZ based on the input size
				pstSPI->TCR = 0x10040000 | (8 - 1);

				/*	Wait for the FIFO to be available	*/
				while((pstSPI->SR & LPSPI_SR_RDF_MASK)>>LPSPI_SR_RDF_SHIFT == 0);
				/* Read data */
				buffer = LPSPI0->RDR;

				data[i] = (buffer) & 0xFF ;

				/* Clean RDF flag */
				pstSPI->SR |= LPSPI_SR_RDF_MASK;
			}

			/* Clean TCF flag */
			pstSPI->SR |= LPSPI_SR_TCF_MASK;

			u16Return = (uint16)OK;

		}
		else{
			/*	Nothing to do	*/
		}
	}
	else{
		/*	Nothing to do	*/
	}

	return u16Return;

}

uint16 SPI_u16SendData(uint8 u8SpiModule, uint16 u16Data){

	uint16 u16Return = (uint16)NOT_OK;
	LPSPI_Type *pstSPI;

	if(u8SpiModule < (uint8)SPI_enTotalSPIs){

		pstSPI = (LPSPI_Type*)SPI_s_vSelectModule(u8SpiModule);

		if(pstSPI != NULL){
			/*	Espera al que este libre	*/
			while((pstSPI->SR & LPSPI_SR_TDF_MASK)>>LPSPI_SR_TDF_SHIFT == 0);
			/* Transmit data */
			pstSPI->TDR = (uint16)u16Data;
			/* Se limpia la bandera TDF */
			pstSPI->SR |= LPSPI_SR_TDF_MASK;

			u16Return = (uint16)OK;

		}
		else{
			/*	Nothing to do	*/
		}
	}
	else{
		/*	Nothing to do	*/
	}

	return u16Return;
}

uint16 SPI_u16ReciveData(uint8 u8SpiModule, uint16* u16Data){

	uint16 u16Return = (uint16)NOT_OK;
	LPSPI_Type *pstSPI;

	if(u8SpiModule < (uint8)SPI_enTotalSPIs){

		pstSPI = (LPSPI_Type*)SPI_s_vSelectModule(u8SpiModule);

		if(pstSPI != NULL){
			/*	Espera al que este libre	*/
			while((pstSPI->SR & LPSPI_SR_RDF_MASK)>>LPSPI_SR_RDF_SHIFT == 0);
			/* Lee el dato recibido */
			*u16Data = (uint16)LPSPI0->RDR;
			 /* Se limpia la bandera RDF */
			LPSPI0->SR |= LPSPI_SR_RDF_MASK;

			u16Return = (uint16)OK;

		}
		else{
			/*	Nothing to do	*/
		}
	}
	else{
		/*	Nothing to do	*/
	}

	return u16Return;

}

/* EOF*/
