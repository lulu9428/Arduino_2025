// ==== RGB Rainbow by Potentiometer ====
// R=3, G=5, B=4, Pot=A0

int R = 3;
int G = 5;
int B = 4;
int potPin = A0;

void setup() {
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
}

void loop() {
  int potValue = analogRead(potPin); // 0~1023
  float hue = map(potValue, 0, 1023, 0, 255); // 映射到0~255色相值

  int r, g, b;
  hsvToRgb(hue, 255, 255, r, g, b); // 全飽和、全亮度的顏色

  analogWrite(R, r);
  analogWrite(G, g);
  analogWrite(B, b);

  delay(10);
}

// ---- HSV 轉 RGB ----
// hue: 0~255, sat: 0~255, val: 0~255
void hsvToRgb(int hue, int sat, int val, int &r, int &g, int &b) {
  float h = hue / 42.5; // 255/6 ≈ 42.5
  int i = int(h);
  float f = h - i;
  float p = val * (1 - sat / 255.0);
  float q = val * (1 - sat / 255.0 * f);
  float t = val * (1 - sat / 255.0 * (1 - f));

  switch (i) {
    case 0: r = val; g = t; b = p; break;
    case 1: r = q; g = val; b = p; break;
    case 2: r = p; g = val; b = t; break;
    case 3: r = p; g = q; b = val; break;
    case 4: r = t; g = p; b = val; break;
    default: r = val; g = p; b = q; break;
  }
}