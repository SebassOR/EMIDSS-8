/*
 * PORT_def.h
 *
 *  Created on: Mar 15, 2024
 *  Author: JARS
 *  Version: 1.0v
 *  Ultima modif.: -
 *  Author: -
 *
 */

#ifndef PORT_DEF_H_
#define PORT_DEF_H_

/*
 * Enumeracion para definir el Pin del puerto.
 */
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


#endif /* PORT_DEF_H_ */
