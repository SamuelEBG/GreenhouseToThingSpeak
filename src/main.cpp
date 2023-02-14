/*
  Post temperature and humidity from ESP32S3
*/

#include <WiFi.h>
#include "ThingSpeak.h"
#include <Arduino.h>
#include "Adafruit_SHT31.h"
#include <SPI.h>

// Insert your network credentials
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

Adafruit_SHT31 sht31 = Adafruit_SHT31();
WiFiClient client;

// Ensure that the credentials here allow you to publish and subscribe to the ThingSpeak channel.
#define channelID 2026062 // Channel 1
//#define channelID 2028068 // Channel 2

unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "66FH6QWIPM8DQ51P"; // Channel 1
//const char * myWriteAPIKey = "9FVC2TLC4AMF5ARC"; // Channel 2

// Connect to WiFi.
void initWifi() {
  Serial.println( "Connecting to Wi-Fi..." );
  // Loop until WiFi connection is successful
  
  while ( WiFi.status() != WL_CONNECTED ) {
    WiFi.begin( WIFI_SSID, WIFI_PASSWORD );
    Serial.print(".");
    delay(1000);
  }

  Serial.println( "Connected to Wi-Fi." );
}

void setup(){
  Serial.begin(115200);
  delay(3000);
  initWifi();
  ThingSpeak.begin(client);

  Serial.println("SHT31 test");
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
}

float t, h;
int tempField = 1;
int humField = 2;
unsigned long lastMillis = 0;

void loop() {
  // Control how often we read temperature from ESP32S3 and if it reads temperature or not
  if((millis() - lastMillis > 16000 || lastMillis == 0)){
    lastMillis = millis();
    t = sht31.readTemperature();
    h = sht31.readHumidity();

    if (! isnan(t)) {  // check if 'is not a number'
      Serial.println("Reading from ESP32S3"); 
      Serial.print("Temperature C˚ = "); Serial.println(t);
    } else {
      Serial.println("Failed to read temperature");
    }

    //Humidity 
    if (! isnan(h)) {  // check if 'is not a number'
      Serial.print("Humidity % = "); Serial.println(h);
    } else { 
      Serial.println("Failed to read humidity");
    }
  
    float testLow = 23;
    float testHigh = 34;
    ThingSpeak.setField(1, t);
    ThingSpeak.setField(2, h);
    
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if(x == 200){
      Serial.println("Channel update successful.");
      Serial.println("");
    }
  }
}