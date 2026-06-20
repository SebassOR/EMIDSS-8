
/* System Headers*/
#include "Lpspi_Ip.h"
#include "Platform_Types.h"
#include "Sensor_Data.h"
/* Headers */
#include "SPI_MCAL.h"

/* RTD Generated Configurations */
extern const Lpspi_Ip_ConfigType Lpspi_Ip_PhyUnitConfig_SpiPhyUnit_0_Instance_0;
extern const Lpspi_Ip_ExternalDeviceType Lpspi_Ip_DeviceAttributes_SpiExternalDevice_1_Instance_1;

/* Define Instances */
#define SPI_INSTANCE_0    (0U)
#define SPI_TIMEOUT       (1000U) /* 1s */

void SPI_vInitMaster(void)
{
    /* Initialize SPI Instance 0 */
    Lpspi_Ip_Init(&Lpspi_Ip_PhyUnitConfig_SpiPhyUnit_0_Instance_0);
}

uint16 SPI_u8Transmit(uint8 u8SpiModule, uint8 *data, uint16 size)
{
    Lpspi_Ip_StatusType status;
    
    /* Using DeviceAttributes for Instance 1 as per your PBcfg */
    status = Lpspi_Ip_SyncTransmit(&Lpspi_Ip_DeviceAttributes_SpiExternalDevice_1_Instance_1, data, NULL, size, SPI_TIMEOUT);

    return (status == LPSPI_IP_STATUS_SUCCESS) ? OK : NOT_OK;
}

uint16 SPI_u8ReceiveData(uint8 u8SpiModule, uint8* data, uint16 size)
{
    Lpspi_Ip_StatusType status;
    uint8 dummy_tx[64] = {0xFF}; /* Typical for SD cards */

    status = Lpspi_Ip_SyncTransmit(&Lpspi_Ip_DeviceAttributes_SpiExternalDevice_1_Instance_1, dummy_tx, data, size, SPI_TIMEOUT);

    return (status == LPSPI_IP_STATUS_SUCCESS) ? OK : NOT_OK;
}

uint16 SPI_CS(uint8 u8SpiModule, uint8 state)
{
    /* 
     * In S32K3 RTD, CS (PCS) is handled automatically by Lpspi_Ip_SyncTransmit.
     * Manual CS control can be done via GPIO if needed, but standard RTD uses the internal PCS.
     */
    return OK;
}
