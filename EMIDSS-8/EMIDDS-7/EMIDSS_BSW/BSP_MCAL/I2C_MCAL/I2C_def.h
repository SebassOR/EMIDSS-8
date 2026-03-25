/*
 * I2C_def.h
 *
 *  Created on: Mar 15, 2024
 *  Author: JARS
 *  Version: 1.0v
 *  Ultima modif.: Jun 9, 2024
 *  Author: Noe Ortiz Terrones
 *
 */

#ifndef I2C_DEF_H_
#define I2C_DEF_H_

#define LPI2C_MTDR_SHIFT				(1U)
#define LPI2C_MTDR_WRITE				(0U)
#define LPI2C_MTDR_READ					(1U)
#define LPI2C_MTDR_ADDRESS_W(x)			((uint32_t)(x) << LPI2C_MTDR_SHIFT) | LPI2C_MTDR_WRITE
#define LPI2C_MTDR_ADDRESS_R(x)			((uint32_t)(x) << LPI2C_MTDR_SHIFT) | LPI2C_MTDR_READ

#define WRITE							0U
#define READ							1U

/** Tiempo de espera para salir de I2C */
#define TimeOutI2C        				(1000U)

/** Tiempo de espera para la lectura de I2C */
#define ReadTimeI2C       				(2000U)

/** Tiempo de espera para el Stop de I2C */
#define StopTimeI2C       				(3000U)

#define I2C_GEN_START_CMD				0x4U
#define I2C_GEN_STOP_CMD				0x2U
#define I2C_RECEIVE_CMD					0x1U

/*
 * Name: I2C_tenI2CsPorts.
 *  Description: Es una enumeracion para los puertos I2C
 *  			 selecionados.
 *
 * */
typedef enum{
	I2C_enI2C0 = 0,
	I2C_enI2C1,
	I2C_enTotalI2Cs
}I2C_tenI2CsPorts;

#endif /* I2C_DEF_H_ */
