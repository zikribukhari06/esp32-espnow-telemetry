#include <WiFi.h> // library Wifi.h

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  delay(1000);
  Serial.println();
  Serial.print("MAC Address ESP32 ini: "); // example = 7C:E8:B1:B1:E7:B8
  Serial.println(WiFi.macAddress());
}

void loop() {
}
