#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#ifndef APSSID
#define APSSID "Minh Vang"  // existing Wifi network
#define APPSK "choxincaighe"
#endif

const char *ssid = APSSID;
const char *password = APPSK;
const char *URL = "http://192.168.1.5:8081/add";

WiFiClient client;
HTTPClient http;
ESP8266WebServer server(80);

const int pirPin = D2;    // PIR sensor connected to D2
const int buzzerPin = D5; // Buzzer connected to D5

unsigned long startTime;
bool motionSensorActive = true;  // Variable to indicate if the motion sensor is active

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");  // You can change the NTP server if needed

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
 
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connect to existing Wifi network...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();  // Initialize the NTP client

  server.on("/", handleOnConnect);
  server.on("/activate", handleActivate);  // New endpoint to activate the sensor
  server.on("/deactivate", handleDeactivate);  // New endpoint to deactivate the sensor
  server.enableCORS(true);
  server.begin();
  Serial.println("HTTP server started");
  startTime = millis();
}

void loop() {
  server.handleClient();
  long duration = millis() - startTime;

  if (motionSensorActive && duration >= 5000) {
    postJsonData();
    startTime = millis();
  }

  timeClient.update();  // Update the NTP client to get the current time

  delay(5000);
}

void handleOnConnect() {
  server.send(200, "text/html", "ok");
}

void handleActivate() {
  motionSensorActive = true;
  server.send(200, "text/html", "Motion sensor activated");
}

void handleDeactivate() {
  motionSensorActive = false;
  server.send(200, "text/html", "Motion sensor deactivated");
}

void postJsonData() {
  Serial.print("connecting to ");

  if (WiFi.status() == WL_CONNECTED && motionSensorActive) {
    Serial.print("[HTTP] begin...\n");

    if (http.begin(client, URL)) {
      Serial.print("[HTTP] POST...\n");

      const int capacity = JSON_OBJECT_SIZE(2000);
      StaticJsonDocument<capacity> doc;
      int motionSensorValue = 0;
      getDatafromSen(motionSensorValue);

      doc["motion_detected"] = motionSensorValue;
      doc["timestamp"] = timeClient.getFormattedTime();

      char output[2048];
      serializeJson(doc, Serial);
      serializeJson(doc, output);

      http.addHeader("Content-Type", "application/json");
      int httpCode = http.POST(output);
      Serial.println(httpCode);
      http.end();
      Serial.println("closing connection");
    }
  }
}

void getDatafromSen(int &motionSensorValue) {
  motionSensorValue = digitalRead(pirPin);

  if (motionSensorValue == HIGH) {
    Serial.println(motionSensorValue);
    digitalWrite(buzzerPin, HIGH);
    delay(1000);
    digitalWrite(buzzerPin, LOW);
  }

  delay(100);
}
