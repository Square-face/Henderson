
// hacky optimization for stupid shit
// why use 2's complement when you can use 2 bytes
struct twoByteSignedChar {
  unsigned char num;
  bool sign;
};


bool isNegative(short n) 
{
    return (n >> (sizeof(n) * 8)) - 1;
}


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

