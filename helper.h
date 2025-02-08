#ifndef _HELPER_

#define _HELPER_

struct dhtReading {
  float temperature;
  float humidity;
};

struct payloadData {
  String node_id;
  String gateway_id;
  dhtReading dht;
  float gas;
  float soil_ph;
  String coordinate;
  float soil_moisture;
  camera_fb_t* image;
};

#endif