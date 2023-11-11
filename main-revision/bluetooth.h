#ifndef BLUETOOTH
#define BLUETOOTH

#include "BLESerial.h"
#include <SoftwareSerial.h>
#include "utils.h"


// Pins for bluetooth serial communication
#define RX 11
#define TX 10


// Initialize serial connection on the correct pins
BLESerial ble(RX, TX);
uint8_t *temp = (uint8_t *)malloc(sizeof(commandStructure));
char *serialTemp = (char *)malloc(30);


// Read data gotten from the BLE
commandStructure bleReadData;


void initializeBLE()
{
  ble.begin();
  ble.setName("Henderson");
  ble.setBaud(BAUD_9600);
}


// If there is data available to read for the BLE, its will return the available data.
commandStructure readAvailable()
{
  commandStructure output;

  if (ble.available() != CPIDSLength)
  {
    Serial.println("Ignored incoming");
    while (ble.available() > 0) ble.read();

    output.command = 0;

    return output;
  }

  for (int i = 0; ble.available() > 0; i++)
  {
    temp[i] = ble.read();
    Serial.println(temp[i], 16);
  }
  
  output.command = temp[0];

  // Converts the 4 correct bytes to a float, storing it in the output Pk
  uint32_t PInt = (uint32_t)temp[1] << 24  | 
                  (uint32_t)temp[2] << 16  | 
                  (uint32_t)temp[3] << 8   | 
                  (uint32_t)temp[4];
  // Serial.println(PInt, 16);
  output.Pk = *(float*)&PInt;

  // Converts the 4 correct bytes to a float, storing it in the output Ik
  uint32_t IInt = (uint32_t)temp[5] << 24  | 
                  (uint32_t)temp[6] << 16  | 
                  (uint32_t)temp[7] << 8   | 
                  (uint32_t)temp[8];
  // Serial.println(IInt, 16);
  output.Ik = *(float*)&IInt;

  // Converts the 4 correct bytes to a float, storing it in the output Dk
  uint32_t DInt = (uint32_t)temp[9] << 24  | 
                  (uint32_t)temp[10] << 16 | 
                  (uint32_t)temp[11] << 8  | 
                  (uint32_t)temp[12];
  // Serial.println(DInt, 16);
  output.Dk = *(float*)&DInt;

  output.speed = temp[13];

  return output;
}


commandStructure readAvailableSerial()
{
  Serial.readBytes(serialTemp, 30);
  Serial.write(serialTemp);

  Serial.print("Received Bytes: ");
  for (int i = 0; i < 30; i++) {
    Serial.print(serialTemp[i]);
  }
  Serial.println(); // Move to the next line

  return hexStringToBytes(serialTemp);

  for (int i = 0; i < sizeof(commandStructure); i++) 
  {
    Serial.print("temp " + String(i) + ": "); 
    Serial.print(temp[i]);
    Serial.println();
  }

  float tempFloat;
  for (int i = 0; i < sizeof(float); i++)
  {
    Serial.println(i);
    *(&tempFloat + i) = temp[1 + i];
    Serial.println(tempFloat);
  }
  Serial.println();
  Serial.println(tempFloat);

  commandStructure output;
  output.command = *temp;
  Serial.println(*(float*)(temp+1));
  output.Pk = *(float*)(temp+1);
  output.Ik = *(float*)(temp+5);
  output.Dk = *(float*)(temp+9);
  output.speed = *(temp+13);

  return output;
}


// Sends the current status to the controller through the BLE
void sendCurrentStatus()
{
  config temp;
  EEPROM.get(PkADRESS * EEPROMDataSize, temp.Pk);
  EEPROM.get(IkADRESS * EEPROMDataSize, temp.Ik);
  EEPROM.get(DkADRESS * EEPROMDataSize, temp.Dk);
  EEPROM.get(SpADRESS * EEPROMDataSize, temp.speed);

  Serial.println(EEPROM.read(PkADRESS));
  Serial.print("State: " + String(state, 8));
  Serial.print(" Pk: " + String(temp.Pk, 8));
  Serial.print(" Ik: " + String(temp.Ik, 8));
  Serial.print(" Dk: " + String(temp.Dk, 8));
  Serial.println(" speed: " + String(temp.speed));

  ble.write(state);
  Serial.print(state, 16);
  for (int i = 0; i < sizeof(temp) - sizeof(u16) * 2; i++) 
  {
    ble.write(*((uint8_t *)((&temp)+i)));
    Serial.print(*((uint8_t *)((&temp)+i)), 16);
  }
  Serial.println();
}


void handleIncoming()
{
  // Fetch the available data *UNCOMMEND THIS*
  if (ble.available() > 0)
  {
    Serial.println("Something was recieved");
    delay(100);
    bleReadData = readAvailable();
  }

  // Fetch serial data instead *COMMENT THIS OUT*
  if (Serial.available() > 0)
  {
    Serial.print("Serial is available: ");
    delay(100);
    Serial.println(Serial.available());
    bleReadData = readAvailableSerial();
  }

  // Resets the state command
  stateCommand = NOTHING;

  // Check that a command has actually been recieved
  if (bleReadData.command != 0)
  {
    // First 3 bits (from the left) represent the state
    stateCommand = (bleReadData.command & 0b11100000) >> 5;

    // If the correct bits are flipped in the command state, overwrite the PID constants and speed.
    if ((bleReadData.command & 0b00010000) >> 4) 
    {
      Serial.println("Pk size: " + String(sizeof(bleReadData.Pk)));
      Serial.println("Before Pk: " + String(bleReadData.Pk)); 
      EEPROM.put(EEPROMDataSize * PkADRESS, bleReadData.Pk); 
      EEPROM.get(EEPROMDataSize * PkADRESS, bleReadData.Pk);
      Serial.println("After Pk: " + String(bleReadData.Pk)); 
    }
    if ((bleReadData.command & 0b00001000) >> 3) 
    {
      EEPROM.put(EEPROMDataSize * IkADRESS, bleReadData.Ik);
    }
    if ((bleReadData.command & 0b00000100) >> 2) 
    {
      EEPROM.put(EEPROMDataSize * DkADRESS, bleReadData.Dk);
    }
    if ((bleReadData.command & 0b00000010) >> 1) 
    {
      EEPROM.put(EEPROMDataSize * SpADRESS, bleReadData.speed);
    }

    Serial.println("Done with changing the values");
    // Sends the current status of the Henderson
    if (bleReadData.command & 0b00000001) 
    {
      Serial.println("Sending current status");
      sendCurrentStatus();
    }

    // Resets the bleReadData
    Serial.println("Resetting BLE data");
    bleReadData.command = 0;
  }

}

#endif