int G = 11;   // 綠燈
int B = 10;   // 黃燈
int R = 9;    // 紅燈
int buttonPin = 2;  // 按鈕

int currentLight = 0; // 0=綠, 1=黃, 2=紅
unsigned long previousMillis = 0;
const long interval = 2000; // 每2秒切換
bool lightsPaused = false;  // 是否暫停（按下按鈕時）

void setup() {
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // 按鈕接 GND + 上拉電阻

  // 一開始亮綠燈
  digitalWrite(R, HIGH);
  digitalWrite(G, LOW);
  digitalWrite(B, HIGH);
}

void loop() {
  int buttonState = digitalRead(buttonPin);

  // 如果按下按鈕（LOW） → 關燈暫停
  if (buttonState == LOW) {
    lightsPaused = true;
    digitalWrite(R, HIGH);
    digitalWrite(G, HIGH);
    digitalWrite(B, HIGH);
  } 
  // 放開按鈕（HIGH） → 恢復循環
  else {
    lightsPaused = false;
  }

  // 如果沒有被暫停，就執行自動切換
  if (!lightsPaused) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      // 切換燈號
      currentLight = (currentLight + 1) % 3;

      if (currentLight == 0) {        // 綠燈
        digitalWrite(R, HIGH);
        digitalWrite(G, LOW);
        digitalWrite(B, HIGH);
      } 
      else if (currentLight == 1) {   // 黃燈
        digitalWrite(R, LOW);
        digitalWrite(G, LOW);
        digitalWrite(B, HIGH);
      } 
      else if (currentLight == 2) {   // 紅燈
        digitalWrite(R, LOW);
        digitalWrite(G, HIGH);
        digitalWrite(B, HIGH);
      }
    }
  }
}
