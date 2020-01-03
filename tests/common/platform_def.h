// HOW TO USE IT: when you select a board in the Arduino IDE, these symbols are automatically created. no need to redefine them
// SUPPORTED PLATFORMS: ARDUINO_AVR_NANO, ARDUINO_AVR_MEGA2560, ARDUINO_BLUEPILL_F103C8

#ifdef ARDUINO_AVR_NANO
  #include "SoftwareSerial.h"
  #define BOARD "arduino_nano"
#endif
#ifdef ARDUINO_AVR_MEGA2560
  #define BOARD "arduino_mega"
#endif
#ifdef ARDUINO_BLUEPILL_F103C8
  #define BOARD "STM32F103C8T6 - Blue Pill"
#endif

#ifdef ARDUINO_AVR_NANO
  #define RX_PIN 7
  #define TX_PIN 6
  #define E32_AUX_PIN 8
  #define E32_M0_PIN 4
  #define E32_M1_PIN 5
#endif
#ifdef ARDUINO_AVR_MEGA2560
  #define RX_PIN 7  // not used. Using Serial3
  #define TX_PIN 6  // not used. Using Serial3
  #define E32_AUX_PIN 2
  #define E32_M0_PIN 3
  #define E32_M1_PIN 4
#endif
#ifdef ARDUINO_BLUEPILL_F103C8
  #define RX_PIN PB11  // not used. Using Serial3
  #define TX_PIN PB10  // not used. Using Serial3
  #define E32_AUX_PIN 2 // not used
  #define E32_M0_PIN PC14
  #define E32_M1_PIN PC15
#endif

#ifdef ARDUINO_AVR_NANO
SoftwareSerial Serial3(RX_PIN, TX_PIN); //rx , tx
#endif
#if defined(ARDUINO_BLUEPILL_F103C8)
HardwareSerial Serial3(USART3); //on PB11 PB10
#endif
