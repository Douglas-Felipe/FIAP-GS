#include "Storage.h"
#include <Preferences.h>

static Preferences preferences;

void initStorage() {
  preferences.begin("rain_data", false);
}

// Funcoes utilizadas para guardar as informacoes de chuva
// de forma que os dados sobrevival ao reniciar o ESP

void storePrecipitation(const String &date, float value) {
  preferences.putFloat(date.c_str(), value);
}

float getPrecipitation(const String &date) {
  return preferences.getFloat(date.c_str(), 0.0f);
}

void storeLastSentDate(const String &date) {
  preferences.putString("last_sent_date", date);
}

String getLastSentDate() {
  return preferences.getString("last_sent_date", String(""));
}
