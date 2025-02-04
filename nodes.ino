#include "DHT.h"
#include "MQ135.h"
#include "TinyGPS.h"
#include "helper.h"

#define DHTPIN 2
#define MQ135_PIN 13
#define SOILPIN 12
#define GPS_RX 15
#define GPS_TX 14
#define PH_PIN 14

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
MQ135 mq135(MQ135_PIN);
TinyGPS gps;

void setup() {
  Serial.begin(115200);
  
  Serial.println("Starting DHT11...");
  dht.begin();

  Serial.println("Starting MQ-135...");
  
  Serial.println("Starting Soil Moisture Sensor...");
  pinMode(SOILPIN, INPUT);

  Serial.println("Starting GPS...");
  Serial1.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  Serial.println("Starting pH Sensor...");
  pinMode(PH_PIN, INPUT);
}

void loop() {
  float ph = readPH();
  Serial.println(ph);
  Serial.println();
  delay(1000);

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

String readGPS() {
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  for(unsigned long start = millis(); millis() - start < 1000;) {
    while(Serial1.available()) {
      char c = Serial1.read();
      if(gps.encode(c)) newData = true;
    }
  }

  if(newData) {
    float flat, flon;
    unsigned long age;

    gps.f_get_position(&flat, &flon, &age);
    String latLon = String(flat, 6) + ", " + String(flon, 6);

    return latLon;
  }

  gps.stats(&chars, &sentences, &failed);
  Serial.print("CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  
  if(chars == 0)
    Serial.println("No charactes received, check wiring!");
  
  return "0.0, 0.0";
}

float readPH() {
  int phADC = analogRead(PH_PIN);
  Serial.println(phADC);
  float ph = -0.01734 * phADC + 7.38552;
  
  return ph;
}