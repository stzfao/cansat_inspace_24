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
#define TEAM_ID (2022ASI-005)

#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 15

//check these pins later
#define XbeeTxPin 12 // xbee's tx
#define XbeeRxPin 13 // xbee's rx 

#define SEALEVELPRESSURE_HPA (1013.25)

struct telemetry_data
{
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
  double check_sum;  
}

struct payload_data
{
  double humidity;
  double VOC_sensor;

  double check_sum;
}

struct gyro_data
{
  
}

Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);
SoftwareSerial Xbee = SoftwareSerial(XbeeRxPin, XbeeTxPin);

void setup() {

  //BME680
  {
    while(!bme.begin()) {
      Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    }

    // Set up oversampling and filter initialization
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms
  }

  //XBee
  Xbee.begin(9600);


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


void send_TelemetryData()
{
  
}

void get_MPU9255_Data()
{

}

void get_BME680_Data()
{
  //BME680 data
  unsigned long endTime = bme.beginReading();
  if (endTime == 0) 
  {
    Serial.println(F("Failed to begin reading :("));
  }

  while(!bme.endReading()) {
    Serial.println(F("Wait till we complete reading :("));
    return;
  }

  telemetry_data.temperature = bme.temperature;
  telemetry_data.pressure = bme.pressure;
  telemetry_data.altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  payload_data.humidity = bme.humidity;
  payload_data.VOC_sensor = bme.gas_resistance;
}

void runState_LAUNCH_PAD()
{
  
}

void runState_ASCENT()
{
  
}

void runState_DESCENT_1()
{
  
}

void runState_DESCENT_2()
{
  
}

void runState_LANDED()
{
  
}