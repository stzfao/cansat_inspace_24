#include <EEPROM.h>
#include <Wire.h>
#include<SoftwareSerial.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define LAUNCH_PAD 1
#define ASCENT 2
#define DESCENT_1 3
#define DESCENT_2 4
#define LANDED 5

//TELEMETRY DEFINITIONS
#define TEAM_ID 2022ASI-005

#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 15

//check these pins later
#define XbeeTxPin 12 // xbee's tx
#define XbeeRxPin 13 // xbee's rx 

#define SEALEVELPRESSURE_HPA (1013.25)

struct telemetry_data{
  // time_stamp;
  int packet_count;
  double altitude;
  double pressure;
  double temperature;
  double voltage;
  // gnss_time;
  double latitude;
  double longitude;
  double gnss_altitude;
  int gnss_satellites;
  //acceleromater data
  //gyro spin rate
  int fsw_state;
}

struct payload_data{

}

Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);
SoftwareSerial Xbee = SoftwareSerial(XbeeRxPin,XbeeTxPin);

void setup() {
  // put your setup code here, to run once:

}

void loop(){
  switch(STATE){
    case LAUNCH_PAD:
      runState_LAUNCH_PAD();
      break;
    case ASCENT:
      runState_ASCENT();
      break;
    case DESCENT_1:
      runState_DESCENT_1();
      break;
    case DESCENT_2:
      runState_DESCENT_2();
      break;
    case LANDED:
      runState_LANDED();
      break;
  }
}

void setupTransmission()
{
  zigbee.begin(9600);  
}

void sendTelemetry()
{
  
}

void calibrateSensors()
{
  setup_BME680();
}

void runState_LAUNCH_PAD()
{
  
}