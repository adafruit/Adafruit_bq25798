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
  
  Serial.println();
  
  // Test automatic OVP battery discharge functions
  // bq.setAutoOVPBattDischarge(false); // Uncomment to disable if desired
  Serial.print(F("Auto OVP battery discharge enabled: "));
  Serial.println(bq.getAutoOVPBattDischarge() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test force battery discharge functions
  // bq.setForceBattDischarge(true); // Uncomment to force discharge if desired
  Serial.print(F("Force battery discharge enabled: "));
  Serial.println(bq.getForceBattDischarge() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test charge enable functions
  // bq.setChargeEnable(false); // Uncomment to disable charging if desired
  Serial.print(F("Charge enabled: "));
  Serial.println(bq.getChargeEnable() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test ICO enable functions
  // bq.setICOEnable(false); // Uncomment to disable ICO if desired
  Serial.print(F("ICO enabled: "));
  Serial.println(bq.getICOEnable() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test force ICO functions
  // bq.setForceICO(true); // Uncomment to force ICO if desired
  Serial.print(F("Force ICO enabled: "));
  Serial.println(bq.getForceICO() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test HIZ mode functions
  // bq.setHIZMode(true); // Uncomment to enable HIZ mode if desired
  Serial.print(F("HIZ mode enabled: "));
  Serial.println(bq.getHIZMode() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test termination enable functions
  // bq.setTerminationEnable(false); // Uncomment to disable termination if desired
  Serial.print(F("Termination enabled: "));
  Serial.println(bq.getTerminationEnable() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test backup mode enable functions
  // bq.setBackupModeEnable(true); // Uncomment to enable backup mode if desired
  Serial.print(F("Backup mode enabled: "));
  Serial.println(bq.getBackupModeEnable() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test backup mode threshold functions
  // bq.setBackupModeThresh(BQ25798_VBUS_BACKUP_60_PERCENT); // Uncomment to set threshold if desired
  Serial.print(F("Current backup mode threshold: "));
  bq25798_vbus_backup_t currentBackupThresh = bq.getBackupModeThresh();
  switch(currentBackupThresh) {
    case BQ25798_VBUS_BACKUP_40_PERCENT: Serial.println(F("40% of VINDPM")); break;
    case BQ25798_VBUS_BACKUP_60_PERCENT: Serial.println(F("60% of VINDPM")); break;
    case BQ25798_VBUS_BACKUP_80_PERCENT: Serial.println(F("80% of VINDPM")); break;
    case BQ25798_VBUS_BACKUP_100_PERCENT: Serial.println(F("100% of VINDPM")); break;
  }
  
  Serial.println();
  
  // Test VAC OVP functions
  // bq.setVACOVP(BQ25798_VAC_OVP_12V); // Uncomment to set VAC OVP if desired
  Serial.print(F("Current VAC OVP threshold: "));
  bq25798_vac_ovp_t currentVACOVP = bq.getVACOVP();
  switch(currentVACOVP) {
    case BQ25798_VAC_OVP_26V: Serial.println(F("26V")); break;
    case BQ25798_VAC_OVP_22V: Serial.println(F("22V")); break;
    case BQ25798_VAC_OVP_12V: Serial.println(F("12V")); break;
    case BQ25798_VAC_OVP_7V: Serial.println(F("7V")); break;
  }
  
  Serial.println();
  
  // Test watchdog reset function
  // bq.resetWDT(); // Uncomment to reset watchdog if desired
  Serial.println(F("Watchdog reset function available"));
  
  Serial.println();
  
  // Test watchdog timer functions
  // bq.setWDT(BQ25798_WDT_20S); // Uncomment to set watchdog timer if desired
  Serial.print(F("Current watchdog timer: "));
  bq25798_wdt_t currentWDT = bq.getWDT();
  switch(currentWDT) {
    case BQ25798_WDT_DISABLE: Serial.println(F("Disabled")); break;
    case BQ25798_WDT_0_5S: Serial.println(F("0.5 seconds")); break;
    case BQ25798_WDT_1S: Serial.println(F("1 second")); break;
    case BQ25798_WDT_2S: Serial.println(F("2 seconds")); break;
    case BQ25798_WDT_20S: Serial.println(F("20 seconds")); break;
    case BQ25798_WDT_40S: Serial.println(F("40 seconds")); break;
    case BQ25798_WDT_80S: Serial.println(F("80 seconds")); break;
    case BQ25798_WDT_160S: Serial.println(F("160 seconds")); break;
  }
  
  Serial.println();
  
  // Test force D+/D- pins detection functions
  // bq.setForceDPinsDetection(true); // Uncomment to force D+/D- detection if desired
  Serial.print(F("Force D+/D- pins detection enabled: "));
  Serial.println(bq.getForceDPinsDetection() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test auto D+/D- pins detection functions
  // bq.setAutoDPinsDetection(false); // Uncomment to disable auto D+/D- detection if desired
  Serial.print(F("Auto D+/D- pins detection enabled: "));
  Serial.println(bq.getAutoDPinsDetection() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test HVDCP 12V enable functions
  // bq.setHVDCP12VEnable(false); // Uncomment to disable HVDCP 12V if desired
  Serial.print(F("HVDCP 12V enabled: "));
  Serial.println(bq.getHVDCP12VEnable() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test HVDCP 9V enable functions
  // bq.setHVDCP9VEnable(false); // Uncomment to disable HVDCP 9V if desired
  Serial.print(F("HVDCP 9V enabled: "));
  Serial.println(bq.getHVDCP9VEnable() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test HVDCP enable functions
  // bq.setHVDCPEnable(false); // Uncomment to disable HVDCP if desired
  Serial.print(F("HVDCP enabled: "));
  Serial.println(bq.getHVDCPEnable() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test ship FET mode functions
  // bq.setShipFETmode(BQ25798_SDRV_SHIP); // Uncomment to set ship mode if desired
  Serial.print(F("Current ship FET mode: "));
  bq25798_sdrv_ctrl_t currentShipMode = bq.getShipFETmode();
  switch(currentShipMode) {
    case BQ25798_SDRV_IDLE: Serial.println(F("IDLE")); break;
    case BQ25798_SDRV_SHUTDOWN: Serial.println(F("Shutdown Mode")); break;
    case BQ25798_SDRV_SHIP: Serial.println(F("Ship Mode")); break;
    case BQ25798_SDRV_SYSTEM_RESET: Serial.println(F("System Power Reset")); break;
  }
  
  Serial.println();
  
  // Test ship FET 10s delay functions
  // bq.setShipFET10sDelay(false); // Uncomment to disable 10s delay if desired
  Serial.print(F("Ship FET 10s delay enabled: "));
  Serial.println(bq.getShipFET10sDelay() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test AC enable functions
  // bq.setACenable(false); // Uncomment to disable AC if desired
  Serial.print(F("AC enabled: "));
  Serial.println(bq.getACenable() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test OTG enable functions
  // bq.setOTGenable(false); // Uncomment to disable OTG if desired
  Serial.print(F("OTG enabled: "));
  Serial.println(bq.getOTGenable() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test OTG PFM functions
  // bq.setOTGPFM(false); // Uncomment to disable OTG PFM if desired
  Serial.print(F("OTG PFM enabled: "));
  Serial.println(bq.getOTGPFM() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test forward PFM functions
  // bq.setForwardPFM(false); // Uncomment to disable forward PFM if desired
  Serial.print(F("Forward PFM enabled: "));
  Serial.println(bq.getForwardPFM() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test ship wakeup delay functions
  // bq.setShipWakeupDelay(BQ25798_WKUP_DLY_15MS); // Uncomment to set wakeup delay if desired
  Serial.print(F("Current ship wakeup delay: "));
  bq25798_wkup_dly_t currentWakeupDelay = bq.getShipWakeupDelay();
  switch(currentWakeupDelay) {
    case BQ25798_WKUP_DLY_1S: Serial.println(F("1 second")); break;
    case BQ25798_WKUP_DLY_15MS: Serial.println(F("15ms")); break;
  }
  
  Serial.println();
  
  // Test BATFET LDO precharge functions
  // bq.setBATFETLDOprecharge(false); // Uncomment to disable BATFET LDO precharge if desired
  Serial.print(F("BATFET LDO precharge enabled: "));
  Serial.println(bq.getBATFETLDOprecharge() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test OTG OOA functions
  // bq.setOTGOOA(false); // Uncomment to disable OTG OOA if desired
  Serial.print(F("OTG OOA enabled: "));
  Serial.println(bq.getOTGOOA() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test forward OOA functions
  // bq.setForwardOOA(false); // Uncomment to disable forward OOA if desired
  Serial.print(F("Forward OOA enabled: "));
  Serial.println(bq.getForwardOOA() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test ACDRV2 enable functions
  // bq.setACDRV2enable(false); // Uncomment to disable ACDRV2 if desired
  Serial.print(F("ACDRV2 enabled: "));
  Serial.println(bq.getACDRV2enable() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test ACDRV1 enable functions
  // bq.setACDRV1enable(false); // Uncomment to disable ACDRV1 if desired
  Serial.print(F("ACDRV1 enabled: "));
  Serial.println(bq.getACDRV1enable() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test PWM frequency functions
  // bq.setPWMFrequency(BQ25798_PWM_FREQ_750KHZ); // Uncomment to set PWM frequency if desired
  Serial.print(F("Current PWM frequency: "));
  bq25798_pwm_freq_t currentPWMFreq = bq.getPWMFrequency();
  switch(currentPWMFreq) {
    case BQ25798_PWM_FREQ_1_5MHZ: Serial.println(F("1.5 MHz")); break;
    case BQ25798_PWM_FREQ_750KHZ: Serial.println(F("750 kHz")); break;
  }
  
  Serial.println();
  
  // Test STAT pin enable functions
  // bq.setStatPinEnable(false); // Uncomment to disable STAT pin if desired
  Serial.print(F("STAT pin enabled: "));
  Serial.println(bq.getStatPinEnable() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test VSYS short protection functions
  // bq.setVSYSshortProtect(false); // Uncomment to disable VSYS short protection if desired
  Serial.print(F("VSYS short protection enabled: "));
  Serial.println(bq.getVSYSshortProtect() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test VOTG UVP protection functions
  // bq.setVOTG_UVPProtect(false); // Uncomment to disable VOTG UVP protection if desired
  Serial.print(F("VOTG UVP protection enabled: "));
  Serial.println(bq.getVOTG_UVPProtect() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test IBUS OCP enable functions
  // bq.setIBUS_OCPenable(false); // Uncomment to disable IBUS OCP if desired
  Serial.print(F("IBUS OCP enabled: "));
  Serial.println(bq.getIBUS_OCPenable() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test VINDPM detection functions
  // bq.setVINDPMdetection(false); // Uncomment to disable VINDPM detection if desired
  Serial.print(F("VINDPM detection enabled: "));
  Serial.println(bq.getVINDPMdetection() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test ship FET present functions
  // bq.setShipFETpresent(false); // Uncomment to disable ship FET present if desired
  Serial.print(F("Ship FET present: "));
  Serial.println(bq.getShipFETpresent() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test battery discharge sense enable functions
  // bq.setBatDischargeSenseEnable(false); // Uncomment to disable battery discharge sense if desired
  Serial.print(F("Battery discharge sense enabled: "));
  Serial.println(bq.getBatDischargeSenseEnable() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test battery discharge current regulation functions
  // bq.setBatDischargeA(BQ25798_IBAT_REG_4A); // Uncomment to set discharge current if desired
  Serial.print(F("Current battery discharge regulation: "));
  bq25798_ibat_reg_t currentBatDischarge = bq.getBatDischargeA();
  switch(currentBatDischarge) {
    case BQ25798_IBAT_REG_3A: Serial.println(F("3A")); break;
    case BQ25798_IBAT_REG_4A: Serial.println(F("4A")); break;
    case BQ25798_IBAT_REG_5A: Serial.println(F("5A")); break;
    case BQ25798_IBAT_REG_DISABLE: Serial.println(F("Disabled")); break;
  }
  
  Serial.println();
  
  // Test IINDPM enable functions
  // bq.setIINDPMenable(false); // Uncomment to disable IINDPM if desired
  Serial.print(F("IINDPM enabled: "));
  Serial.println(bq.getIINDPMenable() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test external ILIM pin functions
  // bq.setExtILIMpin(false); // Uncomment to disable external ILIM pin if desired
  Serial.print(F("External ILIM pin enabled: "));
  Serial.println(bq.getExtILIMpin() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test battery discharge OCP enable functions
  // bq.setBatDischargeOCPenable(false); // Uncomment to disable battery discharge OCP if desired
  Serial.print(F("Battery discharge OCP enabled: "));
  Serial.println(bq.getBatDischargeOCPenable() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test VINDPM VOC percentage functions
  // bq.setVINDPM_VOCpercent(BQ25798_VOC_PCT_75); // Uncomment to set VOC percentage if desired
  Serial.print(F("Current VINDPM VOC percentage: "));
  bq25798_voc_pct_t currentVOCpct = bq.getVINDPM_VOCpercent();
  switch(currentVOCpct) {
    case BQ25798_VOC_PCT_56_25: Serial.println(F("56.25%")); break;
    case BQ25798_VOC_PCT_62_5: Serial.println(F("62.5%")); break;
    case BQ25798_VOC_PCT_68_75: Serial.println(F("68.75%")); break;
    case BQ25798_VOC_PCT_75: Serial.println(F("75%")); break;
    case BQ25798_VOC_PCT_81_25: Serial.println(F("81.25%")); break;
    case BQ25798_VOC_PCT_87_5: Serial.println(F("87.5%")); break;
    case BQ25798_VOC_PCT_93_75: Serial.println(F("93.75%")); break;
    case BQ25798_VOC_PCT_100: Serial.println(F("100%")); break;
  }
  
  Serial.println();
  
  // Test VOC delay functions
  // bq.setVOCdelay(BQ25798_VOC_DLY_2S); // Uncomment to set VOC delay if desired
  Serial.print(F("Current VOC delay: "));
  bq25798_voc_dly_t currentVOCdelay = bq.getVOCdelay();
  switch(currentVOCdelay) {
    case BQ25798_VOC_DLY_50MS: Serial.println(F("50ms")); break;
    case BQ25798_VOC_DLY_300MS: Serial.println(F("300ms")); break;
    case BQ25798_VOC_DLY_2S: Serial.println(F("2 seconds")); break;
    case BQ25798_VOC_DLY_5S: Serial.println(F("5 seconds")); break;
  }
  
  Serial.println();
  
  // Test VOC rate functions
  // bq.setVOCrate(BQ25798_VOC_RATE_10MIN); // Uncomment to set VOC rate if desired
  Serial.print(F("Current VOC measurement rate: "));
  bq25798_voc_rate_t currentVOCrate = bq.getVOCrate();
  switch(currentVOCrate) {
    case BQ25798_VOC_RATE_30S: Serial.println(F("30 seconds")); break;
    case BQ25798_VOC_RATE_2MIN: Serial.println(F("2 minutes")); break;
    case BQ25798_VOC_RATE_10MIN: Serial.println(F("10 minutes")); break;
    case BQ25798_VOC_RATE_30MIN: Serial.println(F("30 minutes")); break;
  }
  
  Serial.println();
  
  // Test MPPT enable functions
  // bq.setMPPTenable(false); // Uncomment to disable MPPT if desired
  Serial.print(F("MPPT enabled: "));
  Serial.println(bq.getMPPTenable() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test thermal regulation threshold functions
  // bq.setThermRegulationThresh(BQ25798_TREG_100C); // Uncomment to set thermal regulation threshold if desired
  Serial.print(F("Current thermal regulation threshold: "));
  bq25798_treg_t currentTREG = bq.getThermRegulationThresh();
  switch(currentTREG) {
    case BQ25798_TREG_60C: Serial.println(F("60°C")); break;
    case BQ25798_TREG_80C: Serial.println(F("80°C")); break;
    case BQ25798_TREG_100C: Serial.println(F("100°C")); break;
    case BQ25798_TREG_120C: Serial.println(F("120°C")); break;
  }
  
  Serial.println();
  
  // Test thermal shutdown threshold functions
  // bq.setThermShutdownThresh(BQ25798_TSHUT_130C); // Uncomment to set thermal shutdown threshold if desired
  Serial.print(F("Current thermal shutdown threshold: "));
  bq25798_tshut_t currentTSHUT = bq.getThermShutdownThresh();
  switch(currentTSHUT) {
    case BQ25798_TSHUT_150C: Serial.println(F("150°C")); break;
    case BQ25798_TSHUT_130C: Serial.println(F("130°C")); break;
    case BQ25798_TSHUT_120C: Serial.println(F("120°C")); break;
    case BQ25798_TSHUT_85C: Serial.println(F("85°C")); break;
  }
  
  Serial.println();
  
  // Test VBUS pulldown functions
  // bq.setVBUSpulldown(true); // Uncomment to enable VBUS pulldown if desired
  Serial.print(F("VBUS pulldown enabled: "));
  Serial.println(bq.getVBUSpulldown() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test VAC1 pulldown functions
  // bq.setVAC1pulldown(true); // Uncomment to enable VAC1 pulldown if desired
  Serial.print(F("VAC1 pulldown enabled: "));
  Serial.println(bq.getVAC1pulldown() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test VAC2 pulldown functions
  // bq.setVAC2pulldown(true); // Uncomment to enable VAC2 pulldown if desired
  Serial.print(F("VAC2 pulldown enabled: "));
  Serial.println(bq.getVAC2pulldown() ? F("true") : F("false"));
  
  Serial.println();
  
  // Test backup ACFET1 on functions
  // bq.setBackupACFET1on(true); // Uncomment to turn on backup ACFET1 if desired
  Serial.print(F("Backup ACFET1 on: "));
  Serial.println(bq.getBackupACFET1on() ? F("true") : F("false"));
}

void loop() {
  delay(1000);
}