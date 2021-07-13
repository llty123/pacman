#pragma once
#include"framework.h"
#include <wtypes.h>
#include<time.h>
#include "GMap.h"

#define PLAYERSPEED 4	//����ٶ�
#define ENEMYSPEED 0	//�����ٶ�
#define LEGCOUNTS 5		//�����ȵ�����
#define DISTANCE 10		//ͼ�η�Χ
#define BLUE_ALERT	8	//��ɫ���䷶Χ
#define D_OFFSET 2		//��ͼ���
#define RD (DISTANCE+D_OFFSET) //��ͼ��Χ


//����ö��
enum TWARDS {LEFT,RIGHT,UP,DOWN,OVER};

//class POINT
//{
//public:
//
//	int x;
//	int y;
//};

class Gobject
{
protected:
	int mX;
	int mY;
	TWARDS twcommand;//����ָ���
	int dRow;//���������߼��������
	int dArray;//���������߼��������
	int speed;//������ٶ�
	TWARDS tw;//����
	int frame;//֡��
	POINT point;//��������

	bool Collision();//�߼���ײ���
	bool Achive();//�ж������Ƿ񵽴��߼�����λ��
	virtual void AchieveCtrl();//�����߼�����������
	int PtTransfrom(int k);//����������Ļ�ϵ�����ת��Ϊ��������

	
	

	

	
	
public:
	void virtual SetPosition(int Row, int Array);
	void virtual DrawBlank(HDC& hdc);
	void virtual Draw(HDC& hdc) = 0;//���ƶ���
	static Gmap* pStage;//ָ���ͼ���ָ�룬ʹ�����������ʹ��ͬ���ĵ�ͼ
	Gobject(int Row, int Array)//����������г�ʼ��
	{
		frame = 1;
		pStage = NULL;
		this->dRow = Row;
		this->dArray = Array;
		this->point.y = dRow * pStage->LD + pStage->LD / 2;
		this->point.x = dArray * pStage->LD + pStage->LD / 2;
		this->mX = point.x;
		this->mY = point.y;
		//tw = LEFT;
		//twcommand = LEFT;
	}
	void virtual action() = 0;//��ֹ�����౻ʵ����
	int GetRow()//���������������
	{
		return dRow;
	}
	int GetArray()//���������������
	{
		return dArray;
	}

};

class PacMan :public Gobject//"���조����
{
protected:
	//virtual void AchiveCtrl();
public:

	POINT GetPos();
	TWARDS GetTw()//���ش���ķ���״̬
	{
		return tw;
	}
	bool Win();
	void Draw(HDC& hdc);
	void SetTwCommands(TWARDS command);
	void Over();

	PacMan(int x, int y) :Gobject(x, y)
	{
		this->speed = PLAYERSPEED;
		twcommand = tw = LEFT;
	}
	void action();

	void AchieveCtrl();

	

	



	
	
};

class Enermy :public Gobject//���˵���
{
protected:
	void Catch();
	void virtual MakeDecision(bool b) = 0;//���˵���Ϊ����
	COLORREF color;
public:

	static PacMan* player;//ָ����ҵ�ָ�룬ʹ���˿���֪�������조��λ����Ϣ
	void virtual Draw(HDC& hdc);
	Enermy(int x, int y) :Gobject(x, y)
	{
		this->speed = ENEMYSPEED;
		tw = LEFT;
		twcommand = UP;
		color = RGB(0, 0, 0);
	}

	void virtual action();
	
};



class RedOne :public Enermy//��ɢ�͵���(����ƶ���
{
protected:
	void MakeDecision(bool b);
public:
	//void Draw(HDC& hdc);
	RedOne(int x, int y) :Enermy(x, y)
	{
		color = RGB(255, 0, 0);
	}

};

class BlueOne :public Enermy//�����͵��ˣ�׷����ң�
{
protected:
	void MakeDecision(bool b);
public:
	//void Draw(HDC& hdc);
	BlueOne(int x, int y) :Enermy(x, y)
	{
		color = RGB(0, 0, 255);
	}

};

class YellowOne :public Enermy//�����͵��ˣ������������ƣ�����Ϊģʽ��ͬ��
{
protected:
	void MakeDecision(bool b);
public:
	//void Draw(HDC& hdc);
	YellowOne(int x, int y) :Enermy(x, y)
	{
		color = RGB(200,200,100);
	}

};