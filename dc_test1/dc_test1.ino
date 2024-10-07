int IN1 = D6;  // 모터 A의 IN1 핀 (PWM 가능 핀)
int IN2 = D7;  // 모터 A의 IN2 핀 (정방향/역방향 제어)
int speedValue = 100;  // 모터 속도 (0~255 사이 값)

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

void loop() {
  motorForward();  // 모터를 정방향으로 회전
  delay(5000);     // 5초간 회전

  motorBackward(); // 모터를 역방향으로 회전
  delay(5000);     // 5초간 회전
}

// 모터를 정방향으로 회전시키는 함수
void motorForward() {
  analogWrite(IN1, speedValue);  // PWM 신호로 속도 제어
  digitalWrite(IN2, LOW);        // 정방향 회전
}

// 모터를 역방향으로 회전시키는 함수
void motorBackward() {
  analogWrite(IN1, speedValue);  // PWM 신호로 속도 제어
  digitalWrite(IN2, HIGH);       // 역방향 회전
}
