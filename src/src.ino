#include <Wire.h>
#include <VL53L0X.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

const int LDR_PIN = A3;
const int SERVO_PIN = 9;
const int RING_PIN = 4;
const int RING_PIXELS_NUM = 12;
const int BUTTON_PIN_LEFT = 5;
const int BUTTON_PIN_RIGHT = 6;
const int STICK_PIN_LEFT = 3;
const int STICK_PIN_RIGHT = 2;
const int STICK_PIXELS_NUM = 8;
const int LIGHT_COLOR[3] = {255, 255, 255};
const int TURN_SIGNAL_COLOR[3] = {255, 50, 0};

VL53L0X distanceSensor;
Servo lightServo;

Adafruit_NeoPixel ring(RING_PIXELS_NUM, RING_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stickL(STICK_PIXELS_NUM, STICK_PIN_LEFT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stickR(STICK_PIXELS_NUM, STICK_PIN_RIGHT, NEO_GRB + NEO_KHZ800);

const int LIGHT_THRESHOLD = 700;
const int DARK_VALUE = 80;
const int BRIGHT_VALUE = 950;
const int MAX_BRIGHTNESS = 255;
const int MIN_BRIGHTNESS = 80;
const int MAX_DISTANCE_DETECTION = 1500;
const int MIN_DISTANCE_DETECTION = 500;
const int REGULAR_ANGLE = 90;
const int TILTED_ANGLE = 120;
const int BLINK_COUNT = 5;
const unsigned long SENSOR_UPDATE_INTERVAL = 100;
const unsigned long SERVO_UPDATE_INTERVAL = 20;
const unsigned long DEBOUNCE_DELAY = 20;
const unsigned long BUTTON_COOLDOWN = 300;
const unsigned long BLINK_INTERVAL = 500;

int lightValue = 0;
int brightness = 0;
bool lightEnabled = false;
int currentServoAngle = REGULAR_ANGLE;
int targetServoAngle = REGULAR_ANGLE;
unsigned long lastSensorUpdate = 0;
unsigned long lastServoUpdate = 0;

bool leftOldState = HIGH;
unsigned long leftDebounceTime = 0;
unsigned long leftLastSwitchTime = 0;
bool leftBlinking = false;
bool leftIndicatorOn = false;
int leftBlinkCount = 0;
unsigned long leftBlinkTimer = 0;

bool rightOldState = HIGH;
unsigned long rightDebounceTime = 0;
unsigned long rightLastSwitchTime = 0;
bool rightBlinking = false;
bool rightIndicatorOn = false;
int rightBlinkCount = 0;
unsigned long rightBlinkTimer = 0;

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_PIN_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_PIN_RIGHT, INPUT_PULLUP);

  setupServo();
  setupDistanceSensor();

  ring.begin();
  ring.show();

  stickL.begin();
  stickL.show();
  stickL.setBrightness(100);

  stickR.begin();
  stickR.show();
  stickR.setBrightness(100);
}

void loop() {
  readDaytime();
  updateLightState();
  updateBrightness();
  updateFrontLight();
  updateDistanceTargetAngle();
  updateServo();
  updateLeftIndicator();
  updateRightIndicator();
}

void readDaytime() {
  lightValue = analogRead(LDR_PIN);
}

void updateLightState() {
  lightEnabled = (lightValue < LIGHT_THRESHOLD);
}

void updateBrightness() {
  if (!lightEnabled) {
    brightness = 0;
    return;
  }

  brightness = map(lightValue, DARK_VALUE, BRIGHT_VALUE, MAX_BRIGHTNESS, MIN_BRIGHTNESS);
  brightness = constrain(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
}

void updateFrontLight() {
  if (!lightEnabled) {
    ring.clear();
    ring.show();
    return;
  }

  ring.setBrightness(brightness);

  for (int i = 0; i < RING_PIXELS_NUM; i++) {
    ring.setPixelColor(i, ring.Color(LIGHT_COLOR[0], LIGHT_COLOR[1], LIGHT_COLOR[2]));
  }

  ring.show();
}

void setupServo() {
  lightServo.attach(SERVO_PIN);
  lightServo.write(REGULAR_ANGLE);
}

void setupDistanceSensor() {
  Wire.begin();
  distanceSensor.init();
  distanceSensor.setTimeout(500);
  distanceSensor.setMeasurementTimingBudget(200000);
  distanceSensor.startContinuous();
}

void updateDistanceTargetAngle() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastSensorUpdate < SENSOR_UPDATE_INTERVAL) {
    return;
  }

  lastSensorUpdate = currentMillis;

  int distance = distanceSensor.readRangeContinuousMillimeters();

  if (distanceSensor.timeoutOccurred()) {
    targetServoAngle = REGULAR_ANGLE;
    return;
  }

  if (distance <= 0 || distance > MAX_DISTANCE_DETECTION) {
    targetServoAngle = REGULAR_ANGLE;
    return;
  }

  if (distance < MIN_DISTANCE_DETECTION) {
    targetServoAngle = TILTED_ANGLE;
    return;
  }

  targetServoAngle = map(distance, MAX_DISTANCE_DETECTION, MIN_DISTANCE_DETECTION, REGULAR_ANGLE, TILTED_ANGLE);
  targetServoAngle = constrain(targetServoAngle, REGULAR_ANGLE, TILTED_ANGLE);
}

void updateServo() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastServoUpdate < SERVO_UPDATE_INTERVAL) {
    return;
  }

  lastServoUpdate = currentMillis;

  if (currentServoAngle < targetServoAngle) {
    currentServoAngle++;
  } else if (currentServoAngle > targetServoAngle) {
    currentServoAngle--;
  }

  lightServo.write(currentServoAngle);
}

void setStrip(Adafruit_NeoPixel &strip, uint32_t color) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void updateLeftIndicator() {
  bool newState = digitalRead(BUTTON_PIN_LEFT);

  if (newState == LOW && leftOldState == HIGH) {
    leftDebounceTime = millis();
  }

  if (millis() - leftDebounceTime > DEBOUNCE_DELAY) {
    newState = digitalRead(BUTTON_PIN_LEFT);
    if (newState == LOW && millis() - leftLastSwitchTime > BUTTON_COOLDOWN) {
      leftLastSwitchTime = millis();
      leftBlinking = true;
      leftBlinkCount = 0;
      leftIndicatorOn = false;
      leftBlinkTimer = millis() - BLINK_INTERVAL;
    }
  }

  leftOldState = newState;

  if (!leftBlinking) {
    return;
  } 

  if (millis() - leftBlinkTimer >= BLINK_INTERVAL) {
    leftBlinkTimer = millis();
    leftIndicatorOn = !leftIndicatorOn;

    if (leftIndicatorOn) {
      setStrip(stickL, stickL.Color(TURN_SIGNAL_COLOR[0], TURN_SIGNAL_COLOR[1], TURN_SIGNAL_COLOR[2]));
      leftBlinkCount++;
    } else {
      setStrip(stickL, stickL.Color(0, 0, 0));
    }

    if (leftBlinkCount >= BLINK_COUNT && !leftIndicatorOn) {
      leftBlinking = false;
      stickL.clear();
      stickL.show();
    }
  }
}

void updateRightIndicator() {
  bool newState = digitalRead(BUTTON_PIN_RIGHT);

  if (newState == LOW && rightOldState == HIGH) {
    rightDebounceTime = millis();
  }

  if (millis() - rightDebounceTime > DEBOUNCE_DELAY) {
    newState = digitalRead(BUTTON_PIN_RIGHT);
    if (newState == LOW && millis() - rightLastSwitchTime > BUTTON_COOLDOWN) {
      rightLastSwitchTime = millis();
      rightBlinking = true;
      rightBlinkCount = 0;
      rightIndicatorOn = false;
      rightBlinkTimer = millis() - BLINK_INTERVAL;
    }
  }

  rightOldState = newState;

  if (!rightBlinking) {
    return;
  }

  if (millis() - rightBlinkTimer >= BLINK_INTERVAL) {
    rightBlinkTimer = millis();
    rightIndicatorOn = !rightIndicatorOn;

    if (rightIndicatorOn) {
      setStrip(stickR, stickR.Color(TURN_SIGNAL_COLOR[0], TURN_SIGNAL_COLOR[1], TURN_SIGNAL_COLOR[2]));
      rightBlinkCount++;
    } else {
      setStrip(stickR, stickR.Color(0, 0, 0));
    }

    if (rightBlinkCount >= BLINK_COUNT && !rightIndicatorOn) {
      rightBlinking = false;
      stickR.clear();
      stickR.show();
    }
  }
}
