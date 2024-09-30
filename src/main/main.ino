#include "dht11.h"

#define DHT11PIN 2 // D2 포트에 DHT11 센서를 연결

dht11 DHT11; // 온습도 센서

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  DHT11.read(DHT11PIN); // 온습도 센서(DTH11) 값 측정
  Serial.println(DHT11.temperature);
  Serial.println(DHT11.humidity);

  delay(100);
}
