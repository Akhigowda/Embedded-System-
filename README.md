---

# Automatic Medicine Dispenser

## Overview

The **Automatic Medicine Dispenser** is an IoT-based system designed to ensure timely and accurate delivery of medicines to patients. It automates the process of dispensing medicines at fixed intervals, monitors stock levels, and ensures that the patient actually takes the medicine. The project combines hardware components (Arduino, sensors, servo motor) with programmed logic to achieve a reliable healthcare solution.

---

## Features

* **RFID Authentication** → System starts only after scanning a valid RFID card.
* **Automated Dispensing** → Servo motor rotates **45° every 10 seconds** to release one dose of medicine.
* **Medicine Stock Monitoring** → Ultrasonic sensor checks medicine levels inside a cylindrical container:

  * If distance ≥ 7 cm → **Low Stock Buzzer** turns ON.
  * If distance < 7 cm → Stock is sufficient.
* **Intake Verification** → IR sensor detects if the medicine is taken within 10 seconds:

  * If not taken → **Alert Buzzer** turns ON.
* **Activity Logs** → All activities (RFID scan, dispensing, alerts) are displayed on the Arduino **Serial Monitor** (LCD support planned).

---

## Hardware Components

* Arduino UNO
* RFID Module (RC522)
* Servo Motor
* Ultrasonic Sensor (HC-SR04)
* IR Sensor
* 2× Buzzer (for stock and intake alerts)
* Medicine Container (cylindrical type)
* Power Supply

---

##  Connections

* **RFID RC522**: SDA → Pin 10, SCK → Pin 13, MOSI → Pin 11, MISO → Pin 12, RST → Pin 9
* **Servo Motor**: Signal → Pin 3
* **Ultrasonic Sensor**: Trig → Pin 5, Echo → Pin 6
* **IR Sensor**: OUT → Pin 7
* **Buzzer 1 (Stock Alert)**: Signal → Pin 4
* **Buzzer 2 (Intake Alert)**: Signal → Pin 8

---

## Workflow

1. Patient scans RFID card → system validates and starts.
2. Every **10 seconds**, servo rotates to dispense medicine.
3. Ultrasonic sensor checks stock level and triggers buzzer if medicine is low.
4. IR sensor monitors whether the medicine is picked up within 10 seconds.

   * If not → buzzer alerts caregiver.
5. Serial Monitor displays system activities in real-time.

---

## Future Enhancements

* LCD/LED display for user-friendly alerts.
* Wi-Fi module (ESP8266/ESP32) for remote monitoring via mobile app.
* SMS/email alerts for caregivers when stock is low.
* Medicine scheduling based on real-world timings (hours/days instead of seconds).

---

## Team & Credits

Developed as part of **MPCA / IoT Mini Project** by: **Akilesh Gowda K S**

---

Do you want me to also make a **shorter version of README** (just overview, features, and workflow) for quick submission, or should I keep this detailed GitHub-style one?
