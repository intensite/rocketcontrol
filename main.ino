
#include "src/config.h"
#include "src/global.h"
#include "src/gyro/Cgyro.h"
#include "src/altimeter/altitude.h"
#include "src/servo/fins_servo.h"
#include "src/flight_correct/correct.h"
#include "src/buzzer/buzzer.h"
#include "src/led_color/led_color.h"

float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
bool is_abort = false;
bool is_parachute_deployed = false;

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


void testSequence() {

    Serial.println("Begin of tests...........");

    Serial.println("Testing servos...");
    testServo();

    Serial.println("Testing LED and Buzzer...");

    // Test LED 
    led_color(LED_COLOR_RED);
    delay(500);               
    led_color(LED_COLOR_GREEN);
    delay(500);               
    led_color(LED_COLOR_BLUE);
    delay(500);               
    led_color(LED_COLOR_OFF);

    // Test PIEZO_BUZZER 
    buzz(PIEZO_BUZZER, 2637, 1000/12);
    buzz(PIEZO_BUZZER, 3136, 1000/12);
    buzz(PIEZO_BUZZER, 2093, 1000/12);
    buzz(PIEZO_BUZZER, 0, 1000/12);
    Serial.println("End of tests...........");
}

void setup() {
    is_abort = false;
    is_parachute_deployed = false;

    // initialize serial communication
    Serial.begin(38400);  // Reduced the speed as it was crashing the arduino at 115200
    delay(2000);                // waits for two second
    pinMode(R_LED, OUTPUT);     digitalWrite(R_LED, HIGH);
    pinMode(G_LED, OUTPUT);     digitalWrite(G_LED, HIGH);
    pinMode(B_LED, OUTPUT);     digitalWrite(B_LED, HIGH);
    pinMode(PIEZO_BUZZER, OUTPUT);
    pinMode(PARACHUTE_IGNITER_PIN, OUTPUT); digitalWrite(PARACHUTE_IGNITER_PIN, LOW);

    pinMode(MPU_INTERRUPT_PIN, INPUT_PULLUP);
    EIFR = (1 << INTF1);
    attachInterrupt(digitalPinToInterrupt(MPU_INTERRUPT_PIN), dmpDataReady, RISING);

    ledStatus = LOW;

    setupServo();

    // if (gyro.setupGyro() != 0) {
    //     setup_error = true;
    //     // LED RED
    //     led_color(LED_COLOR_RED);
    //     is_abort = true;
    //     Serial.println("Problem with Gyroscope not detected...");
    //     return;
    // }
    if (altitude.setupAlti() !=0) {
        setup_error = true;
        // LED RED
        led_color(LED_COLOR_RED);
        is_abort = true;
        Serial.println("Problem with altitmeter not detected...");
        return;
    }

    testSequence();
}

void heartBeat() {
    unsigned long currentHBeatMillis = millis();

    if (setup_error || is_abort) {
        // LED RED
        digitalWrite(R_LED, LOW);
        digitalWrite(G_LED, HIGH);
        digitalWrite(B_LED, HIGH);
        return;
    }

    // Flash Green LED every seconds
    if (currentHBeatMillis - previousHBeatMillis >= 1000) {
        if(ledStatus == LOW) {
            ledStatus = HIGH;
        } else {
            ledStatus = LOW;
        }
        digitalWrite(R_LED, HIGH); // High == OFF
        digitalWrite(G_LED, ledStatus);
        digitalWrite(B_LED, HIGH); 
    }

    // Beep sequence
    if (currentHBeatMillis - previousHBeatMillis >= 2000) {
        previousHBeatMillis = currentHBeatMillis;

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

