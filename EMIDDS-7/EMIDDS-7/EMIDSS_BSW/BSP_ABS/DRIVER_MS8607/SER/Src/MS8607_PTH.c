/*
 * MS8607_PTH.c
 *
 *  Created on: Jun 9, 2024
 *      Author: Noe Ortiz Terrones
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "MS8607_PTH.h"
#include "pins_driver.h"

/*******************************************************************************
 * Global variables
 ******************************************************************************/
ms8607_instance_t ms8607_config;
static uint16_t eeprom_coeff[COEFFICIENT_NUMBERS+1];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static Std_ReturnTypes hsensor_reset(void);
static Std_ReturnTypes hsensor_write_command(uint8_t);
static Std_ReturnTypes hsensor_crc_check(uint16_t, uint8_t);
static Std_ReturnTypes hsensor_read_user_register(uint8_t *, uint8_t);
static Std_ReturnTypes hsensor_write_user_register(uint8_t);
static Std_ReturnTypes hsensor_humidity_conversion_and_read_adc(uint16_t *);
static Std_ReturnTypes hsensor_read_relative_humidity(float32 *);
static Std_ReturnTypes psensor_reset(void);
static Std_ReturnTypes psensor_write_command(uint8_t);
static Std_ReturnTypes psensor_read_eeprom_coeff(uint8_t, uint16_t*);
static Std_ReturnTypes psensor_read_eeprom(void);
static Std_ReturnTypes psensor_conversion_and_read_adc(uint8_t, uint32_t *);
static Std_ReturnTypes psensor_crc_check (uint16_t *n_prom, uint8_t crc);
static Std_ReturnTypes psensor_read_pressure_and_temperature(float32 *, float32 *);
static void MS8607_vConfig(void);
static void MS8607_vGetADCDelay(void);

/*******************************************************************************
 * Static functions
 ******************************************************************************/
/**
 * \brief Reset the humidity sensor
 */
static Std_ReturnTypes hsensor_reset(void)
{
	return hsensor_write_command(H_SENSOR_RESET_COMMAND);
}

/**
 * \brief Writes the Humidity sensor 8-bits command with the value passed
 */
static Std_ReturnTypes hsensor_write_command(uint8_t cmd)
{
	return I2C_u8WriteCmd(H_SENSOR_ADDR, cmd);
}

/**
 * \brief Check CRC
 */
static Std_ReturnTypes hsensor_crc_check(uint16_t value, uint8_t crc)
{
	uint32_t polynom = 0x988000; // x^8 + x^5 + x^4 + 1
	uint32_t msb     = 0x800000;
	uint32_t mask    = 0xFF8000;
	uint32_t result  = (uint32_t)value<<8; // Pad with zeros as specified in spec
	uint8_t res_crc = OK;

	while( msb != 0x80 ){

		// Check if msb of current value is 1 and apply XOR mask
		if( result & msb )
			result = ((result ^ polynom) & mask) | ( result & ~mask);

		// Shift by one
		msb >>= 1;
		mask >>= 1;
		polynom >>=1;
	}
	if(result == crc)
		res_crc = ms8607_status_ok;
	else
		res_crc = ms8607_status_crc_error;

	return res_crc;
}

/**
 * \brief Reads the MS8607 humidity user register.
 */
static Std_ReturnTypes hsensor_read_user_register(uint8_t *value, uint8_t size)
{
	Std_ReturnTypes error;

	// Send the Read Register Command
	error = I2C_u8WriteCmd(H_SENSOR_ADDR, H_SENSOR_READ_USER_REG_COMMAND);
	if(!error){
		/* Request data from the bus slave */
		error = I2C0_u8Read(H_SENSOR_ADDR, value, size);
	}

	return error;
}

/**
 * \brief Writes the MS8607 humidity user register with value
 */
static Std_ReturnTypes hsensor_write_user_register(uint8_t value)
{
	Std_ReturnTypes error;

	error = I2C0_u8Write(H_SENSOR_ADDR, H_SENSOR_WRITE_USER_REG_COMMAND, value);

	return error;
}

/**
 * \brief Reads the relative humidity ADC value
 */
static Std_ReturnTypes hsensor_humidity_conversion_and_read_adc(uint16_t *humidity)
{
	Std_ReturnTypes error;
	uint16_t adc;
	uint8_t buffer[3] = {0};
	uint8_t crc;

	if(ms8607_config.humidityMeasureMode == ms8607_i2c_hold)
		error = hsensor_write_command(H_SENSOR_READ_HUMIDITY_W_HOLD_COMMAND);
	else
		error = hsensor_write_command(H_SENSOR_READ_HUMIDITY_WO_HOLD_COMMAND);

	if(!error){
		Delay_vMs(H_SENSOR_CONVERSION_TIME);
		/* Request data from the bus slave */
		I2C0_u8Read(H_SENSOR_ADDR, buffer, sizeof(buffer));

		adc = (buffer[0] << 8) | buffer[1];
		crc = buffer[2];

		// compute CRC
		error = hsensor_crc_check(adc,crc);
		if(!error)
			*humidity = adc;
		else{
			//Error: CRC reading error
		}
	}
	else{
		//Error: Humidity conversion writing error
	}

	return error;
}

/**
 * \brief Reads the relative humidity value.
 */
static Std_ReturnTypes hsensor_read_relative_humidity(float32 *humidity)
{
	Std_ReturnTypes error;
	uint16_t adc;

	error = hsensor_humidity_conversion_and_read_adc(&adc);
	if(!error){
		// Perform conversion function
		*humidity = (float32)adc * HUMIDITY_COEFF_MUL / (1UL<<16) + HUMIDITY_COEFF_ADD;
	}
	else{
		//Do nothing
	}

	return error;
}

/**
 * \brief Reset the Pressure sensor part
 */
static Std_ReturnTypes psensor_reset(void)
{
	return psensor_write_command(TP_SENSOR_RESET_COMMAND);
}

/**
 * \brief Writes the Pressure Sensor 8-bits command with the value passed
 */
static Std_ReturnTypes psensor_write_command(uint8_t cmd)
{
	return I2C_u8WriteCmd(TP_SENSOR_ADDR, cmd);
}

/**
 * \brief Reads the temperature, pressure sensor EEPROM coefficient stored at address provided.
 */
static Std_ReturnTypes psensor_read_eeprom_coeff(uint8_t coeffIndex, uint16_t *coeff)
{
	Std_ReturnTypes error;
	uint8_t buffer[2] = {0};

	switch(coeffIndex){
		case 0:		error = psensor_write_command(PROM_ADDRESS_READ_ADDRESS_0);		break;
		case 1:		error = psensor_write_command(PROM_ADDRESS_READ_ADDRESS_1);		break;
		case 2:		error = psensor_write_command(PROM_ADDRESS_READ_ADDRESS_2);		break;
		case 3:		error = psensor_write_command(PROM_ADDRESS_READ_ADDRESS_3);		break;
		case 4:		error = psensor_write_command(PROM_ADDRESS_READ_ADDRESS_4);		break;
		case 5:		error = psensor_write_command(PROM_ADDRESS_READ_ADDRESS_5);		break;
		case 6:		error = psensor_write_command(PROM_ADDRESS_READ_ADDRESS_6);		break;
		default:	error = NOT_OK;													break;
	}

	if(!error){
		error = I2C0_u8Read(TP_SENSOR_ADDR, buffer, sizeof(buffer));
		if(!error){
			*coeff = (buffer[0] << 8) | buffer[1];

			if (*coeff == 0){
				error = ms8607_status_i2c_transfer_error;
			}
			else{
				//Do nothing
			}
		}
		else{
			//ERROR: I2C reading operation error
		}
	}
	else{
		//ERROR: Command to read coefficients went wrong
	}

	return error;
}

/**
 * \brief Reads the ms8607 EEPROM coefficients to store them for computation.
 */
static Std_ReturnTypes psensor_read_eeprom(void)
{
	Std_ReturnTypes error = OK;
	uint8_t i;

	for(i=0;i<COEFFICIENT_NUMBERS||error;i++){
		error = psensor_read_eeprom_coeff(i, eeprom_coeff+i);
	}

	if(!error){
		// Check CRC(Coefficients, CRC)
		if(!psensor_crc_check(eeprom_coeff, (eeprom_coeff[CRC_INDEX] & 0xF000)>>12)){
		}
		else{
			error = ms8607_status_crc_error;
		}
	}
	else{
		//ERROR: Error to read coefficients from EEPROM
	}

  	return error;
}

/**
 * \brief Triggers conversion and read ADC value
 */
static Std_ReturnTypes psensor_conversion_and_read_adc(uint8_t cmd, uint32_t *adc)
{
	Std_ReturnTypes error;
	uint8_t buffer[3] = {0};

	//Send conversion command
	error = psensor_write_command(cmd);
	if(!error){
		Delay_vMs(ms8607_config.tempPressAdcDelay);
		//Send ADC reading command
		error = psensor_write_command(TP_SENSOR_READ_ADC);
		if(!error){
			error = I2C0_u8Read(TP_SENSOR_ADDR, buffer, sizeof(buffer));
			if(!error){
				*adc = ((uint32_t)buffer[0] << 16) | ((uint32_t)buffer[1] << 8) | buffer[2];
			}
			else{
				//Error: Error reading adc values
			}
		}
		else{
			//Error: Error sending adc command
		}
	}
	else{
		//Error: Conversion error
	}

	return error;
}

/**
 * \brief CRC check
 */
static Std_ReturnTypes psensor_crc_check(uint16_t *n_prom, uint8_t crc)
{
	uint8_t cnt, n_bit;
	uint16_t n_rem, crc_read;

	n_rem = 0x00;
	crc_read = n_prom[0];
	n_prom[COEFFICIENT_NUMBERS] = 0;
	n_prom[0] = (0x0FFF & (n_prom[0]));    // Clear the CRC byte

	for(cnt=0;cnt<(COEFFICIENT_NUMBERS+1)*2;cnt++){

		// Get next byte
		if(cnt%2 == 1)
			n_rem ^=  n_prom[cnt>>1] & 0x00FF;
		else
			n_rem ^=  n_prom[cnt>>1]>>8;

		for(n_bit=8;n_bit>0;n_bit--){

			if(n_rem & 0x8000)
				n_rem = (n_rem << 1) ^ 0x3000;
			else
				n_rem <<= 1;
		}
	}
	n_rem >>= 12;
	n_prom[0] = crc_read;

    if(n_rem != crc){
        return NOT_OK;
    }

	return OK;
}

/**
 * \brief Compute temperature and pressure
 */
static Std_ReturnTypes psensor_read_pressure_and_temperature(float32 *temperature, float32 *pressure)
{
	Std_ReturnTypes error = OK;
	uint32_t adc_temperature, adc_pressure;
	int32_t dT, TEMP;
	int64_t OFF, SENS, P, T2, OFF2, SENS2;
	uint8_t cmd;

	// Read coefficients
	error = psensor_read_eeprom();
	if(!error){
		// D2 (temperature)
		cmd = ms8607_config.tempPressResolution*2;
		cmd |= TP_SENSOR_START_TEMPERATURE_ADC_CONVERSION;
		error = psensor_conversion_and_read_adc(cmd, &adc_temperature);
		if(!error){
			// D1 (pressure)
			cmd = ms8607_config.tempPressResolution*2;
			cmd |= TP_SENSOR_START_PRESSURE_ADC_CONVERSION;
			error = psensor_conversion_and_read_adc(cmd, &adc_pressure);
			if(!error){
				// Validate if data is not corrupted (i2c adc read command before conversion done)
				if(adc_temperature != 0 && adc_pressure != 0){

					// Difference between actual and reference temperature. dt = D2 - Tref = D2 - C5 * (2^8)
					dT = (int32_t)adc_temperature - ((int32_t)eeprom_coeff[C5_REFERENCE_TEMPERATURE_INDEX] <<8);

					// Actual temperature. TEMP = 20°C + dT * TEMPSENS = 2000 + dT * C6 / (2^23)
					TEMP = 2000 + ((int64_t)dT * (int64_t)eeprom_coeff[C6_TEMP_COEFF_OF_TEMPERATURE_INDEX] >> 23) ;

					// Second order temperature compensation
					if(TEMP < 2000){
						T2 = (3 * ((int64_t)dT * (int64_t)dT)) >> 33;
						//OFF2 = 61 * ((int64_t)TEMP - 2000) * ((int64_t)TEMP - 2000) / 16;
						OFF2 = 61 * ((int64_t)TEMP - 2000) * ((int64_t)TEMP - 2000) / 4;
						//SENS2 = 29 * ((int64_t)TEMP - 2000) * ((int64_t)TEMP - 2000) / 16;
						SENS2 = 29 * ((int64_t)TEMP - 2000) * ((int64_t)TEMP - 2000) / 4;

						if(TEMP < -1500){
							OFF2 += 17 * ((int64_t)TEMP + 1500) * ((int64_t)TEMP + 1500);
							SENS2 += 9 * ((int64_t)TEMP + 1500) * ((int64_t)TEMP + 1500);
						}
					}
					else{
						T2 = (5 * ((int64_t)dT * (int64_t)dT)) >> 38;
						OFF2 = 0;
						SENS2 = 0;
					}

					// OFF. OFF = OFF_T1 + TCO * dT = C2 * (2^17) + (C4 * dT) / (2^6)
					OFF = ((int64_t)(eeprom_coeff[C2_PRESSURE_OFFSET_INDEX]) << 17) + (((int64_t)(eeprom_coeff[C4_TEMP_COEFF_OF_PRESSURE_OFFSET_INDEX]) * dT) >> 6);
					OFF = OFF - OFF2;

					// Sensitivity at actual temperature. SENS = SENS_T1 + TCS * dT = C1 * (2^16) + (C3 * dT) / (2^7)
					SENS = ((int64_t)eeprom_coeff[C1_PRESSURE_SENSITIVITY_INDEX] << 16) + (((int64_t)eeprom_coeff[C3_TEMP_COEFF_OF_PRESSURE_SENSITIVITY_INDEX] * dT) >> 7);
					SENS = SENS - SENS2;

					// Temperature compensated pressure. P = D1 * SENS - OFF = (D1 * SENS / (2^21) - OFF) / (2^15)
					P = (((adc_pressure * SENS) >> 21) - OFF) >> 15;

					*temperature = ((float32)TEMP - T2) / 100;
					*pressure = (float32)P / 100;
				}
				else{
					//Error: Temperature or pressure values corrupted
				}
			}
			else{
				//Error: D1 (pressure) reading error
			}
		}
		else{
			//Error: D2 (temperature) reading error
		}
	}
	else{
		//Error: Coefficients reading error
	}

	return error;
}

/**
 * \brief Configures the MS8607 sensor.
 */
static void MS8607_vConfig(void)
{
	ms8607_config.i2c_instance = INST_LPI2C0;
	ms8607_config.humidityResolution = ms8607_humidity_resolution_12b;
	ms8607_config.tempPressResolution = ms8607_pressure_resolution_osr_1024;
	ms8607_config.humidityMeasureMode = ms8607_i2c_hold;
	ms8607_config.humiditySensorBatteryStatus = ms8607_battery_ok;
	ms8607_config.heaterStatus = ms8607_heater_off;
	ms8607_config.getCompensatedHumidity = false;
}

static void MS8607_vGetADCDelay(void){
	switch(ms8607_config.tempPressResolution){
		case ms8607_pressure_resolution_osr_256:	ms8607_config.tempPressAdcDelay = TP_SENSOR_TIME_CONVERSION_ORS_256;	break;
		case ms8607_pressure_resolution_osr_512:	ms8607_config.tempPressAdcDelay = TP_SENSOR_TIME_CONVERSION_ORS_512;	break;
		case ms8607_pressure_resolution_osr_1024:	ms8607_config.tempPressAdcDelay = TP_SENSOR_TIME_CONVERSION_ORS_1024;	break;
		case ms8607_pressure_resolution_osr_2048:	ms8607_config.tempPressAdcDelay = TP_SENSOR_TIME_CONVERSION_ORS_2048;	break;
		case ms8607_pressure_resolution_osr_4096:	ms8607_config.tempPressAdcDelay = TP_SENSOR_TIME_CONVERSION_ORS_4096;	break;
		case ms8607_pressure_resolution_osr_8192:	ms8607_config.tempPressAdcDelay = TP_SENSOR_TIME_CONVERSION_ORS_8192;	break;
	}
}

/*******************************************************************************
 * Functions
 ******************************************************************************/
/**
 * \brief Configures the SERCOM I2C master to be used with the ms8607 device.
 */
void ms8607_vInit(void)
{	
	I2C_vInit();
	MS8607_vConfig();
	MS8607_vGetADCDelay();
}

/*
 * \brief Check if sensor respond an ack
 */
Std_ReturnTypes ms8607_is_connected(void)
{
	return I2C0_u8Write(TP_SENSOR_ADDR, 1U, 0U);
}

/**
 * \brief Reset the MS8607 device
 */
Std_ReturnTypes ms8607_reset(void)
{
	Std_ReturnTypes error;
	
	error = hsensor_reset();
	if(!error){
		error = psensor_reset();
	}
	else{
		//Do nothing
	}

	return error;
}

/**
 * \brief Set humidity ADC resolution.
 */
Std_ReturnTypes ms8607_set_humidity_resolution(enum ms8607_humidity_resolution resolution)
{
	Std_ReturnTypes error;
	uint8_t data, tmp=0;
	
	switch(resolution){
		case ms8607_humidity_resolution_12b:		tmp = H_SENSOR_USER_REG_RESOLUTION_12b;		break;
		case ms8607_humidity_resolution_8b:			tmp = H_SENSOR_USER_REG_RESOLUTION_8b;		break;
		case ms8607_humidity_resolution_10b:		tmp = H_SENSOR_USER_REG_RESOLUTION_10b;		break;
		case ms8607_humidity_resolution_11b:		tmp = H_SENSOR_USER_REG_RESOLUTION_11b;		break;
	}

	error = hsensor_read_user_register(&data, sizeof(data));
	if(!error){
		data &= H_SENSOR_USER_REG_DATA_MASK;
		data |= tmp;

		error = hsensor_write_user_register(data);
	}
	else{
		//Do nothing
	}

	return error;
}

/**
 * \brief Set Humidity sensor ADC resolution.
 */
void ms8607_set_humidity_i2c_master_mode(enum ms8607_humidity_i2c_master_mode mode)
{
	ms8607_config.humidityMeasureMode = mode;
}

/**
 * \brief Reads the temperature, pressure and relative humidity value.
 */
Std_ReturnTypes ms8607_read_temperature_pressure_humidity(float32 *t, float32 *p, float32 *h)
{
	Std_ReturnTypes error;
	
	error = psensor_read_pressure_and_temperature(t,p);
	if(!error){
		error = hsensor_read_relative_humidity(h);
		if(ms8607_config.getCompensatedHumidity){
			ms8607_get_compensated_humidity(*t, *h, h);
		}
	}
	else{
		//Do nothing
	}
		
	return error;
}

/**
 * \brief Provide battery status
 */
Std_ReturnTypes ms8607_get_humidity_battery_status(uint8_t *battStatus)
{
	Std_ReturnTypes error;
	uint8_t reg_value;

	error = hsensor_read_user_register(&reg_value, sizeof(reg_value));
	if(!error){
		reg_value &= H_SENSOR_USER_REG_BATTERY_STATUS_MASK;
		if(reg_value & H_SENSOR_USER_REG_BATTERY_STATUS_BELOW_2_25V)
			*battStatus = ms8607_battery_low;
		else
			*battStatus = ms8607_battery_ok;
	}
	else{
		//Do nothing
	}
		
	return error;
}

/**
 * \brief Enable heater
 */
Std_ReturnTypes ms8607_enable_humidity_heater(void)
{
	Std_ReturnTypes error;
	uint8_t reg_value;
	
	error = hsensor_read_user_register(&reg_value, sizeof(reg_value));
	if(!error){
		reg_value &= H_SENSOR_USER_REG_NOT_HEATER_MASK;
		reg_value |= H_SENSOR_USER_REG_ONCHIP_HEATER_ENABLE;

		error = hsensor_write_user_register(reg_value);
	}
	else{
		//Do nothing
	}

	return error;
}

/**
 * \brief Disable heater
 */
Std_ReturnTypes ms8607_disable_humidity_heater(void)
{
	Std_ReturnTypes error;
	uint8_t reg_value;
	
	error = hsensor_read_user_register(&reg_value, sizeof(reg_value));
	if(!error){
		reg_value &= H_SENSOR_USER_REG_NOT_HEATER_MASK;
		reg_value |= H_SENSOR_USER_REG_ONCHIP_HEATER_DISABLE;

		error = hsensor_write_user_register(reg_value);
	}else{
		//Do nothing
	}

	return error;
}

/**
 * \brief Get heater status
 */
Std_ReturnTypes ms8607_get_heater_status(uint8_t *heaterStatus)
{
	Std_ReturnTypes error;
	uint8_t reg_value;
	
	error = hsensor_read_user_register(&reg_value, sizeof(reg_value));
	if(!error){
		reg_value &= H_SENSOR_USER_REG_HEATER_MASK;
		if(reg_value & H_SENSOR_USER_REG_ONCHIP_HEATER_ENABLE)
			*heaterStatus = ms8607_heater_on;
		else
			*heaterStatus = ms8607_heater_off;
	}
	else{
		//Do nothing
	}
	
	return error;
}

/**
 * \brief Returns result of compensated humidity
 *        Note : This function shall only be used when the heater is OFF. It will return an error otherwise.
 */
void ms8607_get_compensated_humidity(float32 temperature, float32 relative_humidity, float32 *compensated_humidity)
{
	if(temperature >= 0 && temperature <= 85){
		*compensated_humidity = (relative_humidity + (25 - temperature) * H_SENSOR_TEMPERATURE_COEFFICIENT);
	}
	else{
		*compensated_humidity = relative_humidity;
	}
}

/**
 * \brief Set pressure ADC resolution.
 */
void ms8607_set_pressure_resolution(enum ms8607_pressure_resolution resolution)
{
	ms8607_config.tempPressResolution = resolution;
	switch(resolution){
		case ms8607_pressure_resolution_osr_256:	ms8607_config.tempPressAdcDelay = TP_SENSOR_TIME_CONVERSION_ORS_256;	break;
		case ms8607_pressure_resolution_osr_512:	ms8607_config.tempPressAdcDelay = TP_SENSOR_TIME_CONVERSION_ORS_512;	break;
		case ms8607_pressure_resolution_osr_1024:	ms8607_config.tempPressAdcDelay = TP_SENSOR_TIME_CONVERSION_ORS_1024;	break;
		case ms8607_pressure_resolution_osr_2048:	ms8607_config.tempPressAdcDelay = TP_SENSOR_TIME_CONVERSION_ORS_2048;	break;
		case ms8607_pressure_resolution_osr_4096:	ms8607_config.tempPressAdcDelay = TP_SENSOR_TIME_CONVERSION_ORS_4096;	break;
		case ms8607_pressure_resolution_osr_8192:	ms8607_config.tempPressAdcDelay = TP_SENSOR_TIME_CONVERSION_ORS_8192;	break;
	}
}
