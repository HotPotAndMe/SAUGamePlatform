/**
 * file: Menu.cpp
 * date: 2014/9/10
 * version: 1.0
 * description: �û��˵��ǹ���SAU Game Platformͼ���û�����Ļ���Ԫ��֮һ����е����û��Գ�����Ƶ�ȫ��������
 */


#include "Menu.h"
#include "Function.h"


HMENU hMenu;//�˵����
map<int,FUNCTION> menuFunction;//�˵����ӳ���


/*
 * mapFunction - �˵������������ӳ��
 * @menuID:		�˵���ID
 * @fp:			�˵���ʽ�ļ�ָ��
 */
void mapFunction(int menuID,FILE *fp)
{
	int type,index;
	fscanf(fp,"%d",&type);
	switch(type)
	{
	case 0:
		fscanf(fp,"%d",&index);
		switch(index)
		{
		case IDF_EXIT:
			menuFunction.insert(pair<int, FUNCTION>(menuID, Fuction::Exit));
			break;
		case IDF_BLACK_LOAD:
			menuFunction.insert(pair<int, FUNCTION>(menuID, Fuction::LoadBlackEngine));
			break;
		case IDF_BLACK_UNLOAD:
			menuFunction.insert(pair<int, FUNCTION>(menuID, Fuction::UnloadBlackEngine));
			break;
		case IDF_WHITE_LOAD:
			menuFunction.insert(pair<int, FUNCTION>(menuID, Fuction::LoadWhiteEngine));
			break;
		case IDF_WHITE_UNLOAD:
			menuFunction.insert(pair<int, FUNCTION>(menuID, Fuction::UnloadWhiteEngine));
			break;
		case IDF_GAME_START:
			menuFunction.insert(pair<int, FUNCTION>(menuID, Fuction::GameStart));
			break;
		case IDF_GAME_STOP:
			menuFunction.insert(pair<int, FUNCTION>(menuID, Fuction::GameStop));
			break;
		case IDF_MANUAL_SHOT:
			menuFunction.insert(pair<int, FUNCTION>(menuID, Fuction::ShotBoard));
		default:
			break;
		}
		break;
	}
}


/**
 * CreateUserMenu - �����û��˵�
 * @return��	���ز˵����
 */
HMENU CreateUserMenu()
{
	FILE *fp;
	char tag[30];
	HMENU hMenu=NULL;

	fp=fopen("default.menu","rt");
	if(fp==NULL)
	{
		MsgBox("Open menu script file fail!","error",0);
		return NULL;
	}
	while(fscanf(fp,"%s",tag)!=EOF)
	{
		if(strcmp(tag,"MAIN")==0)//�������˵�
			hMenu=CreateMainMenu(fp);
	}

	return hMenu;
}

/**
 * CreateMainMenu - �������˵�
 * @return��	�������˵����
 */
HMENU CreateMainMenu(FILE *fp)
{
	char tag[30];
	char string[80],num[10];
	HMENU hMenu=NULL;
	MENUITEMINFO menuItem;

	menuItem.cbSize=sizeof(MENUITEMINFO);
	menuItem.fType=MFT_STRING;
	menuItem.dwTypeData=string;
	hMenu=CreateMenu();//�����˵�
	if(hMenu==NULL)
	{
		ErrorBox("Create Language Menu failed !");
		return NULL;
	}

	while(fscanf(fp,"%s",tag)!=EOF)
	{
		if(strcmp(tag,"ITEM")==0)//�˵���
		{
			menuItem.fMask=MIIM_TYPE|MIIM_ID|MIIM_DATA|MIIM_STATE;
			menuItem.fState=MFS_UNCHECKED;
			while(fscanf(fp,"%s",tag)!=EOF)//��ѡ��Ŀ
			{
				if(strcmp(tag,"NAME")==0)
				{
					fscanf(fp,"%s",menuItem.dwTypeData);
					//fgets(menuItem.dwTypeData,sizeof(string)-1,fp);//����
					menuItem.cch=strlen(menuItem.dwTypeData);
				}
				else if(strcmp(tag,"ID")==0)
				{
					fscanf(fp,"%d",&menuItem.dwItemData);//ָ��Ψһ��ʶ
					menuItem.wID=menuItem.dwItemData;
				}
				else if(strcmp(tag,"FUNCTION")==0)
				{
					mapFunction(menuItem.dwItemData,fp);
				}
				else if(strcmp(tag,"ENDI")==0)
				{
					if(!InsertMenuItem(hMenu,-1,true,&menuItem))//����˵���
						ErrorBox("Insert MenuItem failed !");
					break;
				}
			}
		}
		else if(strcmp(tag,"SUB")==0)//�Ӳ˵�
		{
			menuItem.fMask=MIIM_TYPE|MIIM_SUBMENU;
			fscanf(fp,"%s",tag);
			fscanf(fp,"%s",menuItem.dwTypeData);
			//fgets(menuItem.dwTypeData,sizeof(string)-1,fp);//����
			menuItem.cch=strlen(menuItem.dwTypeData);
			menuItem.hSubMenu=CreateSubMenu(fp);
			if(!InsertMenuItem(hMenu,-1,true,&menuItem))//�����Ӳ˵�
				ErrorBox("Insert MenuItem failed !");
		}
		else if(strcmp(tag,"ENDM")==0)//���˵�����
			break;
	}

	return hMenu;
}

/**
 * CreateSubMenu - �����Ӳ˵�
 * @return��	�����Ӿ��
 */
HMENU CreateSubMenu(FILE *fp)
{
	char tag[30];
	char string[80];
	int type;
	HMENU hMenu=NULL;
	MENUITEMINFO menuItem;

	menuItem.cbSize=sizeof(MENUITEMINFO);
	menuItem.fType=MFT_STRING;
	menuItem.dwTypeData=string;
	hMenu=CreatePopupMenu();//�����˵�
	if(hMenu==NULL)
	{
		ErrorBox("Create Language Menu failed !");
		return NULL;
	}

	while(fscanf(fp,"%s",tag)!=EOF)
	{
		if(strcmp(tag,"ITEM")==0)//�˵���
		{
			menuItem.fMask=MIIM_TYPE|MIIM_ID|MIIM_DATA|MIIM_STATE;
			menuItem.fState=MFS_UNCHECKED;
			while(fscanf(fp,"%s",tag)!=EOF)//��ѡ��Ŀ
			{
				if(strcmp(tag,"NAME")==0)
				{
					fscanf(fp,"%s",menuItem.dwTypeData);
					//fgets(menuItem.dwTypeData,sizeof(string)-1,fp);//����
					menuItem.cch=strlen(menuItem.dwTypeData);
				}
				else if(strcmp(tag,"ID")==0)
				{
					fscanf(fp,"%d",&menuItem.dwItemData);//ָ��Ψһ��ʶ
					menuItem.wID=menuItem.dwItemData;
				}
				else if(strcmp(tag,"FUNCTION")==0)
				{
					mapFunction(menuItem.dwItemData,fp);
				}
				else if(strcmp(tag,"ENDI")==0)
				{
					if(!InsertMenuItem(hMenu,-1,true,&menuItem))//����˵���
						ErrorBox("Insert MenuItem failed !");
					break;
				}
			}
		}
		else if(strcmp(tag,"SUB")==0)//�Ӳ˵�
		{
			menuItem.fMask=MIIM_TYPE|MIIM_SUBMENU;
			fscanf(fp,"%s",tag);
			fscanf(fp,"%s",menuItem.dwTypeData);
			//fgets(menuItem.dwTypeData,sizeof(string)-1,fp);//����
			menuItem.cch=strlen(menuItem.dwTypeData);
			menuItem.hSubMenu=CreateSubMenu(fp);
			if(!InsertMenuItem(hMenu,-1,true,&menuItem))//�����Ӳ˵�
				ErrorBox("Insert MenuItem failed !");
		}
		else if(strcmp(tag,"ENDS")==0)//�Ӳ˵�����
			break;
	}

	return hMenu;
}