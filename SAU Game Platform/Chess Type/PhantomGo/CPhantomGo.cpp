#include "CPhantomGo.h"


const int lineVector[4][2] = { 0, 1, 0, -1, -1, 0, 1, 0 };


CPhantomGo::CPhantomGo(HINSTANCE hInst, HWND hWnd, char *LibPath)
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

CPhantomGo::~CPhantomGo()
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
VOID CPhantomGo::SetBoard(RECT rtBoard)
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
VOID CPhantomGo::DrawBoard(HDC hDC)
{
	int i,j;
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
bool CPhantomGo::DrawChess()
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

/**
 * PlaySnd - ������Ч
 * @tag:	��Ч��ǩ
 */
bool CPhantomGo::PlaySnd(int tag)
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
VOID CPhantomGo::InitGame()
{
	memset(StepNum, 0, sizeof(StepNum));
	player = BLACK;
	takeNum = 0;
	flagJ = false;
	flagP = false;
	count = 0;
	CleanStack(stepStack);//����ŷ�ջ
	InitBoard();//��ʼ������
}

/**
 * InitBoard - ��ʼ������
 */
VOID CPhantomGo::InitBoard()
{
	int i, j;
	for (i = 0; i<9; i++)
	{
		for (j = 0; j<9; j++)
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
BOOL CPhantomGo::ProcessMove(char *moveCmd, char *curCmd, char *denCmd)
{	
	Step tStep;
	char *res;
	int pos = 0, i;
	int len=strlen("move ");

	curCmd[0] = denCmd[0] = '\0';//Ĭ�Ͽ�����

	if ((res = strstr(moveCmd, "move")) == NULL)//Ѱ��move�ؼ���
	{
		return 0;//δ�ҵ���move���ؼ���
	}
	else
	{		
		pos = (res - moveCmd);
		pos+=len;
		
		if (strncmp("pass", moveCmd + pos, 4) == 0)// "move pass\n"�������
		{
			//׷���ŷ���ʷ
			ShowStepHis("pass");

			if (flagP)
			{
				//�ۼƲ���
				StepNum[player]++;

				//����ʤ��
				WinOrLose();

				//�������
				strcat(denCmd, "move pass\nend\n");
				strcat(curCmd, "end\n");

				return 2;//�ֳ�ʤ��
			}

			flagP = true;//����pass���

			//�������
			strcat(denCmd, "move pass\n");

			//�ۼƲ���
			StepNum[player]++;

			//���廻��
			player = NEXTPLAYER(player);

			return 1;//��ȡ�ɹ�
		}
		else if (strncmp("go", moveCmd + pos, 2) == 0)// "move go\n"���֪ͨ���壬�����������
		{
			//ѯ���ŷ�
			strcat(curCmd, "move quest\n");
			return 0;
		}
		else if (strncmp("quest", moveCmd + pos, 5) == 0)// "move quest\n"���ѯ���ŷ��������������
		{
			//�����ŷ�
			tStep = stepStack.top();//��ȡ�����ŷ�
			sprintf(curCmd, "move %c%c\n", tStep.point.x + BX, tStep.point.y + BY);//���˫���������Э���׼��һ�£����ܽ����������
			return 0;
		}
		else// "move XY\n"�������
		{
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

			//׷�ӽ����ŷ�����
			strcat(curCmd, "access\n");

			flagP = false;//����Pass���
			flagJ = false;//ȡ���ٱ��
			if (Take(tStep.point.x, tStep.point.y, tStep.side))//���Ӳ���
			{
				//׷����������
				sprintf(denCmd, "taked %d %s\n", takeNum, takeList);
				sprintf(curCmd + strlen(curCmd), "take %d %s\n", takeNum, takeList);

				//����
				if (takeNum == 1)//��һ�ӣ����һ���жϽ�
				{
					//�жϽ�
					if (Take(takeList[0] - BX, takeList[1] - BY, 1 - tStep.side) == 1)//һ��һ��˫�����������öԷ������Ӻ͵�ǰ���ӽ��бȽ�
					{
						flagJ = true;//�ýٱ��
						Jie.x = denCmd[6] - BX;
						Jie.y = denCmd[7] - BY;
					}
					board[denCmd[8] - BX][denCmd[9] - BY] = EMPTY;//����
				}
				else
				{
					for (i = 0; i < takeNum; i++)
						board[takeList[i * 2] - BX][takeList[1 + i * 2] - BY] = EMPTY;//����
				}
			}

			//ˢ������
			InvalidateRect(hWnd, &rtBoard, FALSE);
			UpdateWindow(hWnd);
			//����������Ч
			PlaySnd(0);

			//׷���ŷ���ʷ
			ShowStepHis(moveCmd + pos);

			//׷��֪ͨ����
			strcat(denCmd, "move go\n");

			//�ۼƲ���
			StepNum[player]++;

			//���廻��
			player = NEXTPLAYER(player);

			return 1;//��ȡ�ɹ�
		}
	}
	return 0;
}

/**
 * OnLButtonDown - ��Ӧ��������������ŷ�
 * @x:	ָ�������
 * @y:	ָ��������
 * return:	�����ŷ�����״̬��-1��ʾ�������0��ʾ��������У�1��ʾ�������
 */
BOOL CPhantomGo::OnLButtonDown(int x, int y)
{
	Point point;

	if (count == -1)//������������״̬��count=-1ʱreturn0�����������
		return 0;

	//�Ѵ�������ӳ��Ϊ��������
	point.x = ((x - rtBoard.left) * 10 - side / 2) / side;
	point.y = ((y - rtBoard.top) * 10 - side / 2) / side;
	if (point.x < 0 || point.x >= 9 || point.y < 0 || point.y >= 9)
		return 2;

	return SToS(point);
}

/**
 * SToS - �ֶ��߼�����
 */
BOOL CPhantomGo::SToS(Point point)
{
	Step tStep;

	//�����ʱ�ŷ�
	tStep.side = player;//�������
	tStep.point = point;//������
	stepStack.push(tStep);//��ʱ�ŷ�ѹջ

	//����ŷ��Ϸ���
	if (!FitRules())
	{
		stepStack.pop();//��ʱ�ŷ���ջ
		return -1;//�ŷ��Ƿ�
	}

	//����
	board[point.x][point.y] = player;

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
INT CPhantomGo::OkMove(char *denCmd)
{
	Step tStep;
	char step[5];
	int i;

	denCmd[0] = '\0';//Ĭ�Ͽ�����

	if (count == 0)//pass
	{
		//׷���ŷ���ʷ
		ShowStepHis("pass");

		if (flagP)//˫��pass��������
		{
			//�ۼƲ���
			StepNum[player]++;

			//����ʤ��
			WinOrLose();

			//�������
			strcat(denCmd, "move pass\nend\n");

			return 2;//�ֳ�ʤ��
		}

		flagP = true;//����pass���

		//�������
		strcat(denCmd, "move pass\n");

		//�ۼƲ���
		StepNum[player]++;

		//���廻��
		player = NEXTPLAYER(player);

		return 1;
	}
	else if (count == -1)
	{
		tStep = stepStack.top();//��ȡ�����ŷ�

		//׷���ŷ���ʷ
		sprintf(step, "%c%c", tStep.point.x + BX, tStep.point.y + BY);
		ShowStepHis(step);

		flagP = false;//ȡ��Pass���
		flagJ = false;//ȡ���ٱ��
		if (Take(tStep.point.x, tStep.point.y, tStep.side))//���Ӳ���
		{
			//׷����������
			sprintf(denCmd, "taked %d %s\n", takeNum, takeList);

			//����
			if (takeNum == 1)//��һ�ӣ����һ���жϽ�
			{
				//�жϽ�
				if (Take(takeList[0] - BX, takeList[1] - BY, 1 - tStep.side) == 1)//һ��һ��˫�����������öԷ������Ӻ͵�ǰ���ӽ��бȽ�
				{
					flagJ = true;//�ýٱ��
					Jie.x = denCmd[6] - BX;
					Jie.y = denCmd[7] - BY;
				}
				board[denCmd[8] - BX][denCmd[9] - BY] = EMPTY;
			}
			else
			{
				for (i = 0; i < takeNum; i++)
					board[takeList[i * 2] - BX][takeList[1 + i * 2] - BY] = EMPTY;//����
			}

			//ˢ������
			InvalidateRect(hWnd, &rtBoard, FALSE);
			UpdateWindow(hWnd);
		}

		//׷��֪ͨ��������
		strcat(denCmd, "move go\n");// "move go\n"����

		//�ۼƲ���
		StepNum[player]++;

		//���廻��
		player = NEXTPLAYER(player);

		count = 0;//��������

		return 1;//�ŷ�����
	}
	return 0;
}

INT CPhantomGo::CancelMove()
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

/*
 * FitRules - �������
 */
bool CPhantomGo::FitRules()//�Ƿ���Ϲ���
{
	Step tStep = stepStack.top();//��ȡ�ŷ�
	int deplayer = 1 - tStep.side;

	if (tStep.point.x<0 || tStep.point.x>8 || tStep.point.y<0 || tStep.point.y>8)
	{
		return false;
	}
	if (board[tStep.point.x][tStep.point.y] != EMPTY)
	{
		return false;
	}

	//����
	board[tStep.point.x][tStep.point.y] = tStep.side;

	if (GetQi(tStep.point.x, tStep.point.y, tStep.side) == 0)//��������
	{
		if (!Take(tStep.point.x, tStep.point.y, tStep.side))//������ԣ����е�
		{
			//�ָ�����
			board[tStep.point.x][tStep.point.y] = EMPTY;
			return false;
		}
		if (flagJ)//�жϽٵ�
		{
			if ((Jie.x == tStep.point.x) && (Jie.y == tStep.point.y))//�ٵ㲻�����ӣ���ҪӦ��
			{
				//�ָ�����
				board[tStep.point.x][tStep.point.y] = EMPTY;
				return false;
			}
		}
	}
	
	//�ָ�����
	board[tStep.point.x][tStep.point.y] = EMPTY;

	return true;
}

/**
 * WinOrLose - ���̲���
 */
bool CPhantomGo::WinOrLose()
{
	//��ӰΧ����������Χ������б�ʤ������ֹ���汾�в�֧��
	PostMessage(hWnd, GM_WINLOSE, (WPARAM)(StepNum[BLACK] << 16) + StepNum[WHITE], (LPARAM)0);
	return true;
}

/**
 * GetQi - ��ȡ��������
 * @x:	����x����
 * @y:	����y����
 * @side:	������ɫ
 */
int CPhantomGo::GetQi(int x, int y, int side)
{
	for (int k = 0; k<81; k++)
		Biaoji[k] = -1;
	return GetQiForPoint(x, y, side);
}

int CPhantomGo::GetQiForPoint(int a, int b, int side)
{
	int x, y, i, qs = 0;
	Biaoji[a * 9 + b] = MARK;
	for (i = 0; i<4; i++)
	{
		x = a + lineVector[i][0];
		y = b + lineVector[i][1];
		if (x<0 || y<0 || x >= 9 || y >= 9)//�������̱߽�
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

/**
* Take - ���Ӳ���
* @x:	�����ӵ������
* @y��	�����ӵ�������
* @side:	��������ɫ
*/
bool CPhantomGo::Take(int x, int y, int side)
{
	bool take = false;
	int i, j, tx, ty, deplayer = 1 - side;
	for (i = 0; i < 81; i++)
		NoQi[i] = 0;
	for (i = 0; i < 4; i++)
	{
		tx = x + lineVector[i][0];
		ty = y + lineVector[i][1];
		if (tx<0 || ty<0 || tx >= 9 || ty >= 9)//�������̱߽�
			continue;
		if (board[tx][ty] == deplayer)//�ѱ�����
		{
			if (NoQi[tx * 9 + ty] == 1)
				continue;
			if (GetQi(tx, ty, deplayer) == 0)//��������
			{
				take = true;
				for (j = 0; j < 81; j++)
					if (Biaoji[j] == MARK)
						NoQi[j] = 1;
			}
		}
	}
	takeNum = 0;
	for (i = 0; i < 81; i++)
	{
		if (NoQi[i])
		{
			sprintf(takeList + takeNum * 2, "%c%c", BX + i / 9, BY + i % 9);
			takeNum++;
		}
	}
	return take;
}
