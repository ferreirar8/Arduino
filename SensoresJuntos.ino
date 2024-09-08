#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <LSM303.h>
#include <Wire.h>
#define FIREBASE_HOST "iot-alarm-app-c7a0e-default-rtdb.europe-west1.firebasedatabase.app"
#define FIREBASE_AUTH "XMmemyM6U1X8W6ROnzaAqfQo16jDVhYWLW4hhBzj"
#define WIFI_SSID "TP-Link_19E6"
#define WIFI_PASSWORD "XXXXXXX"
LSM303 accelerometer;

void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  pinMode(D0,INPUT); //inputs botoes
  pinMode(D3,INPUT);

  Wire.begin();
  accelerometer.init();
  accelerometer.enableDefault();
  pinMode(D1,INPUT); //inputs acel
  pinMode(D2,INPUT);
}

void botao() {
  if(digitalRead(D0) == LOW){
    Serial.println("Medico!");
    Firebase.setBool("/medico_status",true);
    if (Firebase.failed()) { 
      Serial.print("setting /medico_status failed:"); 
      Serial.println(Firebase.error());   
      return; 
    } 
  }else{    
    Serial.println(".");
    Firebase.setBool("/medico_status",false);
    if (Firebase.failed()) { 
      Serial.print("setting /medico_status failed:"); 
      Serial.println(Firebase.error());   
      return; 
    }
  }
  if(digitalRead(D3) == LOW){
    Serial.println("Reforços!");
    Firebase.setBool("/reforcos_status",true);
    if (Firebase.failed()) { 
      Serial.print("setting /reforcos_status failed:"); 
      Serial.println(Firebase.error());   
      return; 
    } 
  }else{    
    Serial.println(".");
    Firebase.setBool("/reforcos_status",false);
    if (Firebase.failed()) { 
      Serial.print("setting /reforcos_status failed:"); 
      Serial.println(Firebase.error());   
      return; 
    }
  }
}

void pulse() {
  int Signal;
  int PulseSensorPurplePin = 0;
  Signal = analogRead(PulseSensorPurplePin);                                         
  Serial.println(Signal);               
  delay(400);
  Firebase.setInt("/bpm_status",Signal);
}

void acel() {
  int acel;
  accelerometer.read();
  acel = sqrt(abs(accelerometer.a.x)+ abs(accelerometer.a.y ) + abs(accelerometer.a.z));
  Serial.println(acel);
  if (acel>240){
    Firebase.setBool("/queda_status",true);
  }
  else {
    Firebase.setBool("/queda_status",false);
    delay(1);
  }
}


void loop() {
  botao();
  acel();
  pulse();
  acel();
}
