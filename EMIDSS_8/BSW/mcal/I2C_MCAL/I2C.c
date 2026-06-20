/*
 * I2C.c
 *
 *  Created on: Mar 15, 2024
 *  Author: JARS
 *  Version: 1.0v
 *  Ultima modif.: Jun 9, 2024
 *  Author: Noe Ortiz Terrones
 *
 */

/* System Headers*/
#include "Lpi2c_Ip.h"
#include "Platform_Types.h"
#include "Sensor_Data.h"

/* Headers */
#include "Delay.h"
#include "I2C.h"

/* Define Instances */

#define I2C_TIMEOUT       (100U) /* 100ms */

/* RTD Generated Configurations */

//extern const Lpi2c_Ip_MasterConfigType I2c_Lpi2cMaster_HwChannel0_Channel1;
//extern const Lpi2c_Ip_MasterConfigType I2c_Lpi2cMaster_HwChannel1_Channel0;

void I2C_vInit(void)
{
    /* 
     * Initialize I2C Instance 0 (Sensors -> LPI2C_0) 
     * Mapped to I2c_Lpi2cMaster_HwChannel0_Channel1 (MasterStateIdx = 1)
     */
    Lpi2c_Ip_MasterInit(I2C_INSTANCE_0, &I2c_Lpi2cMaster_HwChannel0_Channel1);
    
    /* 
     * Initialize I2C Instance 1 (Iridium/Other -> LPI2C_1) 
     * Mapped to I2c_Lpi2cMaster_HwChannel1_Channel0 (MasterStateIdx = 0)
     */
    Lpi2c_Ip_MasterInit(I2C_INSTANCE_1, &I2c_Lpi2cMaster_HwChannel1_Channel0);
}

Std_ReturnType I2C0_u8Write(uint8_t u8SlaveAddr, uint8_t u8Register, uint8_t u8Data)
{
    uint8_t txData[2] = {u8Register, u8Data};
    Lpi2c_Ip_StatusType status;

    Lpi2c_Ip_MasterSetSlaveAddr(I2C_INSTANCE_0, u8SlaveAddr, false);
    status = Lpi2c_Ip_MasterSendDataBlocking(I2C_INSTANCE_0, txData, 2U, true, I2C_TIMEOUT);
    
    return (status == LPI2C_IP_SUCCESS_STATUS) ? OK : NOT_OK;
}

Std_ReturnType I2C_u8WriteCmd(uint8_t u8SlaveAddr, uint8_t u8Cmd)
{
    Lpi2c_Ip_StatusType status;

    Lpi2c_Ip_MasterSetSlaveAddr(I2C_INSTANCE_0, u8SlaveAddr, false);
    status = Lpi2c_Ip_MasterSendDataBlocking(I2C_INSTANCE_0, &u8Cmd, 1U, true, I2C_TIMEOUT);

    return (status == LPI2C_IP_SUCCESS_STATUS) ? OK : NOT_OK;
}

Std_ReturnType I2C0_u8Read(uint8_t u8SlaveAddr, uint8_t *pu8Data, uint8_t u8NumData)
{
    Lpi2c_Ip_StatusType status;

    Lpi2c_Ip_MasterSetSlaveAddr(I2C_INSTANCE_0, u8SlaveAddr, false);
    /* Note: If you need to write the register address first, use a combined transfer or a separate write */
    status = Lpi2c_Ip_MasterReceiveDataBlocking(I2C_INSTANCE_0, pu8Data, u8NumData, true, I2C_TIMEOUT);

    return (status == LPI2C_IP_SUCCESS_STATUS) ? OK : NOT_OK;
}

/* API for the second I2C instance */
Std_ReturnType I2C1_u8Write(uint8_t u8SlaveAddr, uint8_t u8Register, uint8_t u8Data)
{
    uint8_t txData[2] = {u8Register, u8Data};
    Lpi2c_Ip_StatusType status;

    Lpi2c_Ip_MasterSetSlaveAddr(I2C_INSTANCE_1, u8SlaveAddr, false);
    status = Lpi2c_Ip_MasterSendDataBlocking(I2C_INSTANCE_1, txData, 2U, true, I2C_TIMEOUT);
    
    return (status == LPI2C_IP_SUCCESS_STATUS) ? OK : NOT_OK;
}

Std_ReturnType I2C1_u8Read(uint8_t u8SlaveAddr, uint8_t *pu8Data, uint8_t u8NumData)
{
    Lpi2c_Ip_StatusType status;

    Lpi2c_Ip_MasterSetSlaveAddr(I2C_INSTANCE_1, u8SlaveAddr, false);
    /* Note: If you need to write the register address first, use a combined transfer or a separate write */
    status = Lpi2c_Ip_MasterReceiveDataBlocking(I2C_INSTANCE_1, pu8Data, u8NumData, true, I2C_TIMEOUT);

    return (status == LPI2C_IP_SUCCESS_STATUS) ? OK : NOT_OK;
}


/* EOF */
