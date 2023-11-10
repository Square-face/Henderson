#include <QTRSensors.h>

const uint8_t sensor_count = 6;
uint16_t sensor_readings[sensor_count];
QTRSensors sensors;

void initializeSensorPins()
{
  sensors.setTypeAnalog();
  sensors.setSensorPins((const unsigned char[]){A0, A1, A2, A3, A4, A5, A6, A7}, sensor_count);
  sensors.setEmitterPin(11);
}

unsigned short readArray()
{
  sensors.read(sensor_readings);
  return sensors.readLineBlack(sensor_readings);
}