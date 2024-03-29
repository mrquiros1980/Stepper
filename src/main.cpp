#include <Arduino.h>

unsigned long startMicros; 
unsigned long currentMicros;
unsigned long period = 27588; // the value is a number of microseconds
const byte ledPin = 13;        // using the built in LED


// A4998 Stepper Driver Pins
const byte ms1 = 8;
const byte ms2 = 7;
const byte ms3 = 6;
byte direction = 4;
const byte step = 5;
const byte enable = 9;

// Joystick Module Pins
const byte switchButton = 2;
const int joy = A0;

int speed = 475;
volatile byte track = LOW; // Variable to enable/disable AR Tracking

const int timeThreshold = 150; // For debouncing (needs some work to fine tuning it)
long startTime = 0;

void enableMotor()
{
    digitalWrite(enable, track);
    track = !track;
}

void moveStepper()
{
    speed = analogRead(joy);
    period = 27588;
    digitalWrite(direction, HIGH);

    if (speed < 200)
    {
        period = period / 8;
        digitalWrite(direction, LOW);
    }

    if (speed > 800)
    {
        period = period / 8;
        digitalWrite(direction, HIGH);
    }

    if (currentMicros - startMicros >= period && track == true) 
    {
        digitalWrite(ledPin, !digitalRead(ledPin)); 
        digitalWrite(step, !digitalRead(step));
        startMicros = currentMicros; 
    }
    if(!track) digitalWrite(ledPin,LOW);
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
    attachInterrupt(digitalPinToInterrupt(switchButton), enableMotor, HIGH); // Interrupt to monitor the track enable switch triggering

    startMicros = micros(); // initial start time

    //digitalWrite(enable, LOW);
    digitalWrite(ms1, HIGH);
    digitalWrite(ms2, HIGH);
    digitalWrite(ms3, HIGH);

    digitalWrite(direction, HIGH);
    digitalWrite(step, HIGH);
    digitalWrite(enable, HIGH);
    
    track = false;
}

void loop()
{
    currentMicros = micros(); // get the current "time" (actually the number of microseconds since the program started)
    moveStepper();
    
}
