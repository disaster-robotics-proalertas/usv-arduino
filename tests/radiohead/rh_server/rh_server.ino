#include <RH_E32.h>
#include "SoftwareSerial.h"

SoftwareSerial mySerial(7, 6); //rx , tx
RH_E32  driver(&mySerial, 4, 5, 8); // m0,m1,aux

void setup() 
{
  Serial.begin(9600);
  while (!Serial) ;

  mySerial.begin(9600); 

  if (!driver.init()) {
        Serial.println("init failed");  
  } else {
        Serial.println("init succeded");  
  }

  //if (!driver.getVersion())
  //Serial.println("Get version failed"); 

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
/*
  my_params.addh = 15;
  my_params.addl = 15;
  my_params.chan = 24;
  
  if (!driver.writeParameters(my_params))
  Serial.println("Write parameters failed"); 

  if (!driver.setDataRate(RH_E32::DataRate1kbps))
    Serial.println("setDataRate failed"); 
  if (!driver.setPower(RH_E32::Power21dBm))
    Serial.println("setPower failed"); 
  if (!driver.setFrequency(434))
    Serial.println("setFrequency failed");

  //Transmit header preset:
 driver.print_tx_header();
 
 uint8_t rx_ADDH = 15;
 uint8_t rx_ADDL = 15;
 uint8_t rx_freq = 24;
 
 uint8_t tx_ADDH = my_params.addh;
 uint8_t tx_ADDL = my_params.addl;
 uint8_t tx_freq = my_params.chan;
 if (!driver.set_tx_header(rx_ADDH,rx_ADDL,rx_freq,tx_ADDH,tx_ADDL,rx_freq)){
      Serial.println("set_tx_header failed"); 
 } else {
        Serial.println("set_tx_header OK"); 
 }
 driver.print_tx_header();   
 */
}
void loop() 
{
  if (driver.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_E32_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (driver.recv(buf, &len))
    {
//      RH_E32::printBuffer("request: ", buf, len);
      Serial.print("got request: ");
      Serial.println((char*)buf);

      // Send a reply
      uint8_t data[] = "And hello back to you";
      driver.send(data, sizeof(data));
      driver.waitPacketSent();
      Serial.println("Sent a reply");
    }
    else
    {
      Serial.println("recv failed");
    }
  }
}
