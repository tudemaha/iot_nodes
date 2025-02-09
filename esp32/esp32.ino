#include "esp_camera.h"

#define NANO_RX 3
#define NANO_TX 1

void setup() {
  Serial2.begin(9600, SERIAL_8N1, NANO_RX, NANO_TX);

  initCamera();
  initSD();
}

void loop() {
  String sensorData = getSensors();
  delay(500);
  
  camera_fb_t* image = takePicture();

  saveImage(image, sensorData);
  delay(200);

  saveMetadata(sensorData);
  delay(100);

  esp_camera_fb_return(image);
}

String getSensors() {
  String sensorData = "";

  Serial2.print("read");
  while(Serial2.available() > 0) {
    sensorData = Serial2.readString();
  }

  return sensorData;
}