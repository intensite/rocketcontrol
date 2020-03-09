#include "../config.h"
#include "configuration.h"
#include "Arduino.h"
#include "FS.h"
#include "SPIFFS.h"
#include <ArduinoJson.h>

//define the static member function in the cpp (this is crucial):
static Configuration& instance(); 

/**
 * Default Constructor
 */
Configuration::Configuration() {
    // initialize the properties with some default values
    this->DEBUG = 1;
    this->BUZZER_ENABLE = 0;
    this->MEMORY_CARD_ENABLED = 1;
    this->DATA_RECOVERY_MODE = 1;
    this->FORMAT_MEMORY = 0;

    this->SCAN_TIME_INTERVAL = 100;
    this->APOGEE_DIFF_METERS = 10;
    this->EXCESSIVE_ANGLE_THRESHOLD = 50;
    this->PARACHUTE_DELAY = 1500;

    this->PITCH_AXIS = 2;
    this->YAW_AXIS = 0;
    this->ROLL_AXIS = 1;
    
    this->SERVO_1_OFFSET = -11;
    this->SERVO_2_OFFSET = -2;
    this->SERVO_1_ORIENTATION = -1;
    this->SERVO_2_ORIENTATION = -1;
    this->MAX_FINS_TRAVEL = 15;
    
    this->PID_PITCH_Kp = 2;
    this->PID_PITCH_Ki = 0;
    this->PID_PITCH_Kd = 0.5;
    this->PID_ROLL_Kp = 2;
    this->PID_ROLL_Ki = 0;
    this->PID_ROLL_Kd = 0.5;
    
    this->X_GYRO_OFFSETS = 24;
    this->Y_GYRO_OFFSETS = 43;
    this->Z_GYRO_OFFSETS = 525;
    this->X_ACCEL_OFFSETS = -1109;
    this->Y_ACCEL_OFFSETS = 841;
    this->Z_ACCEL_OFFSETS = 525;
}

Configuration& Configuration::instance() {
  //Create a static-scope instance (initialized only once).
  static Configuration only_copy;
  return only_copy;                  //return by reference.
};

bool Configuration::readConfig() {

   Serial.println("Reading configuration.....") ;
 
    File configFile = SPIFFS.open("/config.json", "r");
    if (!configFile) {
        Serial.println("Failed to open config file");
    return false;
    }

    size_t size = configFile.size();
    if (size > 1024) {
        Serial.println("Config file size is too large");
        return false;
    }

    // Allocate a buffer to store contents of the file.
    std::unique_ptr<char[]> buf(new char[size]);

    // We don't use String here because ArduinoJson library requires the input
    // buffer to be mutable. If you don't use ArduinoJson, you may as well
    // use configFile.readString instead.
    configFile.readBytes(buf.get(), size);
    Serial.println(buf.get());

    const size_t capacity = JSON_OBJECT_SIZE(29) + 490;
    DynamicJsonDocument doc(capacity);

    auto error = deserializeJson(doc, buf.get());
    if (error) {
        Serial.println("Failed to parse config file");
        return false;
    }

    this->DEBUG = doc["DEBUG"]; // 1
    this->BUZZER_ENABLE = doc["BUZZER_ENABLE"]; // 0
    this->MEMORY_CARD_ENABLED = doc["MEMORY_CARD_ENABLED"]; // 1
    this->DATA_RECOVERY_MODE = doc["DATA_RECOVERY_MODE"]; // 1
    this->FORMAT_MEMORY = doc["FORMAT_MEMORY"]; // 0

    this->SCAN_TIME_INTERVAL = doc["SCAN_TIME_INTERVAL"]; // 100
    this->APOGEE_DIFF_METERS = doc["APOGEE_DIFF_METERS"]; // 10
    this->EXCESSIVE_ANGLE_THRESHOLD = doc["EXCESSIVE_ANGLE_THRESHOLD"]; // 50
    this->PARACHUTE_DELAY = doc["PARACHUTE_DELAY"]; // 1500

    this->PITCH_AXIS = doc["PITCH_AXIS"]; // 2
    this->YAW_AXIS = doc["YAW_AXIS"]; // 0
    this->ROLL_AXIS = doc["ROLL_AXIS"]; // 1
    
    this->SERVO_1_OFFSET = doc["SERVO_1_OFFSET"]; // -11
    this->SERVO_2_OFFSET = doc["SERVO_2_OFFSET"]; // -2
    this->SERVO_1_ORIENTATION = doc["SERVO_1_ORIENTATION"]; // -1
    this->SERVO_2_ORIENTATION = doc["SERVO_2_ORIENTATION"]; // -1
    this->MAX_FINS_TRAVEL = doc["MAX_FINS_TRAVEL"]; // 15
    
    this->PID_PITCH_Kp = doc["PID_PITCH_Kp"]; // 2
    this->PID_PITCH_Ki = doc["PID_PITCH_Ki"]; // 0
    this->PID_PITCH_Kd = doc["PID_PITCH_Kd"]; // 0.5
    this->PID_ROLL_Kp = doc["PID_ROLL_Kp"]; // 2
    this->PID_ROLL_Ki = doc["PID_ROLL_Ki"]; // 0
    this->PID_ROLL_Kd = doc["PID_ROLL_Kd"]; // 0.5
    
    this->X_GYRO_OFFSETS = doc["X_GYRO_OFFSETS"]; // 24
    this->Y_GYRO_OFFSETS = doc["Y_GYRO_OFFSETS"]; // 43
    this->Z_GYRO_OFFSETS = doc["Z_GYRO_OFFSETS"]; // 525
    this->X_ACCEL_OFFSETS = doc["X_ACCEL_OFFSETS"]; // -1109
    this->Y_ACCEL_OFFSETS = doc["Y_ACCEL_OFFSETS"]; // 841
    this->Z_ACCEL_OFFSETS = doc["Z_ACCEL_OFFSETS"]; // 525
     
   Serial.println("Done....") ;

   return true; 
}


int Configuration::saveConfig() {
   Serial.println("Saving configuration.....") ;
    // Serial.print("debug : "); Serial.println(this->debug);
    // Serial.print("buzzer_enabled : "); Serial.println(this->buzzer_enabled);
    // Serial.print("memory_card_enabled : "); Serial.println(this->memory_card_enabled);
    // Serial.print("data_recovery_mode : "); Serial.println(this->data_recovery_mode);
    // Serial.print("format_memory : "); Serial.println(this->format_memory);
    
   Serial.println("Done!.....") ;
   return 5; 
}

