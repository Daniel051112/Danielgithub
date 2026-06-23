// 定义LED引脚，ESP32通常板载LED连接在GPIO 2
const int ledPin = 2;
// SOS时序参数，与课件示例完全匹配
const int SHORT_ON = 200;
const int LONG_ON = 600;
const int DOT_GAP = 200;
const int CHAR_GAP = 500;
const int SOS_GAP = 2000;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // SOS 求救信号模式：三短、三长、三短
  // S：短闪3次
  for(int i=0; i<3; i++){
    digitalWrite(ledPin, HIGH);
    delay(SHORT_ON);
    digitalWrite(ledPin, LOW);
    delay(DOT_GAP);
  }
  delay(CHAR_GAP); // 字母间隔

  // O：长闪3次
  for(int i=0; i<3; i++){
    digitalWrite(ledPin, HIGH);
    delay(LONG_ON);
    digitalWrite(ledPin, LOW);
    delay(DOT_GAP);
  }
  delay(CHAR_GAP); // 字母间隔

  // S：短闪3次
  for(int i=0; i<3; i++){
    digitalWrite(ledPin, HIGH);
    delay(SHORT_ON);
    digitalWrite(ledPin, LOW);
    delay(DOT_GAP);
  }
  delay(SOS_GAP); // 整套SOS单词长停顿
}