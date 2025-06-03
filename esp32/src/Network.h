#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>

constexpr int SERVER_RESPONSE_NO_FLOOD = 1;
constexpr int SERVER_RESPONSE_FLOOD = 2;
constexpr int SERVER_RESPONSE_ERROR = 3;
constexpr int SERVER_IDLE = 4;

int sendPrecipitationData(const String &jsonPayload);

#endif
