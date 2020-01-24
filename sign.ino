/*
Project: Astralis LED Sign  
Created by: Jonas Ibsen
Date: 23rd Feburay 2020

https://github.com/Uberibsen

This project is licensed under a Creative Commons Attribution 4.0 International License.
*/
/*
#include <FastLED.h>
#define NUM_LEDS 72
#define DATA_PIN 6

CRGB leds[NUM_LEDS];

// Defining the sequence button
const int sequenceButton = 3;
int buttonState = LOW;

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

int sequenceNumber = 1;

void setup() { 
    FastLED.addLeds<WS2812B, DATA_PIN>(leds, NUM_LEDS);

    pinMode(sequenceButton, INPUT);


    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

void loop(){

    //sample the state of the button - is it pressed or not?
    buttonState = digitalRead(sequenceButton);

    //filter out any noise by setting a time buffer
    if ( (millis() - lastDebounceTime) > debounceDelay) {
        if (buttonState == HIGH) {
            sequenceNumber++;
            lastDebounceTime = millis(); //set the current time
        }
    }

    if (sequenceNumber = 1){

    }

    else if (sequenceNumber = 2){

    }

    else if (sequenceNumber = 3){

    }

    else if (sequenceNumber = 4){

    } 
}
*/

// Globals

#define LATCH 7
#define CLK 6
#define DATA 5

/*byte digit[]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};

const int dataPin = 5; // blue wire to 74HC595 pin 5
const int latchPin = 7; // green to 74HC595 pin 7
const int clockPin = 6; // yellow to 74HC595 pin 6

const char common = 'c'; // common cathode
unsigned int cnt = 0;
*/

int sequenceNumber = 0;
int buttonState = 0;
int lastButtonState = 0;

const int buttonPin = 4;

const byte digit[]= {
  B00111111,  // 0
  B00000110,  // 1
  B01011011,  // 2
  B01001111,  // 3
  B01100110,  // 4
  B01101101,  // 5
  B01111101,  // 6
  B00000111,  // 7
  B01111111,  // 8
  B01101111,  // 9
};
void setup() {
// initialize I/O pins

    pinMode(buttonPin, INPUT);

    pinMode(LATCH, OUTPUT);
    pinMode(CLK, OUTPUT);
    pinMode(DATA, OUTPUT);

    Serial.begin(9600);
}
void loop(){

    buttonState = digitalRead(buttonPin);

    if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      sequenceNumber++;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(sequenceNumber);
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;

    for(int i=0; i<10; i++){
        digitalWrite(LATCH, LOW);
        shiftOut(DATA, CLK, MSBFIRST, digit[i]); // digitOne
        digitalWrite(LATCH, HIGH);
        delay(1000);    
  }
}