/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v7.0
processor: S32K144W
package_id: S32K144W_LQFP64
mcu_data: s32sdk_s32k1xx_rtm_401
processor_version: 0.0.0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.
 * The external variables will be used in other source files in application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and integer type.
 * The cast is required to initialize a pointer with an unsigned long define, representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * The cast is required to initialize a pointer with an unsigned long define, representing an address.
 *
 */

#include "pin_mux.h"

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0}
- pin_list:
 - {pin_num: '36', peripheral: LPUART1, signal: rxd, pin_signal: PTC6}
  - {pin_num: '35', peripheral: LPUART1, signal: txd, pin_signal: PTC7}
  - {pin_num: '23', peripheral: LPUART2, signal: rxd, pin_signal: PTD6}
  - {pin_num: '22', peripheral: LPUART2, signal: txd, pin_signal: PTD7}
  - {pin_num: '26', peripheral: PORTC, signal: 'port, 0', pin_signal: PTC0, direction: OUTPUT, initValue: state_1}
  - {pin_num: '25', peripheral: PORTC, signal: 'port, 1', pin_signal: PTC1, direction: OUTPUT, initValue: state_1}
  - {pin_num: '43', peripheral: PORTB, signal: 'port, 12', pin_signal: PTB12, direction: INPUT, ISF: state_1, IRQC: state_1010}
  - {pin_num: '60', peripheral: PORTE, signal: 'port, 0', pin_signal: PTE0, direction: OUTPUT}
  - {pin_num: '39', peripheral: PORTE, signal: 'port, 7', pin_signal: PTE7, direction: OUTPUT}
  - {pin_num: '45', peripheral: PORTD, signal: 'port, 3', pin_signal: PTD3, direction: INPUT, ISF: state_1, IRQC: state_1010}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

  /* Generate array of configured pin structures */
  pin_settings_config_t g_pin_mux_InitConfigArr0[NUM_OF_CONFIGURED_PINS0] = {
    // Pin configuration of I2C to sensor PHT
    {
        .base            = PORTA,
        .pinPortIdx      = 2U,
        .pullConfig      = PORT_INTERNAL_PULL_UP_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_ALT3,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = NULL,
        .digitalFilter   = false,
    },
    {
        .base            = PORTA,
        .pinPortIdx      = 3U,
        .pullConfig      = PORT_INTERNAL_PULL_UP_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_ALT3,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = NULL,
        .digitalFilter   = false,
    },
    {
        .base            = PORTD,
        .pinPortIdx      = 1U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_AS_GPIO,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = PTD,
        .direction       = GPIO_OUTPUT_DIRECTION,
        .digitalFilter   = false,
        .initValue       = 0U,
    },
    {
        .base            = PORTD,
        .pinPortIdx      = 2U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_AS_GPIO,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = PTD,
        .direction       = GPIO_OUTPUT_DIRECTION,
        .digitalFilter   = false,
		        .initValue       = 0U,
    },
    {
        .base            = PORTD,
        .pinPortIdx      = 3U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_AS_GPIO,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = PTD,
        .direction       = GPIO_OUTPUT_DIRECTION,
        .digitalFilter   = false,
        .initValue       = 0U,
    },
    {
        .base            = PORTD,
        .pinPortIdx      = 4U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_AS_GPIO,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = PTD,
        .direction       = GPIO_OUTPUT_DIRECTION,
        .digitalFilter   = false,
        .initValue       = 0U,
    },
    {
        .base            = PORTC,
        .pinPortIdx      = 1U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_AS_GPIO,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = PTC,
        .direction       = GPIO_OUTPUT_DIRECTION,
        .digitalFilter   = false,
        .initValue       = 0U,
    },
	{
	        .base            = PORTC,
	        .pinPortIdx      = 6U,
	        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
	        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
	        .passiveFilter   = false,
	        .mux             = PORT_MUX_ALT2,
	        .pinLock         = false,
	        .intConfig       = PORT_DMA_INT_DISABLED,
	        .clearIntFlag    = false,
	        .gpioBase        = NULL,
	        .digitalFilter   = false,
	    },
	    {
	        .base            = PORTC,
	        .pinPortIdx      = 7U,
	        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
	        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
	        .passiveFilter   = false,
	        .mux             = PORT_MUX_ALT2,
	        .pinLock         = false,
	        .intConfig       = PORT_DMA_INT_DISABLED,
	        .clearIntFlag    = false,
	        .gpioBase        = NULL,
	        .digitalFilter   = false,
	    },
	    {
	        .base            = PORTD,
	        .pinPortIdx      = 6U,
	        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
	        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
	        .passiveFilter   = false,
	        .mux             = PORT_MUX_ALT2,
	        .pinLock         = false,
	        .intConfig       = PORT_DMA_INT_DISABLED,
	        .clearIntFlag    = false,
	        .gpioBase        = NULL,
	        .digitalFilter   = false,
	    },
	    {
	        .base            = PORTD,
	        .pinPortIdx      = 7U,
	        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
	        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
	        .passiveFilter   = false,
	        .mux             = PORT_MUX_ALT2,
	        .pinLock         = false,
	        .intConfig       = PORT_DMA_INT_DISABLED,
	        .clearIntFlag    = false,
	        .gpioBase        = NULL,
	        .digitalFilter   = false,
	    },
  };
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
