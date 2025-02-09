#include "esp_camera.h"

#define NANO_RX 3
#define NNAO_TX 1

void setup() {
  Serial2.begin(9600, SERIAL_8N1, NANO_RX, NANO_TX);

}

void loop() {
  // put your main code here, to run repeatedly:

}
