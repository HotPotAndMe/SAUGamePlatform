#pragma comment(lib,"winmm.lib")

#include "CAmazon.h"
#include <stdio.h>

extern HINSTANCE hInst;

VOID __cdecl ErrorBox(LPTSTR ErrorInfo)//������ʾ��
{
	CHAR error1[50],error2[20];
	strcpy(error1,ErrorInfo);
	sprintf(error2,"\n\nerror: %d",GetLastError());	
	strcat(error1,error2);	
	MessageBox(NULL,error1,"error",MB_OK);
}

CAmazon::CAmazon()
{
	BkColor=RGB(0,0,0);
	BoardColor=RGB(128,128,128);
	hPen = NULL;
	hFont = NULL;
	InitGame();
	count = -1;

	HDC hDC = GetDC(hWnd);
	hBlcDC = CreateCompatibleDC(hDC);
	hWhtDC = CreateCompatibleDC(hDC);
	hBarDC = CreateCompatibleDC(hDC);
	hMarkDC = CreateCompatibleDC(hDC);
}

CAmazon::~CAmazon()
{
	if (hPen != NULL)
		DeleteObject(hPen);
	if (hFont != NULL)
		DeleteObject(hFont);
	DeleteDC(hBlcDC);
	DeleteDC(hWhtDC);
	DeleteDC(hBarDC);
	DeleteDC(hMarkDC);
}

VOID CAmazon::SetBoard(RECT rtBoard)
{
	if (hPen != NULL)
		DeleteObject(hPen);
	if (hFont != NULL)
		DeleteObject(hFont);
	this->rtBoard = rtBoard;
	side = rtBoard.right - rtBoard.left + 1;
	d = side / 12;
	pixel = side / 300;
	if (pixel == 0)
		pixel = 1;
	hPen = CreatePen(PS_SOLID, pixel, RGB(0, 0, 0));
	fWidth = d / 3;
	fHeight = d * 2 / 3;
	hFont = CreateSimpleFont(fWidth, fHeight);
	cHeight = d; cWidth = cHeight * 2 / 3;
	offset = (d - cWidth + pixel) / 2;
	DrawChess();
}

VOID CAmazon::DrawBoard(HDC hDC)
{
	int i, j;
	HPEN hOldPen;
	HFONT hOldFont;
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hOldFont = (HFONT)SelectObject(hDC, hFont);

	::SetTextColor(hDC, RGB(0, 0, 0));

	char letter[2], number[2];
	memset(letter, 0, sizeof(letter));
	memset(number, 0, sizeof(number));
	for (i = 0; i < 11; i++)
	{
		letter[0] = 'A' + i;
		number[0] = '0' + i;

		if (i < 10)
		{
			TextOut(hDC, (int)(rtBoard.left + side*(i + 1) / 12 + d / 2 - fWidth / 2), rtBoard.top, letter, 1);
			TextOut(hDC, (int)(rtBoard.left + side*(i + 1) / 12 + d / 2 - fWidth / 2), rtBoard.bottom - fHeight, number, 1);
		}

		MoveToEx(hDC, rtBoard.left + d, rtBoard.top + d*(i + 1), NULL);
		LineTo(hDC, rtBoard.left + 11 * d, rtBoard.top + d*(i + 1));
	}
	for (i = 0; i < 11; i++)
	{
		letter[0] = 'A' + i;
		number[0] = '0' + i;

		if (i < 10)
		{
			TextOut(hDC, rtBoard.left, (int)(rtBoard.top + side*(i + 1) / 12 + d / 2 - fHeight / 2), letter, 1);
			TextOut(hDC, rtBoard.right - fWidth, (int)(rtBoard.top + side*(i + 1) / 12 + d / 2 - fHeight / 2), number, 1);
		}

		MoveToEx(hDC, rtBoard.left + d*(i + 1), rtBoard.top + d, NULL);
		LineTo(hDC, rtBoard.left + d*(i + 1), rtBoard.top + 11 * d);
	}

	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (board[i][j] == BLACK)
			{
				BitBlt(hDC, rtBoard.left + d*(i + 1) + offset, rtBoard.top + d*(j + 1), cWidth, cHeight, hBlcDC, cWidth, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + d*(i + 1) + offset, rtBoard.top + d*(j + 1), cWidth, cHeight, hBlcDC, 0, 0, SRCPAINT);
			}
			else if (board[i][j] == WHITE)
			{
				BitBlt(hDC, rtBoard.left + d*(i + 1) + offset, rtBoard.top + d*(j + 1), cWidth, cHeight, hWhtDC, cWidth, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + d*(i + 1) + offset, rtBoard.top + d*(j + 1), cWidth, cHeight, hWhtDC, 0, 0, SRCPAINT);
			}
			else if (board[i][j] == BAR)
			{
				BitBlt(hDC, rtBoard.left + d*(i + 1), rtBoard.top + d*(j + 1), d, d, hBarDC, d, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + d*(i + 1), rtBoard.top + d*(j + 1), d, d, hBarDC, 0, 0, SRCPAINT);
			}
		}
	}

	if (!stepStack.empty())
	{
		Step curStep = stepStack.top();
		IsChess(curStep.second)
		{
			int x = curStep.second.x; int y = curStep.second.y;
			BitBlt(hDC, rtBoard.left + d*(x + 1), rtBoard.top + d*(y + 1), d, d, hMarkDC, d, 0, SRCAND);
			BitBlt(hDC, rtBoard.left + d*(x + 1), rtBoard.top + d*(y + 1), d, d, hMarkDC, 0, 0, SRCPAINT);
		}
		else IsChess(curStep.first)
		{
			int x = curStep.first.x; int y = curStep.first.y;
			BitBlt(hDC, rtBoard.left + d*(x + 1), rtBoard.top + d*(y + 1), d, d, hMarkDC, d, 0, SRCAND);
			BitBlt(hDC, rtBoard.left + d*(x + 1), rtBoard.top + d*(y + 1), d, d, hMarkDC, 0, 0, SRCPAINT);
		}
	}

}

bool CAmazon::DrawChess()
{	
	char filename[MAX_PATH]={0};
	HBITMAP hBoardBmp,hBlcBmp,hWhtBmp,hBarBmp,hMarkBmp;			
	
	strcpy(filename, LibPath);
	strcat(filename,"\\bmp\\black.bmp");
	hBlcBmp=(HBITMAP)LoadImage(hInst,filename,IMAGE_BITMAP,cWidth*2,cHeight,LR_LOADFROMFILE);

	strcpy(filename, LibPath);
	strcat(filename,"\\bmp\\white.bmp");
	hWhtBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, cWidth * 2, cHeight, LR_LOADFROMFILE);

	strcpy(filename, LibPath);
	strcat(filename,"\\bmp\\bar.bmp");
	hBarBmp=(HBITMAP)LoadImage(hInst,filename,IMAGE_BITMAP,d*2,d,LR_LOADFROMFILE);

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\mark.bmp");
	hMarkBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, d*2, d, LR_LOADFROMFILE);

	SelectObject(hBlcDC,hBlcBmp);
	SelectObject(hWhtDC,hWhtBmp);
	SelectObject(hBarDC,hBarBmp);
	SelectObject(hMarkDC, hMarkBmp);

	DeleteObject(hBlcBmp);
	DeleteObject(hWhtBmp);
	DeleteObject(hBarBmp);
	DeleteObject(hMarkBmp);
	return true;
}

BOOL CAmazon::ProcessMove(char *moveCmd)
{			
	Step tStep;
	char *res;
	int pos=0;
	int len=strlen("move ");
	if ((res = strstr(moveCmd, "move")) == NULL)//Ѱ��move�ؼ���
	{
		return 0;
	}
	else
	{		
		pos = (res - moveCmd);
		pos+=len;
		
		tStep.first.x = moveCmd[pos] - 'A';
		tStep.first.y = moveCmd[pos + 1] - 'A';
		tStep.second.x = moveCmd[pos + 2] - 'A';
		tStep.second.y = moveCmd[pos + 3] - 'A';
		tStep.third.x = moveCmd[pos + 4] - 'A';
		tStep.third.y = moveCmd[pos + 5] - 'A';
		tStep.side = player;
		stepStack.push(tStep);


		if(!FitRules())//�ж��Ƿ���Ϲ���
		{
			return -1;
		}			
		board[tStep.first.x][tStep.first.y] = EMPTY;
		board[tStep.second.x][tStep.second.y] = player;
		board[tStep.third.x][tStep.third.y] = BAR;
		InvalidateRect(hWnd,&rtBoard,FALSE);
		PlaySnd(2);
		moveCmd[pos + 6] = '\0';
		ShowStepHis(moveCmd + pos);

		sprintf(denCmd, "move %c%c%c%c%c%c\n", tStep.first.x + 'A', tStep.first.y + 'A', tStep.second.x + 'A', tStep.second.y + 'A', tStep.third.x + 'A', tStep.third.y + 'A');//����д��Ϣ
		sprintf(curCmd, "\0");
	}
	if(WinOrLose())//�ж�ʤ��
	{				
		sprintf(denCmd + strlen(denCmd), "end\n");
		sprintf(curCmd, "end\n");
		return 2;
	}
	StepNum[player]++;
	player = NEXTPLAYER(player);
	return 1;
}

bool CAmazon::PlaySnd(int sel)//������Ч
{
	char filename[MAX_PATH] = { 0 };
	switch (sel)
	{
	case 0:
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\ѡ��.wav");
		break;
	case 1:
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\����.wav");
		break;
	case 2:
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\�����ϰ�.wav");
		break;
	default:
		break;
	}
	if (!PlaySound(filename, NULL, SND_NOWAIT | SND_FILENAME))
	{
		ErrorBox("PlaySound failed");
		return false;
	}
	return true;
}

VOID CAmazon::ShowStepHis(char *msg)
{
	char step[50];
	memset(step, 0, sizeof(step));
	if (player == BLACK)
	{
		strcpy(step, "Black: ");
	}
	else if (player == WHITE)
	{
		strcpy(step, "White: ");
	}
	strcat(step, msg);
	SendMessage(hWnd, GM_SHOWSTEP, (WPARAM)step, (LPARAM)player);
	return;
}

VOID CAmazon::InitGame()
{	
	memset(StepNum,0,sizeof(StepNum));

	player=BLACK;//�ڷ�����	
	InitBoard();	
	count=0;

	CleanStack(stepStack);
	return;
}

VOID CAmazon::InitBoard()
{
	int i,j;
	for(i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			board[i][j]=EMPTY;
		}
	}
	board[0][3]=WHITE;board[0][6]=BLACK;
	board[3][0]=WHITE;board[3][9]=BLACK;
	board[6][0]=WHITE;board[6][9]=BLACK;
	board[9][3]=WHITE;board[9][6]=BLACK;

	InvalidateRect(hWnd,&rtBoard,FALSE);
	return;
}

BOOL CAmazon::OnLButtonDown(int x,int y)
{
	Point point;
	if(!InsideRect(&rtBoard,x,y)||count==-1)
		return 0;			
	point.x=(int)((x-rtBoard.left)*12/side)-1;	
	point.y=(int)((y-rtBoard.top)*12/side)-1;
	if (point.x < 0 || point.x >= 10 || point.y < 0 || point.y >= 10)
		return 2;
	return SToS(point);
}

BOOL CAmazon::SToS(Point point)
{
	Step tStep;

	if (count == 0)
	{	
		if(board[point.x][point.y]!=player)//ѡ�б�������
			return -1;
		InitStep(tStep);
		tStep.first = point;
		tStep.side = player;
		stepStack.push(tStep);
		count = 1;
		::InvalidateRect(hWnd, &rtBoard, FALSE);
		PlaySnd(0);	
		return 0;
	}
	else if(count==1)
	{		
		tStep = stepStack.top();
		if (board[point.x][point.y] == player)//����ѡ������
		{	
			stepStack.pop();
			tStep.first = point;
			stepStack.push(tStep);
			::InvalidateRect(hWnd, &rtBoard, FALSE);
			PlaySnd(0);
			return 0;
		}
		else if(board[point.x][point.y]==NEXTPLAYER(player)||!JudgeRule(tStep.first,point))//�ŷ�����
		{
			return -1;
		}
		stepStack.pop();
		board[tStep.first.x][tStep.first.y] = EMPTY;
		board[point.x][point.y]=player;
		tStep.second = point;
		stepStack.push(tStep);
		count = 2;
		::InvalidateRect(hWnd,&rtBoard,FALSE);
		PlaySnd(1);
		return 0;
	}
	else if(count==2)
	{	
		tStep = stepStack.top();
		if(board[point.x][point.y]!=EMPTY||!JudgeRule(tStep.second,point))
			return -1;
		stepStack.pop();
		board[point.x][point.y] = BAR;
		tStep.third = point;
		stepStack.push(tStep);
		count = -1;
		::InvalidateRect(hWnd, &rtBoard, FALSE);
		PlaySnd(2);
		return 1;
	}
	return 0;
}

INT CAmazon::OkMove()
{
	Step tStep;
	tStep = stepStack.top();
	sprintf(denCmd, "move %c%c%c%c%c%c", tStep.first.x + 'A', tStep.first.y + 'A', tStep.second.x + 'A', tStep.second.y + 'A', tStep.third.x + 'A', tStep.third.y + 'A');
	ShowStepHis(denCmd + 5);
	StepNum[player]++;//�ۼƲ���
	player = NEXTPLAYER(player);
	if (WinOrLose())
	{
		sprintf(denCmd + strlen(denCmd), "\nend\n");
		return 2;
	}
	sprintf(denCmd + strlen(denCmd), "\n");
	count = 0;
	return 0;
}

VOID CAmazon::CancelMove()
{
	Step tStep;
	if (count == 0)
		return;
	tStep = stepStack.top();
	stepStack.pop();
	IsChess(tStep.third)
	{
		board[tStep.third.x][tStep.third.y] = EMPTY;
	}
	IsChess(tStep.second)
	{
		board[tStep.second.x][tStep.second.y] = EMPTY;
	}
	IsChess(tStep.first)
	{
		board[tStep.first.x][tStep.first.y] = tStep.side;
	}
	InvalidateRect(hWnd, &rtBoard, FALSE);
	SendMessage(hWnd, WM_PAINT, NULL, NULL);
	count = 0;
	return;
}

bool CAmazon::WinOrLose()
{
	bool win = false;
	int blc, wht;
	blc = wht = 0;
	for (int i = 0; i<10; i++)
	{
		for (int j = 0; j<10; j++)
		{
			if (board[i][j] == BLACK)
			{
				if (
					((0<i &&j>0 && board[i - 1][j - 1] != EMPTY) || (i == 0 || j == 0))
					&& ((0<i &&board[i - 1][j] != EMPTY) || i == 0)
					&& ((0<i &&j<9 && board[i - 1][j + 1] != EMPTY) || (i == 0 || j == 9))
					&& ((j>0 && board[i][j - 1] != EMPTY) || j == 0)
					&& ((j<9 && board[i][j + 1] != EMPTY) || j == 9)
					&& ((i<9 && j>0 && board[i + 1][j - 1] != EMPTY) || (i == 9 || j == 0))
					&& ((i<9 && board[i + 1][j] != EMPTY) || (i == 9))
					&& ((i<9 && j<9 && board[i + 1][j + 1] != EMPTY) || (i == 9 || j == 9))
					)
				{
					blc++;//�ڷ����������޷����ӣ������
				}
			}
			if (board[i][j] == WHITE)
			{
				if (
					((0<i &&j>0 && board[i - 1][j - 1] != EMPTY) || (i == 0 || j == 0))
					&& ((0<i &&board[i - 1][j] != EMPTY) || i == 0)
					&& ((0<i &&j<9 && board[i - 1][j + 1] != EMPTY) || (i == 0 || j == 9))
					&& ((j>0 && board[i][j - 1] != EMPTY) || j == 0)
					&& ((j<9 && board[i][j + 1] != EMPTY) || j == 9)
					&& ((i<9 && j>0 && board[i + 1][j - 1] != EMPTY) || (i == 9 || j == 0))
					&& ((i<9 && board[i + 1][j] != EMPTY) || (i == 9))
					&& ((i<9 && j<9 && board[i + 1][j + 1] != EMPTY) || (i == 9 || j == 9))
					)
				{
					wht++;//�׷����������޷����ӣ������
				}
			}
		}
	}
	if (wht == 4)//�׷���·�������Ӯ
	{
		win = true;
	}
	if (blc == 4)//�ڷ���·�������Ӯ
	{
		win = true;
	}

	if (win == true)
	{
		SendMessage(hWnd, GM_WINLOSE, StepNum[BLACK], StepNum[WHITE]);
	}
	return win;
}

bool CAmazon::JudgeRule(Point src, Point des)
{
	int tx, ty;
	if (!(src.x == des.x || src.y == des.y || abs(des.x - src.x) == abs(des.y - src.y)))//���ӷ���
	{
		return false;
	}
	tx = src.x;
	ty = src.y;
	if (src.x == des.x)
	{
		if (des.y > src.y)
		{
			for (ty++; ty <= des.y; ty++)
				if (board[tx][ty] != EMPTY)
					return false;
		}
		else if (des.y<src.y)
		{
			for (ty--; ty >= des.y; ty--)
				if (board[tx][ty] != EMPTY)
					return false;
		}
	}
	else if (des.x > src.x)
	{
		if (src.y == des.y)
		{
			for (tx++; tx <= des.x; tx++)
				if (board[tx][ty] != EMPTY)
					return false;
		}
		else if (des.y > src.y)
		{
			for (tx++, ty++; tx <= des.x; tx++, ty++)
			{
				if (board[tx][ty] != EMPTY)
					return false;
			}
		}
		else
		{
			for (tx++, ty--; tx <= des.x; tx++, ty--)
			{
				if (board[tx][ty] != EMPTY)
					return false;
			}
		}
	}
	else 
	{
		if (des.y==src.y)
		{
			for (tx--; tx >= des.x; tx--)
				if (board[tx][ty] != EMPTY)
					return false;
		}
		else if (des.y > src.y)
		{
			for (tx--, ty++; tx >= des.x; tx--, ty++)
			{
				if (board[tx][ty] != EMPTY)
					return false;
			}
		}
		else
		{
			for (tx--, ty--; tx >= des.x; tx--, ty--)
			{
				if (board[tx][ty] != EMPTY)
					return false;
			}
		}
	}
	return true;
}

bool CAmazon::FitRules()
{
	Step tStep = stepStack.top();
	if (tStep.first.x < 0 || tStep.first.x>9 || tStep.first.y < 0 || tStep.first.y>9)
		return false;
	if (tStep.second.x < 0 || tStep.second.x>9 || tStep.second.y < 0 || tStep.second.y>9)
		return false;
	if (tStep.third.x < 0 || tStep.third.x>9 || tStep.third.y < 0 || tStep.third.y>9)
		return false;
	if (board[tStep.first.x][tStep.first.y] != tStep.side || board[tStep.second.x][tStep.second.y] != EMPTY || board[tStep.third.x][tStep.third.y] != EMPTY
		|| (tStep.second.x == tStep.third.x&&tStep.second.y == tStep.third.y))
		return false;
	if (!JudgeRule(tStep.first, tStep.second))
		return false;
	if (!JudgeRule(tStep.second, tStep.third))
		return false;
	return true;
}
