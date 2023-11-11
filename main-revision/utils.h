#ifndef UTILS
#define UTILS

#include "sensors.h"
#include <stdlib.h>

#define EEPROMDataSize 4
#define CPIDSLength sizeof(commandStructure)

// State enumerators
enum states
{
  // Filler so that the rest of the states line up
  NOTHING,
  // If the robot is on standby
  STANDBY,
  // If the robot is currently calibrating
  CALIBRATING,
  // If the robot is calibrating, manually moved by us
  CALIBRATINGMANUAL,
  // If the robot is currently running
  RUNNING
};

// Adress series for EEPROM data
enum EEPROMAdresses
{
  NoADRESS,
  PkADRESS,
  IkADRESS,
  DkADRESS,
  SpADRESS,
  CalMaxADRESS,
  CalMinADRESS
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
  u16 calibrationMax;
  u16 calibrationMin;
};
// State variable to be used in main function primarily
// Defined in utils.h
uint8_t state;


// Request to change the state
// Defined in utils.h
uint8_t stateCommand;


// Returns true if the number is negative, otherwise it returns false
bool isNegative(short n) 
{
    return (n >> (sizeof(n) * 8)) - 1;
}

// Converts a short into a twoByteSignedChar, a structure containing a number and a bool depending on the sign
twoByteSignedChar convertToReligion(short n)
{
  short num = abs(n);
  bool sign = isNegative(n);

  if (num > 0xFF)
  {
    return {0xFF, sign};
  }

  return {(unsigned char) num, sign};
}

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

void changeState(states requestedState)
{
  stateCommand = requestedState;
}

#endif