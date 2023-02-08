#include <EEPROM.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include "MPU9250.h"
#include <Adafruit_Sensor.h>
#include <RTCx.h>
#include <ESP32Servo.h>
#include "Adafruit_BME680.h"
#include <MPU9250_asukiaaa.h>
#include <TinyGPS++.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

// EEPROM Addresses
#define EEPROM_SIZE 100
#define EEPROM_ADDR_STATE 0
#define EEPROM_ADDR_CALIBRATION 8
#define EEPROM_ADDR_PACKET_COUNT 16
#define EEPROM_ADDR_MISSION_START 24
#define EEPROM_ADDR_P2 32

// I2C Addresses
#define I2C_MPU_ADDR 0x68

// Math constants
#define TELEMETRY_GAP 1000
#define RAD_TO_DEG 57.295779513082320876798154814105

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
#define GPSTxPin 17 // XBee's tx
#define GPSRxPin 16 // XBee's rx 
#define BUZZER 35

#define SEALEVELPRESSURE_HPA (1013.25)

uint8_t packet_count = 1;
uint32_t last_transmit;
uint8_t P2 = 0, play_buzzer = 1;

// telemetry constants
string time_stamp;
int packet_count;
double altitude;
double pressure;
double temperature;
double voltage;
double latitude;
double longitude;
double gnss_altitude;
uint32_t gnss_satellites;
//acceleromater data
float acc_x, acc_y, acc_z;
//gyro spin rate
float gyro_x, gyro_y, gyro_z;
int fsw_state;  
double check_sum;  
double humidity;
double gas;
double dust;  
int p2_status;  

Servo lid;
TinyGPSPlus gnss;
MPU9250 mpu(i2c0, I2C_MPU_ADDR);
Adafruit_BME680 bme;
HardwareSerial XBee = HardwareSerial(0);
HardwareSerial l89 = HardwareSerial(2);

/*
@description
@params: void
@return: return -1 if altitude decreasing, 1 if altitude increasing, 0 if constant (uint8_t)

*/
int ascending()
{
  // 
}

void setup() 
{  
  //EEPROM related work
  EEPROM.begin(EEPROM_SIZE);
  fsw_state = EEPROM.get(EEPROM_ADDR_STATE);
  packet_count = EEPROM.get(EEPROM_ADDR_PACKET_COUNT);

  last_transmit = millis();
  
  
  if(!EEPROM.read(EEPROM_ADDR_CALIBRATION)) calibrate_sensors(); //calibrate sensors if not already done
  
  //start all lines of communications
  XBee.begin(9600, SERIAL_8N1, XBeeRxPin, XBeeTxPin);
  l89.begin(9600, SERIAL_8N1, GPSRxPin, GPSTxPin);
  pinMode(BUZZER, OUTPUT);

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
  /* --------------- BME680 --------------- */
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

  /* --------------- MPU9250 --------------- */
  
  if(mpu.begin() < 0)
  {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while (1);
  }

  // setting the accelerometer full scale range to +/-8G
  mpu.setAccelRange(MPU9250::ACCEL_RANGE_2G);
  // setting the gyroscope full scale range to +/-500 deg/s
  mpu.setGyroRange(MPU9250::GYRO_RANGE_250DPS);
  // setting DLPF bandwidth to 20 Hz
  mpu.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_41HZ);
  // setting SRD to 19 for a 50 Hz update rate
  // IMU.setSrd(19);
  mpu.calibrateMag();
}

void loop(){
  switch(fsw_state){
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
    telemetry_string += String(time_stamp - EEPROM.read(EEPROM_ADDR_MISSION_START)) + delimeter;  //edit this (should be time elapsed)
    telemetry_string += String(packet_count) + delimeter;
    telemetry_string += String(altitude) + delimeter;
    telemetry_string += String(pressure) + delimeter;
    telemetry_string += String(temperature) + delimeter;
    telemetry_string += String(voltage) + delimeter;
    telemetry_string += String(time_stamp) + delimeter;
    telemetry_string += String(latitude) + delimeter;
    telemetry_string += String(longitude) + delimeter;
    telemetry_string += String(gnss_altitude) + delimeter;
    telemetry_string += String(gnss_satellites) + delimeter;
    telemetry_string += String(acc_x) + delimeter;
    telemetry_string += String(acc_y) + delimeter;
    telemetry_string += String(acc_z) + delimeter;
    telemetry_string += String(gyro_x) + delimeter;
    telemetry_string += String(gyro_y) + delimeter;
    telemetry_string += String(gyro_z) + delimeter;
    telemetry_string += String(fsw_state) + delimeter;
    telemetry_string += String(humidity) + delimeter;
    telemetry_string += String(gas) + delimeter;
    telemetry_string += String(dust) + delimeter;
    telemetry_string += String(p2_status) + delimeter;
    
    check_sum = millis() + packet_count + altitude + pressure + temperature + voltage + time_stamp
                  + latitude + longitude + gnss_altitude + gnss_satellites + acc_x + acc_y + acc_z + gyro_x + gyro_y + gyro_z + fsw_state + humidity
                  + gas + dust + p2_status;

    telemetry_string += String(check_sum);

    // send string by XBee
    XBee.println(telemetry_string);
    
    // save string to SD card
    SDNewLine(SD, telemetry_string);

    EEPROM.put(EEPROM_ADDR_PACKET_COUNT, packet_count++);
  }
}

void get_TelemetryCommands()
{
  
}

void get_L89HA_Data()
{
  if(l89.available() > 0 && gnss.encode(l89.read()) && gnss.location.isValid())
  {
    latitude = gnss.location.lat() / 10000; //in 0.0001 degrees
    longitude = gnss.location.lng() / 10000;
    gnss_altitude = gnss.altitude.meters() / 10; // gnss altitude in 0.1m
    gnss_satellites = gnss.satellites.value();
    time_stamp = gnss.time.hour * 3600 + gnss.time.minute * 60 + gnss.time.second();  // gnss time in seconds
  }
}

void get_MPU9250_Data()
{
  mpu.readSensor();

  acc_x = mpu.getAccelX_mss();
  acc_y = mpu.getAccelY_mss();
  acc_z = mpu.getAccelZ_mss();

  gyro_x = getGyroX_rads();
  gyro_y = getGyroY_rads();
  gyro_z = getGyroZ_rads();
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

  temperature = (float)bme.temperature / 10;  // 0.1 C 
  pressure = (float)bme.pressure;
  altitude = (float)bme.readAltitude(SEALEVELPRESSURE_HPA) / 10; // 0.1 m
  humidity = (float)bme.humidity;
  gas = bme.gas_resistance;
}

void audioBeacon()
{
  if(play_buzzer) digitalWrite(BUZZER, HIGH);
  else digitalWrite(BUZZER, LOW);
}

void deployParachute2()
{

}

void runState_LAUNCH_PAD()
{
  EEPROM.write(EEPROM_ADDR_STATE, fsw_state);
  if(ascending() > 0) STATE = ASCENT;
}

void runState_ASCENT()
{ 
  EEPROM.write(EEPROM_ADDR_STATE, fsw_state);  
  if(ascending() < 0){
    STATE =  (altitude > 500) ? DESCENT_1 : DESCENT_2;
  } 
  send_TelemetryData();
}

void runState_DESCENT_1()
{
  EEPROM.write(EEPROM_ADDR_STATE, fsw_state);  
  if(ascending() < 0 && altitude <= 500) STATE = DESCENT_2;  
  send_TelemetryData();
}

void runState_DESCENT_2()
{
  EEPROM.write(EEPROM_ADDR_STATE, fsw_state);  
  if(ascending() == 0) STATE = LANDED;  
  send_TelemetryData();
}

void runState_LANDED()
{
  EEPROM.write(EEPROM_ADDR_STATE, fsw_state);  
  send_TelemetryData();
  audioBeacon();
}