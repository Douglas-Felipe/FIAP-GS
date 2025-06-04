#include "Network.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Config.h"

int sendPrecipitationData(const String &jsonPayload) {
  WiFiClient client;

  HTTPClient http;
  http.addHeader("Content-Type", "application/json");
  String endpoint  = SERVER_URL;
  endpoint += "/predict";
  http.begin(client, endpoint);
  
  int result = SERVER_IDLE;
  
  Serial.println("Sending POST request...");
  int httpCode = http.POST(jsonPayload);

  Serial.print("HTTP status: ");
  Serial.println(httpCode);

  if (httpCode <= 0) {
    Serial.print("HTTP error: ");
    Serial.println(http.errorToString(httpCode).c_str());
    
    http.end();
    return SERVER_RESPONSE_ERROR;
  }

  String res = http.getString();
  Serial.print("Response: ");
  Serial.println(res);

  if (res.length() <= 0) {
    Serial.println("Empty response received");
    
    http.end();
    return SERVER_RESPONSE_ERROR;
  }

  JsonDocument doc;
  auto err = deserializeJson(doc, res);

  if (err) {
    Serial.println("JSON parsing error");

    http.end();
    return SERVER_RESPONSE_ERROR;
  }

  bool predictResultIsBoolean = doc["enchente"].is<bool>();

  if (!predictResultIsBoolean) {
    Serial.println("Unexpected JSON structure");

    http.end();
    return SERVER_RESPONSE_ERROR;
  }

  bool predictionResult = doc["enchente"].as<bool>();
  
  if (predictionResult) {
    result = SERVER_RESPONSE_FLOOD;
  } else {
    result = SERVER_RESPONSE_NO_FLOOD;
  }

  http.end();
  return result;
}
