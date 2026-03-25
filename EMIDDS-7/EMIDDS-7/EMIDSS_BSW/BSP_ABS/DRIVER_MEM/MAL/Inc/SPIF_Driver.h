/*
 * SPIF_Driver.h
 *
 *  Created on: Feb 25, 2024
 *      Author: marco
 */

#ifndef SPIF_DRIVER_H_
#define SPIF_DRIVER_H_


#include "SPI_MCAL.h"
#include "S32K144W.h"


/**************************************************************************
 *
 * API declaration
 *
 **************************************************************************/

void SPIF_Reset(uint32_t instance);
uint32_t SPIF_ReadJedecId(uint32_t instance);
void SPIF_Read(uint32_t instance, uint32_t startPage, uint8_t offset, uint32_t size, uint8_t *rData);
void SPIF_Erase_Chip(uint32_t instance);
void SPIF_Erase_Sector(uint32_t instance, uint16_t numSector);
void SPIF_Write_Clean(uint32_t instance, uint32_t page, uint16_t offset, uint32_t size, uint8_t *data);
void SPIF_Write_Update(uint32_t instance, uint32_t page, uint16_t offset, uint32_t size, uint8_t *data);
uint32_t bytesToModify(uint32_t size, uint32_t offset);
uint32_t bytesToWrite(uint32_t size, uint32_t offset);
void SPIF_Write_Disable(uint32_t instance);
void SPIF_Write_Enable(uint32_t instance);

float SPIF_Read_Num(uint32_t page, uint16_t offset);
void SPIF_Write_Num(uint32_t page, uint16_t offset, float data);
void SPIF_Global_Block_Protection_Unlock(uint32_t instance);


/**************************************************************************
 *
 * Mem constants
 *
 **************************************************************************/
#define SPIF_MAX_NUM_OF_PAGES		16384
#define SPIF_MAX_ADDRESS			4194304

/**************************************************************************
 *
 * External flash commands
 *
 **************************************************************************/
/* Timeout */
#define SPIF_TIME_OUT	1000

/*Configuration*/
#define SPIF_NOP         (uint8_t)0x00    /**No Operation*/
#define SPIF_RSTEN       (uint8_t)0x66    /*Reset Enable*/
#define SPIF_RST         (uint8_t)0x99    /*Reset Memory*/
#define SPIF_EQIO        (uint8_t)0x38    /*Enable Quad I/O*/
#define SPIF_RSTQIO      (uint8_t)0xFF    /*Reset Quad I/O*/
#define SPIF_RDSR        (uint8_t)0x05    /*Read STATUS Register*/
#define SPIF_WRSR        (uint8_t)0x01    /*Write STATUS Register*/
#define SPIF_RDCR        (uint8_t)0x35    /*Read Configuration Register*/
/*Read*/
#define SPIF_READ        (uint8_t)0x03    /*Read Memory*/
#define SPIF_HSREAD      (uint8_t)0x0B    /*Read Memory at Higher Speed*/
#define SPIF_SQOR        (uint8_t)0x6B    /*SPI Quad Output Read*/
#define SPIF_SQIOR       (uint8_t)0xEB    /*SPI Quad I/O Read*/
#define SPIF_SDOR        (uint8_t)0x3B    /*SPI Dual Output REad*/
#define SPIF_SDIOR       (uint8_t)0xBB    /*SPI Dual I/O Read*/
#define SPIF_SB          (uint8_t)0xC0    /*Set Burst Lengt*/
#define SPIF_RBSQI       (uint8_t)0x0C    /*SQI Read Burst with Wrap*/
#define SPIF_RBSPI       (uint8_t)0xEC    /*SPI Read Burst with Wrap*/
/*Identification*/
#define SPIF_JEDEC_ID    (uint8_t)0x9F    /*JEDEC-ID Read*/
#define SPIF_QJ_ID       (uint8_t)0xAF    /*Quad I/O J-ID Read*/
#define SPIF_SFDP        (uint8_t)0x5A    /*Serial Flash Discoverable Parameters*/
/*Write*/
#define SPIF_WREN        (uint8_t)0x06    /*Write Enable*/
#define SPIF_WRDI        (uint8_t)0x04    /*Write Disable*/
#define SPIF_SE          (uint8_t)0x20    /*Erase 4 Kbytes of Memory Array*/
#define SPIF_BE          (uint8_t)0xD8    /*Erase 64,32, or 8 Kbyte of Memory Array*/
#define SPIF_CE          (uint8_t)0xC7    /*Erase Full Array*/
#define SPIF_PP          (uint8_t)0x02    /*Page Program*/
#define SPIF_SQI_PP      (uint8_t)0x32    /*SQI Quad Page Program*/
#define SPIF_WRSU        (uint8_t)0xB0    /*Suspends Program/Erase*/
#define SPIF_WRRE        (uint8_t)0x30    /*Resumes Program/Erase*/
/*Protection*/
#define SPIF_RBPR        (uint8_t)0x72    /*Read Block Protection Register*/
#define SPIF_WBPR        (uint8_t)0x42    /*Write Block Protection Register*/
#define SPIF_LBPR        (uint8_t)0x8D    /*Lock Down Block Protection Register*/
#define SPIF_NVWLDR      (uint8_t)0xE8    /*nonvolatile Write Lock Down Register*/
#define SPIF_ULBPR       (uint8_t)0x98    /*Global Block Protection Unlock*/
#define SPIF_RSID        (uint8_t)0x88    /*Read Security ID*/
#define SPIF_PSID        (uint8_t)0xA5    /*Program User Security ID area*/
#define SPIF_LSID        (uint8_t)0x85    /*Lockout Security ID ProgrammiSPIF_ng*/

/*Configuration Register*/
#define SPIF_CR_IOC         (1U<<1U)
#define SPIF_CR_BPNV        (1U<<3U)
#define SPIF_CR_WPEN        (1U<<7U)

/*Status Register*/
#define SPIF_SR_BUSY0       (1U<<0U)
#define SPIF_SR_WEL         (1U<<1U)
#define SPIF_SR_WSE         (1U<<2U)
#define SPIF_SR_WSP         (1U<<3U)
#define SPIF_SR_WPLD        (1U<<4U)
#define SPIF_SR_SEC         (1U<<5U)
#define SPIF_SR_BUSY1       (1U<<7U)



#endif /* SPIF_DRIVER_H_ */
