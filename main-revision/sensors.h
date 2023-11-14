#ifndef SENSORS
#define SENSORS


#include <QTRSensors.h>
#include "utils.h"


uint16_t sensor_readings[sensor_count];
uint16_t sensor_readings_copy[sensor_count];
QTRSensors sensors;


void initializeSensorPins()
{
  sensors.setTypeAnalog();
  sensors.setSensorPins((const unsigned char[]){A0, A1, A2, A3, A4, A5, A6, A7}, sensor_count);
  sensors.setEmitterPin(12);
  sensors.calibrate();
}


unsigned short readArray()
{
  sensors.readCalibrated(sensor_readings);
  memcpy(sensor_readings_copy, sensor_readings, sizeof(sensor_readings));
  return sensors.readLineBlack(sensor_readings);
}


void endCalibration()
{
  analogWrite(leftMotorSpeed, 0);
  analogWrite(rightMotorSpeed, 0);

  digitalWrite(leftMotorDirection, LEFT_FORWARD);
  digitalWrite(rightMotorDirection, RIGHT_FORWARD);

  for (int i = 0; i < sensor_count; i++)
  {
    EEPROM.put(CalMaxADRESS + i * sizeof(uint16_t), sensors.calibrationOn.maximum[i]);
    EEPROM.put(CalMinADRESS + i * sizeof(uint16_t), sensors.calibrationOn.minimum[i]);
  }

  
  sensors.emittersOff();

  Serial.print("Calibration maximum: ");
  for (int i = 0; i < sensor_count; i++)
    Serial.print(String(sensors.calibrationOn.maximum[i]) + " ");
  Serial.println();

  Serial.print("Calibration minimum: ");
  for (int i = 0; i < sensor_count; i++)
    Serial.print(String(sensors.calibrationOn.minimum[i]) + " ");
  Serial.println();

  // uint16_t tempCalMax[sensor_count];
  // uint16_t tempCalMin[sensor_count];

  // EEPROM.get(CalMaxADRESS, tempCalMax); 
  // EEPROM.get(CalMinADRESS, tempCalMin); 

  // Serial.print("Calibration maximum: ");
  // for (int i = 0; i < sensor_count; i++)
  //   Serial.print(String(tempCalMax[i]) + " ");
  // Serial.println();

  // Serial.print("Calibration minimum: ");
  // for (int i = 0; i < sensor_count; i++)
  //   Serial.print(String(tempCalMin[i]) + " ");
  // Serial.println();

}


uint16_t readLinePosition()
{
  sensors.readCalibrated(sensor_readings);
  return sensors.readLineBlack(sensor_readings);
}


#endif