#include <Wire.h>

#define SLAVE_ADDRESS 0x08  // Set Feather's I2C slave address here

// Define the pins for relay set and reset
#define RELAY_SET_PIN 18   // Pin to turn the relay on (Set)
#define RELAY_RESET_PIN 5 // Pin to turn the relay off (Reset)

// Define threshold for light sensor to control LED
#define LIGHT_THRESHOLD 500 

// Define pin for ALS-PT19 light sensor (analog input)
#define LIGHTSENSOR_PIN A0


// Pin definitions for LEDs
#define APPLE_FRESH_LED 6
#define APPLE_OVERRIPE_LED 9
#define BANANA_FRESH_LED 10
#define BANANA_OVERRIPE_LED 11
#define EMPTY_LED 12
#define STATUS_LED 13 // LED to indicate successful communication
//#define LIGHT_LED 7 // General light controlled by ALS


int fruitState = 4; // Default state (empty)
int lastFruitState = -1; // Variable to store the last fruit state
unsigned long lastCommunicationTime = 0; // Tracks the last time data was received
const unsigned long COMMUNICATION_TIMEOUT = 3000; // 3 seconds timeout

// Function prototypes
void receiveEvent(int howMany);
void controlFruitStateLEDs(int state);
void controlRelay(int state);


// ALS PT19 sensor function
int readLightSensor() {
  int lightValue = analogRead(LIGHTSENSOR_PIN); // Read ALS PT19 sensor value
  return lightValue;
}



void setup() {


  // Initialize both relay control pins as outputs
  pinMode(RELAY_SET_PIN, OUTPUT);
  pinMode(RELAY_RESET_PIN, OUTPUT);

  // Initialize LED pins
  pinMode(APPLE_FRESH_LED, OUTPUT);
  pinMode(APPLE_OVERRIPE_LED, OUTPUT);
  pinMode(BANANA_FRESH_LED, OUTPUT);
  pinMode(BANANA_OVERRIPE_LED, OUTPUT);
  pinMode(EMPTY_LED, OUTPUT);
  //pinMode(LIGHT_LED, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);

  // Initialize ALS-PT19 sensor pin as input
  pinMode(LIGHTSENSOR_PIN, INPUT);

  // Set both pins to LOW initially (relay off)
  digitalWrite(RELAY_SET_PIN, LOW);
  digitalWrite(RELAY_RESET_PIN, LOW);


  // Start I2C with Feather M0 as Slave
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveEvent); // Set the receive event function
  Serial.begin(9600); // Start Serial for debugging
  Serial.println("Feather M0 I2C Slave initialized");

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

  // Only update LEDs if the fruit state has changed
  if (fruitState != lastFruitState) {
    controlFruitStateLEDs(fruitState);
    lastFruitState = fruitState; // Update the last state
  }

  // Check if the last communication was received within the timeout period
  if (millis() - lastCommunicationTime > COMMUNICATION_TIMEOUT) {
    // If no data was received for the timeout period, turn off the status LED
    digitalWrite(STATUS_LED, LOW);
  }

  // Small delay to avoid bouncing or frequent toggling
  delay(500);

}



// Function called when data is received over I2C
void receiveEvent(int howMany) {
  if (Wire.available()) {
    fruitState = Wire.read(); // Receive the fruit state from Nicla Vision
    Serial.print("Received Fruit State: ");
    Serial.println(fruitState);

    // Update last communication time and turn on the status LED
    lastCommunicationTime = millis();
    digitalWrite(STATUS_LED, HIGH); // Communication successful, turn on the status LED
  }
}



// Function to control fruit state LEDs
void controlFruitStateLEDs(int state) {
  // Turn off all fruit LEDs first
  digitalWrite(APPLE_FRESH_LED, LOW);
  digitalWrite(APPLE_OVERRIPE_LED, LOW);
  digitalWrite(BANANA_FRESH_LED, LOW);
  digitalWrite(BANANA_OVERRIPE_LED, LOW);
  digitalWrite(EMPTY_LED, LOW);

  // Turn on the corresponding LED based on the fruit state
  switch (state) {
    case 0:
      Serial.println("Apple Fresh LED ON");
      digitalWrite(APPLE_FRESH_LED, HIGH);
      break;
    case 1:
      Serial.println("Apple Overripe LED ON");
      digitalWrite(APPLE_OVERRIPE_LED, HIGH);
      break;
    case 2:
      Serial.println("Banana Fresh LED ON");
      digitalWrite(BANANA_FRESH_LED, HIGH);
      break;
    case 3:
      Serial.println("Banana Overripe LED ON");
      digitalWrite(BANANA_OVERRIPE_LED, HIGH);
      break;
    case 4:
      Serial.println("Empty State LED ON");
      digitalWrite(EMPTY_LED, HIGH);
      break;
  }
}
