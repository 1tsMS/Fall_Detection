#include <WiFi.h>
#include <esp_now.h>
#include "esp_wifi.h"

typedef struct {
  float value;
} struct_message;

struct_message outgoingMsg;

uint8_t receiverMAC[] = {0xD0, 0xEF, 0x76, 0x32, 0x4C, 0xFC}; // change if different


// Find channel of receiver AP
uint8_t findChannelForSSID(const char* targetSSID) {
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i) {
    if (WiFi.SSID(i) == targetSSID) {
      return WiFi.channel(i);
    }
  }
  return 0; // not found
}

void setup() {
  Serial.begin(115200);

  // Find gateway channel
  Serial.println("Scanning for ESP32_Gateway_AP...");
  uint8_t ch = findChannelForSSID("ESP32_Gateway_AP");
  if (ch == 0) {
    Serial.println("Gateway AP not found! Restarting...");
    delay(2000);
    ESP.restart();
  }
  Serial.print("Found gateway on channel "); Serial.println(ch);

  // Init WiFi in STA mode
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Add receiver as peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = ch;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("ESP-NOW ready to send");
}

void loop() {
  outgoingMsg.value = 69; // constant 10%
  esp_err_t result = esp_now_send(receiverMAC, (uint8_t *) &outgoingMsg, sizeof(outgoingMsg));

  if (result == ESP_OK) {
    Serial.println("Sent successfully");
  } else {
    Serial.println("Error sending the data");
  }
  delay(1000);
}
