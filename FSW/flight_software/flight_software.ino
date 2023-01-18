#include <EEPROM.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define EEPROM_SIZE 1

#define LAUNCH_PAD 1
#define ASCENT 2
#define DESCENT_1 3
#define DESCENT_2 4
#define LANDED 5

//TELEMETRY DEFINITIONS
#define TEAM_ID (2022ASI-005)

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

Adafruit_BME680 bme;
SoftwareSerial Xbee = SoftwareSerial(XbeeRxPin, XbeeTxPin);
uint8_t P1 = 0, P2 = 0; 

int STATE = 1;

int ascending()
{
  // return -1 if altitude decreasing, 1 if altitude increasing, 0 if constant
}

void setup() 
{
  // check last saved state and update as needed
  EEPROM.begin(EEPROM_SIZE);
  STATE = EEPROM.read(0);
  // may need this saved state in the future

  setupSD();
  //check last saved state
  calibrateSensors();
  
  //XBee comms start 
  Xbee.begin(9600);
}

void setupSD()
{
  Serial.begin(115200);
  if(!SD.begin()){
    Serial.println("Card Mount Failed");
    return;
  }

  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }
}

void calibrateSensors()
{
  //BME680
  {
    while(!bme.begin()) 
    {
      Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    }

    // Set up oversampling and filter initialization
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms
  }
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

// append to CSV file
// appendFile(SD, "/final.csv", "2022ASI-005,0022,0022,2000,101.3,27.3,3.5,030742.00,02232.73830,11404.58520,32908.09,12,10,2145,1,3.4\n");

void SDNewLine(fs::FS &fs, const char * path, const char * message)
{
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)){
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void saveToEEPROM()
{
  
}

void send_TelemetryData()
{

  // create telemetry string
  // telemetry_string = 
  // send string by downlink
  // save string to SD card
  SDNewLine(SD, telemetry_string);
  
}

void get_TelemetryCommands()
{
  
}

void get_MPU9250_Data()
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

void audioBeacon()
{
  
}

void runState_LAUNCH_PAD()
{
  EEPROM.write(0, STATE);
  if(ascending() > 0) STATE = ASCENT;
}

void runState_ASCENT()
{ 
  EEPROM.write(0, STATE);  
  if(ascending() < 0){
    STATE =  (telemetry_data.altitude > 500) ? DESCENT_1 : DESCENT_2;
  } 
  send_TelemetryData();
}

void runState_DESCENT_1()
{
  EEPROM.write(0, STATE);  
  if(ascending() < 0 && telemetry_data.altitude <= 500) STATE = DESCENT_2;  
  send_TelemetryData();
}

void runState_DESCENT_2()
{
  EEPROM.write(0, STATE);  
  if(ascending() == 0) STATE = LANDED;  
  send_TelemetryData();
}

void runState_LANDED()
{
  EEPROM.write(0, STATE);  
  send_TelemetryData();
  audioBeacon();
}