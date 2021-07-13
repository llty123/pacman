#pragma once
#include"framework.h"
#include <wtypes.h>
#include<time.h>
#include "GMap.h"

#define PLAYERSPEED 4	//玩家速度
#define ENEMYSPEED 0	//敌人速度
#define LEGCOUNTS 5		//敌人腿的数量
#define DISTANCE 10		//图形范围
#define BLUE_ALERT	8	//蓝色警戒范围
#define D_OFFSET 2		//绘图误差
#define RD (DISTANCE+D_OFFSET) //绘图范围


//方向枚举
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
	TWARDS twcommand;//方向指令缓存
	int dRow;//物体所在逻辑矩阵的行
	int dArray;//物体所在逻辑矩阵的列
	int speed;//物体的速度
	TWARDS tw;//朝向
	int frame;//帧数
	POINT point;//中心坐标

	bool Collision();//逻辑碰撞检测
	bool Achive();//判断物体是否到达逻辑坐标位置
	virtual void AchieveCtrl();//到达逻辑点后更新数据
	int PtTransfrom(int k);//将物体在屏幕上的坐标转变为行列坐标

	
	

	

	
	
public:
	void virtual SetPosition(int Row, int Array);
	void virtual DrawBlank(HDC& hdc);
	void virtual Draw(HDC& hdc) = 0;//绘制对象
	static Gmap* pStage;//指向地图类的指针，使所有物体可以使用同样的地图
	Gobject(int Row, int Array)//对物体类进行初始化
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
	void virtual action() = 0;//防止物体类被实例化
	int GetRow()//获得物体所在行数
	{
		return dRow;
	}
	int GetArray()//获得物体所在列数
	{
		return dArray;
	}

};

class PacMan :public Gobject//"大嘴“的类
{
protected:
	//virtual void AchiveCtrl();
public:

	POINT GetPos();
	TWARDS GetTw()//返回大嘴的方向状态
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

class Enermy :public Gobject//敌人的类
{
protected:
	void Catch();
	void virtual MakeDecision(bool b) = 0;//敌人的行为函数
	COLORREF color;
public:

	static PacMan* player;//指向玩家的指针，使敌人可以知晓”大嘴“的位置信息
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



class RedOne :public Enermy//松散型敌人(随机移动）
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

class BlueOne :public Enermy//守卫型敌人（追踪玩家）
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

class YellowOne :public Enermy//扰乱型敌人（与守卫型相似，但行为模式不同）
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