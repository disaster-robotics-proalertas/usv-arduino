// e32_mesh_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple addressed, routed reliable messaging client
// with the RHMesh class.
// It is designed to work with the other examples e32_mesh_server*
// Hint: you can simulate other network topologies by setting the 
// RH_TEST_NETWORK define in RHRouter.h
/////////////
// client send a hello to the server and waits for a reply. If the reply is received, the LED blinks
/////////////

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

// In this small artifical network of 4 nodes,
#define CLIENT_ADDRESS 1
#define SERVER1_ADDRESS 2
#define SERVER2_ADDRESS 3
#define SERVER3_ADDRESS 4

// Singleton instance of the radio driver
SoftwareSerial mySerial(7, 6); //rx , tx
RH_E32  driver(&mySerial, 4, 5, 8); // m0,m1,aux

// Class to manage message delivery and receipt, using the driver declared above
RHMesh manager(driver, CLIENT_ADDRESS);

void setup() 
{
  //randomSeed(analogRead(0));

  Serial.begin(9600);
  while (!Serial) ;

  mySerial.begin(9600);

  pinMode(13,OUTPUT); // led pin

  // 500 was not enough for timeout time with encryption. I had to use more
  manager.setTimeout(1000);
  
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



uint8_t data[] = "Hello World!";
// Dont put this on the stack:
uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];
uint8_t debugCounter=0;

void loop()
{
  
  Serial.println("Sending to e32_mesh_server1");
    
  // Send a message to a e32_mesh_server
  // A route to the destination will be automatically discovered.
  uint8_t ret = manager.sendtoWait(data, sizeof(data), SERVER1_ADDRESS);
  if (ret == RH_ROUTER_ERROR_NONE)
  {
    // It has been reliably delivered to the next node.
    // Now wait for a reply from the ultimate server
    uint8_t len = sizeof(buf);
    uint8_t from;    

    //manager.printRoutingTable();
    // listen for incoming messages. Wait a random amount of time before we transmit
    // again to the next node
    //delay(random(1000, 2000));    
    if (manager.recvfromAckTimeout(buf, &len, 3000, &from))
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
      Serial.println("No reply, is e32_mesh_server1 running?");
    }
  }
  else{
    printRoutingError(ret);
  }
  
  debugCounter= (debugCounter+1) % 10;
  if (debugCounter>=9){
	  manager.printRoutingTable();
      // uncomment this to see if there are retransmissions wasting energy and bandwidth
      // perhaps the timeout must be increased to reduce retransmissions
      Serial.print("retrasmissions: ");
      Serial.println(manager.retransmissions());	
      driver.clearRxBuf();  
  }  

  // generates some random delay from 1000 to 2000
  int temps=(analogRead(1)%100)*10+1000;
  delay(temps);
}
