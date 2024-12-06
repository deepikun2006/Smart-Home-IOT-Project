#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set up the LCD with the I2C address (0x27) and 16x2 size
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define the pins for the ultrasonic sensor
const int trigPin = 9;  // Trigger pin for the ultrasonic sensor
const int echoPin = 10; // Echo pin for the ultrasonic sensor

// Set a threshold distance (in centimeters) for detecting if the car is close
const int carThreshold = 30;  // If distance is less than or equal to 30 cm, we consider the garage "open"

// Setup function: runs once when the program starts
void setup() {
  // Initialize the LCD display
  lcd.begin();
  lcd.backlight();  // Turn on the LCD backlight

  // Initialize Serial Monitor for debugging (optional)
  Serial.begin(9600);

  // Set the ultrasonic sensor pins as input or output
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Display a welcome message on the LCD screen
  lcd.setCursor(0, 0);  // Set the cursor to the first row, first column
  lcd.print("Garage Status:");  // Display the title on the first row
  delay(2000);  // Wait for 2 seconds to show the title
}

// Loop function: runs continuously after setup
void loop() {
  long duration, distance;

  // Trigger the ultrasonic pulse
  digitalWrite(trigPin, LOW);  // Make sure the trigger pin is LOW initially
  delayMicroseconds(2);  // Wait for 2 microseconds
  digitalWrite(trigPin, HIGH);  // Send the ultrasonic pulse
  delayMicroseconds(10);  // Wait for 10 microseconds
  digitalWrite(trigPin, LOW);  // Turn off the trigger pin

  // Measure the time it takes for the pulse to return
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance in centimeters (speed of sound is 343 m/s or 0.034 cm/µs)
  distance = duration * 0.034 / 2;  // Divide by 2 because the pulse travels to the object and back

  // Clear the previous status on the LCD and print the new status
  lcd.clear();
  lcd.setCursor(0, 0);  // Move to the first row

  // Check if the car is within the threshold distance (i.e., the garage is "open")
  if (distance > 0 && distance <= carThreshold) {
    lcd.print("Garage Open   ");  // Display "Garage Open" on the LCD
    Serial.println("Garage Open");  // Output status to the Serial Monitor (optional)
  } else {
    lcd.print("Garage Closed ");  // Display "Garage Closed" on the LCD
    Serial.println("Garage Closed");  // Output status to the Serial Monitor (optional)
  }

  // Wait for 500 milliseconds before the next reading
  delay(500);
}
