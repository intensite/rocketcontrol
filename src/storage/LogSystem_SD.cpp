//
// Stephen Remillard SD Log system for SD card based on the work of: 
//
// Lucky Resistor's Deluxe Data Logger
// ---------------------------------------------------------------------------
// (c)2015 by Lucky Resistor. See LICENSE for details.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
// ---------------------------------------------------------------------------
//
#include "LogSystem_SD.h"
#include <SPI.h>
#include "SdFat.h"

// Log file base name.  Must be six characters or less.
#define FILE_BASE_NAME "Data"
//------------------------------------------------------------------------------


// Default null record constructor
LogRecord::LogRecord() 
        : _timestamp(0), _altitude(0), _pitch(0), _roll(0), 
        _pitchServo(0), _rollServo(0), _parachute(0), _abort(0), 
        _temperature(0), _battery(0), _gForces(0)
{

}

// constructor
LogRecord::LogRecord(unsigned long timestamp, unsigned int altitude, int pitch, int roll, 
                int pitchServo, int rollServo, bool parachute, bool abort, 
                byte temperature, byte battery, byte gForces)
        : _timestamp(timestamp), _altitude(altitude), _pitch(pitch), _roll(roll), 
        _pitchServo(pitchServo), _rollServo(rollServo), _parachute(parachute), _abort(abort), 
        _temperature(temperature), _battery(battery), _gForces(gForces)
{
    // Make data adjustments here if needed

}

// Destructor
LogRecord::~LogRecord()
{
}




void LogRecord::writeToSerial() const
{
    Serial.print(_timestamp);
    Serial.print(F(","));
    Serial.print(_altitude);
    Serial.print(F(","));
    Serial.print(_pitch);
    Serial.print(F(","));
    Serial.print(_roll);
    Serial.print(F(","));
    Serial.print(_pitchServo);
    Serial.print(F(","));
    Serial.print(_rollServo);
    Serial.print(F(","));
    Serial.print(_parachute);
    Serial.print(F(","));
    Serial.print(_abort);
    Serial.print(F(","));
    Serial.print(_temperature);
    Serial.print(F(","));
    Serial.print(_battery);
    Serial.print(F(","));
    Serial.println(_gForces);
}

LogSystem::LogSystem() {
// Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!this->sd.begin(this->chipSelect, SD_SCK_MHZ(50))) {
    this->sd.initErrorHalt();
  }
  Serial.println(F("LogSystem initialized"));
}

bool LogSystem::begin() 
{
  const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
  char fileName[13] = FILE_BASE_NAME "00.csv";

  // Find an unused file name.
  if (BASE_NAME_SIZE > 6) {
    // error("FILE_BASE_NAME too long");
    sd.errorHalt(F("FILE_BASE_NAME too long"));
  }
  while (this->sd.exists(fileName)) {
    if (fileName[BASE_NAME_SIZE + 1] != '9') {
      fileName[BASE_NAME_SIZE + 1]++;
    } else if (fileName[BASE_NAME_SIZE] != '9') {
      fileName[BASE_NAME_SIZE + 1] = '0';
      fileName[BASE_NAME_SIZE]++;
    } else {
      sd.errorHalt(F("Can't create file name"));
    }
  }
  if (!file.open(fileName, O_WRONLY | O_CREAT | O_EXCL)) {
    sd.errorHalt(F("file.open"));
  }

    // Write data header.
    this->writeHeader();

}

bool LogSystem::appendRecord(const LogRecord &logRecord)
{
    // if (gCurrentNumberOfRecords >= gMaximumNumberOfRecords) {
    //     return false;
    // }
    // // zero the following record if possible
    // if (gCurrentNumberOfRecords+1 < gMaximumNumberOfRecords) {
    //     zeroInternalRecord(gCurrentNumberOfRecords+1);
    // }
    // // convert the record into the internal structure.
    // InternalLogRecord internalRecord;
    // memset(&internalRecord, 0, sizeof(InternalLogRecord));
    // internalRecord.timestamp = logRecord._timestamp;
    // internalRecord.altitude = logRecord._altitude;
    // internalRecord.pitch = logRecord._pitch;
    // internalRecord.roll = logRecord._roll;
    // internalRecord.pitchServo = logRecord._pitchServo;
    // internalRecord.rollServo = logRecord._rollServo;
    // internalRecord.parachute = logRecord._parachute;
    // internalRecord.abort = logRecord._abort;
    // internalRecord.temperature = logRecord._temperature;
    // internalRecord.battery = logRecord._battery;
    // internalRecord.gForces = logRecord._gForces;
    // // internalRecord.crc = getCRCForInternalRecord(&internalRecord);
    // setInternalRecord(&internalRecord, gCurrentNumberOfRecords);
    // gCurrentNumberOfRecords++;
    // return true;
}


//------------------------------------------------------------------------------
// Write data header.
void LogSystem::writeHeader() {
  this->file.print(F("micros"));
  for (uint8_t i = 0; i < 11; i++) {
    this->file.print(F(",adc"));
    this->file.print(i, 12);
  }
  this->file.println();
  Serial.println(F("Header saved!"));
}


