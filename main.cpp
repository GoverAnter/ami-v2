#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <SerialPort.h>

// This is the log line we are waiting for. Feel free to change it to target something else as a serial trigger.
static const std::string dirString = "[Client thread/INFO]: [CHAT] [@] ";

bool stringCheck(std::string tempString) //Checks that I am using the Correct Command/Also used for speed increase
{
    //string dirString = "[23:43:08] [Client thread/INFO]: [CHAT] [@] ";
    unsigned int charCount = 0;
    for(unsigned int i = 0; i < dirString.size(); i++)
    {
        if(dirString[i] == tempString[i])
            charCount++;
    }

    return charCount == 33;
}

static SerialPort* serial = nullptr;

int main(int argc, char *argv[])
{
    try {
        // Control vars
        // Try to communicate with arduino only if this flag is true
        bool useSerial = true;
        // Read the log file only if this flag is true
        bool readLogs = true;

        if(argc > 0) {
            for(int i = 1; i < argc; i++) {
                if(strcmp(argv[i], "--no-serial") == 0) {
                    useSerial = false;
                } else if(strcmp(argv[i], "--no-log-read") == 0) {
                    readLogs = false;
                }
            }
        }

        std::ifstream readFile; // Open Log File to Read
        std::string bufString; // A Specific Line from the Log
        std::string cmdString; // To Be used, will contain the command to be sent to the mcu

        if(useSerial) {
            std::string port;
            std::cout << "Please Enter Your Arduino's Port Number: ";
            std::cin >> port;

            std::vector<char> cstr(port.c_str(), port.c_str() + port.size() + 1);

            serial = new SerialPort(cstr.data());

            if(serial->isConnected()) // Checks for Arduino on Port
                std::cout << "Connection Established!" << std::endl;
            else
            {
                std::cout << "Connection Failed." << std::endl;
                return -1;
            }
        }

        while(readLogs)
        {
            readFile.open("latest.log");

            // Check that the file opened correctly
            if(readFile.is_open())
            {
                // Start cycling through the lines of the file
                while(std::getline(readFile, bufString)) {
                    // Remove the timestamp
                    bufString = bufString.substr(11, bufString.size() - 11);

                    if(stringCheck(bufString))
                    {
                        cmdString = bufString.substr(dirStringSize + 1, bufString.size() - dirStringSize - 2);

                        if(useSerial) {
                            char* cmd = _strdup(cmdString.c_str());
                            serial->writeSerialPort(cmd, strlen(cmd));
                        }

                        std::cout << "Sent command " << cmdString.c_str() << std::endl;
                    }
                    cmdC = 0;
                    valCheck = false;
                }

                // Safely close the file
                readFile.close();
            }

            // Safely Close File
            readFile.close();

            if(pOcheck)
            {
                std::ofstream erase("latest.log");
                erase.close();

                if(useSerial) {
                    char* cmd = _strdup(cmdString.data());
                    serial->writeSerialPort(cmd, strlen(cmd));
                }

                std::cout << "Sent command " << cmdString.data() << std::endl;

                pOcheck = false;
            }

            Sleep(50);
        }
    } catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}