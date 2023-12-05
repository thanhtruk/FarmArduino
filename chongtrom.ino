#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
// #include <NTPtime.h>
#ifndef APSSID
#define APSSID "HSU_Students"  // existing Wifi network
#define APPSK "dhhs12cnvch"
#endif
/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;
const char *URL = "http://10.106.26.221:8081/add";
WiFiClient client;
HTTPClient http;
ESP8266WebServer server(80);

const int pirPin = D2;    // Chân cảm biến PIR kết nối với chân D2 của Wemos D1 R2
const int buzzerPin = D5; // Chân loa kết nối với chân D5 của Wemos D1 R2

unsigned long startTime;
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
  server.on("/", handleOnConnect);
  server.enableCORS(true);
  server.begin();
  Serial.println("HTTP server started");
  startTime = millis();
}
void loop() {
  server.handleClient();
  long duration = millis() - startTime;  // thời gian bằng giá trị hiện tại trừ  giá trị ban đầu 
  if (duration == 5000) {
    postJsonData();
    startTime = millis();
  }

  delay(5000);
}
void handleOnConnect() {
  server.send(200, "text/html", "ok");
}
void postJsonData() {
  Serial.print("connecting to ");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, URL)) {  // HTTP
      Serial.print("[HTTP] POST...\n");
      //gui du lieu len server dang JSON
      const int capacity = JSON_OBJECT_SIZE(2000);
      StaticJsonDocument<capacity> doc;
      /* doc thong tin cam bien vao day */
      int motionSensorValue = 0;
      getDatafromSen(motionSensorValue);
      
      doc["motion_detected"] = motionSensorValue;
      doc["atTime"] = "2023-12-02";
      
      char output[2048];
      serializeJson(doc, Serial);  // ghi ra man hinh
      serializeJson(doc, output);  //ghi ra bien output
      http.addHeader("Content-Type", "application/json");
      int httpCode = http.POST(output);
      Serial.println(httpCode);
      http.end();  //Close connection Serial.println();
      Serial.println("closing connection");
    }
  }
}

void getDatafromSen(int motionSensorValue){
 motionSensorValue = digitalRead(pirPin);

  if (motionSensorValue == HIGH) {
    Serial.println(motionSensorValue);
    digitalWrite(buzzerPin, HIGH); // Bật âm thanh
    delay(1000); // Đợi 1 giây
    digitalWrite(buzzerPin, LOW); // Tắt âm thanh
  }

  delay(100); // Đợi 0.1 giây trước khi kiểm tra lại              
}