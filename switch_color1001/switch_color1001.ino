int buttonState = 0;
int ledcolor = 0;

bool ButtonPressed = false;
String currentcolor = "led";

void setup() {
  // put your setup code here, to run once:
pinMode (3, OUTPUT);
pinMode (4, OUTPUT);
pinMode (5, OUTPUT);
pinMode(2, INPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
buttonState = digitalRead(2);
Serial.print("Current Color: ");
Serial.println(currentcolor);

if(buttonState == HIGH && !ButtonPressed){
  ledcolor = ledcolor + 1;
  ButtonPressed = true;
}
if(buttonState == LOW && ButtonPressed){
  ButtonPressed = false;
}

if(ledcolor==0){
  currentcolor ="LED off";
    digitalWrite(3,1);
    digitalWrite(5,1);
    digitalWrite(4,1);
  }
  else if(ledcolor==1){
    currentcolor ="Red";
    digitalWrite(3,0);
    digitalWrite(5,1);
    digitalWrite(4,1);
  }
  else if(ledcolor==2){
    currentcolor ="Green";
    digitalWrite(3,1);
    digitalWrite(5,0);
    digitalWrite(4,1);
  }
  else if(ledcolor==3){
    currentcolor ="Blue";
    digitalWrite(3,1);
    digitalWrite(5,1);
    digitalWrite(4,0);
  }
  else if(ledcolor==4){
    currentcolor ="Yellow";
    digitalWrite(3,0);
    digitalWrite(5,0);
    digitalWrite(4,1);
  }
  else if(ledcolor==5){
    currentcolor ="Purple";
    digitalWrite(3,0);
    digitalWrite(5,1);
    digitalWrite(4,0);
  }
  else if(ledcolor==6){
    currentcolor ="Cyan";
    digitalWrite(3,1);
    digitalWrite(5,0);
    digitalWrite(4,0);
  }
  else if(ledcolor==7){
    currentcolor ="White";
    digitalWrite(3,0);
    digitalWrite(5,0);
    digitalWrite(4,0);
  }
  else if(ledcolor==8){
    ledcolor=0;
  }
}

