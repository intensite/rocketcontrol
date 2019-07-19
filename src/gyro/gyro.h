// #include "I2Cdev.h"
// #include "MPU6050_6Axis_MotionApps20.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#define OUTPUT_READABLE_YAWPITCHROLL
//#define OUTPUT_READABLE_REALACCEL
//#define OUTPUT_READABLE_WORLDACCEL


// CALIBRATION CONSTANTS CHANGE TO CALIBRATE
#define X_GYRO_OFFSETS 5
#define Y_GYRO_OFFSETS 41
#define Z_GYRO_OFFSETS 57
#define X_ACCEL_OFFSETS -1198
#define Y_ACCEL_OFFSETS 97
#define Z_ACCEL_OFFSETS 1752
    
void setupGyro();
void ProcessGyroData(float ypr[3]);

