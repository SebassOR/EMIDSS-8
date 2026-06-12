/*
 * GPIO_cfg.h
 *
 *  Created on: Mar 15, 2024
 *  Author: JARS
 *  Version: 1.0v
 *  Ultima modif.: -
 *  Author: -
 *
 */

#ifndef GPIO_CFG_H_
#define GPIO_CFG_H_

/* Headers */
#include "GPIO_def.h"
//#include "PORT_def.h"

/* Se configura los defines en base al SDK y su manera tipica de ser llamadas
*  para facilitar el uso de los registro de configuracion.
*/

/* Tabla de configuracion para las salidas */

#define GPIO_Inputs_table_cfg {\
	/*	PORT			|	Pin				|	Status	*/	\
	{(uint8)GPIO_enPortC, (uint8)PORT_enPIN3, 0},	/* PB_USER0 */ \
}

/* Tabla de configuracion para las entradas */

#define GPIO_Outputs_table_cfg {\
	/*	PORT			|	Pin				|	Status	*/	\
	{(uint8)GPIO_enPortC, (uint8)PORT_enPIN2, (uint8)STD_LOW},	/* LED_USER0 */ \
	{(uint8)GPIO_enPortC, (uint8)PORT_enPIN14, (uint8)STD_LOW},	/* LPSPI2_PCS0 */\
	{(uint8)GPIO_enPortE, (uint8)PORT_enPIN10, (uint8)STD_LOW}	/* LPSPI2_PCS1 */\
}

#endif /* GPIO_CFG_H_ */
