#include <EEPROM.h>
#include <Wire.h>
#include <HarwareSerial.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <RTCx.h>
#include "Adafruit_BME680.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"

//EEPROM Addresses
#define EEPROM_SIZE 100
#define EEPROM_ADDR_STATE 0
#define EEPROM_ADDR_CALIBRATION 8
#define EEPROM_ADDR_PACKET_COUNT 16
#define EEPROM_ADDR_MISSION_START 24
#define EEPROM_ADDR_P2 32

#define TELEMETRY_GAP 1000

#define LAUNCH_PAD 1
#define ASCENT 2
#define DESCENT_1 3
#define DESCENT_2 4
#define LANDED 5

//TELEMETRY DEFINITIONS
#define TEAM_ID (2022ASI-005)

//check these pins later
#define XBeeTxPin 12 // XBee's tx
#define XBeeRxPin 13 // XBee's rx 

#define SEALEVELPRESSURE_HPA (1013.25)

uint8_t packet_count = 1;
uint32_t last_transmit;

struct telemetry_data
{
  // time_stamp;
  string time_stamp;
  int packet_count;
  double altitude;
  double pressure;
  double temperature;
  double voltage;
  double latitude;
  double longitude;
  double gnss_altitude;
  int gnss_satellites;
  //acceleromater data
  float acc_x, acc_y, acc_z;
  //gyro spin rate
  float yaw, pitch, roll;
  int fsw_state;  
  double check_sum;  
}

struct payload_data
{
  double humidity;
  double gas;
  double dust;  
  int p2_status;  
}

telemetry_data td;
payload_data pd;
Adafruit_BME680 bme;
HardwareSerial XBee = HardwareSerial(0);
uint8_t P2 = 0; 

int STATE = 1;

int ascending()
{
  // return -1 if altitude decreasing, 1 if altitude increasing, 0 if constant
}

void setup() 
{  
  EEPROM.begin(EEPROM_SIZE);
  //read saved EEPROM vals
  td.fsw_state = EEPROM.get(EEPROM_ADDR_STATE);
  td.packet_count = EEPROM.get(EEPROM_ADDR_PACKET_COUNT);

  last_transmit = millis();
  

  if(!EEPROM.read(EEPROM_ADDR_CALIBRATION)) calibrate_sensors(); //calibrate sensors if not already done
  
  //start all lines of communications
  XBee.begin(9600, SERIAL_8N1, XBeeRxPin, XBeeTxPin);

  //set up the SD, if file doesnt exist then create  
  setupSD();
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

void calibrate_sensors()
{
  /*---------------BME680---------------*/
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

  /*---------------MPU9250---------------*/
  
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

void send_TelemetryData()
{
  if(millis() - last_transmit >= TELEMETRY_GAP)
  {
    // send string by downlink
    String telemetry_String = String("");
    String delimeter = String(",");

    telemetry_string += String(TEAM_ID) + delimeter;
    telemetry_string += String(td.time_stamp - EEPROM.read(EEPROM_ADDR_MISSION_START)) + delimeter;  //edit this (should be time elapsed)
    telemetry_string += String(td.packet_count) + delimeter;
    telemetry_string += String(td.altitude) + delimeter;
    telemetry_string += String(td.pressure) + delimeter;
    telemetry_string += String(td.temperature) + delimeter;
    telemetry_string += String(td.voltage) + delimeter;
    telemetry_string += String(td.time_stamp) + delimeter;
    telemetry_string += String(td.latitude) + delimeter;
    telemetry_string += String(td.longitude) + delimeter;
    telemetry_string += String(td.gnss_altitude) + delimeter;
    telemetry_string += String(td.gnss_satellites) + delimeter;
    telemetry_string += String(td.acc_x) + delimeter;
    telemetry_string += String(td.acc_y) + delimeter;
    telemetry_string += String(td.acc_z) + delimeter;
    telemetry_string += String(td.yaw) + delimeter;
    telemetry_string += String(td.pitch) + delimeter;
    telemetry_string += String(td.roll) + delimeter;
    telemetry_string += String(td.fsw_state) + delimeter;
    telemetry_string += String(pd.humidity) + delimeter;
    telemetry_string += String(pd.gas) + delimeter;
    telemetry_string += String(pd.dust) + delimeter;
    telemetry_string += String(pd.p2_status) + delimeter;
    
    td.check_sum = millis() + packet_count + altitude + pressure + temperature + voltage + time_stamp
                  + latitude + longitude + gnss_altitude + gnss_satellites + acc_x + acc_y + acc_z + yaw + pitch + roll + fsw_state + humidity
                  + gas + dust + p2_status;

    telemetry_string += String(td.check_sum);

    // send string by XBee
    XBee.println(telemetry_string);
    
    // save string to SD card
    SDNewLine(SD, telemetry_string);

    EEPROM.put(EEPROM_ADDR_PACKET_COUNT, td.packet_count++);
  }
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