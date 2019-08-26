#include "fins_servo.h"
#include <PID_v1.h>

Servo servo_1; 
Servo servo_2; 
//Define Variables we'll be connecting to
double Setpoint_Pitch, Input_Pitch, Output_Pitch, Setpoint_Roll, Input_Roll, Output_Roll;

PID pitchPID(&Input_Pitch, &Output_Pitch, &Setpoint_Pitch,2,5,1, DIRECT);
PID rollPID(&Input_Roll, &Output_Roll, &Setpoint_Roll,2,5,1, DIRECT);

void setupServo() {
    servo_1.attach(FINS_SERVO_1_PIN);  // attaches the servo pin
    servo_1.write(90);                  
    servo_2.attach(FINS_SERVO_2_PIN);  // attaches the servo on pin 9 to the servo object
    servo_2.write(90);  

    Setpoint_Pitch = 0;
    Setpoint_Roll = 0;

    //Specify the links and initial tuning parameters
    pitchPID.SetOutputLimits(-90, 90);
    rollPID.SetOutputLimits(-90, 90);
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

// OLD Direct Gyro-Servo connection
void moveServo(float _ypr[]) {
//ypr[0] * 180/M_PI

    uint8_t pos_1;
    uint8_t pos_2;

    pos_1 =(uint8_t) 90-((_ypr[1] * 180/M_PI)*-1);
    pos_1 = ((pos_1 < 5) ? 5 : pos_1);
    pos_1 = ((pos_1 >= 175) ? 175 : pos_1);

    pos_2 =(uint8_t) 90-((_ypr[2] * 180/M_PI));  // Reverse the sign
    pos_2 = ((pos_2 < 5) ? 5 : pos_2);
    pos_2 = ((pos_2 >= 175) ? 175 : pos_2);

    Serial.print("servo-1: ");
    Serial.print(pos_1);
    Serial.print("   servo-2: ");
    Serial.println(pos_2);
    
    servo_1.write(pos_1);  
    servo_2.write(pos_2);  
}





// NEW PID Controlled Gyro-Servo loop
// void moveServo(float _ypr[]) {
// //ypr[0] * 180/M_PI


//     int pos_1;
//     int pos_2;

//     pos_1 =(int) (_ypr[1] * 180/M_PI);
//     pos_2 =(int) (_ypr[2] * 180/M_PI);

//     Input_Pitch = pos_1;
//     Input_Roll = pos_2;

//     pitchPID.Compute();
//     rollPID.Compute();

//     Serial.print("pitchPID I: ");
//     Serial.print(Input_Pitch);
//     Serial.print("  O: ");
//     Serial.println(Output_Pitch);

//     Serial.print("rollhPID I: ");
//     Serial.print(Input_Roll);
//     Serial.print("  O: ");
//     Serial.println(Output_Roll);

 
//     servo_2.write(Output_Roll + 90); 
//     servo_1.write(Output_Pitch + 90);  

// }

//@TODO: Make a PID control loop
