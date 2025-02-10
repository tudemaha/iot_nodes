#include "FS.h"
#include "SD_MMC.h"
#include "esp_camera.h"

void initSD() {
    if(!SD_MMC.begin("/sdcard")){
    // Serial.println("SD Card Mount Failed");
    delay(100);
    ESP.restart();
    return;
  }

  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    // Serial.println("No SD Card attached");
    delay(100);
    ESP.restart();
    return;
  }
}

void saveImage(camera_fb_t* image, String filename) {
  String path = "/images/" + filename + String(".jpg");

  fs::FS &fs = SD_MMC;
  // Serial.printf("Image file name: %s\n", path.c_str());

  File file = fs.open(path.c_str(), FILE_WRITE);
  if(!file){
    // Serial.println("Failed to open file in writing mode");
  } else {
    file.write(image->buf, image->len); // payload (image), payload length
    // Serial.printf("Saved file to path: %s\n", path.c_str());
  }

  file.close();
}

void saveMetadata(String metadata) {
  String path = "/sensors.csv";

  fs::FS &fs = SD_MMC;

  File file = fs.open(path.c_str(), FILE_APPEND);
  if(!file) {
    // Serial.println("Failed to open file in append mode");
  } else {
    file.println(metadata);
  }

  file.close();
}