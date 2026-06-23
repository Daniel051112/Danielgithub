// ex03 作业3：使用millis()函数控制LED产生SOS闪烁信息
// 定义LED引脚，ESP32通常板载LED连接在GPIO 2
const int ledPin = 2;
// SOS时序参数，与实验课件示例完全一致
const int SHORT_ON = 200;
const int LONG_ON = 600;
const int DOT_GAP = 200;
const int CHAR_GAP = 500;
const int SOS_GAP = 2000;

void setup() {
  // 初始化串口通信，设置波特率为115200
  Serial.begin(115200);
  // 将LED引脚设置为输出模式
  pinMode(ledPin, OUTPUT);
  Serial.println("SOS求救灯光程序启动");
}

void loop() {
  Serial.println("===== 开始发送一轮SOS信号 =====");
  // SOS 求救信号模式：连续三次短闪，三次长闪，再三次短闪
  // S：短闪3次
  Serial.println("发送S段：3次短闪");
  for(int i=0; i<3; i++){
    digitalWrite(ledPin, HIGH);
    delay(SHORT_ON);
    digitalWrite(ledPin, LOW);
    delay(DOT_GAP);
  }
  delay(CHAR_GAP); // 字母间隔

  // O：长闪3次
  Serial.println("发送O段：3次长闪");
  for(int i=0; i<3; i++){
    digitalWrite(ledPin, HIGH);
    delay(LONG_ON);
    digitalWrite(ledPin, LOW);
    delay(DOT_GAP);
  }
  delay(CHAR_GAP); // 字母间隔

  // S：短闪3次
  Serial.println("发送S段：3次短闪");
  for(int i=0; i<3; i++){
    digitalWrite(ledPin, HIGH);
    delay(SHORT_ON);
    digitalWrite(ledPin, LOW);
    delay(DOT_GAP);
  }
  Serial.println("一轮SOS发送完成，进入长停顿");
  delay(SOS_GAP); // 整套SOS播放完毕后较长熄灭停顿
}