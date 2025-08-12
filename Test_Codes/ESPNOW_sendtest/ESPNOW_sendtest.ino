#include <WiFi.h>
#include <esp_now.h>

uint8_t receiverMAC[] = {0xD0, 0xEF, 0x76, 0x32, 0x4C, 0xFC};

typedef struct {
  float value;
} struct_message;

struct_message msg;

esp_now_peer_info_t peerInfo;

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  Serial.print("Sender MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;  // 0 = current channel
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  msg.value = 10.0; // constant 10%
  
  esp_err_t result = esp_now_send(receiverMAC, (uint8_t *) &msg, sizeof(msg));
  
  if (result == ESP_OK) {
    Serial.println("Sent successfully");
  } else {
    Serial.println("Error sending data");
  }
  
  delay(1000);
}
