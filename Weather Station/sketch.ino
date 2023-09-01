#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "ThingSpeak.h"

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* weatherApiUrl = "https://api.openweathermap.org/data/2.5/weather?q=Chittagong&appid=985e2a3a6045936a3bd362c3577f1755";
const char* server = "api.thingspeak.com";
const char* apiKey = "226D1OCT2WF8319E";
const int channelID = 2235107;

WiFiClient client;

Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  ThingSpeak.begin(client);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(weatherApiUrl);

    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      float temperature = doc["main"]["temp"];
      float humidity = doc["main"]["humidity"];
      const char* weatherCondition = doc["weather"][0]["main"];

      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.print("Temp: ");
      display.println(temperature);
      display.print("Humidity: ");
      display.println(humidity);
      display.print("Weather: ");
      display.println(weatherCondition);
      display.display();

      ThingSpeak.setField(1,temperature);
      ThingSpeak.setField(2,humidity);
      ThingSpeak.setField(3,weatherCondition);
      int x = ThingSpeak.writeFields(channelID,apiKey);
      http.end();

      // Send data to ThingSpeak
      String tsUrl = "/update";
      tsUrl += "?api_key=" + String(apiKey);
      tsUrl += "&field1=" + String(temperature);
      tsUrl += "&field2=" + String(humidity);
      tsUrl += "&field3=" + String(weatherCondition);

      

      http.begin(server, 80, tsUrl);
      httpCode = http.GET();
      if (httpCode > 0) {
        Serial.println("Data sent to ThingSpeak successfully");
      } else {
        Serial.println("Error sending data to ThingSpeak");
      }
      http.end();
    }
  }
  delay(60000); // Fetch and send data every 60 seconds
}
