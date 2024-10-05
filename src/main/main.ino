#include "dht11.h"

#define DHT11PIN 2 // D2 포트에 DHT11 센서를 연결
#define soil_threshold 600
#define light_threshold 300
#define temperature_threshold 28
#define humidity_threshold 85
#define co2_threshold 1500
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
int led = 8; // LED
int is_fan_on = 0; // 팬이 돌아가고 있는지 확인하는 플래그 / 1 - 동작 중 , 0 - 멈춤 상태
int is_led_on = 0; // LED가 켜져 있는지 확인하는 플래그 / 1 - 켜짐 , 0 - 꺼짐 상태


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
  readSensor(); //센서값 읽기
  supply_water(); // 특정 조건에 물펌프 5초 가동
  controlFan(); // 특정 조건에 팬 동작 && 중지
  controlLED(); // 특정 조건에 led동작 && 중지
  //printSensor(); 디버그용
  send_data_to_raspi(); //센서 데이터 시리얼통신으로 보냄

  delay(2000); //2초 대기 / 나중에 전송 속도 조절
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

  void controlFan() {
    if ((data.temperature > temperature_threshold || data.humidity > humidity_threshold || data.co2 > co2_threshold) && !is_fan_on) {
      digitalWrite(fan, HIGH);  // 팬 동작
      is_fan_on = 1; 
    } else if ((data.temperature <= temperature_threshold && data.humidity <= humidity_threshold && data.co2 <= co2_threshold) && is_fan_on) {
      digitalWrite(fan, LOW);  // 팬 멈춤
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
    Serial.println(); //문장의 끝 표시
  }

}
