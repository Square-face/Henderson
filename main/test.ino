
// Motor 1 pins, left motor
const uint8_t leftMotorDirection = 2;
const uint8_t leftMotorSpeed = 3;

// Motor 2 pins, right motor
const uint8_t rightMotorSpeed = 4;
const uint8_t rightMotorDirection = 5;

void setup()
{
  for (int i = 2; i < 6; i++)
    pinMode(i, OUT);

  digitalWrite(leftMotorDirection, HIGH);
  analogWrite(leftMotorSpeed, 0xFF);
}

void loop()
{
  
}