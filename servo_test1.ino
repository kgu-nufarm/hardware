#include <ESP32Servo.h>

Servo servo1;
Servo servo2;  
int servoPin1 = D6;

void setup() {
  Serial.begin(115200);
  servo1.attach(servoPin1);
}

void loop() {
   servo1.write(180);
   delay(2000);
  
   servo1.write(0);
   delay(2000);

}
