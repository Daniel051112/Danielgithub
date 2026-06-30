// 作业5 (ex05): 多档位触摸调速呼吸灯 [修复版]
// 功能说明：
// LED持续自动运行呼吸灯明暗渐变效果；
// 每触摸一次触摸引脚，循环切换呼吸速度档位，切换顺序：慢速 → 中速 → 快速 → 慢速...
// 修复优化点：
// 将触摸检测嵌入呼吸循环的每一次亮度步进中，每个亮度变化步骤都会执行触摸检测
// 实现随时触摸可立刻响应档位切换，最大延迟仅单次步进的延时，无长时间等待

// 硬件引脚定义
const int ledPin = 2;        // LED控制引脚GPIO2，外接面包板LED，使用LEDC硬件PWM驱动
const int touchPin = T0;     // 电容触摸引脚T0，对应物理GPIO4，ESP32内置触摸检测通道

// ========== PWM波形配置参数区 ==========
const int freq = 5000;               // PWM输出频率5000Hz，人眼无频闪，适合LED调光
const int resolution = 8;            // PWM分辨率8bit，亮度取值范围 0~255
const int maxDuty = 255;             // 最大占空比，对应LED满亮度完全点亮
// ======================================

// ========== 触摸检测参数区 ==========
const int TOUCH_THRESHOLD = 500;     // 触摸判定阈值：touchRead数值小于500，判定为人体触摸引脚
const unsigned long DEBOUNCE_MS = 150; // 触摸防抖延时150ms，过滤人手抖动、电路杂波误触发
// ====================================

// ========== 三档呼吸速度参数数组 ==========
const int SPEED_DELAYS[] = { 15, 6, 2 };   // 每一步亮度变化后的延时(ms)，数值越大呼吸越慢
const int SPEED_STEPS[]  = { 1,  3, 6 };   // 单次循环亮度变化步长，步长越大亮度跳变幅度越大
const char* SPEED_NAMES[] = {"慢速(~8s)", "中速(~1.5s)", "快速(~0.3s)"}; // 各档位文字说明，串口打印显示
const int NUM_SPEEDS = 3;                  // 总档位数量：3档速度循环切换
// ==========================================

int speedLevel = 0; // 当前呼吸速度档位，初始上电默认0档慢速

// 触摸状态全局变量，跨loop循环保存状态，用于防抖判断
bool lastTouchState = false;        // 上一次读取到的原始触摸电平状态
unsigned long lastDebounceTime = 0; // 触摸状态发生变化时的毫秒时间戳，防抖计时基准
bool stableTouchState = false;      // 防抖完成后的稳定触摸状态，确认真实触摸/松开动作

/**
 * @brief 系统初始化函数setup，上电仅执行1次
 */
void setup() {
  Serial.begin(115200); // 初始化硬件串口，波特率115200，用于打印档位切换调试信息
  // 绑定LED引脚到LEDC硬件PWM通道，配置频率与分辨率，无需手动设置pinMode输出
  ledcAttach(ledPin, freq, resolution);

  // 串口打印项目启动标题与初始档位信息
  Serial.println("==========================================");
  Serial.println("ex05: 多档位触摸调速呼吸灯 (修复版)");
  Serial.print("当前: ");
  Serial.print(SPEED_NAMES[speedLevel]);
  Serial.println(" | 触摸切换档位");
  Serial.println("==========================================");
}

/**
 * @brief 主循环函数loop，初始化完成后无限循环执行
 * 整体逻辑：完整执行一次完整呼吸（由暗变亮→由亮变暗），每一步亮度变更都调用触摸检测
 */
void loop() {
  // 读取当前档位对应的单步延时、亮度变化步长
  int delayMs = SPEED_DELAYS[speedLevel];
  int step = SPEED_STEPS[speedLevel];

  // ===== 阶段1：LED从0亮度逐步变亮到最大亮度 =====
  // duty：PWM占空比，0=完全熄灭，maxDuty=255满亮度
  for (int duty = 0; duty <= maxDuty; duty += step) {
    ledcWrite(ledPin, duty); // 设置当前LED亮度
    checkTouch();            // 【核心修复逻辑】每一次亮度更新都执行触摸检测，随时响应触摸
    delay(delayMs);          // 保持当前亮度固定时长，控制呼吸顺滑度
  }

  // ===== 阶段2：LED从最大亮度逐步变暗至完全熄灭 =====
  for (int duty = maxDuty; duty >= 0; duty -= step) {
    ledcWrite(ledPin, duty); // 设置当前LED亮度
    checkTouch();            // 每一步亮度变化都执行触摸检测，无长时间阻塞等待
    delay(delayMs);          // 保持当前亮度固定时长
  }
}

/**
 * @brief 触摸检测函数，每次亮度步进都会调用，实现无延迟触摸响应
 * 功能：读取触摸引脚数值 + 软件防抖处理 + 触摸触发时切换呼吸速度档位
 */
void checkTouch() {
  int touchValue = touchRead(touchPin);               // 读取T0(GPIO4)触摸引脚原始模拟数值
  bool currentTouchState = (touchValue < TOUCH_THRESHOLD); // 判定当前是否有人触摸

  // 检测到触摸状态发生变化（触摸/松开），重置防抖计时起点
  if (currentTouchState != lastTouchState) {
    lastDebounceTime = millis();
  }

  // 防抖延时150ms结束，确认触摸信号稳定无抖动
  if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
    // 防抖后的稳定状态和上一轮稳定状态不一致，代表真实触摸/松开动作
    if (currentTouchState != stableTouchState) {
      stableTouchState = currentTouchState; // 更新稳定触摸状态

      // 判定为【按下触摸】动作，执行档位循环切换
      if (currentTouchState == true) {
        // 取模运算实现0→1→2→0循环，三档无限循环切换
        speedLevel = (speedLevel + 1) % NUM_SPEEDS;
        // 串口打印切换后的档位参数，方便调试查看
        Serial.print(">>> [切换] ");
        Serial.print(SPEED_NAMES[speedLevel]);
        Serial.print(" (delay=");
        Serial.print(SPEED_DELAYS[speedLevel]);
        Serial.print("ms, step=");
        Serial.print(SPEED_STEPS[speedLevel]);
        Serial.println(")");
      }
    }
  }

  // 更新原始触摸状态缓存，用于下一次循环的状态对比
  lastTouchState = currentTouchState;
}