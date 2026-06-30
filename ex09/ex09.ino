// ex09 触摸传感器实时数据网页仪表盘，AJAX无刷新更新数值
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Daniel";
const char* password = "123456whm";
const int TOUCH_PIN = 4;
const int TOUCH_THRESHOLD = 150;

WebServer server(80);

String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>实验9 Touch实时数据面板</title>
</head>
<body style="font-family:Arial; text-align:center; margin-top:80px;">
  <h1>GPIO4 Touch Raw Value Dashboard</h1>
  <div style="font-size:60px;color:red;margin:40px 0;">
    Touch Data: <span id="data">0</span>
  </div>
<script>
// 定时异步拉取数值，页面不刷新，页面状态保持不变
function refreshData(){
  fetch("/getValue")
  .then(res=>res.text())
  .then(val=>{
    document.getElementById("data").innerText = val;
  })
}
setInterval(refreshData, 100);
</script>
</body>
</html>
)rawliteral";
  return html;
}

void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

void handleGetValue() {
  int rawTouch = touchRead(TOUCH_PIN);
  server.send(200, "text/plain", String(rawTouch));
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected successfully");
  Serial.print("Dashboard web address: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/getValue", handleGetValue);
  server.begin();
}

void loop() {
  server.handleClient();
}
