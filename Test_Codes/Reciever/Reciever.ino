#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
  char type[10];
} struct_message;

struct_message incomingMsg;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingMsg, incomingData, sizeof(incomingMsg));
  Serial.print("Alert received: ");
  Serial.println(incomingMsg.type);

  if (strcmp(incomingMsg.type, "FALL") == 0) {
    Serial.println("🚨 Fall detected!");
  } else if (strcmp(incomingMsg.type, "SOS_BTN") == 0) {
    Serial.println("🆘 SOS button pressed!");
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Do nothing, wait for alerts
}
