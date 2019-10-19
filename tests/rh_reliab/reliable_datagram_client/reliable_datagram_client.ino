// e32_reliable_datagram_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple addressed, reliable messaging client
// with the RHReliableDatagram class, using the RH_E32 driver to control a E32 radio.
// It is designed to work with the other example e32_reliable_datagram_server
// Tested on nano with EBYTE E32 module


/*
https://www.airspayce.com/mikem/arduino/RadioHead/serial_gateway_8pde-example.html
https://www.airspayce.com/mikem/arduino/RadioHead/RasPiRH_8cpp-example.html
https://github.com/hallard/RadioHead -- fork for rpi
 - python wrapper for radiohead
 https://pypi.org/project/raspi-lora/
 https://github.com/exmorse/pyRadioHeadRF95
 https://github.com/exmorse/pyRadioHeadNRF24
https://www.airspayce.com/mikem/arduino/RadioHead/serial_reliable_datagram_client_8pde-example.html
https://www.airspayce.com/mikem/arduino/RadioHead/serial_reliable_datagram_server_8pde-example.html
*/

#include <RHReliableDatagram.h>
#include <RH_E32.h>
#include "SoftwareSerial.h"

SoftwareSerial mySerial(7, 6); //rx , tx
// radio driver
RH_E32  driver(&mySerial, 4, 5, 8); // m0,m1,aux

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, CLIENT_ADDRESS);

void setup() 
{
  Serial.begin(9600);
  while (!Serial) ;

  pinMode(13,OUTPUT); // led pin

  mySerial.begin(9600);
  
  if (!manager.init()){
    Serial.println("manager init failed");
  } else {
    Serial.println("manager init succeded");  
  } 

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

  Serial.println("Ending setup");
}

uint8_t data[] = "Hello World!";
// Dont put this on the stack:
uint8_t buf[RH_E32_MAX_MESSAGE_LEN];

void loop()
{
  Serial.println("Sending to e32_reliable_datagram_server");
    
  // Send a message to manager_server
  if (manager.sendtoWait(data, sizeof(data), SERVER_ADDRESS))
  {
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
      Serial.println("No reply, is e32_reliable_datagram_server running?");
    }
  }
  else
    Serial.println("sendtoWait failed");
  delay(500);
}
