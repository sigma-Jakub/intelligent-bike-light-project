#include <Adafruit_NeoPixel.h>

// pins
#define LDR_PIN A3
#define RING_PIN 4
#define NUM_PIXELS 12

// NeoPixel Ring
Adafruit_NeoPixel ring(NUM_PIXELS, RING_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  Serial.begin(9600);

  ring.begin();
  ring.show();
}

// read light sensor through analog pin
// print value to serial monitor (tomorrow)
void loop()
{
  int lightValue = analogRead(LDR_PIN);

  Serial.print("LDR: ");
  Serial.println(lightValue);

  delay(500);
}

//--OUTSIDE VALUES--//
// Daylight 950
// Sun is going down, around 9-10pm 470
// dark evening but things are still visible 80
// very dark room / tunnel at night 3
// phone flashlight 800

// WE WILL BE USING DAYLIGHT (950) AND DARK SUNSET (470) FOR NOW. REST ARE ADDITIONAL VALUES.
// Sunset value (470) will determine ON/OFF.
// Additional step, brightness mapping (80 -> 3 range) will determine brightness after the light is already ON.