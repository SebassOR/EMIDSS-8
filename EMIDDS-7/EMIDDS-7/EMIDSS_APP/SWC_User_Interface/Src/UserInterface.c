#include "UserInterface.h"
#include "GPIO_def.h"
#include "I2C.h"

/*Global Variables*/
UART_tenUartsPorts UARTPort = UART_enUART1;
uint8_t WellcomeMsg[] = "Welcome to EMIDSS-5";
uint8_t * ReceivedData;

void Uart_Start(void)
{
	uint8_t WellcomeBufferLenght = sizeof(WellcomeMsg) / sizeof(WellcomeMsg[0]);
	/* Uart Init Function */
	UART_vInit();

	/*Welcome message*/
	UART_vSendBuffer(UARTPort, WellcomeMsg, WellcomeBufferLenght);

	/*Enable UART Interrupts for RX*/
	UART_vInitIRQs();
}

void SendCommand(uint8_t Command[8])
{
	uint8_t Service = (Command[0] - '0') * 10 + (Command[1] - '0');
	uint8_t DID = 	  (Command[2] - '0') * 10 + (Command[3] - '0');
	uint8_t Data_1 =  (Command[4] - '0') * 10 + (Command[5] - '0');
	uint8_t Data_2 =  (Command[6] - '0') * 10 + (Command[7] - '0');

	switch(Service)
	{	
		case 11: /*Reset Service*/
			ResetService(DID);
			break;
		case 22: /* Read Service*/
			ReadServiceSelect(DID);
			break;
		case 23: /* Write Service*/
			WriteServiceSelect(DID, Data_1, Data_2);
			break;
		default:
			break;
	}
}

uint8_t StartByte_Flag = 0u;
void LPUART1_RxTx_IR(void)
{
	/*This funtion needs to be called twice to accept the command. This is an issue that need to be fixed.*/
	uint8_t tmpByte = 0u;
	static uint8_t BufferCommand[8];
	static uint8_t CommandIndex = 0;
	static uint8_t NumOfReceivedBytes = 3u;

	UART_u8ReceiveData(UARTPort, &tmpByte);

	if((1u == StartByte_Flag) && ( NumOfReceivedBytes >= CommandIndex))/*STORE THE COMMAND*/
	{
		BufferCommand[CommandIndex] = tmpByte;
		CommandIndex++;

		if(('2' == BufferCommand[0]) && ('3' == BufferCommand[1]))/*If service 23 is detected, adjust Received Bytes to get write bytes.*/
		{
			NumOfReceivedBytes = 7u;
		}
	}
	else if((1u == StartByte_Flag) || (CommandIndex >= (NumOfReceivedBytes + 1)))/*PROCESS THE COMMAND AND CLEAN IT*/
	{
		CommandIndex = 0;
		StartByte_Flag = 0;
		if(7u == NumOfReceivedBytes)/*Condition for service 23. Write. This is reached when 7 bytes are received and stored*/
		{
			SendCommand(BufferCommand);	/*Send 8 bytes*/
			BufferCommand[4] = 0;
			BufferCommand[5] = 0;
			BufferCommand[6] = 0;
			BufferCommand[7] = 0;
		}
		else /*Condition for other services. This is reached when 4 bytes are received and stored*/
		{
			BufferCommand[4] = 0;
			BufferCommand[5] = 0;
			BufferCommand[6] = 0;
			BufferCommand[7] = 0;
			SendCommand(BufferCommand);	/*Send 4 bytes, last 2 in 00*/
		}
		
		BufferCommand[0] = 0;
		BufferCommand[1] = 0;
		BufferCommand[2] = 0;
		BufferCommand[3] = 0;
		NumOfReceivedBytes = 3u;
	}
	else
	{
		if('S' == tmpByte)/*START OF BYTE FLAG SET*/
		{
			StartByte_Flag = 1u;
			CommandIndex = 0;
		}
	}
}
