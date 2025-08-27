#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

// RFID Setup
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

// Servo
#define SERVO_PIN 3
Servo servo;
unsigned long lastDispenseTime = 0;
bool systemStarted = false;
bool servoAttached = false;

// Ultrasonic Pins
#define TRIG_PIN 5
#define ECHO_PIN 6

// IR Sensor
#define IR_PIN 7

// Buzzers
#define BUZZER_ULTRA_PIN 4   // 2-pin buzzer (active HIGH)
#define BUZZER_IR_PIN 8      // 3-pin buzzer (active LOW)

// IR Timer Variables
bool irObjectPresent = false;
unsigned long irStartTime = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(IR_PIN, INPUT);

  pinMode(BUZZER_ULTRA_PIN, OUTPUT);
  pinMode(BUZZER_IR_PIN, OUTPUT);

  digitalWrite(BUZZER_ULTRA_PIN, LOW);   // Buzzer OFF
  digitalWrite(BUZZER_IR_PIN, HIGH);     // Buzzer OFF (active LOW)

  Serial.println("📡 Scan RFID Card to Start System...");
}

void loop() {
  // 1. RFID Check
  if (!systemStarted) {
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      Serial.println("✅ RFID Card Scanned. System Started.");
      systemStarted = true;
      rfid.PICC_HaltA();

      if (!servoAttached) {
        servo.attach(SERVO_PIN);
        servo.write(90);  // Start at neutral position
        servoAttached = true;
      }
    }
    return;
  }

  // 2. Dispense every 10 seconds
  if (millis() - lastDispenseTime >= 10000) {
    dispenseMedicine();
    lastDispenseTime = millis();
  }

  // 3. Ultrasonic Sensor
  long distance = getDistance();
  bool medicineLow = (distance >= 7);
  digitalWrite(BUZZER_ULTRA_PIN, medicineLow ? HIGH : LOW);

  // 4. IR Sensor
  int irValue = digitalRead(IR_PIN);  // LOW = object detected

  if (irValue == LOW) {
    if (!irObjectPresent) {
      irObjectPresent = true;
      irStartTime = millis();
    } else {
      if (millis() - irStartTime >= 8000) {  // ⏱️ Now 8 seconds
        digitalWrite(BUZZER_IR_PIN, LOW);  // ON (active LOW)
      }
    }
  } else {
    irObjectPresent = false;
    irStartTime = 0;
    digitalWrite(BUZZER_IR_PIN, HIGH);  // OFF
  }

  // 5. Serial Monitor Debug
  Serial.print("|  Distance: ");
  Serial.print(distance);
  Serial.print(" cm  |  Med Level ≥7cm: ");
  Serial.print(medicineLow ? "YES" : "NO ");
  Serial.print("  |  IR Detect: ");
  Serial.print(irValue == LOW ? "YES" : "NO ");
  Serial.print("  |  IR Buzzer: ");
  Serial.println(digitalRead(BUZZER_IR_PIN) == HIGH ? "OFF" : "ON");

  delay(500);
}

void dispenseMedicine() {
  Serial.println("🔄 Dispensing Medicine...");
  
  // Anti-clockwise rotation 45° from current 90°, return back
  servo.write(135);
  delay(1000);
  servo.write(90);

  Serial.println("✅ Medicine Dispensed.");
}

long getDistance() {
  long total = 0;
  int count = 5;

  for (int i = 0; i < count; i++) {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000);  // 30ms timeout
    if (duration > 0) {
      total += duration;
    }

    delay(10);
  }

  if (total == 0) return 999;  // No reading
  long avgDuration = total / count;
  long distance = avgDuration * 0.0343 / 2;
  return distance;  // In cm
}
