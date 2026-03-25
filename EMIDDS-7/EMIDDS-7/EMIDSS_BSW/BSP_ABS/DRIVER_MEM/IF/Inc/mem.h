/*
 * mem.h
 *
 *  Created on: Mar 5, 2024
 *      Author: marco
 */

#ifndef MEM_H_
#define MEM_H_

#include "SPIF_Driver.h"

typedef struct
{
	float temp;
	float press;
	float hum;
	float batt;
}ReadingData;

typedef union
{
	ReadingData phys;
	uint8_t raw[16];
}MEM_SensorData;

typedef struct
{
    char hour;
    char min;
    MEM_SensorData sensorData;
}MEM_DataFrame;



#define MEM_FRAME_SIZE		18
#define MEM_MAX_ADDRESS		SPIF_MAX_ADDRESS

extern uint32_t MEM_InitMemory(void);
extern uint8_t MEM_Erase_Memory(void);
extern uint32_t MEM_Save_Frame(MEM_DataFrame *frame);
extern uint8_t MEM_getSavedFrames(uint32_t start_frame_idx, uint32_t frames_to_read, uint8_t *rData);
extern uint32_t MEM_getNumSavedFrames(void);
extern uint32_t MEM_getMemAddress(void);
extern void MEM_Convert_FrameToBytes(MEM_DataFrame *data, uint8_t *frameUpdate);
extern void MEM_Convert_BytesToFrame(MEM_DataFrame *data, uint8_t *frameUpdate);
extern void MEM_ConfigureNewMem(void);

#endif /* MEM_H_ */
