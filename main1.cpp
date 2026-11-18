#include <Arduino.h>

// LED Pin Definitions
const int LED_RED = 23;
const int LED_BLUE = 22;
const int LED_GREEN = 21;
const int LED_YELLOW = 19;
const int BUZZER_PIN = 15;

// Button Pin Definitions
const int BTN_1 = 12;
const int BTN_2 = 14;
const int BTN_3 = 25;
const int BTN_4 = 26;
const int BTN_BUZZER = 27;

// LEDC Channel configuration for Core 2.x
const int BUZZER_CHANNEL = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("ESP32 4-LED & Buzzer Controller Initialized");

  // Configure Output Pins
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);

  // Initialize Buzzer PWM using Core 2.x legacy LEDC API
  ledcSetup(BUZZER_CHANNEL, 2000, 8); // Channel, base frequency, resolution
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);

  // Configure Button Pins as Inputs with internal pull-ups
  pinMode(BTN_1, INPUT_PULLUP);
  pinMode(BTN_2, INPUT_PULLUP);
  pinMode(BTN_3, INPUT_PULLUP);
  pinMode(BTN_4, INPUT_PULLUP);
  pinMode(BTN_BUZZER, INPUT_PULLUP);

  // Initialize everything OFF
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  ledcWrite(BUZZER_CHANNEL, 0); // Fixed from ledcWriteTone
}

void loop()
{
  // Button 1 -> Red LED
  if (digitalRead(BTN_1) == LOW)
  {
    digitalWrite(LED_RED, HIGH);
    Serial.println("Button 1 Pressed -> Red LED ON");
  }
  else
  {
    digitalWrite(LED_RED, LOW);
  }

  // Button 2 -> Blue LED
  if (digitalRead(BTN_2) == LOW)
  {
    digitalWrite(LED_BLUE, HIGH);
    Serial.println("Button 2 Pressed -> Blue LED ON");
  }
  else
  {
    digitalWrite(LED_BLUE, LOW);
  }

  // Button 3 -> Green LED
  if (digitalRead(BTN_3) == LOW)
  {
    digitalWrite(LED_GREEN, HIGH);
    Serial.println("Button 3 Pressed -> Green LED ON");
  }
  else
  {
    digitalWrite(LED_GREEN, LOW);
  }

  // Button 4 -> Yellow LED
  if (digitalRead(BTN_4) == LOW)
  {
    digitalWrite(LED_YELLOW, HIGH);
    Serial.println("Button 4 Pressed -> Yellow LED ON");
  }
  else
  {
    digitalWrite(LED_YELLOW, LOW);
  }

  // Buzzer Button -> Activates Buzzer Tone with frequency
  if (digitalRead(BTN_BUZZER) == LOW)
  {
    ledcWriteTone(BUZZER_CHANNEL, 1000); // Sets 1000 Hz frequency & active tone
    Serial.println("Buzzer Button Pressed -> Sounding Alarm");
  }
  else
  {
    ledcWriteTone(BUZZER_CHANNEL, 0); // Stops the tone
  }

  delay(50); // Debounce delay
}