#include <EEPROM.h>
#include <QTRSensors.h>
#include "bluetooth.h"
#include "utils.h"

#define EEPROMDataSize 4

enum
{
  NOTHING,
  STANDBY,
  CALIBRATING,
  RUNNING
};

enum
{
  PkADRESS,
  IkADRESS,
  DkADRESS,
  SpADRESS
};

commandStructure bleReadData;
uint8_t stateCommand;

void setup()
{
  state = STANDBY;
  initializeBLE();
}

void loop()
{
  handleIncoming();

  switch (state)
  {
  case STANDBY:
    standby();
    break;
  
  case CALIBRATING:
    calibrating();
    break;

  case RUNNING:
    running();
    break;

  default:
    break;
  }
}

// Runs whenever the robot has the STANDBY state
void standby()
{
  switch(stateCommand)
  {
    case CALIBRATING:
      state = CALIBRATING;
      stateCommand = NOTHING;
      return;
      break;
    
    case RUNNING:
      state = RUNNING;
      stateCommand = NOTHING;
      return;
      break;
  }

  // STANDBY logic goes here
}

// Runs whenever the robot has the CALIBRATING state
void calibrating()
{
  // CALIBRATING logic goes here
}

// Runs whenever the robot has the RUNNING state
void running()
{
  // RUNNING logic goes here
}

void handleIncoming()
{
  // Fetch the available data;
  bleReadData = readAvailable();
  stateCommand = NOTHING;

  // Check that a command hes actually been recieved
  if (bleReadData.command != 0)
  {
    // First 3 bits (from the left) are the 
    stateCommand = (bleReadData.command & 0b11100000) >> 5;

    // If the correct bits are flipped in the command state, overwrite the PID constants and speed.
    if (bleReadData.command >> 4) EEPROM.write(EEPROMDataSize * PkADRESS, bleReadData.Pk);
    if (bleReadData.command >> 3) EEPROM.write(EEPROMDataSize * IkADRESS, bleReadData.Ik);
    if (bleReadData.command >> 2) EEPROM.write(EEPROMDataSize * DkADRESS, bleReadData.Dk);
    if (bleReadData.command >> 1) EEPROM.write(EEPROMDataSize * SpADRESS, bleReadData.speed);

    // Sends the current status of 
    if (bleReadData.command) sendCurrentStatus();
  }
}