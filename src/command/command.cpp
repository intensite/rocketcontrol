#include "Arduino.h"
#include "command.h"
#include "../configuration/configuration.h"

void handleSerial() {
   const int BUFF_SIZE = 32; // make it big enough to hold your longest command
   static char buffer[BUFF_SIZE+1]; // +1 allows space for the null terminator
   static int length = 0; // number of characters currently in the buffer

   if(Serial.available())
   {
       char c = Serial.read();
    //    if((c == '\r') || (c == '\n'))
       if(c == ';')
       {
           // end-of-line received
           if(length > 0)
           {
               handleReceivedMessage(buffer);
           }
           length = 0;
       }
       else
       {
           if(length < BUFF_SIZE)
           {
               buffer[length++] = c; // append the received character to the array
               buffer[length] = 0; // append the null terminator
           }
           else
           {
               // buffer full - discard the received character
           }
       }
   }
}

void handleReceivedMessage(char *msg)
{
    Serial.print("Serial message received!");

	if(strcmp(msg, "buzzer") == 0)
	{
		// handle the command "on"
        _CONF.BUZZER_ENABLE = ( _CONF.BUZZER_ENABLE == 0) ? 1 : 0;
        Serial.print("_CONF.BUZZER_ENABLE: "); Serial.println(_CONF.BUZZER_ENABLE);
    }
	else
	{
		// etc
	}

}