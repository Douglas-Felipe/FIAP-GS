#include <Arduino.h>

#include "LED.h"
#include "Config.h"

void initLEDs() {
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_ORANGE_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);

  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_ORANGE_PIN, LOW);
  digitalWrite(LED_YELLOW_PIN, LOW);
}

void setLEDSending() {
  digitalWrite(LED_YELLOW_PIN, HIGH);
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_ORANGE_PIN, LOW);
}

void setLEDSuccess() {
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, HIGH);
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_ORANGE_PIN, LOW);
}

void setLEDFailure() {
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_RED_PIN, HIGH);
  digitalWrite(LED_ORANGE_PIN, LOW);
}

void setLEDError() {
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_ORANGE_PIN, HIGH);
}

void turnLEDOn(const int led) {
  digitalWrite(led, HIGH);
}