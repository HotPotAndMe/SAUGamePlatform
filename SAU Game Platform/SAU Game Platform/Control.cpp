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
	int color;
	CEngine *side,*unside;
	char rMsg[BUFSIZE],wMMsg[BUFSIZE],wDMsg[BUFSIZE];
	int temp;//��¼�����ŷ�ǰ��GameMode����ֹ���ʤ��-1ʹ�ŷ����ܷ����Է�����

	if((int)lpParam==BLACK)//�ж��̳߳е��������ɫ
	{
		color = BLACK;
		side=game.GetBlackE();
		unside=game.GetWhiteE();
	}
	else
	{
		color = WHITE;
		side=game.GetWhiteE();
		unside=game.GetBlackE();
	}

	side->WriteMsg("name?\n");//ѯ����������
	side->GetCommand("name",rMsg);//��ȡ��������
	side->SetName(rMsg);
	MainWnd->SetName(rMsg,(int)lpParam);
	side->SetStatus(1);//�������
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
			if (!(chessType[chesstype].type & 8))
			{
				sprintf(errorMsg, "%s break rule!\nError command: %s", (color == BLACK) ? "Black" : "White", rMsg);
				MessageBox(MainWnd->hWnd, errorMsg, "error", MB_OK);
			}
		}
		if ((chessType[chesstype].type & 4) && (CT_GetCurPlayer() == game.GameMode))//����pass��������
		{
			if (game.GameMode == 0)
				game.OpenPB();
			else
				game.OpenPW();
		}
		if (wMMsg[0] != '\0')
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
	passB = passW = false;
	okB = okW = false;
	cancelB = cancelW = false;
}

Game::~Game()
{
}

void Game::OpenCB()
{
	if (!cancelB)
	{
		cancelB = true;
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_BLC), TRUE);
	}
	if (passB)
		ClosePB();
}

void Game::OpenCW()
{
	if (!cancelW)
	{
		cancelW = true;
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_WHT), TRUE);
	}
	if (passW)
		ClosePW();
}

void Game::OpenOCB()
{
	if (!okB)
	{
		okB = true;
		if (passB)
		{
			passB = false;
			SetText(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), "Ok");
		}
		else
		{
			EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), TRUE);
		}
	}
	if (!cancelB)
		OpenCB();
}

void Game::OpenOCW()
{
	if (!okW)
	{
		okW = true;
		if (passW)
		{
			passW = false;
			SetText(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), "Ok");
		}
		else
		{
			EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), TRUE);
		}
	}
	if (!cancelW)
		OpenCW();
}

void Game::OpenPB()
{
	if (!passB)
	{
		passB = true;
		if (okB)
		{
			okB = false;
			SetText(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), "Pass");
		}
		else
		{
			SetText(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), "Pass");
			EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), TRUE);
		}
	}
}

void Game::OpenPW()
{
	if (!passW)
	{
		passW = true;
		if (okW)
		{
			okW = false;
			SetText(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), "Pass");
		}
		else
		{
			SetText(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), "Pass");
			EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), TRUE);
		}
	}
}

void Game::CloseOCB()
{
	if (okB)
	{
		okB = false;
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), FALSE);
	}
	if (cancelB)
	{
		cancelB = false;
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_BLC), FALSE);
	}
	if (passB)
	{
		passB = false;
		SetText(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), "Ok");
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), FALSE);
	}
}

void Game::CloseOCW()
{
	if (okW)
	{
		okW = false;
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), FALSE);
	}
	if (cancelW)
	{
		cancelW = false;
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_WHT), FALSE);
	}
	if (passW)
	{
		passW = false;
		SetText(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), "Ok");
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), FALSE);
	}
}

void Game::ClosePB()
{
	if (passB)
	{
		passB = false;
		SetText(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), "Ok");
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), FALSE);
	}
}

void Game::ClosePW()
{
	if (passW)
	{
		passW = false;
		SetText(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), "Ok");
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), FALSE);
	}
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
		if(BlackE.GetStatus()==-1)//���Ƿ���ж��
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
		if(WhiteE.GetStatus()==-1)//���Ƿ���ж��
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
	if (BlackE.GetStatus()!=-1 || WhiteE.GetStatus()!=-1)//�����洦������״̬
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
	CloseOCB();
	CloseOCW();
	MainWnd->GameStart();
	if (NetWork == 0)//���ض���
	{
		if (BlackE.GetStatus() != -1 && WhiteE.GetStatus() != -1)//�ڰ����涼������
		{
			if (BlackE.GetStatus() == 1 && WhiteE.GetStatus() == 1)
			{
				GameMode = 2;
				BlackE.WriteMsg("new black\n");
				WhiteE.WriteMsg("new white\n");
			}
			else
				MsgBox("����δ����", "error", 3000);
		}
		else if (BlackE.GetStatus() != -1 || WhiteE.GetStatus() != -1)//ֻ����һ������
		{
			if (chessType[chesstype].type & 2)
			{
				MsgBox("�����ֲ�֧���˻����ģ�", "error", 0);
				return;
			}
			if (BlackE.GetStatus() != -1)//��ִ��
			{
				if (BlackE.GetStatus() == 1)
				{
					GameMode = 1;
					BlackE.WriteMsg("new black\n");
				}
				else
					MsgBox("����δ����", "error", 3000);
			}
			else//��ִ��
			{
				if (WhiteE.GetStatus() == 1)
				{
					GameMode = 0;
					WhiteE.WriteMsg("new white\n");
					if (chessType[chesstype].type & 4)//����pass
					{
						OpenPB();
					}
				}
				else
					MsgBox("����δ����", "error", 3000);
			}
		}
		else//���˶���
		{
			if (chessType[chesstype].type & 1)
			{
				MsgBox("�����ֲ�֧�����˶��ģ�", "error", 0);
				return;
			}
			GameMode = 3;
			if (chessType[chesstype].type & 4)//����pass
			{
				OpenPB();
			}
		}
	}
	else//���������
	{//����������У���Ϊ�û�Э�̻��ƣ�ֻ�ṩ����״̬����Ķ��ķ��񣬼��������Ѽ��ص�δ����ʱ��ʼ�Ķ��İ���Ȼ�˶��Ĵ���
		if (chessType[chesstype].type & 8)
		{
			MsgBox("�����ֲ�֧��������ģ�", "error", 0);
		}
		if (GameMode_2 == 0)//ִ��
		{
			if (BlackE.GetStatus() == 1)
			{
				GameMode = 4;
				BlackE.WriteMsg("new black\n");
			}
			else//��ִ��
			{
				GameMode = 0;
				if (chessType[chesstype].type & 4)//����pass
				{
					OpenPB();
				}
			}
		}
		else
		{
			if (WhiteE.GetStatus() == 1)
			{
				GameMode = 5;
				WhiteE.WriteMsg("new white\n");
			}
			else
				GameMode = 1;
		}
	}
}

/**
 * StopGame - ��������
 * description��	�ú������ڽ��������߼����û�ǿ�ƽ������ĺ�����ʱ����
 */
void Game::StopGame()
{
	if(IsStop()==TRUE)
	{
		MsgBox("������δ��ʼ","error",3000);
		return;
	}
	MainWnd->GameStop();//�رռ�ʱ��
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
			if (player == BLACK)
				OpenOCB();
			else
				OpenOCW();
			break;
		case 0://�ŷ�����
			if (player == BLACK)
				OpenCB();
			else
				OpenCW();
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
				if (player == BLACK)
					OpenOCB();
				else
					OpenOCW();
				break;
			case 0://�ŷ�����
				if (player == BLACK)
					OpenCB();
				else
					OpenCW();
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

/**
 * MoveStep - ����
 * @step��	��������
 * description��	�ú������ڿ�������ģ���������һ�˵������ŷ����н���
 */
bool Game::MoveStep(char *step)
{
	char curCmd[256], denCmd[256];
	CT_ProcessMove(step, curCmd, denCmd);
	//��NetShell���ã�Ϊ������һ��ƽ̨�������߼����롣
	if (curCmd[0] != '\0')//���ظ�������һ��ƽ̨�����������Ϊ��һ��ƽ̨������ά�������Բ����ڷǷ��ŷ����ʲ����С�error�����
	{
		NetShell(NULL, ConnectMode ? SERVERINFO.s : CLIENTINFO.s, curCmd, strlen(curCmd) + 1, 2);
	}
	if (denCmd[0] != '\0')//�������������ֵ�����
	{
		if (GameMode == 4)//����ִ��
		{
			BlackE.WriteMsg(denCmd);
		}
		else if (GameMode == 5)//����ִ��
		{
			WhiteE.WriteMsg(denCmd);
		}
		else//������
		{
			if (chessType[chesstype].type & 4)
			{
				if (GameMode == BLACK)
					OpenPB();
				else
					OpenPW();
			}
	}
	}
	return true;
}

void Game::OkMove()
{
	char denCmd[256];
	BYTE player = CT_GetCurPlayer();
	int temp = GameMode;//��¼�����ŷ�ǰ��GameMode����ֹ���ʤ��-1ʹ�ŷ����ܷ����Է�����
	if (CT_OkMove(denCmd) != 0)
	{
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
			if ((chessType[chesstype].type & 4) && GameMode == 3)
			{
				OpenPW();
			}
			CloseOCB();
		}
		else//���ð׷���ť
		{
			if ((chessType[chesstype].type & 4) && GameMode == 3)
			{
				OpenPB();
			}
			CloseOCW();
		}
	}
}

void Game::CancelMove()
{
	BYTE player = CT_GetCurPlayer();
	if (CT_CancelMove() == 1)
	{
		if (player == 0)
		{
			CloseOCB();
			if (chessType[chesstype].type & 4)
			{
				OpenPB();
			}
		}
		else
		{
			CloseOCW();
			if (chessType[chesstype].type & 4)
			{
				OpenPW();
			}
		}
	}
}