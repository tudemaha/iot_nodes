#include "DHT.h"
#include "MQ135.h"
#include "TinyGPS.h"
#include "esp_camera.h"
#include "helper.h"

#define SIM_RX 3
#define SIM_TX 1
#define DHTPIN 2
#define MQ135_PIN 15
#define SOILPIN 13
#define GPS_RX 16
#define GPS_TX 14
#define PH_PIN 12

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
MQ135 mq135(MQ135_PIN);
TinyGPS gps;

void setup() {
  delay(10000);
  // Serial.begin(115200);

  // Serial.println("Starting Camera...");
  initCamera();
  
  // Serial.println("Starting DHT11...");
  dht.begin();

  // Serial.println("Starting MQ-135...");
  
  // Serial.println("Starting Soil Moisture Sensor...");
  pinMode(SOILPIN, INPUT);

  // Serial.println("Starting pH Sensor...");
  pinMode(PH_PIN, INPUT);

  // Serial.println("Starting GPS...");
  // Serial1.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  // Serial.println("Starting SIM Connection...");
  Serial2.begin(9600, SERIAL_8N1, SIM_RX, SIM_TX);
}

void loop() {
  delay(5 * 60 * 1000);

  payloadData p;
  p.node_id = "0194dc54-8715-7da0-864c-7cc6c3d5a77c";
  p.gateway_id = "0194dc54-ea20-7b4c-b06c-38e171d8ce30";
  p.dht = readDht();
  p.gas = readGas(p.dht.temperature, p.dht.humidity);
  p.soil_moisture = readSoilMoisture();
  p.coordinate = readGPS();
  p.soil_ph = readPH();
  p.image = takePicture();

  postPayload(p);

  esp_camera_fb_return(p.image);
}