


#include "SGP40_AIR_QUALITY.h"




#define SGP40_I2C_ADDRESS 0x59

#define COMMAND_SERIAL_NUMBER 0x3682

void delay(volatile int cycles)
{
    /* Delay function - do nothing for a number of cycles */
    while(cycles--);
}

uint16 bytes_to_uint16(const uint8_t* bytes)
{
    return (uint16_t)bytes[0] << 8 | (uint16_t)bytes[1];
}

uint16 add_command_to_buffer(uint8* buffer, uint16 offset, uint16 command)
{
    buffer[offset++] = (uint8)((command & 0xFF00) >> 8);
    buffer[offset++] = (uint8)((command & 0x00FF) >> 0);

    return offset;
}

sint16 sgp40_measure_raw_signal(uint16 relative_humidity,
                                 uint16 temperature, uint16* sraw_voc) {
    sint16 error;
    uint8 buffer[8];
    uint16 offset = 0;
    offset = add_command_to_buffer(&buffer[0], offset, 0x260F);

    offset = sensirion_i2c_add_uint16_to_buffer(&buffer[0], offset,
                                                  relative_humidity);
    offset =
        sensirion_i2c_add_uint16_to_buffer(&buffer[0], offset, temperature);

    error = sensirion_i2c_write_data(SGP40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(30000);

    error = sensirion_i2c_read_data_inplace(SGP40_I2C_ADDRESS, &buffer[0], 2);
    if (error) {
        return error;
    }
    *sraw_voc = sensirion_common_bytes_to_uint16(&buffer[0]);
    return OK;
}

sint16 sgp40_execute_self_test(uint16* test_result) {
    sint16 error;
    uint8 buffer[3];
    uint16 offset = 0;
    offset = add_command_to_buffer(&buffer[0], offset, 0x280E);

    error = sensirion_i2c_write_data(SGP40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(320000);

    error = sensirion_i2c_read_data_inplace(SGP40_I2C_ADDRESS, &buffer[0], 2);
    if (error) {
        return error;
    }
    *test_result = sensirion_common_bytes_to_uint16(&buffer[0]);
    return OK;
}

sint16 sgp40_turn_heater_off(void) {
    sint16 error;
    uint8 buffer[2];
    uint16 offset = 0;
    offset = add_command_to_buffer(&buffer[0], offset, 0x3615);

    error = sensirion_i2c_write_data(SGP40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(1000);
    return OK;
}

sint16 sgp40_get_serial_number(uint16* serial_number,
                                uint8 serial_number_size) {
    sint16 error;
    uint8 buffer[9];
    uint16 offset = 0;
    
    offset = add_command_to_buffer(&buffer[0], offset, COMMAND_SERIAL_NUMBER);

    LPI2C_DRV_MasterSendDataBlocking(INST_LPI2C0, buffer, 9, true, OSIF_WAIT_FOREVER);

    // error = sensirion_i2c_write_data(SGP40_I2C_ADDRESS, &buffer[0], offset);
    // if (error) {
    //     return error;
    // }

    // do change for time to wait
    //sensirion_i2c_hal_sleep_usec(1000);  
    delay(720000);

    LPI2C_DRV_MasterReceiveDataBlocking(INST_LPI2C0, buffer, 9, true, OSIF_WAIT_FOREVER);

    // error = sensirion_i2c_read_data_inplace(SGP40_I2C_ADDRESS, &buffer[0], 6);
    // if (error) {
    //     return error;
    // }

    serial_number[0] = bytes_to_uint16(&buffer[0]);
    serial_number[1] = bytes_to_uint16(&buffer[2]);
    serial_number[2] = bytes_to_uint16(&buffer[4]);

    return OK;
}