#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Digital IO pin connected to the button. This will be driven with a
// pull-up resistor so the switch pulls the pin to ground momentarily.
// On a high -> low transition the button press logic will execute.
#define BUTTON_PIN   5

#define PIXEL_PIN_LEFT    3 // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 8 // Number of NeoPixels

// Declare our NeoPixel strip object:
Adafruit_NeoPixel stripL(PIXEL_COUNT, PIXEL_PIN_LEFT, NEO_GRB + NEO_KHZ800);

boolean oldState = HIGH;
int     mode     = 0;    // Currently-active animation mode, 0-9

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  stripL.begin(); // Initialize NeoPixel strip object 
  stripL.show();  // Initialize all pixels to 'off'
  stripL.setBrightness(50);   // 50 = 20% brightness
}

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 20;
unsigned long lastSwitchTime = 0;

void loop() {
  
  // Get current button state.
  boolean newState = digitalRead(BUTTON_PIN);

  // Check if state changed from high to low (button press).
  if((newState == LOW) && (oldState == HIGH)) {
    lastDebounceTime = millis();
  }
  
  // Check if debounce delay has passed since button press.
  if((millis() - lastDebounceTime) > debounceDelay) {
    newState = digitalRead(BUTTON_PIN);
    if(newState == LOW && (millis() - lastSwitchTime) > 500) {  // Debounced and throttled
      lastSwitchTime = millis();
      if(++mode > 2) mode = 0; // Advance to next mode, wrap around after #2
      switch(mode) {           // Start the new animation...
        case 0:
          colorWipe(stripL.Color(  0,   0,   0), 10);    // Black/off
          break;
        case 1:
          colorWipe(stripL.Color(0,   0,   255), 10);   
          break;
        case 2:
          mode = 0;
          colorWipe(stripL.Color(  0,   0,   0), 10);    // Go back to off
          break;
      }
    }
  }

  // Set the last-read button state to the old state.
  oldState = newState;
}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<stripL.numPixels(); i++) { // For each pixel in strip...
    stripL.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    stripL.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}