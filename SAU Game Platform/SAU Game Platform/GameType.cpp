/**
 * file: GameType.cpp
 * date: 2014/9/16
 * version: 1.0
 * description: ���ֹ����������ְ���ά������֧��ģ��ӿڡ�ͨ����̬���ɼ̳���CChess������������������ģ��ļ��أ�
 *                        Ӧ���麯���Ķ�̬������ʵ�ֶ�����ģ��ӿڡ�
 */


#include "GameType.h"
#include "Control.h"
#include "CMainWnd.h"



//ƽ̨����֧��ģ��ӿں���ָ��
_CheckModule CT_CheckModule = NULL;
_InitModule CT_InitModule = NULL;

CChess *GameType;

int chesstype=0;//���ֱ�־
int chessNum=0;//�������ְ�����
struct _CHESSTYPE chessType[25];//��������


/**
 * FindChessTypeReForDir - ���Ҳ��ռ�ָ��Ŀ¼�µ�����֧��ģ��
 * @Dir:	ָ��Ŀ¼
 */
VOID FindChessTypeReForDir(char *Dir)
{
	HANDLE hFile;
	WIN32_FIND_DATA fData;
	char directory[MAX_PATH] = { 0 };
	char filename[MAX_PATH] = { 0 };
	char check[50];
	strcpy(directory, Dir);
	strcat(directory, "\\*.dll");
	//�����������԰�
	hFile = FindFirstFile(directory, &fData);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	do{
		strcpy(filename, Dir);
		strcat(filename, "\\");
		strcat(filename, fData.cFileName);
		chessType[chessNum].chessTP = (HINSTANCE)LoadLibrary(filename);
		if (chessType[chessNum].chessTP == NULL)
		{
			ErrorBox("Load ChessType resource failed !");
			return;
		}
		else
		{
			CT_CheckModule = (_CheckModule)GetProcAddress(chessType[chessNum].chessTP, "CheckModule");
			if (CT_CheckModule != NULL)
			{
				memset(check, 0, sizeof(check));
				CT_CheckModule(check, chessType[chessNum].chessStr, &chessType[chessNum].type);
				if (strcmp("SAU Game Platform chessType", check) == 0)//У�������ȷ��
				{
					strcpy(chessType[chessNum].LibPath, Dir);
					chessNum++;
				}
				else
					FreeLibrary(chessType[chessNum].chessTP);//�ͷŲ���ȷ���
			}
			else
				FreeLibrary(chessType[chessNum].chessTP);//�ͷŲ���ȷ���
		}
	} while (FindNextFile(hFile, &fData));
}

/**
 * GetChessTypeResourse - ��ȡ���ְ���Դ
 */
BOOL GetChessTypeResourse()
{
	HANDLE hFile;
	WIN32_FIND_DATA fData;
	char directory[MAX_PATH]={0};
	char filename[MAX_PATH]={0};
	char check[50];
	strcpy(directory,gameSet.CurDir);
	strcat(directory,"\\ChessType\\*");
	//�����������԰�
	hFile=FindFirstFile(directory,&fData);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		ErrorBox("Find first ChessType resourse failed!");
		return FALSE;
	}
	chessNum=0;
	do{
		if(fData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)//�ļ���
		{
			if(strcmp(fData.cFileName,".")==0 ||strcmp(fData.cFileName,"..")==0)//������Ŀ¼��'.'���ϼ�Ŀ¼��'..'
				continue;
			strcpy(directory, gameSet.CurDir);
			strcat(directory, "\\ChessType\\");
			strcat(directory, fData.cFileName);
			FindChessTypeReForDir(directory);
		}
	}while(FindNextFile(hFile,&fData));

	strcpy(directory, gameSet.CurDir);
	strcat(directory, "\\ChessType");
	FindChessTypeReForDir(directory);

	if (chessNum == 0)
	{
		ErrorBox("Find first ChessType resourse failed!");
		return FALSE;
	}
	return TRUE;
}

/*
 * CreateChessTypeMenu - ���������Ӳ˵�
 */
VOID CreateChessTypeMenu(HMENU hMenu)
{
	char string[25];
	HMENU hChessType=NULL;
	MENUITEMINFO menuItem;
	menuItem.cbSize=sizeof(MENUITEMINFO);
	menuItem.fMask=MIIM_TYPE|MIIM_ID|MIIM_DATA|MIIM_STATE;
	menuItem.fType=MFT_STRING;
	menuItem.cch=25;
	menuItem.dwTypeData=string;
	hChessType=CreatePopupMenu();//�����Ӳ˵�
	if(hChessType==NULL)
	{
		ErrorBox("Create ChessType Menu failed !");
		return;
	}
	for(int i=0;i<chessNum;i++)
	{
		menuItem.fState=MFS_UNCHECKED;
		menuItem.dwItemData=CHESSTYPE+i;//ָ��Ψһ��ʶ
		menuItem.wID=CHESSTYPE+i;
		strcpy(menuItem.dwTypeData,chessType[i].chessStr);
		menuItem.cch=strlen(menuItem.dwTypeData);
		if(!InsertMenuItem(hChessType,-1,true,&menuItem))//������������Ӳ˵�
			ErrorBox("Insert MenuItem failed !");
	}
	menuItem.cch=25;
	if(!GetMenuItemInfo(hMenu,IDM_GAME_SELECT,false,&menuItem))//��ȡѡ�����ֲ˵�����Ϣ
	{
		ErrorBox("Get MenuItem information failed !");
	}
	else
	{
		menuItem.fMask=MIIM_TYPE|MIIM_SUBMENU;//����˵�����Ӳ˵�
		menuItem.hSubMenu=hChessType;
		if(!SetMenuItemInfo(hMenu,IDM_GAME_SELECT,false,&menuItem))
			ErrorBox("Set MenuItem information failed !");
	}
//	CloseHandle(hChessType);//�ر������Ӳ˵������ʱ������⣬��֪����ʲôԭ��
}

/*
 * InitialChessType - ��ʼ�����ֽӿ�
 * @i	��������
 * description���������ڳ�ʼ������֧��ģ�飬�ڳ��������ڴ���֮ǰ���е��á�
 *				�ڳ��������ڼ䣬����ģ���ڲ��й����̻��ƵĲ����仯�����������ڵ�״̬�����ڴ��ڴ�����Ӧ��Ϣʱ���ýӿں������д���
 *				MainWnd���ڴ����ȴ���WM_CREATE��Ϣ����WM_SIZE��Ϣ������WM_PAINT��Ϣ����������ʱ����WM_DESTROY��Ϣ
 *				CT_InitModule�����ĵ�����MainWnd��������WM_CREATE��Ϣʱ��ɣ����ڴ��ݴ��ھ���ͳ���������Ϣ
 *				CT_OnSize�����ĵ�����MainWnd��������WM_SIZE��Ϣʱ��ɣ����ڴ�������λ��
 *				CT_DrawBoard�����ĵ�����MainWnd�ػ洦��WM_PAINT��Ϣʱ��ɣ����ڻ�����������
 *				CT_ExitModule�����ĵ�����MainWnd���ٴ���WM_DESTROY��Ϣʱ��ɣ������ͷ�����ģ���е�ϵͳ��Դ
 */
VOID InitialChessType(HMENU hMenu)
{
	int i;

	//�����Ա�־
	chesstype = 0;
	for (i = 0; i < chessNum; i++)
	{
		//ʹ��Ĭ�����ֲ���������Դ�����ʧ�����õ�һ��������Դ
		if (strcmp(chessType[i].chessStr, gameSet.DefualtChess) == 0)
		{
			chesstype = i;
			break;
		}
	}

	// ���ѡ������
	for(i=0;i<chessNum;i++)
	{
		if(i==chesstype)
			CheckMenuItem(hMenu,CHESSTYPE+i,MF_CHECKED);
		else
			CheckMenuItem(hMenu,CHESSTYPE+i,MF_UNCHECKED);			
	}

	//��̬��ȡDLL���к�����ַ
	CT_InitModule=(_InitModule)GetProcAddress(chessType[chesstype].chessTP,"InitModule");

	//�����Զ���ͼĿ¼
	strcpy(gameSet.PrintScrDir, ".\\chess manual\\");
	strcat(gameSet.PrintScrDir, chessType[chesstype].chessStr);
	strcat(gameSet.PrintScrDir, "\\bmp");
	CreateFolder(gameSet.PrintScrDir);
	//�������ױ���Ŀ¼
	strcpy(gameSet.cmDir, ".\\chess manual\\");
	strcat(gameSet.cmDir, chessType[chesstype].chessStr);
	strcat(gameSet.cmDir, "\\sgf");
	CreateFolder(gameSet.cmDir);

	return;
}

/*
 * SetChessType - ��������
 * @i:	��������
 */
VOID SetChessType(int i,HMENU hMenu)
{	
	if(i==chesstype||i>=chessNum)//��ѡ���ַǵ�ǰ����
		return;
	if (game.GameMode != -1)
	{
		MsgBox("�������ڽ��У����ܽ��������л���", "error", 0);
		return;
	}

	delete GameType;

	chesstype=i;//�������Ա�־
	strcpy(gameSet.DefualtChess, chessType[chesstype].chessStr);
	for(int i=0;i<chessNum;i++)
	{
		if(i==chesstype)
			CheckMenuItem(hMenu,CHESSTYPE+i,MF_CHECKED);
		else
			CheckMenuItem(hMenu,CHESSTYPE+i,MF_UNCHECKED);			
	}
	//��̬��ȡDLL���к�����ַ
	CT_InitModule = (_InitModule)GetProcAddress(chessType[chesstype].chessTP, "InitModule");

	GameType = (CChess*)CT_InitModule(MainWnd->hWnd, chessType[chesstype].LibPath);
	GameType->SetBoard(MainWnd->GetBoardPos());
	InvalidateRect(MainWnd->hWnd,NULL,false);

	//�����Զ���ͼĿ¼
	strcpy(gameSet.PrintScrDir,".\\chess manual\\");
	strcat(gameSet.PrintScrDir,chessType[chesstype].chessStr);
	strcat(gameSet.PrintScrDir,"\\bmp");
	CreateFolder(gameSet.PrintScrDir);
	//�������ױ���Ŀ¼
	strcpy(gameSet.cmDir, ".\\chess manual\\");
	strcat(gameSet.cmDir,chessType[chesstype].chessStr);
	strcat(gameSet.cmDir,"\\sgf");
	CreateFolder(gameSet.cmDir);

	return;
}
