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
}

void loop() {
  delay(1000);
}