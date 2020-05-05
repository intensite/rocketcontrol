#include "Arduino.h"
#include "../storage/Storage.h"
#include "../storage/LogSystem.h"

void readDataToSerial() {
    uint32_t reccount = 0;
    reccount = lr::LogSystem::currentNumberOfRecords();
    Serial.print("Record Count : "); Serial.println(reccount);
        
    if (reccount > 0) {
        for(uint32_t i = 0; i < reccount; i++) {
            lr::LogRecord logRecord = lr::LogSystem::getLogRecord(i);
            logRecord.writeToSerial();
        }
        Serial.print("Ouff I just read xx records : "); Serial.println(reccount);
    } else {
        Serial.println("Nothing to read");
    }
}