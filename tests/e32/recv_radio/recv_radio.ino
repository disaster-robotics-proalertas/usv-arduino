
// select board to set the pinout
#define ARDUINO_NANO
//#define ARDUINO_MEGA

#ifdef ARDUINO_NANO
  #include "SoftwareSerial.h"
  #define BOARD "arduino_nano"
#endif
#ifdef ARDUINO_MEGA
  #define BOARD "arduino_mega"
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

#if defined(ARDUINO_NANO)
SoftwareSerial Serial3(RX_PIN, TX_PIN); //rx , tx
#endif

byte c;
byte buff[20];

void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }    

  Serial3.begin(9600);
  pinMode(E32_AUX_PIN,INPUT);  // aux pin
  pinMode(13,OUTPUT); // led pin
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

  c=0;
  while(1){
    if (Serial3.available()){
      buff[0] = Serial3.read();
      Serial.print(buff[0],HEX);
      Serial.print(", ");
      c++;
    }
    if (c>=6)
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

 if (Serial3.available()) {
   c = Serial3.read();
   Serial.println(c);
   
   digitalWrite(13, HIGH);
   delay(200);
   digitalWrite(13, LOW);
   delay(200);
 }

}
