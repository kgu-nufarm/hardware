int IN1 = D6;  // 모터 A의 IN1 핀 (PWM 가능 핀)
int IN2 = D7;  // 모터 A의 IN2 핀 (정방향/역방향 제어)
int speedValue = 100;  // 모터 속도 (0~255 사이 값)
int light_sensor = A0; // 조도 센서
int forward_light_threshold = 400; // 정방향 회전용 조도 임계값
int backward_light_threshold = 200; // 역방향 회전용 조도 임계값

bool is_motor_forward = false;  // 모터가 정방향 동작 중인지 체크
bool is_motor_backward = false; // 모터가 역방향 동작 중인지 체크

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(light_sensor, INPUT);
}

void loop() {
  int light_value = analogRead(light_sensor);  // 조도 값 읽기

  if (light_value > forward_light_threshold && !is_motor_forward) {
    motorForward();  // 조도가 forward_light_threshold 이상일 때 모터를 정방향으로 회전
  } else if (light_value <= backward_light_threshold && !is_motor_backward) {
    motorBackward(); // 조도가 backward_light_threshold 이하일 때 모터를 역방향으로 회전
  }

  delay(2000);  // 2초 대기 후 다음 루프 반복
}

// 모터를 정방향으로 회전시키는 함수
void motorForward() {
  analogWrite(IN1, speedValue);  // PWM 신호로 속도 제어
  digitalWrite(IN2, LOW);        // 정방향 회전
  delay(5000);                   // 5초간 회전
  motorStop();                   // 5초 후 모터 정지

  // 모터 상태 업데이트
  is_motor_forward = true;  
  is_motor_backward = false; 
}

// 모터를 역방향으로 회전시키는 함수
void motorBackward() {
  analogWrite(IN1, speedValue);  // PWM 신호로 속도 제어
  digitalWrite(IN2, HIGH);       // 역방향 회전
  delay(5000);                   // 5초간 회전
  motorStop();                   // 5초 후 모터 정지

  // 모터 상태 업데이트
  is_motor_backward = true;  
  is_motor_forward = false; 
}

// 모터를 정지시키는 함수
void motorStop() {
  digitalWrite(IN1, LOW);        // 모터 정지
  digitalWrite(IN2, LOW);        // 모터 정지
}
