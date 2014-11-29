#include "CTime.h"
#include "GameType.h"


INT CTime::UpdateTime(char *strBlcTime,char *strWhtTime)
{
	BYTE player;//��ǰ�ֵ��ĸ�ѡ�־ͼ��ĸ�ѡ�ֵ�ʱ��
	player=CT_GetCurPlayer();
	char *time;
	int len=strlen("Time: ");
	if(player==BLACK)//����ڷ�ʱ��
	{
		time=strBlcTime;
		strcpy(time,"Time: ");
		blcTime.sumTime=GetTickCount()-startTime-whtTime.sumTime;
		timeDeal(BLACK);
		if(blcTime.hours>0)
		{
			itoa(blcTime.hours,&time[len],10);
			len=strlen(time);
			time[len++]=':';
		}
		if(blcTime.minutes<10)//����С��10������
		{			
			time[len++]='0';
			time[len++]='0'+blcTime.minutes;
		}
		else
		{
			itoa(blcTime.minutes,&time[len],10);
			len=strlen(time);
		}
		time[len++]=':';
		if(blcTime.seconds<10)//����С��10������
		{			
			time[len++]='0';
			time[len++]='0'+blcTime.seconds;
		}
		else
		{
			itoa(blcTime.seconds,&time[len],10);
			len=strlen(time);
		}
		time[len]='\0';
	}
	else//����׷�ʱ��									
	{
		time=strWhtTime;
		strcpy(time,"Time: ");
		whtTime.sumTime=GetTickCount()-startTime-blcTime.sumTime;
		timeDeal(WHITE);
		if(whtTime.hours>0)
		{
			itoa(whtTime.hours,&time[len],10);
			len=strlen(time);
			time[len++]=':';
		}
		if(whtTime.minutes<10)//����С��10������
		{			
			time[len++]='0';
			time[len++]='0'+whtTime.minutes;
		}
		else
		{
			itoa(whtTime.minutes,&time[len],10);
			len=strlen(time);
		}
		time[len++]=':';
		if(whtTime.seconds<10)//����С��10������
		{			
			time[len++]='0';
			time[len++]='0'+whtTime.seconds;
		}
		else
		{
			itoa(whtTime.seconds,&time[len],10);
			len=strlen(time);
		}
		time[len]='\0';
	}		
	return player;
}

VOID CTime::timeDeal(int player)	//ʱ��ת��
{
	if(player==BLACK)
	{
		blcTime.seconds=blcTime.sumTime/1000%60;
		blcTime.minutes=blcTime.sumTime/60000%60;
		blcTime.hours=blcTime.sumTime/3600000;
	}
	else if(player==WHITE)
	{
		whtTime.seconds=whtTime.sumTime/1000%60;
		whtTime.minutes=whtTime.sumTime/60000%60;
		whtTime.hours=whtTime.sumTime/3600000;
	}
}

VOID CTime::StartTimer(HWND hTimerWnd)
{
	hSetWnd=hTimerWnd;
	blcTime.sumTime=whtTime.sumTime=0;//ʱ������
	killed=false;
	SetTimer(hSetWnd,0,250,NULL);//������ʱ��	���뼶��ʱ����˫����������ʱ���ܲ���׼ȷ��
	startTime=GetTickCount();
}

//ֹͣ��ʱ
bool CTime::StopTimer()
{	
	if(killed)
		return false;
	if(!KillTimer(hSetWnd,0))//���ж�KillTimer��������ֵ��KillTimer������ִ�У�
	{
		ErrorBox("KillTimer failed");		
		return false;
	}
	killed=true;
	return true;
}

LastTime CTime::GetTime(int side)
{
	if(side==BLACK)
		return blcTime;
	else
		return whtTime;
}