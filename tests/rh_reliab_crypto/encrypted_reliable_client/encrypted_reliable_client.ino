// LoRa Simple Hello World Client with encrypted communications 
// In order for this to compile you MUST uncomment the #define RH_ENABLE_ENCRYPTION_MODULE line
// at the bottom of RadioHead.h, AND you MUST have installed the Crypto directory from arduinolibs:
// http://rweather.github.io/arduinolibs/index.html
//  Philippe.Rochat'at'gmail.com
//  06.07.2017


//same error
// https://forum.arduino.cc/index.php?topic=584164.0
// most similar example
// https://www.airspayce.com/mikem/arduino/RadioHead/serial_encrypted_reliable_datagram_client_8pde-example.html
// mqtt radiohead gateway
// https://github.com/bergos/esp-radiohead-gateway
// rpi rh mqtt
// https://github.com/S3ler/mqtt-sn-sockets
// https://github.com/S3ler/ArduinoSockets
// https://github.com/S3ler/RaspberryPiSockets
// python wrappper for radiohead
// https://github.com/exmorse/pyRadioHeadRF95

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
RHReliableDatagram manager(EDriver, CLIENT_ADDRESS);

unsigned char encryptkey[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}; // The very secret key

void setup()
{
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available

  pinMode(13,OUTPUT); // led pin

  mySerial.begin(9600);

  Serial.println("LoRa E32_Encrypted_Reliable Client");

  myCipher.setKey(encryptkey, sizeof(encryptkey));
  
  if (!manager.init()){
    Serial.println("manager init failed");
  } else {
    Serial.println("manager init succeded");  
  } 
  // 500 was not enough for timeout time with encryption. I had to use more
  manager.setTimeout(800);

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

uint8_t data[] = "Hello World! can you read me?";
// Dont put this on the stack:
uint8_t buf[RH_E32_MAX_MESSAGE_LEN];

void loop()
{
  Serial.println("Sending to e32_encrypted_reliable_datagram_server");
    
  // Send a message to manager_server
  if (manager.sendtoWait(data, sizeof(data), SERVER_ADDRESS))
  {
    //EDriver.waitPacketSent();
    // Now wait for a reply from the server
    uint8_t len = sizeof(buf);
    uint8_t from;   
    if (manager.recvfromAckTimeout(buf, &len, 2000, &from))
    {
      Serial.print("got reply from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)buf);
      // blink led when client receives a reply
      digitalWrite(13, HIGH);
      delay(200);
      digitalWrite(13, LOW);      
    }
    else
    {
      Serial.println("No reply, is e32_encrypted_reliable_datagram_server running?");
    }
  }
  else{
    Serial.println("sendtoWait failed");

  }
  // uncomment this to see if there are retransmissions wasting energy and bandwidth
  // perhaps the timeout must be increased to reduce retransmissions
  //Serial.print("retrasmissions: ");
  //Serial.println(manager.retransmissions());    
  delay(4000);
}
