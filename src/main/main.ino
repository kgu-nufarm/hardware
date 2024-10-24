#include "dht11.h"

#define DHT11PIN 2 // D2 포트에 DHT11 센서를 연결
#define soil_threshold 50
#define light_threshold 800
#define temperature_threshold 26
#define humidity_threshold 20
#define co2_threshold 700

void readSensor();
void printSensor();
void supply_water();
void controlFan();
void controlLED();
void send_data_to_raspi();

dht11 DHT11; // 온습도 센서
int light_sensor = A0; // 조도 센서
int soil_sensor = A1; // 토양 습도 센서
int co2_sensor = A4; // CO2 센서
int AA = 13;  // 펌프 센서, AA를 연결한 디지털 핀 
int AB = 12;  // 펌프 센서, AB를 연결한 디지털 핀
int fan = 7; // 팬
int led = 9; // LED
int is_fan_on = 0; // 팬이 돌아가고 있는지 확인하는 플래그 / 1 - 동작 중 , 0 - 멈춤 상태
int is_led_on = 0; // LED가 켜져 있는지 확인하는 플래그 / 1 - 켜짐 , 0 - 꺼짐 상태

unsigned long previousMillis = 0;
const long pumpInterval = 5000; // 5초 동안 펌프 동작
bool is_pumping = false;

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
  pinMode(AA, OUTPUT);  
  pinMode(AB, OUTPUT);  
  pinMode(fan, OUTPUT); 
  pinMode(led, OUTPUT); 
}

void loop() {
  readSensor(); // 센서값 읽기
  printSensor();
  supply_water(); // 특정 조건에 물펌프 5초 가동
  controlFan(); // 특정 조건에 팬 동작 && 중지
  controlLED(); // 특정 조건에 LED 동작 && 중지
  send_data_to_raspi(); // 센서 데이터 시리얼통신으로 보냄

  delay(2000); // 2초 대기 / 나중에 전송 속도 조절
}

void readSensor() {
  DHT11.read(DHT11PIN); // 온습도 센서(DTH11) 값 측정
  data.temperature = DHT11.temperature;
  data.humidity = DHT11.humidity;
  delay(10);

  int sensorValue = analogRead(light_sensor); // 조도 센서의 아날로그 값 읽기
  float voltage = sensorValue * (3.3 / 1023.0); // 아날로그 값을 전압으로 변환 (3.3V 기준)
  float lux = (500 / voltage); // 센서에 맞는 변환 공식 (예시)
  data.illuminance = lux;
  delay(10);

  int sensorValue2 = analogRead(soil_sensor);  // 센서의 아날로그 값 읽기
  int maxValue = 1300;  // 최대 값 (0% 습도)
  int minValue = 100;  // 최소 값 (100% 습도)

  float humidity = (float)(maxValue - sensorValue2) / (maxValue - minValue) * 100;

  if(humidity < 0) humidity = 0;  // 음수 값 방지
  if(humidity > 100) humidity = 100;  // 100% 이상 방지
  data.soil_moisture = humidity;
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
  unsigned long currentMillis = millis();

  if (data.soil_moisture < soil_threshold && !is_pumping) {
    digitalWrite(AA, HIGH);  // 정방향으로 모터 회전
    digitalWrite(AB, LOW);
    Serial.println("pump on");
    previousMillis = currentMillis;
    is_pumping = true;
  }

  // 물을 5초간 공급한 후 멈춤
  if (is_pumping && currentMillis - previousMillis >= pumpInterval) {
    digitalWrite(AA, LOW);  // 모터 정지
    digitalWrite(AB, LOW);
    Serial.println("pump off");
    is_pumping = false;
  }
}

void controlFan() {
  if ((data.temperature > temperature_threshold || data.humidity > humidity_threshold || data.co2 > co2_threshold) && !is_fan_on) {
    digitalWrite(fan, HIGH);  // 팬 동작
    Serial.println("fan on");
    is_fan_on = 1; 
  } else if ((data.temperature <= temperature_threshold && data.humidity <= humidity_threshold && data.co2 <= co2_threshold) && is_fan_on) {
    digitalWrite(fan, LOW);  // 팬 멈춤
    Serial.println("fan off");
    is_fan_on = 0; 
  }
}

void controlLED() {
  if (data.illuminance < light_threshold && !is_led_on) {
    digitalWrite(led, HIGH);  // LED 켜기
    is_led_on = 1; // LED 상태 업데이트
  } else if (data.illuminance >= light_threshold && is_led_on) {
    digitalWrite(led, LOW);  // LED 끄기
    is_led_on = 0; // LED 상태 업데이트
  }
}

void send_data_to_raspi(){
  // 구조체 데이터 시리얼 전송 (CSV 형식)
  Serial.print(data.temperature);
  Serial.print(",");
  Serial.print(data.humidity);
  Serial.print(",");
  Serial.print(data.illuminance);
  Serial.print(",");
  Serial.print(data.soil_moisture);
  Serial.print(",");
  Serial.print(data.co2);
  Serial.println(); // 문장의 끝 표시
}
