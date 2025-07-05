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
  
  Serial.println("Adafruit BQ25798 test");
  
  if (!bq.begin()) {
    Serial.println("Could not find a valid BQ25798 sensor, check wiring!");
    while (1);
  }
  
  Serial.println("BQ25798 found!");
  
  // Test minimal system voltage functions
  Serial.print("Current minimal system voltage: ");
  float defaultVoltage = bq.getMinSystemV();
  Serial.print(defaultVoltage);
  Serial.println("V");
  
  // Test setting minimal system voltage to 0.5V higher
  float testVoltage = defaultVoltage + 0.5;
  if (bq.setMinSystemV(testVoltage)) {
    Serial.print("Set minimal system voltage to ");
    Serial.print(testVoltage);
    Serial.println("V");
    Serial.print("Read back: ");
    Serial.print(bq.getMinSystemV());
    Serial.println("V");
  } else {
    Serial.println("Failed to set minimal system voltage");
  }
  
  // Restore to default
  bq.setMinSystemV(defaultVoltage);
  Serial.print("Restored to default: ");
  Serial.print(bq.getMinSystemV());
  Serial.println("V");
  
  Serial.println();
  
  // Test charge voltage limit functions
  Serial.print("Current charge voltage limit: ");
  float defaultChargeV = bq.getChargeLimitV();
  Serial.print(defaultChargeV);
  Serial.println("V");
  
  // Test setting charge voltage limit to 0.1V lower
  float testChargeV = defaultChargeV - 0.1;
  if (bq.setChargeLimitV(testChargeV)) {
    Serial.print("Set charge voltage limit to ");
    Serial.print(testChargeV);
    Serial.println("V");
    Serial.print("Read back: ");
    Serial.print(bq.getChargeLimitV());
    Serial.println("V");
  } else {
    Serial.println("Failed to set charge voltage limit");
  }
  
  // Restore to default
  bq.setChargeLimitV(defaultChargeV);
  Serial.print("Restored to default: ");
  Serial.print(bq.getChargeLimitV());
  Serial.println("V");
  
  Serial.println();
  
  // Test charge current limit functions
  Serial.print("Current charge current limit: ");
  float defaultChargeA = bq.getChargeLimitA();
  Serial.print(defaultChargeA);
  Serial.println("A");
  
  // Test setting charge current limit to 0.1A higher
  float testChargeA = defaultChargeA + 0.1;
  if (bq.setChargeLimitA(testChargeA)) {
    Serial.print("Set charge current limit to ");
    Serial.print(testChargeA);
    Serial.println("A");
    Serial.print("Read back: ");
    Serial.print(bq.getChargeLimitA());
    Serial.println("A");
  } else {
    Serial.println("Failed to set charge current limit");
  }
  
  // Restore to default
  bq.setChargeLimitA(defaultChargeA);
  Serial.print("Restored to default: ");
  Serial.print(bq.getChargeLimitA());
  Serial.println("A");
  
  Serial.println();
  
  // Test input voltage limit functions
  Serial.print("Current input voltage limit: ");
  float defaultInputV = bq.getInputLimitV();
  Serial.print(defaultInputV);
  Serial.println("V");
  
  // Test setting input voltage limit to 0.5V higher
  float testInputV = defaultInputV + 0.5;
  if (bq.setInputLimitV(testInputV)) {
    Serial.print("Set input voltage limit to ");
    Serial.print(testInputV);
    Serial.println("V");
    Serial.print("Read back: ");
    Serial.print(bq.getInputLimitV());
    Serial.println("V");
  } else {
    Serial.println("Failed to set input voltage limit");
  }
  
  // Restore to default
  bq.setInputLimitV(defaultInputV);
  Serial.print("Restored to default: ");
  Serial.print(bq.getInputLimitV());
  Serial.println("V");
  
  Serial.println();
  
  // Test input current limit functions
  Serial.print("Current input current limit: ");
  float defaultInputA = bq.getInputLimitA();
  Serial.print(defaultInputA);
  Serial.println("A");
  
  // Test setting input current limit to 0.2A lower
  float testInputA = defaultInputA - 0.2;
  if (bq.setInputLimitA(testInputA)) {
    Serial.print("Set input current limit to ");
    Serial.print(testInputA);
    Serial.println("A");
    Serial.print("Read back: ");
    Serial.print(bq.getInputLimitA());
    Serial.println("A");
  } else {
    Serial.println("Failed to set input current limit");
  }
  
  // Restore to default
  bq.setInputLimitA(defaultInputA);
  Serial.print("Restored to default: ");
  Serial.print(bq.getInputLimitA());
  Serial.println("A");
  
  Serial.println();
  
  // Test battery voltage threshold functions
  // Options: 15%, 62.2%, 66.7%, 71.4% of VREG (default: 71.4%)
  if (bq.setVBatLowV(BQ25798_VBAT_LOWV_66_7_PERCENT)) {
    Serial.print("Set VBat low threshold to ");
    switch(bq.getVBatLowV()) {
      case BQ25798_VBAT_LOWV_15_PERCENT: Serial.println("15% of VREG"); break;
      case BQ25798_VBAT_LOWV_62_2_PERCENT: Serial.println("62.2% of VREG"); break;
      case BQ25798_VBAT_LOWV_66_7_PERCENT: Serial.println("66.7% of VREG"); break;
      case BQ25798_VBAT_LOWV_71_4_PERCENT: Serial.println("71.4% of VREG"); break;
    }
  } else {
    Serial.println("Failed to set VBat low threshold");
  }
  
  Serial.println();
  
  // Test precharge current limit functions
  // Range: 0.04A to 2.0A, 40mA steps (default: 0.12A)
  Serial.print("Current precharge current limit: ");
  Serial.print(bq.getPrechargeLimitA());
  Serial.println("A");
  
  // Test setting precharge current to 0.2A
  if (bq.setPrechargeLimitA(0.2)) {
    Serial.print("Set precharge current limit to ");
    Serial.print(bq.getPrechargeLimitA());
    Serial.println("A");
  } else {
    Serial.println("Failed to set precharge current limit");
  }
  
  Serial.println();
  
  // Test watchdog timer behavior setting
  /*
  Serial.print("Current stopOnWDT setting: ");
  bool currentStopOnWDT = bq.getStopOnWDT();
  Serial.println(currentStopOnWDT ? "true (WDT will NOT reset safety timers)" : "false (WDT will reset safety timers)");
  
  // Toggle the setting
  bool newStopOnWDT = !currentStopOnWDT;
  if (bq.setStopOnWDT(newStopOnWDT)) {
    Serial.print("Set stopOnWDT to ");
    Serial.print(newStopOnWDT ? "true" : "false");
    Serial.print(" - Read back: ");
    Serial.println(bq.getStopOnWDT() ? "true" : "false");
  } else {
    Serial.println("Failed to set stopOnWDT");
  }
  */
  
  Serial.println();
  
  // Test termination current limit functions
  // Range: 0.04A to 1.0A, 40mA steps (default: 0.2A)
  Serial.print("Current termination current limit: ");
  float defaultTermA = bq.getTerminationA();
  Serial.print(defaultTermA);
  Serial.println("A");
  
  // Test setting termination current to 0.32A
  float testTermA = 0.32;
  if (bq.setTerminationA(testTermA)) {
    Serial.print("Set termination current limit to ");
    Serial.print(testTermA);
    Serial.println("A");
    Serial.print("Read back: ");
    Serial.print(bq.getTerminationA());
    Serial.println("A");
  } else {
    Serial.println("Failed to set termination current limit");
  }
  
  // Restore to default
  bq.setTerminationA(defaultTermA);
  Serial.print("Restored to default: ");
  Serial.print(bq.getTerminationA());
  Serial.println("A");
  
  Serial.println();
  
  // Test battery cell count functions
  // bq.setCellCount(BQ25798_CELL_COUNT_2S); // Uncomment to set cell count if desired
  Serial.print("Current cell count: ");
  bq25798_cell_count_t currentCellCount = bq.getCellCount();
  switch(currentCellCount) {
    case BQ25798_CELL_COUNT_1S: Serial.println("1S (1 cell)"); break;
    case BQ25798_CELL_COUNT_2S: Serial.println("2S (2 cells)"); break;
    case BQ25798_CELL_COUNT_3S: Serial.println("3S (3 cells)"); break;
    case BQ25798_CELL_COUNT_4S: Serial.println("4S (4 cells)"); break;
  }
  
  Serial.println();
  
  // Test battery recharge deglitch time functions
  // bq.setRechargeDeglitchTime(BQ25798_TRECHG_256MS); // Uncomment to set deglitch time if desired
  Serial.print("Current recharge deglitch time: ");
  bq25798_trechg_time_t currentDeglitchTime = bq.getRechargeDeglitchTime();
  switch(currentDeglitchTime) {
    case BQ25798_TRECHG_64MS: Serial.println("64ms"); break;
    case BQ25798_TRECHG_256MS: Serial.println("256ms"); break;
    case BQ25798_TRECHG_1024MS: Serial.println("1024ms"); break;
    case BQ25798_TRECHG_2048MS: Serial.println("2048ms"); break;
  }
  
  Serial.println();
  
  // Test battery recharge threshold offset voltage functions
  // bq.setRechargeThreshOffsetV(0.15); // Uncomment to set recharge threshold if desired
  Serial.print("Current recharge threshold offset: ");
  Serial.print(bq.getRechargeThreshOffsetV());
  Serial.println("V (below VREG)");
}

void loop() {
  delay(1000);
}