#include "helper.h"

String apn      = "internet";
String username = "wap";
String password = "wap123";
String endpoint = "https://logpress.my.id/upload";

void postPayload(sensorReading sr) {
  sendATCommand("AT");
  sendATCommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  sendATCommand("AT+CSTT=\"" + apn + "\",\"" + username + "\",\"" + password + "\"");
  sendATCommand("AT+SAPBR=1,1");

  sendATCommand("AT+HTTPINIT");
  sendATCommand("AT+HTTPPARA=\"CID\",1");
  sendATCommand("AT+HTTPPARA=\"URL\",\"" + endpoint +"\"");
  sendATCommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"");

  // parseJson
  String payload;
  payload += "{";
  payload += "\"timestamp\": \"" + sr.gps.date + "\", ";
  payload += "\"node_id\": \"c6f961c4-fde9-460e-842f-5b8487690bfe\", ";
  payload += "\"temp\": " + String(sr.dht.temperature) + ", ";
  payload += "\"humid\": " + String(sr.dht.humidity) + ", ";
  payload += "\"soil_ph\": " + String(sr.ph) + ", ";
  payload += "\"soil_moisture\": " + String(sr.soilMoisture) + ", ";
  payload += "\"gas\": " + String(sr.gas) + ", ";
  payload += "\"gps\": \"" + sr.gps.coordinate + "\"";
  payload += "}";

  Serial.println(payload);

  sendATCommand("AT+HTTPDATA=" + String(payload.length()) + ",100000");
  sendATCommand(payload);
  sendATCommand("AT+HTTPACTION=1");

  sendATCommand("AT+HTTPREAD");
  readSerial();
  sendATCommand("AT+HTTPTERM");
  readSerial();
}

void sendATCommand(String command) {
  gsm.println(command);
  delay(1000);
}

void readSerial() {
  while(gsm.available() != 0) {
    Serial.write(gsm.read());
    delay(500);
  }
}