dhtReading readDht() {
  dhtReading reading;

  reading.temperature = dht.readTemperature();
  reading.humidity = dht.readHumidity();

  if(isnan(reading.temperature) || isnan(reading.humidity)) {
    reading.temperature = reading.humidity = 0;
    Serial.println("Failed to read from DHT sensor.");
  }

  return reading;
}


float readGas(float temperature, float humidity) {
  float ppm = mq135.getCorrectedPPM(temperature, humidity);

  return ppm;
}

float readSoilMoisture() {
  int sensor = analogRead(SOILPIN);
  float soilMoisture = sensor * (100.0 / 1023.0);

  return soilMoisture;
}

gpsReading readGPS() {
  gpsReading gr;

  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  for(unsigned long start = millis(); millis() - start < 1000;) {
    while(gpsSerial.available()) {
      char c = gpsSerial.read();
      if(gps.encode(c)) newData = true;
    }
  }

  if(newData) {
    float flat, flon;
    unsigned long age, date, time;

    gps.f_get_position(&flat, &flon, &age);
    gps.get_datetime(&date, &time);
    
    gr.coordinate = String(flat, 6) + ", " + String(flon, 6);
    gr.date = String(date, 6);
    gr.time = String(time, 6);

    return gr;
  }

  gps.stats(&chars, &sentences, &failed);
  Serial.print("CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  
  if(chars == 0) Serial.println("No charactes received, check wiring!");

  gr.coordinate = "0.0, 0.0";
  gr.date = "0";
  gr.time = "0";
  
  return gr;
}

float readPH() {
  int phADC = analogRead(PH_PIN);
  Serial.println(phADC);
  float ph = -0.06934 * phADC + 7.38552;
  
  return ph;
}