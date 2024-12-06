// Time to calibrate the sensor
int calibrationTime = 30;

// Time when the sensor outputs a low impulse
long unsigned int lowIn;

// The amount of milliseconds the sensor has to be low before we assume no motion
long unsigned int pause = 5000;  // Pause duration in ms before sensor can detect motion again

boolean lockLow = true;
boolean takeLowTime;

int pirPin = 3;   // Digital pin connected to the PIR sensor's output
int ledPin = 13;  // Digital pin connected to the LED

/////////////////////////////
// SETUP
void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);

  // Give the sensor some time to calibrate
  Serial.print("Calibrating sensor ");
  for (int i = 0; i < calibrationTime; i++) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" Done");
  Serial.println("SENSOR ACTIVE");
  delay(50);
}

/////////////////////////////
// LOOP
void loop() {
  // Check if motion is detected
  if (digitalRead(pirPin) == HIGH) {
    digitalWrite(ledPin, HIGH);  // Turn on the LED
    if (lockLow) {
      lockLow = false;  // Ensure this block runs only once when motion is detected
      Serial.println("---");
      Serial.print("Motion detected at ");
      Serial.print(millis() / 1000);
      Serial.println(" sec");
      delay(50);  // Small delay for stability
    }
    takeLowTime = true;  // Indicate motion was detected
    lowIn = millis();  // Store the time when motion was detected
  }

  // If motion is detected, keep the LED on for 5 seconds
  if (!lockLow && millis() - lowIn <= 5000) {  
    // Keep the LED on for 5 seconds
    // Motion detected, LED stays on
    return;  // Exit the loop early and avoid other checks
  }

  // If no motion is detected, the LED turns off
  if (digitalRead(pirPin) == LOW && !lockLow) {
    digitalWrite(ledPin, LOW);  // Turn off the LED

    if (takeLowTime) {
      lowIn = millis();  // Save the time when motion stopped
      takeLowTime = false;  // Ensure this block runs only once
    }

    // If the sensor has been LOW for longer than the pause duration
    if (millis() - lowIn > pause) {
      lockLow = true;  // Reset lockLow for the next motion detection
      Serial.print("Motion ended at ");
      Serial.print((millis() - pause) / 1000);
      Serial.println(" sec");
      delay(50);  // Small delay for stability
    }
  }
}
