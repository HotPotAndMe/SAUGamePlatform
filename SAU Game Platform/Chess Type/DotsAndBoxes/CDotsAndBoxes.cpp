#include "CDotsAndBoxes.h"


VOID CDotsAndBoxes::GetConfig()
{
	char filename[MAX_PATH];
	sprintf(filename, "%s\\Config.ini", LibPath);
	BE = GetPrivateProfileInt("COORDINATE", "E", 'A', filename);
	CChess::GetConfig();
}

CDotsAndBoxes::CDotsAndBoxes(HINSTANCE hInst, HWND hWnd, char *LibPath)
{
	this->hInst = hInst;
	this->hWnd = hWnd;
	strncpy(this->LibPath, LibPath, MAX_PATH - 1);
	hFont = NULL;

	//��ȡ����
	GetConfig();

	//��������DC
	HDC hDC = GetDC(hWnd);
	hBlcDC = CreateCompatibleDC(hDC);
	hWhtDC = CreateCompatibleDC(hDC);
	hPtDC = CreateCompatibleDC(hDC);

	InitGame();
}

CDotsAndBoxes::~CDotsAndBoxes()
{
	if (hFont != NULL)
		DeleteObject(hFont);
	DeleteDC(hBlcDC);
	DeleteDC(hWhtDC);
	DeleteDC(hPtDC);
}

/**
 * SetBoard - �������̲���
 * @rtBoard:	�����ڴ��ڿͻ�����λ��
 */
VOID CDotsAndBoxes::SetBoard(RECT rtBoard)
{
	//�������̲���
	this->rtBoard = rtBoard;
	side = rtBoard.right - rtBoard.left + 1;
	d = (int)(side / 7);
	size = (int)(d*0.8);
	pWidth = (int)(side / 100);
	pR = (int)(side / 70);
	pL = 2 * pR;
\
	//���ÿ̶�����
	if (hFont != NULL)
		DeleteObject(hFont);
	fWidth = d / 3;
	fHeight = d * 2 / 3;
	hFont = CreateSimpleFont(fWidth, fHeight);

	//��������Ԫ��
	DrawChess();
}

/**
 * DrawBoard - ��������
 * @hDC:	������DC
 */
VOID CDotsAndBoxes::DrawBoard(HDC hDC)
{
	int i,j,k;
	HFONT hOldFont;

	// ���Ƶ�
	for(i=0;i<6;i++)
	{		
		for(j=0;j<6;j++)
		{
			BitBlt(hDC, rtBoard.left + (side + side*j) / 7 - pR, rtBoard.top + (side + side*i) / 7 - pR, pL, pL, hPtDC, 0, 0, SRCAND);
		}
	}

	//���ÿ̶����塢��ɫ
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextColor(hDC, RGB(0, 0, 0));

	char letter[2] = { 0 }, number[3] = { 0 };

	//���ƿ̶�
	for (i = 1; i <= 6; i++)
	{
		letter[0] = 'A' + i - 1;
		itoa(i, number, 10);
		TextOut(hDC, rtBoard.left + side*i / 7 - fWidth / 2, rtBoard.top + side / 14 - fHeight / 2, letter, 1);
		TextOut(hDC, rtBoard.left + side*i / 7 - fWidth / 2, rtBoard.top + side * 13 / 14 - fHeight / 2, number, 1);
		TextOut(hDC, rtBoard.left + side / 14 - fWidth / 2, rtBoard.top + side*i / 7 - fHeight / 2, letter, 1);
		TextOut(hDC, rtBoard.left + side * 13 / 14 - fWidth / 2, rtBoard.top + side*i / 7 - fHeight / 2, number, 1);
	}
	
	// ������
	HPEN hBlcPen,hWhtPen,hPen;
	hBlcPen = CreatePen(PS_SOLID, pWidth, RGB(0, 0, 0));
	hWhtPen = CreatePen(PS_SOLID, pWidth, RGB(255, 255, 255));
	hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

	//����
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 5; j++)
		{
			if (line[0][i][j] == BLACK)
			{
				SelectObject(hDC, hBlcPen);
				MoveToEx(hDC, (int)(rtBoard.left + side*(j + 1) / 7), (int)(rtBoard.top + side*(i + 1) / 7), NULL);
				LineTo(hDC, (int)(rtBoard.left + side*(j + 2) / 7), (int)(rtBoard.top + side*(i + 1) / 7));
			}
			else if (line[0][i][j] == WHITE)
			{
				SelectObject(hDC, hWhtPen);
				MoveToEx(hDC, (int)(rtBoard.left + side*(j + 1) / 7), (int)(rtBoard.top + side*(i + 1) / 7), NULL);
				LineTo(hDC, (int)(rtBoard.left + side*(j + 2) / 7), (int)(rtBoard.top + side*(i + 1) / 7));
			}
		}
	}
	//����
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 5; j++)
		{
			if (line[1][i][j] == BLACK)
			{
				SelectObject(hDC, hBlcPen);
				MoveToEx(hDC, (int)(rtBoard.left + side*(i + 1) / 7), (int)(rtBoard.top + side*(j + 1) / 7), NULL);
				LineTo(hDC, (int)(rtBoard.left + side*(i + 1) / 7), (int)(rtBoard.top + side*(j + 2) / 7));
			}
			else if (line[1][i][j] == WHITE)
			{
				SelectObject(hDC, hWhtPen);
				MoveToEx(hDC, (int)(rtBoard.left + side*(i + 1) / 7), (int)(rtBoard.top + side*(j + 1) / 7), NULL);
				LineTo(hDC, (int)(rtBoard.left + side*(i + 1) / 7), (int)(rtBoard.top + side*(j + 2) / 7));
			}

		}
	}

	// ���Ƹ�
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			if (box[i][j] == BLACK)
			{
				BitBlt(hDC, (int)(rtBoard.left + side*(i * 2 + 3) / 14 - size / 2), (int)(rtBoard.top + side*(j * 2 + 3) / 14 - size / 2), size, size, hBlcDC, 0, 0, SRCCOPY);
			}
			else if (box[i][j] == WHITE)
			{
				BitBlt(hDC, (int)(rtBoard.left + side*(i * 2 + 3) / 14 - size / 2), (int)(rtBoard.top + side*(j * 2 + 3) / 14 - size / 2), size, size, hWhtDC, 0, 0, SRCCOPY);
			}
		}
	}

	//������ʾ���
	if (!stepStack.empty())
	{
		Step tStep = stepStack.top();
		SelectObject(hDC, hPen);
		IsLine(tStep.line)
		{
			MoveToEx(hDC, (int)(rtBoard.left + side*(tStep.start.x + 1) / 7), (int)(rtBoard.top + side*(tStep.start.y + 1) / 7), NULL);
			LineTo(hDC, (int)(rtBoard.left + side*(tStep.end.x + 1) / 7), (int)(rtBoard.top + side*(tStep.end.y + 1) / 7));
		}
		else IsChess(tStep.start)
		{
			MoveToEx(hDC, (int)(rtBoard.left + (side + side*tStep.start.x) / 7 - pR), (int)(rtBoard.top + (side + side*tStep.start.y) / 7), NULL);
			LineTo(hDC, (int)(rtBoard.left + (side + side*tStep.start.x) / 7 + pR), (int)(rtBoard.top + (side + side*tStep.start.y) / 7));
			MoveToEx(hDC, (int)(rtBoard.left + (side + side*tStep.start.x) / 7), (int)(rtBoard.top + (side + side*tStep.start.y) / 7 - pR), NULL);
			LineTo(hDC, (int)(rtBoard.left + (side + side*tStep.start.x) / 7), (int)(rtBoard.top + (side + side*tStep.start.y) / 7 + pR));
		}
	}

	DeleteObject(hBlcPen);
	DeleteObject(hWhtPen);
	DeleteObject(hPen);
}

/**
 * DrawChess - ��������Ԫ��
 */
bool CDotsAndBoxes::DrawChess()
{
	HBITMAP hBlcBmp, hWhtBmp, hPtBmp;
	hBlcBmp = CreateCompatibleBitmap(hBlcDC, d, d);
	hWhtBmp = CreateCompatibleBitmap(hWhtDC, d, d);
	hPtBmp = CreateCompatibleBitmap(hPtDC, pL, pL);

	SelectObject(hBlcDC, hBlcBmp);
	SelectObject(hWhtDC, hWhtBmp);
	SelectObject(hPtDC, hPtBmp);

	HBRUSH hBlcBrush, hWhtBrush, hOldBrush;
	LOGBRUSH lb;

	lb.lbColor = RGB(0, 0, 0);
	lb.lbStyle = BS_SOLID;
	hBlcBrush = CreateBrushIndirect(&lb);
	hOldBrush=(HBRUSH)SelectObject(hBlcDC,hBlcBrush);
	PatBlt(hBlcDC,0,0,d,d,PATCOPY);

	lb.lbColor=RGB(255,255,255);
	hWhtBrush=CreateBrushIndirect(&lb);
	hOldBrush=(HBRUSH)SelectObject(hWhtDC,hWhtBrush);
	PatBlt(hWhtDC,0,0,d,d,PATCOPY);

	DrawPoint(hPtDC, pR, RGB(0, 0, 0),RGB(255, 255, 255));

	DeleteObject(hBlcBmp);
	DeleteObject(hWhtBmp);
	DeleteObject(hBlcBrush);
	DeleteObject(hWhtBrush);
	DeleteObject(hPtBmp);

	return true;
}

/**
 * PlaySnd - ������Ч
 * @tag:	��Ч��ǩ
 */
bool CDotsAndBoxes::PlaySnd(int tag)
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
	case 2://�������Ч
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\�����.wav");
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
VOID CDotsAndBoxes::InitGame()
{
	memset(StepNum, 0, sizeof(StepNum));
	player = BLACK;
	lineNum = 0;
	count = 0;
	CleanStack(stepStack);//����ŷ�ջ
	InitBoard();//��ʼ������
}

/**
 * InitBoard - ��ʼ������
 */
VOID CDotsAndBoxes::InitBoard()
{
	int i, j, k;
	for (k = 0; k<2; k++)
	{
		for (i = 0; i<6; i++)
		{
			for (j = 0; j<5; j++)
			{
				line[k][i][j] = EMPTY;
			}
		}
	}
	for (i = 0; i<5; i++)
	{
		for (j = 0; j<5; j++)
		{
			box[i][j] = EMPTY;
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
BOOL CDotsAndBoxes::ProcessMove(char *moveCmd, char *curCmd, char *denCmd)
{				
	char *res;
	int pos=0,cs;
	Step tStep;
	int len=strlen("move ");
	int lineNum;
	bool winF = false;

	curCmd[0] = denCmd[0] = '\0';//Ĭ�Ͽ���Ϣ

	if ((res = strstr(moveCmd, "move")) == NULL)//Ѱ��move�ؼ���
	{
		return 0;//δ�ҵ���move���ؼ���
	}
	else
	{		
		pos = (res - moveCmd);
		
		//�����ŷ�
		sscanf(moveCmd + len, "%d", &lineNum);//������Ŀ
		if (lineNum >= 10)
			len += 3;
		else
			len += 2;
		pos += len;

		InitStep(tStep);
		tStep.side = player;
		cs = 0;
		do{
			if (cs >= lineNum)//���߹���,���⣺�������һ������
			{
				if (WinOrLose())
				{
					winF = true;
					break;
				}
				strcat(curCmd, "error\n");
				while (cs--)//ȡ��֮ǰ�б�����
				{
					tStep = stepStack.top();//��ȡ��ʱ�ŷ�
					line[tStep.line.k][tStep.line.i][tStep.line.j] = EMPTY;//�ָ�����
					UnpackBox(tStep.line);//�������
					stepStack.pop();//��ʱ�ŷ���ջ
				}
				return -1;//�ŷ��Ƿ�
			}
			tStep.line.k = moveCmd[pos + cs * 3] - BE;
			tStep.line.i = moveCmd[pos + cs * 3 + 1] - BX;
			tStep.line.j = moveCmd[pos + cs * 3 + 2] - BY;
			LineToPoint(tStep);//������ӳ�������
			stepStack.push(tStep);//��ʱ�ŷ�ѹջ

			//�ж��Ƿ���Ϲ���
			if (!FitRules())
			{
				strcat(curCmd, "error\n");
				stepStack.pop();//��ʱ�ŷ���ջ
				while (cs--)//ȡ��֮ǰ�б�����
				{
					tStep = stepStack.top();//��ȡ��ʱ�ŷ�
					line[tStep.line.k][tStep.line.i][tStep.line.j] = EMPTY;//�ָ�����
					UnpackBox(tStep.line);//�������
					stepStack.pop();//��ʱ�ŷ���ջ
				}
				return -1;//�ŷ��Ƿ�
			}

			//����
			line[tStep.line.k][tStep.line.i][tStep.line.j] = player;

			cs++;
		} while (HaveBox(tStep.line));//�������

		if (cs != lineNum)//����������
		{
			strcat(curCmd, "error\n");
			while (cs--)//ȡ��֮ǰ�б�����
			{
				tStep = stepStack.top();//��ȡ��ʱ�ŷ�
				line[tStep.line.k][tStep.line.i][tStep.line.j] = EMPTY;//�ָ�����
				UnpackBox(tStep.line);//�������
				stepStack.pop();//��ʱ�ŷ���ջ
			}
			return -1;//�ŷ��Ƿ�
		}

		//ˢ������
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//����������Ч
		PlaySnd(0);

		//׷���ŷ���ʷ
		ShowStepHis(moveCmd + pos);

		//�������
		strncpy(denCmd, res, len + lineNum * 3);
		sprintf(denCmd+ len + lineNum * 3, "\n");

		//�ۼƲ���
		StepNum[player]++;

		//�ж�ʤ��
		if (winF)
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
BOOL CDotsAndBoxes::OnLButtonDown(int x,int y)
{
	Point point;

	if (count == -1)//������������״̬��count=-1ʱreturn0�����������
		return 0;

	//�Ѵ�������ӳ��Ϊ��������
	point.x = ((x - rtBoard.left) * 7 - side / 2) / side;
	point.y = ((y - rtBoard.top) * 7 - side / 2) / side;
	if (point.x < 0 || point.x >= 6 || point.y < 0 || point.y >= 6)
		return 2;

	return SToS(point);
}

/**
 * SToS - �ֶ��߼�����
 */
BOOL CDotsAndBoxes::SToS(Point point)
{
	Step tStep;

	if (count == 0)//ѡ��
	{
		//����ŷ��Ϸ���
		if (HaveLine(point) == false)//�㲻�������ߣ���Ч
			return -1;

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
		tStep.end = point;//�������

		//����ŷ��Ϸ���
		if (PonitToLine(tStep) == false)//�����㲻��ӳ��Ϊ���ߣ���Ч
			return -1;//�ŷ��Ƿ�

		//����
		line[tStep.line.k][tStep.line.i][tStep.line.j] = player;

		stepStack.pop();//��ʱ�ŷ���ջ
		stepStack.push(tStep);//�����ŷ�ѹջ

		count = -1;//�������

		//ˢ������
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//����������Ч
		PlaySnd(1);

		return 1;//�ŷ�����
	}

	return 0;
}

/**
 * OkMove - ȷ���ŷ�
 */
INT CDotsAndBoxes::OkMove(char *denCmd)
{
	Step tStep;

	denCmd[0] = '\0';//Ĭ�Ͽ�����

	if (count == -1)
	{
		tStep = stepStack.top();//��ȡ�����ŷ�

		//�������
		sprintf(lineList + lineNum * 3, "%c%c%c", tStep.line.k + BE, tStep.line.i + BX, tStep.line.j + BY);
		lineNum++;//����������Ŀ��1

		if (HaveBox(tStep.line))//������Ӳ����֣�����ʾ������ɫ
		{
			//ˢ������
			InvalidateRect(hWnd, &rtBoard, FALSE);
			UpdateWindow(hWnd);
			//���Ų��������Ч
			PlaySnd(2);

			//�ۼƲ���
			StepNum[player]++;

			//�ж�ʤ��,�������һ������
			if (WinOrLose())
			{
				//�������
				sprintf(denCmd, "move %d %s\nend\n", lineNum, lineList);
				//׷���ŷ���ʷ
				ShowStepHis(lineList);
				return 2;//�ֳ�ʤ��
			}
		}
		else
		{
			//�������
			sprintf(denCmd, "move %d %s\n", lineNum, lineList);

			//׷���ŷ���ʷ
			ShowStepHis(lineList);

			//�ۼƲ���
			StepNum[player]++;

			//���廻��
			player = NEXTPLAYER(player);

			lineNum = 0;//���֣���������������
		}

		count = 0;//��������

		return 1;//�ŷ�����
	}
	return 0;
}

/**
 * CancelMove - ȡ���ŷ�
 */
INT CDotsAndBoxes::CancelMove()
{
	Step tStep;

	if (count == 0)//�����룬������
		return 0;

	tStep = stepStack.top();//��ȡ�ŷ�
	stepStack.pop();//�ŷ���ջ

	//�ָ�����
	IsLine(tStep.line)
	{
		line[tStep.line.k][tStep.line.i][tStep.line.j] = EMPTY;
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
bool CDotsAndBoxes::FitRules()
{
	Step tStep = stepStack.top();
	if (tStep.line.k < 0 || tStep.line.k > 1 || tStep.line.i < 0 || tStep.line.i > 5 || tStep.line.j < 0 || tStep.line.j > 4)
		return false;
	if (line[tStep.line.k][tStep.line.i][tStep.line.j] != EMPTY)
		return false;
	return true;
}

/**
 * WinOrLose - ���̲���
 */
bool CDotsAndBoxes::WinOrLose()
{
	int numBlc, numWht, numEmt;//�ڷ����ӣ��׷����ӣ��ո���
	numBlc = numWht = numEmt = 0;
	for (int i = 0; i<5; i++)
	{
		for (int j = 0; j<5; j++)
		{
			if (box[i][j] == BLACK)
			{
				numBlc++;
			}
			else if (box[i][j] == WHITE)
			{
				numWht++;
			}
			else
			{
				numEmt++;
			}
		}
	}
	if (numEmt == 0)//���и��ӱ����񣬲�����Ӷ��߻�ʤ
	{
		SendMessage(hWnd, GM_WINLOSE, (WPARAM)(StepNum[BLACK] << 16) + StepNum[WHITE], (LPARAM)(numBlc > numWht ? BLACK : WHITE));
		return true;
	}
	return false;
}

/**
 * NoLine - �жϵ��Ƿ��ܹ�������
 */
bool CDotsAndBoxes::HaveLine(Point point)
{
	if (point.x > 0)
	{
		if (line[0][point.y][point.x - 1] == EMPTY)
			return true;
	}
	if (point.x < 5)
	{
		if (line[0][point.y][point.x] == EMPTY)
			return true;
	}
	if (point.y > 0)
	{
		if (line[1][point.x][point.y - 1] == EMPTY)
			return true;
	}
	if (point.y < 5)
	{
		if (line[1][point.x][point.y] == EMPTY)
			return true;
	}
	return false;
}

/**
 * HaveBox - �ж������Ƿ��ܹ��������
 */
bool CDotsAndBoxes::HaveBox(LINE line)
{
	bool flag = false;

	// 1~5���ж�������
	if (line.i>0 && this->line[line.k][line.i - 1][line.j] != EMPTY &&this->line[line.k ^ 1][line.j][line.i - 1] != EMPTY &&this->line[line.k ^ 1][line.j + 1][line.i - 1] != EMPTY)
	{
		if (line.k == 0)
		{
			box[line.j][line.i - 1] = player;
		}
		else if (line.k == 1)
		{
			box[line.i - 1][line.j] = player;
		}
		flag = true;
	}
	// 0~4���ж��Ҳ����
	if (line.i<5 && this->line[line.k][line.i + 1][line.j] != EMPTY &&this->line[line.k ^ 1][line.j][line.i] != EMPTY &&this->line[line.k ^ 1][line.j + 1][line.i] != EMPTY)
	{
		if (line.k == 0)
		{
			box[line.j][line.i] = player;
		}
		else if (line.k == 1)
		{
			box[line.i][line.j] = player;
		}
		flag = true;
	}
	return flag;
}

/**
 * UnpackBox - ������߲���ĸ���
 */
void CDotsAndBoxes::UnpackBox(LINE line)
{
	if (line.k == 0)
	{
		box[line.j][line.i - 1] = EMPTY;
		box[line.j][line.i] = EMPTY;
	}
	else if (line.k == 1)
	{
		box[line.i - 1][line.j] = EMPTY;
		box[line.i][line.j] = EMPTY;
	}
}

/**
 * PointToLine - ���ߵ���ֹ������ת��Ϊ�ߵ�λ������
 */
bool CDotsAndBoxes::PonitToLine(Step &step)
{
	int dx, dy;
	dx = step.start.x - step.end.x;
	dy = step.start.y - step.end.y;
	if (dx == 0)
	{
		if (dy == 1)
			step.line.j = step.end.y;
		else if (dy == -1)
			step.line.j = step.start.y;
		else
			return false;
		step.line.k = 1;//����
		step.line.i = step.start.x;
	}
	else if (dx == 1)
	{
		if (dy != 0)
			return false;
		step.line.k = 0;//����
		step.line.i = step.start.y;
		step.line.j = step.end.x;
	}
	else if (dx == -1)
	{
		if (dy != 0)
			return false;
		step.line.k = 0;//����
		step.line.i = step.start.y;
		step.line.j = step.start.x;
	}
	else
		return false;

	return true;
}

/**
 * LineToPoint - ���ߵ�λ������ת��Ϊ�ߵ���ֹ������
 */
bool CDotsAndBoxes::LineToPoint(Step &step)
{
	if (step.line.k == 0)
	{
		step.start.x = step.line.j;
		step.start.y = step.line.i;
		step.end.x = step.start.x + 1;
		step.end.y = step.start.y;
	}
	else if (step.line.k == 1)
	{
		step.start.x = step.line.i;
		step.start.y = step.line.j;
		step.end.x = step.start.x;
		step.end.y = step.start.y + 1;
	}
	else
		return false;

	return true;
}
