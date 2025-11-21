#include <WiFi.h>
#include <PubSubClient.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "Adafruit_VL53L0X.h"

// --- CONFIGURAÇÕES DE REDE ---
const char* ssid = "dell vale eh suco";// Wifi
const char* password = "g0g0g0dzilla";// senha
const char* mqtt_server = "test.mosquitto.org"; // Broker

// --- OBJETOS ---
TFT_eSPI tft = TFT_eSPI();
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
WiFiClient espClient;
PubSubClient client(espClient);

// --- VARIÁVEIS ---
unsigned long tempoInicioPresenca = 0;
bool usuarioEstavaPresente = false;
bool alertaJaExibido = false;
const unsigned long TEMPO_LIMITE_MS = 3600000;

// --- CAT ---
const byte gatoMatriz[30][26] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,1,0,1,1,0,0,0,1,1,1,0,0,0,1,1,0,1,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

// --- FUNÇÕES VISUAIS ---
void desenharGato(int x_pos, int y_pos, int escala) {
  for (int y = 0; y < 30; y++) {
    for (int x = 0; x < 26; x++) {
      if (gatoMatriz[y][x] == 1) {
        tft.fillRect(x_pos + (x * escala), y_pos + (y * escala), escala, escala, TFT_BLACK);
      }
    }
  }
}

void desenharBarra(const char* texto1, const char* texto2, uint16_t corFundo) {
  tft.fillRect(0, 180, 320, 60, corFundo);
  tft.setTextSize(2); 
  tft.setTextColor(TFT_WHITE, corFundo);
  
  int x1 = (320 - (strlen(texto1) * 12)) / 2;
  if (x1 < 0) x1 = 0;
  tft.setCursor(x1, 190); 
  tft.print(texto1);
  
  if (strlen(texto2) > 0) {
    int x2 = (320 - (strlen(texto2) * 12)) / 2;
    if (x2 < 0) x2 = 0;
    tft.setCursor(x2, 215); 
    tft.print(texto2);
  }
}

// --- FUNÇÕES DE REDE (WI-FI & MQTT) ---
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando WiFi: ");
  Serial.println(ssid);
  
  // Barra amarela enquanto conecta
  desenharBarra("CONECTANDO", "WI-FI...", TFT_ORANGE);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando MQTT...");
    String clientId = "Timotchi-" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado!");
      client.publish("timotchi/status", "Sistema Iniciado");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

// --- SETUP ---
void setup() {
  Serial.begin(115200);

  // 1. Inicia Tela
  tft.init();
  tft.setRotation(1); 
  tft.fillScreen(TFT_WHITE);

  // 2. Desenha Gato (Fixo)
  desenharGato(82, 0, 6); 

  // 3. Inicia Sensor
  if (!lox.begin()) {
    desenharBarra("ERRO", "SENSOR", TFT_RED);
    while(1);
  }

  // 4. Inicia Rede
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  
  desenharBarra("TIMOTCHI", "ONLINE!", TFT_BLUE);
  delay(2000);
}

// --- LOOP ---
void loop() {
  // Garante conexão MQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Leitura Sensor
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  bool estaPresente = (measure.RangeMilliMeter < 800 && measure.RangeMilliMeter > 0);

  if (estaPresente) {
    // --- USUÁRIO CHEGOU ---
    if (!usuarioEstavaPresente) {
      usuarioEstavaPresente = true;
      alertaJaExibido = false; 
      tempoInicioPresenca = millis(); 
      
      // Ação Visual
      desenharBarra("OLA HUMANO!", "", TFT_DARKGREEN);
      // Ação MQTT
      client.publish("timotchi/status", "Presente");
      Serial.println("MQTT Enviado: Presente");
    }
    
    // --- VERIFICA TEMPO LIMITE (ALERTA) ---
    if (millis() - tempoInicioPresenca > TEMPO_LIMITE_MS) {
      if (!alertaJaExibido) {
        // Ação Visual
        desenharBarra("HORA DE", "DESCANSAR HUMANO", TFT_RED);
        // Ação MQTT
        client.publish("timotchi/alerta", "ATIVO");
        Serial.println("MQTT Enviado: Alerta Ativo");
        
        alertaJaExibido = true;
      }
    }

  } else {
    // --- USUÁRIO SAIU ---
    if (usuarioEstavaPresente) {
      usuarioEstavaPresente = false;
      alertaJaExibido = false;
      
      // Ação Visual
      desenharBarra("BOM DESCANSO", "HUMANO!", TFT_BLUE);
      // Ação MQTT
      client.publish("timotchi/status", "Ausente");
      client.publish("timotchi/alerta", "INATIVO");
      Serial.println("MQTT Enviado: Ausente");
    }
  }
    
  delay(200); 
}