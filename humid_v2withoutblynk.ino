#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME "main"
#define BLYNK_AUTH_TOKEN ""

#include <PubSubClient.h>
#include "DHT.h"
#include "WiFi.h"
#include <ArduinoJson.h>
#include <BlynkSimpleEsp32.h>

#define DHTPIN "DHT Pin"       
#define WATER_PIN "Water Level Pin"      
#define HUMIDIFIER_PIN "Humidifier Pin" 

#define MOTOR_ENA "Enable Pin"
#define MOTOR_IN1 "Input Pin 1"
#define MOTOR_IN2 "Input Pin 2"
#define RELAY_PIN "Relay Pin"

#define DHTTYPE DHT11          
#define HUMIDITY_THRESHOLD 20   

const char* SSID = "Wifi SSID";
const char* PASS = "Wifi Password";
const char* serverUrl = "Server Url";

BlynkTimer timer;

const char* mqtt_server = "MQTT Ip Address";  
const int mqtt_port = 1884;
const char* mqtt_user = "MQTT User";          
const char* mqtt_password = "MQTT Pass";      
const char* mqtt_topic = "Topic Destination";

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

void startMotor(int speed) {
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_ENA, 255);
}

void stopMotor() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_ENA, 0);
}

void setup_wifi() {
    delay(10);
    Serial.println("Connecting to WiFi...");
    WiFi.begin(SSID, PASS);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
    Serial.println("Connected to MQTT broker");
  } 
  else {
    Serial.print("Failed to connect to MQTT broker, rc=");
    Serial.println(client.state());
  }
  Serial.println(" try again in 5 seconds");
  delay(5000);
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();

  Blynk.begin(BLYNK_AUTH_TOKEN, SSID, PASS);
  client.setServer(mqtt_server, mqtt_port);
  dht.begin();

  pinMode(WATER_PIN, INPUT);
  pinMode(HUMIDIFIER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(MOTOR_ENA, OUTPUT);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);

  digitalWrite(HUMIDIFIER_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);

  stopMotor();
  // timer.setInterval(1000L,timer)
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  Blynk.run();

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int waterLevel = analogRead(WATER_PIN);

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (!isnan(humidity) && !isnan(temperature)) {
    Blynk.virtualWrite(V1, humidity);
    Blynk.virtualWrite(V3, temperature);
  }
  float waterLevelPercent = map(waterLevel, 0, 4095, 0, 100);
  Blynk.virtualWrite(V2, waterLevelPercent);

  // Create JSON object with sensor data
  StaticJsonDocument<200> doc;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["waterLevel"] = waterLevel;
  String jsonData;
  serializeJson(doc, jsonData);

  if (humidity > HUMIDITY_THRESHOLD) {
    digitalWrite(HUMIDIFIER_PIN, HIGH);
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Humidifier ON");
    startMotor(200);
  } 
  else {
    digitalWrite(HUMIDIFIER_PIN, LOW);
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Humidifier OFF");
    stopMotor();
  }

  // Display water level status
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %  Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C  Water Level: ");
  Serial.println(waterLevel < 2700 ? "Low" : (waterLevel < 3500 ? "Sufficient" : "Full"));

  delay(1000);
}
