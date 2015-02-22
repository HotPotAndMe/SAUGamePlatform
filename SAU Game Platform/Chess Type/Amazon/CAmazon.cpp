#include "CAmazon.h"


CAmazon::CAmazon(HINSTANCE hInst, HWND hWnd, char *LibPath)
{
	this->hInst = hInst;
	this->hWnd = hWnd;
	strncpy(this->LibPath, LibPath, MAX_PATH - 1);
	hPen = NULL;
	hFont = NULL;

	//��ȡ����
	GetConfig();

	//��������ˢ
	hBrush = CreateSolidBrush(RGB(115, 74, 18));

	//��������DC
	HDC hDC = GetDC(hWnd);
	hBlcDC = CreateCompatibleDC(hDC);
	hWhtDC = CreateCompatibleDC(hDC);
	hBarDC = CreateCompatibleDC(hDC);
	hMarkDC = CreateCompatibleDC(hDC);

	InitGame();
}

CAmazon::~CAmazon()
{
	if (hPen != NULL)
		DeleteObject(hPen);
	if (hFont != NULL)
		DeleteObject(hFont);
	DeleteObject(hBrush);
	DeleteDC(hBlcDC);
	DeleteDC(hWhtDC);
	DeleteDC(hBarDC);
	DeleteDC(hMarkDC);
}

/**
 * SetBoard - �������̲���
 * @rtBoard:	�����ڴ��ڿͻ�����λ��
 */
VOID CAmazon::SetBoard(RECT rtBoard)
{
	//�������̲���
	this->rtBoard = rtBoard;
	side = rtBoard.right - rtBoard.left + 1;
	d = side / 12;

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
VOID CAmazon::DrawBoard(HDC hDC)
{
	int i, j;
	RECT rect;
	HPEN hOldPen;
	HFONT hOldFont;

	//���ÿ��߻���
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	//���ÿ̶����塢��ɫ
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextColor(hDC, RGB(0, 0, 0));

	char letter[2] = { 0 }, number[3] = { 0 };

	//�������̰���
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1))
			{
				rect.left = rtBoard.left + side*(j + 1) / 12;
				rect.top = rtBoard.top + side*(i + 1) / 12;
				rect.right = rtBoard.left + side*(j + 2) / 12;
				rect.bottom = rtBoard.top + side*(i + 2) / 12;
				FillRect(hDC, &rect, hBrush);
			}
		}
	}

	//���Ʊ߿�
	MoveToEx(hDC, rtBoard.left + side / 12, rtBoard.top + side / 12, NULL);
	LineTo(hDC, rtBoard.left + side * 11 / 12, rtBoard.top + side / 12);
	LineTo(hDC, rtBoard.left + side * 11 / 12, rtBoard.top + side * 11 / 12);
	LineTo(hDC, rtBoard.left + side / 12, rtBoard.top + side * 11 / 12);
	LineTo(hDC, rtBoard.left + side / 12, rtBoard.top + side / 12);

	//�������̶̿�
	for (i = 1; i <= 10; i++)
	{
		letter[0] = 'A' + i - 1;
		itoa(i, number, 10);
		TextOut(hDC, rtBoard.left + side / 24 - fWidth / 2, rtBoard.top + side*(i * 2 + 1) / 24 - fHeight / 2, letter, 1);
		TextOut(hDC, rtBoard.left + side*(i * 2 + 1) / 24 - fWidth / 2, rtBoard.top + side / 24 - fHeight / 2, letter, 1);
		if (i < 10)
		{
			TextOut(hDC, rtBoard.left + side * 23 / 24 - fWidth / 2, rtBoard.top + side*(i * 2 + 1) / 24 - fHeight / 2, number, 1);
			TextOut(hDC, rtBoard.left + side*(i * 2 + 1) / 24 - fWidth / 2, rtBoard.top + side * 23 / 24 - fHeight / 2, number, 1);
		}
		else
		{
			TextOut(hDC, rtBoard.left + side * 23 / 24 - fWidth, rtBoard.top + side*(i * 2 + 1) / 24 - fHeight / 2, number, 2);
			TextOut(hDC, rtBoard.left + side*(i * 2 + 1) / 24 - fWidth, rtBoard.top + side * 23 / 24 - fHeight / 2, number, 2);
		}
	}

	//��������
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (board[i][j] == BLACK)
			{
				BitBlt(hDC, rtBoard.left + side*(i + 1) / 12, rtBoard.top + side*(j + 1) / 12, d, d, hBlcDC, d, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i + 1) / 12, rtBoard.top + side*(j + 1) / 12, d, d, hBlcDC, 0, 0, SRCPAINT);
			}
			else if (board[i][j] == WHITE)
			{
				BitBlt(hDC, rtBoard.left + side*(i + 1) / 12, rtBoard.top + side*(j + 1) / 12, d, d, hWhtDC, d, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i + 1) / 12, rtBoard.top + side*(j + 1) / 12, d, d, hWhtDC, 0, 0, SRCPAINT);
			}
			else if (board[i][j] == BAR)
			{
				BitBlt(hDC, rtBoard.left + side*(i + 1) / 12, rtBoard.top + side*(j + 1) / 12, d, d, hBarDC, d, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i + 1) / 12, rtBoard.top + side*(j + 1) / 12, d, d, hBarDC, 0, 0, SRCPAINT);
			}
		}
	}

	//������ʾ���
	if (!stepStack.empty())
	{
		Step curStep = stepStack.top();
		int x, y;
		IsChess(curStep.second)
		{
			x = curStep.second.x;
			y = curStep.second.y;
			BitBlt(hDC, rtBoard.left + side*(x + 1) / 12, rtBoard.top + side*(y + 1) / 12, d, d, hMarkDC, d, 0, SRCAND);
			BitBlt(hDC, rtBoard.left + side*(x + 1) / 12, rtBoard.top + side*(y + 1) / 12, d, d, hMarkDC, 0, 0, SRCPAINT);
		}
		else IsChess(curStep.first)
		{
			x = curStep.first.x;
			y = curStep.first.y;
			BitBlt(hDC, rtBoard.left + side*(x + 1) / 12, rtBoard.top + side*(y + 1) / 12, d, d, hMarkDC, d, 0, SRCAND);
			BitBlt(hDC, rtBoard.left + side*(x + 1) / 12, rtBoard.top + side*(y + 1) / 12, d, d, hMarkDC, 0, 0, SRCPAINT);
		}
	}

	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldFont);
}

/**
 * DrawChess - ��������Ԫ��
 */
VOID CAmazon::DrawChess()
{
	char filename[MAX_PATH] = { 0 };
	HBITMAP hBoardBmp, hBlcBmp, hWhtBmp, hBarBmp, hMarkBmp;

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\black.bmp");
	hBlcBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, d * 2, d, LR_LOADFROMFILE);

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\white.bmp");
	hWhtBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, d * 2, d, LR_LOADFROMFILE);

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\bar.bmp");
	hBarBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, d * 2, d, LR_LOADFROMFILE);

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\mark.bmp");
	hMarkBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, d * 2, d, LR_LOADFROMFILE);

	SelectObject(hBlcDC, hBlcBmp);
	SelectObject(hWhtDC, hWhtBmp);
	SelectObject(hBarDC, hBarBmp);
	SelectObject(hMarkDC, hMarkBmp);

	DeleteObject(hBlcBmp);
	DeleteObject(hWhtBmp);
	DeleteObject(hBarBmp);
	DeleteObject(hMarkBmp);
}

/**
 * PlaySnd - ������Ч
 * @tag:	��Ч��ǩ
 */
bool CAmazon::PlaySnd(int tag)
{
	char filename[MAX_PATH] = { 0 };

	switch (tag)
	{
	case 0://ѡ����Ч
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\ѡ��.wav");
		break;
	case 1://������Ч
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\����.wav");
		break;
	case 2://�����ϰ���Ч
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\�����ϰ�.wav");
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
VOID CAmazon::InitGame()
{
	memset(StepNum, 0, sizeof(StepNum));
	player = BLACK;//�ڷ�����	
	count = 0;
	CleanStack(stepStack);//����ŷ�ջ
	InitBoard();//��ʼ������
}

/**
 * InitBoard - ��ʼ������
 */
VOID CAmazon::InitBoard()
{
	int i, j;
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			board[i][j] = EMPTY;
		}
	}
	board[0][3] = WHITE; board[0][6] = BLACK;
	board[3][0] = WHITE; board[3][9] = BLACK;
	board[6][0] = WHITE; board[6][9] = BLACK;
	board[9][3] = WHITE; board[9][6] = BLACK;

	//ˢ������
	InvalidateRect(hWnd, &rtBoard, FALSE);
	UpdateWindow(hWnd);
}

/**
 * ProcessMove - ����������Ϣ
 * @moveCmd:	�ŷ���Ϣ
 */
BOOL CAmazon::ProcessMove(char *moveCmd, char *curCmd, char *denCmd)
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
		tStep.first.x = moveCmd[pos] - BX;
		tStep.first.y = moveCmd[pos + 1] - BY;
		tStep.second.x = moveCmd[pos + 2] - BX;
		tStep.second.y = moveCmd[pos + 3] - BY;
		tStep.third.x = moveCmd[pos + 4] - BX;
		tStep.third.y = moveCmd[pos + 5] - BY;
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
		board[tStep.first.x][tStep.first.y] = EMPTY;//ѡ��
		board[tStep.second.x][tStep.second.y] = player;//����
		board[tStep.third.x][tStep.third.y] = BAR;//�����ϰ�

		//ˢ������
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//���������ϰ���Ч
		PlaySnd(2);

		//׷���ŷ���ʷ
		ShowStepHis(moveCmd + pos);

		//�������
		sprintf(denCmd, "move %c%c%c%c%c%c\n", tStep.first.x + BX, tStep.first.y + BY, tStep.second.x + BX, tStep.second.y + BY, tStep.third.x + BX, tStep.third.y + BY);

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
BOOL CAmazon::OnLButtonDown(int x, int y)
{
	Point point;

	if (count == -1)//������������״̬
		return 0;

	//�Ѵ�������ӳ��Ϊ��������
	point.x = (x - rtBoard.left) * 12 / side - 1;
	point.y = (y - rtBoard.top) * 12 / side - 1;
	if (point.x < 0 || point.x >= 10 || point.y < 0 || point.y >= 10)
		return 2;

	return SToS(point);
}

/**
 * SToS - �ֶ��߼�����
 */
BOOL CAmazon::SToS(Point point)
{
	Step tStep;

	if (count == 0)//ѡ������
	{
		//����ŷ��Ϸ���
		if (board[point.x][point.y] != player)//δѡ�б�������
			return -1;//�ŷ��Ƿ�

		//�����ʱ�ŷ�
		InitStep(tStep);//��ʼ���ŷ�
		tStep.side = player;//�������
		tStep.first = point;//���ѡ��
		stepStack.push(tStep);//��ʱ�ŷ�ѹջ

		count = 1;

		//ˢ������
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//����ѡ����Ч
		PlaySnd(0);

		return 0;//�ŷ�����
	}
	else if (count == 1)//����
	{
		tStep = stepStack.top();//��ȡ��ʱ�ŷ�

		if (board[point.x][point.y] == player)//����ѡ������
		{
			stepStack.pop();//������ʱ�ŷ�
			tStep.first = point;//����ѡ������
			stepStack.push(tStep);//��ʱ�ŷ�ѹջ

			//ˢ������
			InvalidateRect(hWnd, &rtBoard, FALSE);
			UpdateWindow(hWnd);
			//����ѡ����Ч
			PlaySnd(0);

			return 0;//�ŷ�����
		}
		else if (board[point.x][point.y] == NEXTPLAYER(player) || !JudgeRule(tStep.first, point))//�ж�����
		{
			return -1;//�ŷ��Ƿ�
		}

		//����
		board[tStep.first.x][tStep.first.y] = EMPTY;
		board[point.x][point.y] = player;

		//�����ʱ�ŷ�
		stepStack.pop();//��ʱ�ŷ���ջ
		tStep.second = point;//����������
		stepStack.push(tStep);//��ʱ�ŷ�ѹջ

		count = 2;

		//ˢ������
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//����������Ч
		PlaySnd(1);

		return 0;//�ŷ�����
	}
	else if (count == 2)//�����ϰ�
	{
		tStep = stepStack.top();//��ȡ��ʱ�ŷ�

		if (board[point.x][point.y] != EMPTY || !JudgeRule(tStep.second, point))//�ж�����
			return -1;//�ŷ��Ƿ�

		//�����ϰ�
		board[point.x][point.y] = BAR;

		//�����ʱ�ŷ�
		stepStack.pop();//��ʱ�ŷ���ջ
		tStep.third = point;//����ϰ�����
		stepStack.push(tStep);//�����ŷ�ѹջ

		count = -1;//�������

		//ˢ������
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//���������ϰ���Ч
		PlaySnd(2);

		return 1;//�ŷ�����
	}

	return 0;
}

/**
 * OkMove - ȷ���ŷ�
 */
INT CAmazon::OkMove(char *denCmd)
{
	Step tStep;

	denCmd[0] = '\0';//Ĭ�Ͽ�����

	if (count == -1)//�������
	{
		tStep = stepStack.top();//��ȡ�����ŷ�

		//�������
		sprintf(denCmd, "move %c%c%c%c%c%c", tStep.first.x + BX, tStep.first.y + BY, tStep.second.x + BX, tStep.second.y + BY, tStep.third.x + BX, tStep.third.y + BY);
		
		//׷���ŷ���ʷ
		ShowStepHis(denCmd + 5);

		//�ۼƲ���
		StepNum[player]++;

		//�ж�ʤ��
		if (WinOrLose())
		{
			strcat(denCmd, "\nend\n");//׷����������
			return 2;
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
INT CAmazon::CancelMove()
{
	Step tStep;

	if (count == 0)//�����룬������
		return 0;

	tStep = stepStack.top();//��ȡ�ŷ�
	stepStack.pop();//�ŷ���ջ

	//�ָ��ϰ�
	IsChess(tStep.third)
	{
		board[tStep.third.x][tStep.third.y] = EMPTY;
	}

	//�ָ�����
	IsChess(tStep.second)
	{
		board[tStep.second.x][tStep.second.y] = EMPTY;
	}

	//�ָ�����
	IsChess(tStep.first)
	{
		board[tStep.first.x][tStep.first.y] = tStep.side;
	}

	//ˢ������
	InvalidateRect(hWnd, &rtBoard, FALSE);
	UpdateWindow(hWnd);

	count = 0;//��������

	return 1;
}

/**
 * FitRules - �������
 */
bool CAmazon::FitRules()
{
	Step tStep = stepStack.top();//��ȡ�ŷ�

	if (tStep.first.x < 0 || tStep.first.x>9 || tStep.first.y < 0 || tStep.first.y>9)
		return false;
	if (tStep.second.x < 0 || tStep.second.x>9 || tStep.second.y < 0 || tStep.second.y>9)
		return false;
	if (tStep.third.x < 0 || tStep.third.x>9 || tStep.third.y < 0 || tStep.third.y>9)
		return false;
	if (board[tStep.first.x][tStep.first.y] != tStep.side || board[tStep.second.x][tStep.second.y] != EMPTY						//������ӣ��������
		|| (tStep.second.x == tStep.third.x && tStep.second.y == tStep.third.y)													//�����ϰ��㲻Ϊͬһ��
		|| ((tStep.third.x != tStep.first.x || tStep.third.y != tStep.first.y) && board[tStep.third.x][tStep.third.y] != EMPTY))	//�ϰ�������ʱ���ϰ�������
		return false;
	if (!JudgeRule(tStep.first, tStep.second))//�ж�����
		return false;
	if (!JudgeRule(tStep.second, tStep.third))//�ж�����
		return false;
	return true;
}

/**
 * WinOrLose - ���̲���
 */
bool CAmazon::WinOrLose()
{
	int blc, wht;
	blc = wht = 0;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (board[i][j] == BLACK)
			{
				if (
					((0 < i &&j > 0 && board[i - 1][j - 1] != EMPTY) || (i == 0 || j == 0))			//���Ͻǵ㲻Ϊ��
					&& ((0 < i &&board[i - 1][j] != EMPTY) || i == 0)								//���㲻Ϊ��
					&& ((0 < i &&j < 9 && board[i - 1][j + 1] != EMPTY) || (i == 0 || j == 9))		//���½ǵ㲻Ϊ��
					&& ((j>0 && board[i][j - 1] != EMPTY) || j == 0)								//�Ϸ��㲻Ϊ��
					&& ((j < 9 && board[i][j + 1] != EMPTY) || j == 9)								//�·��㲻Ϊ��
					&& ((i < 9 && j>0 && board[i + 1][j - 1] != EMPTY) || (i == 9 || j == 0))		//���Ͻǵ㲻Ϊ��
					&& ((i < 9 && board[i + 1][j] != EMPTY) || (i == 9))							//�Ҳ�㲻Ϊ��
					&& ((i < 9 && j < 9 && board[i + 1][j + 1] != EMPTY) || (i == 9 || j == 9))		//�����ǵ㲻Ϊ��
					)
				{
					blc++;//�ڷ����������޷����ӣ������
				}
			}
			if (board[i][j] == WHITE)
			{
				if (
					((0 < i &&j > 0 && board[i - 1][j - 1] != EMPTY) || (i == 0 || j == 0))
					&& ((0 < i &&board[i - 1][j] != EMPTY) || i == 0)
					&& ((0 < i &&j < 9 && board[i - 1][j + 1] != EMPTY) || (i == 0 || j == 9))
					&& ((j>0 && board[i][j - 1] != EMPTY) || j == 0)
					&& ((j < 9 && board[i][j + 1] != EMPTY) || j == 9)
					&& ((i < 9 && j>0 && board[i + 1][j - 1] != EMPTY) || (i == 9 || j == 0))
					&& ((i < 9 && board[i + 1][j] != EMPTY) || (i == 9))
					&& ((i < 9 && j < 9 && board[i + 1][j + 1] != EMPTY) || (i == 9 || j == 9))
					)
				{
					wht++;//�׷����������޷����ӣ������
				}
			}
		}
	}
	if (wht == 4 || blc == 4)//�׷���·�������Ӯ,�ڷ���·�������Ӯ
	{
		SendMessage(hWnd, GM_WINLOSE, (WPARAM)(StepNum[BLACK] << 16) + StepNum[WHITE], (LPARAM)(wht == 4 ? BLACK : WHITE));
		return true;
	}
	return false;
}

/**
 * JudgeRule - �ж�����
 * @src:	�������
 * @des:	�������
 */
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
		else if (des.y < src.y)
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
		if (des.y == src.y)
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
