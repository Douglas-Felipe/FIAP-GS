#include <Arduino.h>

#include "Button.h"
#include "Config.h"

void initButton() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

bool isButtonPressed() {
  static unsigned long lastDebounceTime = 0;
  static bool lastState = HIGH;
  
  bool reading = digitalRead(BUTTON_PIN);

  if (reading == LOW && lastState == HIGH) {
    lastState = reading;
    return true;
  }
  
  lastState = reading;
  return false;
}
