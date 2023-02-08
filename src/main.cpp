/*
  Post temperature from ESP32S3
*/

#include <WiFi.h>
#include "ThingSpeak.h"
#include <Arduino.h>
#include "Adafruit_SHT31.h"
#include <SPI.h>

// Insert your network credentials
#define WIFI_SSID "Student"
#define WIFI_PASSWORD "Kristiania1914"
//#define WIFI_SSID "MaxChillOutCrib"
//#define WIFI_PASSWORD "Ch1ll3rn!"
Adafruit_SHT31 sht31 = Adafruit_SHT31();
WiFiClient  client;

unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "66FH6QWIPM8DQ51P";

void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}
void setup(){
  Serial.begin(115200);
  initWiFi();

  ThingSpeak.begin(client);

  Serial.println("SHT31 test");
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
}

float t, h;
unsigned long lastMillis = 0;

void loop() {
  
  // Control how often we read temperature from ESP32S3 and if it reads temperature or not
  if((millis() - lastMillis > 14500 || lastMillis == 0)){
    lastMillis = millis();
    t = sht31.readTemperature();
    h = sht31.readHumidity();

    if (! isnan(t)) {  // check if 'is not a number'
      Serial.print("Reading from ESP32S3, temp *C = "); Serial.println(t);
    } else {
      Serial.println("Failed to read temperature");
    }
    //Humidity 
    if (! isnan(h)) {  // check if 'is not a number'
      Serial.print("Hum. % = "); Serial.println(h);
    } else { 
      Serial.println("Failed to read humidity");
    }

    int x = ThingSpeak.writeField(myChannelNumber, 1, t, myWriteAPIKey);
    if(x == 200){
      Serial.println("Channel update successful.");
    }
  }
  delay(10);
  /*
  char buffer[20];
  sprintf(buffer, "%.2f", t);
  if (millis() - lastMillis > 15000) {
    lastMillis = millis();
    Serial.printf("publishing\n");
    int x = ThingSpeak.writeField(myChannelNumber, 1, t, myWriteAPIKey);
    if(x == 200){
      Serial.println("Channel update successful.");
    }
  }
  */
}