#include "fins_servo.h"
#include <PID_v1.h>
#include "../global.h"

Servo servo_1; 
Servo servo_2; 
//Define Variables we'll be connecting to
double Setpoint_Pitch, Input_Pitch, Output_Pitch, Setpoint_Roll, Input_Roll, Output_Roll;

PID pitchPID(&Input_Pitch, &Output_Pitch, &Setpoint_Pitch, PID_PITCH_Kp, PID_PITCH_Ki, PID_PITCH_Kd, DIRECT);
PID rollPID(&Input_Roll, &Output_Roll, &Setpoint_Roll, PID_ROLL_Kp, PID_ROLL_Ki, PID_ROLL_Kd, DIRECT);

void setupServo() {
    servo_1.attach(FINS_SERVO_1_PIN);  // attaches the servo pin
    servo_1.write(90 + SERVO_1_OFFSET);                  
    servo_2.attach(FINS_SERVO_2_PIN);  // attaches the servo on pin 9 to the servo object
    servo_2.write(90 + SERVO_2_OFFSET);  

    Setpoint_Pitch = 0;
    Setpoint_Roll = 0;

    //Specify the links and initial tuning parameters
    pitchPID.SetOutputLimits(MAX_FINS_TRAVEL *-1, MAX_FINS_TRAVEL);
    rollPID.SetOutputLimits(MAX_FINS_TRAVEL *-1, MAX_FINS_TRAVEL);
    //turn the PID on
    pitchPID.SetMode(AUTOMATIC);
    rollPID.SetMode(AUTOMATIC);
}

void testServo() {
    const int8_t test_amplitude = 30;

    // Test Servo #1
    servo_1.write(90 + test_amplitude);   
    delay(300);
    servo_1.write(90 - test_amplitude);   
    delay(300);
    servo_1.write(90 + SERVO_1_OFFSET);   
    delay(300);

    // Test Servo #2
    servo_2.write(90 + test_amplitude);   
    delay(300);
    servo_2.write(90 - test_amplitude);   
    delay(300);
    servo_2.write(90 + SERVO_2_OFFSET);   
}

void disableServo() {
    servo_1.detach();
    servo_2.detach();
    Serial.println(F("Servos disabled!..."));
}


// NEW PID Controlled Gyro-Servo loop
void moveServo(float _ypr[]) {
//ypr[0] * 180/M_PI


    int pos_1;
    int pos_2;

    if(_ypr[1] == 0 || _ypr[2] ==0) {
        // Data invalid do nothing
        return;
    }

    pos_1 =(int) (_ypr[1] * 180/M_PI);
    pos_2 =(int) (_ypr[2] * 180/M_PI);

    Input_Pitch = pos_1 * SERVO_1_ORIENTATION;
    Input_Roll = pos_2 * SERVO_2_ORIENTATION;

    pitchPID.Compute();
    rollPID.Compute();

    double roll = (Output_Roll + 90 + SERVO_2_OFFSET) ;
    double pitch =(Output_Pitch + 90 + SERVO_1_OFFSET) ;

    Serial.print("pitchPID I: ");
    Serial.print(Input_Pitch);
    Serial.print("  O: ");
    Serial.print(pitch);

    Serial.print("     ------  rollPID I: ");
    Serial.print(Input_Roll);
    Serial.print("  O: ");
    Serial.println(roll);

    servo_2.write(roll); 
    servo_1.write(pitch);  

    g_servo_pitch = (int)Output_Pitch;
    g_servo_roll = (int)Output_Roll;
}

//@TODO: Make a PID control loop
