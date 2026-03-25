/*
 * Std_Types.h
 *
 * 	Name: Std_Types.h
 *
 * 	Version: AUTOSAR_v4.4.0
 *
 *  Created on: 16 ene 2024
 *  Author: Jose Rivas
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_

#include "Platform_Types.h"

/*	Define	*/
#ifndef	NULL
	#define NULL 0
#endif

#ifndef	MIN
	#define MIN(_x,_y)	(((_x) < (_y)) ? (_x) : (_y))
#endif

#ifndef	MAX
	#define MAX(_x,_y)	(((_x) > (_y)) ? (_x) : (_y))
#endif

/*	ReturnType	*/

#define STD_HIGH		0x01U
#define STD_LOW			0x00U

#define STD_ACTIVE		0x01U
#define STD_IDLE		0x00U

#define STD_ON			0x01U
#define STD_OFF			0x00U

#define STD_ON			0x01U
#define STD_OFF			0x00U

typedef uint8 Std_ReturnType;

#define E_OK			((Std_ReturnType)0x00U)
#define E_NOT_OK		((Std_ReturnType)0x01U)


#endif /* STD_TYPES_H_ */
