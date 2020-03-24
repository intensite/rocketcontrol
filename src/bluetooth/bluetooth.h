#include "../command/command.h"
void setupBLE(CliCommand& cliPtr); 
void updateDiagnostics(float ypr[3], int16_t ac_x, int16_t ac_y, int16_t ac_z);
void updateBLEparams();
void updateOrientation(float ypr[3]);
void updateAccels(int16_t ac_x, int16_t ac_y, int16_t ac_z);