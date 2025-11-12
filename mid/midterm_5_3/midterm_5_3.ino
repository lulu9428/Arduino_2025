const int buttonPin = 2;  // 按鈕腳位
const int R = 9;
const int B = 10;
const int G = 11;

int mode = 1;  // 目前模式（1:輪換, 2:黃閃, 3:紅閃）
int lastButtonState = HIGH;
bool buttonPressed = false;
bool inBreathMode = false;
int savedMode = 1;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

unsigned long previousMillis = 0;
unsigned long buttonPressTime = 0;

int colorState = 0;  // 模式1用於輪換
int breathBrightness = 0;
int breathDirection = 1; // 1=變亮, -1=變暗
bool breathRed = true;   // true=紅呼吸, false=藍呼吸

void setup() {
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  int reading = digitalRead(buttonPin);
  unsigned long currentMillis = millis();

  // --- 按鈕防抖 ---
  if (reading != lastButtonState) {
    lastDebounceTime = currentMillis;
  }

  // --- 按鈕行為判斷 ---
  if ((currentMillis - lastDebounceTime) > debounceDelay) {
    // 按鈕剛被按下
    if (reading == LOW && !buttonPressed) {
      buttonPressed = true;
      buttonPressTime = currentMillis;
    }

    // 按鈕放開
    if (reading == HIGH && buttonPressed) {
      unsigned long pressDuration = currentMillis - buttonPressTime;

      if (inBreathMode) {
        // 若在呼吸燈模式 → 放開後回原模式
        inBreathMode = false;
        restorePreviousMode();
      } else if (pressDuration < 1500) {
        // 短按 → 切換模式
        mode++;
        if (mode > 3) mode = 1;
      }
      buttonPressed = false;
    }
  }

  // 檢查是否長按超過 1.5 秒進入呼吸模式
  if (buttonPressed && !inBreathMode && (currentMillis - buttonPressTime >= 1500)) {
    inBreathMode = true;
    savedMode = mode;  // 記錄當前模式
    startBreathing();
  }

  lastButtonState = reading;

  // === 若進入呼吸燈模式 ===
  if (inBreathMode) {
    runBreathing(currentMillis);
    return;
  }

  // === 若按住但未達長按（短暫關燈） ===
  if (buttonPressed && !inBreathMode) {
    digitalWrite(R, HIGH);
    digitalWrite(G, HIGH);
    digitalWrite(B, HIGH);
    return;
  }

  // === 模式執行 ===
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
        colorState = (colorState + 1) % 3;

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

    case 2:  // 黃燈閃爍
      if (currentMillis - previousMillis >= 500) {
        previousMillis = currentMillis;
        static bool ledState = false;
        ledState = !ledState;
        digitalWrite(R, ledState ? 0 : 1);
        digitalWrite(G, ledState ? 0 : 1);
        digitalWrite(B, 1);
      }
      break;

    case 3:  // 紅燈閃爍
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

// === 呼吸燈初始化 ===
void startBreathing() {
  breathBrightness = 0;
  breathDirection = 1;
  breathRed = true;
  analogWrite(R, 255);
  analogWrite(G, 255);
  analogWrite(B, 255);
}

// === 呼吸燈運作 ===
void runBreathing(unsigned long currentMillis) {
  static unsigned long lastUpdate = 0;
  if (currentMillis - lastUpdate >= 20) {  // 每 20ms 更新亮度
    lastUpdate = currentMillis;
    breathBrightness += breathDirection * 5;
    if (breathBrightness >= 255) {
      breathBrightness = 255;
      breathDirection = -1;
    } else if (breathBrightness <= 0) {
      breathBrightness = 0;
      breathDirection = 1;
      breathRed = !breathRed;  // 切換顏色
    }

    // 0=亮 → 255=滅 → 所以取反
    int outputValue = 255 - breathBrightness;
    if (breathRed) {
      analogWrite(R, outputValue);
      analogWrite(G, 255);
      analogWrite(B, 255);
    } else {
      analogWrite(R, 255);
      analogWrite(G, 255);
      analogWrite(B, outputValue);
    }
  }
}

// === 回復長按前的模式 ===
void restorePreviousMode() {
  mode = savedMode;
  previousMillis = millis(); // 重設時間，避免閃爍節奏錯亂
  digitalWrite(R, HIGH);
  digitalWrite(G, HIGH);
  digitalWrite(B, HIGH);
}
