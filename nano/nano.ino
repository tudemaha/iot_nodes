#include "DHT.h"
#include "TinyGPS.h"
#include "helper.h"
#include <MQUnifiedsensor.h>
#include <SoftwareSerial.h>

#define GPS_RX    2
#define GPS_TX    3
#define DHT_PIN   4
#define GSM_RX    5
#define GSM_TX    6
#define MQ2_PIN   A0
#define SOIL_PIN  A1
#define PH_PIN    A2

#define DHTTYPE   DHT11

#define Board               ("Arduino NANO")
#define Type                ("MQ-2")
#define Voltage_Resolution  (5)
#define ADC_Bit_Resolution  (10)
#define RatioMQ2CleanAir    (9.83)

DHT dht(DHT_PIN, DHTTYPE);
TinyGPS gps;
MQUnifiedsensor MQ2(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ2_PIN, Type);

SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
SoftwareSerial gsm(GSM_RX, GSM_TX);

sensorReading sr;

unsigned long previousMillis = 0;
const long interval = 5000;

void setup() {
  startGSM();
  startGPS();
  Serial.begin(9600);

  dht.begin();
  Serial.println("DHT11 Connected.");

  MQ2.setRegressionMethod(1);
  MQ2.setA(574.25);
  MQ2.setB(-2.222);
  MQ2.init();
  Serial.println("Calibrating MQ2");
  float calcR0 = 0;
  for(int i = 1; i <= 10; i++) {
    MQ2.update();
    calcR0 += MQ2.calibrate(RatioMQ2CleanAir);
    Serial.print(".");
  }
  MQ2.setR0(calcR0 / 10);
  if(isinf(calcR0)) {
    Serial.println("MQ2: connection issue, R0 infinite");
    while(1);
  }
  if(calcR0 == 0) {
    Serial.println("MQ2: connection issue, R0 is 0");
    while(1);
  }
  Serial.println("MQ2 Connected.");
  
  pinMode(SOIL_PIN, INPUT);
  Serial.println("Soil Moisture Sensor Connected.");

  pinMode(PH_PIN, INPUT);
  Serial.println("pH Sensor Connected.");
}

void loop() {
    gpsReading gr = readGPS();
    dhtReading dht = readDht();
    float gas = readGas();
    float soil_moisture = readSoilMoisture();
    float soil_ph = readPH();

    sr.dht = dht;
    sr.gps = gr;
    sr.ph = soil_ph;
    sr.soilMoisture = soil_moisture;
    sr.gas = gas;

    postPayload(sr);

    Serial.println(sr.gps.coordinate);
    // Serial.println(sr.gps.time);
    // Serial.println(sr.dht.temperature);
  // }
  delay(2000);
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