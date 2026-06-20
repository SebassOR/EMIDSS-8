#include "Sensor_Data.h"

/*Own Headers*/
#include "GPIO.h"
#include "swc_os.h"
#include "swc_tem_pre_hum.h"
#include "mem.h"
#include "timeCounter.h"
#include "Iridium9603.h"
#include "Delay.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"

/*************** for testing I2C**************/
#include "I2C.h"
#include "Lpi2c_Ip_Types.h"
#include "Iridium9603_def.h"
#include "Lpi2c_Ip.h"
#include "Lpi2c_Ip_Sa_PBcfg.h"
//#define IO_REG  0x10
//****************************************//

#define IRIDIUM_TIMEOUT			(60000U) /*1 minute*/
#define IRIDIUM_WAIT_CYCLE		(1)//changing to 1 to test EMDISS-8
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

    // Temperature: 23.45 �C\n
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

	/********TEEEESTING I2C    ************////

    uint8_t test_msg[] = "im Alive";
    uint32_t msg_length = strlen((char*)test_msg);
    uint8_t reg = IO_REG;
    uint16_t target_address = 0x63;

   // I2C_vInit(void);

    Lpi2c_Ip_MasterInit(I2C_INSTANCE_1, &I2c_Lpi2cMaster_HwChannel1_Channel0);
   // Lpi2c_Ip_MasterInit(I2C_INSTANCE_0, &I2c_Lpi2cMaster_HwChannel1_Channel0);

    Lpi2c_Ip_MasterSetSlaveAddr(I2C_INSTANCE_1, target_address, (boolean)false);
   // Lpi2c_Ip_MasterSetSlaveAddr(I2C_INSTANCE_0, target_address, (boolean)false);

    Lpi2c_Ip_StatusType stat = Lpi2c_Ip_MasterSendDataBlocking(I2C_INSTANCE_1, test_msg, msg_length, true, 100U);


    stat = Lpi2c_Ip_MasterSendDataBlocking(I2C_INSTANCE_0, test_msg, msg_length, true, 100U);
        if (stat != LPI2C_IP_SUCCESS_STATUS)
        	//while(1);


	//*********************************************///

	SensorStatus = getTemperaturePressureHumidity(&sensorData);

    //if(OK == SensorStatus)/*Storing all data in memory if sensor status is OK.*/
    //{
	//	MEM_DataFrame dataFrame;
	//	dataFrame.hour = Time_get_hour();
	//	dataFrame.min = Time_get_minutes();
	//	dataFrame.sensorData.phys.hum = sensorData.humidity;
	//	dataFrame.sensorData.phys.press = sensorData.pressure;
	//	dataFrame.sensorData.phys.temp = sensorData.temperature;
	//	dataFrame.sensorData.phys.batt = 0.0; /*TODO*/

		/*Save frame*/
	//	MEM_Save_Frame(&dataFrame);

		TxFlag++;
    //}

	if(TxFlag >= IRIDIUM_WAIT_CYCLE) //&& SensorStatus == OK)
	{
		uint8_t ModemState = 0;
		uint16_t Len = 0;
		bool SendStatus = false;
		uint32_t time_start = 0;
		char TxMsg[100] = "Im Alive";
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
