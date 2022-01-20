#include <Arduino.h>              // include libraries

// Дебагирование с монитором : раскомментить для использования 1 строчку:
#define DEBUG_ENABLE
#ifdef DEBUG_ENABLE
#define DEBUG(x) Serial.print(x)
#define DEBUGln(x) Serial.println(x)
#else
#define DEBUG(x)
#define DEBUGln(x)
#endif

//==== MILLIS TIMER MACRO ====
// воспомогательное макро
// performs the {subsequent} code once and then, if needed, again after each x ms
#define EVERY_MS(x) \
  static uint32_t tmr;\
  bool flg = millis() - tmr >= (x);\
  if (flg) tmr = millis();\
  if (flg)
//===========================

#define BATTERY_MIN_VOLTAGE 3.5   //Volt min.
#define BATTERY_VOLTAGE_1 3.5 // Мигание 1 раз
#define BATTERY_VOLTAGE_2 3.6
#define BATTERY_VOLTAGE_3 3.8
#define BATTERY_VOLTAGE_4 3.9
#define BATTERY_VOLTAGE_5 4.0
#define PIN_BATTERY 9  // Номер пина Адафрута для измерения батарейки
#define PIN_BATTERY_LED LED_BUILTIN  // Номер пина для показа напряжения батарейки

#define BATTERY_PERIOD 5000 //(5 сек) Каждые столько миллисекунд измеряется напряжение батареи 

float batteryVoltageMultiplier = 1.27;


void setup() {
#ifdef DEBUG_ENABLE
  Serial.begin(9600);
  while (!Serial);
#endif

  DEBUGln(F("Проверка платы BSFrance LoRa32u4"));
  DEBUGln(F("Проверка измерителя напряжения батареи"));

  Serial.println(DDRD, BIN);    //100000
  Serial.println(PORTD, BIN);   //100000
  Serial.println(PIND, BIN);    //110000
  DDRD = DDRD | 100000;
  PORTD = B111111;
  Serial.println(DDRD, BIN);
  Serial.println(PORTD, BIN);
  Serial.println(PIND, BIN);
  
  while (1) {
    EVERY_MS(BATTERY_PERIOD) {
      showBatteryVoltage();
    }
  }
}////main()

void loop() {

}

void showBatteryVoltage() {
  float voltage = batteryVoltage();
  //  delay(1000);
  if (voltage > BATTERY_VOLTAGE_1)   flashBatteryLEDOnce(); //1 раз
  if (voltage > BATTERY_VOLTAGE_2)   flashBatteryLEDOnce(); //2 раз
  if (voltage > BATTERY_VOLTAGE_3)   flashBatteryLEDOnce(); //3 раз
  if (voltage > BATTERY_VOLTAGE_4)   flashBatteryLEDOnce(); //4 раз
  if (voltage > BATTERY_VOLTAGE_5)   flashBatteryLEDOnce(); //5 раз
}

void flashBatteryLEDOnce() {
  digitalWrite(PIN_BATTERY_LED, 1);
  delay(200);
  digitalWrite(PIN_BATTERY_LED, 0);
  delay(200);
}

float batteryVoltage() {
  float measuredvbat = analogRead(PIN_BATTERY);
  measuredvbat = analogRead(PIN_BATTERY);
  measuredvbat *= batteryVoltageMultiplier;    // multiply according to the used board divider
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  DEBUG(F("Battery Voltage: "));
  DEBUGln(measuredvbat);
  return measuredvbat;
}////batteryVoltage()

void flashLedBattery(byte times) { //flash "times" times
  DEBUGln(F("flashLedBattery()"));
  bool flash = true;
  for (int i = 0; i < times * 2; i++) {
    digitalWrite(PIN_BATTERY_LED, flash);
    flash = !flash;
    delay(200);
  }
}
