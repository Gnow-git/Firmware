#include "IRSendRev.h" // 소스 코드 폴더에 복사 (파일 3 개)
#define IR_OUT_PIN 3 // IR 수신기를 D5 에 연결. D3 을 제외한 핀 사용 가능
#include "string.h"
unsigned char dta[20];
String data;
int case_num, num, init_num = 1;
int count = 0;
//unsigned char number[20];
int number_a, number_b, sw, total, number;

void setup()
{
 Serial.begin(9600); // 직렬 통신 초기화
 IR.Init(IR_OUT_PIN); // IR 루틴 초기화
 Serial.println("start");
}


void loop()
{
  int total, num1, num2;
 
 if(IR.IsDta()) // 데이터를 수신하였다면
 {
 
  IR.Recv(dta);
  //Serial.print("입력 신호 :");
  //Serial.print(dta[D_DATA+3], HEX);
  //Serial.print("\n");
  
  data = String(dta[D_DATA+3], HEX);
  
  data.toUpperCase(); // 대문자 처리

  num = numberset(data);  // 버튼 지정
  //Serial.print("변환된 신호 :");
  //Serial.print(num);
  //Serial.print("\n");

  if(data == "4F"){   // 리모컨의 C를 누를 경우
  Serial.print("초기화");
  total = number_a = number_b = sw = 0;
  Serial.print(total);
  Serial.print(number_a);
  Serial.print(number_b);
  Serial.print("\n");
  }else 
    caculator(num); // 숫자 저장
 }
}

void caculator(int num){
   if (num == 11){  // + 누르면
    sw = 1;
  }else if(num == 12){  // - 누르면
    sw = 2;
  } else if (num >= 0 && num <= 9){
    if(sw == 0){
      number_a = number_a *10 + num;
      Serial.print("a에 저장된 숫자 :");
      Serial.print(number_a);
      Serial.print("\n");
    }
    else if(sw == 1 || sw == 2){
     number_b = number_b *10 + num;
      Serial.print("b에 저장된 숫자 :");
      Serial.print(number_b);
      Serial.print("\n"); 
    }
  }else if(num == 13 && sw == 1){
    total = number_a + number_b;
      Serial.print("합산 :");
      Serial.print(total);
      Serial.print("\n");
  }else if(num == 13 && sw == 2){
     total = number_a - number_b;
      Serial.print("빼기 :");
      Serial.print(total);
      Serial.print("\n");
  }
   
}

int numberset(String data){
  if(data == "5D"){ // 전원
    case_num = 14;
    init_num = 0;
  }else if(data == "97"){
    case_num = 0;
  }else if(data == "CF"){
    case_num = 1;
  }else if(data == "E7"){
    case_num = 2;
  }else if(data == "85"){
    case_num = 3;
  }else if(data == "EF"){
    case_num = 4;
  }else if(data == "C7"){
    case_num = 5;
  }else if(data == "A5"){
    case_num = 6;
  }else if(data == "BD"){
    case_num = 7;
  }else if(data == "B5"){
    case_num = 8;
  }else if(data == "AD"){
    case_num = 9;
  }else if(data == "3D"){ // 취소
    case_num = 10;
  }else if(data == "FD"){ // +
    case_num = 11;
  }else if(data == "67"){ // -
    case_num = 12;
  }else if(data == "57"){ // 엔터
    case_num = 13;
  }else case_num = 99;

  return case_num;
}
