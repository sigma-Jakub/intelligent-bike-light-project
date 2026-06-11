#include <Wire.h>
#include <VL53L0X.h>
#include <Servo.h>

VL53L0X distanceSensor;
Servo lightServo;

const int servoPin = 9;


const int maxDetectDistance = 1500;   
const int minDetectDistance = 500;    

//Servo Angles
const int normalAngle = 90; // Normal light position
const int loweredAngle = 120; // Lowered light position

int currentServoAngle = normalAngle;
int targetServoAngle = normalAngle;

//Timing
unsigned long lastSensorUpdate = 0;
const unsigned long sensorUpdateInterval = 100; // every 100 ms

unsigned long lastServoUpdate = 0;
const unsigned long servoUpdateInterval = 20;// smooth servo movement

void setup() {
  Serial.begin(9600);

  setupDistanceServo();

  Serial.println("Distance sensor and servo ready");
}
//Setup Function
void setupDistanceServo() {
  Wire.begin();

  distanceSensor.init();
  distanceSensor.setTimeout(500);





 
  distanceSensor.setMeasurementTimingBudget(200000);

  distanceSensor.startContinuous();

  lightServo.attach(servoPin);
  lightServo.write(normalAngle);
}

//Main Update Function
void updateDistanceServo() {
  updateDistanceTargetAngle();
  smoothServoMovement();
}

//Read Sensor and Calculate Target Angle
void updateDistanceTargetAngle() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastSensorUpdate < sensorUpdateInterval) {
    return;
  }

  lastSensorUpdate = currentMillis;

  int distance = distanceSensor.readRangeContinuousMillimeters();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" mm");

  if (distanceSensor.timeoutOccurred()) {
    targetServoAngle = normalAngle;
    return;
  }

  // If no object is detected or object is too far, keep light normal
  if (distance <= 0 || distance > maxDetectDistance) {
    targetServoAngle = normalAngle;
    return;
  }

  // If object is very close, fully lower the light
  if (distance < minDetectDistance) {
    targetServoAngle = loweredAngle;
    return;
  }






  
  targetServoAngle = map(distance,
                         maxDetectDistance,
                         minDetectDistance,
                         normalAngle,
                         loweredAngle);

  targetServoAngle = constrain(targetServoAngle, normalAngle, loweredAngle);

  Serial.print("Target angle: ");
  Serial.println(targetServoAngle);
}

//Smooth Servo Movement
void smoothServoMovement() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastServoUpdate < servoUpdateInterval) {
    return;
  }

  lastServoUpdate = currentMillis;

  if (currentServoAngle < targetServoAngle) {
    currentServoAngle++;
  } 
  else if (currentServoAngle > targetServoAngle) {
    currentServoAngle--;
  }

  lightServo.write(currentServoAngle);
}
