#include "fins_servo.h"


Servo servo_1; 
Servo servo_2; 

void setupServo() {
    servo_1.attach(FINS_SERVO_1_PIN);  // attaches the servo pin
    servo_1.write(90);                  
    servo_2.attach(FINS_SERVO_2_PIN);  // attaches the servo on pin 9 to the servo object
    servo_2.write(90);  
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
    servo_2.write(90);   
}

void moveServo(float _ypr[]) {
//ypr[0] * 180/M_PI

    uint8_t pos_1;
    uint8_t pos_2;

    pos_1 =(uint8_t) 90-(_ypr[1] * 180/M_PI);
    pos_1 = ((pos_1 < 5) ? 5 : pos_1);
    pos_1 = ((pos_1 >= 175) ? 175 : pos_1);

    pos_2 =(uint8_t) 90-(_ypr[2] * 180/M_PI);
    pos_2 = ((pos_2 < 5) ? 5 : pos_2);
    pos_2 = ((pos_2 >= 175) ? 175 : pos_2);

    // Serial.print(" servo:");
    // Serial.print(pos_1);
    servo_1.write(pos_1);  
    servo_2.write(pos_2);  

}

//@TODO: Make a PID control loop
