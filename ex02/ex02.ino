// 定义LED引脚，ESP32通常板载LED连接在GPIO 2
const int ledPin = 2;
// millis计时相关变量
unsigned long lastTime = 0;
// 500ms切换一次亮灭，周期1000ms = 1Hz
const unsigned long interval = 500;
bool ledState = LOW;

void setup() {
  // 初始化串口通信，设置波特率为115200
  Serial.begin(115200);
  // 将LED引脚设置为输出模式
  pinMode(ledPin, OUTPUT);
}

void loop() {
  unsigned long currentTime = millis();
  if(currentTime - lastTime >= interval){
    lastTime = currentTime;
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    if(ledState){
      Serial.println("LED ON");  // 串口输出提示
    }else{
      Serial.println("LED OFF"); // 串口输出提示
    }
  }
}