#pragma once
#include"framework.h"
#include <list>
#include<WinUser.h>
#include <windef.h>

#define MAPLENTH 19//地图大小
#define P_ROW 10
#define P_ARRAY 9
#define E_ROW 8
#define E_ARRAY 9


using std::list;

class Gmap
{
protected:
	static int LD;//障碍物尺寸
	static int PD;//豆子半径
	void InitOP();//敌我双方出现的位置没有豆子出现
	bool map_data[MAPLENTH][MAPLENTH];//障碍物逻辑地图点阵
	bool pea_map_data[MAPLENTH][MAPLENTH];//豆子逻辑地图点阵
	COLORREF color;
public:
	void DrawMap(HDC& hdc);//绘制地图
	void DrawPeas(HDC& hdc);//绘制豆子
	//virtual ~Gmap();
	Gmap()
	{
		color = RGB(255, 0, 0);
	}

	friend class Gobject;//Gobject物体类可以访问
	friend class PacMan;//允许“大嘴”访问豆子地图
};

//第一关
class Stage_1 :public Gmap
{
private:
	bool static initData[MAPLENTH][MAPLENTH];
	

public:
	Stage_1();
};



