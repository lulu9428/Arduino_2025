const int buttonPin = 2;  // 按鈕腳位
const int R = 9;
const int B = 10;
const int G = 11;

int mode = 1;  // 目前模式
int lastButtonState = LOW;
bool buttonPressed = false;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

unsigned long previousMillis = 0;  // 時間紀錄
unsigned long interval = 2000;     // 預設輪換間隔

int colorState = 0;  // 用於模式1輪換階段

void setup() {
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  // === 按鈕偵測區 ===
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // 檢測到由 HIGH -> LOW 的短按（反轉）
    if (reading == LOW && !buttonPressed) {
      buttonPressed = true;
      mode++;
      if (mode > 3) mode = 1;
    } else if (reading == HIGH) {
      buttonPressed = false;  // 放開後才能再次觸發
    }
  }

  lastButtonState = reading;

  // === 按住按鈕時 → 全關 ===
  if (reading == LOW) {  // ⚠️反轉：按住時 LOW → 關燈
    digitalWrite(R, HIGH);
    digitalWrite(G, HIGH);
    digitalWrite(B, HIGH);
    return;
  }

  // === 模式執行 ===
  unsigned long currentMillis = millis();

  switch (mode) {
    case 1:  // 綠黃紅輪換
      if (previousMillis == 0) {  // 表示剛進入此模式
    previousMillis = currentMillis;
    colorState = 0;
    digitalWrite(R, 1);
    digitalWrite(G, 0);
    digitalWrite(B, 1);
  }
      if (currentMillis - previousMillis >= 2000) {
        previousMillis = currentMillis;
        colorState = (colorState + 1) % 3;  // 0,1,2 循環

        if (colorState == 0) {  // 綠
          digitalWrite(R, 1);
          digitalWrite(G, 0);
          digitalWrite(B, 1);
        } else if (colorState == 1) {  // 黃
          digitalWrite(R, 0);
          digitalWrite(G, 0);
          digitalWrite(B, 1);
        } else {  // 紅
          digitalWrite(R, 0);
          digitalWrite(G, 1);
          digitalWrite(B, 1);
        }
      }
      break;

    case 2:  // 黃燈閃爍 0.5秒
      if (currentMillis - previousMillis >= 500) {
        previousMillis = currentMillis;
        static bool ledState = false;
        ledState = !ledState;
        digitalWrite(R, ledState ? 0 : 1);
        digitalWrite(G, ledState ? 0 : 1);
        digitalWrite(B, 1);
      }
      break;

    case 3:  // 紅燈閃爍 0.5秒
      if (currentMillis - previousMillis >= 500) {
        previousMillis = currentMillis;
        static bool ledState = false;
        ledState = !ledState;
        digitalWrite(R, ledState ? 0 : 1);
        digitalWrite(G, 1);
        digitalWrite(B, 1);
      }
      break;
  }
}
