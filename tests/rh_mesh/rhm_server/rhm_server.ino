// e32_mesh_server1.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple addressed, routed reliable messaging server
// with the RHMesh class.
// It is designed to work with the other examples e32_mesh_*
// Hint: you can simulate other network topologies by setting the 
// RH_TEST_NETWORK define in RHRouter.h

// Mesh has much greater memory requirements, and you may need to limit the
// max message length to prevent wierd crashes
//RH_E32_h
//#define RH_E32_MAX_PAYLOAD_LEN 50
//RHDatagram_h
//#define RH_MAX_MESSAGE_LEN 50
//RHRouter_h
//#define RH_DEFAULT_MAX_HOPS 10
//#define RH_ROUTING_TABLE_SIZE 5

#include <RHMesh.h>
#include <RH_E32.h>
#include "SoftwareSerial.h"

// select board to set the pinout
// SUPPORTED PLATFORMS: ARDUINO_AVR_NANO, ARDUINO_AVR_MEGA2560, ARDUINO_BLUEPILL_F103C8
#include "platform_def.h"

// In this small artifical network of 4 nodes,
#define CLIENT_ADDRESS 1
#define SERVER1_ADDRESS 2
#define SERVER2_ADDRESS 3
#define SERVER3_ADDRESS 4

// radio driver
RH_E32  driver(&Serial3, E32_M0_PIN, E32_M1_PIN, E32_AUX_PIN); // m0,m1,aux

// Class to manage message delivery and receipt, using the driver declared above
RHMesh manager(driver, SERVER1_ADDRESS);

void setup() 
{
  //randomSeed(analogRead(0));
  
  Serial.begin(9600);
  while (!Serial) ;

  Serial3.begin(9600); 

  // 500 was not enough for timeout time with encryption. I had to use more
  manager.setTimeout(1000);
    
  if (!manager.init()){
        Serial.println("init failed");  
  } else {
        Serial.println("init succeded");  
  }
  
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

// help to debug routing errors
void printRoutingError(uint8_t ret){
switch(ret){
  case RH_ROUTER_ERROR_NONE:
    Serial.println("route ok");
    break;
  case RH_ROUTER_ERROR_INVALID_LENGTH:
    Serial.println("invalid lenght");
    break;
  case RH_ROUTER_ERROR_NO_ROUTE:
    Serial.println("no route");
    break;
  case RH_ROUTER_ERROR_TIMEOUT:
    Serial.println("timeout error");
    break;
  case RH_ROUTER_ERROR_NO_REPLY:
    Serial.println("no reply");
    break;
  case RH_ROUTER_ERROR_UNABLE_TO_DELIVER:
    Serial.println("unable to deliver");
    break;
  default:
    Serial.println("unknown routing error");
    break;
  }
}


uint8_t data[] = "hello back from server1";
// Dont put this on the stack:
uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];
uint8_t debugCounter=0;

void loop()
{
  uint8_t len = sizeof(buf);
  uint8_t from;
  if (manager.recvfromAck(buf, &len, &from))
  {
    Serial.print("got request from : 0x");
    Serial.print(from, HEX);
    Serial.print(": ");
    Serial.println((char*)buf);

    //manager.printRoutingTable();

    // listen for incoming messages. Wait a random amount of time before we transmit
    // again to the next node
    //delay(random(3000, 5000));
    // Send a reply back to the originator client
    printRoutingError(manager.sendtoWait(data, sizeof(data), from));
  }
  debugCounter= (debugCounter+1) % 100;
  if (debugCounter>=99){
	  manager.printRoutingTable();
    // uncomment this to see if there are retransmissions wasting energy and bandwidth
    // perhaps the timeout must be increased to reduce retransmissions
    Serial.print("retrasmissions: ");
    Serial.println(manager.retransmissions());	
    driver.clearRxBuf();  
  }
  delay (100);
}
