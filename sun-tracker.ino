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
const int lowerLeftResPin = A4;
const int lowerRightResPin = A5;
const int upperRightResPin = A2;
const int upperLeftResPin = A1;

bool override = false;

int posX = 0;
int posY = 0;
const int minX = 0;
const int maxX = 180;
const int minY = 35;
const int maxY = 151;
int lowerLeftResVal = 0;
int lowerRightResVal = 0;
int upperRightResVal = 0;
int upperLeftResVal = 0;
int upperRes = 0;
int lowerRes = 0;
int rightRes = 0;
int leftRes = 0;
int blendedValue = 0;
int diffTolerance = 40; // photoresisitors return a range of values, this was done with trial and error

int moveX = 0;
int moveY = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // put your setup code here, to run once:
  servoX.attach(servoXPin);
  servoY.attach(servoYPin);

  pinMode(chipSelect, OUTPUT);
  pinMode(lowerLeftResPin, INPUT);
  pinMode(lowerRightResPin, INPUT);
  pinMode(upperRightResPin, INPUT);
  pinMode(upperLeftResPin, INPUT);

  if (!SD.begin(10)) {
    Serial.println("Initialization failed");
  }

  posX = servoX.read();
  posY = servoY.read();
  moveToPositionXY(posX, servoX, posY, servoY); // should maintain starting position, but seems to be buggy
}

void loop() {
    lowerLeftResVal = analogRead(lowerLeftResPin);
    upperLeftResVal = analogRead(upperLeftResPin);
    lowerRightResVal = analogRead(lowerRightResPin);
    upperRightResVal = analogRead(upperRightResPin);

    leftRes = (upperLeftResVal - lowerLeftResVal)/2;
    rightRes = (upperRightResVal - lowerRightResVal)/2;
    upperRes = (upperLeftResVal - upperRightResVal)/2;
    lowerRes = (lowerLeftResVal - lowerRightResVal)/2;

    moveX = 0;
    moveY = 0;

    Serial.print("Left res:");
    Serial.print(leftRes);
    Serial.print(", ");
    Serial.print("Right res:");
    Serial.print(rightRes);
    Serial.print(", ");
    Serial.print("Upper res:");
    Serial.print(upperRes);
    Serial.print(", ");
    Serial.print("Lower res:");
    Serial.print(lowerRes);
    Serial.println(", ");
    

    if (leftRes - rightRes > diffTolerance) {
      moveX = 1;
    }
    if (leftRes - rightRes < -diffTolerance) {
      moveX = -1;
    }
    if (upperRes - lowerRes > diffTolerance) {
      moveY = -1;
    }
    if (upperRes - lowerRes < -diffTolerance) {
      moveY = 1;
    }

    posX = servoX.read();
    posY = servoY.read();

    moveToPositionXY(posX + moveX, servoX, posY + moveY, servoY);

    // initial finding phase will have this go all over
    blendedValue =(upperRes + lowerRes)/2;
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
