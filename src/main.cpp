#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>

const static String ssid = "Your_SSID";
const static String password = "Your_PASSWORD";

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  if(WiFi.status()!= WL_CONNECTED) {
      delay(250);
  };

  
} 

void loop() {

}