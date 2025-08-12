#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

Adafruit_MPU6050 mpu;

// Thresholds
const float FALL_THRESHOLD_G      = 0.8;
const float IMPACT_THRESHOLD_G    = 3.0;
const float STILLNESS_THRESHOLD_G = 0.5;
const unsigned long STILLNESS_TIME_MS = 2000;
const unsigned long FALL_TIMEOUT_MS   = 3000;
const unsigned long IMPACT_TIMEOUT_MS = 3000;

enum FallState {
  NORMAL,
  FALL_DETECTED,
  IMPACT_DETECTED
};

FallState currentState = NORMAL;
unsigned long stateStartTime = 0;
unsigned long stillnessStartTime = 0;

// Store last Z acceleration to detect direction
float lastAz_g = 1.0;

float getAccelerationMagnitude(float ax_g, float ay_g, float az_g) {
  return sqrt(ax_g * ax_g + ay_g * ay_g + az_g * az_g);
}

void printStatus(const char* message) {
  Serial.println(message);
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) delay(10);
  }

  Serial.println("MPU6050 Found!");
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  delay(500);

  // Take initial reading
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  lastAz_g = a.acceleration.z / 9.80665;

  Serial.println("\nReady for downward fall detection!");
  Serial.println("------------------------------------------------");
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float ax_g = a.acceleration.x / 9.80665;
  float ay_g = a.acceleration.y / 9.80665;
  float az_g = a.acceleration.z / 9.80665;

  float totalAcceleration = getAccelerationMagnitude(ax_g, ay_g, az_g);

  switch (currentState) {
    case NORMAL:
      // Detect downward free fall only if Z is decreasing (falling)
      if (az_g < FALL_THRESHOLD_G && (az_g < lastAz_g)) {
        currentState = FALL_DETECTED;
        stateStartTime = millis();
        printStatus("Downward fall state detected...");
      }
      break;

    case FALL_DETECTED:
      if (totalAcceleration > IMPACT_THRESHOLD_G) {
        currentState = IMPACT_DETECTED;
        stateStartTime = millis();
        stillnessStartTime = millis();
        printStatus("Impact detected! Checking for stillness...");
      } 
      else if (millis() - stateStartTime > FALL_TIMEOUT_MS) {
        printStatus("Fall timeout, returning to NORMAL.");
        currentState = NORMAL;
      }
      break;

    case IMPACT_DETECTED:
      if (fabs(totalAcceleration - 1.0) < STILLNESS_THRESHOLD_G) {
        if (millis() - stillnessStartTime >= STILLNESS_TIME_MS) {
          printStatus("--- FALL CONFIRMED! ---");
          currentState = NORMAL;
        }
      } else {
        stillnessStartTime = millis();
      }

      if (millis() - stateStartTime > IMPACT_TIMEOUT_MS) {
        printStatus("Impact timeout, returning to NORMAL.");
        currentState = NORMAL;
      }
      break;
  }

  // Store last Z-axis reading to detect if acceleration is increasing or decreasing
  lastAz_g = az_g;

  delay(20);
}
