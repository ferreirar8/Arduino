#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <LSM303.h>
#include <Wire.h>

#define FIREBASE_HOST "iot-alarm-app-c7a0e-default-rtdb.europe-west1.firebasedatabase.app"
#define FIREBASE_AUTH "XMmemyM6U1X8W6ROnzaAqfQo16jDVhYWLW4hhBzj"
#define WIFI_SSID "TP-Link_19E6"
#define WIFI_PASSWORD "XXXXXXX"

#define BUTTON_MEDICO_PIN D0
#define BUTTON_REFORCOS_PIN D3
#define PULSE_SENSOR_PIN A0

#define ACCEL_THRESHOLD 240

LSM303 accelerometer;

void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
}

void setupAccelerometer() {
  Wire.begin();
  accelerometer.init();
  accelerometer.enableDefault();
}

void updateFirebase(const String &path, bool value) {
  Firebase.setBool(path, value);
  if (Firebase.failed()) {
    Serial.print("Failed to update ");
    Serial.print(path);
    Serial.print(": ");
    Serial.println(Firebase.error());
  }
}

void handleButtonPress() {
  bool medicoStatus = (digitalRead(BUTTON_MEDICO_PIN) == LOW);
  Serial.println(medicoStatus ? "Medico!" : ".");
  updateFirebase("/medico_status", medicoStatus);

  bool reforcosStatus = (digitalRead(BUTTON_REFORCOS_PIN) == LOW);
  Serial.println(reforcosStatus ? "Reforços!" : ".");
  updateFirebase("/reforcos_status", reforcosStatus);
}

void readPulseSensor() {
  int pulseValue = analogRead(PULSE_SENSOR_PIN);
  Serial.print("Pulse: ");
  Serial.println(pulseValue);
  Firebase.setInt("/bpm_status", pulseValue);
}

void readAccelerometer() {
  accelerometer.read();
  int accelMagnitude = sqrt(abs(accelerometer.a.x) + abs(accelerometer.a.y) + abs(accelerometer.a.z));
  Serial.print("Acceleration: ");
  Serial.println(accelMagnitude);

  bool fallDetected = (accelMagnitude > ACCEL_THRESHOLD);
  updateFirebase("/queda_status", fallDetected);
}

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_MEDICO_PIN, INPUT);
  pinMode(BUTTON_REFORCOS_PIN, INPUT);

  connectWiFi();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  setupAccelerometer();
}

void loop() {
  handleButtonPress();
  readAccelerometer();
  readPulseSensor();
  delay(25);
}
