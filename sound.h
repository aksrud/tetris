#ifndef SOUND_HEADER

#define SOUND_HEADER

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <Digitalv.h>
#pragma comment(lib,"winmm.lib")

void mainSound(int a);

void playSoundLine();

void playSoundHold();

void playSoundRotate();

void playSoundMove();

void playSoundSoftDrop();

void playSoundHardDrop();

void playSoundGameOver();

void playSoundSuccess();

MCI_OPEN_PARMS open[10];

int dwID[20]; //음악 파일 불러오는 값

void mainSound(int a) {
	open[0].lpstrElementName = TEXT("sound/background.wav");
	open[0].lpstrDeviceType = TEXT("waveaudio");
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&open[0]);
	dwID[0] = open[0].wDeviceID;
	if (a == 1)
		mciSendCommand(dwID[0], MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&open[0]);
	else
		mciSendCommand(dwID[0], MCI_STOP, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&open[0]);
}

void playSoundLine() {
	open[1].lpstrElementName = TEXT("sound/line.wav");
	open[1].lpstrDeviceType = TEXT("waveaudio");
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&open[1]);
	dwID[1] = open[1].wDeviceID;
	mciSendCommand(dwID[1], MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
	mciSendCommand(dwID[1], MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&open[1]);
}

void playSoundHold() {
	open[3].lpstrElementName = TEXT("sound/hold.wav");
	open[3].lpstrDeviceType = TEXT("waveaudio");
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&open[3]);
	dwID[3] = open[3].wDeviceID;
	mciSendCommand(dwID[3], MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
	mciSendCommand(dwID[3], MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&open[3]);
}

void playSoundRotate() {
	open[4].lpstrElementName = TEXT("sound/rotate.wav");
	open[4].lpstrDeviceType = TEXT("waveaudio");
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&open[4]);
	dwID[4] = open[4].wDeviceID;
	mciSendCommand(dwID[4], MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
	mciSendCommand(dwID[4], MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&open[4]);
}

void playSoundMove() {
	open[5].lpstrElementName = TEXT("sound/move.wav");
	open[5].lpstrDeviceType = TEXT("waveaudio");
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&open[5]);
	dwID[5] = open[5].wDeviceID;
	mciSendCommand(dwID[5], MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
	mciSendCommand(dwID[5], MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&open[5]);
}

void playSoundSoftDrop() {
	open[6].lpstrElementName = TEXT("sound/softdrop.wav");
	open[6].lpstrDeviceType = TEXT("waveaudio");
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&open[6]);
	dwID[6] = open[6].wDeviceID;
	mciSendCommand(dwID[6], MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
	mciSendCommand(dwID[6], MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&open[6]);
}

void playSoundHardDrop() {
	open[7].lpstrElementName = TEXT("sound/harddrop.wav");
	open[7].lpstrDeviceType = TEXT("waveaudio");
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&open[7]);
	dwID[7] = open[7].wDeviceID;
	mciSendCommand(dwID[7], MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
	mciSendCommand(dwID[7], MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&open[7]);
}

void playSoundGameOver() {
	open[8].lpstrElementName = TEXT("sound/gameover.wav");
	open[8].lpstrDeviceType = TEXT("waveaudio");
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&open[8]);
	dwID[8] = open[8].wDeviceID;
	mciSendCommand(dwID[8], MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
	mciSendCommand(dwID[8], MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&open[8]);
}

void playSoundSuccess() {
	open[9].lpstrElementName = TEXT("sound/success.wav");
	open[9].lpstrDeviceType = TEXT("waveaudio");
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&open[9]);
	dwID[9] = open[9].wDeviceID;
	mciSendCommand(dwID[9], MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
	mciSendCommand(dwID[9], MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&open[9]);
}

#endif