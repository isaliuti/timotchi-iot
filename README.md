# 🐱 Timotchi — IoT para monitoramento do uso de telas

> Dispositivo físico que detecta sua presença em frente ao computador e emite um alerta visual após 1 hora contínua de uso, incentivando pausas saudáveis.

---

## O problema

Com o avanço do trabalho remoto, é cada vez mais comum passar 6, 8, 10 horas seguidas em frente a uma tela — sem perceber. Isso causa fadiga ocular, distúrbios do sono e dores de cabeça. Softwares de lembrete existem, mas são fáceis de ignorar ou fechar.

O Timotchi resolve isso com um dispositivo físico: ele fica do lado do seu monitor, detecta quando você está sentado e acende um alerta visual impossível de ignorar quando é hora de descansar.

---

## Como funciona

```
┌─────────────────────────────────────────────────────┐
│  Usuário presente?  →  Inicia contagem              │
│  < 60 min          →  Display verde  (monitorando)  │
│  ≥ 60 min          →  Display azul   (hora de parar)│
│  Usuário saiu      →  Reset do cronômetro           │
└─────────────────────────────────────────────────────┘
```

O sistema também publica os eventos em tempo real via **MQTT**, permitindo que qualquer dashboard ou app externo receba os dados.

- `timotchi/status` → publica `"presente"` ou `"ausente"`
- `timotchi/alerta` → publica `"ativo"` ou `"inativo"`

---

## Hardware necessário

| Componente | Modelo | Função |
|---|---|---|
| Microcontrolador | ESP32 ESP-WROOM-32 | Processamento e Wi-Fi |
| Sensor de distância | VL53L0X (ToF laser) | Detectar presença do usuário |
| Display | TFT 2.0" ST7789V (240x320) | Feedback visual e alertas |
| Protoboard | 830 pontos | Montagem do circuito |
| Jumpers | Macho-macho e macho-fêmea | Conexões elétricas |

**Alimentação:** USB via cabo (5V, corrente do próprio ESP32)

---

## Diagrama de conexões

```
VL53L0X          ESP32
  SDA    →    GPIO 21 (I2C SDA)
  SCL    →    GPIO 22 (I2C SCL)
  VIN    →    3.3V
  GND    →    GND

Display TFT ST7789V    ESP32
  MOSI         →    GPIO 23 (SPI MOSI)
  SCLK         →    GPIO 18 (SPI CLK)
  CS           →    GPIO 15
  DC           →    GPIO 2
  RST          →    GPIO 4
  VCC          →    3.3V
  GND          →    GND
```

> Consulte o arquivo [`docs/Hardware.md`](docs/Hardware.md) para mais detalhes sobre a pinagem.

---

## Como rodar

### Pré-requisitos

- [Arduino IDE](https://www.arduino.cc/en/software) instalado
- Suporte ao ESP32 instalado no Arduino IDE ([tutorial](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html))
- Bibliotecas instaladas via **Gerenciador de Bibliotecas**:
  - `TFT_eSPI` by Bodmer
  - `Adafruit_VL53L0X` by Adafruit
  - `PubSubClient` by Nick O'Leary

### Configuração

1. Clone o repositório:
   ```bash
   git clone https://github.com/isaliuti/timotchi-iot.git
   ```

2. Copie o arquivo de configuração de exemplo:
   ```bash
   cp src/config.example.h src/config.h
   ```

3. Preencha suas credenciais em `src/config.h`:
   ```cpp
   #define WIFI_SSID     "seu_wifi_aqui"
   #define WIFI_PASSWORD "sua_senha_aqui"
   ```

4. Abra `src/timotchi-iot.ino` na Arduino IDE

5. Selecione a placa: **Tools → Board → ESP32 Dev Module**

6. Faça o upload para o ESP32

---

## Resultados

O protótipo foi validado com medições de latência entre evento físico e resposta do sistema:

| Ação | Tempo de resposta |
|---|---|
| Sensor → Display (mudança de cor) | 150 ms |
| Sensor → MQTT (publicação de status) | ~415 ms |
| **Média geral** | **282 ms** |

---

## Estrutura do projeto

```
timotchi-iot/
├── src/
│   ├── timotchi-iot.ino     # Firmware principal
│   ├── config.h             # Suas credenciais (não versionado)
│   └── config.example.h    # Template de configuração
├── docs/
│   └── Hardware.md          # Detalhes de pinagem e componentes
├── README.md
└── LICENSE
```

---

## Tecnologias

![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=cplusplus&logoColor=white)
![Arduino](https://img.shields.io/badge/Arduino-00979D?style=flat&logo=arduino&logoColor=white)
![ESP32](https://img.shields.io/badge/ESP32-E7352C?style=flat&logo=espressif&logoColor=white)
![MQTT](https://img.shields.io/badge/MQTT-660066?style=flat&logo=mqtt&logoColor=white)

---

## Melhorias futuras

- [ ] Case em impressão 3D para acomodar os componentes
- [ ] Modo de baixo consumo energético (deep sleep)
- [ ] Dashboard web para visualizar histórico de uso
- [ ] Notificação por buzzer além do alerta visual

---

## Contexto acadêmico

Projeto desenvolvido para a disciplina de IoT da **Faculdade de Análise e Desenvolvimento de Sistemas — Universidade Presbiteriana Mackenzie (UPM)**, alinhado ao **ODS 3 da ONU** (saúde e bem-estar).

**Autor:** Isabella Schauble Liuti 
