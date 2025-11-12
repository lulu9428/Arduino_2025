/*
  Button

  Turns on and off a light emitting diode(LED) connected to digital pin 13,
  when pressing a pushbutton attached to pin 2.

  The circuit:
  - LED attached from pin 13 to ground through 220 ohm resistor
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.

  created 2005
  by DojoDave <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  https://docs.arduino.cc/built-in-examples/digital/Button/
*/

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;  // the number of the pushbutton pin
const int ledPin = 3;    // the number of the LED pin

// variables will change:
int buttonState = 0;
int lastButtonState = HIGH;
unsigned long pressTime = 0;
bool isPressing = false;
const long longPressTime = 1000;  // 長按 1 秒（可保留，暫不使用）

// === 閃爍控制 ===
int blinkRate = 0;  // 0:恆亮, 1:慢閃, 2:中閃, 3:快閃
unsigned long previousMillis = 0;
bool ledState = false;

// 閃爍間隔（毫秒）
const int blinkIntervals[4] = {0, 1000, 500, 200};  // 0=恆亮

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);  // 內建上拉，按下為 LOW
  Serial.begin(9600);
  digitalWrite(ledPin, HIGH);  // 初始恆亮
  Serial.println("系統啟動：恆亮");
}

void loop() {
  handleButton();
  updateLED();
}

// === 按鈕處理 ===
void handleButton() {
  buttonState = digitalRead(buttonPin);

  // 按下瞬間
  if (buttonState == LOW && lastButtonState == HIGH) {
    pressTime = millis();
    isPressing = true;
  }

  // 放開瞬間 → 判斷短按或長按
  if (buttonState == HIGH && lastButtonState == LOW && isPressing) {
    unsigned long pressDuration = millis() - pressTime;
    isPressing = false;

    if (pressDuration < longPressTime) {
      // === 短按：切換閃爍速率 ===
      blinkRate = (blinkRate + 1) % 4;  // 0 → 1 → 2 → 3 → 0

      // 重置計時器
      previousMillis = millis();
      ledState = true;

      // 印出狀態
      switch (blinkRate) {
        case 0: Serial.println("短按 → 恆亮"); digitalWrite(ledPin, HIGH); break;
        case 1: Serial.println("短按 → 慢閃 (1秒)"); break;
        case 2: Serial.println("短按 → 中閃 (0.5秒)"); break;
        case 3: Serial.println("短按 → 快閃 (0.2秒)"); break;
      }
    } else {
      // 長按（可保留功能）
      Serial.println("長按（未定義）");
    }
  }

  lastButtonState = buttonState;
}

// === 更新 LED 狀態 ===
void updateLED() {
  unsigned long currentMillis = millis();

  if (blinkRate == 0) {
    // 恆亮
    digitalWrite(ledPin, HIGH);
  } else {
    // 閃爍模式
    if (currentMillis - previousMillis >= blinkIntervals[blinkRate]) {
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(ledPin, ledState ? HIGH : LOW);
    }
  }
}