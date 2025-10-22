const int buttonPin = 2;  // Pushbutton pin
const int ledPinr = 8;    // Red LED pin
const int ledPinb = 9;    // Blue LED pin
const int ledPing = 10;   // Green LED pin

// Variables
int buttonState = 0;  // Pushbutton status
bool buttonPress = false;
unsigned long pressTime = 0;
const int longpressInterval = 2000;

int lightNum = 0;
int Rlight = 0;
int Glight = 0;
int Blight = 0;
int Rcurrlight = 0;
int Gcurrlight = 0;
int Bcurrlight = 0;

const int fadeAmount = 2;
int fadeDirection = 1;

int currentMode = 0;
unsigned long blinkTimer = 0;
const int blinkInterval = 500;
bool blinkOn = true;

void setup() {
  pinMode(ledPinr, OUTPUT);
  pinMode(ledPing, OUTPUT);
  pinMode(ledPinb, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  checkButton();
  updateLEDColor();
  setRGBColor(Rcurrlight, Gcurrlight, Bcurrlight);
}

void checkButton() {
  buttonState = digitalRead(buttonPin);

  // Simple debounce: only process button state after stabilization
  static unsigned long lastDebounceTime = 0;
  const int debounceDelay = 50;
  if (millis() - lastDebounceTime < debounceDelay) {
    return;
  }

  if (buttonState == LOW && !buttonPress) {
    pressTime = millis();
    buttonPress = true;
    lastDebounceTime = millis();
  }
  if (buttonState == HIGH && buttonPress) {
    unsigned long currentTime = millis();
    if (currentTime - pressTime < longpressInterval) {
      Serial.println("short");
      changeLEDColor();
    } else {
      Serial.println("long");
      changeMode();
    }
    buttonPress = false;
    lastDebounceTime = millis();
  }
}

void changeLEDColor() {
  lightNum = (lightNum + 1) % 5;
  switch (lightNum) {
    case 0:
      Rlight = 0;
      Glight = 0;
      Blight = 0;
      break;
    case 1:
      Rlight = 0;
      Glight = 255;
      Blight = 255;
      break;
    case 2:
      Rlight = 255;
      Glight = 0;
      Blight = 255;
      break;
    case 3:
      Rlight = 255;
      Glight = 255;
      Blight = 0;
      break;
    case 4:
      Rlight = 0;
      Glight = 0;
      Blight = 255;
      break;
  }
  Rcurrlight = Rlight;
  Gcurrlight = Glight;
  Bcurrlight = Blight;
}

void setRGBColor(int r, int g, int b) {
  analogWrite(ledPinr, r);
  analogWrite(ledPing, g);
  analogWrite(ledPinb, b);
}

void changeMode() {
  currentMode = (currentMode + 1) % 3;
  if (currentMode == 1) {
    blinkTimer = 0;
    blinkOn = true;
  } else if (currentMode == 2) {
    fadeDirection = 1;
  }
}

void updateLEDColor() {
  if (currentMode == 0) {
    Rcurrlight = Rlight;
    Gcurrlight = Glight;
    Bcurrlight = Blight;
  } else if (currentMode == 1) {
    unsigned long currentTime = millis();
    if (currentTime - blinkTimer > blinkInterval) {
      blinkOn = !blinkOn;
      blinkTimer = currentTime;
    }
    if (blinkOn) {
      Rcurrlight = Rlight;
      Gcurrlight = Glight;
      Bcurrlight = Blight;
    } else {
      Rcurrlight = 0;
      Gcurrlight = 0;
      Bcurrlight = 0; // Blink off state is black, not white
    }
  } else if (currentMode == 2) {
    bool negativeDir = false;
    // Fade all channels regardless of target value
    Rcurrlight = Rcurrlight + fadeDirection * fadeAmount;
    Gcurrlight = Gcurrlight + fadeDirection * fadeAmount;
    Bcurrlight = Bcurrlight + fadeDirection * fadeAmount;

    // Check bounds and reverse direction if needed
    if (Rcurrlight > 255 || Rcurrlight < 0) {
      negativeDir = true;
      Rcurrlight = constrain(Rcurrlight, 0, 255);
    }
    if (Gcurrlight > 255 || Gcurrlight < 0) {
      negativeDir = true;
      Gcurrlight = constrain(Gcurrlight, 0, 255);
    }
    if (Bcurrlight > 255 || Bcurrlight < 0) {
      negativeDir = true;
      Bcurrlight = constrain(Bcurrlight, 0, 255);
    }
    if (negativeDir) {
      fadeDirection = -fadeDirection;
    }
    delay(10); // Only delay in fading mode
  }}