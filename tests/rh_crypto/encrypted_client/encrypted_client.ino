// LoRa Simple Hello World Client with encrypted communications 
// In order for this to compile you MUST uncomment the #define RH_ENABLE_ENCRYPTION_MODULE line
// at the bottom of RadioHead.h, AND you MUST have installed the Crypto directory from arduinolibs:
// http://rweather.github.io/arduinolibs/index.html
//  Philippe.Rochat'at'gmail.com
//  06.07.2017

#include <RH_E32.h>
#include <RHEncryptedDriver.h>
#include <Speck.h>   // Speck cipher http://rweather.github.io/arduinolibs/classSpeck.html
#include "SoftwareSerial.h"

SoftwareSerial mySerial(7, 6); //rx , tx
RH_E32  driver(&mySerial, 4, 5, 8); // m0,m1,aux
Speck myCipher;   // Instanciate a Speck block ciphering
RHEncryptedDriver myDriver(driver, myCipher); // Instantiate the driver with those two

unsigned char encryptkey[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}; // The very secret key

void setup()
{
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  
  pinMode(13,OUTPUT); // led pin

  mySerial.begin(9600); 

  Serial.println("LoRa E32_Encrypted Client");

  if (!driver.init()) {
        Serial.println("init failed");  
  } else {
        Serial.println("init succeded");  
  }

  //if (!driver.getVersion())
  //Serial.println("Get version failed"); 

  myCipher.setKey(encryptkey, sizeof(encryptkey));

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

void loop()
{
  Serial.println("Sending to e32_server");
  // Send a message to e32_server
  uint8_t data[] = "Hello World! Can you read me?";
  myDriver.send(data, sizeof(data));
  
  myDriver.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_E32_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (myDriver.waitAvailableTimeout(2000)) // At 1kbps, reply can take a long time
  { 
    // Should be a reply message for us now   
    if (myDriver.recv(buf, &len))
    {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
      // blink led when client receives a reply
      digitalWrite(13, HIGH);
      delay(200);
      digitalWrite(13, LOW);
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
  delay(4000);
}
