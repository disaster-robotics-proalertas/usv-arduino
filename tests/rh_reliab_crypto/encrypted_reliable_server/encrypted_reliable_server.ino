//  LoRa simple server with encrypted communications 
// In order for this to compile you MUST uncomment the #define RH_ENABLE_ENCRYPTION_MODULE line
// at the bottom of RadioHead.h, AND you MUST have installed the Crypto directory from arduinolibs:
// http://rweather.github.io/arduinolibs/index.html
//  Philippe.Rochat'at'gmail.com
//  06.07.2017

#include <RHReliableDatagram.h>
#include <RH_E32.h>
#include <RHEncryptedDriver.h>
#include <Speck.h>    // Speck cipher http://rweather.github.io/arduinolibs/classSpeck.html
#include "SoftwareSerial.h"

SoftwareSerial mySerial(7, 6); //rx , tx

// radio driver
RH_E32  driver(&mySerial, 4, 5, 8); // m0,m1,aux
Speck myCipher;   // Instanciate a Speck block ciphering
RHEncryptedDriver EDriver(driver, myCipher); // Instantiate the encripted driver

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(EDriver, SERVER_ADDRESS);

unsigned char encryptkey[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}; // The very secret key

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available

  mySerial.begin(9600);

  Serial.println("LoRa E32_Encrypted_Reliable Server");

  myCipher.setKey(encryptkey, sizeof(encryptkey));
  
  if (!manager.init()){
    Serial.println("manager init failed");
  } else {
    Serial.println("manager init succeded");  
  } 
  // 500 was not enough for timeout time with encryption. I had to use more
  manager.setTimeout(1000);
  
  // Defaults parameters must be: C0, 0, 0, 1A, 17, 47

  RH_E32 :: Parameters my_params;
  if (!driver.readParameters(my_params))
    Serial.println("Get parameters failed");

  Serial.println(my_params.head, HEX);
  Serial.println(my_params.addh, HEX);
  Serial.println(my_params.addl, HEX);
  Serial.println(my_params.sped, HEX);
  Serial.println(my_params.chan, HEX);
  Serial.println(my_params.option, HEX);

  Serial.println("Waiting for radio to setup");
  delay(1000);
  Serial.println("Setup completed");
}

uint8_t data[] = "And hello back to you";
// Dont put this on the stack:
uint8_t buf[RH_E32_MAX_MESSAGE_LEN];

void loop()
{
  if (manager.available())
  {
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAck(buf, &len, &from))
    {
      Serial.print("got request from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)buf);

      // Send a reply back to the originator client
      if (!manager.sendtoWait(data, sizeof(data), from)){
        Serial.println("sendtoWait failed");       }
    }else{
      Serial.println("recvfromAck failed");
    }
    // uncomment this to see if there are retransmissions wasting energy and bandwidth
    // perhaps the timeout must be increased to reduce retransmissions
    Serial.print("retrasmissions: ");
    Serial.println(manager.retransmissions());
  }
  // apparently it is necessary to clear this buffer from time to time. otherwise it 
  // starts to accumulate errors and it stops working  
  driver.clearRxBuf();
  delay (100);
}
