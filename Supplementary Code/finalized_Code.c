#include <Servo.h>

// === Pin Definitions ===
#define ENA 9   // Right motor speed
#define IN1 8   // Right motor direction
#define IN2 7   // Right motor direction

#define ENB 4   // Left motor speed
#define IN3 6   // Left motor direction
#define IN4 5   // Left motor direction

#define IR_LEFT 3
#define IR_RIGHT 2

#define TRIG_PIN 12
#define ECHO_PIN 13

const int SERVO_PIN = 10;
const int SCAN_CENTER = 0;     // Straight ahead
const int SCAN_LEFT = 90;      // Rotate servo left
const int SCAN_RIGHT = 180;    // Rotate servo right

// === Adjustable Speeds & Thresholds ===
int motorSpeed = 65;
float backwardsScale = 1;
int angleSnapSpeed = 75;
int obstacleThreshold = 15;  // cm

Servo scanServo;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  scanServo.attach(SERVO_PIN);
  scanServo.write(SCAN_CENTER);

  stopMotors();
}

void loop() {
  if (measureDistance() < obstacleThreshold) {
    avoidObstacle();
    return;
  }

  int leftIR = digitalRead(IR_LEFT);
  int rightIR = digitalRead(IR_RIGHT);

  if (leftIR == LOW && rightIR == LOW) {
    moveForward();
  } else if (leftIR == HIGH && rightIR == HIGH) {
    stopMotors();
  } else {
    if (leftIR == LOW && rightIR == HIGH) {
      runRightMotorForward(motorSpeed);
      runLeftMotorBackward(int(motorSpeed * backwardsScale));
    } else if (leftIR == HIGH && rightIR == LOW) {
      runLeftMotorForward(motorSpeed);
      runRightMotorBackward(int(motorSpeed * backwardsScale));
    }
  }
}

// === Obstacle Avoidance Routine ===
void avoidObstacle() {
  stopMotors();
  delay(500);

  // Step 1: turn left
  runLeftMotorBackward(angleSnapSpeed);
  runRightMotorForward(angleSnapSpeed);
  delay(400);

  // Step 2: move forward out of the way
  moveForward();
  delay(600);

  // Step 3: turn right
  runLeftMotorForward(angleSnapSpeed);
  runRightMotorBackward(angleSnapSpeed);
  delay(400);

  // Step 4: move forward across the obstacle
  moveForward();
  delay(800);

  // Step 5: turn right again
  runLeftMotorForward(angleSnapSpeed);
  runRightMotorBackward(angleSnapSpeed);
  delay(400);

  // Step 6: move forward to return to path
  moveForward();
  delay(600);

  // Step 7: final left turn to face original direction
  runLeftMotorBackward(angleSnapSpeed);
  runRightMotorForward(angleSnapSpeed);
  delay(400);

  stopMotors();
  delay(500);

  reorientAfterAvoidance();
}

// === Post-Avoidance Reorientation ===
void reorientAfterAvoidance() {
  stopMotors();
  delay(500);

  int frontDistance, leftDistance, rightDistance;

  // Look right
  scanServo.write(SCAN_RIGHT);
  delay(500);
  rightDistance = measureDistance();

  // Look center
  scanServo.write(SCAN_CENTER);
  delay(500);
  frontDistance = measureDistance();

  // Look left
  scanServo.write(SCAN_LEFT);
  delay(500);
  leftDistance = measureDistance();

  // Return to center
  scanServo.write(SCAN_CENTER);
  delay(300);

  // Decide direction based on where obstacle is
  if (frontDistance < rightDistance && frontDistance < leftDistance) {
    turnRight90(); delay(300); turnRight90();
  } else if (rightDistance < frontDistance && rightDistance < leftDistance) {
    turnLeft90();
  } else if (leftDistance < frontDistance && leftDistance < rightDistance) {
    turnRight90();
  }

  stopMotors();
  delay(500);
}

// === Turning Helpers ===
void turnLeft90() {
  runLeftMotorBackward(angleSnapSpeed);
  runRightMotorForward(angleSnapSpeed);
  delay(400);
  stopMotors();
  delay(300);
}

void turnRight90() {
  runLeftMotorForward(angleSnapSpeed);
  runRightMotorBackward(angleSnapSpeed);
  delay(400);
  stopMotors();
  delay(300);
}

// === Motor Control ===
void moveForward() {
  runRightMotorForward(motorSpeed);
  runLeftMotorForward(motorSpeed);
}

void stopMotors() {
  stopLeftMotor();
  stopRightMotor();
}

void runRightMotorForward(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);
}

void runRightMotorBackward(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);
}

void stopRightMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
}

void runLeftMotorForward(int speed) {
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);
}

void runLeftMotorBackward(int speed) {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed);
}

void stopLeftMotor() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}

// === Ultrasonic Distance ===
long measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;  // Distance in cm
}
