#include "IRSendRev.h" // 소스 코드 폴더에 복사 (파일 3 개)
#define IR_OUT_PIN 3 // IR 수신기를 D5 에 연결. D3 을 제외한 핀 사용 가능
#include "string.h"
unsigned char dta[20];
String data;
int case_num;

void setup()
{
 Serial.begin(9600); // 직렬 통신 초기화
 IR.Init(IR_OUT_PIN); // IR 루틴 초기화
 Serial.println("start");
}


void loop()
{
 if(IR.IsDta()) // 데이터를 수신하였다면
 {
  IR.Recv(dta);
  Serial.print(dta[D_DATA+3], HEX);
  Serial.print("\n");
  
  data = String(dta[D_DATA+3], HEX);
  
  data.toUpperCase();
  Serial.print(data);

  numberset(data);  // 버튼 지정
  
  switch(case_num){
    case 14 :
          Serial.print("전원버튼입니다.");
          break;
    case 11 :
          Serial.print("더하기를 진행합니다.");
          break;
    case 12 :
          Serial.print("빼기를 진행합니다.");
          break;
    default :
          Serial.print("지정 안된 버튼입니다.");
  }
  
  Serial.print("\n");
 }
}

void numberset(String data){
  if(data == "5D"){
    case_num = 14;
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

}
