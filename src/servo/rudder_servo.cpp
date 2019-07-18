#include "rudder_servo.h"

Servo myservo; 

void setupServo() {
    myservo.attach(6);  // attaches the servo on pin 9 to the servo object
    myservo.write(90);  
}

void moveServo(float _ypr[]) {
//ypr[0] * 180/M_PI

    uint8_t pos;

    pos=(uint8_t) 90-(_ypr[1] * 180/M_PI);
    pos = ((pos < 5) ? 5 : pos);
    pos = ((pos >= 180) ? 180 : pos);

    Serial.print(pos);
    myservo.write(pos);  

}

//@TODO: Make a PID control loop
