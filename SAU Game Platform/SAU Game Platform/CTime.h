#ifndef CTIME_H_GAMEPLATFORM
#define CTIME_H_GAMEPLATFORM

#include "Game Platform.h"

typedef struct _LastTime{
	int hours;
	int minutes;
	int seconds;
	DWORD sumTime;
}LastTime;

class CTime
{
public:
	CTime(){killed=true;};
	~CTime(){};

	INT UpdataTime(char *strBlcTime,char *strWhtTime);
	VOID StartTimer(HWND hTimerWnd);
	bool StopTimer();
	VOID timeDeal(int player);	//ʱ��ת��
	LastTime GetTime(int side);
private:
	HWND hSetWnd;
	bool killed;//��ʱ���Ƿ�������
	DWORD startTime;
	LastTime blcTime,whtTime;
};

#endif