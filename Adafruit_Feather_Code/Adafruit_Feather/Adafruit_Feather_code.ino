// Define the pins for relay set and reset
#define RELAY_SET_PIN 18   // Pin to turn the relay on (Set)
#define RELAY_RESET_PIN 5 // Pin to turn the relay off (Reset)

void setup() {
  // Initialize both relay control pins as outputs
  pinMode(RELAY_SET_PIN, OUTPUT);
  pinMode(RELAY_RESET_PIN, OUTPUT);

  // Set both pins to LOW initially (relay off)
  digitalWrite(RELAY_SET_PIN, LOW);
  delay(100);
  digitalWrite(RELAY_RESET_PIN, HIGH);

}

void loop() {
  // Turn the LED on by setting the relay (using the SET pin)
  digitalWrite(RELAY_SET_PIN, HIGH);
  delay(100);  // Short pulse to activate relay
  digitalWrite(RELAY_SET_PIN, LOW); // Turn off the set pin after setting the relay
  delay(1000); // LED remains on for 1 second

  // Turn the LED off by resetting the relay (using the RESET pin)
  digitalWrite(RELAY_RESET_PIN, HIGH);
  delay(100);  // Short pulse to reset the relay
  digitalWrite(RELAY_RESET_PIN, LOW); // Turn off the reset pin after resetting the relay
  delay(1000); // LED remains off for 1 second

}