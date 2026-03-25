/*
 * SPI_MCAL_cfg.h
 *
 *  Created on: Feb 5, 2024
 *      Author: jarsr
 */

#ifndef SPI_MCAL_CFG_H_
#define SPI_MCAL_CFG_H_

/* System Headers*/
#include "Std_Types.h"

/*
 * Revisar la seccion de Referencial Manual 51.3.1.1 LPSPI memory map.
 */

/** Peripheral LPSPI0 base address */
#define LPSPI0_BASE_ADDR_cfg			(0x4002C000U)

/** Peripheral LPSPI1 base address */
#define LPSPI1_BASE_ADDR_cfg			(0x4002D000U)

/** Peripheral LPSPI2 base address */
#define LPSPI2_BASE_ADDR_cfg			(0x4002E000U)

#endif /* SPI_MCAL_CFG_H_ */
