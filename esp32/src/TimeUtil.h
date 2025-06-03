#pragma once

#include <Arduino.h>

void initTime();
String getDateString(int offsetDays = 0);
bool isMidnightUTC();
