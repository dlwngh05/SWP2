int ledPin = 7;  // LED가 연결된 핀 번호

void setup() {
  // LED 핀을 출력 모드로 설정
  pinMode(ledPin, OUTPUT);

}
void loop() {
  // 처음 1초 동안 LED 켜기
  digitalWrite(ledPin, 0);
  delay(1000);

  // 5회 깜빡이기
  for (int i = 0; i < 6; i++) {
    digitalWrite(ledPin, 0);  // LED 켜기
    delay(200);                  // 0.1초 동안 켜기
    digitalWrite(ledPin, 1);   // LED 끄기
    delay(200);                  // 0.1초 동안 끄기
  }

  // LED 끄고 무한루프
  digitalWrite(ledPin, 1);
  while (1) {
    // 무한 루프: 이 상태에서 종료
  }
}
