// C code for the autonomous car circuit made on Tinkercad.
// the ultrasonic sensors detect obstacles within 150 cm (distance set only for test purposes, real implementation would have different values)

// ++ THIS VERSION ONLY CONTAINS PLACEHOLDER CODE FOR THE IR SENSORS
// BECAUSE ON TINKERCAD THERE IS NO WAY TO CHECK IF IR SENSORS DETECT LINES 


void setup()

{

  // motor pins

  pinMode(2, OUTPUT);

  pinMode(3, OUTPUT);


  // ultrasonic sensor 1

  pinMode(6, OUTPUT); // trig1

  pinMode(7, INPUT);  // echo1


  // ultrasonic sensor 2

  pinMode(8, OUTPUT); // trig2

  pinMode(9, INPUT);  // echo2


  // IR sensors

  pinMode(10, INPUT); // IR sensor 1

  pinMode(11, INPUT); // IR sensor 2


  Serial.begin(9600);

}


void loop()

{

  long dis1, dis2;

  long toCm1, toCm2;


  // read ultrasonic sensor 1

  digitalWrite(6, LOW);

  delayMicroseconds(2);

  digitalWrite(6, HIGH);

  delayMicroseconds(10);

  digitalWrite(6, LOW);

  dis1 = pulseIn(7, HIGH); // timeout 30ms

 

  if (dis1 == 0) {

    toCm1 = 999;  // assume nothing detected

  } else {

    toCm1 = microsecondsToCentimeters(dis1);

  }


  // read ultrasonic sensor 2 
  digitalWrite(8, LOW);

  delayMicroseconds(2);

  digitalWrite(8, HIGH);

  delayMicroseconds(10);

  digitalWrite(8, LOW);

  dis2 = pulseIn(9, HIGH); // timeout 30ms

 

  if (dis2 == 0) {

    toCm2 = 999;

  } else {

    toCm2 = microsecondsToCentimeters(dis2);

  }


  // read IR sensors

  int ir1 = digitalRead(10);

  int ir2 = digitalRead(11);


  // motor logic


  if (toCm1 >= 150 && toCm2 >= 150)  

  {

    // STOP motors

    digitalWrite(2, HIGH);

    digitalWrite(3, HIGH);
    
  }

  else

  {

    if (ir1 == LOW) // IR sensor 1 

    {

      // placeholder for right turn logic

      digitalWrite(2, LOW);

      digitalWrite(3, LOW);

    }

    else if (ir2 == LOW) // placeholder IR sensor 2 

    {

      // left turn logic

      digitalWrite(2, LOW);

      digitalWrite(3, LOW);

    }

    else

    {

      // normal driving

      digitalWrite(2, LOW);

      digitalWrite(3, LOW);

    }

  }


  delay(20); // small delay for stability

}


// function to convert time to distance

long microsecondsToCentimeters(long microseconds)

{

  return microseconds / 29 / 2;

} 
