#include <Arduino.h>

unsigned long startMicros; //some global variables available anywhere in the program
unsigned long currentMicros;
unsigned long period = 203987; //the value is a number of microseconds
const byte ledPin = 13;        //using the built in LED
const int joy = A0;
const byte ms1 = 3;
const byte ms2 = 4;
byte direction = 8;
const byte step = 9;
const byte switchButton = 2;
const byte joyButton = 1;
const byte enable = 11;
int speed = 475;
volatile byte state = LOW;
volatile byte track = LOW;
int factor = 100;
byte trackLed = 12;

const int timeThreshold = 150;
long startTime = 0;

void enableMotor()
{

    state = !state;
    track = !track;
    digitalWrite(enable, state);
    digitalWrite(trackLed, track);
}

void moveStepper()
{
    speed = analogRead(joy);
    digitalWrite(ms1, HIGH);
    digitalWrite(ms1, HIGH);
    if (track == LOW)
    {
        digitalWrite(ms1, LOW);
        digitalWrite(ms2, HIGH);
    }
    digitalWrite(direction, LOW);
    period = 203987;

    if (speed <= 425 && speed >= 300)
    {
        if (track == HIGH)
        {
            period = period / 4;
        }
        else
        {
            period = period / 40;
        }

        digitalWrite(direction, LOW);
    }

    if (speed < 300)
    {
        if (track == HIGH)
        {
            period = period / 8;
        }
        else
        {
            period = period / 160;
        }
        digitalWrite(direction, LOW);
    }

    if (speed <= 700 && speed >= 520)
    {
        if (track == HIGH)
        {
            period = period / 4;
        }
        else
        {
            period = period / 40;
        }
        digitalWrite(direction, HIGH);
    }

    if (speed > 700)
    {
        if (track == HIGH)
        {
            period = period / 8;
        }
        else
        {
            period = period / 160;
        }
        digitalWrite(direction, HIGH);
    }

    if (currentMicros - startMicros >= period) //test whether the period has elapsed
    {
        digitalWrite(ledPin, !digitalRead(ledPin)); //if so, change the state of the LED.  Uses a neat trick to change the state
        digitalWrite(step, !digitalRead(step));
        startMicros = currentMicros; //IMPORTANT to save the start time of the current LED state.
    }
};
void setup()
{
    pinMode(ledPin, OUTPUT);
    pinMode(ms1, OUTPUT);
    pinMode(ms2, OUTPUT);
    pinMode(direction, OUTPUT);
    pinMode(step, OUTPUT);
    pinMode(joy, INPUT);
    pinMode(enable, OUTPUT);
    pinMode(switchButton, INPUT_PULLUP);
    pinMode(trackLed, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(switchButton), enableMotor, LOW);

    startMicros = micros(); //initial start time

    digitalWrite(enable, HIGH);
    digitalWrite(ms1, HIGH);
    digitalWrite(ms1, HIGH);
    digitalWrite(direction, LOW);
    digitalWrite(step, HIGH);
    digitalWrite(enable, HIGH);
    digitalWrite(trackLed, LOW);
    state = HIGH;
    track = LOW;
}

void loop()
{
    currentMicros = micros(); //get the current "time" (actually the number of microseconds since the program started)

    if (state == LOW)
    {
        moveStepper();
    }
    else
    {
        if (analogRead(joy) <= 425 || analogRead(joy) >= 500)
        {
            digitalWrite(enable, LOW);
            moveStepper();
        }
        else
        {
            digitalWrite(enable, HIGH);
        }
    }
}
