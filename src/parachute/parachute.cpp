#include <Arduino.h>
#include "parachute.h"
#include "../config.h"
#include "../led_color/led_color.h"

bool deployParachute(void) {


    //TODO: Change parachute deployment logic to use one of the 4 customizable pyro channel 

    Serial.println("POP!!  Parachute deployed");
    
    digitalWrite(PARACHUTE_IGNITER_PIN, true);
    led_color(LED_COLOR_YELLOW);
    return true;
}
