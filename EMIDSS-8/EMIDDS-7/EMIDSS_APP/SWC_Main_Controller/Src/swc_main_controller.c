


/****************************** INCLUDES FILES ************************************/

#include "Std_types.h"
//#include "swc_imu.h"
//#include "swc_air_quality.h"
#include "swc_memory.h"
#include "swc_main_controller.h"

/**********************************************************************************/


//SensorData data;
//SensorData *pdata = &data;


//IRQ handler del RTC

void request_data_sensors(void)
{
	//request_data_voc();

	//request_data_co2();

	//request_data_euler();

	//request_data_humidity();
	
	//request_data_temp(pdata);

	//request_data_press(pdata);
}


//Save data from structure to memory
/*void save_data_memory(void){
	store_sensor_data_structure(&data);
}*/


typedef void (*Functions_Management)(void);
Functions_Management managment[]={
		request_data_sensors,
		save_data_memory
};

Std_ReturnTypes init_controller(void)
{
	Std_ReturnTypes std_result = NOT_OK;
	std_result = modules_init();


	return std_result;
}


void controller(void)
{
	request_data_sensors();

	//managment[0]();
	//managment[1]();
	//MEM_TestComm();
}












