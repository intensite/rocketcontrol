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
    servo_1.write(90 - test_amplitude);   
    servo_1.write(90);   

    // Test Servo #2
    servo_2.write(90 + test_amplitude);   
    servo_2.write(90 - test_amplitude);   
    servo_2.write(85);   
}

// // OLD Direct Gyro-Servo connection
// void moveServo(float _ypr[]) {
// //ypr[0] * 180/M_PI

//     uint8_t pos_1;
//     uint8_t pos_2;

//     // pos_1 =(uint8_t) 90-((_ypr[1] * 180/M_PI)*-1);
//     pos_1 =(uint8_t) 90-((_ypr[1] * 180/M_PI) * SERVO_1_ORIENTATION);
//     pos_1 = ((pos_1 < 5) ? 5 : pos_1);
//     pos_1 = ((pos_1 >= 175) ? 175 : pos_1);

//     pos_2 =(uint8_t) 90-((_ypr[2] * 180/M_PI)  * SERVO_2_ORIENTATION); 
//     pos_2 = ((pos_2 < 5) ? 5 : pos_2);
//     pos_2 = ((pos_2 >= 175) ? 175 : pos_2);

//     Serial.print("servo-1: ");
//     Serial.print(pos_1);
//     Serial.print("   servo-2: ");
//     Serial.println(pos_2);
    
//     servo_1.write(pos_1);  
//     servo_2.write(pos_2);  
// }



// NEW PID Controlled Gyro-Servo loop
void moveServo(float _ypr[]) {
//ypr[0] * 180/M_PI


    int16_t pos_1;
    int16_t pos_2;

    if(_ypr[PITCH_AXIS] == 0 || _ypr[YAW_AXIS] ==0) {
        // Data invalid do nothing
        return;
    }

    pos_1 =(int16_t) (_ypr[PITCH_AXIS] * 180/M_PI);
    pos_2 =(int16_t) (_ypr[YAW_AXIS] * 180/M_PI);

    Input_Pitch = pos_1;
    Input_Roll = pos_2;

    pitchPID.Compute();
    rollPID.Compute();

    // Serial.print("pitchPID I: ");
    // Serial.print(Input_Pitch);
    // Serial.print("  O: ");
    // Serial.print(Output_Pitch);

    // Serial.print("     ------  rollPID I: ");
    // Serial.print(Input_Roll);
    // Serial.print("  O: ");
    // Serial.println(Output_Roll);

    servo_2.write(Output_Roll + 90 + SERVO_2_OFFSET); 
    servo_1.write(Output_Pitch + 90 +  SERVO_1_OFFSET);  

    g_servo_pitch = (int16_t)Output_Pitch;
    g_servo_roll = (int16_t)Output_Roll;
}

//@TODO: Make a PID control loop
