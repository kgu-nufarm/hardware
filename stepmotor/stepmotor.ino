#define ANGLE 4096

#include <CheapStepper.h>

// 두 개의 스텝퍼 객체 생성
CheapStepper stepper1(6, 5, 4, 3);  // 첫 번째 모터 핀 
CheapStepper stepper2(10, 11, A3, A2);  // 두 번째 모터 핀 

void setup() 
{
}

void loop() 
{
  closeBlind();     
  delay(500);                      // 0.5초 대기
  openBlind();  
  delay(500);                      // 0.5초 대기
}

// 차양막 내리기
void closeBlind() {
  for (int i = 0; i < ANGLE; i++)   // 1바퀴 4096스텝
  {
    stepper1.step(true);           
    stepper2.step(false);           
  }
}

// 차양막 올리기
void openBlind() {
  for (int i = 0; i < ANGLE; i++)   // 1바퀴 4096스텝
  {
    stepper1.step(false);          
    stepper2.step(true);          
  }
}
