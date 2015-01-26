#include "CConSix.h"


const int lineVector[4][2] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 } };//��������


CConSix::CConSix(HINSTANCE hInst, HWND hWnd, char *LibPath)
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

/**
 * SetBoard - �������̲���
 * @rtBoard:	�����ڴ��ڿͻ�����λ��
 */
VOID CConSix::SetBoard(RECT rtBoard)
{
	//�������̲���
	this->rtBoard = rtBoard;
	side = rtBoard.right - rtBoard.left;
	d = side / 20;

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
VOID CConSix::DrawBoard(HDC hDC)
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
	for (i = 1; i <= 19; i++)//19γ��
	{
		//���ƿ���
		MoveToEx(hDC, rtBoard.left + side / 20, rtBoard.top + side*i / 20, NULL);
		LineTo(hDC, rtBoard.left + side * 19 / 20, rtBoard.top + side*i / 20);
		//���ƿ̶�
		letter[0] = 'A' + i - 1;
		itoa(i, number, 10);
		TextOut(hDC, rtBoard.left + fWidth, rtBoard.top + side*i / 20 - fHeight / 2, letter, 1);
		if (i < 10)
			TextOut(hDC, rtBoard.left + side * 19 / 20 + fWidth * 3 / 2, rtBoard.top + side*i / 20 - fHeight / 2, number, 2);
		else
			TextOut(hDC, rtBoard.left + side * 19 / 20 + fWidth, rtBoard.top + side*i / 20 - fHeight / 2, number, 2);
	}
	for (i = 1; i <= 19; i++)//19����
	{
		//���ƿ���
		MoveToEx(hDC, rtBoard.left + side*i / 20, rtBoard.top + side / 20, NULL);
		LineTo(hDC, rtBoard.left + side*i / 20, rtBoard.top + side * 19 / 20);
		//���ƿ̶�
		letter[0] = 'A' + i - 1;
		itoa(i, number, 10);
		TextOut(hDC, rtBoard.left + side*i / 20 - fWidth / 2, rtBoard.top + side / 40 - fHeight / 2, letter, 1);
		if (i < 10)
			TextOut(hDC, rtBoard.left + side*i / 20 - fWidth / 2, rtBoard.top + side * 39 / 40 - fHeight / 2, number, 1);
		else
			TextOut(hDC, rtBoard.left + side*i / 20 - fWidth, rtBoard.top + side * 39 / 40 - fHeight / 2, number, 2);
	}

	//��������
	for (i = 0; i < 19; i++)
	{
		for (j = 0; j < 19; j++)
		{
			if (board[i][j] == BLACK)
			{//rtBoard.left+d*(i+1)-d/2=rtBoard.left+(i+1)*side/20-side/40=rtBoard.left+(2*i+1)*side/40
				BitBlt(hDC, rtBoard.left + side*(i * 2 + 1) / 40, rtBoard.top + side*(j * 2 + 1) / 40, d, d, hBlcDC, d, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i * 2 + 1) / 40, rtBoard.top + side*(j * 2 + 1) / 40, d, d, hBlcDC, 0, 0, SRCPAINT);
			}
			else if (board[i][j] == WHITE)
			{
				BitBlt(hDC, rtBoard.left + side*(i * 2 + 1) / 40, rtBoard.top + side*(j * 2 + 1) / 40, d, d, hWhtDC, d, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i * 2 + 1) / 40, rtBoard.top + side*(j * 2 + 1) / 40, d, d, hWhtDC, 0, 0, SRCPAINT);
			}
		}
	}

	//������ʾ���
	if (!stepStack.empty())
	{
		Step curStep = stepStack.top();
		int x, y;
		IsChess(curStep.first)
		{
			x = curStep.first.x;
			y = curStep.first.y;
			BitBlt(hDC, rtBoard.left + side*(x * 2 + 1) / 40, rtBoard.top + side*(y * 2 + 1) / 40, d, d, hMarkDC, d, 0, SRCAND);
			BitBlt(hDC, rtBoard.left + side*(x * 2 + 1) / 40, rtBoard.top + side*(y * 2 + 1) / 40, d, d, hMarkDC, 0, 0, SRCPAINT);
		}
		IsChess(curStep.second)
		{
			x = curStep.second.x;
			y = curStep.second.y;
			BitBlt(hDC, rtBoard.left + side*(x * 2 + 1) / 40, rtBoard.top + side*(y * 2 + 1) / 40, d, d, hMarkDC, d, 0, SRCAND);
			BitBlt(hDC, rtBoard.left + side*(x * 2 + 1) / 40, rtBoard.top + side*(y * 2 + 1) / 40, d, d, hMarkDC, 0, 0, SRCPAINT);
		}
	}

	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldFont);
}

/**
 * DrawChess - ��������Ԫ��
 */
VOID CConSix::DrawChess()
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
bool CConSix::PlaySnd(int tag)
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
	if (!PlaySound(filename, NULL, SND_ASYNC | SND_NOWAIT | SND_FILENAME))
	{
		ErrorBox("PlaySound failed");
		return false;
	}

	return true;
}

/**
 * InitGame - ��Ϸ��ʼ��
 */
VOID CConSix::InitGame()
{
	memset(StepNum, 0, sizeof(StepNum));
	player = BLACK;
	first_hand = true;
	count = 0;
	CleanStack(stepStack);//����ŷ�ջ
	InitBoard();//��ʼ������
}

/**
 * InitBoard - ��ʼ������
 */
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

	//ˢ������
	InvalidateRect(hWnd, &rtBoard, FALSE);
	UpdateWindow(hWnd);
}

/**
 * ProcessMove - ����������Ϣ
 * @moveCmd:	�ŷ���Ϣ
 */
BOOL CConSix::ProcessMove(char *moveCmd)
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
		tStep.side = player;
		stepStack.push(tStep);//�����ŷ�ѹջ

		//�ж��Ƿ���Ϲ���
		if (!FitRules())
		{
			strcat(curCmd, "error\n");
			stepStack.pop();//�Ƿ��ŷ���ջ
			return -1;//����Υ��
		}

		//���һ��
		board[tStep.first.x][tStep.first.y] = tStep.side;

		if (first_hand == true)//��һ��
		{
			first_hand = false;
		}
		else
		{
			//��ڶ���
			if (tStep.second.x != -1 && tStep.second.y != -1)
				board[tStep.second.x][tStep.second.y] = tStep.side;
		}

		//ˢ������
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//����������Ч
		PlaySnd(0);

		//׷���ŷ���ʷ
		ShowStepHis(moveCmd + pos);

		//�������
		sprintf(denCmd, "move %c%c%c%c\n", tStep.first.x + BX, tStep.first.y + BY, tStep.second.x + BX, tStep.second.y + BY);

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
BOOL CConSix::OnLButtonDown(int x, int y)
{
	Point point;

	if (count == -1)//������������״̬
		return 0;

	//�Ѵ�������ӳ��Ϊ��������
	point.x = ((x - rtBoard.left) * 20 - side / 2) / side;
	point.y = ((y - rtBoard.top) * 20 - side / 2) / side;
	if (point.x < 0 || point.x >= 19 || point.y < 0 || point.y >= 19)//����Ч���
		return 2;

	return SToS(point);
}

/**
 * SToS - �ֶ��߼�����
 */
BOOL CConSix::SToS(Point point)
{
	Step tStep;

	//����ŷ��Ϸ���
	if (board[point.x][point.y] != EMPTY)//�ǿ����
		return -1;//�ŷ��Ƿ�

	//����
	board[point.x][point.y] = player;

	if (count == 0)//���һ��
	{
		//�����ʱ�ŷ�
		InitStep(tStep);//��ʼ���ŷ�
		tStep.side = player;//�������
		tStep.first = point;//����һ�����
		stepStack.push(tStep);//��ʱ�ŷ�ѹջ

		if (first_hand)//��һ��
		{
			count = -1;//�������

			//ˢ������
			InvalidateRect(hWnd, &rtBoard, FALSE);
			UpdateWindow(hWnd);
			//����������Ч
			PlaySnd(0);

			return 1;//�ŷ�����
		}
		else
		{
			count = 1;

			//ˢ������
			InvalidateRect(hWnd, &rtBoard, FALSE);
			UpdateWindow(hWnd);
			//������Ч
			PlaySnd(0);

			//ʶ����һ�ӻ�ʤ�ŷ�
			if (WinOrLose())
			{
				count = -1;//�������
				return 1;//�ŷ�����
			}

			return 0;//�ŷ�����
		}
	}
	else if (count == 1)//��ڶ���
	{
		//�����ʱ�ŷ�
		tStep = stepStack.top();//��ȡ��ʱ�ŷ�
		stepStack.pop();//��ʱ�ŷ���ջ
		tStep.second = point;//���ڶ������
		stepStack.push(tStep);//�����ŷ�ѹջ

		count = -1;//�������

		//ˢ������
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//����������Ч
		PlaySnd(0);

		return 1;//�ŷ�����
	}

	return 0;
}

/**
 * OkMove - ȷ���ŷ�
 */
INT CConSix::OkMove()
{
	Step tStep;

	denCmd[0] = '\0';//Ĭ�Ͽ�����

	if (count == -1)//�������
	{
		tStep = stepStack.top();//��ȡ�����ŷ�

		//�������
		sprintf(denCmd, "move %c%c%c%c", tStep.first.x + BX, tStep.first.y + BY, tStep.second.x + BX, tStep.second.y + BY);

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

		//ȡ����һ�ֱ�־
		first_hand = false;

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
INT CConSix::CancelMove()
{
	Step tStep;

	if (count == 0)//�����룬������
		return 0;

	tStep = stepStack.top();//��ȡ�ŷ�
	stepStack.pop();//�ŷ���ջ

	//�ָ��ڶ���
	IsChess(tStep.second)
	{
		board[tStep.second.x][tStep.second.y] = EMPTY;
	}

	//�ָ���һ��
	IsChess(tStep.first)
	{
		board[tStep.first.x][tStep.first.y] = EMPTY;
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
bool CConSix::FitRules()
{
	Step tStep = stepStack.top();//��ȡ�ŷ�

	if (tStep.first.x < 0 || tStep.first.x > 18 || tStep.first.y < 0 || tStep.first.y > 18)//��һ�Ӳ��������ڣ���Ч
	{
		return false;
	}
	if (first_hand == true)//��һ��
	{
		if (board[tStep.first.x][tStep.first.y] != EMPTY)//��һ�Ӳ�Ϊ�գ���Ч
		{
			return false;
		}
	}
	else
	{
		if (tStep.second.x == -1 && tStep.second.y == -1)//�ڶ�����Ч
		{
			if (board[tStep.first.x][tStep.first.y] != EMPTY)//��һ����㲻Ϊ�գ���Ч
				return false;
			else
				return true;
		}
		if (tStep.second.x < 0 || tStep.second.x > 18 || tStep.second.y < 0 || tStep.second.y > 18)//�ڶ��Ӳ��������ڣ���Ч
		{
			return false;
		}
		if (board[tStep.first.x][tStep.first.y] != EMPTY || board[tStep.second.x][tStep.second.y] != EMPTY)//�ڶ�����㲻Ϊ�գ���Ч
		{
			return false;
		}
	}
	return true;
}

/**
 * WinOrLose - ���̲���
 */
bool CConSix::WinOrLose()
{
	Step tStep = stepStack.top();//��ȡ�ŷ�
	BYTE side = tStep.side;
	Point point[2] = { tStep.first, tStep.second };
	int i, j, tx, ty, connect;

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
			if (connect >= 6)//����
			{
				if (count != -1)//countΪ-1ʱ��ʾ�����ִ���ȡ�ŷ���countΪ0ʱ��ʾ�����洦��ȡ�ŷ���countΪ1ʱ��ʾģ����һ�ӻ�ʤ
				{
					PostMessage(hWnd, GM_WINLOSE, (WPARAM)(StepNum[BLACK] << 16) + StepNum[WHITE], (LPARAM)side);
				}
				return true;
			}
		}
	}
	return false;
}

