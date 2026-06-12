/*
 * FlexIO_I2C.c
 *
 *  Created on: 25 abr. 2025
 *      Author: Hector Hdez
 */

#include "FlexIO_I2C.h"
#include "S32K144W.h"


void Flexio_Init_I2C (void)
{

	  PCC->PCCn[PCC_FlexIO_INDEX ]|=PCC_PCCn_PCS(1) 	/* FlexIO src clk: System oscillator */
			  	  	  	  	  	  | PCC_PCCn_CGC_MASK; 	/* Enable clock for PORTE */
	  /* Transmit shifter config */
	  FLEXIO->SHIFTCTL[0]=FLEXIO_SHIFTCTL_SMOD(2) 		/* Shifter mode: Transmit */
						|FLEXIO_SHIFTCTL_PINPOL(1) 		/* Pin active low */
						|FLEXIO_SHIFTCTL_PINSEL(3) 		/* FXDIO3 as SDA*/
						|FLEXIO_SHIFTCTL_PINCFG(1) 		/* Pin config: Open drain*/
						|FLEXIO_SHIFTCTL_TIMPOL(0) 		/* Shift on clk Posedge */
						|FLEXIO_SHIFTCTL_TIMSEL(1);  	/* Shifter Timer 1 is selected for SDA*/
	  FLEXIO->SHIFTCFG[0]=FLEXIO_SHIFTCFG_SSTART(2)  	/* Set shifter start bit to logic low level */
						 |FLEXIO_SHIFTCFG_SSTOP(3)   	/* Stop bit high */
						 |FLEXIO_SHIFTCFG_INSRC(0);  	/* Input source is the pin */

	  /* Receiver shifter config */
	  FLEXIO->SHIFTCTL[1]=FLEXIO_SHIFTCTL_SMOD(1) 	 /* Shifter mode: Receive */
						|FLEXIO_SHIFTCTL_PINPOL(0) 	 /* Pin active high */
						|FLEXIO_SHIFTCTL_PINSEL(3) 	 /* FXDIO3 as SDA*/
						|FLEXIO_SHIFTCTL_PINCFG(0) 	 /* Pin config: output disabled*/
						|FLEXIO_SHIFTCTL_TIMPOL(1) 	 /* Shift on clk negedge */
						|FLEXIO_SHIFTCTL_TIMSEL(1);  /* Shifter Timer 1 is selected for SDA*/
	  FLEXIO->SHIFTCFG[1]=FLEXIO_SHIFTCFG_SSTART(0)  /* Start disable load data on enable */
						 |FLEXIO_SHIFTCFG_SSTOP(2)   /* Stop bit low */
						 |FLEXIO_SHIFTCFG_INSRC(0);  /* Input source is the pin */
	/*SCL timer config */
	  FLEXIO->TIMCFG[0]=FLEXIO_TIMCFG_TSTART(1) 	/* Start bit enabled*/
					   |FLEXIO_TIMCFG_TSTOP(2) 		/* Stop bit enabled on timer disabled*/
					   |FLEXIO_TIMCFG_TIMENA(2) 	/* Timer enable on trigger high*/
					   |FLEXIO_TIMCFG_TIMDIS(2) 	/* Timer disable on timer compare*/
					   |FLEXIO_TIMCFG_TIMRST(2)		/* Timer reset on timer pin = timer output */
					   |FLEXIO_TIMCFG_TIMDEC(0) 	/* Timer decrement on FlexIO clk */
					   |FLEXIO_TIMCFG_TIMOUT(1); 	/* Timer output is zero and is not affected by reset */
	  FLEXIO->TIMCTL[0]=FLEXIO_TIMCTL_TIMOD(1)  	/* Dual 8 bit cntr baud/bit mode */
			  	  	   |FLEXIO_TIMCTL_PINPOL(0) 	/* Pin is active high */
					   |FLEXIO_TIMCTL_PINSEL(2) 	/* FXDIO2 for timer 0 */
					   |FLEXIO_TIMCTL_PINCFG(1) 	/* Pin cfg: open drain */
					   |FLEXIO_TIMCTL_TRGSRC(1) 	/* Internal trigger */
					   |FLEXIO_TIMCTL_TRGPOL(1) 	/* Trg pol: low */
					   |FLEXIO_TIMCTL_TRGSEL(1); 	/* Trg sel: Shifter 0*/

	/*SDA timer config */
	  FLEXIO->TIMCFG[1]=FLEXIO_TIMCFG_TSTART(1) /* Start bit enabled*/
					   |FLEXIO_TIMCFG_TSTOP(1) 	/* Stop bit enabled on timer compare*/
					   |FLEXIO_TIMCFG_TIMENA(1) /* Timer enable on timer 0*/
					   |FLEXIO_TIMCFG_TIMDIS(1) /* Timer disable on timer 0*/
					   |FLEXIO_TIMCFG_TIMRST(0)	/* Timer never reset */
					   |FLEXIO_TIMCFG_TIMDEC(2) /* Timer decrement on pin input*/
					   |FLEXIO_TIMCFG_TIMOUT(0); /* Timer output is one and is not affected by reset */

	  FLEXIO->TIMCTL[1]=FLEXIO_TIMCTL_TIMOD(3)  	/* Single 16 bit mode */
					   |FLEXIO_TIMCTL_PINPOL(1) 	/* Pin is active low */
					   |FLEXIO_TIMCTL_PINSEL(2) 	/* FXDIO2 for timer 	 */
					   |FLEXIO_TIMCTL_PINCFG(0) 	/* Pin cfg: outputdisabled */
					   |FLEXIO_TIMCTL_TRGSRC(1) 	/* Internal trigger */
					   |FLEXIO_TIMCTL_TRGPOL(1) 	/* Trg pol: low */
					   |FLEXIO_TIMCTL_TRGSEL(1); 	/* Trg sel: Shifter 0*/

	 FLEXIO->TIMCMP[1]=0xF;

	 FLEXIO->CTRL=FLEXIO_CTRL_FLEXEN_MASK; 		  /*Enable FlexIO */
}


void FlexIO_I2C_Transmit(uint8_t address, uint8_t *TXDataBuffer,uint8_t num_bytes)
{

	uint32_t  i=0;

    FLEXIO->TIMCMP[0]=((((num_bytes+1)*9+1)*2-1)<<8);
    FLEXIO->TIMCMP[0]|=0x25; /* Buadrate 100kbps. ((8MHz/100kbps)/2)-1=39 - 2(due to sync delays)=0x25*/

    while((FLEXIO->SHIFTSTAT & (1<<0))==0); 	/* Is the buffer empty? */
    FLEXIO->SHIFTBUFBBS[0]=(address<<1)|0;	    /* Transmit data, device address and write command*/
	while((FLEXIO->SHIFTSTAT & (1<<1))==0); 	/* Is the rx shifter filled */
	(void)FLEXIO->SHIFTBUFBBS[1]; 				/* Read RX shifter */

	for(i=0;i<num_bytes;i++)
	{

	    while((FLEXIO->SHIFTSTAT & (1<<0))==0); 	/* Is the buffer empty? */
	    FLEXIO->SHIFTBUFBBS[0]=*TXDataBuffer++;	  	/* Transmit data, register address to read*/
		while((FLEXIO->SHIFTSTAT & (1<<1))==0); 	/* Is the rx shifter filled */
		(void)FLEXIO->SHIFTBUFBBS[1]; 				/* Read RX shifter */
	}

	while((FLEXIO->SHIFTSTAT & (1<<0))==0); 		/* Is the buffer empty? */
	FLEXIO->SHIFTBUF[0]=0x00;	  					/* Fill Tx Shifter with 0xFF to prepare start condition*/

	FLEXIO->SHIFTCFG[0] &= ~FLEXIO_SHIFTCFG_SSTOP_MASK;
	FLEXIO->SHIFTCFG[0] |=FLEXIO_SHIFTCFG_SSTOP(3); 	/* Set stop bit high */
	FLEXIO->SHIFTCFG[1] = FLEXIO_SHIFTCFG_SSTOP(2); 	/* Set stop bit low*/
}

void FlexIO_I2C_Receive(uint8_t address,uint8_t *RXDataBuffer, uint8_t num_bytes, bool Relbus)
{
	uint8_t rxdata;
	uint8_t i=0;
	bool Flag = false;

    FLEXIO->TIMCMP[0]=((((num_bytes+1)*9+1)*2-1)<<8);
    FLEXIO->TIMCMP[0]|=0x25; /* Buadrate 100kbps. ((8MHz/100kbps)/2)-1=39 - 2(due to sync delays)=0x25*/

	while((FLEXIO->SHIFTSTAT & (1<<0))==0); 	/* Is the buffer empty? */
	FLEXIO->SHIFTBUFBBS[0]=(address<<1)|1;	  	/* Transmit data, device address and read command*/
	while((FLEXIO->SHIFTSTAT & (1<<0))==0); 	/* Is the buffer empty? */
    FLEXIO->SHIFTBUFBBS[0]=0xFF;				/* 0xFF in the transmit shifter to tristate the output */
    while((FLEXIO->SHIFTSTAT & (1<<1))==0); 	/* Is the rx shifter filled */
	(void)FLEXIO->SHIFTBUFBBS[1]; 				/* Drain RX shifter */

	for(i=0;i<num_bytes;i++)
	{
		while((FLEXIO->SHIFTSTAT & (1<<0))==0); 	/* Is the buffer empty? */

		if(i == num_bytes - 1)
		{
			// Último byte: preparar para NACK generando STOP (SDA en alto en 9º bit)
			FLEXIO->SHIFTCFG[0] &= ~FLEXIO_SHIFTCFG_SSTOP_MASK;
			FLEXIO->SHIFTCFG[0] |= FLEXIO_SHIFTCFG_SSTOP(3);  // SSTOP = NACK (SDA alto)
			FLEXIO->SHIFTCFG[1] = FLEXIO_SHIFTCFG_SSTOP(3);  // SSTOP = NACK (SDA alto)
		}
		else
		{
			if(Flag == false)
			{
			 // Bytes intermedios: ACK (SDA bajo en 9º bit)
				FLEXIO->SHIFTCFG[0] &= ~FLEXIO_SHIFTCFG_SSTOP_MASK;
				FLEXIO->SHIFTCFG[0] |= FLEXIO_SHIFTCFG_SSTOP(2);  // SSTOP = ACK (SDA bajo)
				Flag = true;
			}
		}

	  	while((FLEXIO->SHIFTSTAT & (1<<1))==0); 	/*Is the rx shifter filled */
	  	rxdata=FLEXIO->SHIFTBUFBIS[1]; 				/* Read data */
	    *RXDataBuffer++ = rxdata;
	}

		FLEXIO->SHIFTCFG[0] &= ~FLEXIO_SHIFTCFG_SSTOP_MASK;
		FLEXIO->SHIFTCFG[0]|=FLEXIO_SHIFTCFG_SSTOP(3); 	/* Set stop bit high */
		FLEXIO->SHIFTCFG[1]=FLEXIO_SHIFTCFG_SSTOP(2); 	/* Set stop bit low*/

		if(Relbus)
		{
			while((FLEXIO->SHIFTSTAT & (1<<0))==0);
			FLEXIO->SHIFTBUFBBS[0]=0x00;
		}
}

void FlexIO_I2C_startCondition(void)
{
	 while((FLEXIO->SHIFTSTAT & (1<<0))==0); 		/* Is the buffer empty? */
	 FLEXIO->SHIFTBUF[0]=0xFF;	  					/* Fill Tx Shifter with 0xFF to prepare start condition*/
}

void FlexIO_I2C_stopCondition(void)
{
	 while((FLEXIO->SHIFTSTAT & (1<<0))==0); 		/* Is the buffer empty? */
	 FLEXIO->SHIFTBUF[0]=0x00;	  					/* Fill Tx Shifter with 0xFF to prepare start condition*/
}
