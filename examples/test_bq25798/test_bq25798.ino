/*
 * Basic test for Adafruit BQ25798 I2C controlled buck-boost battery charger
 * 
 * This example initializes the BQ25798 and verifies communication
 */

#include <Adafruit_BQ25798.h>

Adafruit_BQ25798 bq;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  Serial.println(F("Adafruit BQ25798 test"));
  
  if (!bq.begin()) {
    Serial.println(F("Could not find a valid BQ25798 sensor, check wiring!"));
    while (1);
  }
  
  Serial.println(F("BQ25798 found!"));
  
  // Test minimal system voltage functions
  Serial.print(F("Current minimal system voltage: "));
  float defaultVoltage = bq.getMinSystemV();
  Serial.print(defaultVoltage);
  Serial.println(F("V"));
  
  // Test setting minimal system voltage to 0.5V higher
  float testVoltage = defaultVoltage + 0.5;
  if (bq.setMinSystemV(testVoltage)) {
    Serial.print(F("Set minimal system voltage to "));
    Serial.print(testVoltage);
    Serial.println(F("V"));
    Serial.print(F("Read back: "));
    Serial.print(bq.getMinSystemV());
    Serial.println(F("V"));
  } else {
    Serial.println(F("Failed to set minimal system voltage"));
  }
  
  // Restore to default
  bq.setMinSystemV(defaultVoltage);
  Serial.print(F("Restored to default: "));
  Serial.print(bq.getMinSystemV());
  Serial.println(F("V"));
  
  Serial.println();
  
  // Test charge voltage limit functions
  Serial.print(F("Current charge voltage limit: "));
  float defaultChargeV = bq.getChargeLimitV();
  Serial.print(defaultChargeV);
  Serial.println(F("V"));
  
  // Test setting charge voltage limit to 0.1V lower
  float testChargeV = defaultChargeV - 0.1;
  if (bq.setChargeLimitV(testChargeV)) {
    Serial.print(F("Set charge voltage limit to "));
    Serial.print(testChargeV);
    Serial.println(F("V"));
    Serial.print(F("Read back: "));
    Serial.print(bq.getChargeLimitV());
    Serial.println(F("V"));
  } else {
    Serial.println(F("Failed to set charge voltage limit"));
  }
  
  // Restore to default
  bq.setChargeLimitV(defaultChargeV);
  Serial.print(F("Restored to default: "));
  Serial.print(bq.getChargeLimitV());
  Serial.println(F("V"));
  
  Serial.println();
  
  // Test charge current limit functions
  Serial.print(F("Current charge current limit: "));
  float defaultChargeA = bq.getChargeLimitA();
  Serial.print(defaultChargeA);
  Serial.println(F("A"));
  
  // Test setting charge current limit to 0.1A higher
  float testChargeA = defaultChargeA + 0.1;
  if (bq.setChargeLimitA(testChargeA)) {
    Serial.print(F("Set charge current limit to "));
    Serial.print(testChargeA);
    Serial.println(F("A"));
    Serial.print(F("Read back: "));
    Serial.print(bq.getChargeLimitA());
    Serial.println(F("A"));
  } else {
    Serial.println(F("Failed to set charge current limit"));
  }
  
  // Restore to default
  bq.setChargeLimitA(defaultChargeA);
  Serial.print(F("Restored to default: "));
  Serial.print(bq.getChargeLimitA());
  Serial.println(F("A"));
  
  Serial.println();
  
  // Test input voltage limit functions
  Serial.print(F("Current input voltage limit: "));
  float defaultInputV = bq.getInputLimitV();
  Serial.print(defaultInputV);
  Serial.println(F("V"));
  
  // Test setting input voltage limit to 0.5V higher
  float testInputV = defaultInputV + 0.5;
  if (bq.setInputLimitV(testInputV)) {
    Serial.print(F("Set input voltage limit to "));
    Serial.print(testInputV);
    Serial.println(F("V"));
    Serial.print(F("Read back: "));
    Serial.print(bq.getInputLimitV());
    Serial.println(F("V"));
  } else {
    Serial.println(F("Failed to set input voltage limit"));
  }
  
  // Restore to default
  bq.setInputLimitV(defaultInputV);
  Serial.print(F("Restored to default: "));
  Serial.print(bq.getInputLimitV());
  Serial.println(F("V"));
  
  Serial.println();
  
  // Test input current limit functions
  Serial.print(F("Current input current limit: "));
  float defaultInputA = bq.getInputLimitA();
  Serial.print(defaultInputA);
  Serial.println(F("A"));
  
  // Test setting input current limit to 0.2A lower
  float testInputA = defaultInputA - 0.2;
  if (bq.setInputLimitA(testInputA)) {
    Serial.print(F("Set input current limit to "));
    Serial.print(testInputA);
    Serial.println(F("A"));
    Serial.print(F("Read back: "));
    Serial.print(bq.getInputLimitA());
    Serial.println(F("A"));
  } else {
    Serial.println(F("Failed to set input current limit"));
  }
  
  // Restore to default
  bq.setInputLimitA(defaultInputA);
  Serial.print(F("Restored to default: "));
  Serial.print(bq.getInputLimitA());
  Serial.println(F("A"));
  
  Serial.println();
  
  // Test battery voltage threshold functions
  // Options: 15%, 62.2%, 66.7%, 71.4% of VREG (default: 71.4%)
  if (bq.setVBatLowV(BQ25798_VBAT_LOWV_66_7_PERCENT)) {
    Serial.print(F("Set VBat low threshold to "));
    switch(bq.getVBatLowV()) {
      case BQ25798_VBAT_LOWV_15_PERCENT: Serial.println(F("15% of VREG")); break;
      case BQ25798_VBAT_LOWV_62_2_PERCENT: Serial.println(F("62.2% of VREG")); break;
      case BQ25798_VBAT_LOWV_66_7_PERCENT: Serial.println(F("66.7% of VREG")); break;
      case BQ25798_VBAT_LOWV_71_4_PERCENT: Serial.println(F("71.4% of VREG")); break;
    }
  } else {
    Serial.println(F("Failed to set VBat low threshold"));
  }
  
  Serial.println();
  
  // Test precharge current limit functions
  // Range: 0.04A to 2.0A, 40mA steps (default: 0.12A)
  Serial.print(F("Current precharge current limit: "));
  Serial.print(bq.getPrechargeLimitA());
  Serial.println(F("A"));
  
  // Test setting precharge current to 0.2A
  if (bq.setPrechargeLimitA(0.2)) {
    Serial.print(F("Set precharge current limit to "));
    Serial.print(bq.getPrechargeLimitA());
    Serial.println(F("A"));
  } else {
    Serial.println(F("Failed to set precharge current limit"));
  }
  
  Serial.println();
  
  // Test watchdog timer behavior setting
  /*
  Serial.print(F("Current stopOnWDT setting: "));
  bool currentStopOnWDT = bq.getStopOnWDT();
  Serial.println(currentStopOnWDT ? "true (WDT will NOT reset safety timers)" : "false (WDT will reset safety timers)");
  
  // Toggle the setting
  bool newStopOnWDT = !currentStopOnWDT;
  if (bq.setStopOnWDT(newStopOnWDT)) {
    Serial.print(F("Set stopOnWDT to "));
    Serial.print(newStopOnWDT ? "true" : "false");
    Serial.print(F(" - Read back: "));
    Serial.println(bq.getStopOnWDT() ? "true" : "false");
  } else {
    Serial.println(F("Failed to set stopOnWDT"));
  }
  */
  
  Serial.println();
  
  // Test termination current limit functions
  // Range: 0.04A to 1.0A, 40mA steps (default: 0.2A)
  Serial.print(F("Current termination current limit: "));
  float defaultTermA = bq.getTerminationA();
  Serial.print(defaultTermA);
  Serial.println(F("A"));
  
  // Test setting termination current to 0.32A
  float testTermA = 0.32;
  if (bq.setTerminationA(testTermA)) {
    Serial.print(F("Set termination current limit to "));
    Serial.print(testTermA);
    Serial.println(F("A"));
    Serial.print(F("Read back: "));
    Serial.print(bq.getTerminationA());
    Serial.println(F("A"));
  } else {
    Serial.println(F("Failed to set termination current limit"));
  }
  
  // Restore to default
  bq.setTerminationA(defaultTermA);
  Serial.print(F("Restored to default: "));
  Serial.print(bq.getTerminationA());
  Serial.println(F("A"));
  
  Serial.println();
  
  // Test battery cell count functions
  // bq.setCellCount(BQ25798_CELL_COUNT_2S); // Uncomment to set cell count if desired
  Serial.print(F("Current cell count: "));
  bq25798_cell_count_t currentCellCount = bq.getCellCount();
  switch(currentCellCount) {
    case BQ25798_CELL_COUNT_1S: Serial.println(F("1S (1 cell)")); break;
    case BQ25798_CELL_COUNT_2S: Serial.println(F("2S (2 cells)")); break;
    case BQ25798_CELL_COUNT_3S: Serial.println(F("3S (3 cells)")); break;
    case BQ25798_CELL_COUNT_4S: Serial.println(F("4S (4 cells)")); break;
  }
  
  Serial.println();
  
  // Test battery recharge deglitch time functions
  // bq.setRechargeDeglitchTime(BQ25798_TRECHG_256MS); // Uncomment to set deglitch time if desired
  Serial.print(F("Current recharge deglitch time: "));
  bq25798_trechg_time_t currentDeglitchTime = bq.getRechargeDeglitchTime();
  switch(currentDeglitchTime) {
    case BQ25798_TRECHG_64MS: Serial.println(F("64ms")); break;
    case BQ25798_TRECHG_256MS: Serial.println(F("256ms")); break;
    case BQ25798_TRECHG_1024MS: Serial.println(F("1024ms")); break;
    case BQ25798_TRECHG_2048MS: Serial.println(F("2048ms")); break;
  }
  
  Serial.println();
  
  // Test battery recharge threshold offset voltage functions
  // bq.setRechargeThreshOffsetV(0.15); // Uncomment to set recharge threshold if desired
  Serial.print(F("Current recharge threshold offset: "));
  Serial.print(bq.getRechargeThreshOffsetV());
  Serial.println(F("V (below VREG)"));
  
  Serial.println();
  
  // Test OTG voltage regulation functions
  // bq.setOTGV(5.5); // Uncomment to set OTG voltage if desired
  Serial.print(F("Current OTG voltage: "));
  Serial.print(bq.getOTGV());
  Serial.println(F("V"));
  
  Serial.println();
  
  // Test precharge safety timer functions
  // bq.setPrechargeTimer(BQ25798_PRECHG_TMR_0_5HR); // Uncomment to set timer if desired
  Serial.print(F("Current precharge timer: "));
  bq25798_prechg_timer_t currentTimer = bq.getPrechargeTimer();
  switch(currentTimer) {
    case BQ25798_PRECHG_TMR_2HR: Serial.println(F("2 hours")); break;
    case BQ25798_PRECHG_TMR_0_5HR: Serial.println(F("0.5 hours")); break;
  }
  
  Serial.println();
  
  // Test OTG current limit functions
  // bq.setOTGLimitA(2.0); // Uncomment to set OTG current if desired
  Serial.print(F("Current OTG current limit: "));
  Serial.print(bq.getOTGLimitA());
  Serial.println(F("A"));
  
  Serial.println();
  
  // Test top-off timer functions
  // bq.setTopOffTimer(BQ25798_TOPOFF_TMR_15MIN); // Uncomment to set timer if desired
  Serial.print(F("Current top-off timer: "));
  bq25798_topoff_timer_t currentTopOffTimer = bq.getTopOffTimer();
  switch(currentTopOffTimer) {
    case BQ25798_TOPOFF_TMR_DISABLED: Serial.println(F("Disabled")); break;
    case BQ25798_TOPOFF_TMR_15MIN: Serial.println(F("15 minutes")); break;
    case BQ25798_TOPOFF_TMR_30MIN: Serial.println(F("30 minutes")); break;
    case BQ25798_TOPOFF_TMR_45MIN: Serial.println(F("45 minutes")); break;
  }
  
  Serial.println();
  
  // Test trickle charge timer enable functions
  // bq.setTrickleChargeTimerEnable(false); // Uncomment to disable timer if desired
  Serial.print(F("Trickle charge timer enabled: "));
  Serial.println(bq.getTrickleChargeTimerEnable() ? "true" : "false");
  
  Serial.println();
  
  // Test precharge timer enable functions
  // bq.setPrechargeTimerEnable(false); // Uncomment to disable timer if desired
  Serial.print(F("Precharge timer enabled: "));
  Serial.println(bq.getPrechargeTimerEnable() ? "true" : "false");
  
  Serial.println();
  
  // Test fast charge timer enable functions
  // bq.setFastChargeTimerEnable(false); // Uncomment to disable timer if desired
  Serial.print(F("Fast charge timer enabled: "));
  Serial.println(bq.getFastChargeTimerEnable() ? "true" : "false");
  
  Serial.println();
  
  // Test fast charge timer setting functions
  // bq.setFastChargeTimer(BQ25798_CHG_TMR_8HR); // Uncomment to set timer if desired
  Serial.print(F("Current fast charge timer: "));
  bq25798_chg_timer_t currentFastChargeTimer = bq.getFastChargeTimer();
  switch(currentFastChargeTimer) {
    case BQ25798_CHG_TMR_5HR: Serial.println(F("5 hours")); break;
    case BQ25798_CHG_TMR_8HR: Serial.println(F("8 hours")); break;
    case BQ25798_CHG_TMR_12HR: Serial.println(F("12 hours")); break;
    case BQ25798_CHG_TMR_24HR: Serial.println(F("24 hours")); break;
  }
  
  Serial.println();
  
  // Test timer half-rate enable functions
  // bq.setTimerHalfRateEnable(false); // Uncomment to disable half-rate if desired
  Serial.print(F("Timer half-rate enabled: "));
  Serial.println(bq.getTimerHalfRateEnable() ? F("true") : F("false"));
}

void loop() {
  delay(1000);
}