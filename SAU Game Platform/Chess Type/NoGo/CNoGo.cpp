#include "CNoGo.h"


const int lineVector[4][2] = { 0, 1, 0, -1, -1, 0, 1, 0 };


CNoGo::CNoGo(HINSTANCE hInst, HWND hWnd, char *LibPath)
{
	this->hInst = hInst;
	this->hWnd = hWnd;
	strncpy(this->LibPath, LibPath, MAX_PATH - 1);
	hPen = NULL;
	hFont = NULL;

	//��ȡ����
	GetConfig();

	//��������DC
	HDC hDC = GetDC(hWnd);
	hBlcDC = CreateCompatibleDC(hDC);
	hWhtDC = CreateCompatibleDC(hDC);
	hMarkDC = CreateCompatibleDC(hDC);

	InitGame();
}

CNoGo::~CNoGo()
{
	if (hPen != NULL)
		DeleteObject(hPen);
	if (hFont != NULL)
		DeleteObject(hFont);
	DeleteDC(hBlcDC);
	DeleteDC(hWhtDC);
	DeleteDC(hMarkDC);
}

/**
 * SetBoard - �������̲���
 * @rtBoard:	�����ڴ��ڿͻ�����λ��
 */
VOID CNoGo::SetBoard(RECT rtBoard)
{
	//�������̲���
	this->rtBoard = rtBoard;
	side = rtBoard.right - rtBoard.left;
	d = side / 10;

	//���ÿ��߻���
	if (hPen != NULL)
		DeleteObject(hPen);
	pWidth = (int)(side / 300);
	if (pWidth == 0)
		pWidth = 1;
	hPen = CreatePen(PS_SOLID, pWidth, RGB(0, 0, 0));

	//���ÿ̶�����
	if (hFont != NULL)
		DeleteObject(hFont);
	fWidth = (int)(d / 3);
	fHeight = (int)(d * 2 / 3);
	hFont = CreateSimpleFont(fWidth, fHeight);

	//��������Ԫ��
	DrawChess();
}

/**
 * DrawBoard - ��������
 * @hDC:	������DC
 */
VOID CNoGo::DrawBoard(HDC hDC)
{
	int i, j;
	HPEN hOldPen;
	HFONT hOldFont;

	//���ÿ��߻���
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	//���ÿ̶����塢��ɫ
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextColor(hDC, RGB(0, 0, 0));

	char letter[2] = { 0 }, number[3] = { 0 };

	//��������
	for (i = 1; i <= 9; i++)//9γ��
	{
		//���ƿ���
		MoveToEx(hDC, rtBoard.left + side / 10, rtBoard.top + side*i / 10, NULL);
		LineTo(hDC, rtBoard.left + side * 9 / 10, rtBoard.top + side*i / 10);
		//���ƿ̶�
		letter[0] = 'A' + i - 1;
		itoa(i, number, 10);
		TextOut(hDC, rtBoard.left + side / 20 - fWidth / 2, rtBoard.top + side*i / 10 - fHeight / 2, letter, 1);
		TextOut(hDC, rtBoard.left + side * 19 / 20 - fWidth / 2, rtBoard.top + side*i / 10 - fHeight / 2, number, 2);
	}
	for (i = 1; i <= 9; i++)//9����
	{
		//���ƿ���
		MoveToEx(hDC, rtBoard.left + side*i / 10, rtBoard.top + side / 10, NULL);
		LineTo(hDC, rtBoard.left + side*i / 10, rtBoard.top + side * 9 / 10);
		//���ƿ̶�
		letter[0] = 'A' + i - 1;
		itoa(i, number, 10);
		TextOut(hDC, rtBoard.left + side*i / 10 - fWidth / 2, rtBoard.top + side / 20 - fHeight / 2, letter, 1);			
		TextOut(hDC, rtBoard.left + side*i / 10 - fWidth / 2, rtBoard.top + side * 19 / 20 - fHeight / 2, number, 2);
	}

	//��������
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if (board[i][j] == BLACK)
			{//rtBoard.left+d*(i+1)-d/2=rtBoard.left+(i+1)*side/10-side/20=rtBoard.left+(2*i+1)*side/20
				BitBlt(hDC, rtBoard.left + side*(i * 2 + 1) / 20, rtBoard.top + side*(j * 2 + 1) / 20, d, d, hBlcDC, d, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i * 2 + 1) / 20, rtBoard.top + side*(j * 2 + 1) / 20, d, d, hBlcDC, 0, 0, SRCPAINT);
			}
			else if (board[i][j] == WHITE)
			{
				BitBlt(hDC, rtBoard.left + side*(i * 2 + 1) / 20, rtBoard.top + side*(j * 2 + 1) / 20, d, d, hWhtDC, d, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i * 2 + 1) / 20, rtBoard.top + side*(j * 2 + 1) / 20, d, d, hWhtDC, 0, 0, SRCPAINT);
			}
		}
	}

	//������ʾ���
	if (!stepStack.empty())
	{
		Step curStep = stepStack.top();
		int x = curStep.point.x;
		int y = curStep.point.y;
		BitBlt(hDC, rtBoard.left + side*(x * 2 + 1) / 20, rtBoard.top + side*(y * 2 + 1) / 20, d, d, hMarkDC, d, 0, SRCAND);
		BitBlt(hDC, rtBoard.left + side*(x * 2 + 1) / 20, rtBoard.top + side*(y * 2 + 1) / 20, d, d, hMarkDC, 0, 0, SRCPAINT);
	}

	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldFont);
}

/**
 * DrawChess - ��������Ԫ��
 */
VOID CNoGo::DrawChess()
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
}

/**
 * PlaySnd - ������Ч
 * @tag:	��Ч��ǩ
 */
bool CNoGo::PlaySnd(int tag)
{
	char filename[MAX_PATH] = { 0 };

	switch (tag)
	{
	case 0://������Ч
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\����.wav");
		break;
	default:
		break;
	}

	//������Ч
	if (!PlaySound(filename, NULL, SND_NOWAIT | SND_FILENAME))
	{
		ErrorBox("PlaySound failed");
		return false;
	}

	return true;
}

/**
 * InitGame - ��Ϸ��ʼ��
 */
VOID CNoGo::InitGame()
{
	memset(StepNum, 0, sizeof(StepNum));
	player = BLACK;
	count = 0;
	CleanStack(stepStack);//����ŷ�ջ
	InitBoard();//��ʼ������
}

/**
 * InitBoard - ��ʼ������
 */
VOID CNoGo::InitBoard()
{
	int i, j;
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			board[i][j] = EMPTY;
		}
	}

	//ˢ������
	InvalidateRect(hWnd, &rtBoard, FALSE);
	UpdateWindow(hWnd);
}

/**
 * ProcessMove - ����������Ϣ
 * @moveCmd:	�ŷ���Ϣ
 */
BOOL CNoGo::ProcessMove(char *moveCmd, char *curCmd, char *denCmd)
{
	Step tStep;
	char *res;
	int pos = 0;
	int len = strlen("move ");

	curCmd[0] = denCmd[0] = '\0';//Ĭ�Ͽ�����

	if ((res = strstr(moveCmd, "move")) == NULL)//Ѱ��move�ؼ���
	{
		return 0;//δ�ҵ���move���ؼ���
	}
	else
	{
		pos = (res - moveCmd);
		pos += len;

		//�����ŷ�
		tStep.point.x = moveCmd[pos] - BX;
		tStep.point.y = moveCmd[pos + 1] - BY;
		tStep.side = player;
		stepStack.push(tStep);//�����ŷ�ѹջ

		//�ж��Ƿ���Ϲ���
		if (!FitRules())
		{
			strcat(curCmd, "error\n");
			stepStack.pop();//�Ƿ��ŷ���ջ
			return -1;//����Υ��
		}

		//����
		board[tStep.point.x][tStep.point.y] = tStep.side;

		//ˢ������
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//����������Ч
		PlaySnd(0);

		//׷���ŷ���ʷ
		ShowStepHis(moveCmd + pos);

		//�������
		sprintf(denCmd, "move %c%c\n", tStep.point.x + BX, tStep.point.y + BY);

		//�ۼƲ���
		StepNum[player]++;

		//�ж�ʤ��
		if (WinOrLose())
		{
			strcat(denCmd, "end\n");//׷����������
			strcat(curCmd, "end\n");
			return 2;//�ֳ�ʤ��
		}

		//���廻��
		player = NEXTPLAYER(player);

		return 1;//��ȡ�ɹ�
	}
	return 0;
}

/**
 * OnLButtonDown - ��Ӧ��������������ŷ�
 * @x:	ָ�������
 * @y:	ָ��������
 * return:	�����ŷ�����״̬��-1��ʾ�������0��ʾ��������У�1��ʾ�������
 */
BOOL CNoGo::OnLButtonDown(int x, int y)
{
	Point point;

	if (count == -1)//������������״̬��count=-1ʱreturn0�����������
		return 0;

	//�Ѵ�������ӳ��Ϊ��������
	point.x = ((x - rtBoard.left) * 10 - side / 2) / side;
	point.y = ((y - rtBoard.top) * 10 - side / 2) / side;
	if (point.x < 0 || point.x >= 9 || point.y < 0 || point.y >= 9)//����Ч���
		return 2;

	return SToS(point);
}

/**
 * SToS - �ֶ��߼�����
 */
BOOL CNoGo::SToS(Point point)
{
	Step tStep;

	//����ŷ��Ϸ���
	if (board[point.x][point.y] != EMPTY)//�ǿ����
		return -1;//�ŷ��Ƿ�

	//����
	board[point.x][point.y] = player;

	//�����ʱ�ŷ�
	tStep.side = player;//�������
	tStep.point = point;//������
	stepStack.push(tStep);//��ʱ�ŷ�ѹջ

	count = -1;//�������

	//ˢ������
	InvalidateRect(hWnd, &rtBoard, FALSE);
	UpdateWindow(hWnd);
	//����������Ч
	PlaySnd(0);

	return 1;//�ŷ�����
}

/**
 * OkMove - ȷ���ŷ�
 */
INT CNoGo::OkMove(char *denCmd)
{
	Step tStep;

	denCmd[0] = '\0';//Ĭ�Ͽ�����

	if (count == -1)
	{
		tStep = stepStack.top();//��ȡ�����ŷ�

		//�������
		sprintf(denCmd, "move %c%c", tStep.point.x + BX, tStep.point.y + BY);

		//׷���ŷ���ʷ
		ShowStepHis(denCmd + 5);

		//�ۼƲ���
		StepNum[player]++;

		//�ж�ʤ��
		if (WinOrLose())
		{
			strcat(denCmd, "\nend\n");//׷����������
			return 2;//�ֳ�ʤ��
		}

		//��ȫ���
		strcat(denCmd, "\n");

		//���廻��
		player = NEXTPLAYER(player);

		count = 0;//��������

		return 1;//�ŷ�����
	}

	return 0;
}

/**
 * CancelMove - ȡ���ŷ�
 */
INT CNoGo::CancelMove()
{
	Step tStep;

	if (count == 0)//�����룬������
		return 0;

	tStep = stepStack.top();//��ȡ�ŷ�
	stepStack.pop();//�ŷ���ջ

	//�ָ�����
	board[tStep.point.x][tStep.point.y] = EMPTY;

	//ˢ������
	InvalidateRect(hWnd, &rtBoard, FALSE);
	UpdateWindow(hWnd);

	count = 0;//��������

	return 1;
}

/**
 * FitRules - �������
 */
bool CNoGo::FitRules()
{
	Step tStep = stepStack.top();//��ȡ�ŷ�

	if (tStep.point.x < 0 || tStep.point.x > 8 || tStep.point.y < 0 || tStep.point.y > 8)
	{
		return false;
	}
	if (board[tStep.point.x][tStep.point.y] != EMPTY)//ֻҪ�ǿյ�Ϳ�������
	{
		return false;
	}
	return true;
}

/**
 * WinOrLose - ���̲���
 */
bool CNoGo::WinOrLose()
{
	Step tStep = stepStack.top();
	BYTE side = tStep.side;
	int deplayer = 1 - side;

	//�ж���ɱ
	if (GetQi(tStep.point.x, tStep.point.y, side) == 0)
	{
		PostMessage(hWnd, GM_WINLOSE, (WPARAM)(StepNum[BLACK] << 16) + StepNum[WHITE], (LPARAM)deplayer);
		return true;
	}

	int i, j, k, tx, ty;
	int emptyPN = 0, wnPN = 0;
	for (j = 0; j < 9; j++)
	{
		for (i = 0; i < 9; i++)
		{
			if (board[i][j] == deplayer)
			{
				if (GetQi(i, j, deplayer) == 0)//ʹ�Է�������
				{
					PostMessage(hWnd, GM_WINLOSE, (WPARAM)(StepNum[BLACK] << 16) + StepNum[WHITE], (LPARAM)deplayer);
					return true;
				}
			}
			else if (board[i][j] == EMPTY)
			{
				emptyPN++;
				board[i][j] = deplayer;
				if (GetQi(i, j, deplayer) == 0)//�Է������ӽ���ɱ
				{
					wnPN++;
					board[i][j] = EMPTY;
					continue;
				}
				for (k = 0; k < 4; k++)
				{
					tx = i + lineVector[k][0];
					ty = j + lineVector[k][1];
					if (tx >= 0 && tx < 9 && ty >= 0 && ty < 9 && board[tx][ty] == side && GetQi(tx, ty, side) == 0)//�Է������ӽ�ʹ��������
					{
						wnPN++;
						break;
					}
				}
				board[i][j] = EMPTY;
			}
		}
	}
	if (emptyPN == wnPN)//�Է�û�п��������ӵ�
	{
		PostMessage(hWnd, GM_WINLOSE, (WPARAM)(StepNum[BLACK] << 16) + StepNum[WHITE], (LPARAM)side);
		return true;
	}
	return false;
}

/**
 * GetQi - ��ȡ��������
 * @x:	����x����
 * @y:	����y����
 * @side:	������ɫ
 */
int CNoGo::GetQi(int x, int y, int side)
{
	for (int k = 0; k < 81; k++)
		Biaoji[k] = -1;
	return GetQiForPoint(x, y, side);
}

int CNoGo::GetQiForPoint(int a, int b, int side)
{
	int x, y, i, qs = 0;
	Biaoji[a * 9 + b] = MARK;
	for (i = 0; i < 4; i++)
	{
		x = a + lineVector[i][0];
		y = b + lineVector[i][1];
		if (x < 0 || y < 0 || x >= 9 || y >= 9)//�������̱߽�
			continue;
		if (Biaoji[x * 9 + y] == MARK)
			continue;
		if (board[x][y] == EMPTY)
		{
			qs++;
			Biaoji[x * 9 + y] = MARK;
		}
		else if (board[x][y] == side)
			qs += GetQiForPoint(x, y, side);
	}
	return qs;
}
