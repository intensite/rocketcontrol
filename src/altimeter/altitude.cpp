/* ====================================================================
 * 2019 Stephen Remillard / Francois Paquette
 * based on the excelent I2Cdev device library code from Jeff Rowberg
 * available at https://github.com/jrowberg/i2cdevlib
 ====================================================================== */
/* ==========================================================================
 * I2Cdev device library code is placed under the MIT license
 * Copyright (c) 2012 Jeff Rowberg 
 * MIT license detaild snipped *
 =============================================================================*/

#include "alti.h"

#include "I2Cdev.h"
#include "SparkFunMPL3115A2.h"

MPL3115A2 myPressure;

int altitude_max;
float altitude_offset=0;

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================
void setupAlti() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize device
    myPressure.begin(); // Get sensor online

    //Configure the sensor
    myPressure.setModeAltimeter(); // Measure altitude above sea level in meters

    myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
    myPressure.enableEventFlags(); // Enable all three pressure and temp event flags 
    altitude_offset = myPressure.readAltitude();

}



float ProcessAltiData() {

    float alti = myPressure.readAltitude() - altitude_offset;

    if (alti < 0) {
        alti = 0;
    }
    // Serial.print("Altitude(m):");
    // Serial.print(altitude);
    // Serial.println();

    // if (altitude > altitude_max){
    //     altitude_max = altitude;
    // }
    // int display_altitude_max = digitalRead(4);
    // if (display_altitude_max == HIGH){
    //     altitude=altitude_max;
    // }

    return alti;

}