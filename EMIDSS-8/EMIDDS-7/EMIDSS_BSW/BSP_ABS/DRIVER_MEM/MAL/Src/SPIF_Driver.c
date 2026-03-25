/*
 * SPIF_Driver.c
 *
 *  Created on: Feb 25, 2024
 *      Author: marco
 */

#include "SPIF_Driver.h"
#include "SPI_MCAL.h"
#include "SPI_MCAL_def.h"


/*
 * Utilities and static variables
 */

/* Funcion Delay en milisegundos */
static void vDelayMs(uint32_t u32Time){
	for(uint32_t u8x = 0; u8x < u32Time; u8x++){

		for(uint32_t u16x = 0; u16x < 20000; u16x++){
			/*	Nothing to do	*/
			__asm("nop");
		}

	}
}

void float2bytes(uint8_t* float_bytes_temp, float float_variable)
{
	union{
		float a;
		uint8_t bytes[4];
	}thing;

	thing.a = float_variable;

	for(uint8_t i=0; i<4; i++)
	{
		float_bytes_temp[i] = thing.bytes[i];
	}
}


float bytes2float(uint8_t* float_bytes_temp)
{
	union{
		float a;
		uint8_t bytes[4];
	}thing;

	for(uint8_t i=0; i<4; i++)
	{
		thing.bytes[i] = float_bytes_temp[i];
	}

	float float_variable = thing.a;
	return float_variable;
}

/*
 * APIs for SPI Flash
 */



void SPIF_Write_Num(uint32_t page, uint16_t offset, float data)
{
	uint8_t temBytes[4];
	float2bytes(temBytes, data);

	SPIF_Write_Update(0, page, offset, 4, temBytes);
}


float SPIF_Read_Num(uint32_t page, uint16_t offset)
{
	uint8_t rData[4];
	SPIF_Read(0, page, offset, 4, rData);

	return (bytes2float(rData));
}


static void SPIF_Transmit(uint32_t instance, uint8_t *buffer, uint16_t size, uint32_t timeout)
{
	(void)timeout;	// Timeout is not currently supported
	SPI_u8Transmit((uint8_t)instance,buffer,size);
}


static void SPIF_Receive(uint32_t instance, uint8_t *buffer, uint16_t size, uint32_t timeout)
{
	(void)timeout;	// Timeout is not currently supported
	SPI_u8ReceiveData((uint8_t)instance, buffer, size);
}


static void SPIF_CS_LOW(uint32_t instance)
{
	SPI_CS(instance, 0);
}


static void SPIF_CS_HIGH(uint32_t instance)
{
	SPI_CS(instance, 1);
}


/*
 * APIs definitions
 */

void SPIF_Write_Enable(uint32_t instance)
{
    uint8_t tData = SPIF_WREN;

    SPIF_CS_LOW(instance);
    SPIF_Transmit(instance, &tData, 1, SPIF_TIME_OUT);
    SPIF_CS_HIGH(instance);

    vDelayMs(1);
}

void SPIF_Write_Disable(uint32_t instance)
{
    uint8_t tData = SPIF_WRDI;
    SPIF_CS_LOW(instance);
    SPIF_Transmit(instance, &tData, 1, SPIF_TIME_OUT);
    SPIF_CS_HIGH(instance);
    vDelayMs(1);
}

uint32_t bytesToWrite(uint32_t size, uint32_t offset)
{
    return (((size + offset) < 256) ? size : (256-offset));
}

uint32_t bytesToModify(uint32_t size, uint32_t offset)
{
    return (((size + offset) < 4096) ? size : (4096-offset));
}



void SPIF_Global_Block_Protection_Unlock(uint32_t instance)
{

	uint8_t tData = SPIF_ULBPR;

	SPIF_Write_Enable(instance);
	SPIF_CS_LOW(instance);
	SPIF_Transmit(instance, &tData, 1, SPIF_TIME_OUT);
	SPIF_CS_HIGH(instance);
	SPIF_Write_Disable(instance);
	vDelayMs(100);
}


void SPIF_Write_Update(uint32_t instance, uint32_t page, uint16_t offset, uint32_t size, uint8_t *data)
{
    uint16_t startSector = page/16;
    uint16_t endSector = (page + ((size+offset-1)/256))/16;
    uint16_t numSectors = endSector - startSector + 1;

    uint8_t previousData[4096];
    uint32_t sectorOffset = ((page%16)*256)+offset;
    uint32_t dataIndx=0;

    // First erase the sector
    for(uint16_t i=0; i<numSectors; i++)
    {
        uint32_t startPage = startSector*16;

        // Save curret data
        SPIF_Read(instance,startPage,0,4096,previousData);

        // Update current data
        uint16_t bytesRemaining = bytesToModify(size, sectorOffset);
        for(uint16_t i=0; i<bytesRemaining; i++)
        {
            previousData[i+sectorOffset] = data[i+dataIndx];
        }

        // Write updated data
        SPIF_Write_Clean(instance, startPage, 0, 4096, previousData);

        startSector++;
        sectorOffset = 0;
        dataIndx += bytesRemaining;
        size -= bytesRemaining;
    }

}
void SPIF_Write_Clean(uint32_t instance, uint32_t page, uint16_t offset, uint32_t size, uint8_t *data)
{
    uint8_t tData[266];
    uint32_t startPage = page;
    uint32_t endPage = startPage + ((size + offset - 1)/256);
    uint32_t numPages = endPage - startPage + 1;

    uint16_t startSector = startPage/16;
    uint16_t endSector = endPage/16;
    uint16_t numSectors = endSector - startSector + 1;

    uint32_t dataPosition = 0;

    // First erase the sector
    for(uint16_t i=0; i<numSectors; i++)
    {
        SPIF_Erase_Sector(instance, startSector++);
    }

    // Write the data
    for(uint32_t i=0; i<numPages; i++)
    {
        uint32_t memAddr = startPage*256 + offset;
        uint16_t bytesRemaining = bytesToWrite(size, offset);


        tData[0] = SPIF_PP;
        tData[1] = (memAddr >> 16) & 0xFF;
        tData[2] = (memAddr >> 8) & 0xFF;
        tData[3] = (memAddr) & 0xFF;

        uint16_t bytesToSend = bytesRemaining + 4;

        for(uint16_t i=0; i<bytesRemaining; i++)
        {
            tData[i+4] = data[i+dataPosition];
        }

        // Send the data
        SPIF_Write_Enable(instance);
        SPIF_CS_LOW(instance);
        SPIF_Transmit(instance, tData, bytesToSend, SPIF_TIME_OUT);
        SPIF_CS_HIGH(instance);
        SPIF_Write_Disable(instance);

        startPage++;
        offset=0;
        dataPosition += bytesRemaining;

        // TODO: Add delay
        vDelayMs(5);


    }

}


void SPIF_Erase_Chip(uint32_t instance)
{
	uint8_t tData = SPIF_CE;
	SPIF_Write_Enable(instance);
	SPIF_CS_LOW(instance);
	SPIF_Transmit(instance, &tData, sizeof(tData), SPIF_TIME_OUT);
	SPIF_CS_HIGH(instance);
	SPIF_Write_Disable(instance);

	// TODO: Add timeout
	vDelayMs(200000);
}


void SPIF_Erase_Sector(uint32_t instance, uint16_t numSector)
{
    uint8_t tData[4];
    uint32_t memAddr = numSector*16*256;  // Each sector contains 16 paiges * 256 bytes

    tData[0] = SPIF_SE;
    tData[1] = (memAddr >> 16) & 0xFF;
    tData[2] = (memAddr >> 8) & 0xFF;
    tData[3] = (memAddr) & 0xFF;


    SPIF_Write_Enable(instance);
    SPIF_CS_LOW(instance);
    SPIF_Transmit(instance, tData, sizeof(tData), SPIF_TIME_OUT);
    SPIF_CS_HIGH(instance);
    SPIF_Write_Disable(instance);

    // TODO: Add timeout
    vDelayMs(450);
}

void SPIF_Read(uint32_t instance, uint32_t startPage, uint8_t offset, uint32_t size, uint8_t *rData)
{
    uint8_t tData[4];
    uint32_t memAddr=0;

    // For some reason we can't read more than 554 bytes at once
    if(size > 500 )
    {
		uint32_t idx = 0;

		while((size-idx)>500)
		{
			memAddr = (startPage * 256) + offset + idx;
			tData[0] = SPIF_READ;
			tData[1] = (memAddr >> 16) & 0xFF;
			tData[2] = (memAddr >> 8) & 0xFF;
			tData[3] = (memAddr) & 0xFF;
			SPIF_CS_LOW(instance);
			SPIF_Transmit(instance, tData, sizeof(tData), SPIF_TIME_OUT);
			SPIF_Receive(instance, rData+idx, 500, SPIF_TIME_OUT);
			SPIF_CS_HIGH(instance);
			idx+=500;
		}

		memAddr = (startPage * 256) + offset + idx;
		tData[0] = SPIF_READ;
		tData[1] = (memAddr >> 16) & 0xFF;
		tData[2] = (memAddr >> 8) & 0xFF;
		tData[3] = (memAddr) & 0xFF;
		SPIF_CS_LOW(instance);
		SPIF_Transmit(instance, tData, sizeof(tData), SPIF_TIME_OUT);
		SPIF_Receive(instance, rData+idx, size-idx, SPIF_TIME_OUT);
		SPIF_CS_HIGH(instance);
    }
    else{
		// Calculate the address based on the 256 Bit page
		memAddr = (startPage * 256) + offset;

		tData[0] = SPIF_READ;
		tData[1] = (memAddr >> 16) & 0xFF;
		tData[2] = (memAddr >> 8) & 0xFF;
		tData[3] = (memAddr) & 0xFF;

		SPIF_CS_LOW(instance);
		SPIF_Transmit(instance, tData, sizeof(tData), SPIF_TIME_OUT);
		SPIF_Receive(instance, rData, size, SPIF_TIME_OUT);
		SPIF_CS_HIGH(instance);
    }
}


uint32_t SPIF_ReadJedecId(uint32_t instance)
{
    uint8_t command[] = {SPIF_JEDEC_ID};
    uint8_t DeviceID[3] = {0};

    SPIF_CS_LOW(instance);
    SPIF_Transmit(instance, command, sizeof(command), SPIF_TIME_OUT);
    SPIF_Receive(instance,DeviceID,3,SPIF_TIME_OUT);
    SPIF_CS_HIGH(instance);
    return ((DeviceID[0] << 16) | (DeviceID[1] << 8) | (DeviceID[2]));
}


void SPIF_Reset(uint32_t instance)
{
	uint8_t command[] = {SPIF_RSTEN};

	SPIF_CS_LOW(instance);
	SPIF_Transmit(instance, command, sizeof(command), SPIF_TIME_OUT);
	SPIF_CS_HIGH(instance);

	vDelayMs(500);

	command[0] = SPIF_RST;
	SPIF_CS_LOW(instance);
	SPIF_Transmit(instance, command, sizeof(command), SPIF_TIME_OUT);
	SPIF_CS_HIGH(instance);
	vDelayMs(1);
}
