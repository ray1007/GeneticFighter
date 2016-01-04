#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>

using namespace std;


int getLF2Pid(string targetEXE){
    int pid=0;
    FILE *fp;
    char buffer[1024];

    /* Open the command for reading. */
    fp = _popen("tasklist", "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }

    /* Read the output a line at a time - output it. */
    while (fgets(buffer, sizeof(buffer)-1, fp) != NULL) {
        bool match = true;
        string line(buffer);
        size_t found = line.find(targetEXE);
        if (found!=string::npos){
            sscanf(buffer, "%*s %i %*s %*i %*s",&pid);
        }
    }

    /* close */
    _pclose(fp);

    return pid;
}


int main(int argc, char* argv[])
{
    if (argc == 1) 
        ExitProcess(1);

    HANDLE hFile;
    string targetEXE(argv[1]);
    //cout<< targetEXE <<"\n";

    DWORD process_id = getLF2Pid(targetEXE);
    if( !FreeConsole()){
        //cout<<"fail\n";
    }

    if (AttachConsole(process_id)) {
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hStdOut != NULL)
        {
            DWORD dwWritten;
            //WriteFile(hStdOut, "hello world", 11, &dwWritten, NULL);
            bool success;

            char buffer[80];
            CHAR_INFO pStr[80];
            COORD coordBufSize; 
            COORD coordBufCoord;
            SMALL_RECT srctReadRect;  
            
            // The buffer size is 1 rows x 80 columns. 
            coordBufSize.Y = 1; 
            coordBufSize.X = 80; 

            // The top left destination cell of the buffer is row 0, col 0. 
            coordBufCoord.X = 0; 
            coordBufCoord.Y = 0; 

            srctReadRect.Top = 0;    // top left: row 0, col 0 
            srctReadRect.Left = 0; 
            srctReadRect.Bottom = 1; // bot. right: row 1, col 79 
            srctReadRect.Right = 79; 
            
            bool fin = false;
            while(true){
                success = ReadConsoleOutput(
                    hStdOut,    // screen buffer to read from
                    pStr,       // buffer to copy into 
                    coordBufSize,   // col-row size of buffer 
                    coordBufCoord,  // top left dest. cell in buffer
                    &srctReadRect
                );
                if (! success) {
                    printf("ReadConsoleOutput failed - (%d)\n", GetLastError()); 
                    for (int i=0;i<80;++i)
                        cout<<pStr[i].Char.AsciiChar;
                    return 1;
                }

                for (int i=0;i<80;++i){
                    buffer[i] = pStr[i].Char.AsciiChar;

                    if('0' <= buffer[i] ){
                        //cout<<":o\n";
                        fin = true;
                    }
                }
                if(fin)
                    break;
            }
            //cout<<"lol\n";

            hFile = CreateFile(
                        "fitness.tmp",          // name of the write
                        GENERIC_READ|GENERIC_WRITE,          // open for writing
                        FILE_SHARE_READ|FILE_SHARE_DELETE,  // do share
                        NULL,                   // default security
                        CREATE_ALWAYS,             // create new file only
                        FILE_ATTRIBUTE_NORMAL,  // normal file
                        NULL);
            if (GetLastError() != 0) 
                cout<<"lerr"<<GetLastError()<<"\n";
            success = WriteFile( 
                    hFile,           // open file handle
                    buffer,          // start of data to write
                    80,              // number of bytes to write
                    &dwWritten, // number of bytes that were written
                    NULL);
            if (! success) {
                cout<<"bad"<<GetLastError()<<"\n";
                ExitProcess(1);
            }
            CloseHandle(hFile);
        }
        else{ // error handling
            cout<<"lol2\n";   
        }   
        FreeConsole();   
    }
    else{ // error handling   
        cout<<"lol3\n";
        cout<<GetLastError();
    }   

    ExitProcess(0);;
}