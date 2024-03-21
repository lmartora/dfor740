#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <wtsapi32.h>
#include <tlhelp32.h>
#pragma comment(lib, "Wtsapi32.lib")

void PrintProcessDetails(DWORD processID, bool verbose, bool showServices)
{
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess != NULL)
    {
        TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
        DWORD dwSessionId = 0;
        TCHAR szSessionName[WINSTATIONNAME_LENGTH + 1];
        SIZE_T dwMemoryUsage = 0;
        TCHAR szStatus[MAX_PATH] = TEXT("<unknown>");
        TCHAR szUsername[MAX_PATH] = TEXT("<unknown>");
        DWORD dwCpuTime = 0;
        TCHAR szWindowTitle[MAX_PATH] = TEXT("<unknown>");

        // Get the process name
        if (GetProcessImageFileName(hProcess, szProcessName, MAX_PATH) > 0)
        {
            // Extract only the process name from the full path
            TCHAR* fileName = _tcsrchr(szProcessName, '\\');
            if (fileName != NULL)
            {
                _tcscpy_s(szProcessName, MAX_PATH, fileName + 1);
            }
        }

        // Get session ID
        ProcessIdToSessionId(processID, &dwSessionId);

        // Get session name
        if (dwSessionId != 0)
        {
            HANDLE hServer = WTS_CURRENT_SERVER_HANDLE;
            DWORD dwCount = 0;
            WTS_SESSION_INFO* pSessionInfo;
            if (WTSEnumerateSessions(hServer, 0, 1, &pSessionInfo, &dwCount) != 0)
            {
                for (DWORD i = 0; i < dwCount; i++)
                {
                    if (pSessionInfo[i].SessionId == dwSessionId)
                    {
                        _tcscpy_s(szSessionName, WINSTATIONNAME_LENGTH + 1, pSessionInfo[i].pWinStationName);
                        break;
                    }
                }
                WTSFreeMemory(pSessionInfo);
            }
        }
        else
        {
            _tcscpy_s(szSessionName, WINSTATIONNAME_LENGTH + 1, TEXT("Unknown"));
        }

        // Get memory usage
        PROCESS_MEMORY_COUNTERS pmc;
        if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
        {
            dwMemoryUsage = pmc.WorkingSetSize;
        }

        // Get process status
        DWORD exitCode;
        if (GetExitCodeProcess(hProcess, &exitCode))
        {
            if (exitCode == STILL_ACTIVE)
                _tcscpy_s(szStatus, MAX_PATH, TEXT("Running"));
            else
                _tcscpy_s(szStatus, MAX_PATH, TEXT("Not Running"));
        }

        // Get username who started the process
        HANDLE hToken;
        if (OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
        {
            DWORD dwSize = MAX_PATH;
            if (GetTokenInformation(hToken, TokenUser, NULL, 0, &dwSize) == FALSE)
            {
                PTOKEN_USER pTokenUser = (PTOKEN_USER)malloc(dwSize);
                if (pTokenUser != NULL)
                {
                    if (GetTokenInformation(hToken, TokenUser, pTokenUser, dwSize, &dwSize))
                    {
                        SID_NAME_USE sidUse;
                        DWORD dwNameSize = MAX_PATH;
                        DWORD dwDomainSize = MAX_PATH;
                        TCHAR szName[MAX_PATH];
                        TCHAR szDomain[MAX_PATH];
                        if (LookupAccountSid(NULL, pTokenUser->User.Sid, szName, &dwNameSize, szDomain, &dwDomainSize, &sidUse))
                        {
                            _stprintf_s(szUsername, MAX_PATH, TEXT("%s\\%s"), szDomain, szName);
                        }
                    }
                    free(pTokenUser);
                }
            }
            CloseHandle(hToken);
        }

        // Get CPU time
        FILETIME createTime, exitTime, kernelTime, userTime;
        if (GetProcessTimes(hProcess, &createTime, &exitTime, &kernelTime, &userTime))
        {
            ULARGE_INTEGER time;
            time.LowPart = userTime.dwLowDateTime;
            time.HighPart = userTime.dwHighDateTime;
            dwCpuTime = time.QuadPart / 10000000;
        }

        // Get window title
        HWND hWnd = GetTopWindow(NULL);
        while (hWnd != NULL)
        {
            DWORD_PTR dwWindowProcessId;
            GetWindowThreadProcessId(hWnd, reinterpret_cast<LPDWORD>(&dwWindowProcessId));
            if (dwWindowProcessId == processID)
            {
                GetWindowText(hWnd, szWindowTitle, MAX_PATH);
                break;
            }
            hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
        }

        // Print process details
        _tprintf(TEXT("%s  (PID: %u, Session ID: %u, Session Name: %s, Memory Usage: %Iu KB)\n"),
            szProcessName, processID, dwSessionId, szSessionName, dwMemoryUsage / 1024);

        if (verbose)
        {
            _tprintf(TEXT("  Status: %s\n"), szStatus);
            _tprintf(TEXT("  Username: %s\n"), szUsername);
            _tprintf(TEXT("  CPU Time: %u seconds\n"), dwCpuTime);
            _tprintf(TEXT("  Window Title: %s\n"), szWindowTitle);
        }

        if (showServices)
        {
            // Print service information
            HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID);
            if (hSnapshot != INVALID_HANDLE_VALUE)
            {
                MODULEENTRY32 me32 = { sizeof(me32) };
                if (Module32First(hSnapshot, &me32))
                {
                    do
                    {
                        _tprintf(TEXT("  Service: %s\n"), me32.szModule);
                    } while (Module32Next(hSnapshot, &me32));
                }
                CloseHandle(hSnapshot);
            }
        }

        CloseHandle(hProcess);
    }
}

int main(int argc, char* argv[])
{
    bool verbose = false;
    bool showServices = false;

    // Check for command-line switches
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "/v") == 0)
        {
            verbose = true;
        }
        else if (strcmp(argv[i], "/svc") == 0)
        {
            showServices = true;
        }
    }

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        return 1;
    }

    cProcesses = cbNeeded / sizeof(DWORD);

    for (unsigned int i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] != 0)
        {
            PrintProcessDetails(aProcesses[i], verbose, showServices);
        }
    }

    return 0;
}