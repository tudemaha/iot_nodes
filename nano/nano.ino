#include "DHT.h"
#include "MQ135.h"
#include "TinyGPS.h"
#include <SoftwareSerial.h>
#include "helper.h"

#define GPS_RX    2
#define GPS_TX    3
#define DHTPIN    4
#define MQ135_PIN A0
#define SOILPIN   A1
#define PH_PIN    A2
#define ESP_RX    5
#define ESP_TX    6

#define DHTTYPE   DHT11

DHT dht(DHTPIN, DHTTYPE);
MQ135 mq135(MQ135_PIN);
TinyGPS gps;

SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
SoftwareSerial esp32(ESP_RX, ESP_TX);

String serialInput = "";
String sensorReading = "";
gpsReading gr;

void setup() {
  Serial.begin(9600);

  dht.begin();
  Serial.println("DHT11 Connected.");

  Serial.println("MQ-135 Connected.");
  
  pinMode(SOILPIN, INPUT);
  Serial.println("Soil Moisture Sensor Connected.");

  pinMode(PH_PIN, INPUT);
  Serial.println("pH Sensor Connected.");

  gpsSerial.begin(4800);
  Serial.println("GPS Connected.");

  esp32.begin(9600);
  Serial.println("ESP32 Connection Created.");
}

void loop() {
  gpsReading newGps = readGPS();
  if(gr.coordinate.length() < 10) {
    gr.coordinate = newGps.coordinate;
    gr.date = newGps.date;
    gr.time = newGps.time;
  }

  while(esp32.available() > 0) {
    serialInput = esp32.readString();
    
    if(serialInput == "read") {
      sensorReading = readSensor();
        
      sensorReading += gr.coordinate + ";";
      sensorReading += gr.date + ";";
      sensorReading += gr.time;

      esp32.print(sensorReading);
    }

    serialInput = "";
  }
}

String readSensor() {
  dhtReading dht = readDht();
  float gas = readGas(dht.temperature, dht.humidity);
  float soil_moisture = readSoilMoisture();
  float soil_ph = readPH();

  String sensorReading = String(dht.temperature) + ";" +
                         String(dht.humidity) + ";" +
                         String(gas) + ";" +
                         String(soil_moisture) + ";" +
                         String(soil_ph) + ";";
  return sensorReading;
}