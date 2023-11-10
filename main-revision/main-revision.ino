#include <QTRSensors.h>
#include <BLESerial.h>
#include "utils.h"

uint8_t state;

enum
{
  STANDBY,
  CALIBRATING,
  RUNNING
};

void setup()
{
  state = STANDBY;
}

void loop()
{


  switch (state)
  {
  case STANDBY:
    break;
  
  case CALIBRATING:
    break;

  case RUNNING:
    break;

  default:
    break;
  }
}

