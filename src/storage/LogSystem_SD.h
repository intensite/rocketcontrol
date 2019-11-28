#pragma once


// #include "DateTime.h"

#include <Arduino.h>
#include "SdFat.h"
#include "SdFat.h"




/// A single log record.
///
class LogRecord
{
public:
    /// Create a new log record using the given values.
    ///
    /// @param dateTime The time of the record.
    /// @param temperature The temperature in celsius.
    /// @param humidity The humidity as percentage 0-100.
    ///
    // LogRecord(const DateTime &dateTime, float temperature, float humidity);
    LogRecord(unsigned long timestamp, unsigned int altitude, int pitch, int roll, 
                int pitchServo, int rollServo, bool parachute, bool abort, 
                byte temperature, byte battery, byte gForces);
                
    /// Create a special null record.
    ///
    /// This records are used in error situations.
    ///
    LogRecord();
    
    /// dtor
    ///
    ~LogRecord();

public:
    /// Check if this is a null record.
    ///
    bool isNull() const;
    
    /// Get the time of the record.
    ///
    // inline DateTime getDateTime() const { return _dateTime; }
    
    /// Get the temperature of the record in celsius.
    ///
    // inline float getTemperature() const { return _temperature; }
    
    /// Get the humidity of the record in percent 0-100.
    ///
    // inline float getHumidity() const { return _humidity; }
    
    /// Write this record to the serial interface.
    ///
    /// The format is: date/time, temperature, humidity
    /// Example: 2015-08-22 12:42:21,80,25
    ///
    void writeToSerial() const;
    
public:     // Public for now.  Maybe getter and setters would be more appropriate
    unsigned long _timestamp;  // Milliseconds since start
    unsigned int _altitude;
    int _pitch;
    int _roll;
    int _pitchServo;
    int _rollServo;
    bool _parachute;
    bool _abort;
    byte _temperature;
    byte _battery; // remaining volts x 10 
    byte _gForces;
};




/// The log system to write and read all sensor data.
///
class LogSystem {
    private:
        // SD chip select pin.  Be sure to disable any other SPI devices such as Enet.
        const uint8_t chipSelect = SS;
        // File system object.
        SdFat sd;
        // Log file.
        SdFile file;
    
    public:

        // Constructor
        LogSystem();


    /// Initialize the log system
    bool begin();

    /// Append a record to the storage.
    ///
    /// @param logRecord The record to append.
    /// @return true on success, false otherwize
    ///
    bool appendRecord(const LogRecord &logRecord);
    void writeHeader();
};




    







