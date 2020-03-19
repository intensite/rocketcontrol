#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "Arduino.h"
#include "../configuration/configuration.h"
#include "../command/command.h"
#include "./bluetooth.h"

/* Define the UUID for our Custom Service */
#define serviceID BLEUUID((uint16_t)0x1700)

// Diagnostics Caracteristic (R/O)
BLECharacteristic diagCharacteristic(
  BLEUUID((uint16_t)0x1A00), 
  BLECharacteristic::PROPERTY_READ | 
  BLECharacteristic::PROPERTY_NOTIFY
);
// Parameters Caracteristic (R/O)
BLECharacteristic paramCharacteristic(
  BLEUUID((uint16_t)0x1A02), 
  BLECharacteristic::PROPERTY_READ | 
  BLECharacteristic::PROPERTY_NOTIFY
);
// Input Commands Caracteristic (W/O)
BLECharacteristic commandCharacteristic(
  BLEUUID((uint16_t)0x1A01), 
  // BLECharacteristic::PROPERTY_READ | 
  // BLECharacteristic::PROPERTY_WRITE | 
  // BLECharacteristic::PROPERTY_NOTIFY
  BLECharacteristic::PROPERTY_WRITE
);

/* Define the UUID for our Custom Service */
#define serviceID BLEUUID((uint16_t)0x1700)
CliCommand clii; // Passed from the setupBLE function to process the received commands

void processCommand(const char* msg) {
  clii.handleReceivedMessage(msg);
}

/* This function handles the server callbacks */
bool deviceConnected = false;
class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* MyServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* MyServer) {
      deviceConnected = false;
    }
};

class CharacteristicCallbacks: public BLECharacteristicCallbacks {

      // Define a callback type: a pointer to a function taking no
      // arguments and returning void.
      typedef void (*callback_t)(const char*);

    public:
      CharacteristicCallbacks(callback_t _callback)         
        // Initialize internal callback with the one given as parameter.
        : callback(_callback)
        // empty function body
        {}

     void onWrite(BLECharacteristic *characteristic) {
          //return the pointer to the register containing the current value of the characteristic
          std::string rxValue = characteristic->getValue(); 
          //check if there are data (size greater than zero)
          if (rxValue.length() > 0) {
 
              // for (int i = 0; i < rxValue.length(); i++) {
              //   Serial.print(rxValue[i]);
              //  }
              //  Serial.println();
               Serial.print("Calling CLI.processSetCommand() with : "); Serial.println(rxValue.c_str());
               callback(rxValue.c_str());
          }
     }//onWrite

     private:
    // The callback is kept as private internal data.
    callback_t callback;
};


void setupBLE(CliCommand& cliPtr) {

  clii = cliPtr; // Get a reference to the main's CLI object to parse the commands received.

   // Create and name the BLE Device
  BLEDevice::init("MORGAN flight computer");

  /* Create the BLE Server */
  BLEServer *MyServer = BLEDevice::createServer();
  MyServer->setCallbacks(new ServerCallbacks());  // Set the function that handles Server Callbacks

  /* Add a service to our server */
  BLEService *customService = MyServer->createService(BLEUUID((uint16_t)0x1700)); //  A random ID has been selected

  /* Add a characteristic to the service */
  customService->addCharacteristic(&diagCharacteristic);  //diagCharacteristic was defined above
  customService->addCharacteristic(&commandCharacteristic);  //customCharacteristic2 was defined above  
  customService->addCharacteristic(&paramCharacteristic);  //customCharacteristic2 was defined above  

  /* Add Descriptors to the Characteristic*/
  diagCharacteristic.addDescriptor(new BLE2902());  //Add this line only if the characteristic has the Notify property
  paramCharacteristic.addDescriptor(new BLE2902());  //Add this line only if the characteristic has the Notify property
  // commandCharacteristic.addDescriptor(new BLE2902());  //Add this line only if the characteristic has the Notify property

  // Callback use to receive commands
  commandCharacteristic.setCallbacks(new CharacteristicCallbacks(processCommand));

  BLEDescriptor VariableDescriptor(BLEUUID((uint16_t)0x2901));    /*```````````````````````````````````````````````````````````````*/
  VariableDescriptor.setValue("gyro pitch, roll, yaw");           /* Use this format to add a hint for the user. This is optional. */
  diagCharacteristic.addDescriptor(&VariableDescriptor);          /*```````````````````````````````````````````````````````````````*/

  /* Configure Advertising with the Services to be advertised */
  MyServer->getAdvertising()->addServiceUUID(serviceID);

  // Start the service
  customService->start();

  // Start the Server/Advertising
  MyServer->getAdvertising()->start();

  Serial.println(F("Waiting for a Client to connect..."));
}

void updateBLE(float ypr[3]) {
    float gyro[3];
    char str[25];
    
    gyro[_CONF.YAW_AXIS] = 0;
    gyro[_CONF.ROLL_AXIS] = (ypr[_CONF.ROLL_AXIS] * 180/M_PI);
    gyro[_CONF.PITCH_AXIS] = (ypr[_CONF.PITCH_AXIS] * 180/M_PI);

    sprintf(str, "%.1f|%.1f|%.1f", gyro[_CONF.YAW_AXIS], gyro[_CONF.ROLL_AXIS], gyro[_CONF.PITCH_AXIS]);



    if (deviceConnected) {
      /* Set the value */
      // diagCharacteristic.setValue(gyro, sizeof(gyro));  // This is a value of a single byte
      diagCharacteristic.setValue(str);  // This is a value of a single byte
      diagCharacteristic.notify();  // Notify the client of a change

      updateBLEparams();
      
      // sprintf(str, "%d", _CONF.BUZZER_ENABLE);
      // commandCharacteristic.setValue(str);
      // commandCharacteristic.notify();  // Notify the client of a change
    }
}

/********
 * BLE paramCharacteristic that can be updated 
 * on a slower schedule than the main diagnistics one
 */
void updateBLEparams() {

    char param_str[20];

    //   // PREFERENCES
    // uint8_t DEBUG; // 1                                  // Set to 1 to read collected data from memory: 0 to save data to memory
    // uint8_t BUZZER_ENABLE; // 0                          // Set to 1 to enable the buzzer. Set to 0 otherwise.
    // uint8_t MEMORY_CARD_ENABLED; // 1                    // Set to 1 to activate the logging system.  0 to disable it (for testing)
    // uint8_t DATA_RECOVERY_MODE; // 1                     // Set to 1 to read collected data from memory: 0 to save data to memory
    // uint8_t FORMAT_MEMORY; // 0        

  sprintf(param_str, "%d|%d|%d|%d|%d", _CONF.DEBUG, _CONF.BUZZER_ENABLE, _CONF.MEMORY_CARD_ENABLED, _CONF.DATA_RECOVERY_MODE, _CONF.FORMAT_MEMORY);
  paramCharacteristic.setValue(param_str);
  paramCharacteristic.notify();  // Notify the client of a change

}





// public:     // Public for now.  Maybe getter and setters would be more appropriate
//     int32_t _timestamp;  // Milliseconds since start
//     uint16_t int _altitude;
//     int16_t _pitch;
//     int16_t _roll;
//     int16_t _pitchServo;
//     int16_t _rollServo;
//     bool _parachute;
//     bool _abort;
//     byte _temperature;
//     byte _battery; // remaining volts x 10 
//     byte _gForces;
// };

/**
   Sensors (READ, NOTIFY)
  ================================
  gyro pitch, roll, yaw 
  servos pitch, roll, yaw 
  axel X, Ym Z
  alti meters, pressure(mbar)
  temp(C)
  Battery Voltage(V)
  PyroStatus 1,2,3,4  ()

  Settings (READ, WRITE)
  ================================
  Servos offset 1,2,3,4
  Servos orientation 1,2,3,4
  Pitch PID P, I, D
  Yaw PID P, I, D
  Roll PID P, I, D
  MAX_FINS_TRAVEL (degrees)
   PyroTest 1,2,3,4  (boutons)

  APOGEE_DIFF_METERS 10               // Used to specify minimum altitude for liftoff and minimum decent for parachute deployment. 
  EXCESSIVE_ANGLE_THRESHOLD 50        // Used to specify maximum angle before abort sequence is initiated.
  SCAN_TIME_INTERVAL 100              // Used to specify the refresh rate in mili-seconds of the instruments (altimeter and gyroscope).
  PARACHUTE_DELAY 1500 

  DEBUG 1                             // Set to 1 to display debug info to the serial console. Set to 0 otherwise.
  BUZZER_ENABLE 1                     // Set to 1 to enable the buzzer. Set to 0 otherwise.
  MEMORY_CARD_ENABLED 0               // Set to 1 to activate the logging system.  0 to disable it (for testing)
  DATA_RECOVERY_MODE 0                // Set to 1 to read collected data from memory: 0 to save data to memory
  FORMAT_MEMORY 0   




*/
