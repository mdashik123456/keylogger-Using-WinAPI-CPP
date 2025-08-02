# Windows Keylogger

A simple Windows keylogger that captures keyboard input and logs it to a file.

## Features
- Captures both uppercase and lowercase characters
- Handles shift key and caps lock states correctly
- Logs only printable characters (ignores special keys)
- Stores output sequentially in a clean format
- Lightweight and efficient

## Requirements
- Windows OS
- C++ compiler (Visual Studio, MinGW, etc.)

## Installation
1. Clone this repository
2. Compile the code:
   ```
   g++ main.cpp -o keylogger.exe
   ```

## Usage
1. Run the compiled executable:
   ```
   keylogger.exe
   ```
2. The program will create a `system_log.txt` file in the same directory
3. All keyboard input will be logged to this file

## Important Notes
- This tool is for educational purposes only
- Always get proper authorization before using on any system
- The log file (`system_log.txt`) can be viewed with any text editor

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.