/**
 * file: Function.h
 * date: 2014/9/10
 * version: 1.0
 * description: 
 */


#ifndef MENU_H_GAMEPLATFORM
#define MMNU_H_GAMEPLATFORM


#include "Game Platform.h"


//ƽ̨�ṩ���ܽӿ�
class Function
{
public:
	//�˵������ù���
	static int Exit();//�˳�����
	static int LoadBlackEngine();//���غڷ�����
	static int UnloadBlackEngine();//ж�غڷ�����
	static int LoadWhiteEngine();//���ذ׷�����
	static int UnloadWhiteEngine();//ж�ذڷ�����
	static int GameStart();//��ʼ����
	static int GameStop();//��������
	static int OkMove();//ȷ���ŷ�
	static int CancelMove();//ȡ���ŷ�
	static int CheckEngineLoad();
	static int ShotBoard();//��ͼ
	static int NetworkStart();//���������ս
	static int NetworkStop();//�ر������ս

	//�˵��������ù���
	static int MoveStep(int x, int y);//�û�����
	static int SaveBoardShot();//�����ͼ������ʵ���������Զ���ͼ
	static int SaveChessManual(char *step);//�������ף�����ʵ���������Զ���������
};


#endif