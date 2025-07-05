/*!
 * @file Adafruit_BQ25798.cpp
 *
 * @mainpage Adafruit BQ25798 I2C Controlled Buck-Boost Battery Charger
 *
 * @section intro_sec Introduction
 *
 * This is a library for the Adafruit BQ25798 I2C controlled buck-boost battery charger
 *
 * @section dependencies Dependencies
 *
 * This library depends on <a href="https://github.com/adafruit/Adafruit_BusIO">
 * Adafruit_BusIO</a> being present on your system. Please make sure you have
 * installed the latest version before using this library.
 *
 * @section author Author
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 * @section license License
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#include "Adafruit_BQ25798.h"

/*!
 * @brief  Instantiates a new BQ25798 class
 */
Adafruit_BQ25798::Adafruit_BQ25798() {
  i2c_dev = NULL;
}

/*!
 * @brief  Destroys the BQ25798 object
 */
Adafruit_BQ25798::~Adafruit_BQ25798() {
  if (i2c_dev) {
    delete i2c_dev;
  }
}

/*!
 * @brief  Sets up the hardware and initializes I2C
 * @param  i2c_addr
 *         The I2C address to be used.
 * @param  wire
 *         The Wire object to be used for I2C connections.
 * @return True if initialization was successful, otherwise false.
 */
bool Adafruit_BQ25798::begin(uint8_t i2c_addr, TwoWire *wire) {
  if (i2c_dev) {
    delete i2c_dev;
  }

  i2c_dev = new Adafruit_I2CDevice(i2c_addr, wire);

  if (!i2c_dev->begin()) {
    return false;
  }

  // Check part information register to verify chip
  Adafruit_BusIO_Register part_info_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_PART_INFORMATION);
  uint8_t part_info = part_info_reg.read();
  
  // Verify part number (bits 5-3 should be 011b = 3h for BQ25798)
  if ((part_info & 0x38) != 0x18) {
    return false;
  }

  return true;
}

/*!
 * @brief Get the minimal system voltage setting
 * @return Minimal system voltage in volts
 */
float Adafruit_BQ25798::getMinSystemV() {
  Adafruit_BusIO_Register vsys_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_MINIMAL_SYSTEM_VOLTAGE);
  Adafruit_BusIO_RegisterBits vsys_bits = Adafruit_BusIO_RegisterBits(&vsys_reg, 6, 0);
  
  uint8_t reg_value = vsys_bits.read();
  
  // Convert to voltage: (register_value × 250mV) + 2500mV
  return (reg_value * 0.25f) + 2.5f;
}

/*!
 * @brief Set the minimal system voltage
 * @param voltage Minimal system voltage in volts (2.5V to 16.0V)
 * @return True if successful, false if voltage out of range
 */
bool Adafruit_BQ25798::setMinSystemV(float voltage) {
  if (voltage < 2.5f || voltage > 16.0f) {
    return false;
  }
  
  // Convert voltage to register value: (voltage - 2.5V) / 0.25V
  uint8_t reg_value = (uint8_t)((voltage - 2.5f) / 0.25f);
  
  // Clamp to 6-bit range (0-63)
  if (reg_value > 63) {
    reg_value = 63;
  }
  
  Adafruit_BusIO_Register vsys_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_MINIMAL_SYSTEM_VOLTAGE);
  Adafruit_BusIO_RegisterBits vsys_bits = Adafruit_BusIO_RegisterBits(&vsys_reg, 6, 0);
  
  vsys_bits.write(reg_value);
  
  return true;
}

/*!
 * @brief Get the charge voltage limit setting
 * @return Charge voltage limit in volts
 */
float Adafruit_BQ25798::getChargeLimitV() {
  Adafruit_BusIO_Register vreg_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGE_VOLTAGE_LIMIT, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits vreg_bits = Adafruit_BusIO_RegisterBits(&vreg_reg, 11, 0);
  
  uint16_t reg_value = vreg_bits.read();
  
  // Convert to voltage: register_value × 10mV
  return reg_value * 0.01f;
}

/*!
 * @brief Set the charge voltage limit
 * @param voltage Charge voltage limit in volts (3.0V to 18.8V)
 * @return True if successful, false if voltage out of range
 */
bool Adafruit_BQ25798::setChargeLimitV(float voltage) {
  if (voltage < 3.0f || voltage > 18.8f) {
    return false;
  }
  
  // Convert voltage to register value: voltage / 0.01V
  uint16_t reg_value = (uint16_t)(voltage / 0.01f);
  
  // Clamp to 11-bit range (0-2047)
  if (reg_value > 2047) {
    reg_value = 2047;
  }
  
  Adafruit_BusIO_Register vreg_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGE_VOLTAGE_LIMIT, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits vreg_bits = Adafruit_BusIO_RegisterBits(&vreg_reg, 11, 0);
  
  vreg_bits.write(reg_value);
  
  return true;
}

/*!
 * @brief Get the charge current limit setting
 * @return Charge current limit in amps
 */
float Adafruit_BQ25798::getChargeLimitA() {
  Adafruit_BusIO_Register ichg_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGE_CURRENT_LIMIT, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits ichg_bits = Adafruit_BusIO_RegisterBits(&ichg_reg, 9, 0);
  
  uint16_t reg_value = ichg_bits.read();
  
  // Convert to current: register_value × 10mA
  return reg_value * 0.01f;
}

/*!
 * @brief Set the charge current limit
 * @param current Charge current limit in amps (0.05A to 5.0A)
 * @return True if successful, false if current out of range
 */
bool Adafruit_BQ25798::setChargeLimitA(float current) {
  if (current < 0.05f || current > 5.0f) {
    return false;
  }
  
  // Convert current to register value: current / 0.01A
  uint16_t reg_value = (uint16_t)(current / 0.01f);
  
  // Clamp to 9-bit range (0-511)
  if (reg_value > 511) {
    reg_value = 511;
  }
  
  Adafruit_BusIO_Register ichg_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGE_CURRENT_LIMIT, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits ichg_bits = Adafruit_BusIO_RegisterBits(&ichg_reg, 9, 0);
  
  ichg_bits.write(reg_value);
  
  return true;
}

/*!
 * @brief Get the input voltage limit setting
 * @return Input voltage limit in volts
 */
float Adafruit_BQ25798::getInputLimitV() {
  Adafruit_BusIO_Register vindpm_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_INPUT_VOLTAGE_LIMIT);
  
  uint8_t reg_value = vindpm_reg.read();
  
  // Convert to voltage: register_value × 100mV
  return reg_value * 0.1f;
}

/*!
 * @brief Set the input voltage limit
 * @param voltage Input voltage limit in volts (3.6V to 22.0V)
 * @return True if successful, false if voltage out of range
 */
bool Adafruit_BQ25798::setInputLimitV(float voltage) {
  if (voltage < 3.6f || voltage > 22.0f) {
    return false;
  }
  
  // Convert voltage to register value: voltage / 0.1V
  uint8_t reg_value = (uint8_t)(voltage / 0.1f);
  
  // Clamp to 8-bit range (0-255)
  if (reg_value > 255) {
    reg_value = 255;
  }
  
  Adafruit_BusIO_Register vindpm_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_INPUT_VOLTAGE_LIMIT);
  
  vindpm_reg.write(reg_value);
  
  return true;
}

/*!
 * @brief Get the input current limit setting
 * @return Input current limit in amps
 */
float Adafruit_BQ25798::getInputLimitA() {
  Adafruit_BusIO_Register iindpm_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_INPUT_CURRENT_LIMIT, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits iindpm_bits = Adafruit_BusIO_RegisterBits(&iindpm_reg, 9, 0);
  
  uint16_t reg_value = iindpm_bits.read();
  
  // Convert to current: register_value × 10mA
  return reg_value * 0.01f;
}

/*!
 * @brief Set the input current limit
 * @param current Input current limit in amps (0.1A to 3.3A)
 * @return True if successful, false if current out of range
 */
bool Adafruit_BQ25798::setInputLimitA(float current) {
  if (current < 0.1f || current > 3.3f) {
    return false;
  }
  
  // Convert current to register value: current / 0.01A
  uint16_t reg_value = (uint16_t)(current / 0.01f);
  
  // Clamp to 9-bit range (0-511)
  if (reg_value > 511) {
    reg_value = 511;
  }
  
  Adafruit_BusIO_Register iindpm_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_INPUT_CURRENT_LIMIT, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits iindpm_bits = Adafruit_BusIO_RegisterBits(&iindpm_reg, 9, 0);
  
  iindpm_bits.write(reg_value);
  
  return true;
}