/**
 * file: Function.h
 * date: 2014/9/10
 * version: 1.0
 * description: 
 */


#ifndef MENU_H_GAMEPLATFORM
#define MMNU_H_GAMEPLATFORM


#include "Game Platform.h"


int Exit();//�˳�����
int LoadBlackEngine();//���غڷ�����
int UnloadBlackEngine();//ж�غڷ�����
int LoadWhiteEngine();//���ذ׷�����
int UnloadWhiteEngine();//ж�ذڷ�����
int GameStart();//��ʼ����
int GameStop();//��������
int MoveStep(int x,int y);//�û�����
int OkMove();
int CancelMove();
int CheckEngineLoad();
int ShotBoard();



#endif