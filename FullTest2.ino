const int encoderPinA = 8; //CLK pin
const int encoderPinB = 9; //DT pin
const int encoderPinBtn = 10; //SW pin

const int pinPwm = 3;
const int pinDir = 2;

const int limitSensorBottomPinA = 4; // White cable
const int limitSensorBottomPinB = 5; //Blue cable
const int limitSensorTopPinA = 6; // White cable
const int limitSensorTopPinB = 7; // Blue cable
// Button Switch/Sensor only reads/writes to White cable
// Magnetic limit Switch/Sensor reads/writes to both White and Blue cable, but both values seem to be the same
// Both Switchs/Sensors output 0 (LOW) when detecting something and 1 (HIGH) when not detecting something

int count = 0;
int encoderPinA_prev;
int encoderPinA_val;
bool CW;
bool Moving;
bool BtnPressed;
bool BtnToggle = false;
int BtnPrev;

static int iSpeed = 255; //max speed 255? (orginally 0)

bool atTop;
bool atBottom;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.flush();

  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(encoderPinBtn, INPUT_PULLUP);

  pinMode(pinPwm, OUTPUT);
  pinMode(pinDir, OUTPUT);

  pinMode(limitSensorBottomPinA, INPUT_PULLUP);
  pinMode(limitSensorBottomPinB, INPUT_PULLUP);
  pinMode(limitSensorTopPinA, INPUT_PULLUP);
  pinMode(limitSensorTopPinB, INPUT_PULLUP);

  encoderPinA_prev = digitalRead(encoderPinA);
  BtnPrev = digitalRead(encoderPinBtn);

  // atTop = false;
  // atBottom = true; //Assuming that cart starts at bottom by defalt
}

void loop() {
  // put your main code here, to run repeatedly:
  encoderPosMath();
  limitSensorStuff();

  // Positive speed = CW  (turn dial to the right)
  // Negative speed = CCW (turn dial to the left)
  // Speed is reversed because of gears making motor reversed
  if (BtnToggle) {
    up();
    down();
  } else {
    analogWrite(pinPwm, 0);
  }


  if (Serial.available() > 0) {
    if (Serial.find("stop") == true) {
      exit(0);
    }
  }
  //delay(10);
}

void encoderPosMath() {
  encoderPinA_val = digitalRead(encoderPinA);

  if (encoderPinA_val != encoderPinA_prev) {
    Moving = true;
    if (digitalRead(encoderPinB) != encoderPinA_val) {
      count++;
      CW = true;
    } else {
      count--;
      CW = false;
    }
    // if (CW) {
    //   Serial.print("Clockwise | ");
    // } else {
    //   Serial.print("Counter-Clockwise | ");
    // }
    // Serial.print(count);
    // Serial.println(" | ");
  } else {
    Moving = false;
  }
  encoderPinA_prev = encoderPinA_val;
  if (digitalRead(encoderPinBtn) == LOW) {
    //Serial.println("Button Pressed");
    BtnPressed = true;
  } else if (BtnPressed && digitalRead(encoderPinBtn) == HIGH) {
    BtnPressed = false;
    //Serial.println("Button Released");
    BtnToggle = !BtnToggle;
    //Serial.println(BtnToggle);
  }// else  BtnPressed = false;
  BtnPrev = digitalRead(encoderPinBtn);
}

void up() {
  if (CW) {
    if (!atTop) {
      analogWrite(pinPwm, iSpeed);
      digitalWrite(pinDir, HIGH);
    }
  }
}
void down() {
  if (!CW) {
    if (!atBottom) {
      analogWrite(pinPwm, iSpeed);
      digitalWrite(pinDir, LOW);
    }
  }
}

void limitSensorStuff() {
  if (digitalRead(limitSensorTopPinA) == LOW) { // Push button
    atTop = true;
    atBottom = false;
  }
  if (digitalRead(limitSensorBottomPinA) == LOW || digitalRead(limitSensorBottomPinB) == LOW) { // Magnetic limit switch
    atBottom = true;
    atTop = false;
  }
  // Serial.print("At Top: ");
  // Serial.print(atTop);
  // Serial.print(" | At Bottom: ");
  // Serial.println(atBottom);
}
