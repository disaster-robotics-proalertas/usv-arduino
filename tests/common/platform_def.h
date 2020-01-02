// HOW TO USE IT: define one of the supported platform in the main source file and include this file
// SUPPORTED PLATFORMS: ARDUINO_NANO, ARDUINO_MEGA, BLUEPILL_F103C8

#ifdef ARDUINO_NANO
  #include "SoftwareSerial.h"
  #define BOARD "arduino_nano"
#endif
#ifdef ARDUINO_MEGA
  #define BOARD "arduino_mega"
#endif
#ifdef BLUEPILL_F103C8
  #define BOARD "STM32F103C8T6 - Blue Pill"
#endif

#ifdef ARDUINO_NANO
  #define RX_PIN 7
  #define TX_PIN 6
  #define E32_AUX_PIN 8
  #define E32_M0_PIN 4
  #define E32_M1_PIN 5
#endif
#ifdef ARDUINO_MEGA
  #define RX_PIN 7  // not used. Using Serial3
  #define TX_PIN 6  // not used. Using Serial3
  #define E32_AUX_PIN 2
  #define E32_M0_PIN 3
  #define E32_M1_PIN 4
#endif
#ifdef BLUEPILL_F103C8
/*
#define RH_PLATFORM_STM32            3
#define RH_PLATFORM_STM32STD         7
#define RH_PLATFORM_STM32F4_HAL      8 
#define RH_PLATFORM_STM32F2          12
MCU_STM32F103RE
ARDUINO_ARCH_STM32F1 // ask.cpp

*/
//#define RH_PLATFORM_STM32
//#define RH_PLATFORM_STM32F2
//#define ARDUINO_ARCH_STM32F1

// stm32duino will define ARDUINO_BLUEPILL_F103C8 automatically 
// requires https://github.com/stm32duino/wiki/wiki
  #define RX_PIN PB11  // not used. Using Serial3
  #define TX_PIN PB10  // not used. Using Serial3
  #define E32_AUX_PIN 2 // not used
  #define E32_M0_PIN PC14
  #define E32_M1_PIN PC15
#endif

#ifdef ARDUINO_NANO
SoftwareSerial Serial3(RX_PIN, TX_PIN); //rx , tx
#endif
#if defined(BLUEPILL_F103C8)
HardwareSerial Serial3(USART3); //on PB11 PB10
#endif
