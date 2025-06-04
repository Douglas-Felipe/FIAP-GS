#include "Config.h"

// Credenciais para WiFi.
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

// Server URL (MVP: HTTPS)
// Se o server estiver rodando localmente
// Verifique o ipv4 do seu computador(ipconfig) e coloque aqui
const char* SERVER_URL = "http://192.168.0.151:8000";

// HC-SR04 Pins (powered at 3.3V)
const int TRIG_PIN = 5;   // Trigger pin → GPIO5
const int ECHO_PIN = 18;  // Echo pin    → GPIO18

// Button Pin (usando pull-up interno)
const int BUTTON_PIN = 16; // GPIO16, wired to GND

// LED Pins (all 3.3V-rated GPIOs)
const int LED_GREEN_PIN  = 19; // Green LED -> GPIO19
const int LED_RED_PIN    = 21; // Red LED   -> GPIO21
const int LED_ORANGE_PIN = 22; // Orange LED-> GPIO22
const int LED_YELLOW_PIN = 23; // Yellow LED-> GPIO23

// Container inner height (cm).
// Altura para simular um balde de coleta de agua da chuva.
const float CONTAINER_HEIGHT_CM = 20.0f; 