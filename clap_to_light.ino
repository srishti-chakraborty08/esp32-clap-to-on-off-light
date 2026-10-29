#define SOUND_PIN 27
#define RELAY_PIN 15

bool relayState = LOW;
bool isFirstClapDetected = false;

unsigned long firstClapTime = 0;
unsigned long lastClapTime = 0;

void setup() {
  Serial.begin(115200);

  pinMode(SOUND_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);

  Serial.println("Double Clap Bulb Controller Ready");
}

void loop() {
  unsigned long now = millis();

  // Detect clap using the digital sound sensor
  if (digitalRead(SOUND_PIN) == LOW &&
      now - lastClapTime >= 150) {

    lastClapTime = now;

    if (!isFirstClapDetected) {
      firstClapTime = now;
      isFirstClapDetected = true;
    }
    else {
      // Second clap should be 200-600 ms after first
      if (now - firstClapTime >= 200 &&
          now - firstClapTime < 600) {

        relayState = !relayState;  // Toggle relay ON/OFF
        digitalWrite(RELAY_PIN, relayState);  // Apply new state

        Serial.print("DOUBLE CLAP! Relay now: ");
        Serial.println(relayState ? "ON" : "OFF");
      }

      isFirstClapDetected = false;  // Reset
    }

    delay(150);  // Noise debounce
  }

  // Reset if second clap didn't come in time
  if (isFirstClapDetected &&
      millis() - firstClapTime > 600) {
    isFirstClapDetected = false;
  }
}
