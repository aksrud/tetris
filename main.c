#define _CRT_SECURE_NO_WARNINGS
#include "block.h"

#define LEFT 75	// ���� ����Ű
#define RIGHT 77// ������ ����Ű
#define UP 72	// ���� ����Ű
#define DOWN 80	// �Ʒ� ����Ű	
#define ESC 27	// ESC 
#define C 67 // C
#define Z 90 // Z

#define BX 40	// �ܺ� �� X�� �»�� ��ǥ
#define BY 3	// �ܺ� �� Y�� �»�� ��ǥ

#define BW 10	// �������� ��
#define BH 20	// �������� ����

int board[BW + 2][BH + 2]; // ������

int cx, cy; // ���� ��ġ

int rot; // ȸ��

int brick[2] = {-1, -1}; // �� ����

int hold_block = -1; // Ȧ�� ��

int score = 3000; // ����

int stage = 1; // ��������

int frame = 20; // ������

int nframe; // ���� ����������

float wating; // �ٴڿ� ������ �ð�

BOOL isHold = FALSE; // Ȧ�� �ϰ� �ֳ�
BOOL isOut = FALSE; // Ȧ���Ѱ� ���³�
BOOL isLine = FALSE; // ���� Ŭ����
BOOL isDown = FALSE; // �ٴڿ� ��Ҵ°�
BOOL isForce = FALSE; // �����̽��� ������ �ٷ� �������� ��

void drawScreen(); // ��ü ��ũ��
void drawBoard(); // �� ��ũ��
void printBrick(BOOL show); // �� �κ� ���
void start(); // ���� ����
int getAround(int x, int y, int r); // �ֺ� �� or �� �ִ��� �˻�
BOOL moveDown(); // �Ʒ��� ��������
BOOL processKey(); // Ű �Է�
void testFull(); // �� ���� ä�������� Ȯ��
void next(); // ���� �� �����ִ� ȭ��
void hold(); // Ȧ���ϰ� �ִ� �� �����ִ� ȭ��
void changeStage(); // ������ �������� ����
void ssScreen(); // ������ ���������� �����ִ� ȭ��

int main() {
	mainSound(1);
	start();
	return 0;
}

void start() {
	setcursortype(NOCURSOR); // Ŀ�� ���ֱ�
	randomize(); // ���� �õ� ����
	clrscr(); // ȭ�� Ŭ����

	frame = 20;

	for (int x = 0; x < BW + 2; x++) {
		for (int y = 0; y < BH + 2; y++)
			board[x][y] = (y == 0 || y == BH + 1 || x == 0 || x == BW + 1) ? WALL : EMPTY;
	}

	drawScreen();

	while (1) {
		if (!isOut) {
			if (brick[0] == -1)
				brick[0] = random(sizeof(blocks) / sizeof(blocks[0]));
			else
				brick[0] = brick[1];
			brick[1] = random(sizeof(blocks) / sizeof(blocks[0]));
		}
		isOut = FALSE;
		changeStage();

		next();
		hold();
		ssScreen();

		cx = BW / 2 - 1; // �̵����� ������ x��ǥ
		cy = BY;	// �̵����� ������ y��ǥ
		rot = 0; // ȸ���� ���� ���� ����

		printBrick(TRUE);	// ������ ���
		if (getAround(cx, cy, rot) != EMPTY) break;
		// ��� �̰� ����
		nframe = frame;
		while (1) {
			if (--nframe == 0) {	// ī��Ʈ�ϴٰ� 0�� �Ǹ� MoveDown()�� ȣ���Ѵ�.

				nframe = frame;	// ���� ����� ���� �ٽ� ����

				if (moveDown()) break;	//	�̵��� ������ break;
			}
			if (processKey()) break;
			delay(1000 / 20);
		}
	}
	mainSound(0);
	clrscr();	// ȭ�� û��
	gotoxy(30, 9); puts("G A M E  O V E R");
	Sleep(2000);	// 2���� ���α׷� ����
	setcursortype(NORMALCURSOR);	// ���� ���� �� Ŀ���� ǥ��
}

void next() { // ���� ��� �����ִ� ȭ��
	int next[8][7];
	gotoxy(BX + BW * 2 + 6, BY);
	printf("���� : ");
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

void hold() { // ���� �����ϰ� �ִ� ��� �����ִ� ȭ��
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

void drawScreen() {	// ȭ�� ��ü�� �׸���.
	for (int x = 0; x < BW + 2; x++) {
		for (int y = 0; y < BH + 2; y++) {
			gotoxy(BX + x * 2, BY + y);	// �ܺ� ���� �»�� ��ǥ�������� Ŀ���� x*2, y��ŭ �̵���Ŵ
			printf(arTile[board[x][y]]);	// board[x][y]�� ����� ������ ������ �������� �����Ѵ�.
		}
	}
}

void drawBoard() { // ��� ȭ���� �׸���.
	for (int x = 1; x < BW + 1; x++) {
		for (int y = 1; y < BH + 1; y++) {

			gotoxy(BX + x * 2, BY + y);	// �ܺ� ���� �»�� ��ǥ�������� Ŀ���� x*2, y��ŭ �̵���Ŵ

			puts(arTile[board[x][y]]);	// board[x][y]�� ����� ������ ������ �׿��ִ� ������ �׸���.
		}
	}
}

void printBrick(BOOL show) { // ��� �׸��� �κ�
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (blocks[brick[0]][rot][i][j]) {
				gotoxy(BX + (j + cx) * 2, BY + i + cy);
				printf(arTile[show ? BRICK : EMPTY]);
			}
		}
	}
}

int getAround(int x, int y, int r){	// ���� �ֺ��� ���� �˻�
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (blocks[brick[0]][r][i][j]) {
				if (board[x + j][y + i]) {
					return WALL;
				}
			}
		}
	}
	return EMPTY;
}

BOOL moveDown() { // �Ʒ��� �� ������
	if (getAround(cx, cy + 1, rot) != EMPTY) {	// ���� ny��ǥ�� ä���� ������, �̹� ��ġ���� �̵��� ������ �Ѵ�.
		if (!isForce) {
			isDown = TRUE;
			wating = 1;
			time_t start = time(NULL);
			time_t check = time(NULL);

			while (check - start < wating) {
				check = time(NULL);
				processKey();
			}
			isDown = FALSE;
			if (wating == -1) { // �ٴڿ��� Ȧ�� ��ư�� ������ ���� ó��
				return TRUE;
			}
		}
		testFull();		// ������ ���� �ִ��� �˻��Ѵ�.
		isHold = FALSE;
		return TRUE;	// �̵��� ������ true�� ��ȯ
	}

	printBrick(FALSE);	// ���� ������ ����
	cy++;	// �̵����� ���� ������ y��ǥ�� ������Ų��.
	printBrick(TRUE);	// �� ĭ �Ʒ��� ������ ���

	return FALSE;
}

BOOL processKey() { // ���� �� Ű �Է�
	int ch, trot;	// Ű �Է�, ȸ�� �� ����

	if (_kbhit()) {	// Ű���� �Է��� ���� ������ ���
		ch = _getch();
		if (ch == C + 32 || ch == C) {
			if (!isHold) { // �ȵ�� ������
				if(hold_block == -1){
					printBrick(FALSE);
					isHold = TRUE;
					hold_block = brick[0];
					playSoundHold();
					wating = -1; // �ٴ� ����
					return TRUE;
				} else{
					printBrick(FALSE);
					isHold = TRUE;
					isOut = TRUE;
					int dump = brick[0];
					brick[0] = hold_block;
					hold_block = dump;
					playSoundHold();
					wating = -1; // �ٴ� ����
					return TRUE;
				}
			}
		}
		if (ch == Z+32 || ch == Z) {
			trot = (rot == 0 ? 3 : rot - 1);	// �ٸ� ��� ������� ���� �� �ְ� ȸ������ ����

			// �������� ȸ�� ����� ���� ȸ�� �Ǵ� ����� ���� ȸ��
			for (int i = 0; i <= 2; i++) {
				for (int j = 0; j <= 1; j++) {
					if (j) {
						if (getAround(cx + i, cy, trot) == EMPTY) {	// ���� ȸ���� ������ ��ġ���� �˻�
							printBrick(FALSE);	// ���� ���� (ȸ���ϱ� ��)
							cx += i;
							rot = trot;		// ȸ������ ����
							printBrick(TRUE);	// ���� ��� (ȸ���� ����� ���)
							playSoundRotate();
							wating += 0.05;
							return FALSE;
						}
					}
					else {
						if (getAround(cx - i, cy, trot) == EMPTY) {	// ���� ȸ���� ������ ��ġ���� �˻�
							printBrick(FALSE);	// ���� ���� (ȸ���ϱ� ��)
							cx -= i;
							rot = trot;		// ȸ������ ����
							printBrick(TRUE);	// ���� ��� (ȸ���� ����� ���)
							playSoundRotate();
							wating += 0.05;
							return FALSE;
						}
					}
				}
			}
		}
		if (ch == 0xE0 || ch == 0) {	// Ȯ�� Ű(Function) �� ��쿡 Ű�� �ϳ� �� �Է� �޴´�.
			// ����Ű�� Ȯ��Ű�� ���Եȴ�.( 2����Ʈ�� �Ǿ� �����Ƿ� _getch()�� �ѹ��� �о���� �� ����.)
			// ���� _getch()�� 2�� ȣ���ؾ� �Ѵ�.
			ch = _getch();

			switch (ch) {	// �Է� ���� �˻�

			case LEFT:

				if (getAround(cx - 1, cy, rot) == EMPTY) {

					printBrick(FALSE);	// ���� ��ġ�� ����� �����Ѵ�.
					cx--;	// �������� �� ĭ ��ǥ �̵�
					printBrick(TRUE);	// ���� ����Ű�� ���� �Ŀ�, ����� ���� �׸���.
					playSoundMove();
					wating += 0.05;
				}
				break;
			case RIGHT:
				if (getAround(cx + 1, cy, rot) == EMPTY) {

					printBrick(FALSE);	// ���� ��ġ�� ����� �����Ѵ�.
					cx++;	// ���������� �� ĭ ��ǥ �̵�
					printBrick(TRUE);	// ������ ����Ű�� ���� �Ŀ�, ����� ���� �׸���.
					playSoundMove();
					wating += 0.05;
				}

				break;
			case UP:
				trot = (rot == 3 ? 0 : rot + 1);	// �ٸ� ��� ������� ���� �� �ְ� ȸ������ ����

				// �������� ȸ�� ����� ���� ȸ�� �Ǵ� ����� ���� ȸ��
				for (int i = 0; i <= 2; i++) {
					for (int j = 0; j <= 1; j++) {
						if (j) {
							if (getAround(cx + i, cy, trot) == EMPTY) {	// ���� ȸ���� ������ ��ġ���� �˻�
								printBrick(FALSE);	// ���� ���� (ȸ���ϱ� ��)
								cx += i;
								rot = trot;		// ȸ������ ����
								printBrick(TRUE);	// ���� ��� (ȸ���� ����� ���)
								playSoundRotate();
								wating += 0.05;
								return FALSE;
							}
						}
						else {
							if (getAround(cx - i, cy, trot) == EMPTY) {	// ���� ȸ���� ������ ��ġ���� �˻�
								printBrick(FALSE);	// ���� ���� (ȸ���ϱ� ��)
								cx -= i;
								rot = trot;		// ȸ������ ����
								printBrick(TRUE);	// ���� ��� (ȸ���� ����� ���)
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
					if (moveDown()) {// �Ʒ��� �� ĭ ����.
						playSoundSoftDrop(); //�߰�
						return TRUE;
					}
				}
				break;
			}
		}
		else {

			switch (ch) {
			case ' ':		// �����̽� Ű�� ������ 
				isForce = TRUE;
				while (moveDown() == FALSE) {}	// ����� �ٴ����� ��� ������.
				playSoundHardDrop();
				isForce = FALSE;
				return TRUE;
				break;
			case '@':		// @�� ������
				_kbhit();	// �Ͻ� ����
				break;
			case '!':		// !�� ������
				start();	// �� ���� ����
				break;
			}
		}
	}
	return FALSE;
}

void testFull() { // ä���� �� �ִ��� �˻�
	int i, x, y, ty;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (blocks[brick[0]][rot][i][j]) {
				board[cx + j][cy + i] = BRICK;
			}
		}
	}

	for (y = 1; y < BH + 1; y++) {	// ���̸�ŭ �ݺ��ϸ鼭 �˻�
		for (x = 1; x < BW + 1; x++)	// �ʺ�ŭ �ݺ��ϸ鼭 �˻�
			if (board[x][y] != BRICK) break;	// ��� ������ break

		if (x == BW + 1) {	// ������ for������ x�� break�� ������ �ʰ� 
			// BW + 1�� �Ǿ ���Դٸ� �� ���� ä���� ���̶�� �� �� �ִ�.

			for (ty = y; ty > 1; ty--) {
				for (x = 1; x < BW + 1; x++)

					board[x][ty] = board[x][ty - 1];
				// ä���� ���� ������ �� ���� ������ �����.
				// ������ �� �ٵ鵵 ���ʷ� ����� �ϸ鼭 �� ���� ����� ȿ���� �� �� �ִ�.
			}
			isLine = TRUE;
			score += 50;
		}
	}
	if (isLine) {
		playSoundLine();
		drawBoard(); // ���� �ٽ� �׸���.
		isLine = FALSE;
	}
	else {
		playSoundHardDrop();
	}
}

void ssScreen() {
	char s_score[10];
	_itoa(score, s_score, 10);
	char s_stage[10];
	_itoa(stage, s_stage, 10);

	gotoxy(BX - 25, BY+2);
	printf("�������� : ");
	gotoxy((BX - 25) + 11, BY + 2);
	printf("%s",s_stage);

	gotoxy(BX - 25, BY + 4);
	printf("���� : ");
	gotoxy((BX - 25) + 7, BY + 4);
	printf("%s", s_score);
}

void changeStage() {
	stage = (int)(score / 1000) + 1;
	if (stage > 10) {
		stage = 10;
	}

	frame -= (stage - 1) * 2;
	if (frame < 4) {
		frame = 4;
	}
}