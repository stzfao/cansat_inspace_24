#include <EEPROM.h>

// Memory map
// Addresses -> Value





#define EEPROM_SIZE 100
#define EEPROM_ADDR_STATE 0
#define EEPROM_ADDR_CALIBRATION 8
#define EEPROM_ADDR_PACKET_COUNT 16
#define EEPROM_ADDR_MISSION_START 24
#define EEPROM_ADDR_P2 32

float zero = 0.0;
int prevState = 255;
float num;
int i;

void setup(){
  Serial.begin(9600);
  Serial.println("Initializing EEPROM memory...");
  Serial.println("Writing values...");

  

  // writing eemprom_size
  EEPROM.write(EEPROM_SIZE,100);

  // writing address_state
  EEPROM.write(EEPROM_ADDR_STATE,0);
  // writing calibration
  EEPROM.write(EEPROM_ADDR_CALIBRATION, 8);
  // writing packetCount
  storeInt(EEPROM_ADDR_PACKET_COUNT, 16);

  // writing mission start
  storeInt(EEPROM_ADDR_MISSION_START, 24);

  //writing parachute deployment
  storeInt(EEPROM_ADDR_P2, 32);

  

  Serial.println("OK!");

}

void loop(){
  Serial.println("Checking values written...");

  i = EEPROM.read(EEPROM_ADDR_STATE);
  if(i==0) Serial.println("EEPROM_ADDR_STATE is OK!");
  else Serial.println("Error writing EEPROM_ADDR_STATE");

  i = EEPROM.read(EEPROM_ADDR_CALIBRATION);
  if(i==0) Serial.println("areSensorsCalibrated is OK!");
  else Serial.println("Error writing areSensorsCalibrated!");

  i = readInt(EEPROM_ADDR_PACKET_COUNT);
  if(i==0) Serial.println("Packet Count is OK!");
  else Serial.println("Error writing packet count!");

  i = readInt(EEPROM_ADDR_MISSION_START);
  if(i==0) Serial.println("Mission start is OK!");
  else Serial.println("Error writing mission start!");

  i=readInt(EEPROM_ADDR_P2);
  if(i==0) Serial.println("Parachute hasn't been deployed");
  else Serial.println("Parachute deployed !");
  
  Serial.println("Checks finised! Do not forget to upload the flight software!");

  delay(5000);
}

void storeInt(int addr, uint32_t num){
  // store num in EEPROM starting in address addr
  // EEPROM cells are 1 byte, integers are 4 bytes

  uint32_t byte1,byte2,byte3,byte4;

  byte1 = num & 0xFF;
  byte2 = (num>>8) & 0xFF;
  byte3 = (num>>16) & 0xFF;
  byte4 = (num>>24) & 0xFF;

  // little endian
  EEPROM.write(addr,byte1);
  EEPROM.write(addr+1,byte2);
  EEPROM.write(addr+2,byte3);
  EEPROM.write(addr+3,byte4);
}

uint32_t readInt(int addr){
  // read an integer stored in mem[addr..addr+3] in EEPROM
  // using little endian
  uint32_t result;
  uint32_t byte1,byte2,byte3,byte4;

  byte1 = (uint32_t) EEPROM.read(addr);
  byte2 = (uint32_t) EEPROM.read(addr+1);
  byte3 = (uint32_t) EEPROM.read(addr+2);
  byte4 = (uint32_t) EEPROM.read(addr+3);

  result = 0;
  result = result | byte1;
  result = result | ( (byte2<<8) & 0xFF00 );
  result = result | ( (byte3<<16) & 0xFF0000 );
  result = result | ( (byte4<<24) & 0xFF000000 );

  return result;
}

void writeFloat(int addr, float &num){
  const byte* p = (const byte*)(const void*) &num;

  for(unsigned int i=0;i<sizeof(num);i++){
    EEPROM.write(addr++,*p++);
  }
}


void readFloat(int addr, float &num){
  byte* p = (byte*)(void*) &num;
  for(unsigned int i=0;i<sizeof(num);i++){
    *p++ = EEPROM.read(addr++);
  }
}
