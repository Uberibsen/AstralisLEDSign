/*
Project: Astralis LED Sign
Created by: Jonas Ibsen

https://github.com/Uberibsen

This project is licensed under a Creative Commons Attribution 4.0 International License.
*/

// LED strip WS2812B
#include "FastLED.h"
#define NUM_LEDS 72
#define LED_DATA 12
const int brightnessPin = A1; // Brightness control
CRGB leds[NUM_LEDS];
#define MAX_BRIGHTNESS 220 // Maximum brightness
#define MIN_BRIGHTNESS 20 // Minimum brightness

// 7-segment display
#define LATCH 9
#define CLK 7
#define DATA 8

// Lightning sequence button
#define buttonPin 11
int sequenceNumber = -1;
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

// Declaring the bytes transferred to the 74HC595 making up the 7-segment display
const byte digit[]= {
  B11111101,  // 0
  B01100000,  // 1
  B11011011,  // 2
  B11110011,  // 3
  B01100110,  // 4
  B10110111,  // 5
  B10111111,  // 6
  B11100000,  // 7
  B11111110,  // 8
  B11110111,  // 9
  B10011111,  // E
};

void setup() {

  // 74HC595 Bit Shift Register Pins
  pinMode(LATCH, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DATA, OUTPUT);

  // WS2812B Pins
  pinMode(LED_DATA, OUTPUT);
  FastLED.addLeds<WS2812, LED_DATA, GRB>(leds, NUM_LEDS);

  // Sequence Button Pin
  pinMode(buttonPin, INPUT);

  // Brightness Control Pin
  pinMode(brightnessPin, INPUT);
}

// **************************
// ** LED Effect Functions **
// **************************

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifndef ADAFRUIT_NEOPIXEL_H
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void showStrip() {
 #ifndef ADAFRUIT_NEOPIXEL_H
   FastLED.show();
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

// Fade in and out effect
int iFadeInOut = 0;
bool bFadeInOut = true;

void FadeInOut(byte red, byte green, byte blue) {
  float r, g, b;

  if (bFadeInOut) {
    r = (iFadeInOut/256.0)*red;
    g = (iFadeInOut/256.0)*green;
    b = (iFadeInOut/256.0)*blue;
    setAll(r,g,b);
    showStrip();

    iFadeInOut++;

    bFadeInOut = iFadeInOut <= 255;
  } else {
    r = (iFadeInOut/256.0)*red;
    g = (iFadeInOut/256.0)*green;
    b = (iFadeInOut/256.0)*blue;
    setAll(r,g,b);
    showStrip();
    
    iFadeInOut -= 2;

    bFadeInOut = iFadeInOut <= 0;
  }
}

// Running lights effect
int iRunningLights = 1;
int RunningLightsDistance = 5;
int RunningLightsDelay = 4;

void RunningLights(byte red, byte green, byte blue) {

  for(int i=0; i<NUM_LEDS; i++) {
    setPixel(i,((sin(i+(iRunningLights / RunningLightsDelay)) * 127 + 128)/255)*red,
                ((sin(i+(iRunningLights / RunningLightsDelay)) * 127 + 128)/255)*green,
                ((sin(i+(iRunningLights / RunningLightsDelay)) * 127 + 128)/255)*blue);
  }
  showStrip();

  iRunningLights = (iRunningLights + 1) % (RunningLightsDistance * RunningLightsDelay);
}

// Theater chase effect
int theaterDistance = 3;
int theaterDelay = 300;
int theaterJ = 0;
int theaterI = 0;

void theaterChase(byte red, byte green, byte blue) {
  if (theaterI == 0) {
    for (int i=0; i < NUM_LEDS; i++) {
      setPixel(i, 0,0,0);
    }

    for (int i=0; i < NUM_LEDS; i+=theaterDistance) {
      setPixel(i+theaterJ, red, green, blue);    //turn every theaterDistance pixel on
    }
    showStrip();

    theaterJ = (theaterJ + 1) % theaterDistance;
  }

  theaterI = (theaterI + 1) % theaterDelay;
}

// Meteor rain effect
int meteorRainI = 0;
int meteorRainDelay = 10;
int meteorTrailLength = 30;

void meteorRain(byte red, byte green, byte blue) 
{
  int i = meteorRainI / meteorRainDelay;
  int positionMod = i + NUM_LEDS - meteorTrailLength;

  for (int j = meteorTrailLength; j >= 0; j--)
  {
    int position = (j + positionMod) % NUM_LEDS;
    int strength = ((j * 255) / meteorTrailLength);
    setPixel(position, (red * strength) / 255, (green * strength) / 255, (blue * strength) / 255);
  }
  for (int j = 1; j < NUM_LEDS - meteorTrailLength; j++)
  {
    setPixel((i + j + NUM_LEDS) % NUM_LEDS, 0, 0, 0);
  }
  showStrip();
  
  meteorRainI = (meteorRainI + 1) % (NUM_LEDS * meteorRainDelay);
}

// Sparkle effect
void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,red,green,blue);
  showStrip();
  delay(SpeedDelay);
  setPixel(Pixel,0,0,0);
}

// Helper function for switch/case statements
// Controls the 7-segment display
void writeAndShift(int digitNumber) {
  for (int i = 0; i <= 8; i++)
  {
    digitalWrite(LATCH, LOW);
    digitalWrite(CLK, LOW);

    shiftOut(DATA, CLK, MSBFIRST, digit[digitNumber] >> i);
    digitalWrite(LATCH, HIGH);
    delay(20);
  }
}

// ***********************
// ** LED strip control **
// ***********************

void loop(){
  // LED lightning sequence control
  buttonState = digitalRead(buttonPin);

  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      sequenceNumber = (sequenceNumber+1) % 5;
      writeAndShift(sequenceNumber + 1);
      setAll(0, 0, 0);
    }
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;

  // LED brightness control
  int brightnessValue = map(analogRead(brightnessPin), 0, 1023, 0, 255);
  FastLED.setBrightness(constrain(brightnessValue, MIN_BRIGHTNESS, MAX_BRIGHTNESS));

  switch(sequenceNumber){

    case 0:
      FadeInOut(0xff, 0x00, 0x00); // Only using red
      break;

    case 1:
      RunningLights(0xff, 0x00, 0x00);  // Only using red
      break;

    case 2:
      theaterChase(0xff, 0x00, 0x00); // Only using red
      break;

    case 3:
      meteorRain(0xff, 0x00, 0x00); // Only using red
      break;

    case 4:
      Sparkle(0xff, 0x00, 0x00, 2); // Only using red
      break;

    default: // Failsafe if the sequence number cannot be read
      fill_solid(leds, NUM_LEDS, CRGB::Red); // Sets all LED's to red.
      FastLED.show();
	    writeAndShift(10); // Displays an 'E' on the 7-seg display to indicate an error
      break;
  }
}
