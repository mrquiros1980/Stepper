#include <Arduino.h>
#include <ESP8266WiFi.h>

char ssid[] = "vodafoneDDE0";
char pass[] = "2MQDXADZYQJZ3M";

unsigned long startMicros; // some global variables available anywhere in the program
unsigned long currentMicros;
unsigned long period = 220707; // the value is a number of microseconds
const int joy = A0;
const byte ms1 = D1;
const byte ms2 = D2;
const byte ms3 = D5;
const byte step = D6;
const byte direction = D7;
const byte enable = D8;
const byte switchButton = 3;
const byte ledPin = 16;

int speed = 475;
volatile byte state = LOW;
volatile byte track = LOW;

const int timeThreshold = 150;
long startTime = 0;

//void IRAM_ATTR enableMotor();

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
    digitalWrite(direction, LOW);
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

    if (currentMicros - startMicros >= period) // test whether the period has elapsed
    {
        digitalWrite(ledPin, !digitalRead(ledPin)); // if so, change the state of the LED.  Uses a neat trick to change the state
        digitalWrite(step, !digitalRead(step));
        startMicros = currentMicros; // IMPORTANT to save the start time of the current LED state.
    }
};
void setup()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);

    pinMode(ms1, OUTPUT);
    pinMode(ms2, OUTPUT);
    pinMode(ms3, OUTPUT);
    pinMode(direction, OUTPUT);
    pinMode(step, OUTPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(joy, INPUT);
    pinMode(enable, OUTPUT);
    pinMode(switchButton, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(switchButton), enableMotor, FALLING);

    startMicros = micros(); // initial start time

    digitalWrite(enable, HIGH);
    digitalWrite(ms1, LOW);
    digitalWrite(ms2, HIGH);
    digitalWrite(ms3, HIGH);

    digitalWrite(direction, LOW);
    digitalWrite(step, HIGH);
    digitalWrite(enable, HIGH);
    digitalWrite(ledPin, HIGH);
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
            digitalWrite(ledPin,HIGH);
        }
    }
}
