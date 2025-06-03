#include "Network.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Config.h"

bool sendPrecipitationData(const String &jsonPayload, bool &httpError) {
  WiFiClientSecure client;
  client.setInsecure();  // Obrigatorio em modo de desenvolvimento

  HTTPClient http;
  http.addHeader("Content-Type", "application/json");
  http.begin(client, SERVER_URL);
  
  bool success = false;
  httpError = false;
  
  Serial.println("Sending POST request...");
  int httpCode = http.POST(jsonPayload);

  Serial.print("HTTP status: ");
  Serial.println(httpCode);

  if (httpCode > 0) {
    String res = http.getString();
    Serial.print("Response: ");
    Serial.println(res);
    
    if (res.length() > 0) {
      JsonDocument doc;
      auto err = deserializeJson(doc, res);
      
      if (!err) {
        success = doc["success"] | false;
      } else {
        Serial.print("JSON parsing error: ");
        Serial.println(err.c_str());
        success = false;
      }
    } else {
      Serial.println("Empty response received");
      success = false;
    }
  } else {
    Serial.print("HTTP error: ");
    Serial.println(http.errorToString(httpCode).c_str());
    httpError = true;
    success = false;
  }
  
  http.end();
  return success;
}
