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

void setup() { 
    FastLED.addLeds<WS2812B, DATA_PIN>(leds, NUM_LEDS);

    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}
*/
// Globals

#define LATCH 7
#define CLK 6
#define DATA 5

#define buttonPin 3

int sequenceNumber = 1;
int buttonState = 0;
int lastButtonState = 0;

// Declaring the bytes transfered to the 74HC595 making up the 7-segment display
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

    // Writing a 1 for the 7-segment display

    buttonState = digitalRead(buttonPin);

    if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
        if (buttonState == HIGH){
          // if the current state is HIGH then the button went from off to on:
          sequenceNumber++;
          Serial.print("Lighting Sequence: ");
          Serial.println(sequenceNumber);
          if (sequenceNumber == 6){
            sequenceNumber = 1;
          }
          if (sequenceNumber == 1){
            digitalWrite(LATCH, LOW);
            shiftOut(DATA, CLK, MSBFIRST, digit[sequenceNumber]);
            digitalWrite(LATCH, HIGH);
          }
          else if (sequenceNumber == 2){
            digitalWrite(LATCH, LOW);
            shiftOut(DATA, CLK, MSBFIRST, digit[sequenceNumber]);
            digitalWrite(LATCH, HIGH); 
          }
          else if(sequenceNumber == 3){
            digitalWrite(LATCH, LOW);
            shiftOut(DATA, CLK, MSBFIRST, digit[sequenceNumber]);
            digitalWrite(LATCH, HIGH); 
          }
          else if (sequenceNumber == 4){
            digitalWrite(LATCH, LOW);
            shiftOut(DATA, CLK, MSBFIRST, digit[sequenceNumber]);
            digitalWrite(LATCH, HIGH); 
          }
          else if (sequenceNumber == 5){
            digitalWrite(LATCH, LOW);
            shiftOut(DATA, CLK, MSBFIRST, digit[sequenceNumber]);
            digitalWrite(LATCH, HIGH);
          }
      }
    delay(200);
    // save the current state as the last state, for next time through the loop
    lastButtonState = buttonState;
  }
}