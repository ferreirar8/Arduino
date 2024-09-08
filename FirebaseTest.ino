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


void loop()
{
for(int i = 0; i <= 100; i++)
 { 
    Firebase.setInt("/measurement",i);
    delay(1000);
 }
}
