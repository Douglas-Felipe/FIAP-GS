#include "TimeUtil.h"
#include <time.h>

void initTime() {
  const char* ntpServer = "pool.ntp.org";
  const long gmtOffset_sec = 0;     // UTC
  const int daylightOffset_sec = 0; // No DST
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    delay(500);
  }
}

String getDateString(int offsetDays) {
  struct tm timeinfo;
  
  if (!getLocalTime(&timeinfo)) {
    return String("");
  }
  
  time_t now;
  time(&now);
  now -= offsetDays * 86400; // Subtraindo offsetDays×86400 s

  struct tm * timeinfoOffset = gmtime(&now);
  char buf[11];
  strftime(buf, sizeof(buf), "%Y-%m-%d", timeinfoOffset);
  
  return String(buf);
}

bool isMidnightUTC() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return false;
  }
  return (timeinfo.tm_hour == 0 && timeinfo.tm_min == 0 && timeinfo.tm_sec == 0);
}
