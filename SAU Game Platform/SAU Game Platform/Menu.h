/**
 * file: Menu.h
 * date: 2014/9/10
 * version: 1.0
 * description: 
 */


#ifndef MENU_H_GAMEPLATFORM
#define MMNU_H_GAMEPLATFORM


#include "Game Platform.h"


void mapFunction(int menuID,FILE *fp);//�˵������������ӳ��
HMENU CreateUserMenu();//�����û��˵�
HMENU CreateMainMenu(FILE *fp);//�������˵�
HMENU CreateSubMenu(FILE *fp);//�����Ӳ˵�


extern HMENU hMenu;//�˵����
extern map<int,FUNCTION> menuFunction;//�˵����ӳ���


#endif