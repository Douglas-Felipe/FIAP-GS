## TL; DR

Utilizamos um sensor de distância ultrassônico HC-SR04 para, de forma aproximada, medir a profundidade da água em um recipiente simples e convertê-la em precipitação em milímetros.

Implementamos a sincronização do relógio do ESP32 em UTC via [NTP](https://en.wikipedia.org/wiki/Network_Time_Protocol)
para medir a água do recipiente uma vez por dia (ou ao pressionar um botão).

Calculamos a precipitação diária usando a diferença entre a altura do recipiente e a distância medida e armazenamos o valor de cada dia no armazenamento não volátil do ESP32 ([Preferences](https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/preferences.html)) sob uma chave no formato `YYYY-MM-DD`.

A cada dia (ou ao pressionar o botão), o ESP32 cria um array JSON com os dados dos últimos 7 dias e o envia via HTTP POST para a API.
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

## 1. Overview

### Escolhendo sensor de medição de precipitação

- Sensor ultrassônico HC-SR04: Fornece uma leitura de distância direta (em cm) por meio de temporização de pulso.
- Fácil de simular no Wokwi e não requer hardware especializado em precipitação.

### 1.1 Alimentação do HC-SR04 em 3.3V no Wokwi

- O ESP32 oferece um pino **3V3**; como os pinos de GPIO do mesmo também funcionam a 3.3V, escolhemos conectar o HC-SR04 também com 3.3.V.  
- Alimentar o HC-SR04 em 3,3 V reduz sua faixa máxima confiável para cerca de **200cm**, porém isso faz com que o Echo também opere a 3.3V, sendo seguro para os GPIOs do ESP32.  
- No simulador, não há preocupação de níveis analógicos; O wokwi trata sinal digital HIGH/LOW uniformemente, de modo que conectar `3V3 → VCC` é suficiente.  

### 1.2 Trade-Offs de operar em 3.3V

- Um HC-SR04 padrão normalmente opera de **2 cm a 400 cm** quando alimentado em 5V.  
- A 3.3V, a distância máxima confiável costuma cair para aproximadamente **200 cm**, mas essa faixa é suficiente para a maioria dos recipientes de pluviômetro caseiros.  

### 1.3 Segurança de níveis

- Em um hardware real, a saída do HC-SR04 em 5V (no pino Echo) excede a tolerância de 3.3 V do ESP32 e pode danificá-lo; usar 3.3V elimina a necessidade de um conversor de nível.  

### 1.4 Botão com pull-up interno

- O botão está configurado com **pino GPIO16** e **GND**. Em `pinMode(16, INPUT_PULLUP)`, o pino lê **HIGH** quando não pressionado e **LOW** quando pressionado (porque conecta ao GND) .  
- Não estamos usando um resistor externo, pois o pull-up interno (~45 kΩ) já traciona o pino para 3.3V quando não há contato com o GND.  

### 1.5 Implementação

1. Sincronizar o relógio do ESP32 com um servidor [NTP](https://en.wikipedia.org/wiki/Network_Time_Protocol) (UTC timezone).  
2. Medir distância com HC-SR04 e converter para precipitação (mm).  
3. Armazenar o valor de hoje em [Preferences](https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/preferences.html).  
4. Montar JSON dos últimos 7 dias e enviar para o servidor via HTTP POST.  
5. Exibir status com LEDs:  
   - **Amarelo fixo**: durante a requisição HTTP.  
   - **Verde fixo**: sucesso.  
   - **Vermelho fixo**: servidor retornou `success: false`.  
   - **Laranja fixo**: erro na comunicação HTTP (timeout, 4xx, 5xx, etc.).  
6. Botão manual para forçar envio.  

---

## 2. Seleção do sensor e abordagem de medição

### 2.1 HC-SR04 a 3.3V

- O **HC-SR04** opera normalmente em **5V** (faixa de 2 cm a 400 cm), mas funciona em **3.3V** com alcance limitado ~200 cm.  
- A saída **Echo** do HC-SR04 ligado a 3.3V também fica em 3.3V, compatível com os GPIOs do ESP32 sem necessidade de divisor de tensão .  
- No Wokwi, basta ligar:  
  - `VCC (HC-SR04) → ESP32 3V3`  
  - `GND (HC-SR04) → ESP32 GND`  
  - `TRIG → GPIO 5`  
  - `ECHO → GPIO 18`  

### 2.2 Fórmula de Precipitação

1. **Convertendo duração do pulso para distância** $D$, em cm:  
   - Dispara‐se `TRIG` por 10 μs e lê‐se o pulso **echo** com `pulseIn(ECHO_PIN, HIGH, 30000)` → `duration` (μs).  
   - Convertendo o pulso para centímetros:
   
   $D_{\text{cm}} = \frac{\text{duration (μs)}}{58.0}$

2. **Altura do recipiente** $H_{\text{container}}$, em cm: por exemplo, **20 cm**.
3. **Distância medida** ($D_{\text{medida}}$)  
4. **Profundidade da água** ($h$), em cm:  
   
    $h = \max\bigl(0\, H - D_{\text{medida}}\bigr)$

5. **Precipitação** $P_{\text{mm}}$, em mm:  
   
   $P_{\text{mm}} = 10 \times h \quad(\text{visto que }1\text{cm} = 10\text{mm}).$

## 3. Rodar com API

O ESP32 depende que a API esteja rodando em segundo plano para que ele consiga enviar os dados medidos de precipitação e então aguardar o retorno do servidor.

Para saber como rodar a API localmente acesse [aqui](../api/readme.md)

### Observação
Após estiver com a API rodando será necessário descobrir qual o IPv4 do seu computador e então colocar no arquivo `src/Config.cpp` como no exemplo abaixo:
```c++
const char* SERVER_URL = "http://192.168.0.151:8000";
```

* Para descobrir o seu IPv4 rode no seu terminal (Linux e macOS): 
```bash
ifconfig 
```

* Para descobrir o seu IPv4 rode no seu terminal/powershell (Windows): 
```bash
ipconfig 
```
