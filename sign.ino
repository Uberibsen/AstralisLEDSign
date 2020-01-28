/*
Project: Astralis LED Sign  
Created by: Jonas Ibsen
Date: 23rd Feburay 2020

https://github.com/Uberibsen

This project is licensed under a Creative Commons Attribution 4.0 International License.
*/

// LED strip WS2812B
#include <FastLED.h>
#define NUM_LEDS 75
#define DATA_PIN 2

CRGB leds[NUM_LEDS];

// 7 segment display
#define LATCH 7
#define CLK 6
#define DATA 5

// Lightning sequence button
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

  pinMode(DATA_PIN, OUTPUT);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  Serial.begin(9600);

}

void loop(){

  for(int dot = 0; dot < NUM_LEDS; dot++) { 
              leds[dot] = CRGB::Red;
              FastLED.show();
              // clear this led for the next time around the loop
              leds[dot] = CRGB::Black;
              delay(30);
          }

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
  delay(150);
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;
  }
}
