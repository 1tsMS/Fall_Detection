import serial
import csv

# === Configuration ===
PORT = "COM4"        # Change to your ESP32's serial port (e.g., "/dev/ttyUSB0" on Linux/Mac)
BAUD_RATE = 115200
CSV_FILENAME = "mpu_readings.csv"

# Open serial connection
ser = serial.Serial(PORT, BAUD_RATE, timeout=1)

# Create CSV file with header
with open(CSV_FILENAME, mode="w", newline="") as file:
    writer = csv.writer(file)
    writer.writerow(["timestamp_ms", "accel_x_g", "accel_y_g", "accel_z_g", 
                     "gyro_x_dps", "gyro_y_dps", "gyro_z_dps"])

    print(f"Logging to {CSV_FILENAME}... Press Ctrl+C to stop.")
    
    try:
        while True:
            line = ser.readline().decode("utf-8").strip()
            if not line:
                continue

            # Skip header line from ESP32
            if line.startswith("timestamp_ms"):
                continue

            # Parse CSV from ESP32
            try:
                values = [float(x) for x in line.split(",")]
                writer.writerow(values)
            except ValueError:
                print(f"Bad data: {line}")
    except KeyboardInterrupt:
        print("\nLogging stopped.")

ser.close()
