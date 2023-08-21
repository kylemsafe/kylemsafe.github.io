/**
******************************************************************************
* How to use this sketch
*
* This sketch uses the I2C interface to communicate with the NFC device.
* It writes an NFC tag type URI (Uniform Resource Identifier) and reads this same tag.
* Choose the uri by changing the content of uri_write.
*
* When the NFC module is started and ready, the message "Sytstem init done!" is
* displayed on the monitor window. Next, the tag is written, read and printed on
* the monitor window.
*
* You can also use your smartphone to read/write a tag.
* On Android, download a NFC Tools. Then start the app, check if NFC is activated
* on your smartphone. Put your smartphone near the tag, you can read it. You can
* write a tag with this app.
******************************************************************************
*/

// The ST25DVSensor.h library is required
// The hardware for this project is located TBD
 
#include "ST25DVSensor.h"

#define SerialPort      Serial

#if defined(ARDUINO_B_L4S5I_IOT01A)
// Pin definitions for board B-L4S5I_IOT01A
#define GPO_PIN PE4
#define LPD_PIN PE2
#define SDA_PIN PB11
#define SCL_PIN PB10

#define WireNFC MyWire
TwoWire MyWire(SDA_PIN, SCL_PIN);

#else
// Please define the pin and wire instance used for your board
// This works with nodemcu v3.0
#define GPO_PIN 3
#define LPD_PIN 4
#define SDA_PIN 2
#define SCL_PIN 1

#define WireNFC Wire // Default wire instance

#endif

#if !defined(GPO_PIN) || !defined(LPD_PIN)
#error define the pin and wire instance used for your board
#endif

const int BUTTON_PIN = D2;

const int POS_PIN = D7;  //Blue
const int NEG_PIN = D10;  //White ACCTUALLY CALLED D8 on Wemos d1
const int READY_PIN = D5; //Yellow
const int DONE_PIN = D6;  //Green
//Red is the power pin (3.3V)

int misc = 0;
int mode = 0;
bool oldState = HIGH;

String uri_write_appstore;
String uri_write_positive;
String uri_write_negative;
String uri_write_invalid;


//const char uri_write_protocol_web[] = "";  //Use this line to instead of URI_ID_0x04_STRING to change the message to a text message for the app, also update the const char list accordingly
const char uri_write_protocol_web[] = URI_ID_0x04_STRING;


const char uri_write_message_appstore[] = "apps.apple.com/us/app/safe-healthcheck/id1536698575";
const char uri_write_message_positive[] = "kylemsafe.github.io/positive.html";
const char uri_write_message_negative[] = "kylemsafe.github.io/negative.html";
const char uri_write_message_invalid[] = "kylemsafe.github.io/invalid.html";

//Use these constants if a text message should be sent from the NFC to the phone
/*
const char uri_write_message_appstore[] = "https://apps.apple.com/us/app/safe-healthcheck/id1536698575";
const char uri_write_message_positive[] = "positive";
const char uri_write_message_negative[] = "negative";
const char uri_write_message_invalid[] = "invalid";
*/

unsigned long previousMillis = 0;
const long interval = 1000; 

void setup() {
  pinMode(BUTTON_PIN, INPUT);

  pinMode(POS_PIN, OUTPUT);
  pinMode(NEG_PIN, OUTPUT);
  pinMode(READY_PIN, OUTPUT);
  pinMode(DONE_PIN, OUTPUT);

  digitalWrite(POS_PIN, HIGH);
  digitalWrite(NEG_PIN, HIGH);
  digitalWrite(READY_PIN, HIGH);
  digitalWrite(DONE_PIN, HIGH);  

  uri_write_appstore = String(uri_write_protocol_web) + String(uri_write_message_appstore);
  uri_write_positive = String(uri_write_protocol_web) + String(uri_write_message_positive);
  uri_write_negative = String(uri_write_protocol_web) + String(uri_write_message_negative);
  uri_write_invalid = String(uri_write_protocol_web) + String(uri_write_message_invalid);
 
  String uri_read;

  mode = 0;

  // Initialize serial for output.
  SerialPort.begin(115200);
  while (!SerialPort) delay(10);

  SerialPort.println("MODER 1 # = ");
  SerialPort.println(mode);

  // The wire instance used can be omited in case you use default Wire instance
  if(st25dv.begin(GPO_PIN, LPD_PIN, &WireNFC) == 0) {
    SerialPort.println("System Init done!");
  } else {
    SerialPort.println("System Init failed!");
    while(1);
  }

  if(st25dv.writeURI(uri_write_protocol_web, uri_write_message_positive, "")) {
    SerialPort.println("Write failed!");
    while(1);
  }

  delay(100);
  
  if(st25dv.readURI(&uri_read)) {
    SerialPort.println("Read failed!");
    while(1);
  }

  SerialPort.println(uri_read.c_str());

  if(strcmp(uri_read.c_str(), uri_write_positive.c_str()) == 0) {
    SerialPort.println("Successfully written and read!");
  } else {
    SerialPort.println("Read bad string!");
  }
}

void loop() {  
  bool newState = digitalRead(BUTTON_PIN);

  if((newState == LOW) && (oldState == HIGH)) {
    delay(20);
    newState = digitalRead(BUTTON_PIN);
    Serial.println("I HAVE REGISTERED A BUTTON PRESS");

    if(newState == LOW){
      if(++mode > 4) mode = 1;
      switch(mode){
        case 1:
          st25dv.writeURI(uri_write_protocol_web, uri_write_message_appstore, "");
          //st25dv.writeURI("", "", "");
          digitalWrite(POS_PIN, LOW);
          digitalWrite(NEG_PIN, LOW);
          digitalWrite(READY_PIN, LOW);
          digitalWrite(DONE_PIN, LOW);
        break;    
        case 2:
          //st25dv.writeURI(uri_write_protocol_web, uri_write_message_positive, "");
          st25dv.writeURI("", "positive", "");
          digitalWrite(POS_PIN, HIGH);
          digitalWrite(NEG_PIN, LOW);
          digitalWrite(READY_PIN, LOW);
          digitalWrite(DONE_PIN, HIGH);
        break;
        case 3:
          //st25dv.writeURI(uri_write_protocol_web, uri_write_message_negative, "");
          st25dv.writeURI("", "negative", "");
          digitalWrite(POS_PIN, LOW);
          digitalWrite(NEG_PIN, HIGH);
          digitalWrite(READY_PIN, LOW);
          digitalWrite(DONE_PIN, HIGH);
        break;
        case 4:
          //st25dv.writeURI(uri_write_protocol_web, uri_write_message_invalid, "");
          st25dv.writeURI("", "invalid", "");
          digitalWrite(POS_PIN, HIGH);
          digitalWrite(NEG_PIN, HIGH);
          digitalWrite(READY_PIN, HIGH);
          digitalWrite(DONE_PIN, HIGH);
        break;        
      }
      SerialPort.println("MODE # = ");
      SerialPort.println(mode);
    }
  }
 oldState = newState;
} 