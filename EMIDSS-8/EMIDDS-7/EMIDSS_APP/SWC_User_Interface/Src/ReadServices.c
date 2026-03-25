/*
 * ReadServices.c
 *
 *  Created on: 15 mar. 2024
 *      Author: Maximiliano Mancilla
 */
#include "ReadServices.h"
#include "UART_MCAL_cfg.h"
#include "UART_MCAL_def.h"
#include "UART_MCAL.h"

#include "mem.h"
#include "stdio.h"
#include "I2C.h"
#include "time.h"

extern UART_tenUartsPorts UARTPort;

/* ========================================Response Messages =======================================*/
uint8_t ResetMsg_Mem_Start[] 	  = "The memory reset has started. It will take some minutes";
uint8_t ResetMsg_Mem_Finish[] 	  = "The memory bas been reseted successfully.";
uint8_t ResetMsg_Driver[] 		  = "The driver bas been reseted successfully.";
uint8_t DateUpdate[] 	  		  = "The date has been updated";
uint8_t InvalidCommand[]  		  = "Invalid Command";
uint8_t	OneMinUart[]	  		  = "1m function hit";
uint8_t SW_Version[] 	  		  = "EMIDSS-V.2";
uint8_t EmptyMem_Msg[]	  		  = "Error: Empty memory";
/* =================================================================================================*/

uint8_t MemorySimulation[300] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
	40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
	50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
	60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
	70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
	80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
	90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
	100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
	110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
	120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
	130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
	140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
	150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
	160, 161, 162, 163, 164, 165, 166, 167, 168, 169,
	170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
	180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
	190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
	200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
	210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
	220, 221, 222, 223, 224, 225, 226, 227, 228, 229,
	230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
	240, 241, 242, 243, 244, 245, 246, 247, 248, 249,
	250
};

/*Reset Services*/
void ResetService(uint8_t DID)
{
	switch(DID)
	{
		case 01: /*Reset MS8607 Sensor*/
			ResetService_01();
			break;

		case 02: /*Reset Flash Memory*/
			ResetService_02();
			break;

		default:/*Invalid Command*/
			UART_vSendBuffer(UARTPort, InvalidCommand, strlen(InvalidCommand));
			break;
	}
}

/*Read Services*/
void ReadServiceSelect(uint8_t DID)
{
    switch(DID)
    {
        case 01:
            ReadService_01(); /*Read Stored Hour*/
            break;

        case 02:
            ReadService_02();/*Read all memory*/
            break;

		case 03:
			ReadService_03();/*Read SW Version*/
			break;

        default:/*Invalid Command*/
			UART_vSendBuffer(UARTPort, InvalidCommand, strlen(InvalidCommand));
            break;
    }
}

/*Write Services*/
void WriteServiceSelect(uint8_t DID, uint8_t Data_1, uint8_t Data_2)
{
	switch(DID)
	{
		case 01:
			WriteService_01(Data_1, Data_2);/*Update time*/
			break;

		default:/*Invalid Command*/
			UART_vSendBuffer(UARTPort, InvalidCommand, strlen(InvalidCommand));
			break;
	}
}

/*================================ READ SERVICES FUNCTIONS ===============================*/
void ReadService_01(void)/*Read stored time*/
{
	MEM_DataFrame dataFrame;
	char Actual_Time[50];

	dataFrame.hour = Time_get_hour();
	dataFrame.min = Time_get_minutes();

	sprintf(Actual_Time,"Time: [%d:%d].", dataFrame.hour, dataFrame.min);

	UART_vSendBuffer(UARTPort, Actual_Time, strlen(Actual_Time));
	memset(Actual_Time, 0, strlen(Actual_Time));
}

void ReadService_02(void)/*Read all memory*/
{
	uint32_t SavedFrames = MEM_getNumSavedFrames();
	uint8_t EmptyByte = 0;
	MEM_DataFrame dataFrame;
	size_t longitud;

	char TextUart[200];

	for(uint32_t i = 0; i < SavedFrames; i++)
	{	
		uint8_t frame[MEM_FRAME_SIZE];

		MEM_getSavedFrames(i, 1, frame);

		MEM_Convert_BytesToFrame(&dataFrame, frame);

		sprintf(TextUart, "Hour: %d, Min: %d, Temp: %f, Hum: %f, Press: %f\n", dataFrame.hour, dataFrame.min, dataFrame.sensorData.phys.temp, dataFrame.sensorData.phys.hum, dataFrame.sensorData.phys.press);
		
		longitud = strlen(TextUart);
		UART_vSendBuffer(UARTPort, TextUart, longitud);
		memset(TextUart, 0, strlen(TextUart));
	}

	if(SavedFrames == 0)
	{
		UART_vSendBuffer(UARTPort, EmptyMem_Msg, strlen(EmptyMem_Msg));
	}
}

void ReadService_03(void)/*Read SW Version*/
{
	UART_vSendBuffer(UARTPort, SW_Version, strlen(SW_Version));
}

/*================================ WRITE SERVICES FUNCTIONS ===============================*/
void WriteService_01(uint8_t new_hour, uint8_t new_min)
{
	MEM_DataFrame dataFrame;
	char TimeUpdate[50];

	Time_set_hour(new_hour);
	Time_set_minutes(new_min);

	dataFrame.hour = Time_get_hour();
	dataFrame.min = Time_get_minutes();

	sprintf(TimeUpdate,"Time updated to [%d:%d].", dataFrame.hour, dataFrame.min);

	UART_vSendBuffer(UARTPort, TimeUpdate, strlen(TimeUpdate));
	memset(TimeUpdate, 0, strlen(TimeUpdate));
}

/*================================ RESET SERVICES FUNCTIONS ===============================*/
void ResetService_01(void) /*Reset Sensor*/
{
	I2C_u8WriteCmd(MS8607_ADDRESS, MS8607_RESET);
	UART_vSendBuffer(UARTPort, ResetMsg_Driver, strlen(ResetMsg_Driver));
}

void ResetService_02(void)/*Reset Memory*/
{
	UART_vSendBuffer(UARTPort, ResetMsg_Mem_Start, strlen(ResetMsg_Mem_Start));
	MEM_ConfigureNewMem();
	UART_vSendBuffer(UARTPort, ResetMsg_Mem_Finish, strlen(ResetMsg_Mem_Finish));
}
/*==================================== OTHER FUNCTIONS ====================================*/
void AliveUart(void)/*1 min indicator UART. Used only for testing purposes.*/
{
	UART_vSendBuffer(UARTPort, OneMinUart, 16u);
}