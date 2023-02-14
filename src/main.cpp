/*
  Post temperature and humidity from ESP32S3
*/

#include <WiFi.h>
#include "ThingSpeak.h"
#include <Arduino.h>
#include "Adafruit_SHT31.h"
#include <SPI.h>

// Insert your network credentials
#define WIFI_SSID "Student"
#define WIFI_PASSWORD "Kristiania1914"

Adafruit_SHT31 sht31 = Adafruit_SHT31();
WiFiClient client;

/* MQTT - Credentials for connecting to ThingSpeak MQTT server.
const char mqttUserName[] = "NDgiFBgfEDYQLS43DBsODzM"; 
const char clientID[] = "NDgiFBgfEDYQLS43DBsODzM";
const char mqttPass[] = "/E8Knfx8FUCBNxpMWAfqVFW1";
// It is strongly recommended to use secure connections. However, certain hardware does not work with the WiFiClientSecure library.
// Comment out the following #define to use non-secure MQTT connections to ThingSpeak server. 
//#define USESECUREMQTT

#include <PubSubClient.h>
  const char * PROGMEM thingspeak_ca_cert = \
  "-----BEGIN CERTIFICATE-----\n" \
  "MIIDxTCCAq2gAwIBAgIQAqxcJmoLQJuPC3nyrkYldzANBgkqhkiG9w0BAQUFADBs\n" \
  "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
  "d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j\n" \
  "ZSBFViBSb290IENBMB4XDTA2MTExMDAwMDAwMFoXDTMxMTExMDAwMDAwMFowbDEL\n" \
  "MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3\n" \
  "LmRpZ2ljZXJ0LmNvbTErMCkGA1UEAxMiRGlnaUNlcnQgSGlnaCBBc3N1cmFuY2Ug\n" \
  "RVYgUm9vdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMbM5XPm\n" \
  "+9S75S0tMqbf5YE/yc0lSbZxKsPVlDRnogocsF9ppkCxxLeyj9CYpKlBWTrT3JTW\n" \
  "PNt0OKRKzE0lgvdKpVMSOO7zSW1xkX5jtqumX8OkhPhPYlG++MXs2ziS4wblCJEM\n" \
  "xChBVfvLWokVfnHoNb9Ncgk9vjo4UFt3MRuNs8ckRZqnrG0AFFoEt7oT61EKmEFB\n" \
  "Ik5lYYeBQVCmeVyJ3hlKV9Uu5l0cUyx+mM0aBhakaHPQNAQTXKFx01p8VdteZOE3\n" \
  "hzBWBOURtCmAEvF5OYiiAhF8J2a3iLd48soKqDirCmTCv2ZdlYTBoSUeh10aUAsg\n" \
  "EsxBu24LUTi4S8sCAwEAAaNjMGEwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQF\n" \
  "MAMBAf8wHQYDVR0OBBYEFLE+w2kD+L9HAdSYJhoIAu9jZCvDMB8GA1UdIwQYMBaA\n" \
  "FLE+w2kD+L9HAdSYJhoIAu9jZCvDMA0GCSqGSIb3DQEBBQUAA4IBAQAcGgaX3Nec\n" \
  "nzyIZgYIVyHbIUf4KmeqvxgydkAQV8GK83rZEWWONfqe/EW1ntlMMUu4kehDLI6z\n" \
  "eM7b41N5cdblIZQB2lWHmiRk9opmzN6cN82oNLFpmyPInngiK3BD41VHMWEZ71jF\n" \
  "hS9OMPagMRYjyOfiZRYzy78aG6A9+MpeizGLYAiJLQwGXFK3xPkKmNEVX58Svnw2\n" \
  "Yzi9RKR/5CYrCsSXaQ3pjOLAEFe4yHYSkVXySGnYvCoCWw9E1CAx2/S6cCZdkGCe\n" \
  "vEsXCS+0yx5DaMkHJ8HSXPfqIbloEpw8nL+e/IBcm2PN7EeqJSdnoDfzAIJ9VNep\n" \
  "+OkuE6N36B9K\n" \
  "-----END CERTIFICATE-----\n";

#ifdef USESECUREMQTT
  #include <WiFiClientSecure.h>
  #define mqttPort 8883
  WiFiClientSecure client; 
#else
  #define mqttPort 1883
  WiFiClient client;
#endif

const char* server = "mqtt3.thingspeak.com";
int status = WL_IDLE_STATUS; 
long lastPublishMillis = 0;
int connectionDelay = 1;
int updateInterval = 15;
PubSubClient mqttClient( client );
*/

// Ensure that the credentials here allow you to publish and subscribe to the ThingSpeak channel.
#define channelID 2026062 // Channel 1
//#define channelID 2028068 // Channel 2

unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "66FH6QWIPM8DQ51P"; // Channel 1
//const char * myWriteAPIKey = "9FVC2TLC4AMF5ARC"; // Channel 2

/* MQTT - Function for publishing to MQTT channel with payload.
void mqttPublish(long pubChannelID, String message, String fieldNr) {
  String topicString ="channels/" + String( pubChannelID ) + "/publish/fields/field" + fieldNr;
  Serial.println(topicString.c_str());
  int x = mqttClient.publish( topicString.c_str(), message.c_str() );
  Serial.printf("return from publish %d\n", x);
}
*/

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

/* MQTT - Connecting to MQTT
void mqttConnect() {
  // Loop until connected.
  while ( !mqttClient.connected() )
  {
    // Connect to the MQTT broker.
    if ( mqttClient.connect( clientID, mqttUserName, mqttPass ) ) {
      Serial.print( "MQTT to " );
      Serial.print( server );
      Serial.print (" at port ");
      Serial.print( mqttPort );
      Serial.println( " successful." );
    } else {
      Serial.print( "MQTT connection failed, rc = " );
      // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
      Serial.print( mqttClient.state() );
      Serial.println( " Will try again in a few seconds" );
      delay( connectionDelay*1000 );
    }
  }
}
*/

void setup(){
  Serial.begin(115200);
  delay(3000);
  initWifi();
  ThingSpeak.begin(client);
  
  // mqttClient.setServer( server, mqttPort );

  Serial.println("SHT31 test");
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }

  /* MQTT - Use secure connection
  #ifdef USESECUREMQTT
    // Handle functionality differences of WiFiClientSecure library for different boards.
    #ifdef ESP8266BOARD
      client.setFingerprint(thingspeak_cert_thumbprint);
    #else
      client.setCACert(thingspeak_ca_cert);
    #endif
  #endif
  */
}

float t, h;
int tempField = 1;
int humField = 2;
unsigned long lastMillis = 0;

void loop() {
  /* MQTT - Keep connection to MQTT
  if (!mqttClient.connected()) {
     mqttConnect();
  }

  mqttClient.loop();
  */

  // Control how often we read temperature from ESP32S3 and if it reads temperature or not
  if((millis() - lastMillis > 16000 || lastMillis == 0)){
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
    /* MQTT - Publish temperature and humidity to respective field
    mqttPublish( channelID, (String(t)), String(tempField));
    mqttPublish( channelID, (String(h)), String(humField));
    */
    float testLow = 23;
    float testHigh = 34;
    ThingSpeak.setField(1, testHigh);
    ThingSpeak.setField(2, h);
    
    //int x = ThingSpeak.writeField(myChannelNumber, 1, t, myWriteAPIKey);
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if(x == 200){
      Serial.println("Channel update successful.");
    }
  }

  /*
  char buffer[20];
  sprintf(buffer, "%.2f", t);
  */
}