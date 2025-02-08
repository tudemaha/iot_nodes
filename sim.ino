#include "helper.h"

String apn      = "internet";
String username = "wap";
String password = "wap123";
String endpoint = "34.168.140.97:8081/upload";

void postPayload(payloadData p) {
  sendATCommand("AT");
  sendATCommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  sendATCommand("AT+CSTT=\"" + apn + "\",\"" + username + "\",\"" + password + "\"");
  sendATCommand("AT+SAPBR=1,1");

  sendATCommand("AT+HTTPINIT");
  sendATCommand("AT+HTTPPARA=\"CID\",1");
  sendATCommand("AT+HTTPPARA=\"URL\",\"" + endpoint + "\"");

  // payload
  String payload = "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n";
  payload += "Content-Disposition: form-data; name=\"node_id\"\r\n\r\n" + p.node_id + "\r\n";
  payload += "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n";
  payload += "Content-Disposition: form-data; name=\"gateway_id\"\r\n\r\n" + p.gateway_id + "\r\n";
  payload += "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n";
  payload += "Content-Disposition: form-data; name=\"humidity\"\r\n\r\n" + String(p.dht.humidity) + "\r\n";
  payload += "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n";
  payload += "Content-Disposition: form-data; name=\"temperature\"\r\n\r\n" + String(p.dht.temperature) + "\r\n";
  payload += "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n";
  payload += "Content-Disposition: form-data; name=\"soil_moisture\"\r\n\r\n" + String(p.soil_moisture) + "\r\n";
  payload += "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n";
  payload += "Content-Disposition: form-data; name=\"soil_ph\"\r\n\r\n" + String(p.soil_ph) + "\r\n";
  payload += "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n";
  payload += "Content-Disposition: form-data; name=\"gas\"\r\n\r\n" + String(p.gas) + "\r\n";
  payload += "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n";
  payload += "Content-Disposition: form-data; name=\"coordinate\"\r\n\r\n" + p.coordinate + "\r\n";
  payload += "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n";
  payload += "Content-Disposition: form-data; name=\"image\"; filename=\"picture.jpg\"\r\n";
  payload += "Content-Type: image/jpeg\r\n\r\n";

  String endBoundary = "\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW--\r\n";

  sendATCommand("AT+HTTPPARA=\"CONTENT\",\"multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW\"");
  sendATCommand("AT+HTTPDATA=" + String(payload.length() + p.image->len + endBoundary.length()) + ",60000");
  
  Serial2.print(payload);
  uint8_t* fbBuf = p.image->buf;
  size_t fbLen = p.image->len;
  for(size_t n = 0; n < fbLen; n = n + 1024) {
    if(n + 1024 < fbLen) {
      Serial2.write(fbBuf, 1024);
      fbBuf += 1024;
    } else if(fbLen % 1024 > 0) {
      size_t remainder = fbLen % 1024;
      Serial2.write(fbBuf, remainder);
    }
  }
  Serial2.print(endBoundary);

  sendATCommand("AT+HTTPACTION=1");
  delay(5000);
  sendATCommand("AT+HTTPREAD");
  sendATCommand("AT+HTTPTERM");

  sendATCommand("AT+CIPSHUT");
}

void sendATCommand(String command) {
  Serial2.println(command);
  delay(1000);
}