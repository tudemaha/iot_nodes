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

void saveImage(camera_fb_t* image) {
  String path = "/picture" + String("1") + String(".jpg");

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