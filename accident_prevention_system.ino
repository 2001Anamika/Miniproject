#include <ThingSpeak.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
int sen1=D1;
int sen2=D2;
int sensor=D3;
int buzzer=D5;
int relay=D4;
float alc;
int limit=400;
unsigned long ch_no = 1943415;//Replace with Thingspeak Channel number
const char *write_api = "L5GS4ILNH98O8KKR";//Replace with Thingspeak write API
char auth[] = "mwa0000028107833";
char *ssid = "STUD";
char *pass = "STUD@2019";
unsigned long t1=0;
unsigned long t2=0; 
WiFiClient  client;
float velocity;
void setup()
{
  pinMode(sen1,INPUT);
  pinMode(sen2,INPUT);
  pinMode(sensor,INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(A0,INPUT);
  pinMode(relay,OUTPUT);
  digitalWrite(buzzer,LOW);
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
}
void loop() 
{
    
  int alc=analogRead(A0);
  if(alc>limit){
    digitalWrite(relay,HIGH);
    Serial.println("High");
    ThingSpeak.setField(2, alc);
    ThingSpeak.writeFields(ch_no, write_api);
  }
  else{
    digitalWrite(relay,LOW);
    Serial.println("LOW");
  }

  int statusSensor=digitalRead(sensor);
  if(statusSensor==1){
    digitalWrite(buzzer,LOW);
    noTone(buzzer);
    Serial.println("NOt Detected");
  }
  else
  {
    digitalWrite(buzzer,HIGH);
    tone(buzzer,500);
    Serial.println("Detected");
    ThingSpeak.setField(3, statusSensor);
    ThingSpeak.writeFields(ch_no, write_api);   
  }


  while(digitalRead(sen1));
  while(digitalRead(sen1)==0);
  t1=millis();
  while(digitalRead(sen2));
  t2=millis();
  velocity=t2-t1;
  velocity=velocity/1000;//convert millisecond to second
  velocity=(5.0/velocity);//v=d/t
  velocity=velocity*3600;//multiply by seconds per hr
  velocity=velocity/1000;
  Serial.println(velocity);
  //division by meters per Km
  if(velocity>80){
    digitalWrite(buzzer,HIGH);
    tone(buzzer,400);
    Serial.println("Overspeed");
  }
  else{
    digitalWrite(buzzer,LOW);
    noTone(buzzer);
    Serial.println("Normal speed");
    delay(500); 
  }
  if(velocity>0){
    ThingSpeak.setField(1, velocity);
    ThingSpeak.writeFields(ch_no, write_api);
  }

}
