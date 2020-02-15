/*
Project: Astralis LED Sign  
Created by: Jonas Ibsen
Date: 15th of February 2020

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
#define LATCH 7
#define CLK 6
#define DATA 5

// Lightning sequence button
#define buttonPin 11
int sequenceNumber = 0;
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

// Declaring the bytes transferred to the 74HC595 making up the 7-segment display
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
  B01001111,  // E
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

void FadeInOut(byte red, byte green, byte blue){
  float r, g, b;
     
  for(int k = 0; k < 256; k=k+1) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
     
  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
}

void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position=0;
 
  for(int j=0; j<5; j++)
  {
      Position++; // = 0; //Position + Rate;
      for(int i=0; i<NUM_LEDS; i++) {
        setPixel(i,((sin(i+Position) * 127 + 128)/255)*red,
                   ((sin(i+Position) * 127 + 128)/255)*green,
                   ((sin(i+Position) * 127 + 128)/255)*blue);
      }
      showStrip();
      delay(WaveDelay);
  }
}

void theaterChase(byte red, byte green, byte blue, int SpeedDelay) {
  for (int j=0; j<1; j++) { 
    for (int q=0; q < 3; q++) {
      for (int i=0; i < NUM_LEDS; i=i+3) {
        setPixel(i+q, red, green, blue);    //turn every third pixel on
      }
      showStrip();
      delay(SpeedDelay);
     
      for (int i=0; i < NUM_LEDS; i=i+3) {
        setPixel(i+q, 0,0,0);        //turn every third pixel off
      }
    }
  }
}

void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
  setAll(0,0,0);
 
  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
   
   
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
   
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        setPixel(i-j, red, green, blue);
      }
    }
   
    showStrip();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
   
    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
   
    strip.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}

void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay){

  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
}

void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,red,green,blue);
  showStrip();
  delay(SpeedDelay);
  setPixel(Pixel,0,0,0);
}

// ***********************
// ** LED strip control **
// ***********************

void loop(){

  // LED lightning sequence control
  buttonState = digitalRead(buttonPin);

  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      sequenceNumber++;
    }
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;

  // LED brightness control (Working only for every lighting cycle)
  int brightnessValue = map(analogRead(brightnessPin), 0, 1023, 0, 255);
  FastLED.setBrightness(constrain(brightnessValue, MIN_BRIGHTNESS, MAX_BRIGHTNESS));

  switch(sequenceNumber){

    case 1:
      FadeInOut(0xff, 0x00, 0x00); // Red
      digitalWrite(LATCH, LOW);
      shiftOut(DATA, CLK, MSBFIRST, digit[sequenceNumber]);
      digitalWrite(LATCH, HIGH);
      break;

    case 2:
      RunningLights(0xff, 0x00, 0x00, 50);  // Red
      digitalWrite(LATCH, LOW);
      shiftOut(DATA, CLK, MSBFIRST, digit[sequenceNumber]);
      digitalWrite(LATCH, HIGH);
      break;

    case 3:
      theaterChase(0xff, 0x00, 0x00, 100); // Red
      digitalWrite(LATCH, LOW);
      shiftOut(DATA, CLK, MSBFIRST, digit[sequenceNumber]);
      digitalWrite(LATCH, HIGH);
      break;

    case 4:
      meteorRain(0xff,0x00,0x00,10, 16, false, 20);
      digitalWrite(LATCH, LOW);
      shiftOut(DATA, CLK, MSBFIRST, digit[sequenceNumber]);
      digitalWrite(LATCH, HIGH);
      break;

    case 5:
      CylonBounce(0xff, 0x00, 0x00, 4, 20);
      digitalWrite(LATCH, LOW);
      shiftOut(DATA, CLK, MSBFIRST, digit[sequenceNumber]);
      digitalWrite(LATCH, HIGH);
      break;

    case 6:
      Sparkle(random(0xff), random(0x00), random(0x00), 2);
      digitalWrite(LATCH, LOW);
      shiftOut(DATA, CLK, MSBFIRST, digit[sequenceNumber]);
      digitalWrite(LATCH, HIGH);
      break;

    case 7:
      sequenceNumber = 1; // Resets the sequence number back to one in order to run the code in a cycle
      break;
    
    default: // Failsafe if the sequence number cannot be read
      fill_solid(leds, NUM_LEDS, CRGB::Red); // Sets all LED's to red.
      FastLED.show();
      digitalWrite(LATCH, LOW);
      shiftOut(DATA, CLK, MSBFIRST, digit[10]); // Displays an 'E' on the 7-seg display to indicate an error
      digitalWrite(LATCH, HIGH);
      break;
  }
}