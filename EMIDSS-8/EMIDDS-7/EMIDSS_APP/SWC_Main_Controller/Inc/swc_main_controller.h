/*
 * swc_main_controller.h
 *
 *  Created on: 23 abr 2023
 *      Author: luisf
 */

#include "swc_main_controller_config.h"

#ifndef SWC_MAIN_CONTROLLER_INC_SWC_MAIN_CONTROLLER_H_
#define SWC_MAIN_CONTROLLER_INC_SWC_MAIN_CONTROLLER_H_

void request_data_sensors(void);
void save_data_memory(void);
Std_ReturnTypes init_controller(void);
void controller(void);

#endif /* SWC_MAIN_CONTROLLER_INC_SWC_MAIN_CONTROLLER_H_ */
