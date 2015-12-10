#define WIN32_LEAN_AND_MEAN   
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

BOOL WINAPI ConsoleHandler(DWORD);

int main(int argc, char *argv[])
{
    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler,TRUE)) {
        fprintf(stderr, "Unable to install handler!\n");
        return EXIT_FAILURE;
    }

    for (;;)
        ; /* Null body. */

    return EXIT_SUCCESS;
}

BOOL WINAPI ConsoleHandler(DWORD dwType)
{
    switch(dwType) {
    case CTRL_C_EVENT:
        printf("ctrl-c\n");
        exit(0);
        break;
    case CTRL_BREAK_EVENT:
        printf("break\n");
        break;
    default:
        printf("Some other event\n");
    }
    return TRUE;
}