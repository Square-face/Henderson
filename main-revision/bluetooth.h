#ifndef BLUETOOTH
#define BLUETOOTH

#include <stdlib.h>
#include "BLESerial.h"
#include <SoftwareSerial.h>
#include "utils.h"

// Pins for bluetooth serial communication
#define RX 7
#define TX 8

// Initialize serial connection on the correct pins
BLESerial ble(RX, TX);
uint8_t *temp = (uint8_t *)malloc(sizeof(commandStructure));

void initializeBLE()
{
  ble.begin();
  ble.setName("Henderson");
  ble.setBaud(BAUD_9600);
}

// If there is data available to read for the BLE, its will return the available data.
// If several blocks were sent it will return one block at a time.
commandStructure readAvailable()
{
  if (ble.available())
  {
    for (int i = 0; ble.available(); i++)
    {
      temp[i] = ble.read(); 
    }
    return *((commandStructure*)temp); // we do a little trolling
  }
  return {0, 0, 0, 0, 0};
}

// Sends the current status to the controller through the BLE
void sendCurrentStatus()
{
  statusStructure temp;
  temp.state = state;
  temp.Pk = EEPROM.read(PkADRESS);
  temp.Ik = EEPROM.read(IkADRESS);
  temp.Dk = EEPROM.read(DkADRESS);
  temp.speed = EEPROM.read(SpADRESS);

  for (int i = 0; i < sizeof(temp); i++) ble.write(*((uint8_t *)((&temp)+i)));
}

#endif