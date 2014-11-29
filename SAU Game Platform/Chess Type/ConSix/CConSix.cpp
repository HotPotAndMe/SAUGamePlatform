#pragma comment(lib,"winmm.lib")

#include "CConSix.h"
#include <stdio.h>

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

CConSix::CConSix()
{
	BkColor=RGB(0,0,0);
	BoardColor=RGB(255,255,0);
	hPen = NULL;
	hFont = NULL;
	InitGame();
	count = -1;
}

CConSix::~CConSix()
{
	if (hPen != NULL)
		DeleteObject(hPen);
	if (hFont != NULL)
		DeleteObject(hFont);
}

VOID CConSix::SetBoard(RECT rtBoard)
{
	if (hPen != NULL)
		DeleteObject(hPen);
	if (hFont != NULL)
		DeleteObject(hFont);
	this->rtBoard = rtBoard;
	side = rtBoard.right - rtBoard.left;
	d = side / 20;
	pixel = ((double)side) / 600;
	hPen = CreatePen(PS_SOLID, (int)(2 * pixel), RGB(0, 0, 0));
	fWidth = (int)(d / 3);
	fHeight = (int)(d * 2 / 3);
	hFont = CreateSimpleFont(fWidth, fHeight);
}

VOID CConSix::DrawBoard(HDC hDC)//��������
{
	int i, j;
	HPEN hOldPen;
	HFONT hOldFont;
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hOldFont = (HFONT)SelectObject(hDC, hFont);

	::SetTextColor(hDC, RGB(0, 0, 0));

	char letter[2], number[3];
	memset(letter, 0, sizeof(letter));
	memset(number, 0, sizeof(number));
	//��������
	for (i = 0; i < 19; i++)
	{
		letter[0] = 'A' + i;
		itoa(i, number, 10);

		TextOut(hDC, (int)(rtBoard.left + side*(i + 1) / 20 - fWidth / 2), rtBoard.top, letter, 1);//��������				
		TextOut(hDC, (int)(rtBoard.left + side*(i + 1) / 20 - fWidth), rtBoard.bottom - fHeight, number, 2);

		MoveToEx(hDC, rtBoard.left + d, (int)(rtBoard.top + side*(i + 1) / 20), NULL);//������
		LineTo(hDC, rtBoard.right - d, (int)(rtBoard.top + side*(i + 1) / 20));
	}
	for (i = 0; i < 19; i++)
	{
		letter[0] = 'A' + i;
		itoa(i, number, 10);

		TextOut(hDC, rtBoard.left + fWidth, (int)(rtBoard.top + side*(i + 1) / 20 - fHeight / 2), letter, 1);
		TextOut(hDC, rtBoard.right - fWidth * 2, (int)(rtBoard.top + side*(i + 1) / 20 - fHeight / 2), number, 2);

		MoveToEx(hDC, rtBoard.left + side*(i + 1) / 20, rtBoard.top + d, NULL);
		LineTo(hDC, rtBoard.left + side*(i + 1) / 20, rtBoard.bottom - d);
	}

	HDC hAssistDC, hBlcDC, hWhtDC, hCurBlcDC, hCurWhtDC;
	hAssistDC = CreateCompatibleDC(hDC);
	hBlcDC = CreateCompatibleDC(hDC);
	hWhtDC = CreateCompatibleDC(hDC);
	hCurBlcDC = CreateCompatibleDC(hDC);
	hCurWhtDC = CreateCompatibleDC(hDC);

	HBITMAP hAssistBmp, hBlcBmp, hWhtBmp, hCurBlcBmp, hCurWhtBmp;
	hAssistBmp = CreateCompatibleBitmap(hDC, d, d);
	hBlcBmp = CreateCompatibleBitmap(hDC, d, d);
	hWhtBmp = CreateCompatibleBitmap(hDC, d, d);
	hCurBlcBmp = CreateCompatibleBitmap(hDC, d, d);
	hCurWhtBmp = CreateCompatibleBitmap(hDC, d, d);

	SelectObject(hAssistDC, hAssistBmp);
	SelectObject(hBlcDC, hBlcBmp);
	SelectObject(hWhtDC, hWhtBmp);
	SelectObject(hCurBlcDC, hCurBlcBmp);
	SelectObject(hCurWhtDC, hCurWhtBmp);

	DrawAssist(hAssistDC, d);
	DrawChess(hBlcDC, hWhtDC, hCurBlcDC, hCurWhtDC, d);//��������


	for (i = 0; i < 19; i++)
	{
		for (j = 0; j < 19; j++)
		{
			if (board[i][j] == BLACK)
			{
				BitBlt(hDC, rtBoard.left + side*(i + 1) / 20 - d / 2, rtBoard.top + side*(j + 1) / 20 - d / 2, d, d, hAssistDC, 0, 0, SRCPAINT);
				BitBlt(hDC, rtBoard.left + side*(i + 1) / 20 - d / 2, rtBoard.top + side*(j + 1) / 20 - d / 2, d, d, hBlcDC, 0, 0, SRCAND);
			}
			else if (board[i][j] == WHITE)
			{
				BitBlt(hDC, rtBoard.left + side*(i + 1) / 20 - d / 2, rtBoard.top + side*(j + 1) / 20 - d / 2, d, d, hAssistDC, 0, 0, SRCPAINT);
				BitBlt(hDC, rtBoard.left + side*(i + 1) / 20 - d / 2, rtBoard.top + side*(j + 1) / 20 - d / 2, d, d, hWhtDC, 0, 0, SRCAND);
			}
		}
	}

	if (!stepStack.empty())
	{
		Step curStep = stepStack.top();
		if (curStep.side == BLACK)
		{
			int x1 = curStep.first.x; int y1 = curStep.first.y;
			int x2 = curStep.second.x; int y2 = curStep.second.y;
			IsChess(curStep.first)
			{
				BitBlt(hDC, rtBoard.left + side*(x1 + 1) / 20 - d / 2, rtBoard.top + side*(y1 + 1) / 20 - d / 2, d, d, hAssistDC, 0, 0, SRCPAINT);
				BitBlt(hDC, rtBoard.left + side*(x1 + 1) / 20 - d / 2, rtBoard.top + side*(y1 + 1) / 20 - d / 2, d, d, hCurBlcDC, 0, 0, SRCAND);
			}
			IsChess(curStep.second)
			{
				BitBlt(hDC, rtBoard.left + side*(x2 + 1) / 20 - d / 2, rtBoard.top + side*(y2 + 1) / 20 - d / 2, d, d, hAssistDC, 0, 0, SRCPAINT);
				BitBlt(hDC, rtBoard.left + side*(x2 + 1) / 20 - d / 2, rtBoard.top + side*(y2 + 1) / 20 - d / 2, d, d, hCurBlcDC, 0, 0, SRCAND);
			}
		}
		else if (curStep.side == WHITE)
		{
			int x1 = curStep.first.x; int y1 = curStep.first.y;
			int x2 = curStep.second.x; int y2 = curStep.second.y;
			IsChess(curStep.first)
			{
				BitBlt(hDC, rtBoard.left + side*(x1 + 1) / 20 - d / 2, rtBoard.top + side*(y1 + 1) / 20 - d / 2, d, d, hAssistDC, 0, 0, SRCPAINT);
				BitBlt(hDC, rtBoard.left + side*(x1 + 1) / 20 - d / 2, rtBoard.top + side*(y1 + 1) / 20 - d / 2, d, d, hCurWhtDC, 0, 0, SRCAND);
			}
			IsChess(curStep.second)
			{
				BitBlt(hDC, rtBoard.left + side*(x2 + 1) / 20 - d / 2, rtBoard.top + side*(y2 + 1) / 20 - d / 2, d, d, hAssistDC, 0, 0, SRCPAINT);
				BitBlt(hDC, rtBoard.left + side*(x2 + 1) / 20 - d / 2, rtBoard.top + side*(y2 + 1) / 20 - d / 2, d, d, hCurWhtDC, 0, 0, SRCAND);
			}
		}
	}

	DeleteObject(hAssistBmp);
	DeleteObject(hBlcBmp);
	DeleteObject(hWhtBmp);
	DeleteObject(hCurBlcBmp);
	DeleteObject(hCurWhtBmp);

	DeleteDC(hAssistDC);
	DeleteDC(hBlcDC);
	DeleteDC(hWhtDC);
	DeleteDC(hCurBlcDC);
	DeleteDC(hCurWhtDC);
}


bool CConSix::DrawChess(HDC hBlcDC,HDC hWhtDC,HDC hCurBlcDC,HDC hCurWhtDC,int d)
{
	double pixel=0.8,shadow=0.7;
	BOOL Alias=true;

	int red=255-GetRValue(BoardColor);
	int green=255-GetGValue(BoardColor);
	int blue=255-GetBValue(BoardColor);

	COLORREF pb,pw;
	double di,dj,d2=(double)d/2.0-5e-1,r=d2-2e-1,f=sqrt(3.0);
	double x,y,z,xr,xg,hh;
	int g;

	for (int i=0; i<d; i++)
	{
		for (int j=0; j<d; j++)
		{	
			di=i-d2; dj=j-d2;
			hh=r-sqrt(di*di+dj*dj);
			if (hh>=0)
			{
				z=r*r-di*di-dj*dj;
			    if (z>0) z=sqrt(z)*f;
			    else z=0;
				x=di; y=dj;
				xr=sqrt(6*(x*x+y*y+z*z));
				xr=(2*z-x+y)/xr;
				if (xr>0.9) xg=(xr-0.9)*10;
				else xg=0;
				if (hh>pixel || !Alias)
				{
					g=(int)(10+10*xr+xg*140);
					pb=(g<<16)|(g<<8)|g;
					g=(int)(200+10*xr+xg*45);
					pw=(g<<16)|(g<<8)|g;
				}
				else
				{	hh=(pixel-hh)/pixel;
					g=(int)(10+10*xr+xg*140);
					double shade;
					if (di-dj<r/3) shade=1;
					else shade=shadow;

					pb=(//(255<<24)|
					    (((int)((1-hh)*g+hh*shade*red))<<16)
						|(((int)((1-hh)*g+hh*shade*green))<<8)
						|((int)((1-hh)*g+hh*shade*blue)));

					g=(int)(200+10*xr+xg*45);

					pw=(//(255<<24)|
					    (((int)((1-hh)*g+hh*shade*red))<<16)
						|(((int)((1-hh)*g+hh*shade*green))<<8)
						|((int)((1-hh)*g+hh*shade*blue)));
				}
				//pb=RGB(0,0,0);
				//pw=RGB(255,255,255);
			}
			else 
			{
				pb=pw=RGB(255,255,255);				
			}
					
			SetPixel(hBlcDC,j,i,pb);
			SetPixel(hWhtDC,j,i,pw);			
			
			SetPixel(hCurBlcDC,j,i,pb);
			SetPixel(hCurWhtDC,j,i,pw);			
		}
	}
	HPEN hPen=CreatePen(PS_SOLID,1,RGB(255,0,0));

	HPEN hOldPen=(HPEN)SelectObject(hCurBlcDC,hPen);
	MoveToEx(hCurBlcDC,d/2,d/4,NULL);LineTo(hCurBlcDC,d/2,d*3/4);
	MoveToEx(hCurBlcDC,d/4,d/2,NULL);LineTo(hCurBlcDC,d*3/4,d/2);	
	SelectObject(hCurBlcDC,hOldPen);	
	
	hOldPen=(HPEN)SelectObject(hCurWhtDC,hPen);
	MoveToEx(hCurWhtDC,d/2,d/4,NULL);LineTo(hCurWhtDC,d/2,d*3/4);
	MoveToEx(hCurWhtDC,d/4,d/2,NULL);LineTo(hCurWhtDC,d*3/4,d/2);	
	SelectObject(hCurWhtDC,hOldPen);
	DeleteObject(hPen);

	return true;
}

//����������Ϣ
BOOL CConSix::ProcessMove(char *moveCmd)
{	
	Step tStep;
	char *res;
	int pos=0;
	int len=strlen("move ");
	if ((res = strstr(moveCmd, "move")) == NULL)//Ѱ��move�ؼ���
	{
		return 0;//δ�ҵ��ؼ���
	}
	else
	{		
		pos = (res - moveCmd);
		pos+=len;
		
		tStep.first.x = moveCmd[pos] - 'A';
		tStep.first.y = moveCmd[pos + 1] - 'A';
		tStep.second.x = moveCmd[pos + 2] - 'A';
		tStep.second.y = moveCmd[pos + 3] - 'A';
		tStep.side = player;
		stepStack.push(tStep);

		if(!FitRules())//�ж��Ƿ���Ϲ���
		{
			sprintf(curCmd,"error\n");
			sprintf(denCmd,"\0");
			stepStack.pop();
			return -1;//����Υ��
		}
		board[tStep.first.x][tStep.first.y] = tStep.side;
		if(first_hand==true)
		{
			first_hand=false;
		}
		else
		{
			if (tStep.second.x != -1 && tStep.second.y != -1)
				board[tStep.second.x][tStep.second.y] = tStep.side;
		}
		InvalidateRect(hWnd,&rtBoard,FALSE);	
		SendMessage(hWnd,WM_PAINT,NULL,NULL);
//		PlaySnd(0);
		moveCmd[pos + 4] = '\0';
		ShowStepHis(moveCmd + pos);

		sprintf(denCmd, "move %c%c%c%c\n", tStep.first.x + 'A', tStep.first.y + 'A', tStep.second.x + 'A', tStep.second.y + 'A');//����д��Ϣ
		sprintf(curCmd,"\0");
	}		
	if(WinOrLose())//�ж�ʤ��
	{
		sprintf(denCmd+strlen(denCmd),"end\n");
		sprintf(curCmd,"end\n");
		return 2;//�ֳ�ʤ��
	}	
	StepNum[player]++;//�ۼƲ���
	player=NEXTPLAYER(player);
	return 1;//��ȡ�ɹ�
}

bool CConSix::PlaySnd(int sel)
{
	char filename[MAX_PATH]={0};
	strcpy(filename,gameset->CurDir);
	switch(sel)
	{
	case 0:
		strcat(filename,"\\wav\\ConSix\\����.wav");
		break;
	default:
		break;
	}	
	if(!PlaySound(filename,NULL,SND_ASYNC|SND_NOWAIT |SND_FILENAME))
	{
		ErrorBox("PlaySound failed");
		return false;
	}
	return true;
}

VOID CConSix::ShowStepHis(char *msg)
{
	char step[1000];	
	memset(step,0,sizeof(step));
	if(player==BLACK)
	{
		strcpy(step,"Black: ");
	}
	else if(player==WHITE)
	{
		strcpy(step,"White: ");
	}	
	strcat(step,msg);
	SendMessage(hWnd,GM_SHOWSTEP,(WPARAM)step,(LPARAM)player);
	return;
}

VOID CConSix::InitGame()//��Ϸ��ʼ��
{
	memset(StepNum,0,sizeof(StepNum));

	player=BLACK;	
	first_hand=true;
	count=0;
	InitBoard();	//��ʼ������

	CleanStack(stepStack);
	return;
}

VOID CConSix::InitBoard()
{
	int i,j;
	for(i=0;i<19;i++)
	{
		for(j=0;j<19;j++)
		{
			board[i][j]=EMPTY;
		}
	}

	InvalidateRect(hWnd,&rtBoard,FALSE);//ˢ������
	return;
}

/**
 * OnLButtonDown - ��Ӧ��������������ŷ�
 * @x:	ָ�������
 * @y:	ָ��������
 * return:	�����ŷ�����״̬��-1��ʾ�������0��ʾ��������У�1��ʾ�������
 */
BOOL CConSix::OnLButtonDown(int x,int y)
{
	if(!InsideRect(&rtBoard,x,y)||count==-1)//ָ�������̷�Χ�������������״̬
		return 0;		

	x=(int)((x-rtBoard.left-side/40)*20/side);//����������ת������������
	if(gameset->BasePt==UPLEFT)
	{			
		y=(int)((y-rtBoard.top-side/40)*20/side);
	}	
	else if(gameset->BasePt==DOWNLEFT)
	{		
		y=18-(int)((y-rtBoard.top-side/40)*20/side);
	}
	if (x>=0&&x<19&&y>=0&&y<19)
		return SToS(x,y);
	return 0;
}

BOOL CConSix::SToS(int x,int y)
{
	Step tStep;

	//����ŷ��Ϸ���
	if(board[x][y]!=EMPTY)
		return -1;//�ŷ��Ƿ�
	board[x][y]=player;	

	if(count==0)//�����һ��
	{
		InitStep(tStep);
		tStep.first.x = x;
		tStep.first.y = y;
		tStep.side = player;
		stepStack.push(tStep);
		if(first_hand)//���ǵ�һ��
		{
			count=-1;
			InvalidateRect(hWnd,&rtBoard,FALSE);
			SendMessage(hWnd,WM_PAINT,NULL,NULL);
			PlaySnd(0);	
			return 1;
		}
		else
		{
			count = 1;
			InvalidateRect(hWnd, &rtBoard, FALSE);
			SendMessage(hWnd, WM_PAINT, NULL, NULL);
			PlaySnd(0);
			if (WinOrLose())//ʶ����һ�ӻ�ʤ�ŷ�
			{
				count = -1;
				return 1;
			}
			return 0;//�������Ӳ���
		}
	}
	else//����ڶ���
	{
		tStep = stepStack.top();
		stepStack.pop();
		tStep.second.x = x;
		tStep.second.y = y;
		stepStack.push(tStep);
		count = -1;
		InvalidateRect(hWnd, &rtBoard, FALSE);
		SendMessage(hWnd, WM_PAINT, NULL, NULL);
		PlaySnd(0);
		return 1;
	}
}

INT CConSix::OkMove()
{
	Step tStep;
	tStep = stepStack.top();
	sprintf(denCmd, "move %c%c%c%c", tStep.first.x + 'A', tStep.first.y + 'A', tStep.second.x + 'A', tStep.second.y + 'A');
	ShowStepHis(denCmd + 5);
	StepNum[player]++;//�ۼƲ���
	player = NEXTPLAYER(player);
	if (WinOrLose())
	{
		sprintf(denCmd + strlen(denCmd), "\nend\n");
		return 2;
	}
	first_hand = false;//ȡ����һ�ֱ�־
	count = 0;
	return 0;
}

VOID CConSix::CancelMove()
{
	Step tStep;
	if (count == 0)
		return;
	tStep = stepStack.top();
	stepStack.pop();
	IsChess(tStep.second)
	{
		board[tStep.second.x][tStep.second.y] = EMPTY;
	}
	IsChess(tStep.first)
	{
		board[tStep.first.x][tStep.first.y] = EMPTY;
	}
	InvalidateRect(hWnd, &rtBoard, FALSE);
	SendMessage(hWnd, WM_PAINT, NULL, NULL);
	count = 0;
	return;
}

bool CConSix::WinOrLose()//�ж�ʤ��
{	
	bool win=false;
	int i,j;
	Step tStep = stepStack.top();
	BYTE side=tStep.side;

	if(win==false)
	{
		for(i=0;i<19;i++) //ɨ��������û������
		{
			for(j=0;j<14;j++)
			{
				if(board[i][j]==side &&board[i][j+1]==side &&board[i][j+2]==side &&board[i][j+3]==side &&board[i][j+4]==side &&board[i][j+5]==side )
				  win=true;		  
			}
		}	  
	}	
	if(win==false)	
	{
		for(i=0;i<19;i++) //ɨ�������û������
		{
			for(j=0;j<14;j++)
			{
				if(board[j][i]==side &&board[j+1][i]==side &&board[j+2][i]==side &&board[j+3][i]==side &&board[j+4][i]==side &&board[j+5][i]==side )
				  win=true;        
			}
		}	  
	}
	if(win==false)	
	{
		for(i=0;i<14;i++) //ɨ��45��б��
		{
			for(j=5;j<19;j++)
			{
				if(board[i][j]==side &&board[i+1][j-1]==side &&board[i+2][j-2]==side &&board[i+3][j-3]==side &&board[i+4][j-4]==side &&board[i+5][j-5]==side )
				  win=true; 
			}
		}	   
	}	
	if(win==false)	
	{
		for(i=0;i<14;i++) //ɨ��135��б��
		{
			for(j=13;j>-1;j--)
			{
				if(board[i][j]==side &&board[i+1][j+1]==side &&board[i+2][j+2]==side &&board[i+3][j+3]==side &&board[i+4][j+4]==side &&board[i+5][j+5]==side )
				  win=true;  
			}
		}
	}		   

	if(win==true)
	{
		SendMessage(hWnd,GM_WINLOSE,(WPARAM)(StepNum[BLACK]<<16)+StepNum[WHITE],(LPARAM)side);
	}	
	return win;
}

bool CConSix::FitRules()//�Ƿ���Ϲ���
{
	Step sp = stepStack.top();
	if(sp.first.x<0||sp.first.x>18||sp.first.y<0||sp.first.y>18)
	{
		return false;
	}
	if(first_hand==true)
	{
		if(board[sp.first.x][sp.first.y]!=EMPTY)
		{
			return false;
		}	
	}
	else
	{
		if(sp.second.x==-1&&sp.second.y==-1)
		{
			if(board[sp.first.x][sp.first.y]!=EMPTY)
				return false;
			else
				return true;
		}
		if(sp.second.x<0||sp.second.x>18||sp.second.y<0||sp.second.y>18)
		{
			return false;
		}
		if(board[sp.first.x][sp.first.y]!=EMPTY||board[sp.second.x][sp.second.y]!=EMPTY)
		{
			return false;
		}
	}
	return true;
}

