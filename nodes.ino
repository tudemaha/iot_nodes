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
  
  // Serial.println("Starting DHT11...");
  dht.begin();

  // Serial.println("Starting MQ-135...");
  
  // Serial.println("Starting Soil Moisture Sensor...");
  pinMode(SOILPIN, INPUT);

  // Serial.println("Starting GPS...");
  Serial1.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  // Serial.println("Starting pH Sensor...");
  pinMode(PH_PIN, INPUT);

  // Serial.println("Starting Camera...");
  initCamera();
}

void loop() {
  float test = readPH();

  delay(3000);

}