/*
 * Platform_Types.h
 *
 *  Created on: 19 abr 2023
 *      Author: danie
 */

#ifndef PLATFORM_TYPES_H_
#define PLATFORM_TYPES_H_

#if !defined(NO_STDINT_H)
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#endif /* !defined(NO_STDINT_H) */

#ifndef TRUE
#if !defined(NO_STDINT_H)
    /**
    * @brief Boolean true value
    * @implements TRUE_FALSE_enum
    */
    #define TRUE true
#else
    /**
    * @brief Boolean true value
    * @implements TRUE_FALSE_enum
    */
    #define TRUE 1
#endif /* !defined(NO_STDINT_H) */
#endif
#ifndef FALSE
#if !defined(NO_STDINT_H)
    /**
    * @brief Boolean false value
    * @implements TRUE_FALSE_enum
    */
    #define FALSE false
#else
    /**
    * @brief Boolean false value
    * @implements TRUE_FALSE_enum
    */
    #define FALSE 0
#endif /* !defined(NO_STDINT_H) */
#endif

typedef _Bool 				boolean;
typedef char 				sint8;
typedef unsigned char 		uint8;
typedef short 				sint16;
typedef unsigned short 		uint16;
typedef int 				sint32;
typedef unsigned int 		uint32;
typedef float 				float32;

#endif /* PLATFORM_TYPES_H_ */
