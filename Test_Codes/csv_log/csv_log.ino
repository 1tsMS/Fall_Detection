#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

unsigned long prevMicros = 0;
const unsigned long sampleInterval = 10000; // 10 ms = 100 Hz

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Initialize MPU6050
  Serial.println("Initializing MPU6050...");
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }

  // Print CSV header
  Serial.println("timestamp_ms,accel_x_g,accel_y_g,accel_z_g,gyro_x_dps,gyro_y_dps,gyro_z_dps");
}

void loop() {
  unsigned long currentMicros = micros();
  if (currentMicros - prevMicros >= sampleInterval) {
    prevMicros = currentMicros;

    // Read raw data
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // Convert to physical units
    float accelScale = 16384.0; // LSB/g for ±2g
    float gyroScale = 131.0;    // LSB/(°/s) for ±250°/s

    float ax_g = ax / accelScale;
    float ay_g = ay / accelScale;
    float az_g = az / accelScale;

    float gx_dps = gx / gyroScale;
    float gy_dps = gy / gyroScale;
    float gz_dps = gz / gyroScale;

    unsigned long timestamp = millis();

    // Output as CSV
    Serial.print(timestamp);
    Serial.print(",");
    Serial.print(ax_g, 4);
    Serial.print(",");
    Serial.print(ay_g, 4);
    Serial.print(",");
    Serial.print(az_g, 4);
    Serial.print(",");
    Serial.print(gx_dps, 4);
    Serial.print(",");
    Serial.print(gy_dps, 4);
    Serial.print(",");
    Serial.println(gz_dps, 4);
  }
}
