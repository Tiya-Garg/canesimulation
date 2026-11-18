#include <Arduino.h>

// LED Pin Definitions
const int LED_RED = 23;
const int LED_BLUE = 22;
const int LED_GREEN = 21;
const int LED_YELLOW = 19;
const int BUZZER_PIN = 15;

const int BUZZER_CHANNEL = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("ESP32 Ready for Bi-directional Comms");

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);

  ledcSetup(BUZZER_CHANNEL, 2000, 8);
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  ledcWrite(BUZZER_CHANNEL, 0);
}

void loop()
{
  // Check if Python sent a command over Serial
  if (Serial.available() > 0)
  {
    char command = Serial.read();

    // Reset all outputs first
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_BLUE, LOW);
    ledcWrite(BUZZER_CHANNEL, 0);

    // Process command and send acknowledgment back
    if (command == 'L')
    {
      digitalWrite(LED_RED, HIGH);
      Serial.println("ACK:ESP32_STATUS=LED_RED_ACTIVE"); // Sent back to laptop
    }
    else if (command == 'R')
    {
      digitalWrite(LED_BLUE, HIGH);
      Serial.println("ACK:ESP32_STATUS=LED_BLUE_ACTIVE"); // Sent back to laptop
    }
    else if (command == 'S')
    {
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_BLUE, HIGH);
      ledcWriteTone(BUZZER_CHANNEL, 1000);
      Serial.println("ACK:ESP32_STATUS=STAIRS_ALARM_ACTIVE"); // Sent back to laptop
    }
    else
    {
      Serial.println("ACK:ESP32_STATUS=PATH_CLEAR"); // Sent back to laptop
    }
  }
}