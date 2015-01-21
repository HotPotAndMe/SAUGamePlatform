/**
 * file: Control.h
 * date: 2014/9/13
 * version: 1.0
 * description: 
 */


#include "Control.h"
#include "GameType.h"
#include "CMainWnd.h"
#include "Network.h"


Game game;


/**
 * EngineRun - ���������ȡ�߳�������
 * @lpParam:	ָ�������е��������ɫ�����ڷ�/�׷�
 * description��	������һ���߳��������������ڽ�һ��ѭ������������ȡ�������淢�͵������¼�
 *					���������е�����-���ֽ�ɫģ���еĶԵ�˫���Ľ�����������Ϊ���ĵĳе��ߣ���������ά�������Ķ����߼���
 */
DWORD WINAPI EngineRun(LPVOID lpParam)
{
	CEngine *side,*unside;
	char rMsg[BUFSIZE],wMMsg[BUFSIZE],wDMsg[BUFSIZE];
	int temp;

	if((int)lpParam==BLACK)//�ж��̳߳е��������ɫ
	{
		side=game.GetBlackE();
		unside=game.GetWhiteE();
	}
	else
	{
		side=game.GetWhiteE();
		unside=game.GetBlackE();
	}

	side->WriteMsg("name?\n");//ѯ����������
	side->GetCommand("name",rMsg);//��ȡ��������
	side->SetName(rMsg);
	MainWnd->SetName(rMsg,(int)lpParam);
	while(1)
	{
		side->GetCommand("move",rMsg);//��ȡ�����¼�
		if(CT_GetCurPlayer()!=(int)lpParam)//��ǰ���巽�뱾������ִ����ɫ��ͬ
			continue;
		temp = game.GameMode;
		MainWnd->UpdateTime();
		if (CT_ProcessMove(rMsg, wMMsg, wDMsg) == -1)//���������¼���������Ӧ����
		{
			char errorMsg[500];
			sprintf(errorMsg, "Break rule!\nError command: %s", rMsg);
			MessageBox(MainWnd->hWnd, errorMsg, "error", MB_OK);
		}
		if(wMMsg[0]!='\0')
			side->WriteMsg(wMMsg);
		if (wDMsg[0] != '\0')//�������ģ������Է�����
		{
			switch (temp)
			{
			case 2:
				unside->WriteMsg(wDMsg);
				break;
			case 4:
			case 5:
				NetShell(NULL, ConnectMode ? SERVERINFO.s : CLIENTINFO.s, wDMsg, strlen(wDMsg) + 1, 2);
				break;
			}
		}
	}

	return 0;
}

Game::Game()
{
	GameMode=-1;
}

Game::~Game()
{
}

bool Game::LoadEngine(int side)
{
	if(side==BLACK)
	{
		if(!BlackE.LoadEngine())
			return false;
		if(BlackT.StartThread(EngineRun,(LPVOID)BLACK)==NULL)
			return false;
	}
	else if(side==WHITE)
	{
		if(!WhiteE.LoadEngine())
			return false;
		if(WhiteT.StartThread(EngineRun,(LPVOID)WHITE)==NULL)
			return false;
	}
	return true;
}

bool Game::UnloadEngine(int side)
{
	if(side==BLACK)
	{
		if(BlackE.GetLoaded()==false)//���Ƿ���ж��
		{
			MsgBox("Engine has unloaded!","Msg",1500);
			return true;
		}
		if(BlackT.StopThread()==FALSE)
			return false;
		if(!BlackE.UnloadEngine())
			return false;
	}
	else if(side==WHITE)
	{
		if(WhiteE.GetLoaded()==false)//���Ƿ���ж��
		{
			MsgBox("Engine has unloaded!","Msg",1500);
			return true;
		}
		if(WhiteT.StopThread()==FALSE)
			return false;
		if(!WhiteE.UnloadEngine())
			return false;
	}
	SendMessage(MainWnd->hWnd, GM_NAME, (WPARAM)NULL, (LPARAM)side);
	return true;
}

bool Game::CheckEngineLoad()
{
	if (BlackE.GetLoaded() || WhiteE.GetLoaded())
		return true;
	return false;
}

void Game::StartGame()
{
	if(IsStop()==FALSE)
	{
		MsgBox("�������ڽ���","error",0);
		return;
	}
	CT_OnRun();//��ʼ�����
	EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), FALSE);
	EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_BLC), FALSE);
	EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), FALSE);
	EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_WHT), FALSE);
	MainWnd->GameStart();
	if (NetWork == 0)
	{
		if (BlackE.GetLoaded() && WhiteE.GetLoaded())//�ڰ����涼������
		{
			GameMode = 2;
			BlackE.WriteMsg("new black\n");
			WhiteE.WriteMsg("new white\n");
		}
		else if (BlackE.GetLoaded() || WhiteE.GetLoaded())//ֻ����һ������
		{
			if (chessType[chesstype].type != 0)
			{
				MsgBox("�����ֲ�֧���˻����ģ�", "error", 0);
				return;
			}
			if (BlackE.GetLoaded())
			{
				GameMode = 1;
				BlackE.WriteMsg("new black\n");
			}
			else
			{
				GameMode = 0;
				WhiteE.WriteMsg("new white\n");
			}
		}
		else//���˶���
		{
			if (chessType[chesstype].type != 0)
			{
				MsgBox("�����ֲ�֧�����˶��ģ�", "error", 0);
				return;
			}
			GameMode = 3;
		}
	}
	else
	{
		if (GameMode_2 == 0)//ִ��
		{
			if (BlackE.GetLoaded())
			{
				GameMode = 4;
				BlackE.WriteMsg("new black\n");
			}
			else
				GameMode = 0;
		}
		else
		{
			if (WhiteE.GetLoaded())
			{
				GameMode = 5;
				WhiteE.WriteMsg("new white\n");
			}
			else
				GameMode = 1;
		}
	}
}

void Game::StopGame()
{
	if(IsStop()==TRUE)
	{
		MsgBox("������δ��ʼ","error",3000);
		return;
	}
	MainWnd->GameStop();
	EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), FALSE);
	EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_BLC), FALSE);
	EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), FALSE);
	EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_WHT), FALSE);
	switch(GameMode)
	{
	case 0://��ִ�ڣ�����/����ִ��
	case 1://��ִ�ף�����/����ִ��
	case 2://��������
	case 3://���˶���
		break;
	case 4://����ִ�ڣ�����ִ��
	case 5://����ִ�ף�����ִ��
		break;
	}
	GameMode=-1;
}

bool Game::MoveStep(int x,int y)
{
	int temp,player=CT_GetCurPlayer();

	if(IsStop()==TRUE)
	{
		MsgBox("The game is end or not start!","error",1500);
		return false;
	}
	if(GameMode==2)//��������
	{
		MsgBox("The game's mode is'engine vs engine',stop clicking in board!","Msg",1500);
	}
	else if(GameMode==3)//���˶���
	{
		temp=CT_OnLButtonDown(x,y);
		switch(temp)
		{
		case -1://�����ŷ�
			MessageBox(MainWnd->hWnd, "error step!","error", MB_OK);
			return false;
		case 1://�ŷ�����
			EnableWindow(GetDlgItem(MainWnd->hWnd, player ? IDB_CONTROL_OK_WHT : IDB_CONTROL_OK_BLC), TRUE);
		case 0://�ŷ�����
			EnableWindow(GetDlgItem(MainWnd->hWnd, player ? IDB_CONTROL_CANCEL_WHT : IDB_CONTROL_CANCEL_BLC), TRUE);
			break;
		case 2://���ǿ���λ��
			break;
		}
	}
	else if(GameMode==4||GameMode==5)
	{
		MsgBox("The game's mode is'engine vs net',stop clicking in board!","Msg",1500);
	}
	else if(GameMode!=-1)//�˻�����
	{
		if(GameMode==player)//��ǰ���巽Ϊ��
		{
			temp=CT_OnLButtonDown(x,y);
			switch(temp)
			{
			case -1://�����ŷ�
				MessageBox(MainWnd->hWnd, "error step!", "error", MB_OK);
				return false;
			case 1://�ŷ�����
				EnableWindow(GetDlgItem(MainWnd->hWnd, player ? IDB_CONTROL_OK_WHT : IDB_CONTROL_OK_BLC), TRUE);
			case 0://�ŷ�����
				EnableWindow(GetDlgItem(MainWnd->hWnd, player ? IDB_CONTROL_CANCEL_WHT : IDB_CONTROL_CANCEL_BLC), TRUE);
				break;
			case 2://���ǿ���λ��
				break;
			}
		}
		else
		{
			if(NetWork==0)
			{
				MsgBox("The computer hasn't move,stop clicking in board!","error",1500);
			}
			else
			{
				MsgBox("Opponent hasn't move,stop clicking in board!","error",1500);
			}
		}
	}
	return true;
}

bool Game::MoveStep(char *step)
{
	char wMMsg[256],wDMsg[256];
	int temp = GameMode;
	CT_ProcessMove(step, wMMsg, wDMsg);
	if (temp == 4)
	{
		BlackE.WriteMsg(wDMsg);
	}
	else if (temp == 5)
	{
		WhiteE.WriteMsg(wDMsg);
	}
	return true;
}

void Game::OkMove()
{
	char denCmd[256];
	BYTE player = CT_GetCurPlayer();
	int temp = GameMode;
	CT_OkMove(denCmd);
	if (denCmd[0] != '\0')//��������һ��������Ϣ������
	{
		if (NetWork == 0)//�����繤��ģʽ
		{
			switch (temp)
			{
			case 0://ִ�ڣ����͸��׷�
				WhiteE.WriteMsg(denCmd);
				break;
			case 1://ִ�ף����͸��ڷ�
				BlackE.WriteMsg(denCmd);
				break;
			}
		}
		else//���繤��ģʽ
		{
			NetShell(NULL, ConnectMode ? SERVERINFO.s : CLIENTINFO.s, denCmd, strlen(denCmd) + 1, 2);
		}
	}

	if (player == 0)//���úڷ���ť
	{
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), FALSE);
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_BLC), FALSE);
	}
	else//���ð׷���ť
	{
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), FALSE);
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_WHT), FALSE);
	}
}

void Game::CancelMove()
{
	BYTE player = CT_GetCurPlayer();
	CT_CancelMove();
	if (player == 0)
	{
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), FALSE);
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_BLC), FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), FALSE);
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_WHT), FALSE);
	}
}