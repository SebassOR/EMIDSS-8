/*
 * Platform_Types.h
 *
 * 	Name: Platform_Types.h
 *
 * 	Version: AUTOSAR_v4.4.0
 *
 *  Created on: 16 ene 2024
 *  Author: Jose Rivas
 */

#ifndef PLATFORM_TYPES_H_
#define PLATFORM_TYPES_H_

#include <stdint.h>

/*Specification of Platform Types*/

/*	Type definitions general	*/

/*	CPU_TYPE	*/
#define CPU_TYPE_8	(8)			/* Indicating a 8 bits processor */
#define CPU_TYPE_16	(16)		/* Indicating a 16 bits processor */
#define CPU_TYPE_32	(32)		/* Indicating a 32 bits processor */

/*	CPU_BIT_ORDER	*/
#define MSB_FIRST	(0)			/* The most significant bit is the first bit of the bit sequence */
#define LSB_FIRST	(1)			/* The least significant bit is the first bit of the bit sequence */

/*	CPU_BYTE_ORDER	*/
#define HIGH_BYTE_FIRST	(0)			/* Within uint16, The high byte is located before the low byte (Byte1)*/
#define LOW_BYTE_FIRST	(1)			/* Within uint16, The low byte is located before the high byte (Byte0)*/

/*	Type definitions Freescale	*/

/*	Symbols	*/
//#define CPU_TYPE	(CPU_TYPE_16)
#define CPU_TYPE	(CPU_TYPE_32)
#define CPU_BIT_ORDER (MSB_FIRST)
#define CPU_BYTE_ORDER (HIGH_BYTE_FIRST)

/*	Types	*/
typedef _Bool			boolean;
//typedef unsigned char	boolean;

typedef signed char				sint8;		/* Dec [-128, 127], Hex [0x80, 0x7F] */
typedef unsigned char			uint8;		/* Dec [0, 255], Hex [0x00, 0xFF] */
typedef signed short			sint16;		/* Dec [-32768, 32767], Hex [0x8000, 0x7FFF] */
typedef unsigned short			uint16;		/* Dec [0, 65535], Hex [0x0000, 0xFFFF] */
typedef signed long				sint32;		/* Dec [-2147483648, 2147483647], Hex [0x80000000, 0x7FFFFFFF] */
typedef unsigned long			uint32;		/* Dec [0, 4294967295], Hex [0x00000000, 0xFFFFFFFF] */
typedef signed long long		sint64;		/* Dec [-9223372036854775808, 9223372036854775807], Hex [0x8000000000000000, 0x7FFFFFFFFFFFFFFF] */
//typedef unsigned long long		uint64;		/* Dec [0, 18446744073709551615], Hex [0x0000000000000000, 0xFFFFFFFFFFFFFFFF] */

typedef unsigned short			uint8_least;	/* Dec [0, 255], Hex [0x00, 0xFF] optimized AUTOSAR */
typedef unsigned short			uint16_least;	/* Dec [0, 65535], Hex [0x0000, 0xFFFF] optimized AUTOSAR */
typedef unsigned short			uint32_least;	/* Dec [0, 4294967295], Hex [0x00000000, 0xFFFFFFFF] optimized AUTOSAR */
typedef signed short			sint16_least;	/* Dec [-128, 127], Hex [0x80, 0x7F] optimized AUTOSAR */
typedef signed short			sint16_least;	/* Dec [-32768, 32767], Hex [0x8000, 0x7FFF] optimized AUTOSAR */
typedef signed long				sint32_least;	/* Dec [-2147483648, 2147483647], Hex [0x80000000, 0x7FFFFFFF] optimized AUTOSAR */

typedef float					float32;
//typedef double					float64;

/*	Define	*/

#ifdef TRUE
	#define TRUE ((boolean)1)
#endif

#ifdef FALSE
	#define FALSE ((boolean)0)
#endif

#ifdef True
	#define True ((boolean)1)
#endif

#ifdef False
	#define False ((boolean)0)
#endif



#endif /* PLATFORM_TYPES_H_ */
