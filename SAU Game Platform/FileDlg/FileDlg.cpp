#include "FileDlg.h"

HWND hDlg;

HINSTANCE hInst;

BOOL APIENTRY DllMain( HINSTANCE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			hInst=hModule;
			break;
		case DLL_THREAD_ATTACH:
			hInst=hModule;
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

//�����õ��˾�ת����ϵͳ��ʶ����˾�
void ConvertFilter(LPTSTR Filter)
{//�˾�ָ��һ���Կ��ַ������Ĺ����ַ�����һ�����塣�����е����һ���ַ�������������NULL�ַ�����
	do
	{
		if((*Filter)=='|')
			*Filter='\0';
	}
	while(*(++Filter));
}

//ת���ļ�ȫ·��
void ConvertFileName(char* str)
{
	char path[MAX_PATH];
	memset(path,0,MAX_PATH);
	int i=3;
	strcpy(path,str);
	str+=3;
	*str='\\';	
	do
	{	
		str++;
		if(*str=='\\')
		{
			*str=path[i];
			*++str='/';
			i++;
		}			
		else
			*str=path[i];		
		i++;

	}while(path[i]!='\0');
	str-=i;
}

UINT CALLBACK OFNHookProc(
  HWND hdlg,      // handle to child dialog window
  UINT uiMsg,     // message identifier
  WPARAM wParam,  // message parameter
  LPARAM lParam   // message parameter
)
{
//	hDlg=hdlg;
	return 0;
}

//���캯��,�ڳ�ʼ���ļ���ʱ���ǳ���
CFileDialog::CFileDialog(bool Open,LPCTSTR DefExt,LPTSTR DefFileName,LPTSTR Filter,HWND hWnd,LPCTSTR InitDir)
{
	CHAR m_FileName[MAX_PATH];
	ZeroMemory(m_FileName,sizeof(m_FileName));
	ZeroMemory(&FileName,sizeof(FileName));
	FileName.lStructSize=sizeof(FileName);
	FileName.hwndOwner=hWnd;
	FileName.lpstrInitialDir=InitDir;//default is Current directory
	FileName.Flags=OFN_EXPLORER |OFN_ENABLESIZING |OFN_OVERWRITEPROMPT;// |OFN_ENABLEHOOK ;
	//FileName.lpfnHook=(LPOFNHOOKPROC)OFNHookProc;
	m_FileName[0]='\0';
	FileName.lpstrFile=m_FileName;
	FileName.nMaxFile=MAX_PATH;					

	ConvertFilter(Filter);
	FileName.lpstrFilter=Filter;						

	if(DefExt!=NULL)
	{		
		FileName.lpstrDefExt=DefExt;
	}								
	m_Open=Open;
}

CFileDialog::~CFileDialog()
{

}

//ģʽѡ�񣨴򿪻򱣴棩
INT CFileDialog::DoModal()
{
	INT ID;
	if(m_Open)//ģʽ��־
	{				
		//FileName.lpstrInitialDir="F:\\GamePlatform\\GamePlatform\\BoardMaterial\\";
		ID=GetOpenFileName(&FileName);//���ļ��Ի���
	}
	else
	{
		ID=GetSaveFileName(&FileName);//�����ļ��Ի���
	}	
	return (ID?IDOK:IDCANCEL);
}

//��ȡ�ļ���ȫ·��
LPCTSTR CFileDialog::GetFilePath()
{			
	memset(m_path,0,MAX_PATH);
	strcpy(m_path,FileName.lpstrFile);
	//ConvertFileName(m_path);
	return m_path;
}

//��ȡ�ļ���·��
LPCTSTR CFileDialog::GetFolderPath(LPCTSTR path)
{			
	char *p=m_folder;
	lstrcpyn(m_folder,path,strlen(path));//����·��
	while((*++p));
	while(1)//�ز飬�ҵ��ļ���·��
	{
		if((*p)=='\\')
		{
			(*p)='\0';
			break;
		}		
		(*p)='\0';//����
		p--;
	}
	return m_folder;
}
