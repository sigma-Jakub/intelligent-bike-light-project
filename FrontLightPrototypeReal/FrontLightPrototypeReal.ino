#include <Adafruit_NeoPixel.h>
// start lower
// and gradually higher.
// 

#define LDR_PIN A3
#define RING_PIN 4
#define NUM_PIXELS 12

const int LIGHT_THRESHOLD = 700; // change to 500 if it doesn't work

const int DARK_VALUE = 80;
const int BRIGHT_VALUE = 950;

Adafruit_NeoPixel ring(
    NUM_PIXELS,
    RING_PIN,
    NEO_GRB + NEO_KHZ800
);

int lightValue = 0;
bool lightEnabled = false;
int brightness = 0;

void setup()
{
    Serial.begin(9600);

    ring.begin();
    ring.show();
}

void loop()
{
    readLightSensor();

    updateLightState();

    updateBrightness();

    updateFrontLight();
}

void readLightSensor()
{
    lightValue = analogRead(LDR_PIN);
}

void updateLightState()
{
    lightEnabled = (lightValue < LIGHT_THRESHOLD);
}

void updateBrightness()
{
    if(!lightEnabled)
    {
        brightness = 0;
        return;
    }

    /*brightness = map(
        lightValue,
        DARK_VALUE,
        BRIGHT_VALUE,
        255,
        80
    );*/

    // ALTERNATIVE for the adaptive brightness thing: because brightness is only used when < 500. map above, values > 500 can't occur.

    brightness = map(
        lightValue,
        80,
        LIGHT_THRESHOLD,
        255,
        80
    );


    brightness = constrain(
        brightness,
        80,
        255
    );
}

void updateFrontLight()
{
    if(!lightEnabled)
    {
        ring.clear();
        ring.show();
        return;
    }

    ring.setBrightness(brightness);

    for(int i = 0; i < NUM_PIXELS; i++)
    {
        ring.setPixelColor(
            i,
            ring.Color(255,255,255)
        );
    }

    ring.show();
}