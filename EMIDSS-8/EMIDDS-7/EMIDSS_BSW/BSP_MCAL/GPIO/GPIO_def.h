/*
 * GPIO_def.h
 *
 *  Created on: Mar 15, 2024
 *  Author: JARS
 *  Version: 1.0v
 *  Ultima modif.: -
 *  Author: -
 *
 */

#ifndef GPIO_DEF_H_
#define GPIO_DEF_H_

#include <stdlib.h>

typedef uint8 Std_ReturnType;

#define E_OK			((Std_ReturnType)0x00U)
#define E_NOT_OK		((Std_ReturnType)0x01U)
#define STD_HIGH		0x01U
#define STD_LOW			0x00U

#define STD_ACTIVE		0x01U
#define STD_IDLE		0x00U

#define STD_ON			0x01U
#define STD_OFF			0x00U

#define STD_ON			0x01U
#define STD_OFF			0x00U

#define STD_UNDEFINE	0x02U

/*
 * Enumeracion para definir las salidas.
 */
typedef enum{
	GPIO_enOutput0 = 0,
	GPIO_enOutput1,
	GPIO_enOutput2,
	GPIO_enTotalOutputs
}GPIO_enOutputs;

/*
 * Enumeracion para definir las entradas.
 */
typedef enum{
	GPIO_enInput0 = 0,
	GPIO_enTotalInputs
}GPIO_enInputs;

/*
 * Enumeracion para los puertos.
 */
typedef enum{
	GPIO_enPortA = 0,
	GPIO_enPortB,
	GPIO_enPortC,
	GPIO_enPortD,
	GPIO_enPortE,
	GPIO_enTotalPorts
}GPIO_enPorts;

typedef struct {
	uint32 PDOR;        /**< Port Data Output Register, offset: 0x0 */
	uint32 PSOR;      	/**< Port Set Output Register, offset: 0x4 */
	uint32 PCOR;      	/**< Port Clear Output Register, offset: 0x8 */
	uint32 PTOR;      	/**< Port Toggle Output Register, offset: 0xC */
	uint32 PDIR;      	/**< Port Data Input Register, offset: 0x10 */
	uint32 PDDR;      	/**< Port Data Direction Register, offset: 0x14 */
	uint32 PIDR;		/**< Port Input Disable Register, offset: 0x18 */
} GPIO_tstMemMapPtr;

/*
 * Estrutura que almacena los status iniciales
 */
typedef struct{
	uint8 u8DirPort;
	uint8 u8Pin;
	uint8 u8Status;
}GPIO_stGPIOPin;

/*PIN PORTS*/
typedef enum{
	PORT_enPIN0 = 0,
	PORT_enPIN1,
	PORT_enPIN2,
	PORT_enPIN3,
	PORT_enPIN4,
	PORT_enPIN5,
	PORT_enPIN6,
	PORT_enPIN7,
	PORT_enPIN8,
	PORT_enPIN9,
	PORT_enPIN10,
	PORT_enPIN11,
	PORT_enPIN12,
	PORT_enPIN13,
	PORT_enPIN14,
	PORT_enPIN15,
	PORT_enPIN16,
	PORT_enPIN17,
	PORT_enTotalPIN
}PORT_enPIN;

/*
 * Enumeracion para definir el modo de uson de un Pin.
 */
typedef enum{
	PORT_enPINMUX0 = 0,
	PORT_enPINMUX1,
	PORT_enPINMUX2,
	PORT_enPINMUX3,
	PORT_enPINMUX4,
	PORT_enPINMUX5,
	PORT_enPINMUX6,
	PORT_enPINMUX7,
	PORT_enTotalPINMUX
}PORT_enPINMUX;

#endif /* GPIO_DEF_H_ */
