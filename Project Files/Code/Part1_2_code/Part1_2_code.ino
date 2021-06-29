#include "UbidotsESPMQTT.h"
#include <DHT.h>
#include <stdio.h>

/****************************************
   Define Constants
 ****************************************/
#define BUZZER D7
#define LDR A0
#define TEMP D5
#define LED D4
#define DHTTYPE DHT11   // DHT 11
#define DHTPIN D5

#define WIFISSID "Arihant KR"                               // Put your WifiSSID here
#define PASSWORD "333333333333"                     // Put your wifi password here
#define TOKEN "BBFF-nPzQ3r0ckEHHb3v3IJpcfkQTXvoFiK"   // Ubidots Token 
#define VARIABLE_TO_SUBSCRIBE "home_bulb"
#define DEVICE_LABEL "esp8266"


Ubidots client(TOKEN);
DHT dht = DHT(DHTPIN, DHTTYPE);
bool ismailsent=0;

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
    digitalWrite(LED, LOW);
  }
  else if ((char)payload[0] == '1')
  {
    Serial.println("Since, Output is 1, I'm going to turn on LED");
    digitalWrite(LED, HIGH);
  }
  else {
    Serial.print(" Ubidots Value is ");
    Serial.println((char)payload[0]);
  }
}

/****************************************
   Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);

  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(TEMP, INPUT);

  client.ubidotsSetBroker("industrial.api.ubidots.com");
  client.setDebug(true);
  client.wifiConnection(WIFISSID, PASSWORD);
  client.begin(callback);
  client.ubidotsSubscribe(DEVICE_LABEL, VARIABLE_TO_SUBSCRIBE);

  dht.begin();
}

void loop() {

  // Controlling Buzzer with LDR Sensor
  if (analogRead(LDR) < 200) {
    Serial.println("Light is falling on LDR. Buzzer will be turned on.");
    digitalWrite(BUZZER, HIGH);
    if(!ismailsent)
    {
      String maildetail = "Seems like your Home is under some security threat, an unknown source of unrecognised light has disturbed the LDR sensor.\n";
      maildetail += "I think you must check what happened!\n\n";
      maildetail += "The current Room temperature is " + String(dht.readTemperature()) + " Degree Celsius" + "\n";
      maildetail += "The current Room Relative Humidity is " + String(dht.readHumidity()) + " %\n";
      client.sendMail("mail.smtp2go.com",2525, "QXJpaGFudGty","YXJpaGFudDEyMw==","nodemcu@gmail.com","arihanteshwar@gmail.com","Security Breach",maildetail);
      ismailsent=1;
    }
  }
  else {
    Serial.println("Light is not falling on LDR");
    digitalWrite(BUZZER, LOW);
    ismailsent=0;
  }

  // Controlling Bulb with Website
  if (!client.connected()) {
    client.reconnect();
    client.ubidotsSubscribe(DEVICE_LABEL, VARIABLE_TO_SUBSCRIBE);
  }
  client.loop();

  // Reading Temperature and Humidity
  int humidity_data = (int)dht.readHumidity();
  int temperature_data = (int)dht.readTemperature();

  Serial.print("Current Room Temperature: ");
  Serial.println(humidity_data);
  Serial.print("Current Room Humidity: ");
  Serial.println(temperature_data);
  Serial.print("\n\n");

  delay(100);
}
