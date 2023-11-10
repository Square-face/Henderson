#ifndef UTILS
#define UTILS

#include "sensors.h"

// hacky optimization for stupid shit
// why use 2's complement when you can use 2 bytes
struct twoByteSignedChar {
  unsigned char num;
  bool sign;
};

// Structure of the status to send
struct statusStructure {
  uint8_t state;
  float Pk;
  float Ik;
  float Dk;
  uint8_t speed;
};

// Structure of the command recieved
struct commandStructure {
  uint8_t command;
  float Pk;
  float Ik;
  float Dk;
  uint8_t speed;
};

// State variable to be used in main function primarily
// Defined in utils.h
uint8_t state;


// Returns true if the number is negative, otherwise it returns false
bool isNegative(short n) 
{
    return (n >> (sizeof(n) * 8)) - 1;
}

// Converts a short into a twoByteSignedChar, a structure containing a number and a bool depending on the sign
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

#endif