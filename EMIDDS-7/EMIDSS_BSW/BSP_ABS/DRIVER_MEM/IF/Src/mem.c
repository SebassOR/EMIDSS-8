/*
 * mem.c
 *
 *  Created on: Mar 5, 2024
 *      Author: marco
 */

#include "mem.h"



/*************************************************************************************
 *
 * Local variables
 *
 *************************************************************************************/

uint32_t memAddress;
uint32_t savedFrames;
uint8_t isMemRewritten = 0;

/*************************************************************************************
 *
 * Local functions
 *
 *************************************************************************************/

/*
 *
 *	Iterates through all memory until find a frame where the first byte is equal to 40
 *
 */
static uint32_t getWriteAddressFromMemory(void)
{
	uint8_t firstElement=0;
	uint32_t tempAddress=0;
	uint32_t page=0;
	uint32_t offset;

	savedFrames=0;

	while(firstElement != 40)
	{
		page=tempAddress/256;
		offset = tempAddress%256;

		// Read ID
		SPIF_Read(0,page,offset,1,&firstElement);

		// Count the number of saved frames
		savedFrames++;

		// Increment the memory
		tempAddress += MEM_FRAME_SIZE;

		// We reached the end of the memory and should rewrite from the beginning
		if(tempAddress >= SPIF_MAX_ADDRESS)
		{
			break;
		}
	}

	// Compute number of saved frames
	savedFrames--;

	// We found the end
	if(firstElement == 40)
	{
		// Subtract the step taken
		memAddress = tempAddress - MEM_FRAME_SIZE;

		// Check if the memory is overwritten
		if(tempAddress < SPIF_MAX_ADDRESS)
		{
			page=tempAddress/256;
			offset = tempAddress%256;
			SPIF_Read(0,page,offset,1,&firstElement);

			// If memory is not overwritten, the next element will be in default value (0xFF)
			if(firstElement < 24 )
			{
				// If overwritten, just subtract the endFrame (same size as regular frame)
				savedFrames = (MEM_MAX_ADDRESS/MEM_FRAME_SIZE) - 1;
				isMemRewritten = 1;
			}
		}
	}
	else if(tempAddress >= SPIF_MAX_ADDRESS)
	{
		// If we reached the end, we should start from the beginning
		memAddress = 0;
		isMemRewritten = 1;
		savedFrames = 0;
	}

	return 0;
}

/*************************************************************************************
 *
 * APIs
 *
 *************************************************************************************/


/*
 *	Initializes the memory
 */
uint32_t MEM_InitMemory(void)
{
	// Reset the memory for proper initialization
	SPIF_Reset(0);

	// Unlock the memory for writing
	SPIF_Global_Block_Protection_Unlock(0);

	// Test ID. It should return 0xBF 0x26 0x42 in SST26
	//  and 0xEF 0x40 0x18 in W25Q128
	volatile uint32_t jedec = SPIF_ReadJedecId(0);

	// Get writing address from last saved frame
	getWriteAddressFromMemory();

	return 0;
}

/*
 *  Configure the memory to use it in this program
 */
void MEM_ConfigureNewMem(void)
{
	// Erase all contents in the memory
	MEM_Erase_Memory();

	// Write the EndOfFrame at the start
	uint8_t endOfFrame = 40;
	SPIF_Write_Update(0, 0, 0, 1, &endOfFrame);

}

/*
 *	Convert MEM_DataFrame to byte stream
 */
void MEM_Convert_FrameToBytes(MEM_DataFrame *data, uint8_t *frameUpdate)
{
	frameUpdate[0] = data->hour;
	frameUpdate[1] = data->min;

	for(uint8_t i = 0; i< 16; i++)
	{
		frameUpdate[2+i] = data->sensorData.raw[i];
	}

}

/*
 *	Convert byte stream to MEM_DataFrame
 */
void MEM_Convert_BytesToFrame(MEM_DataFrame *data, uint8_t *frameUpdate)
{
	data->hour = frameUpdate[0];
	data->min = frameUpdate[1];

	for(uint8_t i = 0; i< 16; i++)
	{
		data->sensorData.raw[i] = frameUpdate[2+i];
	}
}

/*
 *	Returns the current writing address
 */
uint32_t MEM_getMemAddress(void)
{
	return memAddress;
}

/*
 * Return the number of saved readings
 */
uint32_t MEM_getNumSavedFrames(void)
{
	return savedFrames;
}


uint8_t MEM_getSavedFrames(uint32_t start_frame_idx, uint32_t frames_to_read, uint8_t *rData)
{
	uint8_t response=0;
	uint32_t startFrameAddr = start_frame_idx * MEM_FRAME_SIZE;

	// If the number do not exceed the saved frames
	if(start_frame_idx + frames_to_read <= savedFrames)
	{
		uint32_t page = startFrameAddr / 256;
		uint16_t offset = startFrameAddr % 256;
		SPIF_Read(0, page, offset, MEM_FRAME_SIZE*frames_to_read, rData);
	}
	else
	{
		// Requested more frames than the available
		response = 1;
	}

	return response;
}


/*
 * Save a sequence of bytes of size MEM_FRAME_SIZE into memory
 */
uint32_t MEM_Save_Frame(MEM_DataFrame *data)
{
	uint8_t frameUpdate[MEM_FRAME_SIZE*2] = {0xFF};
	uint32_t page = 0;
	uint16_t offset = 0;
	uint32_t frameLength = 0;

	MEM_Convert_FrameToBytes(data, frameUpdate);

	// Write end frame
	frameUpdate[MEM_FRAME_SIZE] = 40;

	// Check if there is enough space in memory to write both data frame and EndOfFrame
	if(memAddress + MEM_FRAME_SIZE*2 <= MEM_MAX_ADDRESS)
	{
		// If there is enough space, write the data and the end frame
		page = memAddress / 256;
		offset = memAddress % 256;

		// Write both data frame and EndOfFrame
		frameLength = MEM_FRAME_SIZE*2;

		// Increment memory address
		memAddress += MEM_FRAME_SIZE;
	}
	// Check if there is enough space to write at least the data frame
	else if(memAddress + MEM_FRAME_SIZE <= MEM_MAX_ADDRESS)
	{
		// If there is enough space, write the data and the end frame
		page = memAddress / 256;
		offset = memAddress % 256;

		// Write only data frame
		frameLength = MEM_FRAME_SIZE;

		// Increment memory address
		memAddress += MEM_FRAME_SIZE;

	}
	// If there is no more space, rewrite the memory from the beginning
	else
	{
		// Write the data from the beginning
		page = 0;
		offset = 0;
		// Write both data frame and EndOfFrame
		frameLength = MEM_FRAME_SIZE*2;
		// Increment memory address
		memAddress += MEM_FRAME_SIZE;
		// Reduce number of frames for updated frame and EndOfFrame
		savedFrames--;
		// Update mem rewritten flag
		isMemRewritten=1;
	}

	// Save both data and end frame
	SPIF_Write_Update(0, page, offset, frameLength, frameUpdate);

	// Only increase value if not overwritten
	if(isMemRewritten != 1)
	{
		savedFrames++;
	}

	return 0;
}


/*
 * Erase all data into the memory
 */
uint8_t MEM_Erase_Memory(void)
{
	// Update values
	memAddress = 0;
	savedFrames = 0;
	isMemRewritten = 0;

	// Erase chip
	SPIF_Erase_Chip(0);

	return 0;
}

