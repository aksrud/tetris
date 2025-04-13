#define _CRT_SECURE_NO_WARNINGS
#include "block.h"
#include "sound.h"

#define LEFT 75	// 왼쪽 방향키
#define RIGHT 77// 오른쪽 방향키
#define UP 72	// 위쪽 방향키
#define DOWN 80	// 아래 방향키	
#define ESC 27	// ESC 
#define C 67 // C
#define Z 90 // Z
#define ENTER 13 // 엔터

#define TOPX 15 // 타이틀 x
#define TOPY 2 // 타이틀 y

#define BX 40	// 외부 벽 X의 좌상단 좌표
#define BY 3	// 외부 벽 Y의 좌상단 좌표

#define BW 10	// 게임판의 폭
#define BH 20	// 게임판의 높이

int board[BW + 2][BH + 2]; // 게임판

int cx, cy; // 현재 위치
int rot; // 회전
int brick[2] = {-1, -1}; // 블럭 저장
int hold_block = -1; // 홀드 블럭

int score = 0; // 점수
int stage = 1; // 스테이지

int frame = 20; // 프레임
int nframe; // 실제 감소프레임

float wating; // 바닥에 닿을때 시간

int blockBag[14] = {0}; // 블록 랜덤 주머니
int bagCount = 0; // 랜덤 주머니 꽉찼는지 확인

int cursor = 0; // 타이틀 커서
int selectStage = 1; // 선택 스테이지

BOOL isHold = FALSE; // 홀드 하고 있나
BOOL isOut = FALSE; // 홀드한걸 꺼냈나
BOOL isLine = FALSE; // 라인 클리어
BOOL isDown = FALSE; // 바닥에 닿았는가
BOOL isForce = FALSE; // 스페이스바 누르면 바로 내려가는 것
BOOL isSelectLevel = FALSE; // 레벨 고르기 선택

void drawScreen(); // 전체 스크린
void drawBoard(); // 블럭 스크린
void printBrick(BOOL show); // 블럭 부분 출력
void start(); // 게임 시작
int getAround(int x, int y, int r); // 주변 벽 or 블럭 있는지 검사
BOOL moveDown(); // 아래로 내려가기
BOOL processKey(); // 키 입력
void testFull(); // 한 줄이 채워졌는지 확인
void next(); // 다음 블럭 보여주는 화면
void hold(); // 홀드하고 있는 블럭 보여주는 화면
void changeStage(); // 점수와 스테이지 갱신
void ssScreen(); // 점수와 스테이지를 보여주는 화면

// 타이틀 조작
void title();
void titleText();
void titleKey();
void selectLevelText();
void selectLevelKey();

void gameOver(); // 게임 오버
void winGame(); // 이김

int main() {
	randomize(); // 랜덤 시드 변경
	setcursortype(NOCURSOR); // 커서 없애기
	title();
	start();
	setcursortype(NORMALCURSOR);	// 게임 종료 후 커서를 표시
	return 0;
}

void titleText() {
	gotoxy(TOPX + 42, TOPY + 15);
	printf("START");
	gotoxy(TOPX + 42, TOPY + 17);
	printf("SELECT LEVEL");
}

void selectLevelText() {
	gotoxy(TOPX + 42, TOPY + 15);
	printf("YOUR LEVEL : %d", selectStage);
}

void title() {
	while (1) {
		if (cursor != -1) {
			printTitleScreen(TOPX, TOPY);
			gotoxy(TOPX + 38, TOPY + 15 + cursor * 2);
			printf("> >");
			if (!isSelectLevel) {
				titleText();
				titleKey();
			}
			else {
				selectLevelText();
				selectLevelKey();
			}
		}
		else {
			break;
		}
	}
}

void winGame() {
	//mainSound(0);
	clrscr();	// 화면 청소
	gotoxy(30, 9); puts("G A M E  S U C C E S S!!!!!");
	//playSoundSuccess();
	delay(2000);
}

void gameOver() {
	//mainSound(0);
	clrscr();	// 화면 청소
	gotoxy(30, 9); puts("G A M E  O V E R");
	//playSoundGameOver();
	delay(3600);
}

void start() {
	clrscr(); // 화면 클리어

	frame = 20;

	for (int x = 0; x < BW + 2; x++) {
		for (int y = 0; y < BH + 2; y++)
			board[x][y] = (y == 0 || y == BH + 1 || x == 0 || x == BW + 1) ? WALL : EMPTY;
	}

	drawScreen();
	mainSound(1);
	while (1) {
		if (score == 10000) {
			winGame();
			return;
		}
		if (!isOut) {
			/*if (brick[0] == -1)
				brick[0] = random(sizeof(blocks) / sizeof(blocks[0]));
			else
				brick[0] = brick[1];
			brick[1] = random(sizeof(blocks) / sizeof(blocks[0]));*/
			if (brick[0] == -1) {
				suffleBag(blockBag);
				brick[0] = blockBag[bagCount];
				bagCount = bagCount + 1;
			}
			else {
				brick[0] = brick[1];
			}
			brick[1] = blockBag[bagCount];
			bagCount = bagCount + 1;
			if (bagCount == 14){
				suffleBag(blockBag);
				bagCount = 0;
			}
		}
		isOut = FALSE;
		changeStage();

		next();
		hold();
		ssScreen();

		cx = BW / 2 - 1; // 이동중인 벽돌의 x좌표
		cy = BY - 3;	// 이동중인 벽돌의 y좌표
		rot = 0; // 회전을 하지 않은 상태

		printBrick(TRUE);	// 벽돌을 출력
		if (getAround(cx, cy, rot) != EMPTY) break;

		// 블록 뽑고 난후
		nframe = frame;
		while (1) {
			if (--nframe == 0) {	// 카운트하다가 0이 되면 MoveDown()을 호출한다.

				nframe = frame;	// 다음 블록을 위해 다시 대입

				if (moveDown()) break;	//	이동이 끝나면 break;
			}
			if (processKey()) break;
			delay(1000 / 20);
		}
	}
	gameOver();
}

void next() { // 다음 블록 보여주는 화면
	int next[8][7];
	gotoxy(BX + BW * 2 + 6, BY);
	printf("다음 : ");
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 7; y++) {
			next[x][y] = (y == 0 || y == 6 || x == 0 || x == 7) ? WALL : EMPTY;
			gotoxy((BX + BW * 2 + 6) + x * 2, BY + y + 2);
			printf(arTile[next[x][y]]);
		}
	}
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			gotoxy((BX + BW * 2 + 6) + x * 2 + 4, BY + y + 4);
			printf(arTile[blocks[brick[1]][0][y][x]]);
		}
	}
}

void hold() { // 현재 저장하고 있는 블록 보여주는 화면
	int hold[8][7];
	gotoxy(BX + BW * 2 + 6, BY + 10);
	printf("HOLD : ");
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 7; y++) {
			hold[x][y] = (y == 0 || y == 6 || x == 0 || x == 7) ? WALL : EMPTY;
			gotoxy((BX + BW * 2 + 6) + x * 2, BY + y + 12);
			printf(arTile[hold[x][y]]);
		}
	}
	if (hold_block != -1) {
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				gotoxy((BX + BW * 2 + 6) + x * 2 + 4, BY + y + 14);
				printf(arTile[blocks[hold_block][0][y][x]]);
			}
		}
	}
}

void drawScreen() {	// 화면 전체를 그린다.
	for (int x = 0; x < BW + 2; x++) {
		for (int y = 0; y < BH + 2; y++) {
			gotoxy(BX + x * 2, BY + y);	// 외부 벽의 좌상단 좌표에서부터 커서를 x*2, y만큼 이동시킴
			printf(arTile[board[x][y]]);	// board[x][y]에 저장된 열거형 값으로 게임판을 구성한다.
		}
	}
}

void drawBoard() { // 블록 화면을 그린다.
	for (int x = 1; x < BW + 1; x++) {
		for (int y = 1; y < BH + 1; y++) {

			gotoxy(BX + x * 2, BY + y);	// 외부 벽의 좌상단 좌표에서부터 커서를 x*2, y만큼 이동시킴

			puts(arTile[board[x][y]]);	// board[x][y]에 저장된 열거형 값으로 쌓여있는 벽돌을 그린다.
		}
	}
}

void printBrick(BOOL show) { // 블록 그리는 부분
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (blocks[brick[0]][rot][i][j]) {
				if (BY + i + cy > BY) {
					gotoxy(BX + (j + cx) * 2, BY + i + cy);
					printf(arTile[show ? BRICK : EMPTY]);
				}
			}
		}
	}
}

int getAround(int x, int y, int r){	// 벽돌 주변의 지형 검사
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (blocks[brick[0]][r][i][j]) {
				if (y + i > 0) {
					if (board[x + j][y + i]) {
						return WALL;
					}
				}
			}
		}
	}
	return EMPTY;
}

BOOL moveDown() { // 아래로 블럭 내리기
	if (getAround(cx, cy + 1, rot) != EMPTY) {	// 다음 ny좌표가 채워져 있으면, 이번 위치에서 이동을 끝내야 한다.
		if (!isForce) {
			isDown = TRUE;
			wating = 1;
			time_t start = time(NULL);
			time_t check = time(NULL);

			while (check - start < wating) {
				check = time(NULL);
				processKey();
			}
			if (getAround(cx, cy + 1, rot) == EMPTY) {
				isForce = TRUE;
				while (moveDown() == FALSE) {}	// 블록을 바닥으로 즉시 내린다.
				playSoundHardDrop();
			}
			isDown = FALSE;
			if (wating == -1) { // 바닥에서 홀드 버튼을 누를떄 예외 처리
				return TRUE;
			}
		}
		testFull();		// 삭제할 것이 있는지 검사한다.
		if (!isForce)
			playSoundSoftDrop();
		isHold = FALSE;
		return TRUE;	// 이동이 끝나면 true를 반환
	}

	printBrick(FALSE);	// 현재 벽돌을 삭제
	cy++;	// 이동중인 현재 벽돌의 y좌표를 증가시킨다.
	printBrick(TRUE);	// 한 칸 아래에 벽돌을 출력

	return FALSE;
}

BOOL processKey() { // 게임 중 키 입력
	int ch, trot;	// 키 입력, 회전 값 저장

	if (_kbhit()) {	// 키보드 입력이 있을 때까지 대기
		ch = _getch();
		if (ch == C + 32 || ch == C) {
			if (!isHold) { // 안들고 있을때
				if(hold_block == -1){
					printBrick(FALSE);
					isHold = TRUE;
					hold_block = brick[0];
					playSoundHold();
					wating = -1; // 바닥 예외
					return TRUE;
				} else{
					printBrick(FALSE);
					isHold = TRUE;
					isOut = TRUE;
					int dump = brick[0];
					brick[0] = hold_block;
					hold_block = dump;
					playSoundHold();
					wating = -1; // 바닥 예외
					return TRUE;
				}
			}
		}
		if (ch == Z+32 || ch == Z) {
			trot = (rot == 0 ? 3 : rot - 1);	// 다른 블록 모양으로 쌓을 수 있게 회전값을 지정

			// 벽에서의 회전 경우의 수중 회전 되는 경우의 수로 회전
			for (int i = 0; i <= 2; i++) {
				for (int j = 0; j <= 1; j++) {
					if (j) {
						if (getAround(cx + i, cy, trot) == EMPTY) {	// 벽돌 회전이 가능한 위치인지 검사
							printBrick(FALSE);	// 벽돌 삭제 (회전하기 전)
							cx += i;
							rot = trot;		// 회전값을 대입
							printBrick(TRUE);	// 벽돌 출력 (회전이 적용된 모양)
							playSoundRotate();
							wating += 0.05;
							return FALSE;
						}
					}
					else {
						if (getAround(cx - i, cy, trot) == EMPTY) {	// 벽돌 회전이 가능한 위치인지 검사
							printBrick(FALSE);	// 벽돌 삭제 (회전하기 전)
							cx -= i;
							rot = trot;		// 회전값을 대입
							printBrick(TRUE);	// 벽돌 출력 (회전이 적용된 모양)
							playSoundRotate();
							wating += 0.05;
							return FALSE;
						}
						if (getAround(cx, cy - i, trot) == EMPTY) {	// 벽돌 회전이 가능한 위치인지 검사
							printBrick(FALSE);	// 벽돌 삭제 (회전하기 전)
							cy -= i;
							rot = trot;		// 회전값을 대입
							printBrick(TRUE);	// 벽돌 출력 (회전이 적용된 모양)
							playSoundRotate();
							wating += 0.05;
							return FALSE;
						}
					}
				}
			}
		}
		if (ch == 0xE0 || ch == 0) {	// 확장 키(Function) 의 경우에 키를 하나 더 입력 받는다.
			// 방향키는 확장키에 포함된다.( 2바이트로 되어 있으므로 _getch()로 한번에 읽어들일 수 없다.)
			// 따라서 _getch()를 2번 호출해야 한다.
			ch = _getch();

			switch (ch) {	// 입력 값을 검사

			case LEFT:

				if (getAround(cx - 1, cy, rot) == EMPTY) {

					printBrick(FALSE);	// 이전 위치의 블록을 삭제한다.
					cx--;	// 왼쪽으로 한 칸 좌표 이동
					printBrick(TRUE);	// 왼쪽 방향키를 누른 후에, 블록을 새로 그린다.
					playSoundMove();
					wating += 0.05;
				}
				break;
			case RIGHT:
				if (getAround(cx + 1, cy, rot) == EMPTY) {

					printBrick(FALSE);	// 이전 위치의 블록을 삭제한다.
					cx++;	// 오른쪽으로 한 칸 좌표 이동
					printBrick(TRUE);	// 오른쪽 방향키를 누른 후에, 블록을 새로 그린다.
					playSoundMove();
					wating += 0.05;
				}

				break;
			case UP:
				trot = (rot == 3 ? 0 : rot + 1);	// 다른 블록 모양으로 쌓을 수 있게 회전값을 지정

				// 벽에서의 회전 경우의 수중 회전 되는 경우의 수로 회전
				for (int i = 0; i <= 2; i++) {
					for (int j = 0; j <= 1; j++) {
						if (j) {
							if (getAround(cx + i, cy, trot) == EMPTY) {	// 벽돌 회전이 가능한 위치인지 검사
								printBrick(FALSE);	// 벽돌 삭제 (회전하기 전)
								cx += i;
								rot = trot;		// 회전값을 대입
								printBrick(TRUE);	// 벽돌 출력 (회전이 적용된 모양)
								playSoundRotate();
								wating += 0.05;
								return FALSE;
							}
						}
						else {
							if (getAround(cx - i, cy, trot) == EMPTY) {	// 벽돌 회전이 가능한 위치인지 검사
								printBrick(FALSE);	// 벽돌 삭제 (회전하기 전)
								cx -= i;
								rot = trot;		// 회전값을 대입
								printBrick(TRUE);	// 벽돌 출력 (회전이 적용된 모양)
								playSoundRotate();
								wating += 0.05;
								return FALSE;
							}
							if (getAround(cx, cy - i, trot) == EMPTY) {	// 벽돌 회전이 가능한 위치인지 검사
								printBrick(FALSE);	// 벽돌 삭제 (회전하기 전)
								cy -= i;
								rot = trot;		// 회전값을 대입
								printBrick(TRUE);	// 벽돌 출력 (회전이 적용된 모양)
								playSoundRotate();
								wating += 0.05;
								return FALSE;
							}
						}
					}
				}
				break;
			case DOWN:
				if (!isDown) {
					wating -= 0.05;
					if (moveDown()) // 아래로 한 칸 내림.
						return TRUE;
				}
				break;
			}
		}
		else {

			switch (ch) {
			case ' ':		// 스페이스 키를 누르면 
				isForce = TRUE;
				while (moveDown() == FALSE) {}	// 블록을 바닥으로 즉시 내린다.
				playSoundHardDrop();
				isForce = FALSE;
				return TRUE;
				break;
			case '@':		// @을 누르면
				_kbhit();	// 일시 정지
				break;
			case '!':		// !를 누르면
				start();	// 새 게임 시작
				break;
			}
		}
	}
	return FALSE;
}

void testFull() { // 채워진 줄 있는지 검사
	int i, x, y, ty;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (blocks[brick[0]][rot][i][j]) {
				board[cx + j][cy + i] = BRICK;
			}
		}
	}

	for (y = 1; y < BH + 1; y++) {	// 높이만큼 반복하면서 검사
		for (x = 1; x < BW + 1; x++)	// 너비만큼 반복하면서 검사
			if (board[x][y] != BRICK) break;	// 비어 있으면 break

		if (x == BW + 1) {	// 이전의 for문에서 x가 break를 만나지 않고 
			// BW + 1이 되어서 나왔다면 한 줄이 채워진 것이라고 볼 수 있다.

			for (ty = y; ty > 1; ty--) {
				for (x = 1; x < BW + 1; x++)

					board[x][ty] = board[x][ty - 1];
				// 채워진 줄의 값들을 윗 줄의 값으로 덮어쓴다.
				// 나머지 윗 줄들도 차례로 덮어쓰기 하면서 한 줄을 지우는 효과를 볼 수 있다.
			}
			isLine = TRUE;
			score += 50;
		}
	}
	if (isLine) {
		playSoundLine();
		drawBoard(); // 블럭을 다시 그린다.
		isLine = FALSE;
	}
}

void ssScreen() {
	char s_score[10];
	_itoa(score, s_score, 10);
	char s_stage[10];
	_itoa(stage, s_stage, 10);

	gotoxy(BX - 25, BY+2);
	printf("스테이지 : ");
	gotoxy((BX - 25) + 11, BY + 2);
	printf("%s",s_stage);

	gotoxy(BX - 25, BY + 4);
	printf("점수 : ");
	gotoxy((BX - 25) + 7, BY + 4);
	printf("%s", s_score);
}

void changeStage() {
	stage = (int)(score / 1000) + selectStage;
	if (stage > 10) {
		stage = 10;
	}

	frame -= (stage - 1) * 2;
	if (frame < 4) {
		frame = 4;
	}
}

void selectLevelKey() {
	int ch;
	if (_kbhit()) {
		ch = _getch();
		if (ch == ENTER) {
			isSelectLevel = FALSE; // 선택 됨
			playSoundHold();
			clrscr();
		}
		if (ch == 0xE0 || ch == 0) {
			ch = _getch();
			switch (ch) {
			case UP:
				if (selectStage != 10) {
					selectStage = selectStage + 1;
				}
				else {
					selectStage = 1;
				}
				playSoundHold();
				clrscr();
				break;
			case DOWN:
				if (selectStage != 1) {
					selectStage = selectStage - 1;
				}
				else {
					selectStage = 10;
				}
				playSoundHold();
				clrscr();
				break;
			}
		}
	}
}

void titleKey() {
	int ch;
	if (_kbhit()) {
		ch = _getch();
		if (ch == ENTER) {
			if (cursor == 0) {
				cursor = -1; // 시작하러 ㄱㄱ
				playSoundHold();
				clrscr();
			}
			else {
				isSelectLevel = TRUE; // 레벨 선택 ㄱㄱ
				cursor = 0;
				playSoundHold();
				clrscr();
			}
		}
		if (ch == 0xE0 || ch == 0) {
			ch = _getch();
			switch (ch) {
			case UP:
				cursor = (cursor == 0 ? 1 : 0);
				playSoundHold();
				clrscr();
				break;
			case DOWN:
				cursor = (cursor == 1 ? 0 : 1);
				playSoundHold();
				clrscr();
				break;
			}
		}
	}
}