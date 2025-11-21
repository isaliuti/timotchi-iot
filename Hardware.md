# Esquema de Montagem e Hardware

Este documento detalha a interconexão dos componentes do projeto **Timotchi**, realizada através de uma topologia em estrela, com o ESP32 atuando como o nó central de processamento e distribuição de energia.

## Descrição Técnica da Montagem

O esquema de ligação detalhado segue as especificações dos protocolos de comunicação utilizados:

### 1. Interface com o Sensor (Protocolo I2C)
O sensor de distância **VL53L0X** foi conectado ao barramento I2C padrão do ESP32.
- O pino de dados **SDA** do sensor foi ligado à porta **GPIO 21**.
- O pino de clock **SCL** foi conectado à porta **GPIO 22**.
- A alimentação do módulo é fornecida pela saída de **3.3V** do microcontrolador, com o terra (GND) comum ao circuito.

### 2. Interface com o Atuador (Protocolo SPI)
O display **TFT ST7789V** requer uma comunicação de alta velocidade, sendo conectado ao barramento VSPI do ESP32.
- A linha de dados **SDA/MOSI** do display foi ligada à porta **GPIO 23**.
- O sinal de clock **SCL/SCK** à porta **GPIO 18**.
- Para o controle de fluxo e comandos:
    - O pino de seleção **CS** (Chip Select) foi alocado ao **GPIO 5**.
    - O pino de comando/dados **DC** ao **GPIO 16 (RX2)**.
    - O pino de reinicialização **RST** ao **GPIO 17 (TX2)**.

## Tabela de Pinagem

Para facilitar a reprodução do projeto, utilize a tabela abaixo como guia de conexão:

### Sensor VL53L0X (Entrada)
| Pino Sensor | Pino ESP32 | Função |
| :--- | :--- | :--- |
| VCC | 3V3 / VIN | Alimentação |
| GND | GND | Terra |
| SDA | GPIO 21 | Dados I2C |
| SCL | GPIO 22 | Clock I2C |

### Display TFT ST7789 (Saída)
| Pino Display | Pino ESP32 | Função |
| :--- | :--- | :--- |
| VCC | 3V3 | Alimentação (3.3V) |
| GND | GND | Terra |
| SCL / SCK | GPIO 18 | Clock SPI |
| SDA / MOSI | GPIO 23 | Dados SPI |
| RES / RST | GPIO 17 | Reset (TX2) |
| DC / A0 | GPIO 16 | Data/Command (RX2) |
| CS | GPIO 5 | Chip Select |
    
    Sensor -->|SDA: 21 / SCL: 22| ESP32
    ESP32 -->|MOSI: 23 / SCK: 18| Display
    ESP32 -->|CS: 5 / DC: 16 / RST: 17| Display
