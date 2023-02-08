#include <EEPROM.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include "MPU9250.h"
#include <Adafruit_Sensor.h>
#include <ESP32Servo.h>
#include "Adafruit_BME680.h"
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
#define TELEMETRY_GAP 800
#define GYRO_THRESHOLD 1
#define GYRO_DIVISION_CONSTANT 10
#define RAD_TO_DEG 57.295779513082320876798154814105

#define LAUNCH_PAD 1
#define ASCENT 2
#define DESCENT_1 3
#define DESCENT_2 4
#define LANDED 5

//TELEMETRY DEFINITIONS
#define TEAM_ID "2022ASI-005"

//check these pins later
#define XBeeTxPin 12  // XBee's tx
#define XBeeRxPin 13  // XBee's rx
#define GPSTxPin 17   // XBee's tx
#define GPSRxPin 16   // XBee's rx
#define BUZZER 35
#define DUST_ANALOG 5    // Connect dust sensor analog measure pin to Arduino A0 pin
#define DUST_DIGITAL 34  // For Dust Sensor's PWM input from the uc
#define SERVO_PIN 4
#define FLYWHEEL_PIN 14
#define VOLTAGE_DIVIDER 15

#define SEALEVELPRESSURE_HPA (1013.25)

uint32_t last_transmit;
float last_altitude;
uint8_t command = 0, parachute_servo_position = 0, flywheel_servo_position = 90;  //any commands rcv from the GCS.
bool play_buzzer = true, start_telemetry = false;
/* 1: Calibrate Sensors
2: Start Telemetry
3: Start/Stop audio beacon */


// telemetry constants
// int time_elapsed; ---> this is simply millis()
int packet_count = 1;
double altitude;
double pressure;
double temperature;
double voltage;
double latitude;
double longitude;
int time_stamp;
double gnss_altitude;
uint32_t gnss_satellites;
float acc_x, acc_y, acc_z;
float gyro_x, gyro_y, gyro_z;
int fsw_state;
double velocity;
double check_sum;
double humidity;
double gas;
double dust_density;
int p2_status = 0;

Servo parachute_lid;
Servo flywheel_motor;
MPU9250 mpu(i2c0, I2C_MPU_ADDR);
Adafruit_BME680 bme;
HardwareSerial XBee = HardwareSerial(0);
TinyGPSPlus gnss;
HardwareSerial l89 = HardwareSerial(2);

/*
@description:
@params: void
@return: return velocity (double) ====== velocity > 0 : ascending, velocity <>> 0 : descending
*/
int get_velocity() {
  //distance aka altitude:
  velocity = (altitude - last_altitude) / (millis() - last_transmit);
  last_altitude = altitude;
}

void setup() {
  //EEPROM related work
  EEPROM.begin(EEPROM_SIZE);
  fsw_state = EEPROM.read(EEPROM_ADDR_STATE);
  packet_count = EEPROM.read(EEPROM_ADDR_PACKET_COUNT);

  last_transmit = millis();

  Serial.begin(9600);
  if(!EEPROM.read(EEPROM_ADDR_CALIBRATION)) calibrate_sensors();  //calibrate sensors if not already done

  //start all lines of communications
  XBee.begin(9600, SERIAL_8N1, XBeeRxPin, XBeeTxPin);
  l89.begin(9600, SERIAL_8N1, GPSRxPin, GPSTxPin);
  pinMode(BUZZER, OUTPUT);
  dustSensor.begin();

  /* --------------- Parachute Servo --------------- */
  ESP32PWM::allocateTimer(0);
  parachute_lid.setPeriodHertz(50);            // standard 50 hz servo
  parachute_lid.attach(SERVO_PIN, 500, 2400);  // attaches the servo on pin 18 to the servo object
  parachute_lid.write(parachute_servo_position);

  /* --------------- Flywheel Servo --------------- */
  ESP32PWM::allocateTimer(1);
  flywheel_motor.setPeriodHertz(50);               // standard 50 hz servo
  flywheel_motor.attach(FLYWHEEL_PIN, 500, 2400);  // attaches the servo on pin 18 to the servo object
  flywheel.write(flywheel_servo_position);         //centered around 90 degrees

  /* --------------- SD Card --------------- */
  setupSD();
}

void setupSD() {
  Serial.begin(115200);
  if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }

  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }
}

void calibrate_sensors() {
  
  /* --------------- BME680 --------------- */
  while (!bme.begin()) {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);  // 320*C for 150 ms


  /* --------------- MPU9250 --------------- */
  if (mpu.begin() < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    while (1);
  }

  // setting the accelerometer full scale range to +/-8G
  mpu.setAccelRange(MPU9250::ACCEL_RANGE_16G);
  // setting the gyroscope full scale range to +/-500 deg/s
  mpu.setGyroRange(MPU9250::GYRO_RANGE_2000DPS);
  // setting DLPF bandwidth to 20 Hz
  mpu.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_41HZ);
  // setting SRD to 19 for a 50 Hz update rate
  mpu.calibrateMag();
}

void loop() {
  switch (fsw_state) {
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

void SDNewLine(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void send_TelemetryData() {

  if (!start_telemetry) return;  //if we are not to start telemetry yet:

  if (millis() - last_transmit >= TELEMETRY_GAP) {

    // update variable data
    get_L89HA_Data();
    get_BME680_Data();
    get_MPU9250_Data();
    get_DustSensor_Data();
    get_velocity();

    // send string by downlink
    String telemetry_string = String("");
    String delimeter = String(",");
    telemetry_string += String(TEAM_ID) + delimeter;
    telemetry_string += String(millis() / 1000) + delimeter;  //edit this (should be time elapsed)
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
    telemetry_string += String(velocity) + delimeter;  //m/s
    telemetry_string += String(humidity) + delimeter;
    telemetry_string += String(gas) + delimeter;
    telemetry_string += String(dust) + delimeter;
    telemetry_string += String(p2_status) + delimeter;

    check_sum = millis() + packet_count + altitude + pressure + temperature + voltage
                + latitude + longitude + gnss_altitude + gnss_satellites + velocity
                + acc_x + acc_y + acc_z + gyro_x + gyro_y + gyro_z + fsw_state + humidity
                + gas + dust_density + p2_status;

    telemetry_string += String(check_sum);

    // send string by XBee
    XBee.println(telemetry_string);

    // save string to SD card
    char temp[telemetry_string.length() + 1];
    telemetry_string.toCharArray(temp, telemetry_string.length() + 1);
    SDNewLine(SD, "/data.csv", temp);

    // Serial.println(telemetry_string);          ============ DEBUG

    EEPROM.write(EEPROM_ADDR_PACKET_COUNT, packet_count++);
    EEPROM.commit();
  }
}

void get_TelemetryCommands() {
  if (XBee.available() > 0) {
    command = XBee.read();
    //if the data is 0, turn off the LED
    switch (command) {
      case 1:
        calibrate_sensors();
        break;
      case 1:  //start telemetry
        start_telemetry = true;
        break;
      case 3:
        play_buzzer = ~play_buzzer;
        audio_beacon();
        break;
    }
    command = 0;
  }
}

void get_L89HA_Data() {
  while (l89.available() <= 0);

  if (l89.available() > 0 && gnss.encode(l89.read()) && gnss.location.isValid()) {
    latitude = gnss.location.lat() / 10000;  //in 0.0001 degrees
    longitude = gnss.location.lng() / 10000;
    gnss_altitude = gnss.altitude.meters() / 10;  // gnss altitude in 0.1m
    gnss_satellites = gnss.satellites.value();
    time_stamp = gnss.time.hour() * 3600 + gnss.time.minute() * 60 + gnss.time.second();  // gnss time in seconds
  }
}

void get_MPU9250_Data() {
  mpu.readSensor();

  acc_x = mpu.getAccelX_mss();
  acc_y = mpu.getAccelY_mss();
  acc_z = mpu.getAccelZ_mss();

  gyro_x = mpu.getGyroX_rads();
  gyro_y = mpu.getGyroY_rads();
  gyro_z = mpu.getGyroZ_rads();
}

void get_DustSensor_Data() {
  dust_density = dustSensor.read();
}

void get_BME680_Data() {
  //BME680 data
  unsigned long endTime = bme.beginReading();
  if (endTime == 0) {
    Serial.println(F("Failed to begin reading :("));
  }

  while (!bme.endReading()) {
    Serial.println(F("Wait till we complete reading :("));
    // return;
  }

  temperature = (float)bme.temperature / 10;  // 0.1 C
  pressure = (float)bme.pressure;
  altitude = (float)bme.readAltitude(SEALEVELPRESSURE_HPA) / 100.0;  // 0.1 m
  humidity = (float)bme.humidity;
  gas = bme.gas_resistance;
}

void get_voltage() {
  float R2 = 4.6;
  float R1 = 16.4;

  float value = analogRead(VOLTAGE_DIVIDER);
  float vout = (value * 3.3) / 1024.0;
  vout /= (R2 / (R1 + R2));  //?
  voltage = vout / 100.0;
}

void audio_beacon() {
  if (play_buzzer) digitalWrite(BUZZER, HIGH);
  else digitalWrite(BUZZER, LOW);
}

void deploy_parachute2() {
  if (!p2_status) {
    for (; parachute_servo_position <= 180; parachute_servo_position += 1) {  // goes from 0 degrees to 180 degrees in steps of 1 degree
      myservo.write(parachute_servo_position);                                // tell servo to go to position in variable 'pos'
      delay(5);                                                               // waits 5ms for the servo to reach the position
    }
    p2_status++;
  }
}

void flywheel_mechanism() {
  //the flywheel motor will be centered at 90 degrees initially.
  do {
    /* if gyro is negative, we move in the positive direction. aka add/subtract from curr pos */
    int rotn = gyro_z / GYRO_DIVISION_CONSTANT;
    flywheel_servo_position += rotn;
    if (flywheel_servo_position > 180) flywheel_servo_position = 180;
    else if (flywheel_servo_position < 0) flywheel_servo_position = 0;
    flywheel_motor.write(flywheel_servo_position);
    gyro_z = mpu.getGyroZ_rads();  //updated gyro_z
  } while (gyro_z > GYRO_THRESHOLD);
}

void runState_LAUNCH_PAD() {
  EEPROM.write(EEPROM_ADDR_STATE, fsw_state);
  if (get_velocity() > 2) fsw_state = ASCENT;
}

void runState_ASCENT() {
  EEPROM.write(EEPROM_ADDR_STATE, fsw_state);
  if (get_velocity() < 0) {
    fsw_state = (altitude > 500) ? DESCENT_1 : DESCENT_2;
  }
  send_TelemetryData();
}

void runState_DESCENT_1() {
  EEPROM.write(EEPROM_ADDR_STATE, fsw_state);
  flywheel_mechanism();
  if (get_velocity() < 0 && altitude <= 500) fsw_state = DESCENT_2;
  send_TelemetryData();
}

void runState_DESCENT_2() {
  EEPROM.write(EEPROM_ADDR_STATE, fsw_state);
  if (!p2_status) deploy_parachute2();
  if (get_velocity() > -0.5) fsw_state = LANDED;
  send_TelemetryData();
}

void runState_LANDED() {
  EEPROM.write(EEPROM_ADDR_STATE, fsw_state);
  send_TelemetryData();
  audio_beacon();
}