#include "Network.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Config.h"

int sendPrecipitationData(const String &jsonPayload) {
  WiFiClientSecure client;
  client.setInsecure();  // Obrigatorio em modo de desenvolvimento

  HTTPClient http;
  http.addHeader("Content-Type", "application/json");
  http.begin(client, SERVER_URL);
  
  int result = SERVER_IDLE;
  
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
        bool predictResultIsBoolean = doc["enchente"].is<bool>();

        if (predictResultIsBoolean) {
          bool predictionResult = doc["enchente"].as<bool>();

          if (predictionResult) {
            result = SERVER_RESPONSE_FLOOD;
          } else {
            result = SERVER_RESPONSE_NO_FLOOD;
          }
        } else {
          result = SERVER_RESPONSE_ERROR;
        }
      } else {
        Serial.print("JSON parsing error: ");
        Serial.println(err.c_str());
        
        result = SERVER_RESPONSE_ERROR;
      }
    } else {
      Serial.println("Empty response received");
      result = SERVER_RESPONSE_ERROR;
    }
  } else {
    Serial.print("HTTP error: ");
    Serial.println(http.errorToString(httpCode).c_str());
    
    result = SERVER_RESPONSE_ERROR;
  }
  
  http.end();
  return result;
}
