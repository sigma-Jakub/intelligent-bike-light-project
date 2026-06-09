#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

#define BUTTON_PIN 5
#define PIXEL_PIN_LEFT 3
#define PIXEL_COUNT 8

Adafruit_NeoPixel stripL(PIXEL_COUNT, PIXEL_PIN_LEFT, NEO_GRB + NEO_KHZ800);

boolean oldState = HIGH;

unsigned long lastDebounceTime = 0; 
unsigned long debounceDelay = 20; //Ignore any button changes for 20 ms
unsigned long lastSwitchTime = 0; 

bool blinking = false;
unsigned long blinkTimer = 0;
unsigned long timeBetween  = 500; // 0.5 second per step
int mode = 0;


void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  stripL.begin();
  stripL.show();
  stripL.setBrightness(50);
}

void loop() {
  boolean newState = digitalRead(BUTTON_PIN);

  if ((newState == LOW) && (oldState == HIGH)) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) { // ensures button press doesn't intefere
    newState = digitalRead(BUTTON_PIN);

    if (newState == LOW && (millis() - lastSwitchTime) > 300) {
      lastSwitchTime = millis();

      // Start blinking sequence
      blinking = true;
      mode = 0;
      blinkTimer = millis();
    }
  }

  oldState = newState;

  if (blinking) {
    if (millis() - blinkTimer >= timeBetween) {
      blinkTimer = millis();

      switch (mode) {
        case 0:
          colorWipe(stripL.Color(255, 50, 0), 10); //show 1
          break;

        case 1:
          colorWipe(stripL.Color(0, 0, 0), 10); //blank
          break;

        case 2:
          colorWipe(stripL.Color(255, 50, 0), 10); //show 2
          break;

        case 3:
          colorWipe(stripL.Color(0, 0, 0), 10); //blank
          break;
        
        case 4:
          colorWipe(stripL.Color(255, 50, 0), 10); //show 3
          break;

        case 5:
          colorWipe(stripL.Color(0, 0, 0), 10); //blank
          break;
        
        case 6:
          colorWipe(stripL.Color(255, 50, 0), 10); //show 4
          break; 

        case 7:
          colorWipe(stripL.Color(0, 0, 0), 10); //blank
          break;
        
        case 8:
          colorWipe(stripL.Color(255, 50, 0), 10); //show 5
          break; 

        case 9:
          // Blinking finished
          blinking = false;
          stripL.clear();
          stripL.show();
          break;
      }

      mode++;
    }
  }
}

void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < stripL.numPixels(); i++) {
    stripL.setPixelColor(i, color);
    stripL.show();
    delay(wait);
  }
}