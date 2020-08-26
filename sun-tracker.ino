// Y servo maximum range 32 -> 151

#include <Servo.h>
#include <SPI.h>
#include <SD.h>

// init servos
Servo servoX;
Servo servoY;

//init SD card
Sd2Card card;
SdVolume volume;
SdFile root;

File data;

const int chipSelect = 10;

const int servoXPin = 9;
const int servoYPin = 8;
const int advanceYOneDeg = 3;
const int retardYOneDeg = 2;
const int overridePin = 12;
const int advX = 5;
const int retX = 4;
const int leftResPin = A4;
const int rightResPin = A5;

bool override = false;

int posX = 0;
int posY = 0;
const int minX = 0;
const int maxX = 180;
const int minY = 35;
const int maxY = 151;
int leftResVal = 0;
int rightResVal = 0;
int blendedValue = 0;

int lightDiff = 0;
int movingLightDiff = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // put your setup code here, to run once:
  servoX.attach(servoXPin);
  servoY.attach(servoYPin);

  pinMode(chipSelect, OUTPUT);
  pinMode(advanceYOneDeg, INPUT);
  pinMode(retardYOneDeg, INPUT);
  pinMode(advX, INPUT);
  pinMode(retX, INPUT);
  pinMode(leftResPin, INPUT);
  pinMode(rightResPin, INPUT);
  pinMode(overridePin, INPUT);

  if (!SD.begin(10)) {
    Serial.println("Initialization failed");
  }

  posX = servoX.read();
  posY = servoY.read();
  moveToPositionXY(posX, servoX, posY, servoY); // should maintain starting position, but seems to be buggy

  leftResVal = analogRead(leftResPin);
  rightResVal = analogRead(rightResPin);
  lightDiff = leftResVal - rightResVal;
}

void loop() {
    leftResVal = analogRead(leftResPin);
    rightResVal = analogRead(rightResPin);

    // photoresisitors return a range of values, this was done with trial and error
    if (leftResVal - rightResVal > 60) {
      moveToPositionXY(posX + 1, servoX, posY, servoY);
    }
    if (leftResVal - rightResVal < -60) {
      moveToPositionXY(posX - 1, servoX, posY, servoY);
    }

    posX = servoX.read();
    posY = servoY.read();

    blendedValue =(leftResVal + rightResVal)/2;
    data = SD.open("data.txt", FILE_WRITE);
    if (data) {
      data.println(blendedValue);
      data.close();
    } else {
      Serial.println("Error opening file");
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
  } else if (currentPos > newPos) {
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
