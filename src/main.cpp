#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>

const static String ssid = "Your_SSID";
const static String password = "Your_PASSWORD";

WebServer server(80);

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  if(WiFi.status()!= WL_CONNECTED) {
      delay(250);
  };

  server.on("/", []() {
    server.send(200, "text/plain", "Hello from ESP32!");
  });
  server.begin();
  Serial.println("HTTP server started");
} 

void loop() {

}