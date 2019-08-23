#include "led_color.h"

void led_color(byte color_code) {

    // Used the bits of the color code to turn on or off different color leds (Total 8 colors)
    (color_code &  0x1 )  ? digitalWrite(B_LED, LOW) : digitalWrite(B_LED, HIGH);
    (color_code &  0x2 )  ? digitalWrite(G_LED, LOW) : digitalWrite(B_LED, HIGH);
    (color_code &  0x3 )  ? digitalWrite(R_LED, LOW) : digitalWrite(B_LED, HIGH);

}