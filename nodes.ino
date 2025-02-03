#include "DHT.h"
#include "MQ135.h"
#include "struct.h"

#define DHTPIN 2
#define MQ135_PIN 13
#define SOILPIN 12

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
MQ135 mq135(MQ135_PIN);

void setup() {
  Serial.begin(115200);
  
  Serial.println("Starting DHT11...");
  dht.begin();

  Serial.println("Starting MQ-135...");
  
  Serial.println("Starting Soil Moisture Sensor...");
  pinMode(SOILPIN, INPUT);
}

void loop() {
  float sensor = readSoilMoisture();
  Serial.println(sensor);
  delay(2000);

}

dhtReading readDht() {
  dhtReading reading;

  reading.temperature = dht.readTemperature();
  reading.humidity = dht.readHumidity();

  if(isnan(reading.temperature) || isnan(reading.humidity)) {
    reading.temperature = reading.humidity = 0;
    Serial.println("Failed to read from DHT sensor.");
  }

  return reading;
}

float readGas(float temperature, float humidity) {
  float ppm = mq135.getCorrectedPPM(temperature, humidity);

  return ppm;
}

float readSoilMoisture() {
  int sensor = analogRead(SOILPIN);
  float soilMoisture = sensor * (100.0 / 4095.0);

  return soilMoisture;
}