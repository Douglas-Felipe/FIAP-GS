#include "Sensor.h"
#include "Config.h"
#include <Arduino.h>

#include "Sensor.h"
#include <Arduino.h>

void initSensor() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
}

float measureDistanceCm() {
  // Mandando um pulso HIGH de 10 μs HIGH para o TRIG
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Lendo o resultado do echo, duracao (μs), timeout 30 ms (~5 m)
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  
  if (duration == 0) {
    return CONTAINER_HEIGHT_CM;
  }
  
  // Convertendo de μs para cm: duracao/58
  float distance = (float) duration / 58.0f;
  
  return distance;
}

float measurePrecipitationMm() {
  float distance = measureDistanceCm();
  
  Serial.print("Distance in centimeters: ");
  Serial.println(distance);
  
  float waterHeight = CONTAINER_HEIGHT_CM - distance;
  Serial.print("Water height: ");
  Serial.println(waterHeight);

  if (waterHeight < 0.0f) {
    waterHeight = 0.0f;
  }
 
  // Convertendo de cm para mm (1 cm = 10 mm)
  return waterHeight * 10.0f;
}
