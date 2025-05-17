#ifndef _HELPER_

#define _HELPER_

struct dhtReading {
  float temperature;
  float humidity;
};

struct gpsReading {
  String coordinate;
  String date;
  String time;
};

struct sensorReading {
  dhtReading dht;
  gpsReading gps;
  float ph;
  float soilMoisture;
  float gas;
};

#endif