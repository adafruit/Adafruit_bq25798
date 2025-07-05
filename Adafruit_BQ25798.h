/*!
 * @file Adafruit_BQ25798.h
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

#ifndef __ADAFRUIT_BQ25798_H__
#define __ADAFRUIT_BQ25798_H__

#include "Arduino.h"
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>

#define BQ25798_DEFAULT_ADDR 0x6B ///< Default I2C address

// Register definitions
#define BQ25798_REG_MINIMAL_SYSTEM_VOLTAGE 0x00     ///< Minimal System Voltage
#define BQ25798_REG_CHARGE_VOLTAGE_LIMIT 0x01       ///< Charge Voltage Limit
#define BQ25798_REG_CHARGE_CURRENT_LIMIT 0x03       ///< Charge Current Limit
#define BQ25798_REG_INPUT_VOLTAGE_LIMIT 0x05        ///< Input Voltage Limit
#define BQ25798_REG_INPUT_CURRENT_LIMIT 0x06        ///< Input Current Limit
#define BQ25798_REG_PRECHARGE_CONTROL 0x08          ///< Precharge Control
#define BQ25798_REG_TERMINATION_CONTROL 0x09        ///< Termination Control
#define BQ25798_REG_RECHARGE_CONTROL 0x0A           ///< Re-charge Control
#define BQ25798_REG_VOTG_REGULATION 0x0B            ///< VOTG regulation
#define BQ25798_REG_IOTG_REGULATION 0x0D            ///< IOTG regulation
#define BQ25798_REG_TIMER_CONTROL 0x0E              ///< Timer Control
#define BQ25798_REG_CHARGER_CONTROL_0 0x0F          ///< Charger Control 0
#define BQ25798_REG_CHARGER_CONTROL_1 0x10          ///< Charger Control 1
#define BQ25798_REG_CHARGER_CONTROL_2 0x11          ///< Charger Control 2
#define BQ25798_REG_CHARGER_CONTROL_3 0x12          ///< Charger Control 3
#define BQ25798_REG_CHARGER_CONTROL_4 0x13          ///< Charger Control 4
#define BQ25798_REG_CHARGER_CONTROL_5 0x14          ///< Charger Control 5
#define BQ25798_REG_MPPT_CONTROL 0x15               ///< MPPT Control
#define BQ25798_REG_TEMPERATURE_CONTROL 0x16        ///< Temperature Control
#define BQ25798_REG_NTC_CONTROL_0 0x17              ///< NTC Control 0
#define BQ25798_REG_NTC_CONTROL_1 0x18              ///< NTC Control 1
#define BQ25798_REG_ICO_CURRENT_LIMIT 0x19          ///< ICO Current Limit
#define BQ25798_REG_CHARGER_STATUS_0 0x1B           ///< Charger Status 0
#define BQ25798_REG_CHARGER_STATUS_1 0x1C           ///< Charger Status 1
#define BQ25798_REG_CHARGER_STATUS_2 0x1D           ///< Charger Status 2
#define BQ25798_REG_CHARGER_STATUS_3 0x1E           ///< Charger Status 3
#define BQ25798_REG_CHARGER_STATUS_4 0x1F           ///< Charger Status 4
#define BQ25798_REG_FAULT_STATUS_0 0x20             ///< FAULT Status 0
#define BQ25798_REG_FAULT_STATUS_1 0x21             ///< FAULT Status 1
#define BQ25798_REG_CHARGER_FLAG_0 0x22             ///< Charger Flag 0
#define BQ25798_REG_CHARGER_FLAG_1 0x23             ///< Charger Flag 1
#define BQ25798_REG_CHARGER_FLAG_2 0x24             ///< Charger Flag 2
#define BQ25798_REG_CHARGER_FLAG_3 0x25             ///< Charger Flag 3
#define BQ25798_REG_FAULT_FLAG_0 0x26               ///< FAULT Flag 0
#define BQ25798_REG_FAULT_FLAG_1 0x27               ///< FAULT Flag 1
#define BQ25798_REG_CHARGER_MASK_0 0x28             ///< Charger Mask 0
#define BQ25798_REG_CHARGER_MASK_1 0x29             ///< Charger Mask 1
#define BQ25798_REG_CHARGER_MASK_2 0x2A             ///< Charger Mask 2
#define BQ25798_REG_CHARGER_MASK_3 0x2B             ///< Charger Mask 3
#define BQ25798_REG_FAULT_MASK_0 0x2C               ///< FAULT Mask 0
#define BQ25798_REG_FAULT_MASK_1 0x2D               ///< FAULT Mask 1
#define BQ25798_REG_ADC_CONTROL 0x2E                ///< ADC Control
#define BQ25798_REG_ADC_FUNCTION_DISABLE_0 0x2F     ///< ADC Function Disable 0
#define BQ25798_REG_ADC_FUNCTION_DISABLE_1 0x30     ///< ADC Function Disable 1
#define BQ25798_REG_IBUS_ADC 0x31                   ///< IBUS ADC
#define BQ25798_REG_IBAT_ADC 0x33                   ///< IBAT ADC
#define BQ25798_REG_VBUS_ADC 0x35                   ///< VBUS ADC
#define BQ25798_REG_VAC1_ADC 0x37                   ///< VAC1 ADC
#define BQ25798_REG_VAC2_ADC 0x39                   ///< VAC2 ADC
#define BQ25798_REG_VBAT_ADC 0x3B                   ///< VBAT ADC
#define BQ25798_REG_VSYS_ADC 0x3D                   ///< VSYS ADC
#define BQ25798_REG_TS_ADC 0x3F                     ///< TS ADC
#define BQ25798_REG_TDIE_ADC 0x41                   ///< TDIE ADC
#define BQ25798_REG_DPLUS_ADC 0x43                  ///< D+ ADC
#define BQ25798_REG_DMINUS_ADC 0x45                 ///< D- ADC
#define BQ25798_REG_DPDM_DRIVER 0x47                ///< DPDM Driver
#define BQ25798_REG_PART_INFORMATION 0x48           ///< Part Information

/*!
 * @brief Battery voltage threshold for precharge to fast charge transition
 */
typedef enum {
  BQ25798_VBAT_LOWV_15_PERCENT = 0x00,   ///< 15% of VREG
  BQ25798_VBAT_LOWV_62_2_PERCENT = 0x01, ///< 62.2% of VREG
  BQ25798_VBAT_LOWV_66_7_PERCENT = 0x02, ///< 66.7% of VREG
  BQ25798_VBAT_LOWV_71_4_PERCENT = 0x03  ///< 71.4% of VREG (default)
} bq25798_vbat_lowv_t;

/*!
 * @brief Battery cell count selection
 */
typedef enum {
  BQ25798_CELL_COUNT_1S = 0x00, ///< 1 cell
  BQ25798_CELL_COUNT_2S = 0x01, ///< 2 cells
  BQ25798_CELL_COUNT_3S = 0x02, ///< 3 cells
  BQ25798_CELL_COUNT_4S = 0x03  ///< 4 cells
} bq25798_cell_count_t;

/*!
 * @brief Battery recharge deglitch time
 */
typedef enum {
  BQ25798_TRECHG_64MS = 0x00,   ///< 64ms
  BQ25798_TRECHG_256MS = 0x01,  ///< 256ms
  BQ25798_TRECHG_1024MS = 0x02, ///< 1024ms (default)
  BQ25798_TRECHG_2048MS = 0x03  ///< 2048ms
} bq25798_trechg_time_t;

/*!
 * @brief BQ25798 I2C controlled buck-boost battery charger
 */
class Adafruit_BQ25798 {
public:
  Adafruit_BQ25798();
  ~Adafruit_BQ25798();

  bool begin(uint8_t i2c_addr = BQ25798_DEFAULT_ADDR, TwoWire *wire = &Wire);

  float getMinSystemV();
  bool setMinSystemV(float voltage);

  float getChargeLimitV();
  bool setChargeLimitV(float voltage);

  float getChargeLimitA();
  bool setChargeLimitA(float current);

  float getInputLimitV();
  bool setInputLimitV(float voltage);

  float getInputLimitA();
  bool setInputLimitA(float current);

  bq25798_vbat_lowv_t getVBatLowV();
  bool setVBatLowV(bq25798_vbat_lowv_t threshold);

  float getPrechargeLimitA();
  bool setPrechargeLimitA(float current);

  bool getStopOnWDT();
  bool setStopOnWDT(bool stopOnWDT);

  float getTerminationA();
  bool setTerminationA(float current);

  bq25798_cell_count_t getCellCount();
  bool setCellCount(bq25798_cell_count_t cellCount);

  bq25798_trechg_time_t getRechargeDeglitchTime();
  bool setRechargeDeglitchTime(bq25798_trechg_time_t deglitchTime);

  float getRechargeThreshOffsetV();
  bool setRechargeThreshOffsetV(float voltage);

  bool reset();

private:
  Adafruit_I2CDevice *i2c_dev; ///< Pointer to I2C bus interface
};

#endif // __ADAFRUIT_BQ25798_H__