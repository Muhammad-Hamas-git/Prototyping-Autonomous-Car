// Motor A control pins
const int motorA1 = 5; // IN1
const int motorA2 = 4; // IN2
const int ENA = 6;     // Enable A (PWM)

// Motor B control pins
const int motorB1 = 3; // IN3
const int motorB2 = 2; // IN4
const int ENB = 7;     // Enable B (PWM)

// Motor speed (0 - 255)
const int motorSpeed = 255;

void setup() {
  // Start serial monitor
  Serial.begin(9600);
  delay(1000); // Wait for Serial to initialize

  // Set pins as output
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(ENA, OUTPUT);
  
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Run both motors forward
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);

  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);

  // Serial messages
  Serial.println("Motors started.");
  Serial.print("Speed set to: ");
  Serial.println(motorSpeed);
}

void loop() {
  // Optional: keep printing status every 2 seconds
  Serial.println("Motors running...");
  delay(2000);
}
