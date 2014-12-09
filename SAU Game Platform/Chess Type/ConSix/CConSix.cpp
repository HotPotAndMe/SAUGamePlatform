#pragma comment(lib,"winmm.lib")

#include "CConSix.h"
#include <stdio.h>


const int lineVector[8][2] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 } };//��������


VOID __cdecl ErrorBox(LPTSTR ErrorInfo)//������ʾ��
{
	CHAR error1[50], error2[20];
	strcpy(error1, ErrorInfo);
	sprintf(error2, "\n\nerror: %d", GetLastError());
	strcat(error1, error2);
	MessageBox(NULL, error1, "error", MB_OK);
}

CConSix::CConSix(HINSTANCE hInst, HWND hWnd, char *LibPath)
{
	this->hInst = hInst;
	this->hWnd = hWnd;
	strncpy(this->LibPath, LibPath, MAX_PATH - 1);

	BkColor = RGB(0, 0, 0);
	BoardColor = RGB(255, 255, 0);
	hPen = NULL;
	hFont = NULL;
	InitGame();
	count = -1;

	HDC hDC = GetDC(hWnd);
	hBlcDC = CreateCompatibleDC(hDC);
	hWhtDC = CreateCompatibleDC(hDC);
	hMarkDC = CreateCompatibleDC(hDC);
}

CConSix::~CConSix()
{
	if (hPen != NULL)
		DeleteObject(hPen);
	if (hFont != NULL)
		DeleteObject(hFont);
	DeleteDC(hBlcDC);
	DeleteDC(hWhtDC);
	DeleteDC(hMarkDC);
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
	DrawChess();
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
	for (i = 1; i <= 19; i++)
	{
		letter[0] = 'A' + i - 1;
		itoa(i - 1, number, 10);

		TextOut(hDC, rtBoard.left + side*i / 20 - fWidth / 2, rtBoard.top + side / 40 - fHeight / 2, letter, 1);//��������				
		TextOut(hDC, rtBoard.left + side*i / 20 - fWidth, rtBoard.top + side * 39 / 40 - fHeight / 2, number, 2);

		MoveToEx(hDC, rtBoard.left + side / 20, rtBoard.top + side*i / 20, NULL);//������
		LineTo(hDC, rtBoard.left + side * 19 / 20, rtBoard.top + side*i / 20);
	}
	for (i = 1; i <= 19; i++)
	{
		letter[0] = 'A' + i - 1;
		itoa(i - 1, number, 10);

		TextOut(hDC, rtBoard.left + fWidth, rtBoard.top + side*i / 20 - fHeight / 2, letter, 1);
		TextOut(hDC, rtBoard.left + side * 19 / 20 + fWidth, rtBoard.top + side*i / 20 - fHeight / 2, number, 2);

		MoveToEx(hDC, rtBoard.left + side*i / 20, rtBoard.top + side / 20, NULL);
		LineTo(hDC, rtBoard.left + side*i / 20, rtBoard.top + side * 19 / 20);
	}

	for (i = 0; i < 19; i++)
	{
		for (j = 0; j < 19; j++)
		{
			if (board[i][j] == BLACK)
			{
				BitBlt(hDC, rtBoard.left + side*(i*2 + 1) / 40, rtBoard.top + side*(j*2 + 1) / 40, d, d, hBlcDC, d, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i*2 + 1) / 40, rtBoard.top + side*(j*2 + 1) / 40, d, d, hBlcDC, 0, 0, SRCPAINT);
			}
			else if (board[i][j] == WHITE)
			{
				BitBlt(hDC, rtBoard.left + side*(i*2 + 1) / 40, rtBoard.top + side*(j*2 + 1) / 40, d, d, hWhtDC, d, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i*2 + 1) / 40, rtBoard.top + side*(j*2 + 1) / 40, d, d, hWhtDC, 0, 0, SRCPAINT);
			}
		}
	}

	if (!stepStack.empty())
	{
		Step curStep = stepStack.top();
		int x, y;
		IsChess(curStep.first)
		{
			x = curStep.first.x;
			y = curStep.first.y;
			BitBlt(hDC, rtBoard.left + side*(x*2 + 1) / 40, rtBoard.top + side*(y*2 + 1) / 40, d, d, hMarkDC, d, 0, SRCAND);
			BitBlt(hDC, rtBoard.left + side*(x*2 + 1) / 40, rtBoard.top + side*(y*2 + 1) / 40, d, d, hMarkDC, 0, 0, SRCPAINT);
		}
		IsChess(curStep.second)
		{
			x = curStep.second.x;
			y = curStep.second.y;
			BitBlt(hDC, rtBoard.left + side*(x*2 + 1) / 40, rtBoard.top + side*(y*2 + 1) / 40, d, d, hMarkDC, d, 0, SRCAND);
			BitBlt(hDC, rtBoard.left + side*(x*2 + 1) / 40, rtBoard.top + side*(y*2 + 1) / 40, d, d, hMarkDC, 0, 0, SRCPAINT);
		}
	}

}


bool CConSix::DrawChess()
{
	char filename[MAX_PATH] = { 0 };
	HBITMAP hBlcBmp, hWhtBmp, hMarkBmp;

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\black.bmp");
	hBlcBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, d * 2, d, LR_LOADFROMFILE);

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\white.bmp");
	hWhtBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, d * 2, d, LR_LOADFROMFILE);

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\mark.bmp");
	hMarkBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, d * 2, d, LR_LOADFROMFILE);

	SelectObject(hBlcDC, hBlcBmp);
	SelectObject(hWhtDC, hWhtBmp);
	SelectObject(hMarkDC, hMarkBmp);

	DeleteObject(hBlcBmp);
	DeleteObject(hWhtBmp);
	DeleteObject(hMarkBmp);

	return true;
}

//����������Ϣ
BOOL CConSix::ProcessMove(char *moveCmd)
{
	Step tStep;
	char *res;
	int pos = 0;
	int len = strlen("move ");
	if ((res = strstr(moveCmd, "move")) == NULL)//Ѱ��move�ؼ���
	{
		return 0;//δ�ҵ��ؼ���
	}
	else
	{
		pos = (res - moveCmd);
		pos += len;

		tStep.first.x = moveCmd[pos] - 'A';
		tStep.first.y = moveCmd[pos + 1] - 'A';
		tStep.second.x = moveCmd[pos + 2] - 'A';
		tStep.second.y = moveCmd[pos + 3] - 'A';
		tStep.side = player;
		stepStack.push(tStep);

		if (!FitRules())//�ж��Ƿ���Ϲ���
		{
			sprintf(curCmd, "error\n");
			sprintf(denCmd, "\0");
			stepStack.pop();
			return -1;//����Υ��
		}
		board[tStep.first.x][tStep.first.y] = tStep.side;
		if (first_hand == true)
		{
			first_hand = false;
		}
		else
		{
			if (tStep.second.x != -1 && tStep.second.y != -1)
				board[tStep.second.x][tStep.second.y] = tStep.side;
		}
		InvalidateRect(hWnd, &rtBoard, FALSE);
		SendMessage(hWnd, WM_PAINT, NULL, NULL);
		//		PlaySnd(0);
		moveCmd[pos + 4] = '\0';
		ShowStepHis(moveCmd + pos);

		sprintf(denCmd, "move %c%c%c%c\n", tStep.first.x + 'A', tStep.first.y + 'A', tStep.second.x + 'A', tStep.second.y + 'A');//����д��Ϣ
		sprintf(curCmd, "\0");
	}
	if (WinOrLose())//�ж�ʤ��
	{
		sprintf(denCmd + strlen(denCmd), "end\n");
		sprintf(curCmd, "end\n");
		return 2;//�ֳ�ʤ��
	}
	StepNum[player]++;//�ۼƲ���
	player = NEXTPLAYER(player);
	return 1;//��ȡ�ɹ�
}

bool CConSix::PlaySnd(int sel)
{
	char filename[MAX_PATH] = { 0 };
	switch (sel)
	{
	case 0:
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\����.wav");
		break;
	default:
		break;
	}
	if (!PlaySound(filename, NULL, SND_ASYNC | SND_NOWAIT | SND_FILENAME))
	{
		ErrorBox("PlaySound failed");
		return false;
	}
	return true;
}

VOID CConSix::ShowStepHis(char *msg)
{
	char step[1000];
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

VOID CConSix::InitGame()//��Ϸ��ʼ��
{
	memset(StepNum, 0, sizeof(StepNum));

	player = BLACK;
	first_hand = true;
	InitBoard();	//��ʼ������
	count = 0;

	CleanStack(stepStack);
	return;
}

VOID CConSix::InitBoard()
{
	int i, j;
	for (i = 0; i < 19; i++)
	{
		for (j = 0; j < 19; j++)
		{
			board[i][j] = EMPTY;
		}
	}

	InvalidateRect(hWnd, &rtBoard, FALSE);//ˢ������
	return;
}

/**
 * OnLButtonDown - ��Ӧ��������������ŷ�
 * @x:	ָ�������
 * @y:	ָ��������
 * return:	�����ŷ�����״̬��-1��ʾ�������0��ʾ��������У�1��ʾ�������
 */
BOOL CConSix::OnLButtonDown(int x, int y)
{
	Point point;
	if (!InsideRect(&rtBoard, x, y))
		return 2;
	if (count == -1)//ָ�������̷�Χ�������������״̬
		return 0;
	point.x = ((x - rtBoard.left) * 20 - side / 2) / side;//����������ת������������
	point.y = ((y - rtBoard.top) * 20 - side / 2) / side;
	if (point.x < 0 || point.x >= 19 || point.y < 0 || point.y >= 19)
		return 2;
	return SToS(point);
}

BOOL CConSix::SToS(Point point)
{
	Step tStep;

	//����ŷ��Ϸ���
	if (board[point.x][point.y] != EMPTY)
		return -1;//�ŷ��Ƿ�
	board[point.x][point.y] = player;

	if (count == 0)//�����һ��
	{
		InitStep(tStep);
		tStep.first = point;
		tStep.side = player;
		stepStack.push(tStep);
		if (first_hand)//���ǵ�һ��
		{
			count = -1;
			InvalidateRect(hWnd, &rtBoard, FALSE);
			SendMessage(hWnd, WM_PAINT, NULL, NULL);
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
		tStep.second = point;
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
	sprintf(denCmd + strlen(denCmd), "\n");
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
	bool win = false;
	int i, j, tx, ty, connect;
	Step tStep = stepStack.top();
	Point point[2] = { tStep.first, tStep.second };
	BYTE side = tStep.side;

	for (j = 0; j < 2; j++)
	{
		if (point[j].x == -1 || point[j].y == -1)
			break;
		for (i = 0; i < 4; i++)
		{
			connect = 0;
			//�������������ƶ�
			tx = point[j].x - lineVector[i][0];
			ty = point[j].y - lineVector[i][1];
			while (tx >= 0 && tx < 19 && ty >= 0 && ty < 19 && board[tx][ty] == side)
			{
				tx -= lineVector[i][0];
				ty -= lineVector[i][1];
			}
			//��������
			tx += lineVector[i][0];
			ty += lineVector[i][1];
			while (tx >= 0 && tx < 19 && ty >= 0 && ty < 19 && board[tx][ty] == side)
			{
				connect++;//����
				tx += lineVector[i][0];
				ty += lineVector[i][1];
			}
			if (connect >= 6)	//����
			{
				win = true;
				break;
			}
		}
	}
	if (win == true && count != 1)//countΪ-1ʱ��ʾ�����ִ���ȡ�ŷ���countΪ0ʱ��ʾ�����洦��ȡ�ŷ���countΪ1ʱ��ʾģ����һ�ӻ�ʤ
	{
		PostMessage(hWnd, GM_WINLOSE, (WPARAM)(StepNum[BLACK] << 16) + StepNum[WHITE], (LPARAM)side);
	}
	return win;
}

bool CConSix::FitRules()//�Ƿ���Ϲ���
{
	Step sp = stepStack.top();
	if (sp.first.x < 0 || sp.first.x>18 || sp.first.y < 0 || sp.first.y>18)
	{
		return false;
	}
	if (first_hand == true)
	{
		if (board[sp.first.x][sp.first.y] != EMPTY)
		{
			return false;
		}
	}
	else
	{
		if (sp.second.x == -1 && sp.second.y == -1)
		{
			if (board[sp.first.x][sp.first.y] != EMPTY)
				return false;
			else
				return true;
		}
		if (sp.second.x < 0 || sp.second.x>18 || sp.second.y < 0 || sp.second.y>18)
		{
			return false;
		}
		if (board[sp.first.x][sp.first.y] != EMPTY || board[sp.second.x][sp.second.y] != EMPTY)
		{
			return false;
		}
	}
	return true;
}

