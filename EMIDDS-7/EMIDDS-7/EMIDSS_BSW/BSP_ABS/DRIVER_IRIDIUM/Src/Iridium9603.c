/*
* Iridium9603.c
*
*  Created on: 29 abr. 2025
*      Author: Sergio Ramirez
*/

/*******************************************************************************
* Includes
******************************************************************************/
#include "Iridium9603.h"
#include "Iridium9603_def.h"

#include "Platform_Types.h"
#include "Delay.h"
#include "string.h"
#include "FlexIO_I2C.h"
#include "stdlib.h"
/*******************************************************************************
* Defines
******************************************************************************/
#define IRIDIUM_INIT_TIMEOUT		(30000)
#define IRIDIUM_WAIT_RESPONSE_TIME	(240)
#define IRIDIUM_SENDRECEIVE_TIME    (300) //TEST Valor modificado para evitar el consumo de tiempo de espera (anterior:30)

#define IRIDIUM_MAX_MESSAGE_LENGTH	(340)
#define IRIDIUM_PACKET_SIZE			(8)
#define IRIDIUM_PACKET_TX_SIZE		(8)

#define IRIDIUM_RING_ALERT_FLAG		(0) 	/*set 1 to enable ring alert*/
/*******************************************************************************
* Global variables
******************************************************************************/
static uint8_t IO_REGISTER = 0;
static uint8_t MsgRecv[100] = {0};
/*******************************************************************************
* Private Interfaces
******************************************************************************/
static void Iridium9603_check_pins(void)
{
    uint8_t reg = IO_REG;
    FlexIO_I2C_Transmit(IRIDIUM_I2C_ADDRESS, &reg, 1);
    Delay_vMs(10);
    FlexIO_I2C_Receive(IRIDIUM_I2C_ADDRESS, &IO_REGISTER, 1, true);
}

static void Iridium9603_set_io_register(uint8_t value)
{
	uint8_t TXDataBuffer[2] = {IO_REG, 0};

	TXDataBuffer[1] = value;
	FlexIO_I2C_Transmit(IRIDIUM_I2C_ADDRESS, TXDataBuffer, sizeof(TXDataBuffer));
}

static uint16_t Iridium9603_MsgAvailable(void)
{
    uint8_t reg = LEN_REG;
    uint8_t len_bytes[2] = {0};
    uint8_t Len = 0;

    FlexIO_I2C_Transmit(IRIDIUM_I2C_ADDRESS, &reg, 1);
    Delay_vMs(10);
    FlexIO_I2C_Receive(IRIDIUM_I2C_ADDRESS, len_bytes, 2, true);

    if(len_bytes[0] != 0xff && len_bytes[1] != 0xff)
    {
    	Len = ((uint16_t)len_bytes[0] << 8) | len_bytes[1];
    }

    return Len;
}

static void Iridium9603_send_command(uint8_t* Cmd, uint16_t lenght)
{
	uint8_t TXDataBuffer[64];

	TXDataBuffer[0] = DATA_REG;
	memcpy(&TXDataBuffer[1], Cmd, lenght);

	FlexIO_I2C_Transmit(IRIDIUM_I2C_ADDRESS, TXDataBuffer, lenght + 1);
}

static void Iridium9603_check_data(void)
{
	uint16_t Len = 0;
	uint16_t offset = 0;
	uint8_t reg = DATA_REG;

	memset((void*)MsgRecv, 0, sizeof(MsgRecv));

	Len = Iridium9603_MsgAvailable();

	if(Len > 0)
	{
		FlexIO_I2C_Transmit(IRIDIUM_I2C_ADDRESS, &reg, 1);
		Delay_vMs(100);

		while (Len > IRIDIUM_PACKET_SIZE)
		{
			FlexIO_I2C_Receive(IRIDIUM_I2C_ADDRESS, &MsgRecv[offset], IRIDIUM_PACKET_SIZE, false);
			offset += IRIDIUM_PACKET_SIZE;
			Len -= IRIDIUM_PACKET_SIZE;
		}

		FlexIO_I2C_Receive(IRIDIUM_I2C_ADDRESS, &MsgRecv[offset], Len, true);
	}
}

static bool Iridium9603_wait_for_response(const char *expected, uint32_t timeout_ms)
{
    bool ResponseFlag = false;
    uint32_t start = Delay_u32GetTicks();

    while ((Delay_u32GetTicks() - start) < timeout_ms && !ResponseFlag)
    {
    	Iridium9603_check_data();

		if(strstr((char*)MsgRecv, expected))
		{
			ResponseFlag = true;
		}

        Delay_vMs(1000);
    }

    return ResponseFlag;
}

static void Iridium9603_EnableCapCharger(bool enable)
{
	Iridium9603_check_pins();

	if(enable)
	{
		IO_REGISTER |= IO_SHDN;
	}
	else
	{
		IO_REGISTER &= ~IO_SHDN;
	}

	Iridium9603_set_io_register(IO_REGISTER);
}

static void Iridium9603_EnablePower(bool enable)
{
	Iridium9603_check_pins();

	if(enable)
	{
		IO_REGISTER |= IO_PWR_EN;
	}
	else
	{
		IO_REGISTER &= ~IO_PWR_EN;
	}

	Iridium9603_set_io_register(IO_REGISTER);
}

static void Iridium9603_Enable9603(bool enable)
{
	Iridium9603_check_pins();

	if(enable)
	{
		IO_REGISTER |= IO_ON_OFF;
	}
	else
	{
		IO_REGISTER &= ~IO_ON_OFF;
	}

	Iridium9603_set_io_register(IO_REGISTER);
}

static bool Iridium9603_EnableRingAlert(bool enable)
{
	bool status = false;
	// Clear the 9603 RI flag
	Iridium9603_check_pins();
	IO_REGISTER &= ~IO_RI;
	Iridium9603_set_io_register(IO_REGISTER);

	if(enable)
	{
		uint8_t CmdRingAlertOn[] = {"AT+SBDMTA=1\r"};

		Iridium9603_send_command(CmdRingAlertOn, sizeof(CmdRingAlertOn) - 1);
		Delay_vMs(500);
		status = Iridium9603_wait_for_response("OK", 5000);
	}
	else
	{
		uint8_t CmdRingAlertOff[] = {"AT+SBDMTA=0\r"};

		Iridium9603_send_command(CmdRingAlertOff, 12);
		Delay_vMs(500);
		status = Iridium9603_wait_for_response("OK", 5000);
	}

	return status;
}

static void Iridium9603_SBDIX(uint16_t* moCode, uint16_t* moMSN, uint16_t* mtCode, uint16_t* mtMSN, uint16_t* mtLen, uint16_t* mtRemaining)
{
	uint16_t *values[6] = {moCode, moMSN, mtCode, mtMSN, mtLen, mtRemaining};
	uint8_t CmdSBDIX[] = "AT+SBDIX\r";
	char* prefix = "+SBDIX: ";
	char* ptr;

	Iridium9603_check_data(); /*Clear residual*/

	Iridium9603_send_command(CmdSBDIX, sizeof(CmdSBDIX) - 1);
	Delay_vMs(20000); //TEST: Valor modificado para aumentar el tiempo de espera de enlace (anterior:10000)
	Iridium9603_check_data();

	ptr = strstr((char*)MsgRecv, prefix);
	if(ptr != NULL)
	{
		ptr += strlen(prefix);

		for (uint8_t i = 0; i < 6; i++)
		{
			*values[i] = (uint16_t)atol(ptr); // Convierte a entero y guarda en el puntero

			ptr = strchr(ptr, ','); // Busca la siguiente coma
			if (ptr == NULL && i < 5) break; // Si ya no hay más pero aún faltan datos -> salir
			ptr++; // Avanza más allá de la coma
		}
	}
}

/*******************************************************************************
* Public Interfaces
******************************************************************************/
uint8_t Iridium9603_EnableModem(void)
{
	uint8_t Init_status = IRIDIUM_FAIL_CONN;
	uint32_t time_start = 0;
	uint8_t Cmd[3] = {"AT\r"};
	bool modemAlive = false;
	bool StatusInit = true;

	Iridium9603_EnableCapCharger(true);

    time_start = Delay_u32GetTicks();
    while (Delay_u32GetTicks() - time_start < IRIDIUM_INIT_TIMEOUT)
    {
        Iridium9603_check_pins();
        if(IO_REGISTER & IO_PGOOD)  /*Wait for supercapacitors to charge*/
        {
        	break;
        }

        Delay_vMs(1000);
    }

    if (IO_REGISTER & IO_PGOOD)
    {
    	Iridium9603_EnablePower(true);

    	Iridium9603_check_pins();  /*Update IO_REGISTER with the status of the 9603 pins*/
    	Iridium9603_check_data();  /*Get any waiting 9603 serial data*/

    	Iridium9603_Enable9603(true);
		Delay_vMs(500);

		time_start = Delay_u32GetTicks();
	    while ((Delay_u32GetTicks() - time_start) < IRIDIUM_WAIT_RESPONSE_TIME * 1000 && !modemAlive)
	    {
	    	Iridium9603_send_command(Cmd, sizeof(Cmd)); /* Turn on modem and wait for a response from "AT" command to begin */
	    	Delay_vMs(200);
	    	modemAlive = Iridium9603_wait_for_response("OK", 5000);
	    }

	    if(modemAlive)
	    {
	    	/* Initialization sequence */
	    	char *strings[3] = {"ATE1\r", "AT&D0\r", "AT&K0\r"};
	    	uint8_t strSize[3] = {5, 6, 6};

			for(uint8_t u8i = 0; u8i < 3; u8i++)
			{
				Iridium9603_send_command((uint8_t*)strings[u8i], strSize[u8i]);
				Delay_vMs(200);
				if(!Iridium9603_wait_for_response("OK", 5000))
				{
					StatusInit = false;
				}
			}

			/* Enable or disable RING alerts as requested in IRIDIUM_RING_ALERT_FLAG*/
			if(Iridium9603_EnableRingAlert(IRIDIUM_RING_ALERT_FLAG) && StatusInit)
			{
				Init_status = IRIDIUM_SUCCESS;
			}
	    }
	    else
	    {
	    	Init_status = IRIDIUM_NO_MODEM_DETECTED;
	    }
    }
    else
    {
    	Init_status = IRIDIUM_FAIL_TIMEOUT;
    }

    return Init_status;
}

uint8_t Iridium9603_GetSignalQuality(void)
{
	char *prefix = "+CSQ:";

	uint8_t CmdQuality[] = "AT+CSQ\r";
	uint8_t Quality = 0;

	//Iridium9603_check_data(); /*Get residual data*/

	Iridium9603_send_command(CmdQuality, sizeof(CmdQuality) - 1);
	Delay_vMs(5000);
	Iridium9603_check_data();

	char *ptr = strstr((char*)MsgRecv, prefix);
	if (ptr != NULL)
	{
		ptr += strlen(prefix);
		if (*ptr >= '0' && *ptr <= '9')
		{
			Quality = atoi(ptr);
		}
	}

	return Quality;
}

bool Iridium9603_SendSBDText(uint8_t* buffer, uint16_t Len)
{
	bool Status = false;
	bool Retry = true;
	uint16_t offset = 0;
	uint32_t time_start = 0;
	uint8_t CmdSBD[] = "AT+SBDWT\r";

	uint16_t moCode = 0, moMSN = 0, mtCode = 0, mtMSN = 0, mtLen = 0, mtRemaining = 0;

	char *p = strchr((char *)buffer, '\r');
	if (p) *p = 0;

	if (Len < IRIDIUM_MAX_MESSAGE_LENGTH)
	{
		Iridium9603_check_data(); /*Get residual msg*/

		Iridium9603_send_command(CmdSBD, sizeof(CmdSBD) - 1);
		Delay_vMs(500);
		Iridium9603_check_data();

		if(strstr((char*)MsgRecv, "READY"))
		{
			Iridium9603_check_data(); /*Get residual msg*/

			while (Len > IRIDIUM_PACKET_TX_SIZE)
			{
				Iridium9603_send_command(&buffer[offset], IRIDIUM_PACKET_TX_SIZE);
				offset += IRIDIUM_PACKET_TX_SIZE;
				Len -= IRIDIUM_PACKET_TX_SIZE;
			}

			Iridium9603_send_command(&buffer[offset], Len);
			Iridium9603_send_command((uint8_t*)"\r", 1);
			Delay_vMs(500);

			Status = Iridium9603_wait_for_response("OK", 5000);
		}
	}

	if(Status)
	{
		time_start = Delay_u32GetTicks();
		while ((Delay_u32GetTicks() - time_start) < IRIDIUM_SENDRECEIVE_TIME * 1000 && Retry)
		{
			Iridium9603_SBDIX(&moCode, &moMSN, &mtCode, &mtMSN, &mtLen, &mtRemaining);

			if(moCode <= 4)
			{
				Retry = false;
				Status = true;
			}
			else if (moCode == 12 || moCode == 14 || moCode == 16) // fatal failure: no retry
			{
				Retry = false;
				Status = false;
			}
			else /*retry*/
			{
				Delay_vMs(5000);
				Status = false;
			}
		}
	}

	return Status;
}

bool Iridium9603_ClearMOBuffer(void)
{
	bool status = false;
	uint8_t CmdClean[9] = "AT+SBDD0\r";

	Iridium9603_check_data(); /*Get residual data*/

	Iridium9603_send_command(CmdClean, 9);
	Delay_vMs(200);
	status = Iridium9603_wait_for_response("OK", 5000);

	return status;
}

void Iridium9603_DisableModem(void)
{
	Iridium9603_EnablePower(false);
	Iridium9603_EnableCapCharger(false);
}
