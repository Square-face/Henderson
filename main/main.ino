#include <QTRSensors.h>
#include "BLESerial.h"
#include <SoftwareSerial.h>

// hacky optimization for stupid shit
// why use 2's complement when you can use 2 bytes
struct twoByteSignedChar {
  unsigned char num;
  bool sign;
};

#define RX 2
#define TX 3

const unsigned char calibrationSpeed = 0x80;

// Motor 1 pins, left motor
const unsigned char leftMotorDirection = 4;
const unsigned char leftMotorSpeed = 5;

// Motor 2 pins, right motor
const unsigned char rightMotorSpeed = 6;
const unsigned char rightMotorDirection = 7;

// == state variables ==

// calibration
bool calibrating = true;
unsigned short calibration_cycles = 64;

// analog readings
const uint8_t sensor_count = 6;
uint16_t sensor_readings[sensor_count];
QTRSensors sensors;

// Global variables for the PID controller
// These need a value
const float Pk = 0.07, Ik = 0, Dk = 0.7, IntegralMax = 1000;

int lastError = 0;
int Integral = 0;
// int IntegralResetRange = 8; IN REVIEW

// Something something BLE
BLESerial ble(RX, TX);

void setup() 
{
  ble.begin();
  ble.setName("Henderson");

  sensors.setTypeAnalog();
  sensors.setSensorPins((const unsigned char[]){A0, A1, A2, A3, A4, A5}, sensor_count);
  sensors.setEmitterPin(11);

  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode

  calibrate();
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(9600);
  for (uint8_t i = 0; i < sensor_count; i++)
  {
    Serial.print(sensors.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (uint8_t i = 0; i < sensor_count; i++)
  {
    Serial.print(sensors.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
  
  // Sets the motor pins to OUTPUT
  for (int i = 4; i <= 7; i++)
    pinMode(i, OUTPUT);

  digitalWrite(leftMotorDirection, HIGH);
  digitalWrite(rightMotorDirection, HIGH);
}

void loop() 
{
  unsigned short sensorReadings = readArray();

  twoByteSignedChar controller = PID(sensorReadings);

  // Serial.print("Sensors: ");
  Serial.print(sensorReadings);
  Serial.print(" | ");
  Serial.println((controller.sign) ? -controller.num : controller.num);

  steer(controller);
}

bool isNegative(short n) 
{
    return n >> (sizeof(n) * 8) - 1;
}



/*
 * Get the current line position as a unsigned short between 0 (all the way to the left) and <num_sensors>*1000 (all the way to the right)
 * If the line is lost, i.e the robot did not turn fast enough, the value returned will still be towards the side the line was lost at.
*/
unsigned short readArray()
{
  sensors.read(sensor_readings);
  return sensors.readLineBlack(sensor_readings);
}



/*
 * function to run when calibrating
 * outputs a bool indicating if the calibration should continue the next loop or if it is done
 * false means the calibration is done
*/
bool calibrate() {
  // spin 
  digitalWrite(leftMotorDirection, HIGH);
  digitalWrite(rightMotorDirection, LOW);

  analogWrite(leftMotorSpeed, calibrationSpeed);
  analogWrite(rightMotorSpeed, calibrationSpeed);


  Serial.println("Calibrating...");
  for (int i = 0; i<calibration_cycles; i++){
    sensors.calibrate();
  }

  // unspin 
  digitalWrite(leftMotorDirection, HIGH);
  digitalWrite(rightMotorDirection, HIGH);

  analogWrite(leftMotorSpeed, 0);
  analogWrite(rightMotorSpeed, 0);

  Serial.println("Done!");
}

// PID controller function, takes an input from the 
twoByteSignedChar PID(int input)
{
  int goal = 3000; // Depending on your implementation of readArray(), change this to be the middle value.
  int error = input - goal;

  /* IN REVIEW
  // Reset integral if error is ~0 
  if (error > -IntegralResetRange && error < IntegralResetRange)
    Integral = 0;
  */

  // P
  int Proportional = error;

  // I
  Integral = Integral + error;

  // D
  int Derivative = error - lastError;
  lastError = error;
  
  // Calculates the motor speed and formats it as a twoByteSignedChar
  short motorspeed = round((Proportional * Pk) + (Integral * Ik) + (Derivative * Dk));
  twoByteSignedChar output = convertToReligion(motorspeed);
  
  return output;
}

const uint8_t base_speed = 150;

// Sets the speed of the wheel based on the input
void steer(twoByteSignedChar input)
{
  uint8_t high_speed = base_speed + input.num; 
  uint8_t low_speed = base_speed - input.num; 

  if (!input.sign) { // negative
    // Set right wheel to max and reduce speed on left wheel
    analogWrite(leftMotorSpeed, low_speed); 
    analogWrite(rightMotorSpeed, high_speed);
    return;
  }

  // Set left wheel to max and reduce speed on right wheel
  analogWrite(leftMotorSpeed, high_speed);
  analogWrite(rightMotorSpeed, low_speed);
}
// We might need to decrease the max speed as the motors are basically always maxed here.

// Converts a short to a twoByteSignedChar
twoByteSignedChar convertToReligion(short n)
{
  short num = abs(n);
  bool sign = isNegative(n);

  if (num > 0xFF)
  {
    return {0xFF, sign};
  }

  return {(unsigned char) num, sign};
}