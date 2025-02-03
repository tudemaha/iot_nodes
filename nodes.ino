#include "DHT.h"
#include "MQ135.h"
#include "struct.h"

#define DHTPIN 2
#define MQ135_PIN 13

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
MQ135 mq135(MQ135_PIN);

void setup() {
  Serial.begin(115200);
  
  Serial.println("Starting DHT11...");
  dht.begin();

  Serial.println("Starting MQ-135...");
}

void loop() {
  dhtReading read = readDht();

  float ppm = readGas(read.temperature, read.humidity);
  Serial.println(ppm);
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