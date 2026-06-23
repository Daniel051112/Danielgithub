// ex02 millis 1Hz闪烁 ESP32 GPIO2
const int ledPin = 2;
unsigned long lastTime = 0;
const unsigned long interval = 500;
bool ledState = LOW;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  unsigned long currentTime = millis();
  if(currentTime - lastTime >= interval){
    lastTime = currentTime;
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
}