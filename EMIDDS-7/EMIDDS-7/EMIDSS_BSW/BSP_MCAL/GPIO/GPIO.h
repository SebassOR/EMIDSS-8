/*
 * GPIO.h
 *
 *  Created on: Mar 15, 2024
 *  Author: JARS
 *  Version: 1.0v
 *  Ultima modif.: -
 *  Author: -
 *
 */

#ifndef GPIO_H_
#define GPIO_H_

/*
* Name: GPIO_vInit(void)
* Description: Inicializa las entradas y salidas del microcontrolador.
* Parameters:
*
*/
//extern void GPIO_vInit(void);

/*
* Name: uint32 GPIO_u32SetOutputState(uint8 u8State)
* Description: Inicializa los pines a utilizar del microcontrolador.
* Parameters:
* 				u8Port: Son los puertos [GPIO_enPortA, GPIO_enTotalPorts) de la enumeracion GPIO_enPorts.
* 				u8Pin: Es la enumeracion del Pin a controlar de [PORT_enPin0, PORT_enTotalPIN) de la enumeracion PORT_enPIN.
* 				u8State: Es el estado de control [STD_LOW, STD_HIGH]
*
* 				Return: El estado [E_OK, E_NOT_OK].
*
*/
extern void GPIO_u32SetOutputState(uint8 u8State);
extern void GPIO_ToggleOutputState(void);
void GPIO_ToggleOutputState(void);
void GPIO_vInit(void);

#endif /* GPIO_H_ */
