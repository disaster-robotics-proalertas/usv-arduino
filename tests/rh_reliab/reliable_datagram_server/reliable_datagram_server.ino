// e32_reliable_datagram_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple addressed, reliable messaging server
// with the RHReliableDatagram class, using the RH_E32 driver to control a E32 radio.
// It is designed to work with the other example e32_reliable_datagram_client
// Tested on nano with EBYTE E32 module

#include <RHReliableDatagram.h>
#include <RH_E32.h>

// select board to set the pinout
//#define ARDUINO_NANO
//#define ARDUINO_MEGA
#define BLUEPILL_F103C8
#include "../../common/platform_def.h"

// radio driver
RH_E32  driver(&Serial3, E32_M0_PIN, E32_M1_PIN, E32_AUX_PIN); // m0,m1,aux

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, SERVER_ADDRESS);

void setup() 
{
  Serial.begin(9600);
  while (!Serial) ;

  Serial3.begin(9600);
  while (!Serial3);
  
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

  Serial.println("Ending setup");
}

uint8_t data[] = "And hello back to you";
// Dont put this on the stack:
uint8_t buf[RH_E32_MAX_MESSAGE_LEN];
uint8_t debugCounter=0;

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
      if (!manager.sendtoWait(data, sizeof(data), from))
        Serial.println("sendtoWait failed");
    }else{
      Serial.println("recvfromAck failed");
    }
  }

  debugCounter= (debugCounter+1) % 51;
  if (debugCounter>=50){
    // uncomment this to see if there are retransmissions wasting energy and bandwidth
    // perhaps the timeout must be increased to reduce retransmissions
    Serial.print("retrasmissions: ");
    Serial.println(manager.retransmissions());  
    driver.clearRxBuf();  
  }  
  delay (100);  
}
