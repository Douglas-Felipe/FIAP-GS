#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>

bool sendPrecipitationData(const String &jsonPayload, bool &httpError);

#endif
