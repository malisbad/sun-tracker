// Y servo maximum range 32 -> 151

#include <Servo.h>

Servo servoX;
Servo servoY;

const int servoXPin = 9;
const int servoYPin = 10;
const int advanceYOneDeg = 3;
const int retardYOneDeg = 2;
const int overridePin = 12;
const int advX = 5;
const int retX = 4;
const int leftResPin = A5;
const int rightResPin = A4;

bool override = false;

int posX = 0;
int posY = 0;
const int minX = 0;
const int maxX = 180;
const int minY = 35;
const int maxY = 151;
int leftResVal = 0;
int rightResVal = 0;

int lightDiff = 0;
int movingLightDiff = 0;

void setup() {
  // put your setup code here, to run once:
  servoX.attach(9);
  servoY.attach(10);

  pinMode(advanceYOneDeg, INPUT);
  pinMode(retardYOneDeg, INPUT);
  pinMode(advX, INPUT);
  pinMode(retX, INPUT);
  pinMode(leftResPin, INPUT);
  pinMode(rightResPin, INPUT);
  pinMode(overridePin, INPUT);

  Serial.begin(9600);
  posX = servoX.read();
  posY = servoY.read();
  printPosition(posX, posY);
  moveToPositionXY(posX, servoX, posY, servoY);

  leftResVal = analogRead(leftResPin);
  rightResVal = analogRead(rightResPin);
  lightDiff = leftResVal - rightResVal;
}

void loop() {
  if (digitalRead(overridePin) == HIGH) {
    override = !override;
  }

  if (override) {
    // manual debugging controls
    Serial.println("Override active");
    if (digitalRead(advanceYOneDeg) == HIGH && posY <= 152) {
      moveToPosition(posY, posY + 1, servoY);
    }
    if (digitalRead(retardYOneDeg) == HIGH && posY >= 35) {
      moveToPosition(posY, posY - 1, servoY);
    }
    if (digitalRead(advX) == HIGH && posX <= 270) {
      moveToPosition(posX, posX + 1, servoX);
    }
    if (digitalRead(retX) == HIGH && posX >= 0) {
      moveToPosition(posX, posX - 1, servoX);
    }
  } else {
    leftResVal = analogRead(leftResPin);
    rightResVal = analogRead(rightResPin);

    if (leftResVal - rightResVal > 60) {
      moveToPositionXY(posX + 1, servoX, posY, servoY);
    }
    if (leftResVal - rightResVal < -60) {
      moveToPositionXY(posX - 1, servoX, posY, servoY);
    }

    printPosition(posX, posY);
    posX = servoX.read();
    posY = servoY.read();
  }
}

void moveToPositionXY(int newPosX, Servo servX, int newPosY, Servo servY) {
  if (posX >= minX && posX <= maxX) {
    moveToPosition(servoX.read(), newPosX, servX);
  }
  if (posY < maxY && posY > minY) {
    moveToPosition(servoY.read(), newPosY, servY);
  }
}

void moveToPosition(int currentPos, int newPos, Servo servo) {
  if (currentPos < newPos) {
    for (currentPos; currentPos <= newPos; currentPos += 1) { // goes from 0 degrees to 180 degrees in steps of 1 degree
      servo.write(currentPos);              // tell servo to go to position
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  } else if (currentPos >= newPos) {
    for (currentPos; currentPos >= newPos; currentPos -= 1) { // goes from 180 degrees to 0 degrees
      servo.write(currentPos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  } else {
    ;
  }
  delay(10);
}

void printPosition(int x, int y) {
  Serial.print("Current position: ");
  Serial.print(x);
  Serial.print(" , ");
  Serial.println(y);
}
