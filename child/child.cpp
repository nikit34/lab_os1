#include "stdio.h"
#include "windows.h"
#include "string"
#include <iostream>

 
int main() {
    HANDLE readHandle = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE writeHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD readedBytes, writedBytes;

    int x, divider = 0;
    int count_run = 0;

    int res;
    std::string str_res;

    HANDLE hFile = CreateFile(
        L"logs.txt",
        GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    while (true) {
        if (!ReadFile(readHandle, &x, sizeof(int), &readedBytes, NULL))
            return -1;

        if (count_run == 0) {
            divider = x;
        }

        if (count_run > 0) {
            if (hFile == INVALID_HANDLE_VALUE)
                return -1;

            if (divider != 0) {
                res = x / divider;
                str_res = std::to_string(res);
                WriteFile(hFile, str_res.c_str(), str_res.size(), &writedBytes, NULL);
                WriteFile(writeHandle, &res, sizeof(int), &writedBytes, NULL);
            }
            else {
                WriteFile(writeHandle, "E", 1, &writedBytes, NULL);
                //res = 0;
            }



        }
        count_run++;
    }

    CloseHandle(hFile);

    return 0;
}