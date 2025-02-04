#include "DHT.h"
#include "MQ135.h"
#include "TinyGPS.h"
#include "esp_camera.h"
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

  Serial.println("Starting Camera...");
  initCamera();
}

void loop() {
  float test = readPH();

  delay(3000);

}