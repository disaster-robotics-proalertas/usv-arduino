/*
  Power control board
  by Guilherme Heck
  guilherme.heck@acad.pucrs.br
  guilherme.heck@edu.pucrs.br
  last update: 04/10/2019
 */

#include"PWM.hpp"
PWM my_pwm(2);

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int GPIO = 9;
int button = 11;
int led = 10;
int led_onboard = 13;
int rele = 8;
int buttonstate = LOW;
int remote = 1500;
int relestate = LOW;
int cont = 0;



// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  pinMode(GPIO, OUTPUT);
  pinMode(rele, OUTPUT);
  pinMode(button, INPUT);
  digitalWrite(button, LOW);
  digitalWrite(rele, LOW);
  digitalWrite(led, LOW);
  digitalWrite(led_onboard, LOW);
//  Serial.begin(9600);
  my_pwm.begin(1);
}

// the loop routine runs over and over again forever:
void loop() {
  remote = my_pwm.getValue();
  buttonstate = digitalRead(button);
  if (buttonstate == HIGH || remote > 1600){
    relestate = digitalRead(rele);
    switch (relestate) {
     case HIGH: 
        digitalWrite(GPIO, HIGH);
        digitalWrite(led, LOW);
        digitalWrite(led_onboard, LOW);
        delay(500);
        digitalWrite(led, HIGH); 
        digitalWrite(led_onboard,HIGH);
        delay(500);
        remote = my_pwm.getValue();
        buttonstate = digitalRead(button);
        if (buttonstate == LOW && remote < 1600){
          digitalWrite(GPIO, LOW);
          for(cont=0;cont<60;cont++){
             digitalWrite(led, LOW); 
             digitalWrite(led_onboard, LOW);
             delay(500);
             digitalWrite(led, HIGH); 
             digitalWrite(led_onboard,HIGH);
             delay(500);
          }
        }
        else{
           for(cont=0;cont<3;cont++){
              digitalWrite(led, LOW); 
              digitalWrite(led_onboard, LOW);
              delay(500);
              digitalWrite(led, HIGH); 
              digitalWrite(led_onboard,HIGH);
              delay(500);
           }
           digitalWrite(GPIO, LOW);
           for(cont=0;cont<21;cont++){
              digitalWrite(led, LOW); 
              digitalWrite(led_onboard, LOW);
              delay(500);
              digitalWrite(led, HIGH); 
              digitalWrite(led_onboard,HIGH);
              delay(500);
           }
           if (buttonstate == HIGH || remote > 1600) {
             digitalWrite(led, LOW); 
             digitalWrite(led_onboard, LOW);
             digitalWrite(rele, LOW);
             delay(1000);
             remote = my_pwm.getValue();
             buttonstate = digitalRead(button);
             if (buttonstate == LOW && remote < 1600) {
               digitalWrite(rele, HIGH);
               digitalWrite(led, HIGH); 
               digitalWrite(led_onboard,HIGH);
               delay(500);
               for(cont=0;cont<60;cont++){
                  digitalWrite(led, LOW); 
                  digitalWrite(led_onboard, LOW);
                  delay(500);
                  digitalWrite(led, HIGH); 
                  digitalWrite(led_onboard,HIGH);
                  delay(500);
               }
             }
             else{
               while (buttonstate == HIGH || remote > 1600){
                  digitalWrite(led, LOW); 
                  digitalWrite(led_onboard, LOW);
                  remote = my_pwm.getValue();
                  buttonstate = digitalRead(button);
               }
             }
           }
        }
        break;
    case LOW:
        digitalWrite(rele, HIGH);
        digitalWrite(led, HIGH); 
        digitalWrite(led_onboard,HIGH);
        digitalWrite(GPIO, LOW);
        delay(500);
        for(cont=0;cont<60;cont++){
           digitalWrite(led, LOW); 
           digitalWrite(led_onboard, LOW);
           delay(500);
           digitalWrite(led, HIGH); 
           digitalWrite(led_onboard,HIGH);
           delay(500);
        }
        break;
    default:
        break;
    }
  }
}
