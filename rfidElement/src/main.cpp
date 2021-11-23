#include <Arduino.h>

//Time management
#include <time.h>
  //need absolute time for syncing across devices
const char timeServer1[] = "pool.ntp.org"; 
const char timeServer2[] = "time.nist.gov"; 
const char timeServer3[] = "time.google.com"; 

//RFID Card
#include <MFRC522.h> //library responsible for communicating with the module RFID-RC522
#include <SPI.h> //library responsible for communicating of SPI bus
#define SS_PIN 5
#define RST_PIN 33
MFRC522 mfrc522(SS_PIN, RST_PIN);

//LEDs
#include <FastLED.h>
#define LEDS_DATA_PIN 15
#define NUM_LEDS 12
#define CHIPSET WS2811
#define COLOR_ORDER GRB
#define BRIGHTNESS 4
CRGB leds[NUM_LEDS];
String currentProfile;

//Button
#define BUTTON_PIN 27

//WiFi
#include <wifiSetup.h>
WiFiClient wifi_client;
//const char wifiName[] = "Ponderosa";
//const char wifiPW[] = "Zaq12wsx";
const char wifiName[] = "NPS_Devices";
const char wifiPW[] = "FourScoreAnd7YearsAgo";


//MQTT
#include <mqttSetup.h>
MQTTClient mqtt_client;
const char clientName[] = "rfidElement";
const char brokerName[] = "broker.hivemq.com";
const int numPubs = 1;
mqtt_pubSubDef_t pubs[numPubs];
const int numSubs = 1;
mqtt_pubSubDef_t subs[numSubs];
void readSubs(String &topic, String &payload){
    Serial.println("incoming: " + topic + " - " + payload);
    for(int i=0; i < numSubs; i++){
        if(topic == subs[i].topic){ //check each message in the array for the correct subscriber
            subs[i].payload = payload; //store payload in the correct subscriber message
            break;
        }
    }
}

//General inits and defs
MQTT_Client_Handler rfid_mqtt_client(mqtt_client, wifi_client, brokerName, subs, numSubs, readSubs); //initialize the mqtt handler
void checkAndPublishTag();
void updateLEDs();
String getTimestamp();


void setup() {
  Serial.begin(115200);
///////////////   RFID   ///////////////
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 readers
  delay(4);				// time after init to be fully setup and ready 
///////////////   LEDS   ///////////////
  FastLED.addLeds<CHIPSET, LEDS_DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(BRIGHTNESS);
///////////////   WiFi   ///////////////
  wifi_init(wifiName, wifiPW);
///////////////   MQTT   ///////////////
  String deviceName = clientName; //converting const char to str
                //$$ SUBS $$//
    //listening to broker status
  subs[0].topic = "/" + deviceName + "/LEDs"; 
                //$$ PUBS $$//
    //posting score data from rfid readings
  pubs[0].topic = "/" + deviceName + "/tag";
  pubs[0].qos = 2; 
                //$$ connect $$//
  rfid_mqtt_client.connect(clientName);
///////////////   Time   ///////////////
  configTime(-5 * 3600, 0, timeServer1, timeServer2, timeServer3);
}

void loop() {
  if(!rfid_mqtt_client.loop()){
    rfid_mqtt_client.connect(clientName);
  }
  checkAndPublishTag();
  updateLEDs(subs[0].payload.toInt());
}

void checkAndPublishTag(){
  if (!mfrc522.PICC_IsNewCardPresent()){return;} //waiting for an RFID tag to approach
  if (!mfrc522.PICC_ReadCardSerial()){return;} //check if it's readable

    //obtain the tag's UID
  String uid = "";
  byte letter;
  for (letter = 0; letter < mfrc522.uid.size; letter++) {
     uid.concat(String(mfrc522.uid.uidByte[letter] < 0x10 ? " 0" : " "));
     uid.concat(String(mfrc522.uid.uidByte[letter], HEX));
  }

    //publish the uid read at the timestamp
  uid.toUpperCase();
  pubs[0].payload = uid + " @" + getTimestamp();
  rfid_mqtt_client.publish(pubs[0]);
    //wait a quarter second before reading and publishing another tag uid  
  delay(250); //TODO: determine correct delay
}

void updateLEDs(int numToShow){
    if(numToShow >= NUM_LEDS){
      numToShow = NUM_LEDS;
    }
    for(int i = 0; i < numToShow; i++){
      leds[i] = CRGB::BlueViolet;
    }
    for(int i = numToShow + 1; i < NUM_LEDS; i++){
      leds[i] = CRGB::Black;
    }
    FastLED.show();
}

String getTimestamp(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return "0000-00-00T-00:00:00-00:00";
  }

  char timeYear[5];
  strftime(timeYear, 5, "%Y", &timeinfo);
  char timeMonth[3];
  strftime(timeMonth, 5, "%m", &timeinfo);
  char timeDay[3];
  strftime(timeDay, 3, "%d", &timeinfo);
  char timeHr[3];
  strftime(timeHr, 3, "%H", &timeinfo);
  char timeMin[3];
  strftime(timeMin, 3, "%M", &timeinfo);
  char timeSec[3];
  strftime(timeSec, 3, "%S", &timeinfo);

  String timestamp;
  timestamp.concat(timeYear);
  timestamp.concat("-");
  timestamp.concat(timeMonth);
  timestamp.concat("-");
  timestamp.concat(timeDay);
  timestamp.concat("T-");
  timestamp.concat(timeHr);
  timestamp.concat(":");
  timestamp.concat(timeMin);
  timestamp.concat(":");
  timestamp.concat(timeSec);
  timestamp.concat("-");
  timestamp.concat("05:00"); //cst offset
  return timestamp;
}