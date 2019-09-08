#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

// PIN ASSIGNMENT 
#define FINS_SERVO_1_PIN 6 
#define FINS_SERVO_2_PIN 5
#define PARACHUTE_IGNITER_PIN 2
#define R_LED A3                            // Analog pin for the Red LED
#define B_LED A0                            // Analog pin for the Blue LED
#define G_LED A1                            // Analog pin for the Green LED
#define PIEZO_BUZZER 9                            // Analog pin for the Green LED
 

#define APOGEE_DIFF_METERS 10               //  Used to specify minimum altitude for liftoff and minimum decent for parachute deployment. 
#define EXCESSIVE_ANGLE_THRESHOLD 50         //  Used to specify maximum angle before abort sequence is initiated.
#define SCAN_TIME_INTERVAL 20                //  Used to specify the refresh rate in mili-seconds of the instruments (altimeter and gyroscope).


// SERVO STUFF
#define SERVO_1_OFFSET 10                    // Used to compensate the servo #1  misalignment
#define SERVO_2_OFFSET 0                    // Used to compensate the servo #1  misalignment
#define SERVO_1_ORIENTATION 1               // Used to reverse the orientation of the servo #1 (possible values 1, -1) 
#define SERVO_2_ORIENTATION 1               // Used to reverse the orientation of the servo #2 (possible values 1, -1)

#define MAX_FINS_TRAVEL 25                  // Used to specify limits of the fins travel in degrees (+/-)


// PITCH PID CONSTANTS
#define PID_PITCH_Kp 3
#define PID_PITCH_Ki 0
#define PID_PITCH_Kd 0

// ROLL PID CONSTANTS
#define PID_ROLL_Kp 3
#define PID_ROLL_Ki 0
#define PID_ROLL_Kd 0

#define DEBUG 0                             // Set to 1 to display debug info to the serial console. Set to 0 otherwise.
#define BUZZER_ENABLE 0                     // Set to 1 to enable the buzzer. Set to 0 otherwise.

#define DATA_RECOVERY_MODE 0                // Set to 1 to read collected data from memory
#define FORMAT_MEMORY 1                     // Set to 1 to erase memory.

#endif // CONFIG_FILE_H

