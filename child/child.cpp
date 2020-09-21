#include "stdio.h"
#include "windows.h"

int main()
{
    HANDLE readHandle = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE writeHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD readedBytes, writedBytes;

    int x, divider = 0;
    int count_run = 0;

    int res;

    while (true) {
        if (!ReadFile(readHandle, &x, sizeof(int), &readedBytes, NULL))
            return -1;

        if (count_run == 0) {
            divider = x;
        }

        if (count_run > 0) {
            if (divider != 0)
                res = x / divider;
            else
                res = 0;
            WriteFile(writeHandle, &res, sizeof(int), &writedBytes, NULL);
        }
        count_run++;
    }

    return 0;
}