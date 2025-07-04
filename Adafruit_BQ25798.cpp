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