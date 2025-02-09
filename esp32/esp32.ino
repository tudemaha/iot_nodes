#include "esp_camera.h"

#define NANO_RX 3
#define NANO_TX 1

void setup() {
  Serial2.begin(9600, SERIAL_8N1, NANO_RX, NANO_TX);

  initCamera();
  initSD();
}

void loop() {
  camera_fb_t* image = takePicture();
  saveImage(image);
  delay(100);
  esp_camera_fb_return(image);
}
