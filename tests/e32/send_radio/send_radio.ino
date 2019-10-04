#include <SoftwareSerial.h>

//SoftwareSerial mySerial(2, 3); // RX, TX
SoftwareSerial mySerial(7, 6); //rx , tx
byte cnt;
byte buff[20];

void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }  
  
  mySerial.begin(9600);
  cnt=0;
  pinMode(4,OUTPUT); // m0
  pinMode(5,OUTPUT); // m1
  
  // get configuration parameters
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  buff[0] = 0xc1;
  buff[1] = 0xc1;
  buff[2] = 0xc1;
  mySerial.write(buff,3);
  delay(10);

  while(1){
    if (mySerial.available()){
      buff[0] = mySerial.read();
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
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  delay(10);

  
}

void loop() {

 mySerial.write(cnt);
 cnt = (cnt + 1) % 0x0F;
 
 delay(500);

}


