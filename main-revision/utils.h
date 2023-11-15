#ifndef UTILS
#define UTILS


#include <stdlib.h>


#define CPIDSLength sizeof(commandStructure)
#define sensor_count 8


#define changeState(requestedState) stateCommand = requestedState


// State enumerators
enum states
{
  // Filler so that the rest of the states line up
  NOTHING,
  // If the robot is on standby
  STANDBY,
  // If the robot is currently running
  RUNNING,
  // If the robot is currently running with logs
  RUNNING_LOGGED,
  // If the robot is currently calibrating
  CALIBRATING,
  // If the robot is calibrating, manually moved by us
  CALIBRATING_MANUAL
};


// Adress series for EEPROM data
enum EEPROMAdresses
{
  PkADRESS = 0,       // EEPROM Adress for Pk constant
  IkADRESS = 4,       // EEPROM Adress for Ik constant
  DkADRESS = 8,       // EEPROM Adress for Dk constant
  SpADRESS = 12,       // EEPROM Adress for speed constant
  CalMaxADRESS = 13,  // EEPROM Adress for calibration maximum
  CalMinADRESS = 36   // EEPROM Adress for calibration minimum
};


enum motorDirections
{
  LEFT_FORWARD = LOW,
  LEFT_BACKWARD = HIGH,
  RIGHT_FORWARD = LOW,
  RIGHT_BACKWARD = HIGH
};


// hacky optimization for stupid shit
// why use 2's complement when you can use 2 bytes
struct twoByteSignedChar {
  unsigned char num;
  bool sign;
};


// Structure of the status to send
struct statusStructure {
  uint8_t state;
  float Pk;
  float Ik;
  float Dk;
  uint8_t speed;
};


// Structure of the command recieved
struct commandStructure {
  uint8_t command;
  float Pk;
  float Ik;
  float Dk;
  uint8_t speed;
};


// Storage for the config, used when following the line.
struct config
{
  float Pk;
  float Ik;
  float Dk;
  uint8_t speed;
  uint16_t calibrationMax[sensor_count];
  uint16_t calibrationMin[sensor_count];
};


// State variable to be used in main function primarily
// Defined in utils.h
uint8_t state;


// Request to change the state
// Defined in utils.h
uint8_t stateCommand;


// Current settings containing the Pk, Ik, Dk, speed and calibration settings
config settings;


// Calculated speed based on given speed value
double proportionalSpeed;


// Motor 1 left motor direction pin
const unsigned char leftMotorDirection = 2;
// Motor 1 left motors speed pin
const unsigned char leftMotorSpeed = 3;

// Motor 2 right motor speed pin
const unsigned char rightMotorSpeed = 5;
// Motor 2 right motor direction pin
const unsigned char rightMotorDirection = 4;


commandStructure hexStringToBytes(const String s) {
  commandStructure output;

  // Convert hexadecimal string to unsigned long integer
  unsigned long ChexValue = strtoul(s.substring(0, 2).c_str(), NULL, 16);
  unsigned long PhexValue = strtoul(s.substring(2, 10).c_str(), NULL, 16);
  unsigned long IhexValue = strtoul(s.substring(10, 18).c_str(), NULL, 16);
  unsigned long DhexValue = strtoul(s.substring(18, 26).c_str(), NULL, 16);
  unsigned long ShexValue = strtoul(s.substring(26, 28).c_str(), NULL, 16);

  output.command = ChexValue;
  output.Pk = *(float*)&PhexValue;
  output.Ik = *(float*)&IhexValue;
  output.Dk = *(float*)&DhexValue;
  output.speed = ShexValue;

  return output;
  
  // Typecast the unsigned long integer to a float
  /* float PfloatValue = *(float*)&PhexValue;
  float IfloatValue = *(float*)&IhexValue;
  float DfloatValue = *(float*)&DhexValue; */

  /* Serial.println(ChexValue);
  Serial.println(PfloatValue);
  Serial.println(IfloatValue);
  Serial.println(DfloatValue);
  Serial.println(ShexValue); */
}


#endif