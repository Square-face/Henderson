
// hacky optimization for stupid shit
// why use 2's complement when you can use 2 bytes
struct twoByteSignedChar {
  char num;
  bool sign;
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  char x = 255;
  analogWrite(13, x);
}

bool isNegative(short n) {
    return n >> (sizeof(n) * 8) - 1;
}

int readArray()
{
  // TODO
}