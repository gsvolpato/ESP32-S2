#include "credentials.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_PRINT Serial

BlynkTimer timer;

BLYNK_WRITE(V0)
{
  int value = param.asInt();
  Blynk.virtualWrite(V1, value);
}

BLYNK_WRITE(V4)
{
  String message = param.asStr();
  Serial.print("Terminal received: ");
  Serial.println(message);
  
  if (message == "help") {
    Blynk.virtualWrite(V4, "Available commands:\n");
    Blynk.virtualWrite(V4, "- help: Show this help\n");
    Blynk.virtualWrite(V4, "- status: Show system status\n");
    Blynk.virtualWrite(V4, "- ip: Show IP address\n");
    Blynk.virtualWrite(V4, "- uptime: Show uptime\n");
    Blynk.virtualWrite(V4, "- restart: Restart ESP32\n");
  }
  else if (message == "status") {
    Blynk.virtualWrite(V4, "System Status:\n");
    Blynk.virtualWrite(V4, "WiFi: Connected\n");
    Blynk.virtualWrite(V4, "Blynk: Connected\n");
    Blynk.virtualWrite(V4, "Free Heap: " + String(ESP.getFreeHeap()) + " bytes\n");
  }
  else if (message == "ip") {
    Blynk.virtualWrite(V4, "IP Address: " + WiFi.localIP().toString() + "\n");
  }
  else if (message == "uptime") {
    unsigned long uptime = millis() / 1000;
    Blynk.virtualWrite(V4, "Uptime: " + String(uptime) + " seconds\n");
  }
  else if (message == "restart") {
    Blynk.virtualWrite(V4, "Restarting ESP32...\n");
    delay(1000);
    ESP.restart();
  }
  else {
    Blynk.virtualWrite(V4, "Unknown command: " + message + "\n");
    Blynk.virtualWrite(V4, "Type 'help' for available commands\n");
  }
}

BLYNK_CONNECTED()
{
  Blynk.virtualWrite(V4, "ESP32-S2 Terminal Ready!\n");
  Blynk.virtualWrite(V4, "Type 'help' for available commands\n");
}

void myTimerEvent()
{
  Blynk.virtualWrite(V2, millis() / 1000);
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("ESP32-S2 Starting...");
  Serial.println("Connecting to WiFi...");
  Serial.print("SSID: ");
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  Serial.println("Connecting to Blynk...");
  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();
  
  if (Blynk.connected()) {
    Serial.println("Blynk connected!");
  } else {
    Serial.println("Blynk connection failed!");
  }

  timer.setInterval(1000L, myTimerEvent);
  Serial.println("Setup complete!");
}

void loop()
{
  Blynk.run();
  timer.run();
}
