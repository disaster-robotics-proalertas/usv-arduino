// select board to set the pinout
// SUPPORTED PLATFORMS: ARDUINO_AVR_NANO, ARDUINO_AVR_MEGA2560, ARDUINO_BLUEPILL_F103C8
#include "platform_def.h"

byte c;
byte buff[20];

// E32 is ready to accept commands when E32_AUX_PIN is high, otherwise, just wait
void waitAuxHigh(){
  while(digitalRead(E32_AUX_PIN)== LOW);
  delay(10); // this extra time is required before starting a new command
}

void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }  
  
  Serial3.begin(9600);
  pinMode(E32_AUX_PIN,INPUT); // aux pin
  pinMode(E32_M0_PIN,OUTPUT); // m0
  pinMode(E32_M1_PIN,OUTPUT); // m1

  digitalWrite(E32_M0_PIN,HIGH);
  digitalWrite(E32_M1_PIN,HIGH);

  // write the default configuration into the device. Disable it if you dont want to change the configuration
  delay(10); // delay required by stm32
  buff[0] = 0xc0;
  buff[1] = 0x0;
  buff[2] = 0x0;
  buff[3] = 0x1a;
  buff[4] = 0x17;
  buff[5] = 0x44;
  Serial3.write(buff,6);
  // just discard the data in the input buffer
  c=0;
  while(1){
    if (Serial3.available()){
      buff[0] = Serial3.read();
      c++;
    }
    if (c>=6)
          break;
  }  
  waitAuxHigh();

  // get configuration parameters to check if it was written 
  buff[0] = 0xc1;
  buff[1] = 0xc1;
  buff[2] = 0xc1;
  Serial3.write(buff,3);

  c=0;
  Serial.print("Reading current device configuration: ");
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
  Serial.println("");
  // expected configuration is 'C0, 0, 0, 1A, 17, 44'. this is the factory configuration

  // get the version number
  waitAuxHigh();
  buff[0] = 0xc3;
  buff[1] = 0xc3;
  buff[2] = 0xc3;
  Serial3.write(buff,3);

  c=0;
  Serial.print("Reading device version: ");
  while(1){
    if (Serial3.available()){
      buff[0] = Serial3.read();
      Serial.print(buff[0],HEX);
      Serial.print(", ");
      c++;
    }
    if (c>=4)
          break;
  }
  Serial.println("");
  // expected version number is 'C3 32 47 14'.

  // return to normal mode
  digitalWrite(E32_M0_PIN,LOW);
  digitalWrite(E32_M1_PIN,LOW);
  
  waitAuxHigh(); 
  c=0;
  Serial.println("End setup!");
}

void loop() {

 Serial3.write(c);
 c = (c + 1) % 0x0F;
 
 delay(500);

}
