#include <esp_now.h>
#include <WiFi.h>

// GANTI dengan MAC Address ESP Udara kamu!
uint8_t airMAC[] = {0x24, 0x0A, 0xC4, 0x9A, 0x03, 0xB5};

uint8_t sendBuffer[250];
int bufLen = 0;

// Callback saat data MAVLink diterima dari ESP Udara
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  // Muntahkan data langsung ke port USB Laptop (Mission Planner)
  Serial.write(incomingData, len);
}

void setup() {
  // Serial USB ke Laptop (Baudrate wajib sama dengan Pixhawk/Mission Planner)
  Serial.begin(57600);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  // Daftarkan ESP Udara sebagai Peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, airMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  esp_now_add_peer(&peerInfo);
}

void loop() {
  // Ambil data perintah dari Mission Planner (via USB)
  while (Serial.available()) {
    sendBuffer[bufLen] = Serial.read();
    bufLen++;

    // Jika buffer penuh (250 byte), tembak ke ESP Udara
    if (bufLen >= 250) {
      esp_now_send(airMAC, sendBuffer, bufLen);
      bufLen = 0;
    }
  }

  // Siram sisa data kalau aliran dari USB lagi jeda
  if (bufLen > 0) {
    esp_now_send(airMAC, sendBuffer, bufLen);
    bufLen = 0;
  }
}
