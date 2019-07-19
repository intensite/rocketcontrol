
#include "src/config.h"
#include "src/gyro/gyro.h"
#include "src/servo/fins_servo.h"
#include "src/flight_correct/correct.h"

float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================
#define MPU_INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define ALTI_INTERRUPT_PIN 3  // use pin 3 on Arduino Uno & most boards

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}
 

void setup() {
    // initialize serial communication
    Serial.begin(115200);
    pinMode(MPU_INTERRUPT_PIN, INPUT);

    attachInterrupt(digitalPinToInterrupt(MPU_INTERRUPT_PIN), dmpDataReady, RISING);

    setupGyro();
    setupServo();
    
}

void loop() {
    if(mpuInterrupt) {
        mpuInterrupt = false;
        ProcessGyroData(ypr);
    }

    processTrajectory(ypr);
    Serial.println(ypr[1] * 180/M_PI);

}