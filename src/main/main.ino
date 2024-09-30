#include "dht11.h"

#define DHT11PIN 2 // D2 포트에 DHT11 센서를 연결
#define soil_threshold 600
#define light_threshold 300
#define temperature_threshold 28
#define humidity_threshold 85
// Function prototypes
void readSensor();
void printSensor();

dht11 DHT11; // 온습도 센서
int light_sensor = A0; // 조도 센서
int soil_sensor = A1; // 토양 습도 센서
int co2_sensor = A4; // CO2 센서
int AA = 13;  // 펌프 센서, AA를 연결한 디지털 핀 
int AB = 12;  // 펌프 센서, AB를 연결한 디지털 핀
int fan = 7; // 팬

struct SensorData {
  int temperature;
  int humidity;
  int illuminance;
  int soil_moisture;
  int co2;
};

SensorData data;

void setup() {
  Serial.begin(9600);
  pinMode(AA, OUTPUT);  // AA를 출력 핀으로 설정
  pinMode(AB, OUTPUT);  // AB를 출력 핀으로 설정
  pinMode(fan, OUTPUT); // 팬을 출력 핀으로 설정
}

void loop() {
  readSensor();
  supply_water();
  printSensor();
  delay(100);
}

void readSensor() {
  DHT11.read(DHT11PIN); // 온습도 센서(DTH11) 값 측정
  data.temperature = DHT11.temperature;
  data.humidity = DHT11.humidity;
  delay(10);
  data.illuminance = analogRead(light_sensor);
  delay(10);
  data.soil_moisture = analogRead(soil_sensor);
  delay(100);
  data.co2 = analogRead(co2_sensor);
  delay(100);
}

void printSensor() {
  Serial.print("대기 온도 : ");
  Serial.print(data.temperature);
  Serial.print(" 도  ");

  Serial.print("대기 습도 : ");
  Serial.print(data.humidity);
  Serial.print(" %  ");
  if (data.temperature > temperature_threshold || data.humidity > humidity_threshold) {
    digitalWrite(fan, HIGH);  // 팬 동작
  } else {
    digitalWrite(fan, LOW);  // 팬 멈춤
  }

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

void supply_water() {
  if (data.soil_moisture < soil_threshold) {
    digitalWrite(AA, HIGH);  // 정방향으로 모터 회전
    digitalWrite(AB, LOW);
    delay(5000);  // 5초 동안 상태 유지

    digitalWrite(AA, LOW);  // 모터 정지
    digitalWrite(AB, LOW);
  }
}
