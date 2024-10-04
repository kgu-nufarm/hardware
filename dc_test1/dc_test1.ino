int AA = D8;               //모터A의 A를 D8번 핀에 배치
int AB = D9;               //모터A의 B를 D9번 핀에 배치
int BA = D6;               //모터B의 A를 D6번 핀에 배치
int BB = D7;               //모터B의 B를 D7번 핀에 배치

void setup() 
{
  pinMode(AA, OUTPUT);
  pinMode(AB, OUTPUT);
  pinMode(BA, OUTPUT);
  pinMode(BB, OUTPUT);    
}
 
void loop() 
{
  digitalWrite(AA, HIGH);          //모터A를 정회전
  digitalWrite(AB, LOW);
  digitalWrite(BA, HIGH);          //모터B를 정회전
  digitalWrite(BB, LOW);
  delay(1000);                     
 
  digitalWrite(AA, LOW);           //모터A를 정지
  digitalWrite(AB, LOW);
  digitalWrite(BA, LOW);           //모터B를 정지
  digitalWrite(BB, LOW);
  delay(250);
  
  digitalWrite(AA, LOW);           //모터A를 역회전     
  digitalWrite(AB, HIGH);
  digitalWrite(BA, LOW);           //모터B를 역회전     
  digitalWrite(BB, HIGH);
  delay(1000);
}