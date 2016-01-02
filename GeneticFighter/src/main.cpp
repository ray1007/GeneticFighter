#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <iostream>

#pragma comment(lib, "user32.lib")

#define BUFSIZE 4096 

using namespace std;



/*  global variables  */
const char fitnessFilename[] = "fitness.tmp";


void press_key(HWND window, byte key, int times = 1){
    for (int i = 0; i < times; ++i){
        SendMessage(window, WM_KEYDOWN, key, 0);
        Sleep(33);
        SendMessage(window, WM_KEYUP, key, 0);
        Sleep(33);
    }
}

/*  function prototypes */
BOOL WINAPI ConsoleHandler(DWORD);
double ReadFitness(void);

 
/* main function */
int main(int argc, char *argv[])
{
    /* enable Ctrl-C signal as an interface */
    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler,TRUE)) {
        fprintf(stderr, "Unable to install handler!\n");
        return EXIT_FAILURE;
    }

    /* main program */
    int vs = 0, stage = 0; 
    HWND window_vs = 0, window_stage = 0;
    while (~(vs | stage)){
        vs = ~(~GetKeyState('V') & 0x80);
        stage = ~(~GetKeyState('S') & 0x80);
    }
    if (vs == -1){
        //system("start C:/Users/user/Desktop/lf2/train_vs.exe");
        //system("start C:\Users\user\Desktop\GA\LittleFighter\train_vs.exe");
        system("start train_vs.exe.lnk");
        Sleep(300);
        window_vs = FindWindow(NULL, "Little Fighter 2");
        SetWindowText(window_vs, "train_vs");
    }
    else {
        //system("start C:/Users/user/Desktop/lf2/train_stage.exe");
        system("start train_stage.exe.lnk");
        Sleep(300);
        window_stage = FindWindow(NULL, "Little Fighter 2");
        SetWindowText(window_stage, "train_stage");
        
    }
    while (~GetKeyState(VK_RETURN) & 0x80)
    {
    }
    if (vs == -1){
        press_key(window_vs, 'K');
        Sleep(150);
        press_key(window_vs, 'K', 3);
        press_key(window_vs, VK_SPACE, 5);
        press_key(window_vs, 'L');
        press_key(window_vs, 'K', 3);
        press_key(window_vs, 'J', 2);
        press_key(window_vs, 'K', 2);
        press_key(window_vs, 'I', 2);
        press_key(window_vs, 'K');

        /*  start cmd and wait for response */
        system("grabConsole train_vs");
        cout<<"We've got : "<<ReadFitness()<<"\n";

        while (true){
            //while (~GetKeyState('K') & 0x80){}
            if (GetKeyState(VK_ESCAPE) & 0x80) break;
            Sleep(5000);
            press_key(window_vs, VK_F4);
            Sleep(100);
            press_key(window_vs, 188, 2);
            press_key(window_vs, 'K');
            press_key(window_vs, 'I', 2);
            press_key(window_vs, 'K');
        }
    }
    else{
        press_key(window_stage, 188);
        press_key(window_stage, 'K');
        Sleep(150);
        press_key(window_stage, 'K', 2);
        press_key(window_stage, VK_SPACE, 5);
        press_key(window_stage, 'L');
        press_key(window_stage, 'K');
        press_key(window_stage, 'J', 2);
        press_key(window_stage, 'K');
        press_key(window_stage, 'I', 2);
        press_key(window_stage, 'K');
        
        /*  start cmd and wait for response */
        system("grabConsole train_stage");
        cout<<"We've got : "<<ReadFitness()<<"\n"; 
        
        while (true){
            //while (~GetKeyState('K') & 0x80){}
            if (GetKeyState(VK_ESCAPE) & 0x80) break;
            Sleep(20000);
            press_key(window_stage, VK_F4);
            Sleep(100);
            press_key(window_stage, 'K');
        }
    }

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


double ReadFitness(void) { 
    DWORD dwRead, dwWritten; 
    CHAR chBuf[BUFSIZE]; 
    DWORD dwTotalAvailBytes;
    DWORD dwBytesLeft;
    BOOL bSuccess = FALSE;
    HANDLE hFile;
    double fitness;

    hFile = CreateFile(
        fitnessFilename, 
        GENERIC_READ, 
        0, 
        NULL, 
        OPEN_EXISTING, 
        FILE_ATTRIBUTE_READONLY, 
        NULL);

    bSuccess = ReadFile( hFile, chBuf, BUFSIZE, &dwRead, NULL);
    //printf("hide n seek\n");
    if( ! bSuccess || dwRead == 0 ) 
        ExitProcess(1);
    //sscanf(chBuf, "%*s %i %*s %*i %*s",&fitness);
    
    system("del fitness.tmp");
    fitness = 0.5;
    return fitness;
} 