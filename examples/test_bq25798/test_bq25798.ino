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
}

void loop() {
  delay(1000);
}