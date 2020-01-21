/*
Project: Astralis LED Sign  
Created by: Jonas Ibsen
Date: 23rd Feburay 2020

https://github.com/Uberibsen

This project is licensed under a Creative Commons Attribution 4.0 International License.
*/

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