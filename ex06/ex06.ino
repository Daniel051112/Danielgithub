// ex06 双通道反向PWM渐变警车灯程序
#define LEDA 2    // 优先引脚GPIO2，面包板第一路外接LED
#define LEDB 5     // 次选引脚GPIO5，面包板第二路外接LED

// PWM亮度基础数值0~255
int pwmValue = 0;
// 亮度变化步长
int step = 1;

void setup() {
  // 两路外接LED引脚全部设为输出
  pinMode(LEDA, OUTPUT);
  pinMode(LEDB, OUTPUT);
  // 开启串口调试
  Serial.begin(115200);
  Serial.println("ex06 dual reverse pwm light start, ledA:GPIO2, ledB:GPIO5");
}

void loop() {
  // A灯使用原始亮度值输出
  analogWrite(LEDA, pwmValue);
  // B灯亮度取255-pwmValue，与A灯亮度完全反相
  analogWrite(LEDB, 255 - pwmValue);

  // 亮度持续递增
  pwmValue += step;
  // 到达亮度极值后反向变化
  if (pwmValue >= 255 || pwmValue <= 0) {
    step = -step;
  }
  // 渐变平滑延时
  delay(8);
}