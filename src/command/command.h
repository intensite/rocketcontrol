#include <SimpleCLI.h>

class CliCommand {
    private:
        SimpleCLI cli;
        Command cmdGet;
        Command cmdSet;
        void handleReceivedMessage(char* msg);

    public:
        CliCommand();
        void handleSerial();
        void processGetCommand(const char*);
        void processSetCommand(const char* setting, const char* value);
};