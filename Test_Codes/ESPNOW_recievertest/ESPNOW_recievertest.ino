#include <WiFi.h>
#include <esp_now.h>

typedef struct {
  float value;
} struct_message;

struct_message incomingMsg;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingMsg, incomingData, sizeof(incomingMsg));
  Serial.print("Received value: ");
  Serial.println(incomingMsg.value);
}

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  Serial.print("Receiver MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {}
