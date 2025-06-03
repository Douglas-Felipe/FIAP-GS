#ifndef LED_H
#define LED_H

void initLEDs();
void setLEDSending();
void setLEDSuccess();
void setLEDFailure();
void setLEDError();

void turnLEDOn(const int led);

#endif
