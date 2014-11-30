#include "CommFunc.h"

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

//ɾ���ļ����µ��ļ�
VOID __cdecl DelFile(char *dir,bool all)
{
	char szPath[MAX_PATH]={0};
	char directroy[MAX_PATH];
	strcpy(szPath,dir);
	strcat(szPath,"/*");
	WIN32_FIND_DATA fData;
	HANDLE hFile=FindFirstFile(szPath,&fData);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		return;
	} 
	do
	{
		if(fData.dwFileAttributes==FILE_ATTRIBUTE_ARCHIVE)//�ļ�
		{
			char filename[MAX_PATH]={0};
			strcpy(filename,dir);
			strcat(filename,"\\");
			strcat(filename,fData.cFileName);
			DeleteFile(filename);		
		}
		else if(fData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY&&all)//�ļ���
		{
			if(strcmp(fData.cFileName,".")==0 ||strcmp(fData.cFileName,"..")==0)//������Ŀ¼��'.'���ϼ�Ŀ¼��'..'
				continue;
			strcpy(directroy,dir);
			strcat(directroy,"\\");
			strcat(directroy,fData.cFileName);
			DelFile(directroy,all);
		}
	}while(FindNextFile(hFile,&fData));
	return;
}

//��ʾͼƬ
BOOL __cdecl DisplayImage(HDC hDC, LPCTSTR szImagePath,RECT *rt)//չʾbmp��jpg��gif����һ֡��ͼƬ
{
	//���ȴ�szImagePathָ���ļ�
	HANDLE hFile=CreateFile(szImagePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 
	if(hFile==INVALID_HANDLE_VALUE)
		return FALSE;
	DWORD dwFileSize=GetFileSize(hFile, NULL); //���ͼƬ�ļ��Ĵ�С����������ȫ���ڴ�
	HGLOBAL hImageMemory=GlobalAlloc(GMEM_MOVEABLE, dwFileSize); //��ͼƬ����ȫ���ڴ�
	void *pImageMemory=GlobalLock(hImageMemory); //�����ڴ�
	DWORD dwReadedSize; //����ʵ�ʶ�ȡ���ļ���С
	ReadFile(hFile, pImageMemory, dwFileSize, &dwReadedSize, NULL); //��ȡͼƬ��ȫ���ڴ浱��
	GlobalUnlock(hImageMemory); //�����ڴ�
	CloseHandle(hFile); //�ر��ļ����
	IStream *pIStream;//����һ��IStream�ӿ�ָ�룬��������ͼƬ��
	IPicture *pIPicture;//����һ��IPicture�ӿ�ָ�룬��ʾͼƬ����
	CreateStreamOnHGlobal(hImageMemory, false, &pIStream); //��ȫ���ڴ��ʹ��IStream�ӿ�ָ��
	OleLoadPicture(pIStream, 0, false, IID_IPicture, (LPVOID*)&(pIPicture));//��OleLoadPicture���IPicture�ӿ�ָ��
	//�õ�IPicture COM�ӿڶ������Ϳ��Խ��л��ͼƬ��Ϣ����ʾͼƬ�Ȳ���
	OLE_XSIZE_HIMETRIC hmWidth;
	OLE_YSIZE_HIMETRIC hmHeight;	
	pIPicture->get_Width(&hmWidth); //�ýӿڷ������ͼƬ�Ŀ�͸�
	pIPicture->get_Height(&hmHeight);	
	//��ָ����DC�ϻ��ͼƬ
	pIPicture->Render(hDC,rt->left,rt->top,rt->right-rt->left,rt->bottom-rt->top,0,hmHeight,hmWidth,-hmHeight,NULL); 
	GlobalFree(hImageMemory); //�ͷ�ȫ���ڴ�
	pIStream->Release(); //�ͷ�
	pIPicture->Release(); //�ͷ�pIPicture	
	/*HWND hWnd=::WindowFromDC(hDC);
	static CPictureEx m_flash(hWnd);
	m_flash.Load(szImagePath);
	m_flash.Draw();*/
	return TRUE;
}

//��������
VOID __cdecl PlayMusic(char *filename,bool circle,int vol)
{	
	char shortname[MAX_PATH];
	char cmd[MAX_PATH+100];	
	memset(shortname,sizeof(shortname),0);
	memset(cmd,sizeof(cmd),0);
		
	GetShortPathName(filename,shortname,sizeof(shortname)/sizeof(char));//����ļ������пո���ȥ���ո�
	if(circle)
		sprintf(cmd,"play %s repeat",shortname);
	else
		sprintf(cmd,"play %s",shortname);

	mciSendString(cmd,NULL,0,NULL);	//��������	

	memset(cmd,sizeof(cmd),0);
	sprintf(cmd,"setaudio %s volume to %d",shortname,vol*10);
	mciSendString(cmd,0,0,0);
	return;
}

//ֹͣ���ֲ���
VOID __cdecl StopMusic(char *filename)
{	
	char shortname[MAX_PATH]={0};
	char cmd[MAX_PATH+100]={0};		
		
	GetShortPathName(filename,shortname,sizeof(shortname)/sizeof(char));//����ļ������пո���ȥ���ո�
	sprintf(cmd,"stop %s",shortname);

	mciSendString(cmd,NULL,0,NULL);	//ֹͣ����
	return;
}

//����
BOOL __cdecl PrintScreen(char *filename,int left,int top,int width,int height)
{
	HDC hDC=GetDC(NULL);
	HDC hMemDC=CreateCompatibleDC(hDC);
	HBITMAP hBitmap=CreateCompatibleBitmap(hDC,width,height);
	HBITMAP hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBitmap);

	BitBlt(hMemDC,0,0,width,height,hDC,left,top,SRCCOPY);
	
	BITMAPINFO BmpInfo;
	ZeroMemory(&BmpInfo,sizeof(BmpInfo));
	BmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	BmpInfo.bmiHeader.biWidth=width;
	BmpInfo.bmiHeader.biHeight=height;
	BmpInfo.bmiHeader.biPlanes=1;
	BmpInfo.bmiHeader.biBitCount=32;
	BmpInfo.bmiHeader.biCompression=BI_RGB;	
	
	int OffSize=sizeof(BITMAPFILEHEADER)+BmpInfo.bmiHeader.biSize;
	int BmpSize=BmpInfo.bmiHeader.biWidth*BmpInfo.bmiHeader.biHeight*4;
	int FileSize=OffSize+BmpSize;

	BITMAPFILEHEADER FileHeader;
	ZeroMemory(&FileHeader,sizeof(FileHeader));
	FileHeader.bfSize=FileSize;
	FileHeader.bfType='B'+('M'<<8);
	FileHeader.bfOffBits=OffSize;

	PVOID pBmpData;
	pBmpData=HeapAlloc(GetProcessHeap(),HEAP_NO_SERIALIZE,BmpSize);
	ZeroMemory(pBmpData,BmpSize);	
		
	if(GetDIBits(hMemDC,hBitmap,0,BmpInfo.bmiHeader.biHeight,pBmpData,&BmpInfo,DIB_RGB_COLORS)==0)
	{
		ErrorBox("GetDIBits faied");
		return FALSE;
	}

	HANDLE hFile;
	DWORD dwWriten;
	hFile=CreateFile(
		filename,
		GENERIC_READ |GENERIC_WRITE,
		FILE_SHARE_READ |FILE_SHARE_WRITE,
		NULL,CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		ErrorBox("CreateFile faied");
		return FALSE;
	}
	BOOL Written[3];
	Written[0]=WriteFile(hFile,&FileHeader,sizeof(FileHeader),&dwWriten,NULL);
	Written[1]=WriteFile(hFile,&BmpInfo,sizeof(BmpInfo),&dwWriten,NULL);
	Written[2]=WriteFile(hFile,pBmpData,BmpSize,&dwWriten,NULL);
	if(!Written[0] ||!Written[1] ||!Written[2])
	{
		ErrorBox("WriteFile faied");
		CloseHandle(hFile);
		return FALSE;
	}
	CloseHandle(hFile);

	HeapFree(GetProcessHeap(),0,pBmpData);	
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
	DeleteDC(hDC);
	return TRUE;
}

/**
 * PrintScreenEx - ����ָ������ָ�������ͼ��
 * @filename:	Ŀ���ļ���
 * @hWnd:		���ھ��
 * @rtCut:		���ڲü�����
 * @Draw:		��ͼ����
 * return:		TRUE������ɹ���False������ʧ�ܡ�
 * description�������̿ͻ������ڽ��У�ͨ�����ݴ��ڵĻ�ͼ����ָ�룬ʵ�ֶԴ���ʵ��ͼ��Ļ�ȡ�������Ǵ���������������Ļ�������ֵ�ͼ��
 */
BOOL __cdecl PrintScreenEx(char *filename,HWND hWnd,RECT rtCut,void (*Draw)(HDC hDC))
{
	RECT rt;
	GetClientRect(hWnd,&rt);

	HDC hDC=GetDC(hWnd);
	HDC hMemDC=CreateCompatibleDC(hDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, rt.right - rt.left, rt.bottom - rt.top);
	SelectObject(hMemDC,hBitmap);
	Draw(hMemDC);

	int width = rtCut.right - rtCut.left;
	int height = rtCut.bottom - rtCut.top;
	HDC hPicDC = CreateCompatibleDC(hDC); 
	HBITMAP hPicBitmap = CreateCompatibleBitmap(hDC, width, height);
	SelectObject(hPicDC, hPicBitmap);
	BitBlt(hPicDC, 0, 0, width, height, hMemDC, rtCut.left, rtCut.top, SRCCOPY);
	
	BITMAPINFO BmpInfo;
	ZeroMemory(&BmpInfo,sizeof(BmpInfo));
	BmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	BmpInfo.bmiHeader.biWidth=width;
	BmpInfo.bmiHeader.biHeight=height;
	BmpInfo.bmiHeader.biPlanes=1;
	BmpInfo.bmiHeader.biBitCount=32;
	BmpInfo.bmiHeader.biCompression=BI_RGB;	
	
	int OffSize=sizeof(BITMAPFILEHEADER)+BmpInfo.bmiHeader.biSize;//�ļ�ͷ��С
	int BmpSize=BmpInfo.bmiHeader.biWidth*BmpInfo.bmiHeader.biHeight*4;//ͼ����Ϣ��С
	int FileSize=OffSize+BmpSize;//�ļ���С

	BITMAPFILEHEADER FileHeader;
	ZeroMemory(&FileHeader,sizeof(FileHeader));
	FileHeader.bfSize=FileSize;
	FileHeader.bfType='B'+('M'<<8);
	FileHeader.bfOffBits=OffSize;

	PVOID pBmpData;
	pBmpData=HeapAlloc(GetProcessHeap(),HEAP_NO_SERIALIZE,BmpSize);
	ZeroMemory(pBmpData,BmpSize);	
		
	if(GetDIBits(hPicDC,hPicBitmap,0,BmpInfo.bmiHeader.biHeight,pBmpData,&BmpInfo,DIB_RGB_COLORS)==0)
	{
		ErrorBox("GetDIBits faied");
		return FALSE;
	}

	HANDLE hFile;
	DWORD dwWriten;
	hFile=CreateFile(
		filename,
		GENERIC_READ |GENERIC_WRITE,
		FILE_SHARE_READ |FILE_SHARE_WRITE,
		NULL,CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		ErrorBox("CreateFile faied");
		return FALSE;
	}
	BOOL Written[3];
	Written[0]=WriteFile(hFile,&FileHeader,sizeof(FileHeader),&dwWriten,NULL);
	Written[1]=WriteFile(hFile,&BmpInfo,sizeof(BmpInfo),&dwWriten,NULL);
	Written[2]=WriteFile(hFile,pBmpData,BmpSize,&dwWriten,NULL);
	if(!Written[0] ||!Written[1] ||!Written[2])
	{
		ErrorBox("WriteFile faied");
		CloseHandle(hFile);
		return FALSE;
	}
	CloseHandle(hFile);

	HeapFree(GetProcessHeap(),0,pBmpData);	
	DeleteObject(hPicBitmap);
	DeleteDC(hPicDC);
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
	DeleteDC(hDC);
	return TRUE;
}

VOID __cdecl ErrorBox(LPTSTR ErrorInfo)//������ʾ��
{
	CHAR error1[50],error2[20];
	strcpy(error1,ErrorInfo);
	sprintf(error2,"\n\nerror: %d",GetLastError());	
	strcat(error1,error2);	
	MessageBox(NULL,error1,"error",MB_OK);
}

VOID __cdecl MsgBox(LPTSTR msg)//��Ϣ��
{
	MessageBox(NULL,msg,"OK",MB_OK);
}


//��ȡָ������ID�������ھ��
//=================================================================================================
typedef struct tagWNDINFO 
{ 
	DWORD dwProcessId; 
	HWND hWnd; 
} WNDINFO, *LPWNDINFO; 
BOOL CALLBACK YourEnumProc(HWND hWnd,LPARAM lParam) //ö�ٴ��ڽ���ID
{ 
	DWORD dwProcessId; 
	GetWindowThreadProcessId(hWnd, &dwProcessId); 
	LPWNDINFO pInfo = (LPWNDINFO)lParam; 
	if(dwProcessId == pInfo->dwProcessId) 
	{ 
		pInfo->hWnd = hWnd; 
		return FALSE; 
	} 
	return TRUE; 
} 
HWND __cdecl GetProcessMainWnd(DWORD dwProcessId) //��ȡ���̴��ھ��
{ 
	WNDINFO wi; 
	wi.dwProcessId = dwProcessId; 
	wi.hWnd = NULL; 
	EnumWindows(YourEnumProc,(LPARAM)&wi); 
	while(GetParent(wi.hWnd))
		wi.hWnd=GetParent(wi.hWnd);
	return wi.hWnd; 
}


//��ȡϵͳʱ��
VOID __stdcall GetSystemTimeEx(SYSTEMTIME *st,int region)
{
	GetSystemTime(st);
	st->wHour+=region;//�й�ʱ��λ�ڶ�����(�򵥽���ʱ����ӻ��������)
	if(st->wHour>=24)
	{
		st->wHour-=24;
		st->wDay++;
		if(st->wMonth==1 ||st->wMonth==3 ||st->wMonth==5 ||st->wMonth==7 ||st->wMonth==8 ||st->wMonth==10 ||st->wMonth==12)//��31����·�
		{
			if(st->wDay>31)
			{
				st->wDay=1;
				st->wMonth++;
				if(st->wMonth>12)
				{
					st->wMonth=1;
					st->wYear++;
				}
			}
		}
		else if(st->wMonth==4 ||st->wMonth==6 ||st->wMonth==9 ||st->wMonth==11)//��30����·�
		{
			if(st->wDay>30)
			{
				st->wDay=1;
				st->wMonth++;
			}
		}
		else//����
		{
			if(st->wYear%400==0||(st->wYear%4==0&&st->wYear%100!=0))//����
			{
				if(st->wDay>29)
				{
					st->wDay=1;
					st->wMonth++;
				}			
			}
			else
			{
				if(st->wDay>28)
				{
					st->wDay=1;
					st->wMonth++;
				}			
			}
		}
	}
	return;
}
