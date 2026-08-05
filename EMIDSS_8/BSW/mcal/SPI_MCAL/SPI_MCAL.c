
/* System Headers*/
#include "Lpspi_Ip.h"
#include "Platform_Types.h"
#include "Sensor_Data.h"
#include "SPI_MCAL_def.h"
#include <string.h>
/* Headers */
#include "SPI_MCAL.h"

/* RTD Generated Configurations */
extern const Lpspi_Ip_ConfigType Lpspi_Ip_PhyUnitConfig_SpiPhyUnit_0_Instance_0;
extern const Lpspi_Ip_ConfigType Lpspi_Ip_PhyUnitConfig_SpiPhyUnit_1_Instance_1;
extern const Lpspi_Ip_ExternalDeviceType Lpspi_Ip_DeviceAttributes_SpiExternalDevice_1_Instance_1;

#define SPI_TIMEOUT       (100000U)

/* Hardware Instance 2 (LPSPI2: PTA8 MOSI, PTB2 MISO, PTC15 SCK, PTC10 PCS1) */
static Lpspi_Ip_DeviceParamsType LPSPI2_DeviceParams = {
    (uint8)8U,      /* 8-bit frame size */
    (boolean)FALSE, /* MSB First */
    (boolean)FALSE, /* ByteSwap disabled */
    (uint32)0U
};

static const Lpspi_Ip_ExternalDeviceType Lpspi_Ip_DeviceAttributes_LPSPI2_BME280 = {
    2U,  /* Hardware Instance 2 (LPSPI2) */
    (uint32)(LPSPI_CFGR0_HRDIR(0U) | LPSPI_CFGR0_HREN(0U) | LPSPI_CFGR0_HRSEL(0U) | LPSPI_CFGR0_HRPOL(0U)),
    (uint32)(LPSPI_CCR_SCKPCS(4U) | LPSPI_CCR_PCSSCK(4U)),
    (uint32)(LPSPI_CCR1_SCKSCK(123U) | LPSPI_CCR1_PCSPCS(4U) | LPSPI_CCR1_SCKHLD(10U) | LPSPI_CCR1_SCKSET(10U)), /* SCKDIV = 123 */
    (uint32)(LPSPI_TCR_WIDTH(0U) | LPSPI_TCR_CPOL(0U) | LPSPI_TCR_CPHA(0U) | LPSPI_TCR_PRESCALE(6U) | LPSPI_TCR_PCS(1U) | LPSPI_TCR_CONT(1U)), /* PRESCALE = 6 (/64), CONT = 1 (Keep CS LOW) */
    &LPSPI2_DeviceParams
};

static const Lpspi_Ip_ConfigType Lpspi_Ip_PhyUnitConfig_LPSPI2 = {
    2U,  /* Hardware Instance 2 (LPSPI2) */
    (uint32)0,
    (uint32)(LPSPI_CFGR1_PINCFG(0U) | LPSPI_CFGR1_PCSPOL(0U) | LPSPI_CFGR1_MASTER(1U) | LPSPI_CFGR1_SAMPLE(0U)),
    LPSPI_IP_POLLING,
    2U   /* Dedicated StateIndex = 2U for Instance 2 */
};

static const Lpspi_Ip_ExternalDeviceType* SPI_pGetDeviceAttr(uint8 u8SpiModule)
{
    if (u8SpiModule == SPI_enSPI2 || u8SpiModule == 2U)
    {
        return &Lpspi_Ip_DeviceAttributes_LPSPI2_BME280;
    }
    return &Lpspi_Ip_DeviceAttributes_SpiExternalDevice_1_Instance_1;
}

void SPI_vInitMaster(void)
{
    /* Initialize SPI Instance 0, Instance 1, and Hardware Instance 2 (LPSPI2) */
    Lpspi_Ip_Init(&Lpspi_Ip_PhyUnitConfig_SpiPhyUnit_0_Instance_0);
    Lpspi_Ip_Init(&Lpspi_Ip_PhyUnitConfig_SpiPhyUnit_1_Instance_1);
    Lpspi_Ip_Init(&Lpspi_Ip_PhyUnitConfig_LPSPI2);
}

uint16 SPI_u8Transmit(uint8 u8SpiModule, uint8 *data, uint16 size)
{
    Lpspi_Ip_StatusType status;
    const Lpspi_Ip_ExternalDeviceType *pDevice = SPI_pGetDeviceAttr(u8SpiModule);

    status = Lpspi_Ip_SyncTransmit(pDevice, data, NULL_PTR, size, SPI_TIMEOUT);

    return (status == LPSPI_IP_STATUS_SUCCESS) ? OK : NOT_OK;
}

uint16 SPI_u8ReceiveData(uint8 u8SpiModule, uint8* data, uint16 size)
{
    Lpspi_Ip_StatusType status;
    uint8 dummy_tx[64];
    const Lpspi_Ip_ExternalDeviceType *pDevice = SPI_pGetDeviceAttr(u8SpiModule);

    memset(dummy_tx, 0xFF, sizeof(dummy_tx));
    status = Lpspi_Ip_SyncTransmit(pDevice, dummy_tx, data, size, SPI_TIMEOUT);

    return (status == LPSPI_IP_STATUS_SUCCESS) ? OK : NOT_OK;
}

uint16 SPI_u8Transfer(uint8 u8SpiModule, uint8 *txData, uint8 *rxData, uint16 size)
{
    Lpspi_Ip_StatusType status;
    const Lpspi_Ip_ExternalDeviceType *pDevice = SPI_pGetDeviceAttr(u8SpiModule);

    status = Lpspi_Ip_SyncTransmit(pDevice, txData, rxData, size, SPI_TIMEOUT);

    return (status == LPSPI_IP_STATUS_SUCCESS) ? OK : NOT_OK;
}

uint16 SPI_u8ReadRegister(uint8 u8SpiModule, uint8 reg, uint8 *pData, uint16 size)
{
    uint8 txBuffer[32];
    uint8 rxBuffer[32] = {0};
    uint16 status;
    uint16 i;

    if (size > 31U)
    {
        return NOT_OK;
    }

    memset(txBuffer, 0xFF, sizeof(txBuffer));
    txBuffer[0] = (uint8)((reg & 0x7FU) | 0x80U);

    status = SPI_u8Transfer(u8SpiModule, txBuffer, rxBuffer, (uint16)(size + 1U));
    if (OK == status )
    {
        for (i = 0U; i < size; i++)
        {
            pData[i] = rxBuffer[i + 1U];
        }
    }
    return status;
}

uint16 SPI_u8WriteRegister(uint8 u8SpiModule, uint8 reg, uint8 data)
{
    uint8 txBuffer[2];

    txBuffer[0] = reg & 0x7FU; /* SPI write bit 7 clear */
    txBuffer[1] = data;

    return SPI_u8Transmit(u8SpiModule, txBuffer, 2U);
}

uint16 SPI_CS(uint8 u8SpiModule, uint8 state)
{
    /* In S32K3 RTD, CS (PCS) is handled automatically by Lpspi_Ip_SyncTransmit. */
    (void)u8SpiModule;
    (void)state;
    return OK;
}

