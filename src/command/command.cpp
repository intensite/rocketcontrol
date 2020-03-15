#include "Arduino.h"
#include "command.h"
#include "../configuration/configuration.h"
#include <SimpleCLI.h>


CliCommand::CliCommand() {
    cmdGet = cli.addCommand("get");
    cmdGet.addPositionalArgument("setting");
    // cmdGet.addPositionalArgument("value");

    cmdSet = cli.addCommand("set");
    cmdSet.addPositionalArgument("setting");
    cmdSet.addPositionalArgument("value");
}

void CliCommand::handleSerial() {
   const int BUFF_SIZE = 32; // make it big enough to hold your longest command
   static char buffer[BUFF_SIZE+1]; // +1 allows space for the null terminator
   static int length = 0; // number of characters currently in the buffer
   static boolean newData = false;

    //    if(Serial.available())
    // while (Serial.available() > 0 && newData == false) {
    while (Serial.available() > 0 ) {
        char c = Serial.read();
        if((c == '\r') || (c == '\n')){
        // if(c == ';') {
            // end-of-line received
            Serial.println("end-of-line received");
            
            if(length > 0) {
                this->handleReceivedMessage(buffer);
            }
            length = 0;
            // newData = true;
        } else {
            if(length < BUFF_SIZE) {
                buffer[length++] = c; // append the received character to the array
                buffer[length] = 0; // append the null terminator
                // Serial.print("Caractere ajouté : "); Serial.println(c);
            } else {
                // buffer full - discard the received character
                Serial.println("buffer full");
                
            }
        }
    }
    // Serial.println("End of handleSerial() function...");
}

void CliCommand::handleReceivedMessage(char* msg)
{
    // Serial.println("Serial message received!");

    this->cli.parse(msg);

    // Check if a new error occurred
    if(this->cli.errored()) {
    CommandError e = this->cli.getError();

    // Print the error, or do whatever you want with it
    Serial.println(e.toString());
    }

    // First check if a newly parsed command is available
    if(this->cli.available()) {
        // Serial.print("It appears to be a valid command!");

        // Get the command out of the queue
        Command cmd = this->cli.getCommand();

        // Get the Argument(s) you want
        Argument argSetting = cmd.getArgument("setting"); // via name
        Argument argValue = cmd.getArgument("value"); // via index
        // strcpy(setting, argSetting.getValue());
        // strcpy(value, argValue.getValue());
        String setting = argSetting.getValue();
        String value = argValue.getValue();


        // Check if it's the command you're looking for
        if(cmd == this->cmdGet) {
            this->processGetCommand(setting.c_str());
        } 
        if(cmd == this->cmdSet) {
            this->processSetCommand(setting.c_str(), value.c_str());
        }
    } else {

        Serial.print("It appears to be a INVALID command!");
    }

    return;
}

void CliCommand::processGetCommand(const char* setting) {
    if(strcmp(setting, "BUZZER") == 0) {
        Serial.print("_CONF.BUZZER_ENABLE: "); Serial.println(_CONF.BUZZER_ENABLE);    
    } 
    else if(strcmp(setting, "DEBUG") == 0) {
        Serial.print("_CONF.DEBUG: "); Serial.println(_CONF.DEBUG);    
    } 
    // else if(strcmp(setting, "buzzer") == 0) {
    // } 
    // else if(strcmp(setting, "buzzer") == 0) {
    // } 
    // else if(strcmp(setting, "buzzer") == 0) {
    // } 
    // else if(strcmp(setting, "buzzer") == 0) {
    // } 
    // else if(strcmp(setting, "buzzer") == 0) {
    // } 
    else if(strcmp(setting, "ALL") == 0) {
        Serial.print("_CONF.DEBUG: "); Serial.println(_CONF.DEBUG);    
        Serial.print("_CONF.BUZZER_ENABLE: "); Serial.println(_CONF.BUZZER_ENABLE);    
        Serial.print("_CONF.MEMORY_CARD_ENABLED: "); Serial.println(_CONF.MEMORY_CARD_ENABLED);    
        Serial.print("_CONF.DATA_RECOVERY_MODE: "); Serial.println(_CONF.DATA_RECOVERY_MODE);    
        Serial.print("_CONF.FORMAT_MEMORY: "); Serial.println(_CONF.FORMAT_MEMORY);    
        Serial.print("_CONF.APOGEE_DIFF_METERS: "); Serial.println(_CONF.APOGEE_DIFF_METERS);    
        Serial.print("_CONF.EXCESSIVE_ANGLE_THRESHOLD: "); Serial.println(_CONF.EXCESSIVE_ANGLE_THRESHOLD);    
        Serial.print("_CONF.SCAN_TIME_INTERVAL: "); Serial.println(_CONF.SCAN_TIME_INTERVAL);    
        Serial.print("_CONF.PARACHUTE_DELAY: "); Serial.println(_CONF.PARACHUTE_DELAY);    
        Serial.print("_CONF.PITCH_AXIS: "); Serial.println(_CONF.PITCH_AXIS);    
        Serial.print("_CONF.YAW_AXIS: "); Serial.println(_CONF.YAW_AXIS);    
        Serial.print("_CONF.ROLL_AXIS: "); Serial.println(_CONF.ROLL_AXIS);    
        Serial.print("_CONF.SERVO_1_OFFSET: "); Serial.println(_CONF.SERVO_1_OFFSET);    
        Serial.print("_CONF.SERVO_2_OFFSET: "); Serial.println(_CONF.SERVO_2_OFFSET);   

        Serial.print("_CONF.SERVO_1_ORIENTATION: "); Serial.println(_CONF.SERVO_1_ORIENTATION);    
        Serial.print("_CONF.SERVO_2_ORIENTATION: "); Serial.println(_CONF.SERVO_2_ORIENTATION);    
        Serial.print("_CONF.MAX_FINS_TRAVEL: "); Serial.println(_CONF.MAX_FINS_TRAVEL);    
        Serial.print("_CONF.PID_PITCH_Kp: "); Serial.println(_CONF.PID_PITCH_Kp);    
        Serial.print("_CONF.PID_PITCH_Ki: "); Serial.println(_CONF.PID_PITCH_Ki);    
        Serial.print("_CONF.PID_PITCH_Kd: "); Serial.println(_CONF.PID_PITCH_Kd);    
        Serial.print("_CONF.PID_ROLL_Kp: "); Serial.println(_CONF.PID_ROLL_Kp);    

        Serial.print("_CONF.PID_ROLL_Ki: "); Serial.println(_CONF.PID_ROLL_Ki);    
        Serial.print("_CONF.PID_ROLL_Kd: "); Serial.println(_CONF.PID_ROLL_Kd);    
        Serial.print("_CONF.X_GYRO_OFFSETS: "); Serial.println(_CONF.X_GYRO_OFFSETS);    
        Serial.print("_CONF.Y_GYRO_OFFSETS: "); Serial.println(_CONF.Y_GYRO_OFFSETS);    
        Serial.print("_CONF.Z_GYRO_OFFSETS: "); Serial.println(_CONF.Z_GYRO_OFFSETS);    
        Serial.print("_CONF.X_ACCEL_OFFSETS: "); Serial.println(_CONF.X_ACCEL_OFFSETS);    
        Serial.print("_CONF.Y_ACCEL_OFFSETS: "); Serial.println(_CONF.Y_ACCEL_OFFSETS);    
        Serial.print("_CONF.Z_ACCEL_OFFSETS: "); Serial.println(_CONF.Z_ACCEL_OFFSETS);    
    }
}

void CliCommand::processSetCommand(const char* setting, const char* value) {

    if(strcmp(setting, "BUZZER") == 0) {
        _CONF.BUZZER_ENABLE = atoi(value); 
        Serial.print("_CONF.BUZZER_ENABLE: "); Serial.println(_CONF.BUZZER_ENABLE);  
    } 
    else if(strcmp(setting, "DEBUG") == 0) {
        _CONF.DEBUG = atoi(value); 
        Serial.print("_CONF.DEBUG: "); Serial.println(_CONF.DEBUG);    
    } 
}

    