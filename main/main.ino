#include <QTRSensors.h>


// hacky optimization for stupid shit
// why use 2's complement when you can use 2 bytes
struct twoByteSignedChar {
  unsigned char num;
  bool sign;
};



// Motor 1 pins, left motor
const unsigned char E1 = 5;
const unsigned char M1 = 4;

// Motor 2 pins, right motor
const unsigned char E2 = 6;
const unsigned char M2 = 7;


// analog readings
unsigned short sensor_readings[8];
QTRSensorsAnalog sensors((unsigned char[]) {A1, A2, A3, A4, A5, A6}, 6);

void setup() 
{
  // Sets the motor pins to OUTPUT
  for (int i = 4; i <= 7; i++)
    pinMode(i, OUTPUT);

  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  char x = 255;
  analogWrite(13, x);
}

bool isNegative(short n) 
{
    return n >> (sizeof(n) * 8) - 1;
}




unsigned int readArray()
{
  sensors.read(sensor_readings);
  return sensors.readLine(sensor_readings)
}





twoByteSignedChar PID(int input)
{
  int goal = 0; // Depending on your implementation of readArray(), change this to be the middle value.
  int error = input - goal;

  // TODO
}

// Sets the speed of the wheel based oon the input
void steer(twoByteSignedChar input)
{
  if (input.sign) { // If negative
    // Set right wheel to max and reduce speed on left wheel
    analogWrite(E1, (input.num ^ 0xFF)); 
    analogWrite(E2, 0xFF);
  } else {  // If positive
    // Set left wheel to max and reduce speed on right wheel
    analogWrite(E1, 0xFF);
    analogWrite(E1, (input.num ^ 0xFF));
  }
}

