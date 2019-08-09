
#include "src/config.h"
// #include "src/gyro/gyro.h"
#include "src/gyro/Cgyro.h"
#include "src/altimeter/altitude.h"
#include "src/servo/fins_servo.h"
#include "src/flight_correct/correct.h"

float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
bool is_abort = false;
const long interval = 100;
unsigned long previousMillis = 0;

Altitude altitude;
Gyro gyro;
bool ledStatus;

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================
#define MPU_INTERRUPT_PIN 3  // use pin 3 as pin 2 interfere with servo.

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}
 

void displaySensorData() {

            // Debug stuff
        Serial.print(" Gyro:");
        Serial.print(gyro.ypr[1] * 180/M_PI);
        Serial.print(" : ");
        Serial.println(gyro.ypr[2] * 180/M_PI);
        
        Serial.print("Altitude:");
        Serial.println(altitude.current_altitude);
        previousMillis = previousMillis+interval; 

} 

void setup() {
    // initialize serial communication
    Serial.begin(38400);  // Reduced the speed as it was crashing the arduino at 115200
    pinMode(MPU_INTERRUPT_PIN, INPUT_PULLUP);
    EIFR = (1 << INTF1);
    attachInterrupt(digitalPinToInterrupt(MPU_INTERRUPT_PIN), dmpDataReady, RISING);

    pinMode(12, OUTPUT);
    ledStatus = LOW;

    gyro.setupGyro();
    altitude.setupAlti();
    setupServo();
}

void loop() {
   unsigned long currentMillis = millis();
    
    if(mpuInterrupt) {
        mpuInterrupt = false;
        gyro.ProcessGyroData();
    }
   
     if (currentMillis - previousMillis >= interval) {

        altitude.processAltiData();
        processTrajectory(gyro.ypr);

        if(ledStatus == LOW) {
            ledStatus = HIGH;
        } else {
            ledStatus = LOW;
        }
        digitalWrite(12, ledStatus);

        // Debug stuff
        if (DEBUG)
            displaySensorData();
    }
   }

