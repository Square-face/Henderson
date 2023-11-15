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


template <typename T>
void ble_write(T data)
{
  byte* pData = reinterpret_cast<byte*>(&data);

  for (char i = 0; i < sizeof(data); i++) {
      ble.write(pData[i]);
  }
}


template <typename T>
void srl_write(T data)
{
  byte* pData = reinterpret_cast<byte*>(&data);

  for (char i = 0; i < sizeof(data); i++) {
    String hexString = String(pData[i], 16);
    for (char j = 0; j < (2 - hexString.length()); j++)
    {
      Serial.print("0");
    }
    Serial.print(hexString);
    Serial.print(" ");
  }

  Serial.print("- ");
  Serial.println(sizeof(data));
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
}


// Sends the current status to the controller through the BLE
void sendCurrentStatus()
{
  config temp;
  EEPROM.get(PkADRESS, temp.Pk);
  EEPROM.get(IkADRESS, temp.Ik);
  EEPROM.get(DkADRESS, temp.Dk);
  EEPROM.get(SpADRESS, temp.speed);

  Serial.print("State: " + String(state, 8));
  Serial.print(" Pk: " + String(temp.Pk, 8));
  Serial.print(" Ik: " + String(temp.Ik, 8));
  Serial.print(" Dk: " + String(temp.Dk, 8));
  Serial.println(" speed: " + String(temp.speed));

  ble_write(state);
  ble_write(temp.Pk);
  ble_write(temp.Ik);
  ble_write(temp.Dk);
  ble_write(temp.speed);

  srl_write(state);
  srl_write(temp.Pk);
  srl_write(temp.Ik);
  srl_write(temp.Dk);
  srl_write(temp.speed);

  Serial.println();
}


void handleIncoming()
{
  // Fetch the available BLE data
  if (ble.available() > 0)
  {
    Serial.println("Something was recieved");
    delay(100);
    bleReadData = readAvailable();
  }

  // Fetch the available serial data
  if (Serial.available() > 0)
  {
    Serial.print("Serial is available: ");
    delay(100);
    Serial.println(Serial.available());
    bleReadData = readAvailableSerial();
  }

  // Check that a command has actually been recieved
  if (bleReadData.command != 0)
  {
    // First 3 bits (from the left) represent the state
    stateCommand = (bleReadData.command) >> 5;

    // If the correct bits are flipped in the command state, overwrite the PID constants and speed.
    if ((bleReadData.command & 0b00010000) >> 4) 
    {
      EEPROM.put(PkADRESS, bleReadData.Pk); 
      settings.Pk = bleReadData.Pk;
    }
    if ((bleReadData.command & 0b00001000) >> 3) 
    {
      EEPROM.put(IkADRESS, bleReadData.Ik);
      settings.Ik = bleReadData.Ik;
    }
    if ((bleReadData.command & 0b00000100) >> 2) 
    {
      EEPROM.put(DkADRESS, bleReadData.Dk);
      settings.Dk = bleReadData.Dk;
    }
    if ((bleReadData.command & 0b00000010) >> 1) 
    {
      EEPROM.put(SpADRESS, bleReadData.speed);
      settings.speed = bleReadData.speed;
      proportionalSpeed = settings.speed / 255.0; // Calculates new proportional speed when speed is recieved
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
