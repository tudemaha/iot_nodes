#include "DHT.h"
#include "struct.h"

#define DHTPIN 2

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);



void setup() {
  Serial.begin(115200);
  
  Serial.println("Starting DHT11...");
  dht.begin();
}

void loop() {
  dhtReading read = readDht();
  Serial.print(read.temperature);
  Serial.print("\t");
  Serial.println(read.humidity);
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
