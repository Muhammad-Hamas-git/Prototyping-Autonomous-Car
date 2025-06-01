#define S0 A2
#define S1 A1
#define S2 0
#define S3 1
#define sensorOut A0

int redMax = 24390;
int redMin = 27776;

int blueMax = 17543;
int blueMin = 6756;

int redColor = 0;
int blueColor = 0;

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  // Set frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  Serial.begin(9600);
}

void loop() {
  evaluatingColour();
  delay(2000); // delay between reads
}

void evaluatingColour() {
  Serial.println("------------------------------");

  // RED
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  float redEdgeTime = pulseIn(sensorOut, LOW);
  float redFrequency = (1 / (redEdgeTime / 1000000.0));
  redColor = map(redFrequency, redMax, redMin, 255, 0);
  redColor = constrain(redColor, 0, 255);
  Serial.print("Red Frequency: ");
  Serial.println(redFrequency);
  Serial.print("R = ");
  Serial.println(redColor);

  delay(100);

  // BLUE
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  float blueEdgeTime = pulseIn(sensorOut, LOW);
  float blueFrequency = (1 / (blueEdgeTime / 1000000.0));
  blueColor = map(blueFrequency, blueMax, blueMin, 255, 0);
  blueColor = constrain(blueColor, 0, 255);
  Serial.print("Blue Frequency: ");
  Serial.println(blueFrequency);
  Serial.print("B = ");
  Serial.println(blueColor);

  delay(100);

  // Simple Decision
  if (redColor == 255 && blueColor == 0) {
    Serial.println("No Object Detected");
  } else if (redColor < 255 && blueColor == 0) {
    Serial.println("Red Detected");
  } else if (redColor <= 255 && blueColor > 0) {
    Serial.println("Blue Detected");
  }

  Serial.println("------------------------------");
}
