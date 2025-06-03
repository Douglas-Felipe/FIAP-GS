## TL; DR

Utilizaremos um sensor de distância ultrassônico HC-SR04 para aproximar a profundidade da água em um recipiente simples e convertê-la em precipitação em milímetros.

O ESP32 sincronizará seu relógio em UTC via [NTP](https://en.wikipedia.org/wiki/Network_Time_Protocol),
medirá a profundidade da água do recipiente uma vez por dia (ou ao pressionar um botão),
calculará a precipitação diária usando a diferença entre a altura do recipiente e a distância medida e armazenará o valor de cada dia no armazenamento não volátil
do ESP32 ([Preferences](https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/preferences.html)) sob uma chave no formato "YYYY-MM-DD".

A cada dia (ou ao pressionar um botão), o ESP32 cria um array JSON com os dados dos últimos 7 dias e o envia via HTTP POST para a API.
O campo booleano `success` da resposta aciona LEDs coloridos (verde/vermelho/laranja). Durante a transmissão, um LED amarelo acende.

Todo o código é dividido, para maximizar a legibilidade, a manutenibilidade e a testabilidade, em módulos:
* Sensor;
* Storage;
* Network;
* LED;
* Button; e
* TimeUtil 

O diagrama de simulação do Wokwi usa:
* board-esp32-devkit-c-v4;
* wokwi-hc-sr04 como o sensor de distância;
* um botão de pressão; e
* quatro LEDs de 5 mm (verde, vermelho, laranja, amarelo), cada um com um resistor de 220Ω.

![Screenshot 2025-06-03 142057](https://github.com/user-attachments/assets/072fc881-f43e-4819-9976-de19ed447afd)
