#include <esp_now.h>
#include <WiFi.h>

// GANTI dengan MAC Address ESP Darat kamu!
uint8_t groundMAC[] = {0x24, 0x0A, 0xC4, 0x9A, 0x03, 0xA4};

uint8_t sendBuffer[250];
int bufLen = 0;

// Callback saat data diterima dari ESP Darat via ESP-NOW
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  // Kirim data balasan dari laptop langsung ke Pixhawk
  Serial2.write(incomingData, len);
}

void setup() {
  // Serial2 disambungkan ke Port TELEM1 Pixhawk (Baudrate 57600)
  // RX2 = GPIO 16 (Colok ke TX Pixhawk)
  // TX2 = GPIO 17 (Colok ke RX Pixhawk)
  Serial2.begin(57600, SERIAL_8N1, 16, 17);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  // Daftarkan ESP Darat sebagai Peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, groundMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  esp_now_add_peer(&peerInfo);
}

void loop() {
  // Ambil data MAVLink dari Pixhawk
  while (Serial2.available()) {
    sendBuffer[bufLen] = Serial2.read();
    bufLen++;

    // Jika buffer penuh (250 byte), langsung tembak ke udara
    if (bufLen >= 250) {
      esp_now_send(groundMAC, sendBuffer, bufLen);
      bufLen = 0;
    }
  }

  // Siram sisa data di buffer kalau aliran serial lagi jeda (mencegah lag)
  if (bufLen > 0) {
    esp_now_send(groundMAC, sendBuffer, bufLen);
    bufLen = 0;
  }
}
