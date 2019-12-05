#include <Arduino.h>
#include "parachute.h"
#include "../config.h"
#include "../led_color/led_color.h"
#include "../servo/fins_servo.h"

bool deployParachute(void) {
    disableServo();             // Disable the servo to reserve maximum for parachute deployment.
    delay(PARACHUTE_DELAY);     // Delay to allow maximum battery power for parachute charge ignition.
    Serial.println(F("Deploying Parachute......."));
    digitalWrite(PARACHUTE_IGNITER_PIN, true);
    Serial.println(F("POP!!  Parachute deployed"));
    led_color(LED_COLOR_YELLOW);
    return true;
}
