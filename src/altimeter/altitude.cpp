
/* ====================================================================
 * 2019 Stephen Remillard / Francois Paquette
 * based on the excelent I2Cdev device library code from Jeff Rowberg
 * available at https://github.com/jrowberg/i2cdevlib
 ====================================================================== */


#include "../config.h"
#include "altitude.h"
#include "I2Cdev.h"
#include "../parachute/parachute.h"


// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif


// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

Altitude::Altitude() {};

int Altitude::setupAlti() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    altitude_max = 0; previous_altitude = 0; is_apogee = false;

    // initialize device
    myPressure.begin(); // Get sensor online

    //Configure the sensor
    myPressure.setModeAltimeter(); // Measure altitude above sea level in meters

    myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
    myPressure.enableEventFlags(); // Enable all three pressure and temp event flags 
    altitude_offset = myPressure.readAltitude();
    Serial.print("Altitude ofset = "); Serial.println(altitude_offset); 
    if (altitude_offset == -999) {
        return -999;  // Error out after max of 512ms for a read
    } else {
        return 0;
    }


}



float Altitude::processAltiData() {

    // Get the current altitude using the altitude_offset
    current_altitude = myPressure.readAltitude() - altitude_offset;

    if (current_altitude < 0) {
        current_altitude = 0;
    }

    if (current_altitude > altitude_max){
        altitude_max = current_altitude;
    }

    if(altitude_max > APOGEE_DIFF_METERS) {
        if((current_altitude - APOGEE_DIFF_METERS) < altitude_max) {
            Serial.print("Apogee passed. Max altitude: ");
            Serial.println(altitude_max);
            deployParachute();
        }
    }

    previous_altitude = current_altitude;
    return current_altitude;

}