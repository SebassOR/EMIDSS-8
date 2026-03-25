/*
 * Delay.h
 *
 *  Created on: Mar 15, 2024
 *  Author: JARS
 *  Version: 1.0v
 *  Ultima modif.: -
 *  Author: -
 *
 */

#ifndef DELAY_H_
#define DELAY_H_

/*
* Name: void Delay_vPauseMs(uint32 u32DelayMs)
* Description: La funion pausa el sistema aprox. la cantidad de Milisegundos.
* Parameters:
* 				u32DelayMs: Es la cantidad de Milisegundos de pausa
*/
extern void Delay_vPauseMs(uint32 u32DelayMs);

/*
* Name: void Delay_vMs(uint32 u32DelayMs)
* Description: La funcion cuenta la cantidad de ticks en milissegundos.
* Parameters:
* 				u32DelayMs: Es la cantidad de Milisegundos.
*/
extern void Delay_vMs(uint32 u32DelayMs);

/*
* Name: uint32 Delay_u32GetTicks(void)
* Description: Retorna la cantidad de ticks actual.
* Parameters:
* 			Return u32vTicks la cantidad de ticks de [0, 2^32-1).
*/
extern uint32 Delay_u32GetTicks(void);

/*
* Name: void Delay_vTicksUpdate(uint32 u32Ticks)
* Description: actualiza la ticks del sistema.
* Parameters:
* 			u32Ticks: La cantidad de ticks agregar de [0, 2^32-1) se deja en 1 en la interrupcion por la confguracion.
*/
extern void Delay_vTicksUpdate(uint32 u32Ticks);

/*
* Name: uint8_t Delay_count_1_minute(void)
* Description: returns 1 when its internal counter reaches 1 minute. Returns 0 otherwise
* Parameters:
*
*/
uint8_t Delay_count_1_minute(void);
uint8_t Delay_count_1_second(void);

/*
* Name: void SysTick_Handler(void)
* Description: Se encarga de la interrupcion del Systick
* Parameters:
*
*/
extern void SysTick_Handler(void);


#endif /* DELAY_H_ */
