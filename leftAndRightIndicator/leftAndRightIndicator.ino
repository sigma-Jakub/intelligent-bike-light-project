#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

// Buttons
#define BUTTON_PIN_LEFT 5
#define BUTTON_PIN_RIGHT 6

// LED Strips
#define PIXEL_PIN_LEFT 3
#define PIXEL_PIN_RIGHT 2
#define PIXEL_COUNT 8

Adafruit_NeoPixel stripL(PIXEL_COUNT, PIXEL_PIN_LEFT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripR(PIXEL_COUNT, PIXEL_PIN_RIGHT, NEO_GRB + NEO_KHZ800);

// LEFT state
boolean oldStateL = HIGH;
unsigned long lastDebounceTimeL = 0;
unsigned long lastSwitchTimeL = 0;
bool blinkingL = false;
unsigned long blinkTimerL = 0;
int modeL = 0;

// RIGHT state
boolean oldStateR = HIGH;
unsigned long lastDebounceTimeR = 0;
unsigned long lastSwitchTimeR = 0;
bool blinkingR = false;
unsigned long blinkTimerR = 0;
int modeR = 0;

unsigned long debounceDelay = 20;
unsigned long timeBetween = 500;

void setup() {
  pinMode(BUTTON_PIN_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_PIN_RIGHT, INPUT_PULLUP);

  stripL.begin();
  stripL.show();
  stripL.setBrightness(100);

  stripR.begin();
  stripR.show();
  stripR.setBrightness(100);
}

void loop() {
  readLeftButton();
  readRightButton();

  leftIndicator();
  rightIndicator();
}

void colorWipe(Adafruit_NeoPixel &strip, uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}

void readLeftButton() {
  boolean newState = digitalRead(BUTTON_PIN_LEFT);

  if (newState == LOW && oldStateL == HIGH)
    lastDebounceTimeL = millis();

  if (millis() - lastDebounceTimeL > debounceDelay) {
    newState = digitalRead(BUTTON_PIN_LEFT);

    if (newState == LOW && millis() - lastSwitchTimeL > 300) {
      lastSwitchTimeL = millis();
      blinkingL = true;
      modeL = 0;
      blinkTimerL = millis();
    }
  }
  oldStateL = newState;
}

void leftIndicator() {
  if (blinkingL && millis() - blinkTimerL >= timeBetween) {
    blinkTimerL = millis();

    switch (modeL) {
      case 0: colorWipe(stripL, stripL.Color(255,50,0), 10); 
      break;
      
      case 1: colorWipe(stripL, stripL.Color(0,0,0), 10); 
      break;
      
      case 2: colorWipe(stripL, stripL.Color(255,50,0), 10); 
      break;
      
      case 3: colorWipe(stripL, stripL.Color(0,0,0), 10); 
      break;
      
      case 4: colorWipe(stripL, stripL.Color(255,50,0), 10); 
      break;
      
      case 5: colorWipe(stripL, stripL.Color(0,0,0), 10); 
      break;
      
      case 6: colorWipe(stripL, stripL.Color(255,50,0), 10); 
      break;
      
      case 7: colorWipe(stripL, stripL.Color(0,0,0), 10); 
      break;
      
      case 8: colorWipe(stripL, stripL.Color(255,50,0), 10); 
      break;
      
      case 9:
        blinkingL = false;
        stripL.clear();
        stripL.show();
        break;
    }
    modeL++;
  }
}

void readRightButton() {
  boolean newState = digitalRead(BUTTON_PIN_RIGHT);

  if (newState == LOW && oldStateR == HIGH)
    lastDebounceTimeR = millis();

  if (millis() - lastDebounceTimeR > debounceDelay) {
    newState = digitalRead(BUTTON_PIN_RIGHT);

    if (newState == LOW && millis() - lastSwitchTimeR > 300) {
      lastSwitchTimeR = millis();
      blinkingR = true;
      modeR = 0;
      blinkTimerR = millis();
    }
  }
  oldStateR = newState;
}

void rightIndicator() {
  if (blinkingR && millis() - blinkTimerR >= timeBetween) {
    blinkTimerR = millis();

    switch (modeR) {
      case 0: colorWipe(stripR, stripR.Color(255,50,0), 10); 
      break;

      case 1: colorWipe(stripR, stripR.Color(0,0,0), 10); 
      break;

      case 2: colorWipe(stripR, stripR.Color(255,50,0), 10); 
      break;
      
      case 3: colorWipe(stripR, stripR.Color(0,0,0), 10); 
      break;

      case 4: colorWipe(stripR, stripR.Color(255,50,0), 10); 
      break;
      
      case 5: colorWipe(stripR, stripR.Color(0,0,0), 10); 
      break;
      
      case 6: colorWipe(stripR, stripR.Color(255,50,0), 10); 
      break;
      
      case 7: colorWipe(stripR, stripR.Color(0,0,0), 10); 
      break;
      
      case 8: colorWipe(stripR, stripR.Color(255,50,0), 10); 
      break;
      
      case 9:
        blinkingR = false;
        stripR.clear();
        stripR.show();
        break;
    }
    modeR++;
  }
}