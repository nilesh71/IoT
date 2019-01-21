#include "dht.h"
dht DHT;
#include <ESP8266WiFi.h>
#include "FirebaseArduino.h"  

#define FIREBASE_HOST "finalpro1.firebaseio.com"  //should not have http:// and the end /
#define FIREBASE_AUTH "Przo7gd2RV2F68HV8IFE2CJ3qWwFed4EAyJcGlTJ"  // go to settings in firebase,select project settings and service account, secrets auth key
#define WIFI_SSID "Honor7X"
#define WIFI_PASSWORD "neeraj123"

int dhtin=D5;
int pir1=D6;
int pir2=D7;
int gasdetect=D3;     //pin high when smoke detected
int fire=D8;          //pin to detect smoke
int soilpin=A0;  // to read the values frm the soil
int soilpump=D0;  //to run pump if soil moisture is less

void setup()
{
  pinMode(gasdetect, OUTPUT);// act after detecting smoke
  pinMode(fire, INPUT); //detect smoke
  pinMode(pir2, INPUT);   // declare sensor as input
  pinMode(pir1, OUTPUT);  // pir output to buzzer
  pinMode(dhtin, INPUT);   // input form dht sensor
  pinMode(soilpump,OUTPUT);
  Serial.begin(115200); // begin serial communication
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
  Firebase.set("temp",1);
  Firebase.set("humidity",1);
  Firebase.set("gasdetect",0);
  Firebase.set("intruder",0);
}

void humtemp()
{
  int readdata = DHT.read11(dhtin);
  float t=DHT.temperature;
  float h=DHT.humidity;

 // Serial.print("current humidity = ");
 // Serial.print(h);
  Firebase.setFloat("temp",t);
  Firebase.setFloat("humidity",h);
  //Serial.print("temperature = ");
  //Serial.print(t);
  delay(50000);
}

void intruder()
 {
   long state = digitalRead(pir2);
   if(state== HIGH)
  {
      digitalWrite (pir1, HIGH);
      Firebase.set("intruder",1);
  }
   else 
  {
      digitalWrite (pir1, LOW);
  }
 }
void soilmoist()
 {
  int moisture=analogRead(soilpin);
  if(moisture>1000)
    digitalWrite(soilpump,LOW);
    Serial.print("motor inactive")
   else
   {
    digitalWrite(soilpump,HIGH);
    Serial.print("active active active")
    Firebase.set("pump",1);
 }

void loop()
{
  while(1)
  {
    if (Firebase.failed()) 
    {
      Serial.print("setting number failed:");
      Serial.println(Firebase.error());  
      firebasereconnect();
      return;
    }  
    humtemp();
    intruder();
    soilmoist();
  }
}
