
#include "src/config.h"
// #include "src/gyro/gyro.h"
#include "src/gyro/Cgyro.h"
#include "src/altimeter/altitude.h"
#include "src/servo/fins_servo.h"
#include "src/flight_correct/correct.h"
#include "src/buzzer/buzzer.h"

float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
bool is_abort = false;
const long interval = 100;
unsigned long previousMillis = 0;
unsigned long previousHBeatMillis = 0;
int setup_error = false;

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
        

} 

void setup() {
    // initialize serial communication
    Serial.begin(38400);  // Reduced the speed as it was crashing the arduino at 115200
    delay(3000);                // waits for two second
    pinMode(R_LED, OUTPUT);
    pinMode(G_LED, OUTPUT);
    pinMode(B_LED, OUTPUT);
    pinMode(PIEZO_BUZZER, OUTPUT);

    pinMode(MPU_INTERRUPT_PIN, INPUT_PULLUP);
    EIFR = (1 << INTF1);
    attachInterrupt(digitalPinToInterrupt(MPU_INTERRUPT_PIN), dmpDataReady, RISING);

    pinMode(12, OUTPUT);
    ledStatus = LOW;

    if (gyro.setupGyro() != 0) {
        // setup_error = true;
        // // LED RED
        // digitalWrite(R_LED, LOW);
        // digitalWrite(G_LED, HIGH);
        // digitalWrite(B_LED, HIGH);
        // is_abort = true;
        // Serial.println("Problem with Gyroscope detected...");
        // // return;
    }
    if (altitude.setupAlti() !=0) {
        setup_error = true;
        // LED RED
        digitalWrite(R_LED, LOW);
        digitalWrite(G_LED, HIGH);
        digitalWrite(B_LED, HIGH);
        is_abort = true;
        Serial.println("Problem with altitude detected...");
        return;
    }
    setupServo();


    Serial.println("Begin of tests...........");

    testServo();

    // Test LED 
    digitalWrite(R_LED, LOW);
    digitalWrite(G_LED, HIGH);
    digitalWrite(B_LED, HIGH);
    delay(500);                // waits for two second
    digitalWrite(R_LED, HIGH);
    digitalWrite(G_LED, LOW);
    digitalWrite(B_LED, HIGH); 
    delay(500);                // waits for two second
    digitalWrite(R_LED, HIGH);
    digitalWrite(G_LED, HIGH);
    digitalWrite(B_LED, LOW);
    delay(500);                // waits for two second
    digitalWrite(B_LED, HIGH);

    // Test PIEZO_BUZZER 
    tone(PIEZO_BUZZER, 900);         // Send 1KHz sound signal...
    delay(500);                 // waits for two second
    tone(PIEZO_BUZZER, 2000);         // Send 1KHz sound signal...
    delay(500);                 // waits for two second
    tone(PIEZO_BUZZER, 900);         // Send 1KHz sound signal...
    delay(500);                 // waits for two second
    tone(PIEZO_BUZZER, 3000);         // Send 1KHz sound signal...
    delay(500);                 // waits for two second
    noTone(PIEZO_BUZZER);             // Stop sound...
    Serial.println("End of tests...........");
}

void heartBeat() {
    unsigned long currentMillis = millis();

    if (setup_error || is_abort) {
        // LED RED
        digitalWrite(R_LED, LOW);
        digitalWrite(G_LED, HIGH);
        digitalWrite(B_LED, HIGH);
        return;
    }

        // Flash Green LED
        if(ledStatus == LOW) {
            ledStatus = HIGH;
        } else {
            ledStatus = LOW;
        }

        // digitalWrite(R_LED, HIGH); // High == OFF
        // digitalWrite(G_LED, ledStatus);
        // digitalWrite(B_LED, HIGH); 

    if (currentMillis - previousHBeatMillis >= 2000) {


        previousHBeatMillis = currentMillis;

        if(BUZZER_ENABLE) {
            buzz(PIEZO_BUZZER, 2637, 1000/12);
            buzz(PIEZO_BUZZER, 3136, 1000/12);
            buzz(PIEZO_BUZZER, 2093, 1000/12);
            buzz(PIEZO_BUZZER, 0, 1000/12);
        }
    }


}

void loop() {
   unsigned long currentMillis = millis();
    
    if (is_abort) {
        return;
    }


    if(mpuInterrupt) {
        mpuInterrupt = false;
        gyro.ProcessGyroData();
    }
   
     if (currentMillis - previousMillis >= interval) {

        previousMillis = currentMillis; 

        altitude.processAltiData();
        processTrajectory(gyro.ypr);

        // Debug stuff
        if (DEBUG)
            displaySensorData();
    
    
        heartBeat();
    }
   }

