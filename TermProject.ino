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
  
  data = String(dta[D_DATA+3], HEX);
  
  data.toUpperCase(); // 대문자 처리

  num = numberset(data);  // 버튼 지정


  if(data == "4F"){   // 리모컨의 C를 누를 경우
  Serial.print("초기화 : ");
  total = number_a = number_b = sw = 0;
  Serial.print(total);
  Serial.print(number_a);
  Serial.print(number_b);
  Serial.print("\n");
  }else if(data == "3D"){ // 리모컨의 취소 버튼을 누를 경우
    cancle();
  }else 
    caculator(num); // 숫자 저장
 }
}

void caculator(int num){
   if (num == 11){  // + 누르면
    sw = 1;
    Serial.print("더하기 \n");
  }else if(num == 12){  // - 누르면
    sw = 2;
    Serial.print("빼기 \n");
  } else if (num >= 0 && num <= 9){ // 0 ~ 9 사이의 숫자를 입력하면
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
  }else if(num == 13 && sw == 1){ // 더하기 결과
    total = number_a + number_b;
      Serial.print("합산 :");
      Serial.print(total);
      Serial.print("\n");
    number_a = total; // 합산 결과를 첫번째 숫자로
    number_b = 0;
    sw = 0;
  }else if(num == 13 && sw == 2){ // 빼기 결과
     total = number_a - number_b;
      Serial.print("빼기 :");
      Serial.print(total);
      Serial.print("\n");
    number_a = total; // 합산 결과를 첫번째 숫자로
    number_b = 0;
    sw = 0;
  }else if(num == 13){  // 첫번째 숫자 입력 후 엔터를 입력 했을 경우
    total = number_a;
    Serial.print("결과 :");
      Serial.print(total);
      Serial.print("\n");
    number_a = total; // 합산 결과를 첫번째 숫자로
    number_b = 0;
  } 
}

void cancle(){    // 숫자 취소
  if (sw == 0 && number_a != 0){  // 첫번째 숫자 입력 받을 차례이며 0이 아닌 경우
    number_a = number_a / 10; // 마지막 숫자 취소
    Serial.print("a에 저장된 숫자 :");
    Serial.print(number_a);
    Serial.print("\n");
  } else if (sw == 1 || sw == 2){ // 두번째 입력 받을 차례이며
     if(number_b != 0){ // 0이 아닌 경우
      number_b = number_b / 10; // 마지막 숫자 취소
      Serial.print("b에 저장된 숫자 :");
      Serial.print(number_b);
      Serial.print("\n");
     }else {
      Serial.print("두번째 숫자를 입력해 주세요");
      Serial.print("\n");
     }
    }else if(number_a == 0){ 
    Serial.print("첫번째 숫자를 입력해 주세요");
    Serial.print("\n");
    }
}

int numberset(String data){ // 필요 기능 숫자로 변경
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
