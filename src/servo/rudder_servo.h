
#include "Arduino.h"
#include "Servo.h"

#define SERVO_PIN 6 // (PWM Pin to control the servo)

void moveServo(float _ypr[]);
void setupServo();