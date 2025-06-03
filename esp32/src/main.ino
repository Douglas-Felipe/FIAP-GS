#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>

#include "Config.h"
#include "Button.h"
#include "Sensor.h"
#include "Storage.h"
#include "Network.h"
#include "LED.h"
#include "TimeUtil.h"

static String lastSentDate = "";

void setup() {
  Serial.begin(115200);
  delay(1000);

  initSensor();         // HC-SR04 usando 3.3V
  initLEDs();           // LEDs usando 3.3V GPIOs
  initButton();         // Botao com pull-up interno

  // Connectando a WiFi
  Serial.print("Starting WiFi connection...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, 6);
  Serial.print(" connecting to WiFi");
  
  unsigned long start = millis();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - start > 10000) {
      Serial.println(" failed to connect");
      break;
    }
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" WiFi connected");
  }

  Serial.print("Initializing storage...");
  initStorage(); // Initializando Preferences NVS
  Serial.println(" storage initialized!");

  Serial.print("Initializing time management...");
  initTime();   // Sincronizando com servidor NTP
  Serial.println(" time management initialized!");

  lastSentDate = getLastSentDate();
}

void loop() {
  // 1. Enviando dados para o server manualmente ao pressionar o botão
  if (isButtonPressed()) {
    Serial.println("Manually trigerring today's collection...");
    performDailyRoutine(true);
  }

  // 2. Enviando dados para o server autoomaticamente a meia noite UTC
  static unsigned long lastCheck = 0;
  unsigned long currentMillis = millis();
  
  // Checando a cada 1 minuto
  if (currentMillis - lastCheck >= 60000) {
    lastCheck = currentMillis;
    
    if (isMidnightUTC()) {
      String today = getDateString(0);
      
      if (today.length() > 0 && today != lastSentDate) {
        Serial.println("Midnight UTC reached; triggering daily sync...");
        performDailyRoutine(false);
      }
    }
  }

  // Delay mais curto para atualizar os LEDs de forma mais responsiva
  delay(50);
}

void performDailyRoutine(bool manualTrigger) {
  // 1. Medindo e guardando informacao de precipitacao
  String today = getDateString(0);
  float precip_mm = measurePrecipitationMm();
  
  Serial.print("Measured precipitation for ");
  Serial.print(today);
  Serial.print(": ");
  Serial.print(precip_mm);
  Serial.println(" mm");
  
  storePrecipitation(today, precip_mm);

  // 2. Gerando JSON array com os dados de precipitacao dos ultimos 7 dias.
  JsonDocument doc;
  JsonArray arr = doc.to<JsonArray>();
  
  for (int i = 0; i < 7; i++) {
    String dateKey = getDateString(i);
    float value = getPrecipitation(dateKey);
    arr.add(value);
  }
  
  String payload;
  serializeJson(arr, payload);
  Serial.print("Payload: ");
  Serial.println(payload);

  // 3. Enviando dados para o servidor
  setLEDSending();
  int result = sendPrecipitationData(payload);

  switch (result) {
  case SERVER_IDLE:
    Serial.println("Server is in IDLE");
    turnOffLEDs();

    break;
  case SERVER_RESPONSE_FLOOD:
    Serial.println("Flood detected");
    setLEDFailure();

    lastSentDate = today;
    storeLastSentDate(lastSentDate);

    break;
  case SERVER_RESPONSE_NO_FLOOD:
    Serial.println("No flood detected");
    setLEDSuccess();

    lastSentDate = today;
    storeLastSentDate(lastSentDate);

    break;
  case SERVER_RESPONSE_ERROR:
    Serial.println("Server error");
    setLEDError();

    break;
  }
}
