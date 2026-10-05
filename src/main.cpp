#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define WARN_LED_PIN 23
#define SENSOR_TEMP_PIN 4
#define LDR_PIN 12

const static String ssid = "Your_SSID";
const static String password = "Your_PASSWORD";

enum update
{
  LED,
  OLED,
  LDR,
  DS18B20
};

OneWire oneWire(SENSOR_TEMP_PIN);
DallasTemperature sensors(&oneWire);

WebServer server(80);

unsigned long prevLdr = 0;
unsigned long prevOled = 0;
unsigned long prevTemp = 0;
int temp = 0;
int ldr = 0;

float readTemp();
int readLdr();
void logicLed();
void updateFunc();

void setup()
{
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
  };

  server.on("/", []()
            {
    String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 Sensor Dashboard</title>
</head>
<body>

    <h1>ESP32 Sensor Dashboard</h1>

    <h2>Temperature</h2>
    <p>)rawliteral";

    html += temp;
    html += R"rawliteral( °C</p>

    <h2>Light</h2>
    <p>)rawliteral";

    html += ldr;
    html += R"rawliteral( %</p>

</body>
</html>
)rawliteral";

    server.send(200, "text/html", html); });
  server.begin();
  Serial.print("HTTP server started");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  sensors.begin();

  pinMode(SENSOR_TEMP_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(WARN_LED_PIN, OUTPUT);

  ledcSetup(0, 5000, 8);
  ledcAttachPin(WARN_LED_PIN, 0);
}

void loop()
{
  updateFunc();
  server.handleClient();
}

void updateFunc()
{
  unsigned long now = millis();

  if (now - prevLdr >= 200)
  {
    prevLdr = now;

    readLdr();
  }

  if (now - prevOled >= 100)
  {
    prevOled = now;
  }

  if (now - prevTemp >= 1000)
  {
    prevTemp = now;

    readTemp();
    logicLed();
  }
}

float readTemp()
{
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);
  return temp;
};
int readLdr()
{
  ldr = analogRead(LDR_PIN);
  ldr = map(ldr, 0, 4095, 0, 100);
  return ldr;
}

void logicLed()
{
  if (temp < 25)
  {
    ledcWrite(0, 0);
  }
  else if (temp > 25 && temp < 30)
  {
    ledcWrite(0, 76);
  }
  else if (temp > 30 && temp < 35)
  {
    ledcWrite(0, 153);
  }
  else if (temp > 35)
  {
    ledcWrite(0, 256);
  }
}