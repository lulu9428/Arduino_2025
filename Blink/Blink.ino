/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://docs.arduino.cc/hardware/

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://docs.arduino.cc/built-in-examples/basics/Blink/
*/

// the setup function runs once when you press reset or power the board
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

// 顏色表 (紅, 綠, 藍)
int colors[][3] = {
  {255,   0,   0},  // 紅
  {255, 127,   0},  // 橙
  {255, 255,   0},  // 黃
  {0,   255,   0},  // 綠
  {0,     0, 255},  // 藍
  {75,    0, 130},  // 靛
  {148,   0, 211}   // 紫
};

const int steps = 100;    // 漸變步數，數字越大越平滑
const int delayTime = 20; // 每步延遲 (ms)，越大變化越慢

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  // 先順向：紅 -> 紫
  for (int i = 0; i < 6; i++) {
    fadeColor(colors[i], colors[i + 1]);
  }
  // 再反向：紫 -> 紅
  for (int i = 6; i > 0; i--) {
    fadeColor(colors[i], colors[i - 1]);
  }
}

// 顏色漸變函式
void fadeColor(int startColor[3], int endColor[3]) {
  for (int step = 0; step <= steps; step++) {
    int r = map(step, 0, steps, startColor[0], endColor[0]);
    int g = map(step, 0, steps, startColor[1], endColor[1]);
    int b = map(step, 0, steps, startColor[2], endColor[2]);

    analogWrite(redPin,   r);
    analogWrite(greenPin, g);
    analogWrite(bluePin,  b);

    delay(delayTime);
  }
}
