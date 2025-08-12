# Helmet/Belt-Mounted Fall Detection & SOS Alert System

A **wearable safety device** designed for workers, bikers, elderly individuals, and outdoor enthusiasts.  
It detects **falls**, confirms **impact + stillness**, and sends **real-time alerts** via ESP-NOW and Blynk IoT.  
Includes a **manual SOS button** for emergencies and a **sensor recalibration button** for accuracy when repositioned.

---

## 📦 Features

- **Fall detection algorithm** using MPU6050 accelerometer & gyroscope.
- **Impact + stillness verification** to reduce false alarms.
- **Direction control** for downward fall detection (reduces false triggers).
- **Manual SOS button** for immediate emergency alerts.
- **Sensor recalibration button** for accurate readings after repositioning (helmet/belt).
- **Wireless ESP-NOW** transmission between transmitter & receiver.
- **Blynk IoT** integration for remote alerting to smartphones.
- **Custom 3D-printed enclosures** for transmitter & receiver.

---

## 🛠 Hardware Components

**Transmitter:**
- ESP32 (or ESP8266 if adjusted)
- MPU6050 Accelerometer + Gyroscope
- Push button for **SOS**
- Push button for **Recalibration**
- LiPo battery or power source

**Receiver:**
- ESP32
- Blynk IoT connectivity
- Power supply or USB power

**Optional:**
- 3D printed cases for both units (helmet/belt mount & desktop receiver)

---

## 📜 How It Works

### 1. **Transmitter**
- Reads acceleration & gyro data from **MPU6050**.
- Runs a fall detection algorithm:
  1. Detects **downward free fall** (Z-axis drop).
  2. Waits for **impact detection**.
  3. Confirms **stillness** for a set time.
- Sends **fall alert** via ESP-NOW to receiver.
- Sends **SOS alert** instantly when SOS button is pressed.
- Can be recalibrated when worn in a new position using the recalibration button.

### 2. **Receiver**
- Listens for alerts via **ESP-NOW**.
- Sends warning to Blynk app for remote notifications.

---

## 🖨 3D Printed Cases

- **Transmitter Case** — Designed for helmet/belt mounting with button access.
- **Receiver Case** — Desktop or wall-mounted case with status LED window.

---

## 📲 Blynk Integration

- Create a Blynk project and set up widgets for **fall alert** and **SOS alert**.

---

## 📜 License
MIT License — free to use, modify, and distribute.

---

## 🙌 Acknowledgements
- MPU6050 Fall Detection research
- Adafruit sensor libraries
- Blynk IoT platform
- ESP-NOW documentation
