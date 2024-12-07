
// Motor 1 left motor direction pin
const unsigned char leftMotorDirection = 2;
// Motor 1 left motors speed pin
const unsigned char leftMotorSpeed = 3;

// Motor 2 right motor speed pin
const unsigned char rightMotorSpeed = 5;
// Motor 2 right motor direction pin
const unsigned char rightMotorDirection = 4;

int minSensorReadings[8] = {1024};
int maxSensorReadings[8] = {0};

#define readSensor(i) map(analogRead(A0 + i), minSensorReadings[i], maxSensorReadings[i], 0, 1024)

int clamp(int v, int min, int max) {
    if (v < min) {
        return min;
    }

    if (v > max) {
        return max;
    }

    return v;
}

float P = 0.65;
float D = 1.4;

float sp = 5.0;
float pv, e, sum = 0, pe = 0;
int o;

float last_pv = 0.0;

void setup() {
    Serial.begin(9600);
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
    } while(millis() < 5000);
}

void loop() {
    pv = 0;
    sum = 0;
    for (int i=0; i<8; i++) {
        pv += readSensor(i) * (i+1);
        sum += analogRead(A0 + i);
    }



    if (sum < 700) {
        pv = (last_pv>sp)*8.0;
    } else {
        pv /= sum;
        last_pv = pv;
    }


    Serial.print("Line at: ");
    Serial.print(pv);

    e = pv-sp;
    o = (e*P + (e-pe)*D)*100;
    pe = e;

    Serial.print(", o: ");
    Serial.println(sum);

    analogWrite(rightMotorSpeed, clamp(255 - o, 0, 255));
    analogWrite(leftMotorSpeed, clamp(255 + o, 0, 255));
}
