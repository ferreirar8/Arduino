#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "iot-alarm-app-c7a0e-default-rtdb.europe-west1.firebasedatabase.app"
#define FIREBASE_AUTH "XMmemyM6U1X8W6ROnzaAqfQo16jDVhYWLW4hhBzj"
#define WIFI_SSID "TP-Link_19E6"
#define WIFI_PASSWORD "XXXXXXX"

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int i = 0;
bool alarm;

void setup2(){
pinMode(13, INPUT);
pinMode(4, OUTPUT);
}


void toque() {
  tone(4, 10000);
  delay(300);
  noTone(4);
  delay(300);
  tone(4, 10000);
  delay(300);
  noTone(4);
}


bool sound;

void loop() {
  if (digitalRead(13) == HIGH){
    Firebase.setBool("/alarm_status", true);
    i++;
    Firebase.setFloat("/measurement", i);
    sound = Firebase.getBool("enable_alert_sound");
    if (sound){
      toque();
    }
    else {
      noTone(4);
    }
  }
  else {
    noTone(3);
    Firebase.setBool("/alarm_status", false);
  }
  delay(1000);
}
