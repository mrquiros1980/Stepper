#include <Arduino.h>

unsigned long startMicros; 
unsigned long currentMicros;
unsigned long period = 220707; // the value is a number of microseconds
const byte ledPin = 13;        // using the built in LED


// A4998 Stepper Driver Pins
const byte ms1 = 3;
const byte ms2 = 4;
const byte ms3 = 5;
byte direction = 8;
const byte step = 9;
const byte enable = 11;

// Joystick Module Pins
const byte switchButton = 2;
const byte joyButton = 1;
const int joy = A0;

int speed = 475;
volatile byte state = LOW; //Set the motor driver "enable" pin value
volatile byte track = LOW; // Variable to enable/disable AR Tracking

const int timeThreshold = 150; // For debouncing (needs some work to fine tuning it)
long startTime = 0;

void enableMotor()
{
    state = !state;
    track = !track;
    digitalWrite(enable, state);
}

void moveStepper()
{
    speed = analogRead(joy);
    digitalWrite(ms1, HIGH);
    digitalWrite(ms2, HIGH);
    digitalWrite(ms3, HIGH);
    if (track == LOW)
    {
        digitalWrite(ms1, LOW);
        digitalWrite(ms2, HIGH);
        digitalWrite(ms3, LOW);
    }
    digitalWrite(direction, HIGH);
    period = 110354;

    if (speed <= 400 && speed >= 200)
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

    if (speed < 200)
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

    if (speed <= 800 && speed >= 600)
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

    if (speed > 800)
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

    if (currentMicros - startMicros >= period)
    {
        digitalWrite(ledPin, !digitalRead(ledPin)); 
        digitalWrite(step, !digitalRead(step));
        startMicros = currentMicros; 
    }
};
void setup()
{
    pinMode(ledPin, OUTPUT);
    pinMode(ms1, OUTPUT);
    pinMode(ms2, OUTPUT);
    pinMode(ms3, OUTPUT);
    pinMode(direction, OUTPUT);
    pinMode(step, OUTPUT);
    pinMode(joy, INPUT);
    pinMode(enable, OUTPUT);
    pinMode(switchButton, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(switchButton), enableMotor, LOW); // Interrupt to monitor the track enable switch triggering

    startMicros = micros(); // initial start time

    digitalWrite(enable, HIGH);
    digitalWrite(ms1, LOW);
    digitalWrite(ms2, HIGH);
    digitalWrite(ms3, HIGH);

    digitalWrite(direction, HIGH);
    digitalWrite(step, HIGH);
    digitalWrite(enable, HIGH);
    
    state = HIGH;
    track = LOW;
}

void loop()
{
    currentMicros = micros(); // get the current "time" (actually the number of microseconds since the program started)

    if (state == LOW)
    {
        moveStepper();
    }
    else
    {
        if (analogRead(joy) <= 400 || analogRead(joy) >= 600)
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