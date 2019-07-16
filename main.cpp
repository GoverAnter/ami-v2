#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <SerialPort.h>

// This is the log line we are waiting for. Feel free to change it to target something else as a serial trigger.
#define DIR_STR "[Client thread/INFO]: [CHAT] [@] "
#define DIR_STR_LEN strlen(DIR_STR)

bool stringCheck(const std::string& tempString) //Checks that I am using the Correct Command/Also used for speed increase
{
    if(tempString.size() < DIR_STR_LEN) {
        return false;
    }

    for(unsigned int i = 0; i < DIR_STR_LEN; i++)
    {
        if(DIR_STR[i] != tempString[i])
            return false;
    }

    return true;
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
        // Whether to delete log file content after reading it. Will speed up processing if running for a long time.
        bool autoClean = true;

        if(argc > 0) {
            for(int i = 1; i < argc; i++) {
                if(strcmp(argv[i], "--no-serial") == 0) {
                    useSerial = false;
                } else if(strcmp(argv[i], "--no-log-read") == 0) {
                    readLogs = false;
                } else if(strcmp(argv[i], "--no-autoclean") == 0) {
                    autoClean = false;
                }
            }
        }

        std::ifstream readFile; // Open Log File to Read
        std::string bufString; // A Specific Line from the Log
        std::string cmdString; // To Be used, will contain the command to be sent to the mcu

        if(useSerial) {
            std::string port;
            std::cout << "Please enter your Arduino's port (generally '//./COMX'): ";
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
                        cmdString = bufString.substr(DIR_STR_LEN + 1, bufString.size() - DIR_STR_LEN - 2);

                        if(useSerial) {
                            char* cmd = _strdup(cmdString.c_str());
                            serial->writeSerialPort(cmd, strlen(cmd));
                        }

                        std::cout << "Sent command " << cmdString.c_str() << std::endl;
                    }
                }

                // Safely close the file
                readFile.close();
            }

            // If we need to clean the log file
            if(autoClean) {
                std::ofstream erase("latest.log");
                erase.close();
            }

            Sleep(50);
        }
    } catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    std::cin.ignore();
}