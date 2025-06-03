#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>

void initStorage();
void storePrecipitation(const String &date, float value);
float getPrecipitation(const String &date);
void storeLastSentDate(const String &date);
String getLastSentDate();

#endif
