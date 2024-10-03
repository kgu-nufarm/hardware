#include <ESP32Servo.h>

Servo servo1;
Servo servo2;  
int servoPin1 = D6;
int servoPin2 = D7;

void setup() {
  Serial.begin(115200);
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
}

void loop() {
   servo1.write(180);
   servo2.write(180);
   delay(2000);
  
   servo1.write(0);
   servo2.write(0);
   delay(2000);

}
