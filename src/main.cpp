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

BLYNK_CONNECTED()
{
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
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
