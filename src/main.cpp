#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define TRIG 21
#define ECHO 18
#define LED 33

const char* ssid = "FIAP-IOT";
const char* password = "F!@p25.IOT";

const char* serverUrl = "http://172.22.0.99:5000/dados";

float medirDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duracao = pulseIn(ECHO, HIGH, 30000);
  if (duracao == 0) return -1;

  return duracao * 0.034 / 2;
}

String classificar(float dist) {

  if (dist > 0 && dist < 0.04) return "RISCO";  // até ~0.03

  if (dist >= 0.04 && dist < 40) return "SEGURO"; // 0.05 entra aqui

  return "SEGURO";
}

void setup() {
  Serial.begin(115200);

  dht.begin();

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED, OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando WiFi...");
  }

  Serial.println("WiFi conectado!");
}

void loop() {

  float temp = dht.readTemperature();
  float umidade = dht.readHumidity();
  float dist = medirDistancia();

  if (isnan(temp) || isnan(umidade) || dist == -1) {
    delay(2000);
    return;
  } 
  
  String estado = classificar(dist);

  digitalWrite(LED, estado == "RISCO" ? HIGH : LOW);

  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    String json = "{";
    json += "\"temp\":" + String(temp) + ",";
    json += "\"umidade\":" + String(umidade) + ",";
    json += "\"distancia\":" + String(dist) + ",";
    json += "\"estado\":\"" + estado + "\"";
    json += "}";

    int httpResponseCode = http.POST(json);

    Serial.print("Resposta API: ");
    Serial.println(httpResponseCode);

    http.end();
  }

  delay(5000);
}