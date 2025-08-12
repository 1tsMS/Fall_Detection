#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

bool fallStage1 = false;   // Free fall detected
unsigned long stage1Time = 0;
bool awaitingUpright = false;
unsigned long lastFallTime = 0;

const float FREEFALL_THRESHOLD = 0.5;  // g
const float IMPACT_THRESHOLD   = 2.0;  // g
const unsigned long FREEFALL_TO_IMPACT_MS = 1000;
const unsigned long STILLNESS_TIME_MS = 3000;
const unsigned long COOLDOWN_MS = 4000;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }
  Serial.println("MPU6050 ready, fall detection armed.");
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float axg = ax / 16384.0;
  float ayg = ay / 16384.0;
  float azg = az / 16384.0;

  float totalAcc = sqrt(axg * axg + ayg * ayg + azg * azg);

  // Calculate pitch/roll in degrees
  float pitch = atan2(axg, sqrt(ayg * ayg + azg * azg)) * 180.0 / PI;
  float roll  = atan2(ayg, sqrt(axg * axg + azg * azg)) * 180.0 / PI;

  unsigned long now = millis();

  // === Stage 1: Detect free fall (downward or diagonal-downward only) ===
  if (!fallStage1 && !awaitingUpright) {
    if (totalAcc < FREEFALL_THRESHOLD && azg < 0) { // azg < 0 means downward acceleration
      fallStage1 = true;
      stage1Time = now;
      Serial.println("Free fall detected...");
    }
  }

  // === Stage 2: Detect impact after free fall ===
  if (fallStage1 && (now - stage1Time) <= FREEFALL_TO_IMPACT_MS) {
    if (totalAcc > IMPACT_THRESHOLD) {
      Serial.println("Impact detected, checking for stillness...");
      fallStage1 = false;
      lastFallTime = now;
      awaitingUpright = true;
    }
  }

  // === Stage 3: Confirm stillness for 3 seconds ===
  if (awaitingUpright && (now - lastFallTime) >= STILLNESS_TIME_MS) {
    if (totalAcc > 0.95 && totalAcc < 1.05) { // near 1g still
      Serial.println("FALL CONFIRMED!");
      lastFallTime = now; // mark cooldown start
    }
  }

  // === Reset for multiple detections ===
  if (awaitingUpright) {
    // Reset if upright OR cooldown time has passed
    if ((abs(pitch) < 30 && abs(roll) < 30) || (now - lastFallTime) > COOLDOWN_MS) {
      awaitingUpright = false;
      Serial.println("Re-armed for next fall detection.");
    }
  }

  delay(50);
}
