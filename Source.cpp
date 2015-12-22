#include <windows.h>
#include<iostream>
using namespace std;




void press_key(HWND window, byte key, int times = 1){
	for (int i = 0; i < times; ++i){
		SendMessage(window, WM_KEYDOWN, key, 0);
		Sleep(33);
		SendMessage(window, WM_KEYUP, key, 0);
		Sleep(33);
	}
}

int main()
{
	int vs = 0, stage = 0; 
	HWND window_vs = 0, window_stage = 0;
	while (~(vs | stage)){
		vs = ~(~GetKeyState('V') & 0x80);
		stage = ~(~GetKeyState('S') & 0x80);
	}
	if (vs == -1){
		system("start C:/Users/user/Desktop/lf2/train_vs.exe");
		Sleep(300);
		window_vs = FindWindow(NULL, "Little Fighter 2");
		SetWindowText(window_vs, "train_vs");
	}
	else {
		system("start C:/Users/user/Desktop/lf2/train_stage.exe");
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
		while (true){
			//while (~GetKeyState('K') & 0x80){}
			if (GetKeyState(VK_ESCAPE) & 0x80) break;
			Sleep(20000);
			press_key(window_stage, VK_F4);
			Sleep(100);
			press_key(window_stage, 'K');
		}
	}

	system("PAUSE");
	return 0;
}