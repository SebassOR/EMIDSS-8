/*
 * swc_os.c
 *
 *  Created on: 3 jun. 2024
 *      Author: Maximiliano Mancilla
 */
/* System Headers*/
#include "S32K144W.h"
#include "Platform_Types.h"
#include "Std_Types.h"

/*Own Headers*/
#include "GPIO.h"
#include "swc_os.h"
#include "ReadServices.h"
#include "swc_tem_pre_hum.h"
#include "mem.h"
#include "time.h"
#include "Iridium9603.h"
#include "Delay.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"

#define IRIDIUM_TIMEOUT			(60000U) /*1 minute*/
#define IRIDIUM_WAIT_CYCLE		(7) //Original at 10Min
#define IRIDIUM_SIGNAL_OK		(4U)

static uint32_t TxFlag = 0;

static void float_to_string(float value, char* buffer)
{
    if (value < 0)
    {
        *buffer++ = '-';
        value = -value;
    }

    int int_part = (int)value;
    int decimal_part = (int)((value - int_part) * 100 + 0.5f);

    char temp[12];
    int i = 0;

    do
    {
        temp[i++] = '0' + (int_part % 10);
        int_part /= 10;
    } while (int_part && i < sizeof(temp));

    while (i--)
    {
        *buffer++ = temp[i];
    }

    *buffer++ = '.';
    *buffer++ = '0' + (decimal_part / 10);
    *buffer++ = '0' + (decimal_part % 10);
    *buffer = '\0';
}

static uint16_t build_sensor_string_struct(SensorData_t datos, char* out_buffer)
{
    char temp_str[12];
    char hum_str[12];
    char pres_str[14];
    char *p = out_buffer;

    float_to_string(datos.temperature, temp_str);
    float_to_string(datos.humidity, hum_str);
    float_to_string(datos.pressure, pres_str);

    // Temperature: 23.45 °C\n
    const char *label_temp = "Temperature: ";
    while (*label_temp) *p++ = *label_temp++;
    for (char *s = temp_str; *s; ++s) *p++ = *s;
    *p++ = 'C'; *p++ = ' ';


    // Humidity:    56.78 %\n
    const char *label_hum = "Humidity: ";
    while (*label_hum) *p++ = *label_hum++;
    for (char *s = hum_str; *s; ++s) *p++ = *s;
    *p++ = '%'; *p++ = ' ';

    // Pressure:    1013.25 hPa\n
    const char *label_pres = "Pressure: ";
    while (*label_pres) *p++ = *label_pres++;
    for (char *s = pres_str; *s; ++s) *p++ = *s;
    *p++ = 'h'; *p++ = 'P'; *p++ = 'a';

    return (uint16_t)(p - out_buffer);
}

static uint16_t build_sensor_short_labeled(SensorData_t datos, char* out_buffer)
{
    char temp_str[12];
    char hum_str[12];
    char pres_str[14];
    char *p = out_buffer;

    float_to_string(datos.temperature, temp_str);
    float_to_string(datos.humidity, hum_str);
    float_to_string(datos.pressure, pres_str);

    // T:<valor>
    *p++ = 'T';
    for (char *s = temp_str; *s; ++s) *p++ = *s;
    *p++ = ' ';

    // H:<valor>
    *p++ = 'H';
    for (char *s = hum_str; *s; ++s) *p++ = *s;
    *p++ = ' ';

    // P:<valor>
    *p++ = 'P';
    for (char *s = pres_str; *s; ++s) *p++ = *s;

    return (uint16_t)(p - out_buffer);
}

void Task_1s(void)/*Task Called Every 1s*/
{
    GPIO_ToggleOutputState();/*Alive indicator.*/
}

void Task_1m(void)/*Task Called Every 1min*/
{
	SensorData_t sensorData;
	Std_ReturnTypes SensorStatus = 0;

	SensorStatus = getTemperaturePressureHumidity(&sensorData);

    if(OK == SensorStatus)/*Storing all data in memory if sensor status is OK.*/
    {
		MEM_DataFrame dataFrame;
		dataFrame.hour = Time_get_hour();
		dataFrame.min = Time_get_minutes();
		dataFrame.sensorData.phys.hum = sensorData.humidity;
		dataFrame.sensorData.phys.press = sensorData.pressure;
		dataFrame.sensorData.phys.temp = sensorData.temperature;
		dataFrame.sensorData.phys.batt = 0.0; /*TODO*/ 

		/*Save frame*/
		MEM_Save_Frame(&dataFrame);

		TxFlag++;
    }

	if(TxFlag >= IRIDIUM_WAIT_CYCLE && SensorStatus == OK)
	{
		uint8_t ModemState = 0;
		uint16_t Len = 0;
		bool SendStatus = false;
		uint32_t time_start = 0;
		char TxMsg[100] = {0};
		uint8_t SignalQlty = 0;

		ModemState = Iridium9603_EnableModem();

		if(ModemState)
		{
			Len = build_sensor_short_labeled(sensorData, TxMsg);
			//Len = build_sensor_string_struct(sensorData, TxMsg);

			time_start = Delay_u32GetTicks();
			while (Delay_u32GetTicks() - time_start < IRIDIUM_TIMEOUT  && !SendStatus)
			{
				SignalQlty = Iridium9603_GetSignalQuality();
				// TEST: Condicion para forzar la transmision
				if(SignalQlty >= (uint8_t)IRIDIUM_SIGNAL_OK)
				{
					(void)Iridium9603_ClearMOBuffer();
					SendStatus = Iridium9603_SendSBDText((uint8_t*)TxMsg, Len);
				}

				Delay_vMs(1000);
			}

			(void)Iridium9603_ClearMOBuffer();
		}

		(void)Iridium9603_DisableModem();
		TxFlag = 0;
	}
}
