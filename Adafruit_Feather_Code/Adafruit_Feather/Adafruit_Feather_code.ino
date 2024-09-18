// Define the pins for relay set and reset
#define RELAY_SET_PIN 18   // Pin to turn the relay on (Set)
#define RELAY_RESET_PIN 5 // Pin to turn the relay off (Reset)

// Define pin for ALS-PT19 light sensor (analog input)
#define LIGHTSENSOR_PIN A0

// Define threshold for light sensor to control LED
#define LIGHT_THRESHOLD 25 

void setup() {


  // Initialize both relay control pins as outputs
  pinMode(RELAY_SET_PIN, OUTPUT);
  pinMode(RELAY_RESET_PIN, OUTPUT);

  // Initialize ALS-PT19 sensor pin as input
  pinMode(LIGHTSENSOR_PIN, INPUT);

  // Set both pins to LOW initially (relay off)
  digitalWrite(RELAY_SET_PIN, LOW);
  digitalWrite(RELAY_RESET_PIN, LOW);

  // Initialize Serial Monitor for debugging
  Serial.begin(9600);

}

void loop() {

  
  // Read the value from the ALS-PT19 light sensor
  int lightLevel = analogRead(LIGHTSENSOR_PIN);

  // Display the light sensor reading on the Serial Monitor
  Serial.print("Light Sensor Reading: ");
  Serial.println(lightLevel);

  // Compare the light level to the threshold
  if (lightLevel < LIGHT_THRESHOLD) {

    // If the light level is below the threshold (dark), turn the relay on (LED on)
    digitalWrite(RELAY_SET_PIN, HIGH); // Set the relay to ON
    delay(100);  // Short pulse to activate relay
    digitalWrite(RELAY_SET_PIN, LOW); // Turn off the set pin

  } else {

    // If the light level is above the threshold (bright), turn the relay off (LED off)
    digitalWrite(RELAY_RESET_PIN, HIGH); // Reset the relay to OFF
    delay(100);  // Short pulse to reset the relay
    digitalWrite(RELAY_RESET_PIN, LOW); // Turn off the reset pin
  
  }

  // Small delay to avoid bouncing or frequent toggling
  delay(500);

}
