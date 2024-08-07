#ifndef TURBOC_HEADER

#define TURBOC_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>	// _getch()
#include <time.h>
#include <windows.h>
#include <windowsx.h>

int blockList[14] = { 0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6 };

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

void clrscr(); // 화면을 모두 지운다.

void gotoxy(int x, int y); // 커서를 x,y좌표로 이동시킨다.

void setcursortype(CURSOR_TYPE c); // 커서를 숨기거나 다시 표시한다.

void suffleBag(int blockList[14]);

#define delay(n) Sleep(n)                              // n/1000초만큼 시간 지연

#define randomize() srand((unsigned)time(NULL))         // 난수 발생기 초기화

#define random(n) (rand() % (n))                        //0~n까지의 난수 발생

#ifndef TURBOC_PROTOTYPE_ONLY // 이 매크로가 정의되어 있으면 함수의 원형만 선언하고 정의는 하지 않는다.

void clrscr() {
	system("cls");
}

void gotoxy(int x, int y) {
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void setcursortype(CURSOR_TYPE c) {
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

void suffleBag(int blockBag[14]) { // (사톨로 알고리즘 배열 섞기)
	int len = 14;
	int i;
	int dump;
	int a[14];
	memcpy(a, blockList, sizeof(int) * 14);
	while (len > 1) {
		len = len - 1;
		i = random(len);
		dump = a[i];
		a[i] = a[len];
		a[len] = dump;
	}
	memcpy(blockBag, a, sizeof(int) * 14);
}

void printTitleScreen(int x, int y) {
	gotoxy(x, y + 0); printf("              ");
	gotoxy(x, y + 1); printf("■■■■■■■");
	gotoxy(x, y + 2); printf("■            ");
	gotoxy(x, y + 3); printf("■            ");
	gotoxy(x, y + 4); printf("■            ");
	gotoxy(x, y + 5); printf("■■■■■■■");
	gotoxy(x, y + 6); printf("■            ");
	gotoxy(x, y + 7); printf("■            ");
	gotoxy(x, y + 8); printf("■            ");
	gotoxy(x, y + 9); printf("■■■■■■■");
	gotoxy(x, y + 10); printf("              ");
	x = x + 16;
	gotoxy(x, y + 0); printf("      ■    ■");
	gotoxy(x, y + 1); printf("      ■    ■");
	gotoxy(x, y + 2); printf("      ■    ■");
	gotoxy(x, y + 3); printf("      ■    ■");
	gotoxy(x, y + 4); printf("      ■    ■");
	gotoxy(x, y + 5); printf("■■■■■■■");
	gotoxy(x, y + 6); printf("      ■    ■");
	gotoxy(x, y + 7); printf("      ■    ■");
	gotoxy(x, y + 8); printf("      ■    ■");
	gotoxy(x, y + 9); printf("      ■    ■");
	gotoxy(x, y + 10); printf("      ■    ■");
	x = x + 16;
	gotoxy(x, y + 0); printf("■■■■■■■");
	gotoxy(x, y + 1); printf("■            ");
	gotoxy(x, y + 2); printf("■            ");
	gotoxy(x, y + 3); printf("■■■■■■■");
	gotoxy(x, y + 4); printf("■            ");
	gotoxy(x, y + 5); printf("■            ");
	gotoxy(x, y + 6); printf("■■■■■■■");
	gotoxy(x, y + 7); printf("              ");
	gotoxy(x, y + 8); printf("              ");
	gotoxy(x, y + 9); printf("■■■■■■■");
	gotoxy(x, y + 10); printf("              ");

	x = x + 16;
	gotoxy(x, y + 0); printf("■■■■■■■");
	gotoxy(x, y + 1); printf("            ■");
	gotoxy(x, y + 2); printf("            ■");
	gotoxy(x, y + 3); printf("            ■");
	gotoxy(x, y + 4); printf("■■■■■■■");
	gotoxy(x, y + 5); printf("■            ");
	gotoxy(x, y + 6); printf("■            ");
	gotoxy(x, y + 7); printf("■            ");
	gotoxy(x, y + 8); printf("■            ");
	gotoxy(x, y + 9); printf("■■■■■■■");
	gotoxy(x, y + 10); printf("              ");
	x = x + 16;
	gotoxy(x, y + 0); printf("    ■    ");
	gotoxy(x, y + 1); printf("    ■    ");
	gotoxy(x, y + 2); printf("    ■    ");
	gotoxy(x, y + 3); printf("    ■    ");
	gotoxy(x, y + 4); printf("    ■    ");
	gotoxy(x, y + 5); printf("    ■    ");
	gotoxy(x, y + 6); printf("    ■    ");
	gotoxy(x, y + 7); printf("    ■    ");
	gotoxy(x, y + 8); printf("    ■    ");
	gotoxy(x, y + 9); printf("    ■    ");
	gotoxy(x, y + 10); printf("    ■    ");
	x = x + 10;
	gotoxy(x, y + 0); printf("      ■      ");
	gotoxy(x, y + 1); printf("    ■  ■    ");
	gotoxy(x, y + 2); printf("  ■      ■  ");
	gotoxy(x, y + 3); printf("■          ■");
	gotoxy(x, y + 4); printf("              ");
	gotoxy(x, y + 5); printf("              ");
	gotoxy(x, y + 6); printf("■■■■■■■");
	gotoxy(x, y + 7); printf("              ");
	gotoxy(x, y + 8); printf("              ");
	gotoxy(x, y + 9); printf("              ");
	gotoxy(x, y + 10); printf("              ");
}

#endif // TURBOC_PROTOTYPE_ONLY

#endif // TURBOC_HEADER