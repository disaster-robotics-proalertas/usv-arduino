// client send a hello to the server and waits for a reply. If the reply is received, the LED blinks 
#include <RH_E32.h>
#include "platform_def.h"

RH_E32  driver(&Serial3, E32_M0_PIN, E32_M1_PIN, E32_AUX_PIN); 

void setup() 
{
  Serial.begin(9600);
  while (!Serial) ;

  pinMode(LED_BUILTIN,OUTPUT); // led pin

  Serial3.begin(9600); 

  // init is equivalent to: getVersion, readParameters, C2, 
  if (!driver.init()) {
        Serial.println("init failed");  
  } else {
        Serial.println("init succeded");  
  }

  //if (!driver.getVersion())
  //  Serial.println("Get version failed"); 

  RH_E32 :: Parameters my_params;
  // these command bellow are protected in the original RH distribution. just comment the line with 'protected' to enable compilation
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
  if (!driver.setPower(RH_E32::Power30dBm))
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
  Serial.println("Setup finished!");

}

uint8_t data[] = "Hello World!";
uint8_t buf[RH_E32_MAX_MESSAGE_LEN];
byte cnt_sent=0;
byte cnt_recv=0;

void loop() 
{
  sprintf((char*)buf, "Sending msg %02d to e32_server",cnt_sent);
  Serial.println((char*)buf);
  // Send a message to e32_server
  sprintf((char*)buf, "%s_%02d",data,cnt_sent);
  driver.send(buf, strlen((char*)buf));
  cnt_sent = (cnt_sent + 1) % 15;
  
  driver.waitPacketSent();
  // Now wait for a reply
  uint8_t len = sizeof(buf);

  if (driver.waitAvailableTimeout(1000)) // At 1kbps, reply can take a long time
  { 
    // Should be a reply message for us now   
    if (driver.recv(buf, &len))
    {
      Serial.print("got reply: ");
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
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is e32_server running?");
  }
  delay(1000);
}
