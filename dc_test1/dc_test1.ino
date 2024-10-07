// int AA = D8;               // 모터 A의 A 핀
// int AB = D9;               // 모터 A의 B 핀
int BA = D6;               // 모터 B의 A 핀
int BB = D7;               // 모터 B의 B 핀
int STATUS = 0;

void setup() 
{
  // pinMode(AA, OUTPUT);
  // pinMode(AB, OUTPUT);
  pinMode(BA, OUTPUT);
  pinMode(BB, OUTPUT);    
}

void loop() 
{
  
  motorForward();           // 모터를 정방향으로 회전
  delay(5000);              // 5초간 대기
}

// 모터를 정방향으로 회전시키는 함수
void motorForward() 
{
  // digitalWrite(AA, HIGH);   // 모터 A 정방향
  // digitalWrite(AB, LOW);    
  digitalWrite(BA, HIGH);   // 모터 B 정방향
  digitalWrite(BB, LOW);
}
