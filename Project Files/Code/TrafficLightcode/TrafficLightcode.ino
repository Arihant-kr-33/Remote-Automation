#include "UbidotsESPMQTT.h"
#include <stdio.h>
#define WIFISSID "Arihant KR"                               // Put your WifiSSID here
#define PASSWORD "333333333333"                     // Put your wifi password here
#define TOKEN "BBFF-nPzQ3r0ckEHHb3v3IJpcfkQTXvoFiK"   // Ubidots Token 
#define VARIABLE_TO_SUBSCRIBE "home_bulb"
#define DEVICE_LABEL "esp8266"
Ubidots client(TOKEN);
bool ifon=0;
/****************************************
   Auxiliar Functions
 ****************************************/
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");                                                                        
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  if ((char)payload[0] == '0')
  {
    Serial.println("Since, Output is 0, I'm going to turn off LED");
    ifon=0;
  }
  else if ((char)payload[0] == '1')
  {
    Serial.println("Since, Output is 1, I'm going to turn on LED");
    ifon=1;
  }
  else {
    Serial.print(" Ubidots Value is ");
    Serial.println((char)payload[0]);
  }
}
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(115200);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  client.ubidotsSetBroker("industrial.api.ubidots.com");
  client.setDebug(true);
  client.wifiConnection(WIFISSID, PASSWORD);
  client.begin(callback);
  client.ubidotsSubscribe(DEVICE_LABEL, VARIABLE_TO_SUBSCRIBE);
}

// the loop function runs over and over again forever
void loop() {
  if (!client.connected()) {
    client.reconnect();
    client.ubidotsSubscribe(DEVICE_LABEL, VARIABLE_TO_SUBSCRIBE);
  }
  client.loop();
  if(ifon)
  {
    digitalWrite(D6, HIGH);
  digitalWrite(D7, HIGH);
  digitalWrite(D3, LOW);
  digitalWrite(D0, HIGH);// turn the LED on (HIGH is the voltage level)
  digitalWrite(D5, LOW);
  delay(5000);                       // wait for a second
  digitalWrite(D1, HIGH);// turn the LED on (HIGH is the voltage level)
  digitalWrite(D0, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D4, HIGH);
  delay(5000);
  digitalWrite(D2, HIGH);// turn the LED on (HIGH is the voltage level)
  digitalWrite(D7, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D5, HIGH);
  delay(5000);
  digitalWrite(D4, LOW);
  digitalWrite(D3, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(D2, LOW);
  digitalWrite(D6, HIGH);
  delay(5000);
  }
  else
  {
    digitalWrite(D0,LOW);
    digitalWrite(D1,LOW);
    digitalWrite(D2,LOW);
    digitalWrite(D3,LOW);
    digitalWrite(D4,LOW);
    digitalWrite(D5,LOW);
    digitalWrite(D6,LOW);
    digitalWrite(D7,LOW);
  }
  loop();
  
}
