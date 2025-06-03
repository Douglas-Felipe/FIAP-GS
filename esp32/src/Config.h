#ifndef CONFIG_H
#define CONFIG_H

// WiFi Credentials
extern const char* WIFI_SSID;
extern const char* WIFI_PASSWORD;

// Server URL (MVP: HTTP)
extern const char* SERVER_URL;

// HC-SR04 Pins (powered at 3.3V)
extern const int TRIG_PIN;   // Trigger pin → GPIO5
extern const int ECHO_PIN;  // Echo pin    → GPIO18

// Button Pin (using internal pull-up)
extern const int BUTTON_PIN; // GPIO16, wired to GND

// LED Pins (all 3.3V-rated GPIOs)
extern const int LED_GREEN_PIN;  // Green LED -> GPIO19
extern const int LED_RED_PIN;    // Red LED   -> GPIO21
extern const int LED_ORANGE_PIN; // Orange LED-> GPIO22
extern const int LED_YELLOW_PIN; // Yellow LED-> GPIO23

// Container inner height (cm). Adjust if gauge height differs.
extern const float CONTAINER_HEIGHT_CM;

#endif // CONFIG_H
