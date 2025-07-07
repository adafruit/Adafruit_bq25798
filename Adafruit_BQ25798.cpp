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

  // Reset all registers to default values
  reset();

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

/*!
 * @brief Get the battery voltage threshold for precharge to fast charge transition
 * @return Battery voltage threshold as percentage of VREG
 */
bq25798_vbat_lowv_t Adafruit_BQ25798::getVBatLowV() {
  Adafruit_BusIO_Register precharge_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_PRECHARGE_CONTROL);
  Adafruit_BusIO_RegisterBits vbat_lowv_bits = Adafruit_BusIO_RegisterBits(&precharge_reg, 2, 6);
  
  uint8_t reg_value = vbat_lowv_bits.read();
  
  return (bq25798_vbat_lowv_t)reg_value;
}

/*!
 * @brief Set the battery voltage threshold for precharge to fast charge transition
 * @param threshold Battery voltage threshold as percentage of VREG
 * @return True if successful
 */
bool Adafruit_BQ25798::setVBatLowV(bq25798_vbat_lowv_t threshold) {
  if (threshold > BQ25798_VBAT_LOWV_71_4_PERCENT) {
    return false;
  }
  
  Adafruit_BusIO_Register precharge_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_PRECHARGE_CONTROL);
  Adafruit_BusIO_RegisterBits vbat_lowv_bits = Adafruit_BusIO_RegisterBits(&precharge_reg, 2, 6);
  
  vbat_lowv_bits.write((uint8_t)threshold);
  
  return true;
}

/*!
 * @brief Get the precharge current limit setting
 * @return Precharge current limit in amps
 */
float Adafruit_BQ25798::getPrechargeLimitA() {
  Adafruit_BusIO_Register precharge_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_PRECHARGE_CONTROL);
  Adafruit_BusIO_RegisterBits iprechg_bits = Adafruit_BusIO_RegisterBits(&precharge_reg, 6, 0);
  
  uint8_t reg_value = iprechg_bits.read();
  
  // Convert to current: register_value × 40mA
  return reg_value * 0.04f;
}

/*!
 * @brief Set the precharge current limit
 * @param current Precharge current limit in amps (0.04A to 2.0A)
 * @return True if successful, false if current out of range
 */
bool Adafruit_BQ25798::setPrechargeLimitA(float current) {
  if (current < 0.04f || current > 2.0f) {
    return false;
  }
  
  // Convert current to register value: current / 0.04A
  uint8_t reg_value = (uint8_t)(current / 0.04f);
  
  // Clamp to 6-bit range (0-63)
  if (reg_value > 63) {
    reg_value = 63;
  }
  
  Adafruit_BusIO_Register precharge_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_PRECHARGE_CONTROL);
  Adafruit_BusIO_RegisterBits iprechg_bits = Adafruit_BusIO_RegisterBits(&precharge_reg, 6, 0);
  
  iprechg_bits.write(reg_value);
  
  return true;
}

/*!
 * @brief Get the watchdog timer reset behavior for safety timers
 * @return True if watchdog expiration will NOT reset safety timers, false if it will reset them
 */
bool Adafruit_BQ25798::getStopOnWDT() {
  Adafruit_BusIO_Register term_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TERMINATION_CONTROL);
  Adafruit_BusIO_RegisterBits stop_wd_bit = Adafruit_BusIO_RegisterBits(&term_reg, 1, 5);
  
  return stop_wd_bit.read() == 1;
}

/*!
 * @brief Set the watchdog timer reset behavior for safety timers
 * @param stopOnWDT True = watchdog expiration will NOT reset safety timers, false = will reset them
 * @return True if successful
 */
bool Adafruit_BQ25798::setStopOnWDT(bool stopOnWDT) {
  Adafruit_BusIO_Register term_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TERMINATION_CONTROL);
  Adafruit_BusIO_RegisterBits stop_wd_bit = Adafruit_BusIO_RegisterBits(&term_reg, 1, 5);
  
  stop_wd_bit.write(stopOnWDT ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the termination current limit setting
 * @return Termination current limit in amps
 */
float Adafruit_BQ25798::getTerminationA() {
  Adafruit_BusIO_Register term_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TERMINATION_CONTROL);
  Adafruit_BusIO_RegisterBits iterm_bits = Adafruit_BusIO_RegisterBits(&term_reg, 5, 0);
  
  uint8_t reg_value = iterm_bits.read();
  
  // Convert to current: register_value × 40mA
  return reg_value * 0.04f;
}

/*!
 * @brief Set the termination current limit
 * @param current Termination current limit in amps (0.04A to 1.0A)
 * @return True if successful, false if current out of range
 */
bool Adafruit_BQ25798::setTerminationA(float current) {
  if (current < 0.04f || current > 1.0f) {
    return false;
  }
  
  // Convert current to register value: current / 0.04A
  uint8_t reg_value = (uint8_t)(current / 0.04f);
  
  // Clamp to 5-bit range (0-31)
  if (reg_value > 31) {
    reg_value = 31;
  }
  
  Adafruit_BusIO_Register term_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TERMINATION_CONTROL);
  Adafruit_BusIO_RegisterBits iterm_bits = Adafruit_BusIO_RegisterBits(&term_reg, 5, 0);
  
  iterm_bits.write(reg_value);
  
  return true;
}

/*!
 * @brief Get the battery cell count setting
 * @return Battery cell count
 */
bq25798_cell_count_t Adafruit_BQ25798::getCellCount() {
  Adafruit_BusIO_Register recharge_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_RECHARGE_CONTROL);
  Adafruit_BusIO_RegisterBits cell_bits = Adafruit_BusIO_RegisterBits(&recharge_reg, 2, 6);
  
  uint8_t reg_value = cell_bits.read();
  
  return (bq25798_cell_count_t)reg_value;
}

/*!
 * @brief Set the battery cell count
 * @param cellCount Battery cell count (1S to 4S)
 * @return True if successful
 */
bool Adafruit_BQ25798::setCellCount(bq25798_cell_count_t cellCount) {
  if (cellCount > BQ25798_CELL_COUNT_4S) {
    return false;
  }
  
  Adafruit_BusIO_Register recharge_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_RECHARGE_CONTROL);
  Adafruit_BusIO_RegisterBits cell_bits = Adafruit_BusIO_RegisterBits(&recharge_reg, 2, 6);
  
  cell_bits.write((uint8_t)cellCount);
  
  return true;
}

/*!
 * @brief Get the battery recharge deglitch time setting
 * @return Battery recharge deglitch time
 */
bq25798_trechg_time_t Adafruit_BQ25798::getRechargeDeglitchTime() {
  Adafruit_BusIO_Register recharge_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_RECHARGE_CONTROL);
  Adafruit_BusIO_RegisterBits trechg_bits = Adafruit_BusIO_RegisterBits(&recharge_reg, 2, 4);
  
  uint8_t reg_value = trechg_bits.read();
  
  return (bq25798_trechg_time_t)reg_value;
}

/*!
 * @brief Set the battery recharge deglitch time
 * @param deglitchTime Battery recharge deglitch time (64ms to 2048ms)
 * @return True if successful
 */
bool Adafruit_BQ25798::setRechargeDeglitchTime(bq25798_trechg_time_t deglitchTime) {
  if (deglitchTime > BQ25798_TRECHG_2048MS) {
    return false;
  }
  
  Adafruit_BusIO_Register recharge_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_RECHARGE_CONTROL);
  Adafruit_BusIO_RegisterBits trechg_bits = Adafruit_BusIO_RegisterBits(&recharge_reg, 2, 4);
  
  trechg_bits.write((uint8_t)deglitchTime);
  
  return true;
}

/*!
 * @brief Get the battery recharge threshold offset voltage
 * @return Recharge threshold offset voltage in volts (below VREG)
 */
float Adafruit_BQ25798::getRechargeThreshOffsetV() {
  Adafruit_BusIO_Register recharge_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_RECHARGE_CONTROL);
  Adafruit_BusIO_RegisterBits vrechg_bits = Adafruit_BusIO_RegisterBits(&recharge_reg, 4, 0);
  
  uint8_t reg_value = vrechg_bits.read();
  
  // Convert to voltage: (register_value × 50mV) + 50mV
  return (reg_value * 0.05f) + 0.05f;
}

/*!
 * @brief Set the battery recharge threshold offset voltage
 * @param voltage Recharge threshold offset voltage in volts (0.05V to 0.8V)
 * @return True if successful, false if voltage out of range
 */
bool Adafruit_BQ25798::setRechargeThreshOffsetV(float voltage) {
  if (voltage < 0.05f || voltage > 0.8f) {
    return false;
  }
  
  // Convert voltage to register value: (voltage - 0.05V) / 0.05V
  uint8_t reg_value = (uint8_t)((voltage - 0.05f) / 0.05f);
  
  // Clamp to 4-bit range (0-15)
  if (reg_value > 15) {
    reg_value = 15;
  }
  
  Adafruit_BusIO_Register recharge_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_RECHARGE_CONTROL);
  Adafruit_BusIO_RegisterBits vrechg_bits = Adafruit_BusIO_RegisterBits(&recharge_reg, 4, 0);
  
  vrechg_bits.write(reg_value);
  
  return true;
}

/*!
 * @brief Get the OTG mode regulation voltage setting
 * @return OTG voltage in volts
 */
float Adafruit_BQ25798::getOTGV() {
  Adafruit_BusIO_Register votg_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_VOTG_REGULATION, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits votg_bits = Adafruit_BusIO_RegisterBits(&votg_reg, 11, 0);
  
  uint16_t reg_value = votg_bits.read();
  
  // Convert to voltage: (register_value × 10mV) + 2800mV
  return (reg_value * 0.01f) + 2.8f;
}

/*!
 * @brief Set the OTG mode regulation voltage
 * @param voltage OTG voltage in volts (2.8V to 22.0V)
 * @return True if successful, false if voltage out of range
 */
bool Adafruit_BQ25798::setOTGV(float voltage) {
  if (voltage < 2.8f || voltage > 22.0f) {
    return false;
  }
  
  // Convert voltage to register value: (voltage - 2.8V) / 0.01V
  uint16_t reg_value = (uint16_t)((voltage - 2.8f) / 0.01f);
  
  // Clamp to 11-bit range (0-2047)
  if (reg_value > 2047) {
    reg_value = 2047;
  }
  
  Adafruit_BusIO_Register votg_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_VOTG_REGULATION, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits votg_bits = Adafruit_BusIO_RegisterBits(&votg_reg, 11, 0);
  
  votg_bits.write(reg_value);
  
  return true;
}

/*!
 * @brief Get the precharge safety timer setting
 * @return Precharge timer setting
 */
bq25798_prechg_timer_t Adafruit_BQ25798::getPrechargeTimer() {
  Adafruit_BusIO_Register iotg_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_IOTG_REGULATION);
  Adafruit_BusIO_RegisterBits prechg_tmr_bit = Adafruit_BusIO_RegisterBits(&iotg_reg, 1, 7);
  
  uint8_t reg_value = prechg_tmr_bit.read();
  
  return (bq25798_prechg_timer_t)reg_value;
}

/*!
 * @brief Set the precharge safety timer
 * @param timer Precharge timer setting (0.5hr or 2hr)
 * @return True if successful
 */
bool Adafruit_BQ25798::setPrechargeTimer(bq25798_prechg_timer_t timer) {
  if (timer > BQ25798_PRECHG_TMR_0_5HR) {
    return false;
  }
  
  Adafruit_BusIO_Register iotg_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_IOTG_REGULATION);
  Adafruit_BusIO_RegisterBits prechg_tmr_bit = Adafruit_BusIO_RegisterBits(&iotg_reg, 1, 7);
  
  prechg_tmr_bit.write((uint8_t)timer);
  
  return true;
}

/*!
 * @brief Get the OTG current limit setting
 * @return OTG current limit in amps
 */
float Adafruit_BQ25798::getOTGLimitA() {
  Adafruit_BusIO_Register iotg_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_IOTG_REGULATION);
  Adafruit_BusIO_RegisterBits iotg_bits = Adafruit_BusIO_RegisterBits(&iotg_reg, 7, 0);
  
  uint8_t reg_value = iotg_bits.read();
  
  // Convert to current: register_value × 40mA
  return reg_value * 0.04f;
}

/*!
 * @brief Set the OTG current limit
 * @param current OTG current limit in amps (0.16A to 3.36A)
 * @return True if successful, false if current out of range
 */
bool Adafruit_BQ25798::setOTGLimitA(float current) {
  if (current < 0.16f || current > 3.36f) {
    return false;
  }
  
  // Convert current to register value: current / 0.04A
  uint8_t reg_value = (uint8_t)(current / 0.04f);
  
  // Clamp to 7-bit range (0-127)
  if (reg_value > 127) {
    reg_value = 127;
  }
  
  Adafruit_BusIO_Register iotg_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_IOTG_REGULATION);
  Adafruit_BusIO_RegisterBits iotg_bits = Adafruit_BusIO_RegisterBits(&iotg_reg, 7, 0);
  
  iotg_bits.write(reg_value);
  
  return true;
}

/*!
 * @brief Get the top-off timer setting
 * @return Top-off timer setting
 */
bq25798_topoff_timer_t Adafruit_BQ25798::getTopOffTimer() {
  Adafruit_BusIO_Register timer_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TIMER_CONTROL);
  Adafruit_BusIO_RegisterBits topoff_bits = Adafruit_BusIO_RegisterBits(&timer_reg, 2, 6);
  
  uint8_t reg_value = topoff_bits.read();
  
  return (bq25798_topoff_timer_t)reg_value;
}

/*!
 * @brief Set the top-off timer
 * @param timer Top-off timer setting (disabled, 15min, 30min, 45min)
 * @return True if successful
 */
bool Adafruit_BQ25798::setTopOffTimer(bq25798_topoff_timer_t timer) {
  if (timer > BQ25798_TOPOFF_TMR_45MIN) {
    return false;
  }
  
  Adafruit_BusIO_Register timer_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TIMER_CONTROL);
  Adafruit_BusIO_RegisterBits topoff_bits = Adafruit_BusIO_RegisterBits(&timer_reg, 2, 6);
  
  topoff_bits.write((uint8_t)timer);
  
  return true;
}

/*!
 * @brief Get the trickle charge timer enable setting
 * @return True if trickle charge timer is enabled, false if disabled
 */
bool Adafruit_BQ25798::getTrickleChargeTimerEnable() {
  Adafruit_BusIO_Register timer_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TIMER_CONTROL);
  Adafruit_BusIO_RegisterBits trickle_bit = Adafruit_BusIO_RegisterBits(&timer_reg, 1, 5);
  
  return trickle_bit.read() == 1;
}

/*!
 * @brief Set the trickle charge timer enable
 * @param enable True = enable trickle charge timer, false = disable
 * @return True if successful
 */
bool Adafruit_BQ25798::setTrickleChargeTimerEnable(bool enable) {
  Adafruit_BusIO_Register timer_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TIMER_CONTROL);
  Adafruit_BusIO_RegisterBits trickle_bit = Adafruit_BusIO_RegisterBits(&timer_reg, 1, 5);
  
  trickle_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the precharge timer enable setting
 * @return True if precharge timer is enabled, false if disabled
 */
bool Adafruit_BQ25798::getPrechargeTimerEnable() {
  Adafruit_BusIO_Register timer_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TIMER_CONTROL);
  Adafruit_BusIO_RegisterBits prechg_bit = Adafruit_BusIO_RegisterBits(&timer_reg, 1, 4);
  
  return prechg_bit.read() == 1;
}

/*!
 * @brief Set the precharge timer enable
 * @param enable True = enable precharge timer, false = disable
 * @return True if successful
 */
bool Adafruit_BQ25798::setPrechargeTimerEnable(bool enable) {
  Adafruit_BusIO_Register timer_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TIMER_CONTROL);
  Adafruit_BusIO_RegisterBits prechg_bit = Adafruit_BusIO_RegisterBits(&timer_reg, 1, 4);
  
  prechg_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the fast charge timer enable setting
 * @return True if fast charge timer is enabled, false if disabled
 */
bool Adafruit_BQ25798::getFastChargeTimerEnable() {
  Adafruit_BusIO_Register timer_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TIMER_CONTROL);
  Adafruit_BusIO_RegisterBits chg_bit = Adafruit_BusIO_RegisterBits(&timer_reg, 1, 3);
  
  return chg_bit.read() == 1;
}

/*!
 * @brief Set the fast charge timer enable
 * @param enable True = enable fast charge timer, false = disable
 * @return True if successful
 */
bool Adafruit_BQ25798::setFastChargeTimerEnable(bool enable) {
  Adafruit_BusIO_Register timer_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TIMER_CONTROL);
  Adafruit_BusIO_RegisterBits chg_bit = Adafruit_BusIO_RegisterBits(&timer_reg, 1, 3);
  
  chg_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the fast charge timer setting
 * @return Fast charge timer setting
 */
bq25798_chg_timer_t Adafruit_BQ25798::getFastChargeTimer() {
  Adafruit_BusIO_Register timer_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TIMER_CONTROL);
  Adafruit_BusIO_RegisterBits chg_tmr_bits = Adafruit_BusIO_RegisterBits(&timer_reg, 2, 1);
  
  uint8_t reg_value = chg_tmr_bits.read();
  
  return (bq25798_chg_timer_t)reg_value;
}

/*!
 * @brief Set the fast charge timer
 * @param timer Fast charge timer setting (5hr, 8hr, 12hr, 24hr)
 * @return True if successful
 */
bool Adafruit_BQ25798::setFastChargeTimer(bq25798_chg_timer_t timer) {
  if (timer > BQ25798_CHG_TMR_24HR) {
    return false;
  }
  
  Adafruit_BusIO_Register timer_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TIMER_CONTROL);
  Adafruit_BusIO_RegisterBits chg_tmr_bits = Adafruit_BusIO_RegisterBits(&timer_reg, 2, 1);
  
  chg_tmr_bits.write((uint8_t)timer);
  
  return true;
}

/*!
 * @brief Get the timer half-rate enable setting
 * @return True if timer half-rate is enabled, false if disabled
 */
bool Adafruit_BQ25798::getTimerHalfRateEnable() {
  Adafruit_BusIO_Register timer_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TIMER_CONTROL);
  Adafruit_BusIO_RegisterBits tmr2x_bit = Adafruit_BusIO_RegisterBits(&timer_reg, 1, 0);
  
  return tmr2x_bit.read() == 1;
}

/*!
 * @brief Set the timer half-rate enable
 * @param enable True = enable timer half-rate during regulation, false = disable
 * @return True if successful
 */
bool Adafruit_BQ25798::setTimerHalfRateEnable(bool enable) {
  Adafruit_BusIO_Register timer_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TIMER_CONTROL);
  Adafruit_BusIO_RegisterBits tmr2x_bit = Adafruit_BusIO_RegisterBits(&timer_reg, 1, 0);
  
  tmr2x_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the automatic OVP battery discharge enable setting
 * @return True if automatic OVP battery discharge is enabled, false if disabled
 */
bool Adafruit_BQ25798::getAutoOVPBattDischarge() {
  Adafruit_BusIO_Register chg_ctrl0_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_0);
  Adafruit_BusIO_RegisterBits auto_ibatdis_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl0_reg, 1, 7);
  
  return auto_ibatdis_bit.read() == 1;
}

/*!
 * @brief Set the automatic OVP battery discharge enable
 * @param enable True = enable automatic OVP battery discharge, false = disable
 * @return True if successful
 */
bool Adafruit_BQ25798::setAutoOVPBattDischarge(bool enable) {
  Adafruit_BusIO_Register chg_ctrl0_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_0);
  Adafruit_BusIO_RegisterBits auto_ibatdis_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl0_reg, 1, 7);
  
  auto_ibatdis_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the force battery discharge setting
 * @return True if force battery discharge is enabled, false if disabled
 */
bool Adafruit_BQ25798::getForceBattDischarge() {
  Adafruit_BusIO_Register chg_ctrl0_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_0);
  Adafruit_BusIO_RegisterBits force_ibatdis_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl0_reg, 1, 6);
  
  return force_ibatdis_bit.read() == 1;
}

/*!
 * @brief Set the force battery discharge
 * @param enable True = force battery discharge, false = disable
 * @return True if successful
 */
bool Adafruit_BQ25798::setForceBattDischarge(bool enable) {
  Adafruit_BusIO_Register chg_ctrl0_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_0);
  Adafruit_BusIO_RegisterBits force_ibatdis_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl0_reg, 1, 6);
  
  force_ibatdis_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the charge enable setting
 * @return True if charging is enabled, false if disabled
 */
bool Adafruit_BQ25798::getChargeEnable() {
  Adafruit_BusIO_Register chg_ctrl0_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_0);
  Adafruit_BusIO_RegisterBits en_chg_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl0_reg, 1, 5);
  
  return en_chg_bit.read() == 1;
}

/*!
 * @brief Set the charge enable
 * @param enable True = enable charging, false = disable charging
 * @return True if successful
 */
bool Adafruit_BQ25798::setChargeEnable(bool enable) {
  Adafruit_BusIO_Register chg_ctrl0_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_0);
  Adafruit_BusIO_RegisterBits en_chg_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl0_reg, 1, 5);
  
  en_chg_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the ICO (Input Current Optimizer) enable setting
 * @return True if ICO is enabled, false if disabled
 */
bool Adafruit_BQ25798::getICOEnable() {
  Adafruit_BusIO_Register chg_ctrl0_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_0);
  Adafruit_BusIO_RegisterBits en_ico_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl0_reg, 1, 4);
  
  return en_ico_bit.read() == 1;
}

/*!
 * @brief Set the ICO (Input Current Optimizer) enable
 * @param enable True = enable ICO, false = disable ICO
 * @return True if successful
 */
bool Adafruit_BQ25798::setICOEnable(bool enable) {
  Adafruit_BusIO_Register chg_ctrl0_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_0);
  Adafruit_BusIO_RegisterBits en_ico_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl0_reg, 1, 4);
  
  en_ico_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the force ICO setting
 * @return True if force ICO is enabled, false if disabled
 */
bool Adafruit_BQ25798::getForceICO() {
  Adafruit_BusIO_Register chg_ctrl0_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_0);
  Adafruit_BusIO_RegisterBits force_ico_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl0_reg, 1, 3);
  
  return force_ico_bit.read() == 1;
}

/*!
 * @brief Set the force ICO
 * @param enable True = force ICO, false = disable
 * @return True if successful
 */
bool Adafruit_BQ25798::setForceICO(bool enable) {
  Adafruit_BusIO_Register chg_ctrl0_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_0);
  Adafruit_BusIO_RegisterBits force_ico_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl0_reg, 1, 3);
  
  force_ico_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the HIZ (High Impedance) mode setting
 * @return True if HIZ mode is enabled, false if disabled
 */
bool Adafruit_BQ25798::getHIZMode() {
  Adafruit_BusIO_Register chg_ctrl0_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_0);
  Adafruit_BusIO_RegisterBits en_hiz_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl0_reg, 1, 2);
  
  return en_hiz_bit.read() == 1;
}

/*!
 * @brief Set the HIZ (High Impedance) mode
 * @param enable True = enable HIZ mode, false = disable HIZ mode
 * @return True if successful
 */
bool Adafruit_BQ25798::setHIZMode(bool enable) {
  Adafruit_BusIO_Register chg_ctrl0_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_0);
  Adafruit_BusIO_RegisterBits en_hiz_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl0_reg, 1, 2);
  
  en_hiz_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the charge termination enable setting
 * @return True if charge termination is enabled, false if disabled
 */
bool Adafruit_BQ25798::getTerminationEnable() {
  Adafruit_BusIO_Register chg_ctrl0_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_0);
  Adafruit_BusIO_RegisterBits en_term_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl0_reg, 1, 1);
  
  return en_term_bit.read() == 1;
}

/*!
 * @brief Set the charge termination enable
 * @param enable True = enable charge termination, false = disable charge termination
 * @return True if successful
 */
bool Adafruit_BQ25798::setTerminationEnable(bool enable) {
  Adafruit_BusIO_Register chg_ctrl0_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_0);
  Adafruit_BusIO_RegisterBits en_term_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl0_reg, 1, 1);
  
  en_term_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the backup mode enable setting
 * @return True if backup mode is enabled, false if disabled
 */
bool Adafruit_BQ25798::getBackupModeEnable() {
  Adafruit_BusIO_Register chg_ctrl0_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_0);
  Adafruit_BusIO_RegisterBits en_backup_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl0_reg, 1, 0);
  
  return en_backup_bit.read() == 1;
}

/*!
 * @brief Set the backup mode enable
 * @param enable True = enable backup mode, false = disable backup mode
 * @return True if successful
 */
bool Adafruit_BQ25798::setBackupModeEnable(bool enable) {
  Adafruit_BusIO_Register chg_ctrl0_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_0);
  Adafruit_BusIO_RegisterBits en_backup_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl0_reg, 1, 0);
  
  en_backup_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the backup mode threshold setting
 * @return Backup mode threshold setting
 */
bq25798_vbus_backup_t Adafruit_BQ25798::getBackupModeThresh() {
  Adafruit_BusIO_Register chg_ctrl1_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_1);
  Adafruit_BusIO_RegisterBits vbus_backup_bits = Adafruit_BusIO_RegisterBits(&chg_ctrl1_reg, 2, 6);
  
  uint8_t reg_value = vbus_backup_bits.read();
  
  return (bq25798_vbus_backup_t)reg_value;
}

/*!
 * @brief Set the backup mode threshold
 * @param threshold Backup mode threshold setting (percentage of VINDPM)
 * @return True if successful
 */
bool Adafruit_BQ25798::setBackupModeThresh(bq25798_vbus_backup_t threshold) {
  if (threshold > BQ25798_VBUS_BACKUP_100_PERCENT) {
    return false;
  }
  
  Adafruit_BusIO_Register chg_ctrl1_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_1);
  Adafruit_BusIO_RegisterBits vbus_backup_bits = Adafruit_BusIO_RegisterBits(&chg_ctrl1_reg, 2, 6);
  
  vbus_backup_bits.write((uint8_t)threshold);
  
  return true;
}

/*!
 * @brief Get the VAC overvoltage protection setting
 * @return VAC OVP threshold setting
 */
bq25798_vac_ovp_t Adafruit_BQ25798::getVACOVP() {
  Adafruit_BusIO_Register chg_ctrl1_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_1);
  Adafruit_BusIO_RegisterBits vac_ovp_bits = Adafruit_BusIO_RegisterBits(&chg_ctrl1_reg, 2, 4);
  
  uint8_t reg_value = vac_ovp_bits.read();
  
  return (bq25798_vac_ovp_t)reg_value;
}

/*!
 * @brief Set the VAC overvoltage protection threshold
 * @param threshold VAC OVP threshold setting
 * @return True if successful
 */
bool Adafruit_BQ25798::setVACOVP(bq25798_vac_ovp_t threshold) {
  if (threshold > BQ25798_VAC_OVP_7V) {
    return false;
  }
  
  Adafruit_BusIO_Register chg_ctrl1_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_1);
  Adafruit_BusIO_RegisterBits vac_ovp_bits = Adafruit_BusIO_RegisterBits(&chg_ctrl1_reg, 2, 4);
  
  vac_ovp_bits.write((uint8_t)threshold);
  
  return true;
}

/*!
 * @brief Reset the watchdog timer
 * @return True if successful
 */
bool Adafruit_BQ25798::resetWDT() {
  Adafruit_BusIO_Register chg_ctrl1_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_1);
  Adafruit_BusIO_RegisterBits wd_rst_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl1_reg, 1, 3);
  
  wd_rst_bit.write(1);
  
  return true;
}

/*!
 * @brief Get the watchdog timer setting
 * @return Watchdog timer setting
 */
bq25798_wdt_t Adafruit_BQ25798::getWDT() {
  Adafruit_BusIO_Register chg_ctrl1_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_1);
  Adafruit_BusIO_RegisterBits watchdog_bits = Adafruit_BusIO_RegisterBits(&chg_ctrl1_reg, 3, 0);
  
  uint8_t reg_value = watchdog_bits.read();
  
  return (bq25798_wdt_t)reg_value;
}

/*!
 * @brief Set the watchdog timer
 * @param timer Watchdog timer setting
 * @return True if successful
 */
bool Adafruit_BQ25798::setWDT(bq25798_wdt_t timer) {
  if (timer > BQ25798_WDT_160S) {
    return false;
  }
  
  Adafruit_BusIO_Register chg_ctrl1_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_1);
  Adafruit_BusIO_RegisterBits watchdog_bits = Adafruit_BusIO_RegisterBits(&chg_ctrl1_reg, 3, 0);
  
  watchdog_bits.write((uint8_t)timer);
  
  return true;
}

/*!
 * @brief Get the force D+/D- pins detection setting
 * @return True if force D+/D- detection is enabled, false if disabled
 */
bool Adafruit_BQ25798::getForceDPinsDetection() {
  Adafruit_BusIO_Register chg_ctrl2_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_2);
  Adafruit_BusIO_RegisterBits force_indet_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl2_reg, 1, 7);
  
  return force_indet_bit.read() == 1;
}

/*!
 * @brief Set the force D+/D- pins detection
 * @param enable True = force D+/D- detection, false = disable
 * @return True if successful
 */
bool Adafruit_BQ25798::setForceDPinsDetection(bool enable) {
  Adafruit_BusIO_Register chg_ctrl2_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_2);
  Adafruit_BusIO_RegisterBits force_indet_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl2_reg, 1, 7);
  
  force_indet_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the automatic D+/D- pins detection setting
 * @return True if auto D+/D- detection is enabled, false if disabled
 */
bool Adafruit_BQ25798::getAutoDPinsDetection() {
  Adafruit_BusIO_Register chg_ctrl2_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_2);
  Adafruit_BusIO_RegisterBits auto_indet_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl2_reg, 1, 6);
  
  return auto_indet_bit.read() == 1;
}

/*!
 * @brief Set the automatic D+/D- pins detection
 * @param enable True = enable auto D+/D- detection, false = disable
 * @return True if successful
 */
bool Adafruit_BQ25798::setAutoDPinsDetection(bool enable) {
  Adafruit_BusIO_Register chg_ctrl2_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_2);
  Adafruit_BusIO_RegisterBits auto_indet_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl2_reg, 1, 6);
  
  auto_indet_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the HVDCP 12V enable setting
 * @return True if HVDCP 12V is enabled, false if disabled
 */
bool Adafruit_BQ25798::getHVDCP12VEnable() {
  Adafruit_BusIO_Register chg_ctrl2_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_2);
  Adafruit_BusIO_RegisterBits en_12v_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl2_reg, 1, 5);
  
  return en_12v_bit.read() == 1;
}

/*!
 * @brief Set the HVDCP 12V enable
 * @param enable True = enable HVDCP 12V, false = disable
 * @return True if successful
 */
bool Adafruit_BQ25798::setHVDCP12VEnable(bool enable) {
  Adafruit_BusIO_Register chg_ctrl2_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_2);
  Adafruit_BusIO_RegisterBits en_12v_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl2_reg, 1, 5);
  
  en_12v_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the HVDCP 9V enable setting
 * @return True if HVDCP 9V is enabled, false if disabled
 */
bool Adafruit_BQ25798::getHVDCP9VEnable() {
  Adafruit_BusIO_Register chg_ctrl2_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_2);
  Adafruit_BusIO_RegisterBits en_9v_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl2_reg, 1, 4);
  
  return en_9v_bit.read() == 1;
}

/*!
 * @brief Set the HVDCP 9V enable
 * @param enable True = enable HVDCP 9V, false = disable
 * @return True if successful
 */
bool Adafruit_BQ25798::setHVDCP9VEnable(bool enable) {
  Adafruit_BusIO_Register chg_ctrl2_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_2);
  Adafruit_BusIO_RegisterBits en_9v_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl2_reg, 1, 4);
  
  en_9v_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the HVDCP enable setting
 * @return True if HVDCP is enabled, false if disabled
 */
bool Adafruit_BQ25798::getHVDCPEnable() {
  Adafruit_BusIO_Register chg_ctrl2_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_2);
  Adafruit_BusIO_RegisterBits hvdcp_en_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl2_reg, 1, 3);
  
  return hvdcp_en_bit.read() == 1;
}

/*!
 * @brief Set the HVDCP enable
 * @param enable True = enable HVDCP, false = disable
 * @return True if successful
 */
bool Adafruit_BQ25798::setHVDCPEnable(bool enable) {
  Adafruit_BusIO_Register chg_ctrl2_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_2);
  Adafruit_BusIO_RegisterBits hvdcp_en_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl2_reg, 1, 3);
  
  hvdcp_en_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the ship FET mode setting
 * @return Ship FET mode setting
 */
bq25798_sdrv_ctrl_t Adafruit_BQ25798::getShipFETmode() {
  Adafruit_BusIO_Register chg_ctrl2_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_2);
  Adafruit_BusIO_RegisterBits sdrv_ctrl_bits = Adafruit_BusIO_RegisterBits(&chg_ctrl2_reg, 2, 1);
  
  uint8_t reg_value = sdrv_ctrl_bits.read();
  
  return (bq25798_sdrv_ctrl_t)reg_value;
}

/*!
 * @brief Set the ship FET mode
 * @param mode Ship FET mode setting (IDLE, Shutdown, Ship, System Reset)
 * @return True if successful
 */
bool Adafruit_BQ25798::setShipFETmode(bq25798_sdrv_ctrl_t mode) {
  if (mode > BQ25798_SDRV_SYSTEM_RESET) {
    return false;
  }
  
  Adafruit_BusIO_Register chg_ctrl2_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_2);
  Adafruit_BusIO_RegisterBits sdrv_ctrl_bits = Adafruit_BusIO_RegisterBits(&chg_ctrl2_reg, 2, 1);
  
  sdrv_ctrl_bits.write((uint8_t)mode);
  
  return true;
}

/*!
 * @brief Get the ship FET 10s delay setting
 * @return True if ship FET 10s delay is enabled, false if disabled
 */
bool Adafruit_BQ25798::getShipFET10sDelay() {
  Adafruit_BusIO_Register chg_ctrl2_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_2);
  Adafruit_BusIO_RegisterBits sdrv_dly_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl2_reg, 1, 0);
  
  return sdrv_dly_bit.read() == 1;
}

/*!
 * @brief Set the ship FET 10s delay
 * @param enable True = enable 10s delay, false = disable
 * @return True if successful
 */
bool Adafruit_BQ25798::setShipFET10sDelay(bool enable) {
  Adafruit_BusIO_Register chg_ctrl2_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_2);
  Adafruit_BusIO_RegisterBits sdrv_dly_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl2_reg, 1, 0);
  
  sdrv_dly_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the AC driver enable setting
 * @return True if AC driver is enabled, false if disabled
 */
bool Adafruit_BQ25798::getACenable() {
  Adafruit_BusIO_Register chg_ctrl3_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_3);
  Adafruit_BusIO_RegisterBits dis_acdrv_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl3_reg, 1, 7);
  
  // Invert the DIS_ACDRV bit - 1 = disabled, 0 = enabled
  return dis_acdrv_bit.read() == 0;
}

/*!
 * @brief Set the AC driver enable
 * @param enable True = enable AC driver, false = disable AC driver
 * @return True if successful
 */
bool Adafruit_BQ25798::setACenable(bool enable) {
  Adafruit_BusIO_Register chg_ctrl3_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_3);
  Adafruit_BusIO_RegisterBits dis_acdrv_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl3_reg, 1, 7);
  
  // Invert the enable logic - write 0 to enable, 1 to disable
  dis_acdrv_bit.write(enable ? 0 : 1);
  
  return true;
}

/*!
 * @brief Get the OTG enable setting
 * @return True if OTG is enabled, false if disabled
 */
bool Adafruit_BQ25798::getOTGenable() {
  Adafruit_BusIO_Register chg_ctrl3_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_3);
  Adafruit_BusIO_RegisterBits en_otg_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl3_reg, 1, 6);
  
  return en_otg_bit.read() == 1;
}

/*!
 * @brief Set the OTG enable
 * @param enable True = enable OTG, false = disable OTG
 * @return True if successful
 */
bool Adafruit_BQ25798::setOTGenable(bool enable) {
  Adafruit_BusIO_Register chg_ctrl3_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_3);
  Adafruit_BusIO_RegisterBits en_otg_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl3_reg, 1, 6);
  
  en_otg_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the OTG PFM enable setting
 * @return True if OTG PFM is enabled, false if disabled
 */
bool Adafruit_BQ25798::getOTGPFM() {
  Adafruit_BusIO_Register chg_ctrl3_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_3);
  Adafruit_BusIO_RegisterBits pfm_otg_dis_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl3_reg, 1, 5);
  
  // Invert the PFM_OTG_DIS bit - 1 = disabled, 0 = enabled
  return pfm_otg_dis_bit.read() == 0;
}

/*!
 * @brief Set the OTG PFM enable
 * @param enable True = enable OTG PFM, false = disable OTG PFM
 * @return True if successful
 */
bool Adafruit_BQ25798::setOTGPFM(bool enable) {
  Adafruit_BusIO_Register chg_ctrl3_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_3);
  Adafruit_BusIO_RegisterBits pfm_otg_dis_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl3_reg, 1, 5);
  
  // Invert the enable logic - write 0 to enable, 1 to disable
  pfm_otg_dis_bit.write(enable ? 0 : 1);
  
  return true;
}

/*!
 * @brief Get the forward PFM enable setting
 * @return True if forward PFM is enabled, false if disabled
 */
bool Adafruit_BQ25798::getForwardPFM() {
  Adafruit_BusIO_Register chg_ctrl3_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_3);
  Adafruit_BusIO_RegisterBits pfm_fwd_dis_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl3_reg, 1, 4);
  
  // Invert the PFM_FWD_DIS bit - 1 = disabled, 0 = enabled
  return pfm_fwd_dis_bit.read() == 0;
}

/*!
 * @brief Set the forward PFM enable
 * @param enable True = enable forward PFM, false = disable forward PFM
 * @return True if successful
 */
bool Adafruit_BQ25798::setForwardPFM(bool enable) {
  Adafruit_BusIO_Register chg_ctrl3_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_3);
  Adafruit_BusIO_RegisterBits pfm_fwd_dis_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl3_reg, 1, 4);
  
  // Invert the enable logic - write 0 to enable, 1 to disable
  pfm_fwd_dis_bit.write(enable ? 0 : 1);
  
  return true;
}

/*!
 * @brief Get the ship mode wakeup delay setting
 * @return Ship mode wakeup delay setting
 */
bq25798_wkup_dly_t Adafruit_BQ25798::getShipWakeupDelay() {
  Adafruit_BusIO_Register chg_ctrl3_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_3);
  Adafruit_BusIO_RegisterBits wkup_dly_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl3_reg, 1, 3);
  
  uint8_t reg_value = wkup_dly_bit.read();
  
  return (bq25798_wkup_dly_t)reg_value;
}

/*!
 * @brief Set the ship mode wakeup delay
 * @param delay Ship mode wakeup delay setting (1s or 15ms)
 * @return True if successful
 */
bool Adafruit_BQ25798::setShipWakeupDelay(bq25798_wkup_dly_t delay) {
  if (delay > BQ25798_WKUP_DLY_15MS) {
    return false;
  }
  
  Adafruit_BusIO_Register chg_ctrl3_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_3);
  Adafruit_BusIO_RegisterBits wkup_dly_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl3_reg, 1, 3);
  
  wkup_dly_bit.write((uint8_t)delay);
  
  return true;
}

/*!
 * @brief Get the BATFET LDO precharge enable setting
 * @return True if BATFET LDO precharge is enabled, false if disabled
 */
bool Adafruit_BQ25798::getBATFETLDOprecharge() {
  Adafruit_BusIO_Register chg_ctrl3_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_3);
  Adafruit_BusIO_RegisterBits dis_ldo_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl3_reg, 1, 2);
  
  // Invert the DIS_LDO bit - 1 = disabled, 0 = enabled
  return dis_ldo_bit.read() == 0;
}

/*!
 * @brief Set the BATFET LDO precharge enable
 * @param enable True = enable BATFET LDO precharge, false = disable
 * @return True if successful
 */
bool Adafruit_BQ25798::setBATFETLDOprecharge(bool enable) {
  Adafruit_BusIO_Register chg_ctrl3_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_3);
  Adafruit_BusIO_RegisterBits dis_ldo_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl3_reg, 1, 2);
  
  // Invert the enable logic - write 0 to enable, 1 to disable
  dis_ldo_bit.write(enable ? 0 : 1);
  
  return true;
}

/*!
 * @brief Get the OTG out-of-audio enable setting
 * @return True if OTG OOA is enabled, false if disabled
 */
bool Adafruit_BQ25798::getOTGOOA() {
  Adafruit_BusIO_Register chg_ctrl3_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_3);
  Adafruit_BusIO_RegisterBits dis_otg_ooa_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl3_reg, 1, 1);
  
  // Invert the DIS_OTG_OOA bit - 1 = disabled, 0 = enabled
  return dis_otg_ooa_bit.read() == 0;
}

/*!
 * @brief Set the OTG out-of-audio enable
 * @param enable True = enable OTG OOA, false = disable OTG OOA
 * @return True if successful
 */
bool Adafruit_BQ25798::setOTGOOA(bool enable) {
  Adafruit_BusIO_Register chg_ctrl3_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_3);
  Adafruit_BusIO_RegisterBits dis_otg_ooa_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl3_reg, 1, 1);
  
  // Invert the enable logic - write 0 to enable, 1 to disable
  dis_otg_ooa_bit.write(enable ? 0 : 1);
  
  return true;
}

/*!
 * @brief Get the forward out-of-audio enable setting
 * @return True if forward OOA is enabled, false if disabled
 */
bool Adafruit_BQ25798::getForwardOOA() {
  Adafruit_BusIO_Register chg_ctrl3_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_3);
  Adafruit_BusIO_RegisterBits dis_fwd_ooa_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl3_reg, 1, 0);
  
  // Invert the DIS_FWD_OOA bit - 1 = disabled, 0 = enabled
  return dis_fwd_ooa_bit.read() == 0;
}

/*!
 * @brief Set the forward out-of-audio enable
 * @param enable True = enable forward OOA, false = disable forward OOA
 * @return True if successful
 */
bool Adafruit_BQ25798::setForwardOOA(bool enable) {
  Adafruit_BusIO_Register chg_ctrl3_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_3);
  Adafruit_BusIO_RegisterBits dis_fwd_ooa_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl3_reg, 1, 0);
  
  // Invert the enable logic - write 0 to enable, 1 to disable
  dis_fwd_ooa_bit.write(enable ? 0 : 1);
  
  return true;
}

/*!
 * @brief Get the ACDRV2 enable setting
 * @return True if ACDRV2 is enabled, false if disabled
 */
bool Adafruit_BQ25798::getACDRV2enable() {
  Adafruit_BusIO_Register chg_ctrl4_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_4);
  Adafruit_BusIO_RegisterBits en_acdrv2_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl4_reg, 1, 7);
  
  return en_acdrv2_bit.read() == 1;
}

/*!
 * @brief Set the ACDRV2 enable
 * @param enable True = enable ACDRV2, false = disable ACDRV2
 * @return True if successful
 */
bool Adafruit_BQ25798::setACDRV2enable(bool enable) {
  Adafruit_BusIO_Register chg_ctrl4_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_4);
  Adafruit_BusIO_RegisterBits en_acdrv2_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl4_reg, 1, 7);
  
  en_acdrv2_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the ACDRV1 enable setting
 * @return True if ACDRV1 is enabled, false if disabled
 */
bool Adafruit_BQ25798::getACDRV1enable() {
  Adafruit_BusIO_Register chg_ctrl4_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_4);
  Adafruit_BusIO_RegisterBits en_acdrv1_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl4_reg, 1, 6);
  
  return en_acdrv1_bit.read() == 1;
}

/*!
 * @brief Set the ACDRV1 enable
 * @param enable True = enable ACDRV1, false = disable ACDRV1
 * @return True if successful
 */
bool Adafruit_BQ25798::setACDRV1enable(bool enable) {
  Adafruit_BusIO_Register chg_ctrl4_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_4);
  Adafruit_BusIO_RegisterBits en_acdrv1_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl4_reg, 1, 6);
  
  en_acdrv1_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the PWM switching frequency setting
 * @return PWM frequency setting
 */
bq25798_pwm_freq_t Adafruit_BQ25798::getPWMFrequency() {
  Adafruit_BusIO_Register chg_ctrl4_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_4);
  Adafruit_BusIO_RegisterBits pwm_freq_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl4_reg, 1, 5);
  
  uint8_t reg_value = pwm_freq_bit.read();
  
  return (bq25798_pwm_freq_t)reg_value;
}

/*!
 * @brief Set the PWM switching frequency
 * @param frequency PWM frequency setting (1.5MHz or 750kHz)
 * @return True if successful
 */
bool Adafruit_BQ25798::setPWMFrequency(bq25798_pwm_freq_t frequency) {
  if (frequency > BQ25798_PWM_FREQ_750KHZ) {
    return false;
  }
  
  Adafruit_BusIO_Register chg_ctrl4_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_4);
  Adafruit_BusIO_RegisterBits pwm_freq_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl4_reg, 1, 5);
  
  pwm_freq_bit.write((uint8_t)frequency);
  
  return true;
}

/*!
 * @brief Get the STAT pin enable setting
 * @return True if STAT pin is enabled, false if disabled
 */
bool Adafruit_BQ25798::getStatPinEnable() {
  Adafruit_BusIO_Register chg_ctrl4_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_4);
  Adafruit_BusIO_RegisterBits dis_stat_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl4_reg, 1, 4);
  
  // Invert the DIS_STAT bit - 1 = disabled, 0 = enabled
  return dis_stat_bit.read() == 0;
}

/*!
 * @brief Set the STAT pin enable
 * @param enable True = enable STAT pin, false = disable STAT pin
 * @return True if successful
 */
bool Adafruit_BQ25798::setStatPinEnable(bool enable) {
  Adafruit_BusIO_Register chg_ctrl4_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_4);
  Adafruit_BusIO_RegisterBits dis_stat_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl4_reg, 1, 4);
  
  // Invert the enable logic - write 0 to enable, 1 to disable
  dis_stat_bit.write(enable ? 0 : 1);
  
  return true;
}

/*!
 * @brief Get the VSYS short protection enable setting
 * @return True if VSYS short protection is enabled, false if disabled
 */
bool Adafruit_BQ25798::getVSYSshortProtect() {
  Adafruit_BusIO_Register chg_ctrl4_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_4);
  Adafruit_BusIO_RegisterBits dis_vsys_short_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl4_reg, 1, 3);
  
  // Invert the DIS_VSYS_SHORT bit - 1 = disabled, 0 = enabled
  return dis_vsys_short_bit.read() == 0;
}

/*!
 * @brief Set the VSYS short protection enable
 * @param enable True = enable VSYS short protection, false = disable
 * @return True if successful
 */
bool Adafruit_BQ25798::setVSYSshortProtect(bool enable) {
  Adafruit_BusIO_Register chg_ctrl4_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_4);
  Adafruit_BusIO_RegisterBits dis_vsys_short_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl4_reg, 1, 3);
  
  // Invert the enable logic - write 0 to enable, 1 to disable
  dis_vsys_short_bit.write(enable ? 0 : 1);
  
  return true;
}

/*!
 * @brief Get the VOTG UVP protection enable setting
 * @return True if VOTG UVP protection is enabled, false if disabled
 */
bool Adafruit_BQ25798::getVOTG_UVPProtect() {
  Adafruit_BusIO_Register chg_ctrl4_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_4);
  Adafruit_BusIO_RegisterBits dis_votg_uvp_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl4_reg, 1, 2);
  
  // Invert the DIS_VOTG_UVP bit - 1 = disabled, 0 = enabled
  return dis_votg_uvp_bit.read() == 0;
}

/*!
 * @brief Set the VOTG UVP protection enable
 * @param enable True = enable VOTG UVP protection, false = disable
 * @return True if successful
 */
bool Adafruit_BQ25798::setVOTG_UVPProtect(bool enable) {
  Adafruit_BusIO_Register chg_ctrl4_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_4);
  Adafruit_BusIO_RegisterBits dis_votg_uvp_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl4_reg, 1, 2);
  
  // Invert the enable logic - write 0 to enable, 1 to disable
  dis_votg_uvp_bit.write(enable ? 0 : 1);
  
  return true;
}

/*!
 * @brief Set the VINDPM detection enable
 * @param enable True = enable VINDPM detection, false = disable
 * @return True if successful
 */
bool Adafruit_BQ25798::setVINDPMdetection(bool enable) {
  Adafruit_BusIO_Register chg_ctrl4_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_4);
  Adafruit_BusIO_RegisterBits force_vindpm_det_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl4_reg, 1, 1);
  
  force_vindpm_det_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the VINDPM detection enable setting
 * @return True if VINDPM detection is enabled, false if disabled
 */
bool Adafruit_BQ25798::getVINDPMdetection() {
  Adafruit_BusIO_Register chg_ctrl4_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_4);
  Adafruit_BusIO_RegisterBits force_vindpm_det_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl4_reg, 1, 1);
  
  return force_vindpm_det_bit.read() == 1;
}

/*!
 * @brief Get the IBUS OCP enable setting
 * @return True if IBUS OCP is enabled, false if disabled
 */
bool Adafruit_BQ25798::getIBUS_OCPenable() {
  Adafruit_BusIO_Register chg_ctrl4_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_4);
  Adafruit_BusIO_RegisterBits en_ibus_ocp_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl4_reg, 1, 0);
  
  return en_ibus_ocp_bit.read() == 1;
}

/*!
 * @brief Set the IBUS OCP enable
 * @param enable True = enable IBUS OCP, false = disable IBUS OCP
 * @return True if successful
 */
bool Adafruit_BQ25798::setIBUS_OCPenable(bool enable) {
  Adafruit_BusIO_Register chg_ctrl4_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_4);
  Adafruit_BusIO_RegisterBits en_ibus_ocp_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl4_reg, 1, 0);
  
  en_ibus_ocp_bit.write(enable ? 1 : 0);
  
  return true;
}

/*!
 * @brief Get the ship FET present status
 * @return True if ship FET is present
 */
bool Adafruit_BQ25798::getShipFETpresent() {
  Adafruit_BusIO_Register chg_ctrl5_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_5);
  Adafruit_BusIO_RegisterBits sfet_present_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl5_reg, 1, 7);
  
  return sfet_present_bit.read();
}

/*!
 * @brief Set the ship FET present status
 * @param enable True to set ship FET present
 * @return True if successful
 */
bool Adafruit_BQ25798::setShipFETpresent(bool enable) {
  Adafruit_BusIO_Register chg_ctrl5_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_5);
  Adafruit_BusIO_RegisterBits sfet_present_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl5_reg, 1, 7);
  
  sfet_present_bit.write(enable);
  
  return true;
}

/*!
 * @brief Get the battery discharge sense enable status
 * @return True if battery discharge sense is enabled
 */
bool Adafruit_BQ25798::getBatDischargeSenseEnable() {
  Adafruit_BusIO_Register chg_ctrl5_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_5);
  Adafruit_BusIO_RegisterBits en_ibat_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl5_reg, 1, 5);
  
  return en_ibat_bit.read();
}

/*!
 * @brief Set the battery discharge sense enable status
 * @param enable True to enable battery discharge sense
 * @return True if successful
 */
bool Adafruit_BQ25798::setBatDischargeSenseEnable(bool enable) {
  Adafruit_BusIO_Register chg_ctrl5_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_5);
  Adafruit_BusIO_RegisterBits en_ibat_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl5_reg, 1, 5);
  
  en_ibat_bit.write(enable);
  
  return true;
}

/*!
 * @brief Get the battery discharge current regulation setting
 * @return Current regulation setting
 */
bq25798_ibat_reg_t Adafruit_BQ25798::getBatDischargeA() {
  Adafruit_BusIO_Register chg_ctrl5_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_5);
  Adafruit_BusIO_RegisterBits ibat_reg_bits = Adafruit_BusIO_RegisterBits(&chg_ctrl5_reg, 2, 3);
  
  return (bq25798_ibat_reg_t)ibat_reg_bits.read();
}

/*!
 * @brief Set the battery discharge current regulation setting
 * @param current Current regulation setting
 * @return True if successful
 */
bool Adafruit_BQ25798::setBatDischargeA(bq25798_ibat_reg_t current) {
  Adafruit_BusIO_Register chg_ctrl5_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_5);
  Adafruit_BusIO_RegisterBits ibat_reg_bits = Adafruit_BusIO_RegisterBits(&chg_ctrl5_reg, 2, 3);
  
  ibat_reg_bits.write((uint8_t)current);
  
  return true;
}

/*!
 * @brief Get the IINDPM enable status
 * @return True if IINDPM is enabled
 */
bool Adafruit_BQ25798::getIINDPMenable() {
  Adafruit_BusIO_Register chg_ctrl5_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_5);
  Adafruit_BusIO_RegisterBits en_iindpm_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl5_reg, 1, 2);
  
  return en_iindpm_bit.read();
}

/*!
 * @brief Set the IINDPM enable status
 * @param enable True to enable IINDPM
 * @return True if successful
 */
bool Adafruit_BQ25798::setIINDPMenable(bool enable) {
  Adafruit_BusIO_Register chg_ctrl5_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_5);
  Adafruit_BusIO_RegisterBits en_iindpm_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl5_reg, 1, 2);
  
  en_iindpm_bit.write(enable);
  
  return true;
}

/*!
 * @brief Get the external ILIM pin enable status
 * @return True if external ILIM pin is enabled
 */
bool Adafruit_BQ25798::getExtILIMpin() {
  Adafruit_BusIO_Register chg_ctrl5_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_5);
  Adafruit_BusIO_RegisterBits en_extilim_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl5_reg, 1, 1);
  
  return en_extilim_bit.read();
}

/*!
 * @brief Set the external ILIM pin enable status
 * @param enable True to enable external ILIM pin
 * @return True if successful
 */
bool Adafruit_BQ25798::setExtILIMpin(bool enable) {
  Adafruit_BusIO_Register chg_ctrl5_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_5);
  Adafruit_BusIO_RegisterBits en_extilim_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl5_reg, 1, 1);
  
  en_extilim_bit.write(enable);
  
  return true;
}

/*!
 * @brief Get the battery discharge OCP enable status
 * @return True if battery discharge OCP is enabled
 */
bool Adafruit_BQ25798::getBatDischargeOCPenable() {
  Adafruit_BusIO_Register chg_ctrl5_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_5);
  Adafruit_BusIO_RegisterBits en_batoc_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl5_reg, 1, 0);
  
  return en_batoc_bit.read();
}

/*!
 * @brief Set the battery discharge OCP enable status
 * @param enable True to enable battery discharge OCP
 * @return True if successful
 */
bool Adafruit_BQ25798::setBatDischargeOCPenable(bool enable) {
  Adafruit_BusIO_Register chg_ctrl5_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_CHARGER_CONTROL_5);
  Adafruit_BusIO_RegisterBits en_batoc_bit = Adafruit_BusIO_RegisterBits(&chg_ctrl5_reg, 1, 0);
  
  en_batoc_bit.write(enable);
  
  return true;
}

/*!
 * @brief Get the VINDPM VOC percentage setting
 * @return VOC percentage setting
 */
bq25798_voc_pct_t Adafruit_BQ25798::getVINDPM_VOCpercent() {
  Adafruit_BusIO_Register mppt_ctrl_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_MPPT_CONTROL);
  Adafruit_BusIO_RegisterBits voc_pct_bits = Adafruit_BusIO_RegisterBits(&mppt_ctrl_reg, 3, 5);
  
  return (bq25798_voc_pct_t)voc_pct_bits.read();
}

/*!
 * @brief Set the VINDPM VOC percentage setting
 * @param percentage VOC percentage setting
 * @return True if successful
 */
bool Adafruit_BQ25798::setVINDPM_VOCpercent(bq25798_voc_pct_t percentage) {
  Adafruit_BusIO_Register mppt_ctrl_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_MPPT_CONTROL);
  Adafruit_BusIO_RegisterBits voc_pct_bits = Adafruit_BusIO_RegisterBits(&mppt_ctrl_reg, 3, 5);
  
  voc_pct_bits.write((uint8_t)percentage);
  
  return true;
}

/*!
 * @brief Get the VOC delay time setting
 * @return VOC delay setting
 */
bq25798_voc_dly_t Adafruit_BQ25798::getVOCdelay() {
  Adafruit_BusIO_Register mppt_ctrl_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_MPPT_CONTROL);
  Adafruit_BusIO_RegisterBits voc_dly_bits = Adafruit_BusIO_RegisterBits(&mppt_ctrl_reg, 2, 3);
  
  return (bq25798_voc_dly_t)voc_dly_bits.read();
}

/*!
 * @brief Set the VOC delay time setting
 * @param delay VOC delay setting
 * @return True if successful
 */
bool Adafruit_BQ25798::setVOCdelay(bq25798_voc_dly_t delay) {
  Adafruit_BusIO_Register mppt_ctrl_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_MPPT_CONTROL);
  Adafruit_BusIO_RegisterBits voc_dly_bits = Adafruit_BusIO_RegisterBits(&mppt_ctrl_reg, 2, 3);
  
  voc_dly_bits.write((uint8_t)delay);
  
  return true;
}

/*!
 * @brief Get the VOC measurement rate setting
 * @return VOC rate setting
 */
bq25798_voc_rate_t Adafruit_BQ25798::getVOCrate() {
  Adafruit_BusIO_Register mppt_ctrl_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_MPPT_CONTROL);
  Adafruit_BusIO_RegisterBits voc_rate_bits = Adafruit_BusIO_RegisterBits(&mppt_ctrl_reg, 2, 1);
  
  return (bq25798_voc_rate_t)voc_rate_bits.read();
}

/*!
 * @brief Set the VOC measurement rate setting
 * @param rate VOC rate setting
 * @return True if successful
 */
bool Adafruit_BQ25798::setVOCrate(bq25798_voc_rate_t rate) {
  Adafruit_BusIO_Register mppt_ctrl_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_MPPT_CONTROL);
  Adafruit_BusIO_RegisterBits voc_rate_bits = Adafruit_BusIO_RegisterBits(&mppt_ctrl_reg, 2, 1);
  
  voc_rate_bits.write((uint8_t)rate);
  
  return true;
}

/*!
 * @brief Get the MPPT enable status
 * @return True if MPPT is enabled
 */
bool Adafruit_BQ25798::getMPPTenable() {
  Adafruit_BusIO_Register mppt_ctrl_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_MPPT_CONTROL);
  Adafruit_BusIO_RegisterBits en_mppt_bit = Adafruit_BusIO_RegisterBits(&mppt_ctrl_reg, 1, 0);
  
  return en_mppt_bit.read();
}

/*!
 * @brief Set the MPPT enable status
 * @param enable True to enable MPPT
 * @return True if successful
 */
bool Adafruit_BQ25798::setMPPTenable(bool enable) {
  Adafruit_BusIO_Register mppt_ctrl_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_MPPT_CONTROL);
  Adafruit_BusIO_RegisterBits en_mppt_bit = Adafruit_BusIO_RegisterBits(&mppt_ctrl_reg, 1, 0);
  
  en_mppt_bit.write(enable);
  
  return true;
}

/*!
 * @brief Get the thermal regulation threshold setting
 * @return Thermal regulation threshold setting
 */
bq25798_treg_t Adafruit_BQ25798::getThermRegulationThresh() {
  Adafruit_BusIO_Register temp_ctrl_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TEMPERATURE_CONTROL);
  Adafruit_BusIO_RegisterBits treg_bits = Adafruit_BusIO_RegisterBits(&temp_ctrl_reg, 2, 6);
  
  return (bq25798_treg_t)treg_bits.read();
}

/*!
 * @brief Set the thermal regulation threshold setting
 * @param threshold Thermal regulation threshold setting
 * @return True if successful
 */
bool Adafruit_BQ25798::setThermRegulationThresh(bq25798_treg_t threshold) {
  Adafruit_BusIO_Register temp_ctrl_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TEMPERATURE_CONTROL);
  Adafruit_BusIO_RegisterBits treg_bits = Adafruit_BusIO_RegisterBits(&temp_ctrl_reg, 2, 6);
  
  treg_bits.write((uint8_t)threshold);
  
  return true;
}

/*!
 * @brief Get the thermal shutdown threshold setting
 * @return Thermal shutdown threshold setting
 */
bq25798_tshut_t Adafruit_BQ25798::getThermShutdownThresh() {
  Adafruit_BusIO_Register temp_ctrl_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TEMPERATURE_CONTROL);
  Adafruit_BusIO_RegisterBits tshut_bits = Adafruit_BusIO_RegisterBits(&temp_ctrl_reg, 2, 4);
  
  return (bq25798_tshut_t)tshut_bits.read();
}

/*!
 * @brief Set the thermal shutdown threshold setting
 * @param threshold Thermal shutdown threshold setting
 * @return True if successful
 */
bool Adafruit_BQ25798::setThermShutdownThresh(bq25798_tshut_t threshold) {
  Adafruit_BusIO_Register temp_ctrl_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TEMPERATURE_CONTROL);
  Adafruit_BusIO_RegisterBits tshut_bits = Adafruit_BusIO_RegisterBits(&temp_ctrl_reg, 2, 4);
  
  tshut_bits.write((uint8_t)threshold);
  
  return true;
}

/*!
 * @brief Get the VBUS pulldown enable status
 * @return True if VBUS pulldown is enabled
 */
bool Adafruit_BQ25798::getVBUSpulldown() {
  Adafruit_BusIO_Register temp_ctrl_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TEMPERATURE_CONTROL);
  Adafruit_BusIO_RegisterBits vbus_pd_bit = Adafruit_BusIO_RegisterBits(&temp_ctrl_reg, 1, 3);
  
  return vbus_pd_bit.read();
}

/*!
 * @brief Set the VBUS pulldown enable status
 * @param enable True to enable VBUS pulldown
 * @return True if successful
 */
bool Adafruit_BQ25798::setVBUSpulldown(bool enable) {
  Adafruit_BusIO_Register temp_ctrl_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TEMPERATURE_CONTROL);
  Adafruit_BusIO_RegisterBits vbus_pd_bit = Adafruit_BusIO_RegisterBits(&temp_ctrl_reg, 1, 3);
  
  vbus_pd_bit.write(enable);
  
  return true;
}

/*!
 * @brief Get the VAC1 pulldown enable status
 * @return True if VAC1 pulldown is enabled
 */
bool Adafruit_BQ25798::getVAC1pulldown() {
  Adafruit_BusIO_Register temp_ctrl_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TEMPERATURE_CONTROL);
  Adafruit_BusIO_RegisterBits vac1_pd_bit = Adafruit_BusIO_RegisterBits(&temp_ctrl_reg, 1, 2);
  
  return vac1_pd_bit.read();
}

/*!
 * @brief Set the VAC1 pulldown enable status
 * @param enable True to enable VAC1 pulldown
 * @return True if successful
 */
bool Adafruit_BQ25798::setVAC1pulldown(bool enable) {
  Adafruit_BusIO_Register temp_ctrl_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TEMPERATURE_CONTROL);
  Adafruit_BusIO_RegisterBits vac1_pd_bit = Adafruit_BusIO_RegisterBits(&temp_ctrl_reg, 1, 2);
  
  vac1_pd_bit.write(enable);
  
  return true;
}

/*!
 * @brief Get the VAC2 pulldown enable status
 * @return True if VAC2 pulldown is enabled
 */
bool Adafruit_BQ25798::getVAC2pulldown() {
  Adafruit_BusIO_Register temp_ctrl_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TEMPERATURE_CONTROL);
  Adafruit_BusIO_RegisterBits vac2_pd_bit = Adafruit_BusIO_RegisterBits(&temp_ctrl_reg, 1, 1);
  
  return vac2_pd_bit.read();
}

/*!
 * @brief Set the VAC2 pulldown enable status
 * @param enable True to enable VAC2 pulldown
 * @return True if successful
 */
bool Adafruit_BQ25798::setVAC2pulldown(bool enable) {
  Adafruit_BusIO_Register temp_ctrl_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TEMPERATURE_CONTROL);
  Adafruit_BusIO_RegisterBits vac2_pd_bit = Adafruit_BusIO_RegisterBits(&temp_ctrl_reg, 1, 1);
  
  vac2_pd_bit.write(enable);
  
  return true;
}

/*!
 * @brief Get the backup ACFET1 on status
 * @return True if backup ACFET1 is on
 */
bool Adafruit_BQ25798::getBackupACFET1on() {
  Adafruit_BusIO_Register temp_ctrl_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TEMPERATURE_CONTROL);
  Adafruit_BusIO_RegisterBits bkup_acfet1_bit = Adafruit_BusIO_RegisterBits(&temp_ctrl_reg, 1, 0);
  
  return bkup_acfet1_bit.read();
}

/*!
 * @brief Set the backup ACFET1 on status
 * @param enable True to turn on backup ACFET1
 * @return True if successful
 */
bool Adafruit_BQ25798::setBackupACFET1on(bool enable) {
  Adafruit_BusIO_Register temp_ctrl_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TEMPERATURE_CONTROL);
  Adafruit_BusIO_RegisterBits bkup_acfet1_bit = Adafruit_BusIO_RegisterBits(&temp_ctrl_reg, 1, 0);
  
  bkup_acfet1_bit.write(enable);
  
  return true;
}

/*!
 * @brief Reset all registers to default values
 * @return True if successful
 */
bool Adafruit_BQ25798::reset() {
  Adafruit_BusIO_Register term_reg = Adafruit_BusIO_Register(i2c_dev, BQ25798_REG_TERMINATION_CONTROL);
  Adafruit_BusIO_RegisterBits reg_rst_bit = Adafruit_BusIO_RegisterBits(&term_reg, 1, 6);
  
  reg_rst_bit.write(1);
  
  return true;
}