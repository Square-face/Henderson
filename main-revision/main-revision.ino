#include <EEPROM.h>
#include <QTRSensors.h>
#include "bluetooth.h"
#include "utils.h"

config settings;

void setup()
{
  state = STANDBY;
  initializeBLE();

  Serial.begin(9600);
  Serial.println(" - Setup complete");
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
    
    case RUNNING:
      state = RUNNING;
      stateCommand = NOTHING;
      return;
  }

  // STANDBY logic goes here
}

// Runs whenever the robot has the CALIBRATING state
void calibrating()
{
  switch(stateCommand)
  {
    case STANDBY:
      state = CALIBRATING;
      stateCommand = NOTHING;
      return;
    
    case RUNNING:
      stateCommand = NOTHING;
      return;
  }

  // CALIBRATING logic goes here
  while (true) // Something something calibrating
  {
    /* code */
  }

  changeState(STANDBY);
}

// Runs whenever the robot has the RUNNING state
void running()
{
  switch(stateCommand)
  {
    case CALIBRATING:
      stateCommand = NOTHING;
      return;
    
    case RUNNING:
      state = RUNNING;
      stateCommand = NOTHING;
      return;
  }

  // RUNNING logic goes here
}