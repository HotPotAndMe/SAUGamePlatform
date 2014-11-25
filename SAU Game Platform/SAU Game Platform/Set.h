/**
 * file: Set.h
 * date: 2014/11/1
 * version: 1.0
 * description: SAU_GamePalform �������ù���ͷ�ļ������ݰ�����
 */


#ifndef SET_H_GAMEPLATFROM
#define SET_H_GAMEPLATFROM

#include "Game Platform.h"


//��������
typedef struct _GameSet{
	//·������
	char CurDir[MAX_PATH];//��ǰӦ�ó����Ŀ¼
	char BoardFileName[MAX_PATH];//���̲���·��
	char BkPicFileName[MAX_PATH];//����ͼƬ·��
	char BlcPerFileName[MAX_PATH];//�ڷ�����ͼƬ·��
	char WhtPerFileName[MAX_PATH];//�׷�����ͼƬ·��
	char BkMusicFileName[MAX_PATH];//��������·��
	char cmDir[MAX_PATH];//���ױ���·��
	char PrintScrDir[MAX_PATH];//��ͼ·��
	char cmFileName[MAX_PATH];//�����ļ�·��
	char EngineInitDir[MAX_PATH-100];//�����ʼĿ¼

	//ƽ̨�������־
	bool fullScr;//ȫ�� ��־
	bool SingleStep;//������������ ��־
	bool AllStep;//һ�𱣴����� ��־
	bool PrintScr;//��ͼ ��־
	bool CirclePlay;//ѭ�����ű������� ��־
	bool StopPlay;//ֹͣ���ű������� ��־
	bool swEngine;//��ʾ������Ϣ ��־
	int indexBdPic;//���̲���
	int indexBkPic;//����ͼƬ
	int indexBlcPer;//����ͼƬ
	int indexWhtPer;
	int volume;//����
}GameSet;


VOID InitialWithIni();
VOID RecordUserOper();

extern GameSet gameSet;


#endif