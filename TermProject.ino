#include <TM1637Display.h>

#include "IRSendRev.h" // 소스 코드 폴더에 복사 (파일 3 개)
#include "TimerOne.h"
#define ledPin 9
#define IR_OUT_PIN 3 // IR 수신기를 D5 에 연결. D3 을 제외한 핀 사용 가능
#define CLK 10   // 4-digit display 의 CLK 핀을 지정
#define DIO 11   // 4-digit display 의 DIO 핀을 지정
TM1637Display display(CLK, DIO); // TM1637Display 객체를 생성

unsigned char dta[20];  // 리모컨으로부터 입력된 코드를 저장할 함수
String data;  // 리모컨으로부터 입력된 코드 중 일부만 저장할 함수
int case_num, num, led_s; // 리모컨 코드에서 정수로 변환할 함수, 변경한 정수, led on/off 함수
// 첫번째 숫자 자리수, 두번째 숫자 자리수, 전원 상태
int num_a_count = 1, num_b_count = 1, power_count = 0;
int number_a, number_b, sw, total; // 첫번째 숫자, 두번째 숫자, 부호, 합산
int ledState = LOW; // led 상태

void setup()
{
 Serial.begin(9600); // 직렬 통신 초기화
 IR.Init(IR_OUT_PIN); // IR 루틴 초기화
 display.setBrightness(0x04, false); // 4-digit display 의 밝기 지정 및 false로 미출력
 display.showNumberDec(0); // 초기 0으로 초기화
 pinMode(ledPin, OUTPUT); // led 설정
 Timer1.initialize(500000); // 타이머 0.5초로 지정
 Timer1.attachInterrupt(led); // 타이머 인터럽트로 led 설정
 Serial.println("start"); // 프로그램 실행 파악을 위해 시리얼 모니터에 출력
}


void loop()
{
  int total, num1, num2;  // 합산, 첫번째 값, 두번째 값
 
 if(IR.IsDta()) // 데이터를 수신하였다면
 {
  IR.Recv(dta); // 수신된 데이터 값 읽어오기
  
  data = String(dta[D_DATA+3], HEX);  // 읽어온 값중 끝 2자리만 저장
  
  data.toUpperCase(); // 대문자 처리

  num = numberset(data);  // 버튼 지정

  if(data == "5D"){   // 리모컨의 전원버튼(5D)를 누를 경우
    if(power_count == 0){ // 전원이 꺼져있다면
      Serial.println("전원 켜기");
     led_s = 1; // led 실행
     display.setBrightness(7, true); // 4-digit display 의 밝기 지정
     display.showNumberDec(0);  // 초기 값으로 0 출력
     power_count = 1; // 전원 값 변경
    }else if (power_count == 1){  // 전원이 켜져 있다면
      Serial.println("전원 끄기");  
      total = number_a = number_b = sw = 0; // 입력되었던 값 초기화
      num_a_count = num_b_count = 1;  // 각 숫자 자리값 초기화
      led_s = 0;  // led 종료
      power_count = 0;  // 전원 값 변경
      display.setBrightness(7, false); // FND 종료
      display.showNumberDec(0);
    }
    
  }
  
  if(power_count == 1 && data == "4F"){   // 리모컨의 C(4F)를 누를 경우
  Serial.println("초기화");
  total = number_a = number_b = sw = 0; // 합산, 첫&두번째 값, 부호 0으로 초기화
  num_a_count = num_b_count = 1;  // 자리수 1로 초기화
  display.showNumberDec(number_a);  // 초기화된 값 출력 0
  }else if(power_count == 1 && data == "3D"){ // 리모컨의 취소(3D) 버튼을 누를 경우
    cancle(); // 마지막자리 숫자 취소
  }else if(power_count == 1){ // 전원이 켜져 있다면
    caculator(num); // 숫자 저장
  }
 }
}

void led(){     // led 상태를 관리하는 함수
  if(led_s == 1){ // FND가 켜져 있다면
    ledState = !ledState; // 0.5초 마다 상태 변경
    digitalWrite(ledPin, ledState); // LED ON/OFF
  }else{
    digitalWrite(ledPin, LOW);  // LED OFF
  }
}

void caculator(int num){    // 계산이 진행되는 함수
   if (number_a != 0 && num == 11){  // 첫번째 숫자가 0이 아니고 + 누르면
    num_b_count = 1;  // 두번째 숫자 자리 수 초기화
    sw = 1; // +가 입력됨을 설정
    Serial.print("더하기 \n"); // +가 눌렸을때 시리얼 모니터에 출력
  }else if(number_a != 0 && num == 12){  // 첫번째 숫자가 0이 아니고 - 누르면
    num_b_count = 1;  // 두번째 숫자 자리 수 초기화
    sw = 2; // -가 입력됨을 설정
    Serial.print("빼기 \n");  // -가 눌렸을때 시리얼 모니터에 출력
  } else if (num >= 0 && num <= 9){ // 0 ~ 9 사이의 숫자를 입력하면
      if(sw == 0){  // +나 -가 입력이 안되었을 경우 첫번째 숫자로 저장
        if(num_a_count <= 4){ // 첫번째 숫자가 4자리가 안넘었다면
          number_a = number_a *10 + num;  // 자리수를 늘려가면서 첫번째 숫자 저장 
          display.showNumberDec(number_a);  // 첫번째 숫자 FND에 출력
          Serial.print("a에 저장된 숫자 :");  // 시리얼 모니터에 첫번째 숫자 출력
          Serial.println(number_a);
          num_a_count ++; // 첫번째 숫자 자리수 확인을 위해 증가
        }else{  // 첫번째 숫자가 4자리가 넘었을 경우
          Serial.println("4자리 이상 입력할 수 없습니다."); // 시리얼 모니터에 출력
        }
      }else if(sw == 1 || sw == 2){ // +나 -가 입력되었을 경우
        if(num_b_count <= 4) {  // 두번째 숫자가 4자리가 안넘었다면
          number_b = number_b *10 + num;  // 자리수를 늘려가면서 두번째 숫자 저장
          display.showNumberDec(number_b);  // 두번째 숫자 FND에 출력
          Serial.print("b에 저장된 숫자 :");  // 시리얼 모니터에 두번째 숫자 출력
          Serial.println(number_b); 
          num_b_count ++; // 두번째 숫자 자리수 확인을 위해 증가
        }else{  // 두번째 숫자가 4자리가 넘었을 경우
          Serial.println("4자리 이상 입력할 수 없습니다."); // 시리얼 모니터에 출력
        }
      }else { // 기타의 상황에서 4자리가 넘었을 경우
        Serial.println("4자리 이상 입력할 수 없습니다."); // 시리얼 모니터에 출력
      }
  }else if(num == 13 && sw == 1){ // 엔터를 입력하고 더하기일 경우
    total = number_a + number_b;  // 첫번째, 두번째 숫자 합산
      Serial.print("합산 :"); // 합산된 값 시리얼 모니터에 출력
      Serial.println(total);
    if(total <= 9999){  // 합산된 값이 9999가 안넘을 경우
      display.showNumberDec(total); // FND에 출력
      number_a = total; // 합산 후 결과를 첫번째 숫자로
      number_b = sw = 0;  // 두번째 숫자와 부호를 초기화
    }else{  // 합산된 값이 9999가 넘을 경우
      Serial.println("합산 숫자가 표현할 수 있는 범위를 벗어 났습니다.");
    }
  }else if(num == 13 && sw == 2){ // 엔터를 입력하고 빼기일 경우
    if(number_a > number_b){  // 첫번째 숫자가 두번째 숫자보다 클 경우
     total = number_a - number_b; // 뺄셈한 값 저장
     display.showNumberDec(total);  // 뺄셈한 값 FND에 출력
      Serial.print("빼기 :"); // 뺄셈한 값 시리얼 모니터에 출력
      Serial.println(total);
    number_a = total; // 뺄셈 결과를 첫번째 숫자로
    number_b = sw =0; // 두번째 숫자와 부호를 초기화
    }else{  // 두번째 숫자가 첫번째 숫자보다 클 경우
      Serial.println("두번째 숫자가 더 커 뺄셈을 할 수 없습니다.");
    } 
  }else if(num == 13){  // 첫번째 숫자만 입력 후 엔터를 입력 했을 경우
    total = number_a; // 결과에 첫번째 숫자 저장
    display.showNumberDec(total); // 결과를 FND에 출력
    Serial.print("결과 :"); // 결과를 시리얼 모니터에 출력
      Serial.println(total);
    number_a = total; // 합산 결과를 첫번째 숫자로
    number_b = 0; // 부호는 사용하지 않았으므로 두번째 숫자만 초기화
  }
}

void cancle(){    // 숫자 취소
  if (sw == 0 && number_a != 0){  // 첫번째 숫자 입력 받을 차례이며 0이 아닌 경우
    number_a = number_a / 10; // 첫번째 숫자의 마지막자리 숫자 취소
    display.showNumberDec(number_a);  // 첫번째 숫자를 FND에 출력
    num_a_count --; // 첫번째 숫자의 자리수를 줄임
    Serial.print("a에 저장된 숫자 :");  // 시리얼 모니터에 첫번째 숫자 출력
    Serial.println(number_a);
  } else if (sw == 1 || sw == 2){ // 두번째 입력 받을 차례이며
     if(number_b != 0){ // 0이 아닌 경우
      number_b = number_b / 10; // 두번째 숫자의 마지막자리 숫자 취소
      display.showNumberDec(number_b);  // 두번째 숫자를 FND에 출력
      num_b_count --; // 두번째 숫자의 자리수를 줄임
      Serial.print("b에 저장된 숫자 :");  // 시리얼 모니터에 두번째 숫자 출력
      Serial.println(number_b);
     }else {
      Serial.println("두번째 숫자를 입력해 주세요"); // 두번째 숫자 입력시 지울 숫자가 더 없을 경우
     }
    }else if(number_a == 0){  // 첫번째 숫자 입력시 지울 숫자가 더 없을 경우
    Serial.println("첫번째 숫자를 입력해 주세요");
    }
}

int numberset(String data){ // 필요 기능 숫자로 변경
  if(data == "5D"){ // 전원
    case_num = 14;
  }else if(data == "97"){ // 버튼의 0
    case_num = 0;
  }else if(data == "CF"){ // 버튼의 1
    case_num = 1;
  }else if(data == "E7"){ // 버튼의 2
    case_num = 2;
  }else if(data == "85"){ // 버튼의 3
    case_num = 3;
  }else if(data == "EF"){ // 버튼의 4
    case_num = 4;
  }else if(data == "C7"){ // 버튼의 5
    case_num = 5;
  }else if(data == "A5"){ // 버튼의 6
    case_num = 6;
  }else if(data == "BD"){ // 버튼의 7
    case_num = 7;
  }else if(data == "B5"){ // 버튼의 8
    case_num = 8;
  }else if(data == "AD"){ // 버튼의 9
    case_num = 9;
  }else if(data == "3D"){ // 취소
    case_num = 10;
  }else if(data == "FD"){ // +
    case_num = 11;
  }else if(data == "67"){ // -
    case_num = 12;
  }else if(data == "57"){ // 엔터
    case_num = 13;
  }else case_num = 99;  // 기타 다른 버튼은 99로 지정

  return case_num;  // 입력된 숫자를 리턴
}
