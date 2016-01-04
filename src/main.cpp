#include <windows.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <iostream>
#include <string>
#include <ctime>

#include "../sga-c++/statistics.h"
#include "../sga-c++/ga.h"
#include "../sga-c++/chromosome.h"
#include "../sga-c++/global.h"

#pragma comment(lib, "user32.lib")

#define BUFSIZE 4096 

using namespace std;



/*  global variables  */
const char fitnessFilename[] = "fitness.tmp";

/*  function prototypes */
BOOL WINAPI ConsoleHandler(DWORD);
int ReadFitness(void);

void press_key(HWND window, byte key, int times );

void vs_setup(HWND window_vs);

double vs_one_run(HWND window_vs);

void stage_setup(HWND window_stage);

void stage_one_run(HWND window_stage);

void update_as(int* currentGene, int len);

 
/* main function */
int main(int argc, char *argv[])
{
    /* enable Ctrl-C signal as an interface */
    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler,TRUE)) {
        fprintf(stderr, "Unable to install handler!\n");
        return EXIT_FAILURE;
    }

    /* main program */
    if (argc != 4 && argc != 5) {
        printf ("main nInitial nElite maxGen [spore_file]\n");
        return -1;
    }
    // GA hyperparameters
    int nInitial = atoi (argv[1]); // initial population size
    int nElite = atoi (argv[2]); // selection pressure
    int maxGen = atoi (argv[3]); // max generation
    int seqAlph = 9;    // alphabet size                           
    int selectionPressure = 2; // selection pressure
    double pc = 1;  // pc
    double pm = 0;  // pm
    int maxFe = -1;
    int posLen = 1;


    // GA initialization
    GA ga (seqAlph, posLen, nInitial, nElite, 
        selectionPressure, pc, pm, maxFe);

    if(argc == 5)
        ga.loadfile(argv[4]);

    // loop variables
    int i, j, k, seqLen;

    // start LF2
    int vs = 0, stage = 0; 
    HWND window_vs = 0, window_stage = 0;
    while (~(vs | stage)){
        vs = ~(~GetKeyState('V') & 0x80);
        stage = ~(~GetKeyState('S') & 0x80);
    }
    if (vs == -1){
        //system("start C:/Users/user/Desktop/lf2/train_vs.exe");
        //system("start C:\Users\user\Desktop\GA\LittleFighter\train_vs.exe");
        //system("start lf2_fast.exe.lnk");
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
		vs_setup(window_vs);

        /*  if first generation, do initial evaluation */
        //if(argc != 5){
        if(true){
            double* allFitness = new double[ga.getNChromosome()];
            
            for(j = 0; j < ga.getNChromosome(); j++){
                clock_t begin = clock();

                int* currentGene = ga.getChromosomeGene(j, seqLen);
                for(int x=0; x<posLen+seqLen; ++x)
                    printf("%i ", currentGene[x]);
                printf("\n");
                update_as(currentGene, posLen+seqLen);
                allFitness[j] = vs_one_run(window_vs);

                clock_t end = clock();
                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                printf("init evaluation chromosome:%i: time elapsed:%f\n", j, elapsed_secs);
            }
            ga.setPopulationFitness(allFitness);
            delete[] allFitness;
        }

        for (i = 0; i < maxGen; i++) {
			// start of generation
            ga.oneRun(true);

            // evaluate chromosomes after crossover.
            double* allFitness = new double[ga.getNChromosome()];
            
            for(j = 0; j < ga.getNChromosome(); j++){
                clock_t begin = clock();

                int* currentGene = ga.getChromosomeGene(j, seqLen);
                for(int x=0; x<posLen+seqLen; ++x)
                    printf("%i ", currentGene[x]);
                printf("\n");
                update_as(currentGene, posLen+seqLen);
                allFitness[j] = vs_one_run(window_vs);

                clock_t end = clock();
                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                printf("gen:%i chromosome:%i, time elapsed:%f\n", i, j, elapsed_secs);
            }
            ga.setPopulationFitness(allFitness);
            delete[] allFitness;
            //ga.oneRun(true);
            ga.pickElite();
            string fname = "vs_"+to_string(ga.getGeneration())+".spore";
            ga.savefile(fname.c_str());
        }
    }
    else{
		stage_setup(window_stage);
        /*  start cmd and wait for response */
        
        while (true){
			stage_one_run(window_stage);
        }
    }
	system("pause");
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


int ReadFitness(void) { 
    DWORD dwRead, dwWritten; 
    CHAR chBuf[BUFSIZE]; 
    DWORD dwTotalAvailBytes;
    DWORD dwBytesLeft;
    BOOL bSuccess = FALSE;
    HANDLE hFile;
    int fitness;
    //printf("yo:D\n");
    hFile = CreateFile(
        fitnessFilename, 
        GENERIC_READ, 
        FILE_SHARE_READ|FILE_SHARE_DELETE, 
        NULL, 
        OPEN_EXISTING, 
        FILE_ATTRIBUTE_READONLY, 
        NULL);

    bSuccess = ReadFile( hFile, chBuf, BUFSIZE, &dwRead, NULL);
    //printf("hide n seek\n");
	if (!bSuccess || dwRead == 0){
		ExitProcess(1);
	}
    fitness = atoi(chBuf);
    CloseHandle(hFile);
    //system("del fitness.tmp");
    return fitness;
} 

void press_key(HWND window, byte key, int times = 1){
	for (int i = 0; i < times; ++i){
		SendMessage(window, WM_KEYDOWN, key, 0);
		Sleep(33);
		SendMessage(window, WM_KEYUP, key, 0);
		Sleep(33);
	}
}

/*void vs_setup(HWND window_vs){
	press_key(window_vs, 'K');
	Sleep(150);
	press_key(window_vs, 'K', 3);
	press_key(window_vs, VK_SPACE, 5);
	press_key(window_vs, 'L');
	press_key(window_vs, 'K', 3);
	press_key(window_vs, 'J', 2);
	press_key(window_vs, 'K', 2);
	press_key(window_vs, 'I', 2);
}*/

void vs_setup(HWND window_vs){
    press_key(window_vs, 'K');
    Sleep(150);
    press_key(window_vs, 'K');
    press_key(window_vs, 'L');
    press_key(window_vs, 'K');
    press_key(window_vs, 'L');
    press_key(window_vs, 'K');
    press_key(window_vs, VK_SPACE, 5);
    press_key(window_vs, 'L');
    press_key(window_vs, 'K');
    press_key(window_vs, 'J', 2);
    press_key(window_vs, 'K');
    press_key(window_vs, 'L');
    press_key(window_vs, 'K');
    press_key(window_vs, 'L');
    press_key(window_vs, 'K',2);
    press_key(window_vs, 188);
    press_key(window_vs, 'K', 3);
    press_key(window_vs, 'I', 3);
}   

/*void vs_one_run(HWND window_vs){
	press_key(window_vs, VK_F4);
	Sleep(100);
	press_key(window_vs, 188, 2);
	press_key(window_vs, 'K');
	press_key(window_vs, 'I', 2);
	press_key(window_vs, 'K');
    //system("grabConsole train_vs");
	system("grabConsole lf2");
	cout << "We've got : " << ReadFitness() << "\n";
}*/

double vs_one_run(HWND window_vs){
    press_key(window_vs, 'K');
    system("grabConsole train_vs");
    //system("grabConsole lf2");
    double f = ReadFitness();
    cout << "We've got : " << f << "\n";
    //while(true);

    press_key(window_vs, VK_F4);
    Sleep(150);
    return f;
}    

void stage_setup(HWND window_stage){
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
}

void stage_one_run(HWND window_stage){
	
	press_key(window_stage, 'K');
	system("grabConsole train_stage");
	cout << "We've got : " << ReadFitness() << "\n";
    press_key(window_stage, VK_F4);
    Sleep(500);
}

void update_as(int* currentGene, int len){
	int line2write = 13;
	int line = 1;
	string str;
	ofstream buff("LittleFighter/ai/buffer.as", ios::out);
	ifstream input("LittleFighter/ai/10.as", ifstream::in);
	while (getline(input, str))
	{
		if (line++ == line2write){
			buff << "array<int> AttSeq = {";
			for (int i = 0; i < len; ++i){
				buff << currentGene[i];
				if (i != len - 1)
					buff << ",";
			}
			buff << "};" << endl;
		}
		else
			buff << str << endl;
	}
    input.close();
	buff.close();
	system("copy LittleFighter\\ai\\buffer.as LittleFighter\\ai\\10.as /Y");
    Sleep(150);
}

