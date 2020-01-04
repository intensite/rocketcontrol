
#include "correct.h"
#include "../global.h"
#include "../parachute/parachute.h"

void processTrajectory(float ypr[3]) {
    int8_t pos_1;
    int8_t pos_2;

    if(ypr[1] == 0 || ypr[2] ==0) {
        // Data invalid do nothing
        return;
    }

    pos_1 =(int8_t) 90-(ypr[1] * 180/M_PI);
    pos_2 =(int8_t) 90-(ypr[2] * 180/M_PI);

    //TODO: Compare angle to EXCESSIVE_ANGLE_THRESHOLD config constant to abort 
    if(pos_1 < (90 - EXCESSIVE_ANGLE_THRESHOLD) || pos_1 > (90 + EXCESSIVE_ANGLE_THRESHOLD) || 
        pos_2 < (90 - EXCESSIVE_ANGLE_THRESHOLD) || pos_2 > (90 + EXCESSIVE_ANGLE_THRESHOLD)) {
        // Here we initiate the abort sequence
        Serial.println("Excessive angle. ABORT SEQUENCE...........");
        Serial.print("pos_1: ");
        Serial.print(pos_1);
        Serial.print("  pos_2: ");
        Serial.print(pos_2);
        Serial.print("  ypr[1]: ");
        Serial.print(ypr[1]);
        Serial.print("  ypr[2]: ");
        Serial.println(ypr[2]);
        deployParachute();
        //is_abort = true;
    } else {
        moveServo(ypr);
    }
}