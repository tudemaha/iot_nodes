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

#endif