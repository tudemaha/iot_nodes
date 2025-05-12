#include "DHT.h"
#include "MQ135.h"
#include "TinyGPS.h"
#include "helper.h"
#include <SoftwareSerial.h>

#define GPS_RX    2
#define GPS_TX    3
#define DHT_PIN   4
#define GSM_RX    5
#define GSM_TX    6
#define MQ135_PIN A0
#define SOIL_PIN  A1
#define PH_PIN    A2

#define DHTTYPE   DHT11

DHT dht(DHT_PIN, DHTTYPE);
MQ135 mq135(MQ135_PIN);
TinyGPS gps;

SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
SoftwareSerial gsm(GSM_RX, GSM_TX);

sensorReading sr;

unsigned long previousMillis = 0;
const long interval = 5000;

void setup() {
  Serial.begin(9600);

  dht.begin();
  Serial.println("DHT11 Connected.");

  Serial.println("MQ-135 Connected.");
  
  pinMode(SOIL_PIN, INPUT);
  Serial.println("Soil Moisture Sensor Connected.");

  pinMode(PH_PIN, INPUT);
  Serial.println("pH Sensor Connected.");
}

void loop() {
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    startGPS();
    gpsReading gr = readGPS();
    endGPS();
    dhtReading dht = readDht();
    float gas = readGas(dht.temperature, dht.humidity);
    float soil_moisture = readSoilMoisture();
    float soil_ph = readPH();

    sr.dht = dht;
    sr.gps = gr;
    sr.ph = soil_ph;
    sr.soilMoisture = soil_moisture;
    sr.gas = gas;

    startGSM();
    // postPayload(sr);
    endGSM();

    Serial.println(sr.gps.date);
    Serial.println(sr.gps.time);
    Serial.println(sr.dht.temperature);
  }
}

void startGPS() {
  gpsSerial.begin(9600);
  Serial.println("GPS Connected.");
}

void endGPS() {
  gpsSerial.end();
  Serial.println("GPS Disconnected.");
}

void startGSM() {
  gsm.begin(9600);
  Serial.println("GSM Connected.");
}

void endGSM() {
  gsm.end();
  Serial.println("GSM Disconnected.");
}