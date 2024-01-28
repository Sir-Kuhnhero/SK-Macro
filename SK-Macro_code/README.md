# SK-Macro Code Documentation

This project contains the source code for a utility that interacts with a hardware device using the MCP23017 and HID protocols.

## Source Code

The main source code for the project is located in [src/main.cpp](src/main.cpp).

### Main Functionality

The main functionality of the code is divided into two parts. The MCP23017 is used to read switch inputs and the HID protocol is being used.

#### MCP23017

If the MCP23017 protocol is being used, the code initializes the MCP23017 device with the address `0x20` on the I2C1 bus. If the initialization fails, the program enters a failsafe mode.

After initialization, the code sets up interrupts for the MCP23017 device and configures all 16 pins as input pullups. Each pin is set to trigger an interrupt on state change.

#### HID

If the HID protocol is being used, the code initializes the USB HID device. If the initialization fails, the program enters a failsafe mode.

The code then waits until the device is mounted. While waiting, it blinks the built-in LED.

### Error Handling

The code includes basic error handling. If an error occurs during the initialization of the MCP23017 or HID device, the program enters a failsafe mode and prints an error message.

## Workspace Structure

The workspace includes the following directories and files:

- `.gitignore`: Specifies intentionally untracked files to ignore.
- `.vscode/`: Contains settings specific to Visual Studio Code.
- `include/`: Contains header files for the project.
- `lib/`: Contains libraries used in the project.
- `platformio.ini`: Configuration file for PlatformIO.
- `README.md`: This file, which provides an overview of the project.
- `SK-Macro_code.code-workspace`: The Visual Studio Code workspace file for the project.
- `src/`: Contains the source code for the project.
- `test/`: Contains nothing.


This Documentation has been written by Copilot and might have some issues. I am just to0 lazy :)