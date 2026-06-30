// ex07 网页PWM无极调光，AJAX异步更新，滑块位置保持不复位
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Daniel";
const char* password = "123456whm";
const int LED_PIN = 2;
WebServer server(80);
int brightnessVal = 0;

// 生成网页，页面加载时绑定当前亮度到滑块，AJAX无刷新发送亮度
String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>实验7 PWM无极调光控制台</title>
</head>
<body style="font-family:Arial; text-align:center; margin-top:50px;">
  <h1>ESP32 PWM亮度调节</h1>
  <p>拖动滑块调节LED亮度，范围0~255，滑块拖动后保持当前位置</p>
  <input type="range" min="0" max="255" value=)rawliteral" + String(brightnessVal) + R"rawliteral(" id="slider" style="width:80%;height:35px;">
  <p>当前亮度数值：<span id="num">)rawliteral" + String(brightnessVal) + R"rawliteral(</span></p>
<script>
const slider = document.getElementById("slider");
const numText = document.getElementById("num");
// 滑块拖动触发，使用fetch异步请求，页面不刷新，滑块不复位
slider.oninput = function(){
  let val = this.value;
  numText.innerText = val;
  // 异步发送亮度参数，不跳转页面
  fetch("/set?bri="+val);
}
</script>
</body>
</html>
)rawliteral";
  return html;
}

// 首页页面
void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

// 亮度设置接口，仅修改亮度，不页面跳转
void handleSetBright() {
  if(server.hasArg("bri")){
    brightnessVal = server.arg("bri").toInt();
    analogWrite(LED_PIN, brightnessVal);
  }
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);

  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected successfully");
  Serial.print("Web access address: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/set", handleSetBright);
  server.begin();
}

void loop() {
  server.handleClient();
}