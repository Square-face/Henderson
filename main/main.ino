#include <QTRSensors.h>


// hacky optimization for stupid shit
// why use 2's complement when you can use 2 bytes
struct twoByteSignedChar {
  unsigned char num;
  bool sign;
};


const unsigned char calibrationSpeed = 0x50;


// Motor 1 pins, left motor
const unsigned char leftMotorDirection = 4;
const unsigned char leftMotorSpeed = 5;

// Motor 2 pins, right motor
const unsigned char rightMotorSpeed = 6;
const unsigned char rightMotorDirection = 7;


// == state variables ==

// calibration
bool calibrating = false;
unsigned short calibration_cycles = 0xFFFF;

// analog readings
unsigned short sensor_readings[6];
QTRSensors sensors;



void setup() 
{
  sensors.setTypeAnalog();
  sensors.setSensorPins((const unsigned char[]){A1, A2, A3, A4, A5, A6}, 6);
  
  // Sets the motor pins to OUTPUT
  for (int i = 4; i <= 7; i++)
    pinMode(i, OUTPUT);

  digitalWrite(leftMotorDirection, HIGH);
  digitalWrite(rightMotorDirection, HIGH);
}

void loop() 
{
  // calibration check
  if (calibrating) {calibrating = calibrate(); return;}
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
  if (!calibration_cycles > 0){
    calibrating = false;

      // unspin 
      digitalWrite(leftMotorDirection, HIGH);
      digitalWrite(rightMotorDirection, HIGH);

      analogWrite(leftMotorSpeed, 0);
      analogWrite(rightMotorSpeed, 0);
  }
  sensors.calibrate()
  
  // spin 
  digitalWrite(leftMotorDirection, HIGH);
  digitalWrite(rightMotorDirection, LOW);

  analogWrite(leftMotorSpeed, calibrationSpeed);
  analogWrite(rightMotorSpeed, calibrationSpeed);

  calibration_cycles--;
}




twoByteSignedChar PID(int input)
{
  int goal = (sizeof(sensor_readings) / sizeof(unsigned int))*500; // Depending on your implementation of readArray(), change this to be the middle value.
  int error = input - goal;

  // TODO
}

// Sets the speed of the wheel based oon the input
void steer(twoByteSignedChar input)
{
  if (input.sign) { // negative
    // Set right wheel to max and reduce speed on left wheel
    analogWrite(leftMotorSpeed, (input.num ^ 0xFF)); 
    analogWrite(rightMotorSpeed, 0xFF);
    return
  }

  // Set left wheel to max and reduce speed on right wheel
  analogWrite(leftMotorSpeed, 0xFF);
  analogWrite(rightMotorSpeed, (input.num ^ 0xFF));
}

