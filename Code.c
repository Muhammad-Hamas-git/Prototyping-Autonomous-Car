#include <math.h>

#define S0 A2
#define S1 A1
#define S2 0
#define S3 1
#define colorSensorOutput A0

// Color calibration thresholds
int redTop = 24390;
int redBottom = 27776;
int blueTop = 17543;
int blueBottom = 6756;

int redVal = 0;
int blueVal = 0;

int redFreq = 0;
int blueFreq = 0;
int redTime = 0;
int blueTime = 0;

bool detectRed = false;
bool detectBlue = false;

bool trackingWhite = true;

// Motor pin assignments
const int leftMotorPWM = 11;
const int leftMotorIn1 = 10;
const int leftMotorIn2 = 9;

const int rightMotorIn1 = 8;
const int rightMotorIn2 = 7;
const int rightMotorPWM = 6;

// Infrared sensors
const int irLeft = 2;
const int irRight = 3;

// Ultrasonic pins
const int sonarTrigger = 4;
const int sonarEcho = 5;
float obstacleDistance = 5000;

int normalSpeed = 100;
int curveSpeed = 70;

int leftIRState = 1;
int rightIRState = 1;

void readColorSensor() {
  Serial.println("=== COLOR CHECK ===");

  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  redTime = pulseIn(colorSensorOutput, LOW);
  redFreq = 1000000 / redTime;
  redVal = map(redFreq, redTop, redBottom, 255, 0);
  redVal = constrain(redVal, 0, 255);

  Serial.print("Red Freq: ");
  Serial.println(redFreq);
  Serial.print("Red Value: ");
  Serial.println(redVal);
  delay(100);

  digitalWrite(S3, HIGH);
  blueTime = pulseIn(colorSensorOutput, LOW);
  blueFreq = 1000000 / blueTime;
  blueVal = map(blueFreq, blueTop, blueBottom, 255, 0);
  blueVal = constrain(blueVal, 0, 255);

  Serial.print("Blue Freq: ");
  Serial.println(blueFreq);
  Serial.print("Blue Value: ");
  Serial.println(blueVal);
  delay(100);

  if (redVal == 255 && blueVal == 0) {
    Serial.println("No Color Detected");
  } else if (redVal < 255 && blueVal == 0) {
    Serial.println("Red Color Found");
    detectRed = true;
    detectBlue = false;
  } else if (blueVal > 0) {
    Serial.println("Blue Color Found");
    detectRed = false;
    detectBlue = true;
  }

  delay(1000);
}

void triggerSonar() {
  digitalWrite(sonarTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(sonarTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(sonarTrigger, LOW);
}

float getDistance() {
  triggerSonar();
  long duration = pulseIn(sonarEcho, HIGH, 30000);
  return duration * 0.34 / 2;
}

void runLeftMotorForward(int pwm) {
  analogWrite(leftMotorPWM, pwm);
  digitalWrite(leftMotorIn1, LOW);
  digitalWrite(leftMotorIn2, HIGH);
}

void runRightMotorForward(int pwm) {
  analogWrite(rightMotorPWM, pwm);
  digitalWrite(rightMotorIn1, HIGH);
  digitalWrite(rightMotorIn2, LOW);
}

void moveAhead(int speed) {
  runLeftMotorForward(speed);
  runRightMotorForward(speed);
}

void reverseLeftMotor(int pwm) {
  analogWrite(leftMotorPWM, pwm);
  digitalWrite(leftMotorIn1, HIGH);
  digitalWrite(leftMotorIn2, LOW);
}

void reverseRightMotor(int pwm) {
  analogWrite(rightMotorPWM, pwm);
  digitalWrite(rightMotorIn1, LOW);
  digitalWrite(rightMotorIn2, HIGH);
}

void moveBack(int speed) {
  reverseLeftMotor(speed);
  reverseRightMotor(speed);
}

void stopAllMotors() {
  analogWrite(leftMotorPWM, 0);
  analogWrite(rightMotorPWM, 0);
}

void rotate180() {
  reverseLeftMotor(normalSpeed);
  runRightMotorForward(normalSpeed);
  delay(2200);
  stopAllMotors();
}

void goAroundObstacle() {
  rotateLeft45();
  obstacleDistance = getDistance();

  if (obstacleDistance >= 10 && obstacleDistance < 200) {
    moveBack(normalSpeed);
    delay(600);
    turnRight90();
    goForwardFar();

    reverseLeftMotor(180);
    runRightMotorForward(180);
    delay(300);
    stopAllMotors();

    goForwardClose();
  } else {
    goForwardFar();
    turnRight90();
    goForwardClose();
  }
}

void checkObstacle() {
  obstacleDistance = getDistance();
  Serial.println(obstacleDistance);

  if (obstacleDistance >= 10 && obstacleDistance <= 55) {
    stopAllMotors();
    delay(2000);

    readColorSensor();
    readColorSensor();
    readColorSensor();

    if (detectRed) {
      moveBack(normalSpeed);
      delay(500);
      stopAllMotors();
      goAroundObstacle();
    } else if (detectBlue) {
      moveBack(normalSpeed);
      delay(500);
      stopAllMotors();
      parkVehicle();
    } else {
      moveBack(normalSpeed);
      delay(500);
      stopAllMotors();
      rotate180();
    }

    detectRed = false;
    detectBlue = false;
  }
}

void parkVehicle() {
  runLeftMotorForward(180);
  reverseRightMotor(180);
  delay(500);
  stopAllMotors();

  reverseLeftMotor(200);
  reverseRightMotor(220);
  delay(500);
  stopAllMotors();
}

void rotateLeft45() {
  reverseLeftMotor(180);
  runRightMotorForward(180);
  delay(200);
  stopAllMotors();
}

void turnRight90() {
  runLeftMotorForward(180);
  reverseRightMotor(180);
  delay(400);
  stopAllMotors();
}

void goForwardFar() {
  moveAhead(normalSpeed);
  delay(2500);
  stopAllMotors();
}

void goForwardClose() {
  do {
    moveAhead(normalSpeed);
    leftIRState = digitalRead(irLeft);
    rightIRState = digitalRead(irRight);
  } while (leftIRState == HIGH && rightIRState == HIGH);
  stopAllMotors();
  delay(500);
}

void monitorIR() {
  leftIRState = digitalRead(irLeft);
  rightIRState = digitalRead(irRight);
}

void setup() {
  pinMode(leftMotorIn1, OUTPUT);
  pinMode(leftMotorIn2, OUTPUT);
  pinMode(rightMotorIn1, OUTPUT);
  pinMode(rightMotorIn2, OUTPUT);
  pinMode(leftMotorPWM, OUTPUT);
  pinMode(rightMotorPWM, OUTPUT);

  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);

  pinMode(sonarTrigger, OUTPUT);
  pinMode(sonarEcho, INPUT);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(colorSensorOutput, INPUT);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  Serial.begin(9600);
}

void loop() {
  monitorIR();
  checkObstacle();

  if (trackingWhite) {
    if (leftIRState == HIGH && rightIRState == HIGH) {
      moveAhead(normalSpeed);
    } else if (leftIRState == LOW && rightIRState == HIGH) {
      reverseLeftMotor(150);
      runRightMotorForward(normalSpeed + curveSpeed);
    } else if (leftIRState == HIGH && rightIRState == LOW) {
      runLeftMotorForward(normalSpeed + curveSpeed);
      reverseRightMotor(150);
    } else {
      moveBack(normalSpeed);
    }
  } else {
    if (leftIRState == LOW && rightIRState == LOW) {
      moveAhead(normalSpeed);
    } else if (leftIRState == HIGH && rightIRState == LOW) {
      reverseLeftMotor(150);
      runRightMotorForward(normalSpeed + curveSpeed);
    } else if (leftIRState == LOW && rightIRState == HIGH) {
      runLeftMotorForward(normalSpeed + curveSpeed);
      reverseRightMotor(150);
    } else {
      moveBack(normalSpeed);
    }
  }
}
