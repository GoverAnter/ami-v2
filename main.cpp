#include <iostream>
#include <fstream>
#include <string>

#include <SerialPort.h>

// Uncomment this if you only want to have console message, and not having to have a wired arduino
// #define NO_SERIAL

using namespace std;

static const string dirString = "[Client thread/INFO]: [CHAT] [@] ";

bool stringCheck(string tempString) //Checks that I am using the Correct Command/Also used for speed increase
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

int main()
{
    ifstream readFile; // Open Log File to Read
    string bufString; // A Specific Line from the Log
    string cmdString; // To Be used, will contain the command to be sent to the mcu

#ifndef NO_SERIAL
    int port;
    cout << "Please Enter Your Arduino's Port Number: ";
    cin >> port;

    char* osPort = nullptr;
    sprintf(osPort, R"(\\.\COM%d)", port);

    auto serial = new SerialPort(osPort);

    if(serial->isConnected())//Checks for Arduino on Port
        std::cout << "Connection Established!" << std::endl;
    else
    {
        std::cout << "Connection Failed." << std::endl;
        return -1;
    }
#endif

    int cmdC = 0; // Iterator
    bool valCheck = false; // A variable to allow multiple commands from a single line
    bool pOcheck = false; // A variable used to determine if the port needs to be opened.
    while(true)
    {
        readFile.open("latest.log");

        // Check that the file opened correctly
        if(readFile.is_open())
        {
            // Start cycling through the lines of the file
            for(int i = 0; i < 50; i++)
            {
                // Pull single lines to read
                getline(readFile, bufString);

                for(int i = 0; i < 33; i++)
                    bufString[i] = bufString[i+11];

                // Cycles through String chars
                for(unsigned int i2 = 0; i2 < bufString.size(); i2++)
                {
                    // Checks for ! indicator, checks for end of line/end of cmd string,
                    // checks that of the commands it is only reading the command once
                    if(bufString[i2] == '!' && !valCheck && stringCheck(bufString))
                    {
                        valCheck = true;
                        i2++;
                        // This Loop Writes the Command to another string for ease of use.
                        for(; bufString[i2] != '!'; i2++)
                        {
                            // Checks for end of line
                            if(bufString[i2] == '\0')
                                break;

                            //writes the command to a new string
                            cmdString[cmdC] = bufString[i2];
                            cmdC++;
                            pOcheck = true;
                        }

                        // Checks for end of line/more commands
                        if(bufString[i2 + 1] != '\0')
                            valCheck = false;
                    }
                }
                cmdC = 0;
                valCheck = false;
            }
        }

        // Safely Close File
        readFile.close();

        if(pOcheck)
        {
            ofstream erase("latest.log");
            erase.close();

#ifndef NO_SERIAL
            char* cmd = strdup(cmdString.data());
            serial->writeSerialPort(cmd, strlen(cmd));
#endif
            std::cout << "Sent command " << cmdString.data() << std::endl;

            pOcheck = false;
        }
    }
}