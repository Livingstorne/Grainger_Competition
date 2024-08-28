#include <Adafruit_NeoPixel.h>

// Define the control pins
#define RELAY_SET_PIN 18 // Replace with the actual pin connected to SET
#define RELAY_RESET_PIN 5 // Replace with the actual pin connected to RESET
#define LIGHT_SENSOR_PIN A0 // Analog pin for the light sensor
#define APPLE_FRESH 12  // GPIO pin for green LED
#define APPLE_OVERRIPE 13    // GPIO pin for red LED
#define BANANA_FRESH 14
#define BANANA_OVERRIPE 15
#define FRIDGE_ON 16
#define FRIDGE_EMPTY_OR_UNKOWN 17

// Define the light threshold for turning on/off the LED
#define LIGHT_THRESHOLD 500 // Adjust based on your environment

void setup() {
  pinMode(RELAY_SET_PIN, OUTPUT);
  pinMode(RELAY_RESET_PIN, OUTPUT);
  pinMode(LIGHT_SENSOR_PIN, INPUT);

  // Initially reset the relay to ensure it's off
  digitalWrite(RELAY_RESET_PIN, HIGH);
  delay(100);
  digitalWrite(RELAY_RESET_PIN, LOW);
}

void loop() {

  int lightLevel = analogRead(LIGHT_SENSOR_PIN);
  Serial.print("Light Level: "); // Print the sensor reading to the serial monitor
  Serial.println(lightLevel); 

  if (lightLevel > LIGHT_THRESHOLD) {

    // Turn on the relay (and LED)
    digitalWrite(RELAY_SET_PIN, HIGH);
    delay(100);
    digitalWrite(RELAY_SET_PIN, LOW);
    

  } else {

    // If light level is below the threshold, turn off the relay (and LED)
    digitalWrite(RELAY_RESET_PIN, HIGH);
    delay(100);
    digitalWrite(RELAY_RESET_PIN, LOW);
    
  }

  delay(500); // Small delay to avoid rapid switching

}
