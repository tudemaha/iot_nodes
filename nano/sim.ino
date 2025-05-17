#include "helper.h"

String apn      = "internet";
String username = "wap";
String password = "wap123";
String endpoint = "http://logpress.my.id/sensors";

void postPayload(sensorReading sr) {
  sendATCommand("AT");
  sendATCommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  sendATCommand("AT+CSTT=\"" + apn + "\",\"" + username + "\",\"" + password + "\"");
  sendATCommand("AT+SAPBR=1,1");

  sendATCommand("AT+HTTPINIT");
  sendATCommand("AT+HTTPPARA=\"CID\",1");
  sendATCommand("AT+HTTPPARA=\"URL\",\"" + endpoint + "\"");
  sendATCommand("AT+HTTPPARA=\"CONTENT\",\"text/plain\"");

  // parseJson
  String payload = "";

  payload += parseTimestamp(sr.gps.date, sr.gps.time) + ";";
  payload += "2a7902f1-dfd2-448e-8802-03055f4584ff;";
  payload += String(sr.dht.temperature) + ";";
  payload += String(sr.dht.humidity) + ";";
  payload += String(sr.ph) + ";";
  payload += String(sr.soilMoisture) + ";";
  payload += String(sr.gas) + ";";
  if(sr.gps.coordinate == "0.0, 0.0") {
    payload += "-8.588111, 115.101969";
  } else {
    payload += sr.gps.coordinate;
  }

  // payload += "{";
  // payload += "\"timestamp\": \"";
  // payload += parseTimestamp(sr.gps.date, sr.gps.time);
  // payload += "\", ";
  // payload += "\"node_id\": \"kon\", ";
  // payload += "\"temp\": " + String(sr.dht.temperature) + ", ";
  // payload += "\"humid\": " + String(sr.dht.humidity) + ", ";
  // payload += "\"soil_ph\": " + String(sr.ph) + ", ";
  // payload += "\"soil_moisture\": " + String(sr.soilMoisture) + ", ";
  // payload += "\"gas\": " + String(sr.gas) + ", ";
  // payload += "\"gps\": \"" + sr.gps.coordinate + "\"";
  // payload += "}";

  Serial.println(payload);

  sendATCommand("AT+HTTPDATA=" + String(payload.length()) + ",30000");
  delay(100);
  gsm.print(payload);
  delay(1000);  
  sendATCommand("AT+HTTPACTION=1");

  // sendATCommand("AT+HTTPREAD");
  sendATCommand("AT+HTTPTERM");
}

String parseTimestamp(String date, String time) {
  String timestamp = "";

  if(date != "0") {
    // ddmmyy
    timestamp += "20" + date.substring(4, 6) + "-";
    timestamp += date.substring(2, 4) + "-";
    timestamp += date.substring(0, 2) + "T";
    // hhmmsscc
    timestamp += time.substring(0, 2) + ":";
    timestamp += time.substring(2, 4) + ":";
    timestamp += time.substring(4, 6) + "Z";
  } else {
    timestamp +=  "2000-01-01T00:00:00Z";
  }

  return timestamp;
}

void sendATCommand(String command) {
  gsm.println(command);
  delay(500);
}

void readSerial() {
  while(gsm.available() != 0) {
    Serial.write(gsm.read());
    delay(500);
  }
}