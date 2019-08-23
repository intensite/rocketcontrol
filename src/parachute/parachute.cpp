#include <Arduino.h>
#include "parachute.h"
#include "../config.h"
#include "../led_color/led_color.h"

bool deployParachute(void) {


    //TODO: Put parachute deployment code here!

    Serial.println("POP!!  Parachute deployed");
    digitalWrite(PARACHUTE_IGNITER_PIN, true);
    led_color(LED_COLOR_YELLOW);
    return true;
}
