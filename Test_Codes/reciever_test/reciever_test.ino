// Blynk macros BEFORE include
#define BLYNK_TEMPLATE_ID "TMPL3ldQi1gIT"
#define BLYNK_TEMPLATE_NAME "Fall Detection"
#define BLYNK_AUTH_TOKEN "oVVAg6G_rn-5ibme608U-nnSTJS3EVjE"

#include <WiFi.h>
#include <esp_now.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Mr Samosa";     // hotspot SSID
char pass[] = "whoknows?";     // hotspot password

typedef struct {
  float value;
} struct_message;

struct_message incomingMsg;

// ESP-NOW receive callback
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  Serial.println("\n--- ESP-NOW PACKET RECEIVED ---");

  Serial.print("From MAC: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();

  Serial.print("Bytes received: ");
  Serial.println(len);

  memcpy(&incomingMsg, incomingData, sizeof(incomingMsg));
  Serial.print("Received value: ");
  Serial.println(incomingMsg.value);

  // Send to Blynk
  Blynk.virtualWrite(V1, incomingMsg.value);
}

void setup() {
  Serial.begin(115200);
  Serial.println("=== Gateway setup ===");

  WiFi.mode(WIFI_AP_STA);

  // 1) Connect STA to hotspot
  Serial.print("Connecting STA to hotspot...");
  WiFi.begin(ssid, pass);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    delay(300);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("STA connected to hotspot");
    Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());
    Serial.print("Router channel: "); Serial.println(WiFi.channel());

    // 2) Start AP on same channel as hotspot
    WiFi.softAP("ESP32_Gateway_AP", "", WiFi.channel());
    Serial.print("SoftAP MAC: "); Serial.println(WiFi.softAPmacAddress());
    Serial.print("SoftAP channel: "); Serial.println(WiFi.channel());
  } else {
    Serial.println("STA connection failed — defaulting AP to channel 1");
    WiFi.softAP("ESP32_Gateway_AP", "", 1);
  }

  // 3) Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  // 4) Start Blynk
  if (WiFi.status() == WL_CONNECTED) {
    Blynk.config(auth);
    Blynk.connect();
  }

  Serial.println("Gateway ready — waiting for ESP-NOW data...");
}

void loop() {
  Blynk.run();
}
