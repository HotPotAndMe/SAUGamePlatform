/**
 * file: Function.cpp
 * date: 2014/9/10
 * version: 1.0
 * description: 
 */


#include "Function.h"
#include "CMainWnd.h"
#include "Control.h"
#include "Manual.h"
#include "Network.h"


/** �˳����� */
int Function::Exit()
{
	return PostMessage(MainWnd->hWnd,WM_CLOSE,NULL,NULL);
}

/** ���غڷ����� */
int Function::LoadBlackEngine()
{
	return game.LoadEngine(BLACK);
}

/** ж�غڷ����� */
int Function::UnloadBlackEngine()
{
	return game.UnloadEngine(BLACK);
}

/** ���ذ׷����� */
int Function::LoadWhiteEngine()
{
	return game.LoadEngine(WHITE);
}

/** ж�ذ׷����� */
int Function::UnloadWhiteEngine()
{
	return game.UnloadEngine(WHITE);
}

/** ��ʼ���� */
int Function::GameStart()
{
	game.StartGame();
	return 0;
}

/** �������� */
int Function::GameStop()
{
	game.StopGame();
	return 0;
}

/** ���� */
int Function::MoveStep(int x, int y)
{
	return game.MoveStep(x,y);
}

/** ȷ���ŷ� */
int Function::OkMove()
{
	game.OkMove();
	return 0;
}

/** ȡ���ŷ� */
int Function::CancelMove()
{
	game.CancelMove();
	return 0;
}

/** ����Ƿ���������� */
int Function::CheckEngineLoad()
{
	if (game.CheckEngineLoad())
		return 1;
	return 0;
}

/** ��ȡ����ͼƬ */
int Function::ShotBoard()
{
	::PrintScrOnTemp();
	return 0;
}

/** ���������ͼ */
int Function::SaveBoardShot()
{
	::SavePrintScreen();
	return 0;
}

/** �������� */
int Function::SaveChessManual(char *step)
{
	::SaveChessManual(step);
	return 0;
}

/** ���������ս */
int Function::NetworkStart()
{
	//������������壬ͬʱ���������ս����
	if (NetWork==0)
		NetWnd->CreateWnd();
	return 0;
}

/** �ر������ս */
int Function::NetworkStop()
{
	//���������ս�����壬ͬʱ�ر������ս����
	if (NetWork!=0)
		DestroyWindow(NetWnd->hWnd);
	return 0;
}