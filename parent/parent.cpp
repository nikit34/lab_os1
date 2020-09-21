#include "stdio.h"
#include "windows.h"

HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;

int CreateChildProcess();

int main()
{
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
        return -1;

    if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
        return -1;

    if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
        return -1;

    if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
        return -1;

    if (!CreateChildProcess())
        return -1;

    DWORD dwWritten, dwRead;
    

    char ch;
    int num = 0;
    int count_request = 0;

    int x, result;

    while (true) {
        scanf_s("%c", &ch, 1);

        if (ch == 0x20 || ch == 0X0A) {  // code space key or code enter key
            x = num;
            
            WriteFile(g_hChildStd_IN_Wr, &x, sizeof(int), &dwWritten, NULL);

            if (count_request != 0) {
                if (!ReadFile(g_hChildStd_OUT_Rd, &result, sizeof(int), &dwRead, NULL))
                    return -1;

                printf("Result call %d: %d\n", count_request, result);
            }
            count_request++;
            
            if (ch == 0x20) {
                num = 0;
                continue;
            }
                
            if (ch == 0x0A)
                break;
        }

        num = 10 * num + (ch - '0');
    }

    printf("Finish calc\n");
    
    return 0;
}

int CreateChildProcess()
{
    TCHAR szCmdline[] = TEXT("child.exe");
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE;

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    // Set up members of the STARTUPINFO structure. 
    // This structure specifies the STDIN and STDOUT handles for redirection.

    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = g_hChildStd_OUT_Wr;
    siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
    siStartInfo.hStdInput = g_hChildStd_IN_Rd;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    // Create the child process. 

    bSuccess = CreateProcess(NULL,
        szCmdline,     // command line 
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        TRUE,          // handles are inherited 
        0,             // creation flags 
        NULL,          // use parent's environment 
        NULL,          // use parent's current directory 
        &siStartInfo,  // STARTUPINFO pointer 
        &piProcInfo);  // receives PROCESS_INFORMATION 

     // If an error occurs, exit the application. 
    if (!bSuccess)
        return -1;
    else
    {
        CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);

        CloseHandle(g_hChildStd_OUT_Wr);
        CloseHandle(g_hChildStd_IN_Rd);
    }
}