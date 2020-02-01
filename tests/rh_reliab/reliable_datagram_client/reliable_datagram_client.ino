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

// select board to set the pinout
// SUPPORTED PLATFORMS: ARDUINO_AVR_NANO, ARDUINO_AVR_MEGA2560, ARDUINO_BLUEPILL_F103C8
#include "platform_def.h"

// radio driver
RH_E32  driver(&Serial3, E32_M0_PIN, E32_M1_PIN, E32_AUX_PIN); // m0,m1,aux

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, CLIENT_ADDRESS);

void setup() 
{
  Serial.begin(9600);
  while (!Serial) ;

  pinMode(LED_BUILTIN,OUTPUT); // led pin

  Serial3.begin(9600);
  //while (!Serial3); 
   
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
  else
    Serial.println("Get parameters OK");
    
  Serial.println(my_params.head, HEX);
  Serial.println(my_params.addh, HEX);
  Serial.println(my_params.addl, HEX);
  Serial.println(my_params.sped, HEX);
  Serial.println(my_params.chan, HEX);
  Serial.println(my_params.option, HEX);

  Serial.println("Setup finished!");
}

uint8_t data[] = "Hello World!";
// Dont put this on the stack:
uint8_t buf[RH_E32_MAX_MESSAGE_LEN];
byte cnt_sent=0;
byte cnt_recv=0;

void loop()
{
  sprintf((char*)buf, "Sending msg %02d to e32_reliable_datagram_server",cnt_sent);
  Serial.println((char*)buf);
  // Send a message to e32_server
  sprintf((char*)buf, "%s_%02d",data,cnt_sent);
  //manager.send(buf, strlen((char*)buf));
  cnt_sent = (cnt_sent + 1) % 15;
  
  //manager.waitPacketSent();
  // Now wait for a reply
  //uint8_t len = sizeof(buf);
      
  // Send a message to manager_server
  if (manager.sendtoWait(buf, strlen((char*)buf), SERVER_ADDRESS))
  {
    // Now wait for a reply from the server
    uint8_t len = sizeof(buf);
    uint8_t from;   
    if (manager.recvfromAckTimeout(buf, &len, 2000, &from))
    {
      Serial.print("got reply from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.print((char*)buf);
      Serial.print("_");
      Serial.println(cnt_recv);
      cnt_recv = (cnt_recv + 1) % 15;      
      // blink led when client receives a reply
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);      
    }
    else
    {
      Serial.println("No reply, is e32_reliable_datagram_server running?");
    }
  }
  else
    Serial.println("sendtoWait failed");

  // uncomment this to see if there are retransmissions wasting energy and bandwidth
  // perhaps the timeout must be increased to reduce retransmissions
  Serial.print("retrasmissions: ");
  Serial.println(manager.retransmissions());

  // apparently it is necessary to clear this buffer from time to time. otherwise it 
  // starts to accumulate errors and it stops working
  driver.clearRxBuf();

  // generates some random delay from 1000 to 2000
  int temps=(analogRead(1)%100)*10+1000;
  delay(temps);
}
