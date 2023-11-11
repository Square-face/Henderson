#include <EEPROM.h>
#include "bluetooth.h"
#include "sensors.h"
#include "utils.h"


uint16_t calibrationCycles = 0;
const uint16_t calibrationInterval = 20;
const uint16_t goal = sensor_count * 500;


// PID controller proportional error
int error = 0;
// PID controller error of last cylce, used to calculate derivative
int lastError = 0;
// PID controller error integral
int integral = 0;
// PID controller error derivative
int derivative = 0;
// PID controller steering output
int steering;
// PID speed multiplier for steering
float proportionalSpeed = 0;


uint8_t baseSpeed = 150;


void setup()
{
  state = STANDBY;
  initializeBLE();

  Serial.begin(9600);
  Serial.println(" - Setup complete");

  initializeSensorPins();

  // Retrieve calibration values from EEPROM 
  EEPROM.get(CalMaxADRESS, settings.calibrationMax); 
  EEPROM.get(CalMinADRESS, settings.calibrationMin); 

  Serial.print("Calibratin max values: ");
  for (u8 i = 0; i < sensor_count; i++)
  {
    Serial.print(String(settings.calibrationMax[i]) + " ");
    sensors.calibrationOn.maximum[i] = settings.calibrationMax[i];
  }
  Serial.println();

  Serial.print("Calibratin min values: ");
  for (u8 i = 0; i < sensor_count; i++)
  {
    Serial.print(String(settings.calibrationMin[i]) + " ");
    sensors.calibrationOn.minimum[i] = settings.calibrationMin[i];
  }
  Serial.println();

  // Retrieve PID values and speed from EEPROM
  EEPROM.get(PkADRESS, settings.Pk);
  EEPROM.get(IkADRESS, settings.Ik);
  EEPROM.get(DkADRESS, settings.Dk);
  EEPROM.get(SpADRESS, settings.speed);
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

  case CALIBRATING_MANUAL:
    calibratingManual();
    break;

  case RUNNING:
    running();
    break;

  default:
    break;
  }

  /* Serial.print("Sensors: ");
  for (u8 i = 0; i < sensor_count; i++)
  {
    Serial.print(analogRead(A0 + i));
    Serial.print(" ");
  }
  Serial.println(); */
}

// Runs whenever the robot has the STANDBY state
void standby()
{
  switch(stateCommand)
  {
    case CALIBRATING:
      sensors.emittersOn(); 
      stateCommand = NOTHING;
      state = CALIBRATING;
      return;

    case CALIBRATING_MANUAL:
      sensors.emittersOn();
      stateCommand = NOTHING;
      state = CALIBRATING_MANUAL;
      return;
    
    case RUNNING:
      sensors.emittersOn();
      proportionalSpeed = settings.speed / 255.0;
      stateCommand = NOTHING;
      state = RUNNING;
      return;
  }

  // STANDBY logic goes here
  /* sensors.emittersOn();

  analogWrite(leftMotorSpeed, settings.speed);
  analogWrite(rightMotorSpeed, settings.speed);

  digitalWrite(leftMotorDirection, LEFT_FORWARD);
  digitalWrite(rightMotorDirection, RIGHT_FORWARD); */
}

// Runs whenever the robot has the CALIBRATING state
void calibrating()
{
  switch(stateCommand)
  {
    case STANDBY:
      endCalibration();
      state = STANDBY;
      return;
    
    case CALIBRATING_MANUAL:
      state = CALIBRATING_MANUAL;
      return;
  }
  stateCommand = NOTHING;

  // CALIBRATING logic goes here

  switch(calibrationCycles)
  {
    case 0:
      digitalWrite(leftMotorDirection, LEFT_FORWARD);
      digitalWrite(rightMotorDirection, RIGHT_BACKWARD);

      analogWrite(leftMotorSpeed, settings.speed);
      analogWrite(rightMotorSpeed, settings.speed);

      Serial.println("Case 0");
      break;
    
    case calibrationInterval:
      digitalWrite(leftMotorDirection, LEFT_BACKWARD);
      digitalWrite(rightMotorDirection, RIGHT_FORWARD);

      Serial.println("Case calibrationInterval");
      break;

    case calibrationInterval * 3:
      digitalWrite(leftMotorDirection, LEFT_FORWARD);
      digitalWrite(rightMotorDirection, RIGHT_BACKWARD);

      Serial.println("Case calibrationInterval * 3");
      break;

    case calibrationInterval * 4:
      analogWrite(leftMotorSpeed, 0);
      analogWrite(rightMotorSpeed, 0);

      calibrationCycles = 0;
      
      changeState(STANDBY);

      Serial.println("Case CalibrationInterval * 4");

      return;
  }

  sensors.calibrate(QTRReadMode::On);

  calibrationCycles++;
}

// Runs whenever the robot has the CALIBRATING_MANUAL state
void calibratingManual()
{
  switch(stateCommand)
  {
    case STANDBY:
      endCalibration();
      state = STANDBY;
      return;
    
    case CALIBRATING:
      state = CALIBRATING;
      return;
  }
  stateCommand = NOTHING;

  sensors.calibrate(QTRReadMode::On);
}

// Runs whenever the robot has the RUNNING state
void running()
{
  if (stateCommand == STANDBY)
  {
    sensors.emittersOff();

    analogWrite(leftMotorSpeed, 0);
    analogWrite(rightMotorSpeed, 0);

    integral = 0;
    error = 0;
    lastError = 0;
    derivative = 0;

    state = STANDBY;
    stateCommand = NOTHING;
    return;
  }

  // RUNNING logic goes here
  uint16_t linePos = readLinePosition();
  error = linePos - goal;
  // Serial.println("Line: " + String(linePos) + ", Error: " + String(error));

  integral += error;

  derivative = error - lastError;
  lastError = error;

  steering =  (error       * settings.Pk) +
              (integral    * settings.Ik) + 
              (derivative  * settings.Dk);

  Serial.print("Left: " + String((uint8_t) round((baseSpeed + steering) * proportionalSpeed)));
  Serial.println(", Right: " + String((uint8_t) round((baseSpeed - steering) * proportionalSpeed)));

  analogWrite(leftMotorSpeed,  (uint8_t) round((baseSpeed + steering) * proportionalSpeed));
  analogWrite(rightMotorSpeed, (uint8_t) round((baseSpeed - steering) * proportionalSpeed));
}

