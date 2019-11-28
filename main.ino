
#include "src/config.h"
#include "src/global.h"
#include "src/gyro/Cgyro.h"
#include "src/altimeter/altitude.h"
#include "src/servo/fins_servo.h"
#include "src/flight_correct/correct.h"
#include "src/buzzer/buzzer.h"
#include "src/led_color/led_color.h"
// #include "src/storage/Storage.h"
// #include "src/storage/LogSystem.h"
// #include "src/storage/LogSystem_SD.h"
#include "src/parachute/parachute.h"

float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
bool is_abort = false;
bool is_parachute_deployed = false;
int g_servo_pitch = 0;
int g_servo_roll = 0;

// const long interval = 100;
unsigned long previousMillis = 0;
unsigned long previousHBeatMillis = 0;
bool setup_error = false;

Altitude altitude;
Gyro gyro;
// LogSystem log2;
bool ledStatus;

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================
#define MPU_INTERRUPT_PIN 3  // use pin 3 as pin 2 interfere with servo.

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}
 

/*****************************************************************
 *  Used to display sensor data onthe cosole. Mainly to debug.
 *  Should no be used durring flight as it disturbs the interupts of the I2C
 * ***************************************************************/
void displaySensorData() {

        // Debug stuff
        Serial.print(F(" Gyro:"));
        Serial.print(gyro.ypr[1] * 180/M_PI);
        Serial.print(F(" : "));
        Serial.println(gyro.ypr[2] * 180/M_PI);
        
        Serial.print(F("Altitude:"));
        Serial.println(altitude.current_altitude);
} 


void testSequence() {

    Serial.println(F("Begin of tests..........."));
    Serial.println(F("Testing servos..."));
    testServo();

    Serial.println(F("Testing LED and Buzzer..."));

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
    Serial.println(F("End of tests..........."));
}

void debugParachute() {
    byte countdown = 10;
    // Serial.println("Debug mode. Press any key to deploy parachute....................");
    // while(Serial.available() == 0) { }  // There really is nothing between the {} braces

    while(countdown >=0) {
        delay(1000);
        buzz(PIEZO_BUZZER, 2637, 1000/12);
        countdown--;
    }

    buzz(PIEZO_BUZZER, 2637, 1000/12);
    buzz(PIEZO_BUZZER, 2637, 1000/12);
    buzz(PIEZO_BUZZER, 2637, 1000/12);
    buzz(PIEZO_BUZZER, 2637, 1000/12);
    buzz(PIEZO_BUZZER, 2637, 10000/12);
    char x = Serial.read();
    deployParachute();
    is_abort = true;
    is_parachute_deployed = true;
    return;
}

int8_t persistData() {

    if(gyro.ypr[1] == 0 || gyro.ypr[2] ==0) {
        // Data invalid do nothing
        return 1;
    }

    //// lr::LogRecord logRecord(
    // LogRecord logRecord(
    //     millis(), 
    //     altitude.current_altitude, 
    //     (int) (gyro.ypr[1] * 180/M_PI),  // Pitch: Must be improved
    //     (int) (gyro.ypr[2] * 180/M_PI),  // Roll:  Must be improved
    //     g_servo_pitch, // Servo Pitch: ToDo
    //     g_servo_roll, // Servo Roll : ToDo
    //     is_parachute_deployed, 
    //     is_abort, 
    //     altitude.temperature, // Temperature
    //     72, // Batt
    //     gyro.z_gforce  // gForces
    // );
    // if (!lr::LogSystem::appendRecord(logRecord)) {
    //     Serial.println("Probleme de storrage: verifier memoire pleine");
    //     return 0;
    // } else {
    //     Serial.println("Record saved: ");
    // }
    return 1;
}

// void readData() {
//     int reccount = 0;
//     // if (reccount = lr::LogSystem::currentNumberOfRecords()) {
//     //     for(int i = 0; i < reccount; i++) {
//     //         lr::LogRecord logRecord = lr::LogSystem::getLogRecord(i);
//     //         logRecord.writeToSerial();
//     //     }
//     // }
// }

void setup() {
    is_abort = false;
    is_parachute_deployed = false;
    g_servo_pitch = 0;
    g_servo_roll = 0;

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
    //     Serial.println(F("Problem with Gyroscope not detected..."));
    //     return;
    // }
    
    if (altitude.setupAlti() !=0) {
        setup_error = true;
        // LED RED
        led_color(LED_COLOR_RED);
        is_abort = true;
        Serial.println(F("Problem with altitmeter not detected..."));
        return;
    }

    //Storage system initialization
    Serial.println(F("Initialize the log system"));
    //// FRAM LOG SYSTEM
    // if (!lr::Storage::begin()) {
    //     Serial.println("Storage Problem");
    //     is_abort = true;
    //     return;
    // } else {
    //     lr::LogSystem::begin(0);  
    //     Serial.println("Storage seems OK");
    // }

    //// SD CARD LOG SYSTEM
    // if (!log2.begin()) {
    //     Serial.println(F("Storage Problem"));
    //     is_abort = true;
    //     return;
    // } else {
    //     Serial.println(F("Storage seems OK"));
    // }


    // End of Storage system initialization

    if (DATA_RECOVERY_MODE == 1) {
        Serial.println(F("Data recovery mode detected.  Reading memory...."));
        //readData();
        Serial.println(F("Data recovery completed...."));
        return;
    }

    // if(FORMAT_MEMORY == 1){
    //     Serial.println("Erassing memory....");
    //     lr::LogSystem::format();
    // }

    testSequence();
    //debugParachute();
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

    // In DATA_RECOVERY_MODE exit the main loop
    if (DATA_RECOVERY_MODE == 1)
        return;

   unsigned long currentMillis = millis();
    
    // The abort sequence was triggered (throw your arms in the air) exit the main loop
    if (is_abort) {
        return;
    }

    // Some data from the Gyroscope is ready to be processed
    if(mpuInterrupt) {
        mpuInterrupt = false;
        gyro.ProcessGyroData();
    }
   
    // Used to slowdown the process to the number of milliseconds defined in variable interval (see config.h file)
     if (currentMillis - previousMillis >= SCAN_TIME_INTERVAL) {

        previousMillis = currentMillis; 

        // Read the altitude and process trajectory with the servos
        altitude.processAltiData();
        processTrajectory(gyro.ypr);

        // Debug stuff
        if (DEBUG) 
            displaySensorData();
    
        // Persist flight data to memory
        // persistData();
    
        heartBeat();
    }
   }

