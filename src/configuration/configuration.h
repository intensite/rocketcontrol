#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include "Arduino.h"
class Configuration {

   // Private constructor, to forbid the construction from outside this class.
   Configuration ();
   //Non-copyable (such that another instance cannot be created)
   Configuration(const Configuration&);
   //Non-assignable for the same reason.
   Configuration& operator=(const Configuration&);
public:

    uint8_t DEBUG; // 1                                          // Set to 1 to read collected data from memory: 0 to save data to memory
    uint8_t BUZZER_ENABLE; // 0                          // Set to 1 to enable the buzzer. Set to 0 otherwise.
    uint8_t MEMORY_CARD_ENABLED; // 1              // Set to 1 to activate the logging system.  0 to disable it (for testing)
    uint8_t DATA_RECOVERY_MODE; // 1                // Set to 1 to read collected data from memory: 0 to save data to memory
    uint8_t FORMAT_MEMORY; // 0                          // Set to 1 to erase memory.
    uint8_t APOGEE_DIFF_METERS; // 10
    uint8_t EXCESSIVE_ANGLE_THRESHOLD;
    uint16_t SCAN_TIME_INTERVAL;
    uint16_t PARACHUTE_DELAY;
    uint8_t PITCH_AXIS;
    uint8_t YAW_AXIS;
    uint8_t ROLL_AXIS;
    int8_t SERVO_1_OFFSET;
    int8_t SERVO_2_OFFSET;
    int8_t SERVO_1_ORIENTATION;
    int8_t SERVO_2_ORIENTATION;
    uint8_t MAX_FINS_TRAVEL;
    float PID_PITCH_Kp;
    float PID_PITCH_Ki;
    float PID_PITCH_Kd;
    float PID_ROLL_Kp;
    float PID_ROLL_Ki;
    float PID_ROLL_Kd;
    int16_t X_GYRO_OFFSETS;
    int16_t Y_GYRO_OFFSETS;
    int16_t Z_GYRO_OFFSETS;
    int16_t X_ACCEL_OFFSETS;
    int16_t Y_ACCEL_OFFSETS;
    int16_t Z_ACCEL_OFFSETS;



   //Static member function that returns the instance of the singleton by reference.
   static Configuration& instance(); 
   bool readConfig();
   int saveConfig();
};

#define _CONF Configuration::instance()
#endif

