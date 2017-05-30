#include <GPRS_Shield_Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define TX_PIN    7
#define RX_PIN    8
#define APN       "internet"

GPRS gprs(TX_PIN, RX_PIN, 9600);

void setup() {
  Serial.begin(9600);
  
  dht.begin();  
  
  while(!gprs.init()) {
    delay(1000);
    Serial.print(F("gprs init error\r\n"));
  }
  delay(3000);
  
  while(!gprs.join(F(APN))) {
    Serial.print(F("error join network\r\n"));
    delay(2000);
  }
}

void loop() {
  delay(5000);
  
  // Build url
  char http_cmd[] = "POST / HTTP/1.1\r\nHost: example.com\r\nContent-Type: application/x-www-form-urlencoded\r\n";
  
  char data[64] = {0};
  strcat(data, "id=testing");
  
  // Read data len
  int dl = strlen(data);
  char dls[10] = {0};
  itoa(dl, dls, 10);
  
  // Add separator for HTTP
  strcat(data, "\r\n\r\n");
  
  // Build Content-Length
  char ct[20] = {0};
  strcat(ct, "Content-Length:");
  strcat(ct, dls);
  strcat(ct, "\r\n\r\n");
  
  //build it up together
  char http_cmd1[200] = {0};
  
  strcat(http_cmd1, http_cmd);
  strcat(http_cmd1, ct);
  strcat(http_cmd1, data);

  // Start connection
  if(!gprs.connect(TCP,"example.com", 80)) {
      Serial.println("Connect error");
  } else {
    gprs.send(http_cmd1, strlen(http_cmd1));
  }

  gprs.close();
  gprs.disconnect();
}

