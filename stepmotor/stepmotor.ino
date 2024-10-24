#define ANGLE 20480

#include <CheapStepper.h>

// 두 개의 스텝퍼 객체 생성
CheapStepper stepper1(D6, D7, D8, D9);  // 첫 번째 모터 핀 (IN1, IN2, IN3, IN4)
CheapStepper stepper2(D2, D3, D4, D5);  // 두 번째 모터 핀 (IN1, IN2, IN3, IN4)

void setup() 
{
}

void loop() 
{
  rotateBothMotorsClockwise();     // 두 모터를 동시에 시계 방향으로 회전
  delay(500);                      // 0.5초 대기
  rotateBothMotorsCounterClockwise();  // 두 모터를 동시에 반시계 방향으로 회전
  delay(500);                      // 0.5초 대기
}

// 차양막 내리기
void rotateBothMotorsClockwise() {
  for (int i = 0; i < ANGLE; i++)   // 1바퀴 4096스텝
  {
    stepper1.step(true);           
    stepper2.step(false);           
  }
}

// 차양막 올리기
void rotateBothMotorsCounterClockwise() {
  for (int i = 0; i < ANGLE; i++)   // 1바퀴 4096스텝
  {
    stepper1.step(false);          
    stepper2.step(true);          
  }
}
