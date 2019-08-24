
#include "correct.h"
#include "../global.h"

void processTrajectory(float ypr[3]) {
    uint8_t pos_1;
    uint8_t pos_2;

    pos_1 =(uint8_t) 90-(ypr[1] * 180/M_PI);
    pos_2 =(uint8_t) 90-(ypr[2] * 180/M_PI);

    //TODO: Compare angle to EXCESSIVE_ANGLE_THRESHOLD config constant to abort 
    if(pos_1 < (90 - EXCESSIVE_ANGLE_THRESHOLD) || pos_1 > (90 + EXCESSIVE_ANGLE_THRESHOLD) || 
        pos_2 < (90 - EXCESSIVE_ANGLE_THRESHOLD) || pos_2 > (90 + EXCESSIVE_ANGLE_THRESHOLD)) {
        // Here we initiate the abort sequence
        //is_abort = true;
        Serial.println("Excessive angle. ABORT SEQUENCE...........");
    } else {
        moveServo(ypr);
    }
}