/**
 * file: Set.h
 * date: 2014/11/1
 * version: 1.0
 * description: SAU_GamePalform �������ù����ļ������ݰ�����
 */


#include "Set.h"


GameSet gameSet;//��������


/*
 * InitialWithIni - ���������ļ����г�ʼ��
 */
VOID InitialWithIni()
{
	//Ĭ������ѡ��
	GetPrivateProfileString("CHESS", "Default", "\0", gameSet.DefualtChess, sizeof(gameSet.DefualtChess), ".\\Config.ini");
	//������Ϣ��ʾ��Ĭ����ʾ
	gameSet.swEngine=GetPrivateProfileInt("SHOW","EngineInfo",TRUE,".\\Config.ini");
	//���̲��ʣ�Ĭ�������̲���
	GetPrivateProfileString("BOARD","Material","\0",gameSet.BoardFileName,sizeof(gameSet.BoardFileName),".\\Config.ini");
	//���ױ���λ��
	GetPrivateProfileString("SAVE","sfgDir","\0",gameSet.cmDir,sizeof(gameSet.cmDir),".\\Config.ini");
	//�Ƿ񵥲��������ף�Ĭ��Ϊ��
	gameSet.SingleStep=GetPrivateProfileInt("SAVE","SingleStep",FALSE,".\\Config.ini");
	//�Ƿ�һ�𱣴����ף�Ĭ��Ϊ��
	gameSet.AllStep=GetPrivateProfileInt("SAVE","AllStep",FALSE,".\\Config.ini");
	//�Ƿ��ͼ��Ĭ��Ϊ��
	gameSet.PrintScr=GetPrivateProfileInt("SAVE","PrintScr",FALSE,".\\Config.ini");
	//��ͼλ��
	GetPrivateProfileString("SAVE","PrintScrDir","\0",gameSet.PrintScrDir,sizeof(gameSet.PrintScrDir),".\\Config.ini");
	//����ͼƬ
	GetPrivateProfileString("PICTURE","BackPicture","\0",gameSet.BkPicFileName,sizeof(gameSet.BkPicFileName),".\\Config.ini");
	//�ڷ�����ͼƬ*/
	GetPrivateProfileString("PICTURE","BlackPer","\0",gameSet.BlcPerFileName,sizeof(gameSet.BlcPerFileName),".\\Config.ini");
	//�׷�����ͼƬ
	GetPrivateProfileString("PICTURE","WhitePer","\0",gameSet.WhtPerFileName,sizeof(gameSet.WhtPerFileName),".\\Config.ini");
	//���������ļ���
	GetPrivateProfileString("MUSIC","BackMuick","\0",gameSet.BkMusicFileName,sizeof(gameSet.BkMusicFileName),".\\Config.ini");
	//������Ĭ��30
	gameSet.volume=GetPrivateProfileInt("MUSIC","Volume",30,".\\Config.ini");
	//����ѭ����Ĭ��ѭ��
	gameSet.CirclePlay=GetPrivateProfileInt("MUSIC","CirclePlay",TRUE,".\\Config.ini");
	//ֹͣ���ţ�Ĭ��ֹͣ
	gameSet.StopPlay=GetPrivateProfileInt("MUSIC","StopPlay",TRUE,".\\Config.ini");
	//��ȡͼƬ����ֵ
	gameSet.indexBdPic=GetPrivateProfileInt("PICTURE","indexBdPic",0,".\\Config.ini");
	gameSet.indexBkPic=GetPrivateProfileInt("PICTURE","indexBkPic",0,".\\Config.ini");
	gameSet.indexBlcPer=GetPrivateProfileInt("PICTURE","indexBlcPer",4,".\\Config.ini");
	gameSet.indexWhtPer=GetPrivateProfileInt("PICTURE","indexWhtPer",4,".\\Config.ini");

	return;
}

/*
 * RecordUsrOper - ��¼�û�����
 */
VOID RecordUserOper()
{
	char num[10];
	//Ĭ������ѡ��
	WritePrivateProfileString("CHESS", "Default", gameSet.DefualtChess, ".\\Config.ini");
	/*д��������Ϣ��ʾ*/
	WritePrivateProfileString("SHOW","EngineInfo",itoa(gameSet.swEngine,num,10),".\\Config.ini");
	/*д�����̲���*/
	WritePrivateProfileString("BOARD","Material",gameSet.BoardFileName,".\\Config.ini");
	/*д�����ױ���λ��*/
	WritePrivateProfileString("SAVE","sfgDir",gameSet.cmDir,".\\Config.ini");
	/*д���Ƿ񵥲���������*/
	WritePrivateProfileString("SAVE","SingleStep",itoa(gameSet.SingleStep,num,10),".\\Config.ini");
	/*д���Ƿ�һ�𱣴�����*/
	WritePrivateProfileString("SAVE","AllStep",itoa(gameSet.AllStep,num,10),".\\Config.ini");
	/*д���Ƿ��ͼ*/
	WritePrivateProfileString("SAVE","PrintScr",itoa(gameSet.PrintScr,num,10),".\\Config.ini");
	/*д���ͼλ��*/
	WritePrivateProfileString("SAVE","PrintScrDir",gameSet.PrintScrDir,".\\Config.ini");
	/*д�뱳��ͼƬ*/
	WritePrivateProfileString("PICTURE","BackPicture",gameSet.BkPicFileName,".\\Config.ini");
	/*д��ڷ�����ͼƬ*/
	WritePrivateProfileString("PICTURE","BlackPer",gameSet.BlcPerFileName,".\\Config.ini");
	/*д��׷�����ͼƬ*/
	WritePrivateProfileString("PICTURE","WhitePer",gameSet.WhtPerFileName,".\\Config.ini");
	/*д�뱳�������ļ���*/
	WritePrivateProfileString("MUSIC","BackMuick",gameSet.BkMusicFileName,".\\Config.ini");
	/*д������*/
	WritePrivateProfileString("MUSIC","Volume",itoa(gameSet.volume,num,10),".\\Config.ini");
	/*д�뵥��ѭ��*/
	WritePrivateProfileString("MUSIC","CirclePlay",itoa(gameSet.CirclePlay,num,10),".\\Config.ini");
	/*д��ֹͣ����*/
	WritePrivateProfileString("MUSIC","StopPlay",itoa(gameSet.StopPlay,num,10),".\\Config.ini");
	/*д��ͼƬ����ֵ*/
	WritePrivateProfileString("PICTURE","indexBdPic",itoa(gameSet.indexBdPic,num,10),".\\Config.ini");
	WritePrivateProfileString("PICTURE","indexBkPic",itoa(gameSet.indexBkPic,num,10),".\\Config.ini");
	WritePrivateProfileString("PICTURE","indexBlcPer",itoa(gameSet.indexBlcPer,num,10),".\\Config.ini");
	WritePrivateProfileString("PICTURE","indexWhtPer",itoa(gameSet.indexWhtPer,num,10),".\\Config.ini");
	return;
}
