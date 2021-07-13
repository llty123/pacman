#include "GMap.h"
#include <wingdi.h>
#define A true		//A表示可以通行
#define B false		//B表示有障碍物

bool Stage_1::initData[MAPLENTH][MAPLENTH] =
{
	B,B,B,B,B,B,B,B,B,A,B,B,B,B,B,B,B,B,B,//0
	B,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,B,//1
	B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//2
	B,A,B,B,A,A,A,A,A,A,A,A,A,A,A,B,B,A,B,//3
	B,A,B,A,A,A,B,B,B,A,B,B,B,A,A,A,B,A,B,//4
	B,A,B,A,A,A,A,A,A,A,A,A,A,A,A,A,B,A,B,//5
	B,A,A,A,A,A,B,B,A,A,A,B,B,A,A,A,A,A,B,//6
	B,A,B,A,A,A,A,A,A,A,A,A,A,A,A,A,B,A,B,//7
	B,A,B,A,A,A,A,A,B,A,B,A,A,A,A,A,B,A,B,//8
	A,A,A,A,A,A,A,A,B,B,B,A,A,A,A,A,A,A,A,// 9
	B,A,B,A,A,A,A,A,A,A,A,A,A,A,A,A,B,A,B,//10
	B,A,B,A,A,B,A,A,A,A,A,A,A,B,A,A,B,A,B,//11
	B,A,B,A,B,B,B,A,A,A,A,A,B,B,B,A,B,A,B,//12
	B,A,A,A,A,B,A,A,A,A,A,A,A,B,A,A,A,A,B,//13
	B,A,B,B,A,A,A,A,A,A,A,A,A,A,A,B,B,A,B,//14
	B,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,B,//15
	B,A,A,A,A,B,B,B,A,B,A,B,B,B,A,A,A,A,B,//16
	B,A,A,A,A,B,A,A,A,A,A,A,A,B,A,A,A,A,B,//17
	B,B,B,B,B,B,B,B,B,A,B,B,B,B,B,B,B,B,B,//18
};

bool Stage_2::initData[MAPLENTH][MAPLENTH] =
{
	B,B,B,B,B,B,B,B,B,A,B,B,B,B,B,B,B,B,B,//0
	B,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,B,//1
	B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//2
	B,A,B,B,A,A,A,A,A,A,A,A,A,A,A,B,B,A,B,//3
	B,A,B,A,A,A,B,B,B,A,B,B,B,A,A,A,B,A,B,//4
	B,A,B,A,A,A,A,A,A,A,A,A,A,A,A,A,B,A,B,//5
	B,A,A,A,A,A,B,B,A,A,A,B,B,A,A,A,A,A,B,//6
	B,A,B,A,A,A,A,A,A,A,A,A,A,A,A,A,B,A,B,//7
	B,A,B,A,A,A,A,A,B,A,B,A,A,A,A,A,B,A,B,//8
	A,A,A,A,A,A,A,A,B,B,B,A,A,A,A,A,A,A,A,// 9
	B,A,B,A,A,A,A,A,A,A,A,A,A,A,A,A,B,A,B,//10
	B,A,B,A,A,B,A,A,A,A,A,A,A,B,A,A,B,A,B,//11
	B,A,B,A,B,B,B,A,A,A,A,A,B,B,B,A,B,A,B,//12
	B,A,A,A,A,B,A,A,A,A,A,A,A,B,A,A,A,A,B,//13
	B,A,B,B,A,A,A,A,A,A,A,A,A,A,A,B,B,A,B,//14
	B,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,B,//15
	B,A,A,A,A,B,B,B,A,B,A,B,B,B,A,A,A,A,B,//16
	B,A,A,A,A,B,B,B,B,B,B,B,B,B,A,A,A,A,B,//17
	B,B,B,B,B,B,B,B,B,A,B,B,B,B,B,B,B,B,B,//18
};

bool Stage_3::initData[MAPLENTH][MAPLENTH] =
{
	B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,//0
	B,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,B,//1
	B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,A,A,A,B,//2
	B,A,B,B,A,A,A,A,A,A,A,A,A,A,A,B,B,A,B,//3
	B,A,B,A,A,A,B,B,B,B,B,B,B,A,A,A,B,A,B,//4
	B,A,B,A,A,B,A,A,A,A,A,A,A,B,A,A,B,A,B,//5
	B,A,A,A,A,A,B,B,B,A,B,B,B,A,A,A,A,A,B,//6
	B,A,B,A,A,A,A,A,A,A,A,A,A,A,A,A,B,A,B,//7
	B,A,B,A,A,B,A,A,B,A,B,A,A,B,A,A,B,A,B,//8
	A,A,A,A,A,B,A,A,B,B,B,A,A,B,A,A,A,A,A,//9
	B,A,B,A,A,B,A,A,A,A,A,A,A,B,A,A,B,A,B,//10
	B,A,B,A,A,B,A,A,A,A,A,A,A,B,A,A,B,A,B,//11
	B,A,B,A,B,B,B,A,A,A,A,A,B,B,B,A,B,A,B,//12
	B,A,B,A,A,B,A,A,A,A,A,A,A,B,A,A,B,A,B,//13
	B,A,B,B,B,B,A,A,A,A,A,A,A,B,B,B,B,A,B,//14
	B,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,B,//15
	B,A,A,A,A,B,B,B,A,B,A,B,B,B,A,A,A,A,B,//16
	B,A,A,A,A,B,A,A,A,A,A,A,A,B,A,A,A,A,B,//17
	B,B,B,B,B,B,B,B,B,A,B,B,B,B,B,B,B,B,B,//18
};

int Gmap::LD = 36;//地图方格大小
int Gmap::PD = 3;// 豆子的绘图半径

//敌我双方出现的位置没有豆子出现
void Gmap::InitOP()
{
	pea_map_data[E_ARRAY][E_ROW] = false;
	pea_map_data[P_ARRAY][P_ROW] = false;
}

Stage_1::Stage_1()
{
	color = RGB(140, 240, 240);
	for (int i = 0; i < MAPLENTH; i++)
	{
		for (int j = 0; j < MAPLENTH; j++)
		{
			this->map_data[i][j] = this->initData[i][j];
			this->pea_map_data[i][j] = initData[i][j];
		}
	}

	//敌我双方出现的位置没有豆子出现

	pea_map_data[10][10] = true;
	this->InitOP();
}

Stage_2::Stage_2()
{
	color = RGB(140, 240, 240);
	for (int i = 0; i < MAPLENTH; i++)
	{
		for (int j = 0; j < MAPLENTH; j++)
		{
			this->map_data[i][j] = this->initData[i][j];
			this->pea_map_data[i][j] = initData[i][j];
		}
	}

	//敌我双方出现的位置没有豆子出现

	pea_map_data[10][10] = true;
	this->InitOP();
}

Stage_3::Stage_3()
{
	color = RGB(140, 240, 240);
	for (int i = 0; i < MAPLENTH; i++)
	{
		for (int j = 0; j < MAPLENTH; j++)
		{
			this->map_data[i][j] = this->initData[i][j];
			this->pea_map_data[i][j] = initData[i][j];
		}
	}

	//敌我双方出现的位置没有豆子出现

	pea_map_data[10][10] = true;
	this->InitOP();
}

void Gmap::DrawMap(HDC& memDC)//绘制地图里的墙壁
{
	for (int i = 0; i < MAPLENTH; i++)
	{
		for (int j = 0; j < MAPLENTH; j++)
		{

			//绘制墙壁
			if (!map_data[i][j])
			{
				RECT rect;
				rect.left = j * LD;
				rect.top = i * LD;
				rect.right = (j + 1) * LD;
				rect.bottom = (i + 1) * LD;
				FillRect(memDC, &rect, CreateSolidBrush(RGB(255,0,0)));
			}
		}
	}
}

void Gmap::DrawPeas(HDC& hdc)//绘制地图中的豆子
{
	for (int i = 0; i < MAPLENTH; i++)
	{
		for (int j = 0; j < MAPLENTH; j++)
		{

			if (pea_map_data[i][j])
			{
				Ellipse(hdc, (LD / 2 - PD) + j * LD, (LD / 2 - PD) + i * LD, (LD / 2 + PD) + j * LD, (LD / 2 + PD) + i * LD);
			}
		}
	}
}





#undef A
#undef B