# Adafruit BQ25798 Library [![Build Status](https://github.com/adafruit/Adafruit_bq25798/workflows/Arduino%20Library%20CI/badge.svg)](https://github.com/adafruit/Adafruit_bq25798/actions)[![Documentation](https://github.com/adafruit/ci-arduino/blob/master/assets/doxygen_badge.svg)](http://adafruit.github.io/Adafruit_bq25798/html/index.html)

Arduino library for the TI BQ25798 I2C controlled buck-boost battery charger with dual-input selector

## About this Driver

The BQ25798 is a fully integrated switch-mode buck-boost charger for 1-4 cell Li-ion batteries and Li-polymer batteries. Key features include:

- 1- to 4-cell Li-ion/Li-polymer battery support
- 5A charging current with 10mA resolution
- 3.6V to 24V wide input operating voltage range
- Dual-input power mux controller
- Built-in MPPT for solar panel charging
- USB OTG output (2.8V to 22V)
- Integrated 16-bit ADC for monitoring
- I2C interface for full control
- 29-pin 4mm x 4mm QFN package

## Installation

To install, use the Arduino Library Manager and search for "Adafruit BQ25798", or download the ZIP archive from the releases page.

## Dependencies

This library depends on the Adafruit BusIO library.

## Quick Start

```cpp
#include <Adafruit_BQ25798.h>

Adafruit_BQ25798 bq;

void setup() {
  Serial.begin(115200);
  
  if (!bq.begin()) {
    Serial.println("Could not find BQ25798 chip");
    while (1);
  }
  
  Serial.println("BQ25798 found!");
}

void loop() {
  // Your code here
}
```

## Hardware

The BQ25798 communicates via I2C. Connect:
- VIN to 3.3V or 5V power
- GND to ground
- SCL to I2C clock
- SDA to I2C data

The default I2C address is 0x6B.

## License

This library is licensed under the MIT license. See LICENSE for more details.

## Contributing

Contributions are welcome! Please read our Code of Conduct and Contributing guidelines before submitting pull requests.