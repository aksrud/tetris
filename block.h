#ifndef TURBOC_HEADER

#define TURBOC_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>	// _getch()
#include <time.h>
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <Digitalv.h>
#pragma comment(lib,"winmm.lib")

MCI_OPEN_PARMS open[10];

int dwID[20]; //음악 파일 불러오는 값

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

enum { EMPTY, BRICK, WALL };	// 빈 공간: 0, 벽돌: 1, 벽: 2

char* arTile[] = { ". ","■","▦" }; // 열거형의 값과 배열의 요소가 대응된다.

int blocks[7][4][4][4] = {
	{
	 {{0,0,0,0},
	 {0,1,1,0},
	 {0,1,1,0},
	 {0,0,0,0}},

	 {{0,0,0,0},
	 {0,1,1,0},
	 {0,1,1,0},
	 {0,0,0,0}},

	 {{0,0,0,0},
	 {0,1,1,0},
	 {0,1,1,0},
	 {0,0,0,0}},

	 {{0,0,0,0},
	 {0,1,1,0},
	 {0,1,1,0},
	 {0,0,0,0}}
	},

	{
	 {{0,0,0,0},
	 {0,0,0,0},
	 {1,1,1,1},
	 {0,0,0,0}},

	 {{0,1,0,0},
	 {0,1,0,0},
	 {0,1,0,0},
	 {0,1,0,0}},

	 {{0,0,0,0},
	 {0,0,0,0},
	 {1,1,1,1},
	 {0,0,0,0}},

	 {{0,1,0,0},
	 {0,1,0,0},
	 {0,1,0,0},
	 {0,1,0,0}}
	},

	{
	 {{0,0,0,0},
	 {1,1,0,0},
	 {0,1,1,0},
	 {0,0,0,0}},

	 {{0,0,0,0},
	 {0,0,1,0},
	 {0,1,1,0},
	 {0,1,0,0}},

	 {{0,0,0,0},
	 {1,1,0,0},
	 {0,1,1,0},
	 {0,0,0,0}},

	 {{0,0,0,0},
	 {0,0,1,0},
	 {0,1,1,0},
	 {0,1,0,0}}
	},

	{
	 {{0,0,0,0},
	 {0,1,1,0},
	 {1,1,0,0},
	 {0,0,0,0}},

	 {{0,0,0,0},
	 {1,0,0,0},
	 {1,1,0,0},
	 {0,1,0,0}},

	 {{0,0,0,0},
	 {0,1,1,0},
	 {1,1,0,0},
	 {0,0,0,0}},

	 {{0,0,0,0},
	 {1,0,0,0},
	 {1,1,0,0},
	 {0,1,0,0}}
	},

	{
	 {{0,0,0,0},
	 {0,0,1,0},
	 {1,1,1,0},
	 {0,0,0,0}},

	 {{0,0,0,0},
	 {0,1,0,0},
	 {0,1,0,0},
	 {0,1,1,0}},

	 {{0,0,0,0},
	 {0,0,0,0},
	 {1,1,1,0},
	 {1,0,0,0}},

	 {{0,0,0,0},
	 {1,1,0,0},
	 {0,1,0,0},
	 {0,1,0,0}}
	},

	{
	 {{0,0,0,0},
	 {1,0,0,0},
	 {1,1,1,0},
	 {0,0,0,0}},

	 {{0,0,0,0},
	 {0,1,1,0},
	 {0,1,0,0},
	 {0,1,0,0}},

	 {{0,0,0,0},
	 {0,0,0,0},
	 {1,1,1,0},
	 {0,0,1,0}},

	 {{0,0,0,0},
	 {0,1,0,0},
	 {0,1,0,0},
	 {1,1,0,0}}
	},

	{
	 {{0,0,0,0},
	 {0,1,0,0},
	 {1,1,1,0},
	 {0,0,0,0}},

	 {{0,0,0,0},
	 {0,1,0,0},
	 {0,1,1,0},
	 {0,1,0,0}},

	 {{0,0,0,0},
	 {0,0,0,0},
	 {1,1,1,0},
	 {0,1,0,0}},

	 {{0,0,0,0},
	 {0,1,0,0},
	 {1,1,0,0},
	 {0,1,0,0}}
	}
};

void clrscr();

void gotoxy(int x, int y);

int wherex();

int wherey();

void setcursortype(CURSOR_TYPE c);

#define delay(n) Sleep(n)                              // n/1000초만큼 시간 지연

#define randomize() srand((unsigned)time(NULL))         // 난수 발생기 초기화

#define random(n) (rand() % (n))                        //0~n까지의 난수 발생



// 이 매크로가 정의되어 있으면 함수의 원형만 선언하고 정의는 하지 않는다.

#ifndef TURBOC_PROTOTYPE_ONLY



// 화면을 모두 지운다.

void clrscr()
{
	system("cls");
}



// 커서를 x,y좌표로 이동시킨다.

void gotoxy(int x, int y)

{

	COORD Cur;

	Cur.X = x;

	Cur.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);

}



// 커서의 x 좌표를 조사한다.

int wherex()

{

	CONSOLE_SCREEN_BUFFER_INFO BufInfo;



	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);

	return BufInfo.dwCursorPosition.X;

}



// 커서의 y좌표를 조사한다.

int wherey()

{

	CONSOLE_SCREEN_BUFFER_INFO BufInfo;



	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);

	return BufInfo.dwCursorPosition.Y;

}



// 커서를 숨기거나 다시 표시한다.

void setcursortype(CURSOR_TYPE c)

{

	CONSOLE_CURSOR_INFO CurInfo;



	switch (c) {

	case NOCURSOR:

		CurInfo.dwSize = 1;

		CurInfo.bVisible = FALSE;

		break;

	case SOLIDCURSOR:

		CurInfo.dwSize = 100;

		CurInfo.bVisible = TRUE;

		break;

	case NORMALCURSOR:

		CurInfo.dwSize = 20;

		CurInfo.bVisible = TRUE;

		break;

	}

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);

}

void mainSound(int a)
{
	open[0].lpstrElementName = TEXT("sound/background.wav");
	open[0].lpstrDeviceType = TEXT("waveaudio");
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&open[0]);
	dwID[0] = open[0].wDeviceID;
	if (a == 1)		
		mciSendCommand(dwID[0], MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&open[0]);
	else	
		mciSendCommand(dwID[0], MCI_STOP, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&open[0]);
}

void playSoundLine()
{
	open[1].lpstrElementName = TEXT("sound/line.wav");
	open[1].lpstrDeviceType = TEXT("waveaudio");
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&open[1]);
	dwID[1] = open[1].wDeviceID;
	mciSendCommand(dwID[1], MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
	mciSendCommand(dwID[1], MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&open[1]);
}

void playSoundDrop()
{
	open[2].lpstrElementName = TEXT("sound/drop.wav");
	open[2].lpstrDeviceType = TEXT("waveaudio");
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&open[2]);
	dwID[2] = open[2].wDeviceID;
	mciSendCommand(dwID[2], MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
	mciSendCommand(dwID[2], MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&open[2]);
}

void playSoundHold()
{
	open[3].lpstrElementName = TEXT("sound/hold.wav");
	open[3].lpstrDeviceType = TEXT("waveaudio");
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&open[3]);
	dwID[3] = open[3].wDeviceID;
	mciSendCommand(dwID[3], MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
	mciSendCommand(dwID[3], MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&open[3]);
}

void playSoundRotate()
{
	open[4].lpstrElementName = TEXT("sound/rotate.wav");
	open[4].lpstrDeviceType = TEXT("waveaudio");
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&open[4]);
	dwID[4] = open[4].wDeviceID;
	mciSendCommand(dwID[4], MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
	mciSendCommand(dwID[4], MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&open[4]);
}

void playSoundMove()
{
	open[5].lpstrElementName = TEXT("sound/move.wav");
	open[5].lpstrDeviceType = TEXT("waveaudio");
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&open[5]);
	dwID[5] = open[5].wDeviceID;
	mciSendCommand(dwID[5], MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
	mciSendCommand(dwID[5], MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&open[5]);
}

void playSoundSoftDrop()
{
	open[6].lpstrElementName = TEXT("sound/softdrop.wav");
	open[6].lpstrDeviceType = TEXT("waveaudio");
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&open[6]);
	dwID[6] = open[6].wDeviceID;
	mciSendCommand(dwID[6], MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
	mciSendCommand(dwID[6], MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&open[6]);
}

void playSoundHardDrop()
{
	open[7].lpstrElementName = TEXT("sound/harddrop.wav");
	open[7].lpstrDeviceType = TEXT("waveaudio");
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&open[7]);
	dwID[7] = open[7].wDeviceID;
	mciSendCommand(dwID[7], MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
	mciSendCommand(dwID[7], MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&open[7]);
}


#endif // TURBOC_PROTOTYPE_ONLY

#endif // TURBOC_HEADER