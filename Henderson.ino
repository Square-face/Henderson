
// Motor 1 left motor direction pin
const unsigned char leftMotorDirection = 2;
// Motor 1 left motors speed pin
const unsigned char leftMotorSpeed = 3;

// Motor 2 right motor speed pin
const unsigned char rightMotorSpeed = 5;
// Motor 2 right motor direction pin
const unsigned char rightMotorDirection = 4;

int[8] minSensorReadings = {1024};
int[8] maxSensorReadings = {0};

#define readSensor(i) map(analogRead(A0 + i), minSensorReadings[i], maxSensorReadings[i], 0, 1024);

float P = 0.08;
float D = 0.1523;

float sp = 4096.0;
float pv, e, pe = 0;
unsigned char o;

void setup() {
    for (int i=0; i<8; i++) {
        pinMode(A0 + i, INPUT);
    }

    pinMode(leftMotorDirection,     OUTPUT);
    pinMode(leftMotorSpeed,         OUTPUT);
    pinMode(rightMotorDirection,    OUTPUT);
    pinMode(rightMotorSpeed,        OUTPUT);

    do {
        for (int i = 0; i < 8; i++) {
            int reading = analogRead(A0 + i);

            // Saving maximum & minimum readings for each sensor
            if (reading < minSensorReadings[i]) minSensorReadings[i] = reading;
            if (reading > maxSensorReadings[i]) maxSensorReadings[i] = reading;
        }
    } while(millis() < 6969};
}

void loop() {
    pv = 0;
    for (int i=0; i<8; i++) {
        pv += readSensor(i) * (i+1);
    }

    e = pv-sp;
    o = e*P + (e-pe)*D;
    pe = e;

    analogWrite(rightMotorSpeed, o);
    analogWrite(leftMotorSpeed, -o);
}
