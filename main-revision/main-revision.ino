#include <EEPROM.h>
#include "bluetooth.h"
#include "sensors.h"
#include "utils.h"


// Desired line position. calculated to be half the maximum value that can be read by the sensor array
#define SETPOINT (sensor_count - 1) * 500;


// Limit how over verbose logs are sent over BLE
// Higher value will result in lower performance loss but more inacurate data
#define CYCLES_PER_LOG 25


// variables for verbose logging
// last_log_millis - what millis() returned when the last log was written
// last_log_cycles - how many cycles ago the last log was
long last_log_millis, last_log_cycles;


// calibration 
uint16_t calibrationCycles = 0;
const uint16_t calibrationInterval = 20;


// PID values
double P, I, D;
int input, error, previous_error, result;
double integral;
int output;
uint8_t left, right;

// arbitrary value to base the speed around
uint16_t baseSpeed = 255;





void setup()
{
  Serial.begin(9600);
  Serial.println("Initializing...");

  state = STANDBY;
  initializeBLE();
  initializeSensorPins();

  Serial.println("Loading saved EEPROM data");


  // Retrieve calibration values from EEPROM 
  EEPROM.get(CalMaxADRESS, settings.calibrationMax); 
  EEPROM.get(CalMinADRESS, settings.calibrationMin); 

  // for (u8 i = 0; i < sensor_count; i++)
  // {
  //   settings.calibrationMax[i] = sensors.calibrationOn.maximum[i];
  //   settings.calibrationMin[i] = sensors.calibrationOn.minimum[i];
  // }
  
  // Load calibration
  Serial.println("Loading calibration values from EEPROM");

  // Load minimum calibration values
  Serial.print("MIN: ");
  for (u8 i = 0; i < sensor_count; i++)
  {
    Serial.print(String(settings.calibrationMin[i]) + " ");
    sensors.calibrationOn.minimum[i] = settings.calibrationMin[i];
  }
  Serial.println();


  // Load maximum calibration vaues
  Serial.print("MAX: ");
  for (u8 i = 0; i < sensor_count; i++)
  {
    Serial.print(String(settings.calibrationMax[i]) + " ");
    sensors.calibrationOn.maximum[i] = settings.calibrationMax[i];
  }
  Serial.println();


  // Load PID and speed values
  Serial.println("Loading Pk, Ik, Dk, Speed from EEPROM");
  EEPROM.get(PkADRESS, settings.Pk);
  EEPROM.get(IkADRESS, settings.Ik);
  EEPROM.get(DkADRESS, settings.Dk);
  EEPROM.get(SpADRESS, settings.speed);
  
  Serial.print(settings.Pk);
  Serial.print(" ");
  Serial.print(settings.Ik);
  Serial.print(" ");
  Serial.print(settings.Dk);
  Serial.print(" ");
  Serial.println(settings.speed);

  Serial.println("Setup complete\n\n");
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
      for (u8 i = 0; i < sensor_count; i++)
      {
        sensors.calibrationOn.maximum[i] = 0;
        sensors.calibrationOn.minimum[i] = 1024;
      }
      calibrating();
      break;

    case CALIBRATING_MANUAL:
      calibratingManual();
      break;

    case RUNNING:
      running();
      break;

    case RUNNING_LOGGED:
      running_logged();
      break;

    default:
      break;
  }
}

// Runs whenever the robot has the STANDBY state
void standby()
{

  // State change logic
  switch(stateCommand)
  {
    case CALIBRATING:
      for (u8 i = 0; i < sensor_count; i++)
      {
        sensors.calibrationOn.maximum[i] = 0;
        sensors.calibrationOn.minimum[i] = 1024;
      } 
      Serial.println("State change: Standby -> Calibrating");
      sensors.emittersOn();
      stateCommand = NOTHING;
      state = CALIBRATING;
      return;

    case CALIBRATING_MANUAL:
      for (u8 i = 0; i < sensor_count; i++)
      {
        sensors.calibrationOn.maximum[i] = 0;
        sensors.calibrationOn.minimum[i] = 1024;
      } 
      Serial.println("State change: Standby -> CalibratingManual");
      sensors.emittersOn();
      stateCommand = NOTHING;
      state = CALIBRATING_MANUAL;
      return;
    
    case RUNNING:
      Serial.println("State change: Standby -> Running");
      // sensors.emittersOn();
      proportionalSpeed = settings.speed / 255.0;
      stateCommand = NOTHING;
      state = RUNNING;
      return;

    case RUNNING_LOGGED:
      Serial.println("State change: Standby -> Running");
      // sensors.emittersOn();
      proportionalSpeed = settings.speed / 255.0;
      stateCommand = NOTHING;
      state = RUNNING_LOGGED;
      return;
  }
}

// Runs whenever the robot has the CALIBRATING state
void calibrating()
{
  switch(stateCommand)
  {
    case STANDBY:
      Serial.println("State change: Calibrating -> Standby");
      endCalibration();
      state = STANDBY;
      return;
    
    case CALIBRATING_MANUAL:
      Serial.println("State change: Calibrating -> CalibratingManual");
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

      Serial.println("1/4");
      break;
    
    case calibrationInterval:
      digitalWrite(leftMotorDirection, LEFT_BACKWARD);
      digitalWrite(rightMotorDirection, RIGHT_FORWARD);

      Serial.println("2/4");
      break;

    case calibrationInterval * 3:
      digitalWrite(leftMotorDirection, LEFT_FORWARD);
      digitalWrite(rightMotorDirection, RIGHT_BACKWARD);

      Serial.println("3/4");
      break;

    case calibrationInterval * 4:
      analogWrite(leftMotorSpeed, 0);
      analogWrite(rightMotorSpeed, 0);

      calibrationCycles = 0;
      
      changeState(STANDBY);

      Serial.println("4/4");

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
      Serial.println("State change: calibratingManual -> Standby");
      endCalibration();
      state = STANDBY;
      stateCommand = NOTHING;
      return;
    
    case CALIBRATING:
      Serial.println("State change: calibratingManual -> Calibrating");
      state = CALIBRATING;
      stateCommand = NOTHING;
      return;
  }

  sensors.calibrate(QTRReadMode::On);

  Serial.print("Max: ");
  for (u8 i = 0; i < sensor_count; i++)
  {
    Serial.print(String(sensors.calibrationOn.maximum[i]) + " ");
  }
  Serial.print("Min: ");
  for (u8 i = 0; i < sensor_count; i++)
  {
    Serial.print(String(sensors.calibrationOn.minimum[i]) + " ");
  }
  Serial.println();
}


// Runs whenever the robot has the RUNNING state
void running()
{ 
  // PID steering
  PIDController();
}

void running_logged()
{
  PIDController();

  // inc counter
  if (last_log_cycles < CYCLES_PER_LOG)
  {
    last_log_cycles++;
    return;
  }

  int delta_t = millis() - last_log_millis;
  char mask = 0x0;

  // Write logs to BLE
  ble_write(mask);          // Log indication       1 1
  ble_write(CYCLES_PER_LOG);// Log interval         1 2
  ble_write(delta_t);       // Time since last log  2 4
  ble_write(input);         // Line position        2 8
  ble_write(P);             // Proportional         8 14
  ble_write(I);             // Integral             8 22
  ble_write(D);             // Derivative           8 30
  ble_write(output);        // PID output           2 32
  ble_write(left);          // Left motor speed     1 33
  ble_write(right);         // Right motor speed    1 34
  for (u8 i = 0; i < sensor_count; i++)
    ble_write(sensor_readings[i]); // Raw sensor data    16 50

  // reset counters
  last_log_cycles = 0;
  last_log_millis = millis();
}

// Steers the robot using the PID controller algorithm
void PIDController()
{
  switch (stateCommand)
  {
    case STANDBY:
      Serial.println("State change: Running -> Standby");

      // Disable IRLeds
      sensors.emittersOff();

      // Disable motors
      analogWrite(leftMotorSpeed, 0);
      analogWrite(rightMotorSpeed, 0);

      // Reset PID
      P = 0.0;
      I = 0.0;
      D = 0.0;
      previous_error = 0;

      // Change state
      state = STANDBY;
      stateCommand = NOTHING;
      return;

    case RUNNING:
      state = RUNNING;
      stateCommand = NOTHING;
      break;

    case RUNNING_LOGGED:
      state = RUNNING_LOGGED;
      stateCommand = NOTHING;
      break;
  }
  
  // Get line
  input = readLinePosition();
  error = input - SETPOINT;

  // PID
  P = settings.Pk * error;
  I += settings.Ik * error;
  D = settings.Dk * (error - previous_error);

  // combine and constrain
  result = P + I + D;
  output = constrain(result, -255, 255);

  // calculate motor speeds
  left = constrain(round((baseSpeed + output) * proportionalSpeed), 0, 255);
  right = constrain(round((baseSpeed - output) * proportionalSpeed), 0, 255);

  // Set motor speeds
  analogWrite(leftMotorSpeed, left);
  analogWrite(rightMotorSpeed, right);

  // update last error
  previous_error = error;
}
