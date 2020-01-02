
// select board to set the pinout
//#define ARDUINO_NANO
//#define ARDUINO_MEGA
#define BLUEPILL_F103C8

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
// requires https://github.com/stm32duino/wiki/wiki
  #define RX_PIN PB11  // not used. Using Serial3
  #define TX_PIN PB10  // not used. Using Serial3
  #define E32_AUX_PIN 2 // not used
  #define E32_M0_PIN PC14
  #define E32_M1_PIN PC15
#endif


#if defined(ARDUINO_NANO)
SoftwareSerial Serial3(RX_PIN, TX_PIN); //rx , tx
#endif
#if defined(BLUEPILL_F103C8)
HardwareSerial Serial3(USART3); //on PB11 PB10
#endif

byte cnt;
byte buff[20];

void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }  
  
  Serial3.begin(9600);
  cnt=0;
  pinMode(E32_M0_PIN,OUTPUT); // m0
  pinMode(E32_M1_PIN,OUTPUT); // m1


  // get configuration parameters
  digitalWrite(E32_M0_PIN,HIGH);
  digitalWrite(E32_M1_PIN,HIGH);
  buff[0] = 0xc1;
  buff[1] = 0xc1;
  buff[2] = 0xc1;
  Serial3.write(buff,3);
  delay(10);

  while(1){
    if (Serial3.available()){
      buff[0] = Serial3.read();
      Serial.print(buff[0],HEX);
      Serial.print(", ");
      cnt++;
    }
    if (cnt>=6)
          break;
  }
  // expected configuration is 'C0, 0, 0, 1A, 17, 44'. this is the factory configuration

  // return to normal mode
  delay(10);
  digitalWrite(E32_M0_PIN,LOW);
  digitalWrite(E32_M1_PIN,LOW);
  delay(10);

  
}

void loop() {

 Serial3.write(cnt);
 cnt = (cnt + 1) % 0x0F;
 
 delay(500);

}
