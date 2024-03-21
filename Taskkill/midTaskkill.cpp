//taskkill
//	/pid - specify PID to kill
//	/im - specify image name of process to kill, or * for all
//	/f - force kill process
//	/t - ends specified process and any of its children

#include <windows.h>
#include <iostream>
#include <string>
#include <tlhelp32.h>
#include <sstream>
#include <vector>

// Function to get the children processes of a given process ID
std::vector<DWORD> getProcessChildren(DWORD parentProcessID) {
    std::vector<DWORD> children;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 processEntry;
        processEntry.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(hSnapshot, &processEntry)) {
            do {
                if (processEntry.th32ParentProcessID == parentProcessID) {
                    children.push_back(processEntry.th32ProcessID);
                }
            } while (Process32Next(hSnapshot, &processEntry));
        }
        CloseHandle(hSnapshot);
    }
    return children;
}

// Function to terminate a process by its process ID
void terminateProcessByPID(DWORD processID, bool force = false, bool terminateChildren = false) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processID);
    if (hProcess != NULL) {
        if (terminateChildren) {
            std::vector<DWORD> childProcesses = getProcessChildren(processID);
            for (DWORD childProcessID : childProcesses) {
                terminateProcessByPID(childProcessID, force, terminateChildren);
            }
        }

        if (!TerminateProcess(hProcess, 0)) {
            std::cerr << "Failed to terminate process with PID " << processID << "." << std::endl;
        }
        else {
            std::cout << "Process with PID " << processID << " terminated successfully." << std::endl;
        }
        CloseHandle(hProcess);
    }
    else {
        std::cerr << "Failed to open process with PID " << processID << "." << std::endl;
    }
}

// Function to terminate a process by its name
void terminateProcessByName(const std::wstring& processName, bool force = false, bool terminateChildren = false) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create snapshot of processes." << std::endl;
        return;
    }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &processEntry)) {
        std::cerr << "Failed to retrieve information about the first process." << std::endl;
        CloseHandle(hSnapshot);
        return;
    }

    do {
        std::wstring currentProcessName = processEntry.szExeFile;
        if (currentProcessName == processName) {
            terminateProcessByPID(processEntry.th32ProcessID, force, terminateChildren);
        }
    } while (Process32Next(hSnapshot, &processEntry));

    CloseHandle(hSnapshot);
}

// Main function
int main(int argc, char* argv[]) {
    if (argc < 3 || (std::string(argv[1]) != "/pid" && std::string(argv[1]) != "/im")) {
        std::cerr << "Usage: " << argv[0] << " [/pid <ProcessID> | /im <ImageName>] [/f] [/t]" << std::endl;
        return 1;
    }

    std::string option = argv[1];
    bool forceTerminate = false;
    bool terminateChildren = false;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "/f") {
            forceTerminate = true;
        }
        else if (arg == "/t") {
            terminateChildren = true;
        }
    }

    if (option == "/pid" && argc >= 3) {
        DWORD processID;
        std::istringstream(argv[2]) >> processID;
        terminateProcessByPID(processID, forceTerminate, terminateChildren);
    }
    else if (option == "/im" && argc >= 3) {
        std::wstring processName = std::wstring(argv[2], argv[2] + strlen(argv[2]));
        terminateProcessByName(processName, forceTerminate, terminateChildren);
    }
    else {
        std::cerr << "Invalid option or insufficient arguments provided." << std::endl;
        return 1;
    }

    return 0;
}
