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

#include "altitude.h"
#include "../config.h"
// #include "../altitude/altitude.h"

#include "I2Cdev.h"
#include "SparkFunMPL3115A2.h"

MPL3115A2 myPressure;

static float altitude_max;
static float previous_altitude;
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

    altitude_max = 0; previous_altitude = 0;

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

    if (alti > altitude_max){
        altitude_max = alti;
    }

    if(altitude_max > APOGEE_DIFF_METERS) {
        if((alti - APOGEE_DIFF_METERS) < altitude_max) {
            Serial.print("Apogee passed. Max altitude: ");
            Serial.println(altitude_max);
            deployParachute();
        }
    }

    previous_altitude = alti;
    return alti;

}