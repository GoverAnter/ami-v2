# Arduino Minecraft Interface v2

## Disclaimer

The original idea and part of this code is not mine !

[See this link for the original work.](https://www.instructables.com/id/Arduino-Minecraft-Interface/)

## How it works

It simply parses the Minecraft logs, searching for a specific pattern (in this case, a `/say` emitted by a command block), and sends the argument to Arduino using serial communication.

## Dependencies

Only dependency is `SerialPort`, which is included as a git submodule in this repository.
It is used to have an abstraction layer between this code, and the serial stuff.

## How to build

Clone this repo :

`git clone https://github.com/GoverAnter/ami-v2`

Create a folder to contain built objects :

`mkdir build`

Go to this folder :

`cd build`

Run CMake :

`cmake ../`

Compile the thing (depends on the compiler cmake is configure to target) :

On linux : `make`

On windows : Compile the generated solution.

## License

Do what you want with my code.

Initial code was copy-pasted without much modifications, and it was provided without any license.

SerialPort is MIT-licensed.