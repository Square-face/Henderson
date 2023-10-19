
// hacky optimization for stupid shit
// why use 2's complement when you can use 2 bytes
struct twoByteSignedChar {
  unsigned char num;
  bool sign;
};

// Motor 1 pins, left motor
const char E1 = 5;
const char M1 = 4;
// Motor 2 pins, right motor
const char E2 = 6;
const char M2 = 7;

// Global variables for the PID controller
// These need a value
const float Pk, Ik, Dk;

int lastError = 0;
int Integral = 0;
int IntegralResetRange = 8;

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

int readArray()
{
  // TODO
}

twoByteSignedChar PID(int input)
{
  int goal = 0; // Depending on your implementation of readArray(), change this to be the middle value.
  int error = input - goal;

  /* IN REVIEW
  // Reset integral if error is ~0 
  if (error > -IntegralResetRange && error < IntegralResetRange)
    Integral = 0;
  */

  // P
  int Proportional = error;

  // I
  Integral = min(Integral + error, );

  // D
  int Derivative = error - lastError;
  lastError = error;
  
  // Calculates the motor speed and formats it as a twoByteSignedChar
  short motorspeed = min(round((P * Pk) + (I * Ik) + (D * Dk)), 0xFF);
  twoByteSignedChar output;
  output.sign = isNegative(motorspeed);
  output.num = (char) abs(motorspeed);

  return output;
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
// We might need to decrease the max speed as the motors are basically always maxed here.
