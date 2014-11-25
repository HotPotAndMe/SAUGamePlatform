/**
 * file: Control.h
 * date: 2014/9/13
 * version: 1.0
 * description: 
 */


#include "Control.h"
#include "GameType.h"

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
	int tag;

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
	while(1)
	{
		side->GetCommand("move",rMsg);//��ȡ�����¼�
		if(CT_GetCurPlayer()!=(int)lpParam)//��ǰ���巽�뱾������ִ����ɫ��ͬ
			continue;
		tag=CT_ProcessMsg(rMsg,wMMsg,wDMsg);//���������¼���������Ӧ����
		if(wMMsg[0]!='\0')
			side->WriteMsg(wMMsg);
		if(wDMsg[0]!='\0')
		{
			if(game.GameMode==2)//�������ģ������Է�����
				unside->WriteMsg(wDMsg);
		}
		if(tag==2)
			game.GameMode=-1;
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
	return true;
}

void Game::StartGame()
{
	if(IsStop()==FALSE)
	{
		MsgBox("�������ڽ���","error",0);
		return;
	}
	CT_OnRun();//��ʼ�����
		if(BlackE.GetLoaded() && WhiteE.GetLoaded())//�ڰ����涼������
		{
			GameMode=2;
			BlackE.WriteMsg("new black\n");
			WhiteE.WriteMsg("new white\n");
		}
		else if(BlackE.GetLoaded() || WhiteE.GetLoaded())//ֻ����һ������
		{
			if(chessType[chesstype].type!=0)
			{
				MsgBox("�����ֲ�֧���˻����ģ�","error",0);
				return;
			}
			if(BlackE.GetLoaded())
			{
				GameMode=1;
				BlackE.WriteMsg("new black\n");
			}			
			else
			{
				GameMode=0;
				WhiteE.WriteMsg("new white\n"); 
			}								
		}
		else//���˶���
		{
			if(chessType[chesstype].type!=0)
			{
				MsgBox("�����ֲ�֧�����˶��ģ�","error",0);
				return;
			}
			GameMode=3;
		}

}

void Game::StopGame()
{
	if(IsStop()==TRUE)
	{
		MsgBox("������δ��ʼ","error",3000);
		return;
	}
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
	char wMsg[256];
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
		temp=CT_OnLButtonDown(x,y,wMsg);
		switch(temp)
		{
		case -1://�����ŷ�
			MsgBox("error step!","error",1000);
			return false;
		case 0://�ŷ�����
			break;
		case 1://�ŷ�����
			break;
		case 2://���Ľ���
			GameMode=-1;
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
			temp=CT_OnLButtonDown(x,y,wMsg);
			switch(temp)
			{
			case -1://�����ŷ�
				MsgBox("error step!","error",1000);
				return false;
			case 0://�ŷ�����
				break;
			case 1://�ŷ�����
				break;
			case 2://���Ľ���
				GameMode=-1;
				break;
			}
			player=CT_GetCurPlayer();
			if(player!=GameMode)//���廻��
			{
//				if(NetWork==0)
//				{
					if(player==BLACK)
						BlackE.WriteMsg(wMsg);
					else
						WhiteE.WriteMsg(wMsg);
//				}
//				else
//				{
//					if(player==BLACK)
//						NetShell(NULL,ConnectMode?SERVERINFO.s:CLIENTINFO.s,wMsg,strlen(wMsg)+1,2);
//					else
//						NetShell(NULL,ConnectMode?SERVERINFO.s:CLIENTINFO.s,wMsg,strlen(wMsg)+1,2);
//				}
			}
		}
		else
		{
//			if(NetWork==0)
//			{
//				MsgBox("The computer hasn't move,stop clicking in board!","error",1500);
//			}
//			else
//			{
				MsgBox("Opponent hasn't move,stop clicking in board!","error",1500);
//			}
		}
	}
	return true;
}

bool Game::MoveStep(char *step)
{
	char wMMsg[256],wDMsg[256];
	int temp;
	temp=CT_ProcessMsg(step,wMMsg,wDMsg);
	if(GameMode==4)
	{
		BlackE.WriteMsg(wDMsg);
	}
	else if(GameMode==5)
	{
		WhiteE.WriteMsg(wDMsg);
	}
	if(temp==2)
		GameMode=-1;
	return true;
}
