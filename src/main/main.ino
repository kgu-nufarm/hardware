#include "dht11.h"

#define DHT11PIN 2 // D2 포트에 DHT11 센서를 연결

// Function prototypes
void readSensor();
void printSensor();

dht11 DHT11; // 온습도 센서
int light_sensor = A0; // 조도 센서
int soil_sensor = A1; //토양 습도 센서
int co2_sensor = A4; //co2 센서

struct SensorData{
  int temperature;
  int humidity;
  int illuminance;
  int soil_moisture;
  int co2;
};

SensorData data;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  readSensor();
  printSensor();
  delay(100);
}

void readSensor(){
  DHT11.read(DHT11PIN); // 온습도 센서(DTH11) 값 측정
  data.temperature=DHT11.temperature;
  data.humidity=DHT11.humidity;
  delay(10);
  data.illuminance=analogRead(light_sensor);
  delay(10);
  data.soil_moisture=analogRead(soil_sensor);
  delay(100);
  data.co2=analogRead(co2_sensor);
  delay(100);
}

void printSensor(){
  Serial.print("대기 온도 : ");
  Serial.print(data.temperature);
  Serial.print(" 도  ");

  Serial.print("대기 습도 : ");
  Serial.print(data.humidity);
  Serial.print(" %  ");

  Serial.print(" 조도 : ");
  Serial.print(data.illuminance);
  Serial.print(" LUX ");

  Serial.print(" 토양 습도 :  ");
  Serial.print(data.soil_moisture);
  Serial.print(" % ");

  Serial.print(" CO2 : ");
  Serial.print(data.co2);
  Serial.print(" ppm ");

  Serial.println();
}