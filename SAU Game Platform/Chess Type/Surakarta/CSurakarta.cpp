#include "CSurakarta.h"


#define PI 3.1415926


/**
 * IsCorner - �ж϶���
 */
inline bool IsCorner(const int &x, const int &y)
{
	if ((x == 0 && y == 0) || (x == 0 && y == 5) || (x == 5 && y == 0) || (x == 5 && y == 5))//�ĸ���
		return true;
	else
		return false;
}

//С��·��
Point SmaWay[28] = { { 1, 1 }, { 1, 2 }, { 1, 3 }, { 1, 4 }, { 1, 5 }, { 0, 5 }, { 0, 4 }, { 1, 4 }, { 2, 4 }, { 3, 4 }, { 4, 4 }, { 5, 4 }, { 5, 5 }, { 4, 5 },
{ 4, 4 }, { 4, 3 }, { 4, 2 }, { 4, 1 }, { 4, 0 }, { 5, 0 }, { 5, 1 }, { 4, 1 }, { 3, 1 }, { 2, 1 }, { 1, 1 }, { 0, 1 }, { 0, 0 }, { 1, 0 } };
//��·��
Point BigWay[28] = { { 2, 2 }, { 2, 3 }, { 2, 4 }, { 2, 5 }, { 0, 5 }, { 0, 3 }, { 1, 3 }, { 2, 3 }, { 3, 3 }, { 4, 3 }, { 5, 3 }, { 5, 5 }, { 3, 5 }, { 3, 4 },
{ 3, 3 }, { 3, 2 }, { 3, 1 }, { 3, 0 }, { 5, 0 }, { 5, 2 }, { 4, 2 }, { 3, 2 }, { 2, 2 }, { 1, 2 }, { 0, 2 }, { 0, 0 }, { 2, 0 }, { 2, 1 } };

CSurakarta::CSurakarta(HINSTANCE hInst, HWND hWnd, char *LibPath)
{
	this->hInst = hInst;
	this->hWnd = hWnd;
	strncpy(this->LibPath, LibPath, MAX_PATH - 1);
	hRPen = NULL;
	hGPen = NULL;
	hBPen = NULL;
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

CSurakarta::~CSurakarta()
{
	if (hRPen != NULL)
		DeleteObject(hRPen);
	if (hGPen != NULL)
		DeleteObject(hGPen);
	if (hBPen != NULL)
		DeleteObject(hBPen);
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
VOID CSurakarta::SetBoard(RECT rtBoard)
{
	//�������̲���
	this->rtBoard = rtBoard;
	side = rtBoard.right - rtBoard.left;
	d = side / 11;
	size = (int)(d*0.6);

	//���ÿ��߻���
	if (hRPen != NULL)
		DeleteObject(hRPen);
	if (hGPen != NULL)
		DeleteObject(hGPen);
	if (hBPen != NULL)
		DeleteObject(hBPen);
	pWidth = (int)(side / 220);
	if (pWidth == 0)
		pWidth = 1;
	hRPen = CreatePen(PS_SOLID, pWidth, RGB(255, 0, 0));
	hGPen = CreatePen(PS_SOLID, pWidth, RGB(0, 255, 0));
	hBPen = CreatePen(PS_SOLID, pWidth, RGB(0, 0, 255));

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
VOID CSurakarta::DrawBoard(HDC hDC)
{
	HFONT hOldFont;
	HPEN hOldPen;

	//��������

	int r1 = side / 11;//С���뾶
	int r2 = side * 2 / 11;//�󻡰뾶
	int gap = side * 3 / 11;
	int left = rtBoard.left + gap;
	int top = rtBoard.top + gap;
	int right = rtBoard.right - gap;
	int bottom = rtBoard.bottom - gap;

	//���ƺ�ɫ����
	hOldPen = (HPEN)SelectObject(hDC, hRPen);//���ú�ɫ���߻���
	MoveToEx(hDC, left, top, NULL);
	LineTo(hDC, right, top);
	LineTo(hDC, right, bottom);
	LineTo(hDC, left, bottom);
	LineTo(hDC, left, top);
	SelectObject(hDC, hOldPen);

	//������ɫ����
	hOldPen = (HPEN)SelectObject(hDC, hGPen);//������ɫ��������
	MoveToEx(hDC, left + r1, top, NULL);
	AngleArc(hDC, left, top, r1, 0, 270);
	LineTo(hDC, right, top + r1);
	AngleArc(hDC, right, top, r1, 270, 270);
	LineTo(hDC, right - r1, bottom);
	AngleArc(hDC, right, bottom, r1, 180, 270);
	LineTo(hDC, left, bottom - r1);
	AngleArc(hDC, left, bottom, r1, 90, 270);
	LineTo(hDC, left + r1, top);
	SelectObject(hDC, hOldPen);

	//������ɫ����
	hOldPen = (HPEN)SelectObject(hDC, hBPen);//������ɫ����
	MoveToEx(hDC, left + r2, top, NULL);
	AngleArc(hDC, left, top, r2, 0, 270);
	LineTo(hDC, right, top + r2);
	AngleArc(hDC, right, top, r2, 270, 270);
	LineTo(hDC, right - r2, bottom);
	AngleArc(hDC, right, bottom, r2, 180, 270);
	LineTo(hDC, left, bottom - r2);
	AngleArc(hDC, left, bottom, r2, 90, 270);
	LineTo(hDC, left + r2, top);
	SelectObject(hDC, hOldPen);

	//���ÿ̶����塢��ɫ
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextColor(hDC, RGB(0, 0, 0));

	int i, j;
	char letter[2] = { 0 }, number[2] = { 0 };

	//���ƿ̶�
	for (i = 0; i < 6; i++)
	{
		letter[0] = 'A' + i;
		number[0] = '1' + i;
		TextOut(hDC, rtBoard.left + (d - fWidth) / 2, (int)(top + side*i / 11 - fHeight / 2), letter, 1);
		TextOut(hDC, rtBoard.right - (d + fWidth) / 2, (int)(top + side*i / 11 - fHeight / 2), number, 1);
		TextOut(hDC, (int)(left - fWidth / 2 + side*i / 11), rtBoard.top + (d - fHeight) / 2, letter, 1);
		TextOut(hDC, (int)(left - fWidth / 2 + side*i / 11), rtBoard.bottom - (d + fHeight) / 2, number, 1);
	}

	//��������
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 6; j++)
		{
			if (board[i][j] == BLACK)
			{
				BitBlt(hDC, rtBoard.left + side*(i + 3) / 11 - size / 2, rtBoard.top + side*(j + 3) / 11 - size / 2, size, size, hBlcDC, size, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i + 3) / 11 - size / 2, rtBoard.top + side*(j + 3) / 11 - size / 2, size, size, hBlcDC, 0, 0, SRCPAINT);
			}
			else if (board[i][j] == WHITE)
			{
				BitBlt(hDC, rtBoard.left + side*(i + 3) / 11 - size / 2, rtBoard.top + side*(j + 3) / 11 - size / 2, size, size, hWhtDC, size, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i + 3) / 11 - size / 2, rtBoard.top + side*(j + 3) / 11 - size / 2, size, size, hWhtDC, 0, 0, SRCPAINT);
			}
		}
	}

	//������ʾ���
	if (!stepStack.empty())
	{
		Step tStep = stepStack.top();
		IsChess(tStep.end)
		{
			IsChess(ptMovie)//���Ӷ�������
			{
				if (ptMovie.side == BLACK)
				{
					BitBlt(hDC, rtBoard.left + ptMovie.x - size / 2, rtBoard.top + ptMovie.y - size / 2, size, size, hBlcDC, size, 0, SRCAND);
					BitBlt(hDC, rtBoard.left + ptMovie.x - size / 2, rtBoard.top + ptMovie.y - size / 2, size, size, hBlcDC, 0, 0, SRCPAINT);
				}
				else if (ptMovie.side == WHITE)
				{
					BitBlt(hDC, rtBoard.left + ptMovie.x - size / 2, rtBoard.top + ptMovie.y - size / 2, size, size, hWhtDC, size, 0, SRCAND);
					BitBlt(hDC, rtBoard.left + ptMovie.x - size / 2, rtBoard.top + ptMovie.y - size / 2, size, size, hWhtDC, 0, 0, SRCPAINT);
				}
			}
			else//�����
			{
				BitBlt(hDC, rtBoard.left + side*(tStep.end.x + 3) / 11 - size / 2, rtBoard.top + side*(tStep.end.y + 3) / 11 - size / 2, size, size, hMarkDC, size, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(tStep.end.x + 3) / 11 - size / 2, rtBoard.top + side*(tStep.end.y + 3) / 11 - size / 2, size, size, hMarkDC, 0, 0, SRCPAINT);
			}
		}
		else IsChess(tStep.start)//ѡ������
		{
			BitBlt(hDC, rtBoard.left + side*(tStep.start.x + 3) / 11 - size / 2, rtBoard.top + side*(tStep.start.y + 3) / 11 - size / 2, size, size, hMarkDC, size, 0, SRCAND);
			BitBlt(hDC, rtBoard.left + side*(tStep.start.x + 3) / 11 - size / 2, rtBoard.top + side*(tStep.start.y + 3) / 11 - size / 2, size, size, hMarkDC, 0, 0, SRCPAINT);
		}
	}

	SelectObject(hDC, hOldFont);
}

/**
 * DrawChess - ��������Ԫ��
 */
VOID CSurakarta::DrawChess()
{
	char filename[MAX_PATH] = { 0 };
	HBITMAP hBlcBmp, hWhtBmp, hMarkBmp;

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\black.bmp");
	hBlcBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, size * 2, size, LR_LOADFROMFILE);

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\white.bmp");
	hWhtBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, size * 2, size, LR_LOADFROMFILE);

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\mark.bmp");
	hMarkBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, size * 2, size, LR_LOADFROMFILE);

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
bool CSurakarta::PlaySnd(int tag)
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
	case 2://������Ч
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
VOID CSurakarta::InitGame()
{
	memset(StepNum, 0, sizeof(StepNum));
	player = BLACK;
	ptMovie.side = EMPTY;
	ptMovie.x = ptMovie.y = -1;
	count = 0;
	CleanStack(stepStack);//����ŷ�ջ
	InitBoard();//��ʼ������
	return;
}

/**
 * InitBoard - ��ʼ������
 */
VOID CSurakarta::InitBoard()
{
	int i, j;
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 6; j++)
		{
			if (j == 0 || j == 1)
			{
				board[i][j] = BLACK;
			}
			else if (j == 2 || j == 3)
			{
				board[i][j] = EMPTY;
			}
			else
			{
				board[i][j] = WHITE;
			}
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
INT CSurakarta::ProcessMove(char *moveCmd, char *curCmd, char *denCmd)
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
		tStep.start.x = moveCmd[pos] - BX;
		tStep.start.y = moveCmd[pos + 1] - BY;
		tStep.end.x = moveCmd[pos + 2] - BX;
		tStep.end.y = moveCmd[pos + 3] - BY;
		tStep.side = player;
		stepStack.push(tStep);//��ʱ�ŷ�ѹջ

		//�ж��Ƿ���Ϲ���
		if (!FitRules())
		{
			strcat(curCmd, "error\n");
			stepStack.pop();//�Ƿ��ŷ���ջ
			return -1;//����Υ��
		}

		//����
		MoveChess();//������ˢ�����̣�������Ч

		//׷���ŷ���ʷ
		ShowStepHis(moveCmd + pos);

		//�������
		sprintf(denCmd, "move %c%c%c%c\n", tStep.start.x + BX, tStep.start.y + BY, tStep.end.x + BX, tStep.end.y + BY);

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
BOOL CSurakarta::OnLButtonDown(int x, int y)
{
	Point point;

	if (count == -1)//������������״̬
		return 0;

	//�Ѵ�������ӳ��Ϊ��������
	point.x = (int)((x - rtBoard.left - side / 22) * 11 / side) - 2;
	point.y = (int)((y - rtBoard.top - side / 22) * 11 / side) - 2;
	if (point.x < 0 || point.x >= 6 || point.y < 0 || point.y >= 6)
		return 2;

	return SToS(point);
}

/**
 * SToS - �ֶ��߼�����
 */
BOOL CSurakarta::SToS(Point point)
{
	Step tStep;

	if (count == 0)//ѡ��
	{
		//����ŷ��Ϸ���
		if (board[point.x][point.y] != player)//ѡ�б�������
			return -1;//�ŷ��Ƿ�

		//�����ʱ�ŷ�
		InitStep(tStep);//��ʼ���ŷ�
		tStep.side = player;//�������
		tStep.start = point;//���ѡ��
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
		//�����ʱ�ŷ�
		tStep = stepStack.top();//��ȡ��ʱ�ŷ�
		tStep.end = point;//������
		stepStack.push(tStep);//��ʱ�ŷ�ѹջ��ΪFitRules��׼��

		//����ŷ��Ϸ���
		if (!FitRules())
		{
			stepStack.pop();//��ʱ�ŷ���ջ
			return -1;//�ŷ��Ƿ�
		}

		stepStack.pop();//��ջ������FitRules��step
		stepStack.pop();//��ջ��������һ����ʱstep
		stepStack.push(tStep);//��ʱ�ŷ�ѹջ

		//����
		MoveChess();

		count = -1;//�������

		return 1;//�ŷ�����
	}

	return 0;
}

/**
 * OkMove - ȷ���ŷ�
 */
INT CSurakarta::OkMove(char *denCmd)
{
	Step tStep;
		
	denCmd[0] = '\0';//Ĭ�Ͽ�����

	if (count == -1)//�������
	{
		tStep = stepStack.top();//��ȡ�����ŷ�

		//�������
		sprintf(denCmd, "move %c%c%c%c", tStep.start.x + BX, tStep.start.y + BY, tStep.end.x + BX, tStep.end.y + BY);

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
INT CSurakarta::CancelMove()
{
	Step tStep;

	if (count == 0)//�����룬������
		return 0;

	tStep = stepStack.top();//��ȡ�ŷ�
	stepStack.pop();//�ŷ���ջ

	//�ָ����
	IsChess(tStep.end)
	{
		if (tStep.flagE)//�����ŷ�
			board[tStep.end.x][tStep.end.y] = NEXTPLAYER(tStep.side);
		else
			board[tStep.end.x][tStep.end.y] = EMPTY;
	}

	//�ָ�ѡ��
	IsChess(tStep.start)
	{
		board[tStep.start.x][tStep.start.y] = tStep.side;
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
bool CSurakarta::FitRules()
{
	Step tStep = stepStack.top();//��ȡ�ŷ�

	if (tStep.start.x < 0 || tStep.start.x >= 6 || tStep.start.y < 0 || tStep.start.y >= 6)
		return false;
	if (tStep.end.x < 0 || tStep.end.x >= 6 || tStep.end.y < 0 || tStep.end.y >= 6)
		return false;
	if (board[tStep.start.x][tStep.start.y] != tStep.side)
		return false;
	if (board[tStep.end.x][tStep.end.y] == tStep.side)//���Ϊ������
		return false;
	else if (board[tStep.end.x][tStep.end.y] == NEXTPLAYER(tStep.side))//���Ϊ�Է���
	{
		if (SearchWay(tStep))//�ɳ�
			return true;
		else
			return false;
	}
	else//���Ϊ��
	{
		if (pow(tStep.end.x - tStep.start.x, 2) + pow(tStep.end.y - tStep.start.y, 2) >= 4)//dx^2+dy^2>=4���ƶ�����һ��
			return false;
	}
	return true;
}

/**
 * WinOrLose - ���̲���
 */
bool CSurakarta::WinOrLose()
{
	int numBlc, numWht;

	numBlc = numWht = 0;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (board[i][j] == BLACK)
			{
				numBlc++;
			}
			else if (board[i][j] == WHITE)
			{
				numWht++;
			}
		}
	}
	if (numBlc == 0 || numWht == 0)//һ������
	{
		PostMessage(hWnd, GM_WINLOSE, (WPARAM)(StepNum[BLACK] << 16) + StepNum[WHITE], (LPARAM)(numBlc == 0 ? WHITE : BLACK));
		return true;
	}
	return false;
}

/**
* MoveChess - ��������
*/
VOID CSurakarta::MoveChess()
{
	Step tStep = stepStack.top();//��ȡ��ʱ�ŷ�
	stepStack.pop();//��ʱ�ŷ���ջ

	//����
	board[tStep.start.x][tStep.start.y] = EMPTY;//ѡ��
	if (board[tStep.end.x][tStep.end.y] == NEXTPLAYER(tStep.side))//����
	{
		//�����ŷ��Ϸ����ж�ʱ��������·��
		tStep.flagE = true;//�����ŷ����ӱ��

		//���ų��Ӷ���
		PlayMovie();

		board[tStep.end.x][tStep.end.y] = tStep.side;//����

		//���ų�����Ч
		PlaySnd(2);
	}
	else//����
	{
		board[tStep.end.x][tStep.end.y] = tStep.side;//����

		//����������Ч
		PlaySnd(1);
	}

	stepStack.push(tStep);//�����ŷ�ѹջ

	//ˢ������
	InvalidateRect(hWnd, &rtBoard, FALSE);
	UpdateWindow(hWnd);
}

/**
* SearchWay - ��������·�߲�����
*/
bool CSurakarta::SearchWay(Step sp)
{
	Point first, second;
	int deplayer = NEXTPLAYER(sp.side);
	int SmaID[2] = { -1, -1 };
	int BigID[2] = { -1, -1 };
	int fx, fy;
	int k, i, j;
	bool circle;
	int id;

	first = sp.start;
	second = sp.end;

	//��¼�����ڹ����λ��
	for (k = 0, i = 0, j = 0; k < 28; k++)
	{
		if (SmaWay[k].x == first.x &&SmaWay[k].y == first.y)
			SmaID[i++] = k;
		if (BigWay[k].x == first.x &&BigWay[k].y == first.y)
			BigID[j++] = k;
	}

	//��������·��
	for (i = 0; i < 2; i++)
	{
		if (SmaID[i] != -1)//С�����
		{
			//����
			circle = false;
			id = (SmaID[i] + 1) % 28;//+1(mod28)
			do{
				fx = SmaWay[id].x;
				fy = SmaWay[id].y;
				if (IsCorner(fx, fy))//���㣬������
					circle = true;
				else if (board[fx][fy] != EMPTY && (fx != first.x || fy != first.y))
				{
					if (board[fx][fy] == deplayer && second.x == fx && second.y == fy && circle)
					{
						SaveWay(SmaWay, SmaID[i], id, true);
						return true;
					}
					break;
				}
				id = (id + 1) % 28;//+1(mod28)
			} while (id != SmaID[i]);

			//����
			circle = false;
			id = (SmaID[i] + 27) % 28;
			do{
				fx = SmaWay[id].x;
				fy = SmaWay[id].y;
				if (IsCorner(fx, fy))//���㣬������
					circle = true;
				else if (board[fx][fy] != EMPTY && (fx != first.x || fy != first.y))
				{
					if (board[fx][fy] == deplayer && second.x == fx && second.y == fy && circle)
					{
						SaveWay(SmaWay, SmaID[i], id, false);
						return true;
					}
					break;
				}
				id = (id + 27) % 28;//-1(mod28)
			} while (id != SmaID[i]);
		}
	}
	for (i = 0; i < 2; i++)
	{
		if (BigID[i] != -1)//�󻡹��
		{
			//����
			circle = false;
			id = (BigID[i] + 1) % 28;//+1(mod28)
			do{
				fx = BigWay[id].x;
				fy = BigWay[id].y;
				if (IsCorner(fx, fy))//���㣬������
					circle = true;
				else if (board[fx][fy] != EMPTY && (fx != first.x || fy != first.y))
				{
					if (board[fx][fy] == deplayer && second.x == fx && second.y == fy && circle)
					{
						SaveWay(BigWay, BigID[i], id, true);
						return true;
					}
					break;
				}
				id = (id + 1) % 28;//+1(mod28)
			} while (id != BigID[i]);

			//����
			circle = false;
			id = (BigID[i] + 27) % 28;//-1(mod28)
			do{
				fx = BigWay[id].x;
				fy = BigWay[id].y;
				if (IsCorner(fx, fy))//���㣬������
					circle = true;
				else if (board[fx][fy] != EMPTY  && (fx != first.x || fy != first.y))
				{
					if (board[fx][fy] == deplayer && second.x == fx && second.y == fy && circle)
					{
						SaveWay(BigWay, BigID[i], id, false);
						return true;
					}
					break;
				}
				id = (id + 27) % 28;//-1(mod28)
			} while (id != BigID[i]);
		}
	}
	return false;
}

/**
 * SaveWay - �������·��
 */
VOID CSurakarta::SaveWay(Point *way, int start, int end, bool dir)
{
	MoveWay.clear();//�������
	for (int i = start; i != end;)
	{
		MoveWay.push_back(way[i]);
		dir ? i++ : i--;
		i = (i + 28) % 28;
	}
	MoveWay.push_back(way[end]);
}

/**
* PlayMovie - ���ų��Ӷ���
*/
VOID CSurakarta::PlayMovie()
{
	vector<Point> ptWay;
	vector<Point>::iterator iter;

	SavePtWay(&ptWay);
	ptMovie.side = player;
	for (iter = ptWay.begin(); iter != ptWay.end(); iter++)
	{
		ptMovie.x = iter->x;
		ptMovie.y = iter->y;
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//Sleep(20);//����ѭ������sleep
	}
	ptMovie.x = ptMovie.y = -1;

	return;
}

/**
 * SavePtWay - ���ɳ��Ӷ����켣
 */
VOID CSurakarta::SavePtWay(vector<Point> *ptWay)
{
	PointF pt;
	vector<PointF> vpt;//�����ƶ�·������
	vector<PointF>::iterator fIter;
	vector<Point>::iterator iter;
	int r;//���İ뾶
	double aleph;

	for (iter = MoveWay.begin(); iter != MoveWay.end() - 1; iter++)
	{
		if (IsCorner((iter + 1)->x, (iter + 1)->y))//���ϻ���
		{
			//����㼰Բ�Ķ���
			if ((iter + 1)->x < iter->x)//o.x<s.x
			{
				if ((iter + 1)->y == 0)//����
				{
					for (int i = 1; i <= 15; i++)//���Ͻ���ʱ�뻡
					{
						r = iter->x - (iter + 1)->x;
						aleph = i * 0.1 * PI;// 3/2 PI �Ļ���10��
						pt.x = (iter + 1)->x + r * cos(aleph);
						pt.y = (iter + 1)->y - r * sin(aleph);
						vpt.push_back(pt);
					}
				}
				else if ((iter + 1)->y == 5)//�ױ�
				{
					for (int i = 1; i <= 15; i++)//���½�˳ʱ�뻡
					{
						r = iter->x - (iter + 1)->x;
						aleph = i * 0.1 * PI;// 3/2 PI �Ļ���10��
						pt.x = (iter + 1)->x + r * cos(aleph);
						pt.y = (iter + 1)->y + r * sin(aleph);
						vpt.push_back(pt);
					}
				}
			}
			else if ((iter + 1)->x > iter->x)//o.x>s.x
			{
				if ((iter + 1)->y == 0)//����
				{
					for (int i = 1; i <= 15; i++)//���Ͻ�˳ʱ�뻡
					{
						r = (iter + 1)->x - iter->x;
						aleph = i * 0.1 * PI;// 3/2 PI �Ļ���10��
						pt.x = (iter + 1)->x - r * cos(aleph);
						pt.y = (iter + 1)->y - r * sin(aleph);
						vpt.push_back(pt);
					}
				}
				if ((iter + 1)->y == 5)//�ױ�
				{
					for (int i = 1; i <= 15; i++)//���½���ʱ�뻡
					{
						r = (iter + 1)->x - iter->x;
						aleph = i * 0.1 * PI;// 3/2 PI �Ļ���10��
						pt.x = (iter + 1)->x - r * cos(aleph);
						pt.y = (iter + 1)->y + r * sin(aleph);
						vpt.push_back(pt);
					}
				}
			}
			else if ((iter + 1)->y < iter->y)//o.y<s.y
			{
				if ((iter + 1)->x == 0)//���
				{
					for (int i = 1; i <= 15; i++)//���Ͻ�˳ʱ�뻡
					{
						r = iter->y - (iter + 1)->y;
						aleph = i * 0.1 * PI;// 3/2 PI �Ļ���10��
						pt.x = (iter + 1)->x - r * sin(aleph);
						pt.y = (iter + 1)->y + r * cos(aleph);
						vpt.push_back(pt);
					}
				}
				if ((iter + 1)->x == 5)//�ұ�
				{
					for (int i = 1; i <= 15; i++)//���Ͻ���ʱ�뻡
					{
						r = iter->y - (iter + 1)->y;
						aleph = i * 0.1 * PI;// 3/2 PI �Ļ���10��
						pt.x = (iter + 1)->x + r * sin(aleph);
						pt.y = (iter + 1)->y + r * cos(aleph);
						vpt.push_back(pt);
					}
				}
			}
			else if ((iter + 1)->y > iter->y)//o.y>s.y
			{
				if ((iter + 1)->x == 0)//���
				{
					for (int i = 1; i <= 15; i++)//���½���ʱ�뻡
					{
						r = (iter + 1)->y - iter->y;
						aleph = i * 0.1 * PI;// 3/2 PI �Ļ���10��
						pt.x = (iter + 1)->x - r * sin(aleph);
						pt.y = (iter + 1)->y - r * cos(aleph);
						vpt.push_back(pt);
					}
				}
				if ((iter + 1)->x == 5)//�ұ�
				{
					for (int i = 1; i <= 15; i++)//���½�˳ʱ�뻡
					{
						r = (iter + 1)->y - iter->y;
						aleph = i * 0.1 * PI;// 3/2 PI �Ļ���10��
						pt.x = (iter + 1)->x + r * sin(aleph);
						pt.y = (iter + 1)->y - r * cos(aleph);
						vpt.push_back(pt);
					}
				}
			}
			iter++;
		}
		else if (iter->x == (iter + 1)->x &&iter->y == (iter + 1)->y - 1)//����
		{
			for (int i = 1; i <= 2; i++)
			{
				pt.x = iter->x;
				pt.y = iter->y + 0.5 * i;
				vpt.push_back(pt);
			}
		}
		else if (iter->x == (iter + 1)->x &&iter->y == (iter + 1)->y + 1)//����
		{
			for (int i = 1; i <= 2; i++)
			{
				pt.x = iter->x;
				pt.y = iter->y - 0.5 * i;
				vpt.push_back(pt);
			}
		}
		else if (iter->x == (iter + 1)->x - 1 && iter->y == (iter + 1)->y)//����
		{
			for (int i = 1; i <= 2; i++)
			{
				pt.x = iter->x + 0.5 * i;
				pt.y = iter->y;
				vpt.push_back(pt);
			}
		}
		else if (iter->x == (iter + 1)->x + 1 && iter->y == (iter + 1)->y)//����
		{
			for (int i = 1; i <= 2; i++)
			{
				pt.x = iter->x - 0.5 * i;
				pt.y = iter->y;
				vpt.push_back(pt);
			}
		}
		else if (iter->x == (iter + 1)->x - 1 && iter->y == (iter + 1)->y - 1)//����
		{
			for (int i = 1; i <= 2; i++)
			{
				pt.x = iter->x + 0.5 * i;
				pt.y = iter->y + 0.5 * i;
				vpt.push_back(pt);
			}
		}
		else if (iter->x == (iter + 1)->x + 1 && iter->y == (iter + 1)->y + 1)//����
		{
			for (int i = 1; i <= 2; i++)
			{
				pt.x = iter->x - 0.5 * i;
				pt.y = iter->y - 0.5 * i;
				vpt.push_back(pt);
			}
		}
		else if (iter->x == (iter + 1)->x - 1 && iter->y == (iter + 1)->y + 1)//����
		{
			for (int i = 1; i <= 2; i++)
			{
				pt.x = iter->x + 0.5 * i;
				pt.y = iter->y - 0.5 * i;
				vpt.push_back(pt);
			}
		}
		else if (iter->x == (iter + 1)->x + 1 && iter->y == (iter + 1)->y - 1)//����
		{
			for (int i = 1; i <= 2; i++)
			{
				pt.x = iter->x - 0.5 * i;
				pt.y = iter->y + 0.5 * i;
				vpt.push_back(pt);
			}
		}
	}

	Point ptw;
	//��������ת��Ϊ��������
	for (fIter = vpt.begin(); fIter != vpt.end(); fIter++)
	{
		ptw.x = (int)((fIter->x + 3) * side / 11);
		ptw.y = (int)((fIter->y + 3) * side / 11);
		ptWay->push_back(ptw);
	}
	return;
}

