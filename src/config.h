#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

// PIN ASSIGNMENT 
#define FINS_SERVO_1_PIN 6 
#define FINS_SERVO_2_PIN 5
#define PARACHUTE_IGNITER_PIN 8
#define R_LED A3                            // Analog pin for the Red LED
#define B_LED A0                            // Analog pin for the Blue LED
#define G_LED A1                            // Analog pin for the Green LED
#define PIEZO_BUZZER 9                            // Analog pin for the Green LED
 

#define APOGEE_DIFF_METERS 10               //  Used to specify minimum altitude for liftoff and minimum decent for parachute deployment 
#define BEEPER_ENABLED 1                    //  Used to specify if the beeped should be neabled or not
#define SCAN_TIME_INTERVAL 50              //  Used to specify the refresh rate in mili-seconds of the instruments (altimeter and gyroscope)

#define DEBUG 1                             // Set to 1 to display debug info to the serial console. Set to 0 otherwise.
#define BUZZER_ENABLE 0                     // Set to 1 to enable the buzzer. Set to 0 otherwise.



#endif // CONFIG_FILE_H

