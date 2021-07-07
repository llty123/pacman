#include "GObject.h"
#include"framework.h"

PacMan* Enermy::player = NULL;
Gmap* Gobject::pStage = NULL;
bool Gobject::Achive()//判断物体是否到达逻辑坐标位置
{
	int n = (point.x - pStage->LD / 2) % pStage->LD;
	int k = (point.y - pStage->LD / 2) % pStage->LD;
	bool i = (n == 0 && k == 0);//物体中心与方格中心重合
		return i;
}

int Gobject::PtTransfrom(int k)//将物体在屏幕上的坐标转变为行列坐标
{
	return (k - (pStage->LD) / 2) / pStage->LD;
}

void Gobject::AchieveCtrl()//到达逻辑点后更新数据
{
	if (Achive())
	{
		dArray = PtTransfrom(point.x);//更新列
		dRow = PtTransfrom(point.y);//更新行
	}
}

bool Gobject::Collision()//逻辑碰撞检测
{
	bool b = false;
	AchieveCtrl();//更新行列坐标

	//判断指令的有效性
	if (dArray<0 || dRow<0 || dArray>MAPLENTH || dRow>MAPLENTH)
	{
		b = true;//指令无效
	}
	else if (Achive())
	{
		switch (twcommand)//判断行进的方向
		{
		case LEFT:
			if (dArray > 0 && !pStage->map_data[dRow][dArray - 1])//判断下一个格子是否能通行
				b = true;
			break;
		case RIGHT:
			if (dArray < (MAPLENTH-1) && !pStage->map_data[dRow][dArray + 1])
				b = true;
			break;
		case UP:
			if (dRow > 0 && !pStage->map_data[dRow - 1][dArray])
				b = true;
			break;
		case DOWN:
			if (dRow < (MAPLENTH-1) && !pStage->map_data[dRow + 1][dArray])
				b = true;
			break;
		case OVER:
			break;
		default:
			break;
		}

		if (!b)//b=false执行命令
		{
			tw = twcommand;//没撞墙，指令成功
		}
	}
	//更新物体坐标数据
	mX = point.x;
	mY = point.y;
	int MAX = pStage->LD * MAPLENTH + pStage->LD / 2;
	int MIN = pStage->LD / 2;
	//判断行进方向
	switch (tw)
	{
	case LEFT:
		if (dArray > 0 && !pStage->map_data[dRow][dArray - 1])//判断下一个格子是否能通行
		{
			b = true;
			break;//撞墙了
		}
		if (point.x < MIN)//如果超出地图范围，则会从地图另一边出现
			point.x = MAX;
		point.x -= speed;
		break;
	case RIGHT:
		if (dArray < (MAPLENTH - 1) && !pStage->map_data[dRow][dArray + 1])
		{
			b = true;
			break;
		}
		point.x += speed;
		if (point.x > MAX)
			point.x = MIN;
		break;
	case UP:
		if (dRow > 0 && !pStage->map_data[dRow - 1][dArray])
		{
			b = true;
			break;
		}
		if (point.y < MIN)
			point.y = MAX;
		point.y -= speed;
		break;
	case DOWN:
		if (dRow < (MAPLENTH - 1) && !pStage->map_data[dRow + 1][dArray])
		{
			b = true;
			break;
		}
		point.y += speed;
		if (point.y > MAX)
			point.y = MIN;
		break;
	case OVER:
		break;
	default:
		break;
	}
	return b;
}

void Gobject::DrawBlank(HDC& hdc)//绘制一个矩形背景，将前一帧的图像覆盖
{
	RECT rect;
	rect.top = mY - RD;
	rect.left = mX - RD;
	rect.right = mX + RD;
	rect.bottom = mY + RD;
	FillRect(hdc, &rect, ::CreateSolidBrush(RGB(255, 255, 255)));
}

void Gobject::SetPosition(int Row, int Array)
{
	dRow = Row;
	dArray = Array;
	this->point.y = dRow * pStage->LD + pStage->LD / 2;
	this->point.x = dArray * pStage->LD + pStage->LD / 2;
}

void PacMan::action()
{
	Collision();
}

void PacMan::AchieveCtrl()//大嘴吃豆子函数实现
{
	Gobject::AchieveCtrl();//实现物体类更新行、列坐标的功能
	if (Achive())
	{
		if (dRow >= 0 && dRow < MAPLENTH && dArray >= 0 && dArray < MAPLENTH)//大嘴在地图范围内
		{
			if (pStage->pea_map_data[dRow][dArray])//原来的地方有豆子
			{
				pStage->pea_map_data[dRow][dArray] = false;//将豆子标记改为”无“
			}
		}
	}
}

bool PacMan::Win()//判断玩家是否赢得游戏
{
	for (int i = 0; i <= MAPLENTH; i++)
	{
		for (int j = 0; j <= MAPLENTH; j++)
		{
			if (pStage->pea_map_data[i][j] == true)//地图上还有豆子
				return false;
		}
	}
	return true;
}

void PacMan::Over()//当大嘴的方向为OVER时，表示敌人已将大嘴抓住
{
	tw = OVER;
}

POINT PacMan::GetPos()//提供自身位置
{
	return point;
}

void Enermy::Catch()//敌人抓捕大嘴的判定函数
{
	int DX = point.x - player->GetPos().x;//自身与玩家的距离
	int DY = point.y - player->GetPos().y;

	if ((-RD < DX && DX < RD) && (-RD < DY && DY < RD))//自身与大嘴图像有重叠
		player->Over();
}

void Enermy::action()//描述敌人的行为
{
	bool b = Collision();
	MakeDecision(b);
	Catch();
}

void Enermy::Draw(HDC& hdc)//绘制敌人
{
	HPEN pen = ::CreatePen(0, 0, color);
	HPEN oldpen = (HPEN)SelectObject(hdc, pen);
	Arc(hdc, point.x - DISTANCE, point.y - DISTANCE, point.x + DISTANCE, point.y + DISTANCE, point.x + DISTANCE, point.y, point.x - DISTANCE, point.y);//绘制半圆形的头
	int const LEGLENTH = (DISTANCE / LEGCOUNTS);
	//根据帧数绘制身体和腿部
	if (frame % 2 == 0)
	{
		MoveToEx(hdc, point.x - DISTANCE, point.y, NULL);//绘制身体
		LineTo(hdc, point.x - DISTANCE, point.y + DISTANCE - LEGLENTH);
		MoveToEx(hdc, point.x + DISTANCE, point.y, NULL);
		LineTo(hdc, point.x + DISTANCE, point.y + DISTANCE - LEGLENTH);
		//从左往右绘制腿部
		for (int i = 0; i < LEGCOUNTS; i++)
		{
			Arc(hdc, point.x - DISTANCE + i * 2 * LEGLENTH, point.y + DISTANCE - 2 * LEGLENTH,
				point.x - DISTANCE + (i + 1) * 2 * LEGLENTH, point.y + DISTANCE,
				point.x - DISTANCE + i * 2 * LEGLENTH, point.y + DISTANCE - LEGLENTH,
				point.x - DISTANCE + (i + 1) * 2 * LEGLENTH, point.y + DISTANCE - LEGLENTH);
		}

	}
	else
	{
		MoveToEx(hdc, point.x - DISTANCE, point.y, NULL);//绘制身体
		LineTo(hdc, point.x - DISTANCE, point.y + DISTANCE);
		MoveToEx(hdc, point.x + DISTANCE, point.y, NULL);
		LineTo(hdc, point.x + DISTANCE, point.y + DISTANCE);
		//从左往右绘制腿部
		for (int i = 0; i < LEGCOUNTS-1; i++)
		{
			Arc(hdc, point.x - DISTANCE + (1 + i * 2) * LEGLENTH, point.y + DISTANCE - 2 * LEGLENTH,
				point.x - DISTANCE + (3 + i * 2) * LEGLENTH, point.y + DISTANCE,
				point.x - DISTANCE + (1 + i * 2) * LEGLENTH, point.y + DISTANCE - LEGLENTH,
				point.x - DISTANCE + (3 + i * 2) * LEGLENTH, point.y + DISTANCE - LEGLENTH);
		}
		MoveToEx(hdc, point.x + DISTANCE, point.y+DISTANCE, NULL);
		LineTo(hdc, point.x + DISTANCE - LEGLENTH, point.y + DISTANCE - LEGLENTH);
	}

	//根据方向绘制眼睛
	int R = DISTANCE / 5;
	switch (tw)
	{
	case UP:
	
		Ellipse(hdc, point.x - 2 * R, point.y - 2 * R, point.x, point.y);
		Ellipse(hdc, point.x, point.y - 2 * R, point.x + 2 * R, point.y);
		break;
	case DOWN:
		Ellipse(hdc, point.x - 2 * R, point.y , point.x, point.y+ 2 * R);
		Ellipse(hdc, point.x, point.y , point.x + 2 * R, point.y+ 2 * R);
		break;
	case LEFT:
		Ellipse(hdc, point.x - 3 * R, point.y-R, point.x-R, point.y + R);
		Ellipse(hdc, point.x - R, point.y - R, point.x + R, point.y + R);
		break;
	case RIGHT:
		Ellipse(hdc, point.x - R, point.y - R, point.x + R, point.y + R);
		Ellipse(hdc, point.x + R, point.y - R, point.x + 3 * R, point.y + R);
		break;
	default:
		break;
	}
	frame++;//准备绘制下一帧
	SelectObject(hdc, oldpen);
	DeleteObject(pen);
	return;
}

void PacMan::Draw(HDC& memDC)//绘制”“大嘴”
{
	if (tw == OVER)
	{

	}
	else if (frame % 2 == 0)//第四帧和第二帧动画
	{
		int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
		int offsetX = DISTANCE / 2 + D_OFFSET;//弧和弦的交点
		int offsetY = DISTANCE / 2 + D_OFFSET;

		switch (tw)
		{
		case UP:
			x1 = point.x - offsetX;
			x2 = point.x + offsetX;
			y1 = point.y - offsetY;
			y2 = point.y - offsetY;
			break;
		case DOWN:
			x1 = point.x + offsetX;
			x2 = point.x - offsetX;
			y1 = point.y + offsetY;
			y2 = point.y + offsetY;
			break;
		case LEFT:
			x1 = point.x - offsetX;
			x2 = point.x - offsetX;
			y1 = point.y + offsetY;
			y2 = point.y - offsetY;
			break;
		case RIGHT:
			x1 = point.x + offsetX;
			x2 = point.x + offsetX;
			y1 = point.y - offsetY;
			y2 = point.y + offsetY;
			break;
		default:
			break;
		}
		Arc(memDC, point.x - DISTANCE, point.y - DISTANCE ,
			point.x + DISTANCE ,point.y + DISTANCE,
			x1,y1,
			x2,y2);
		MoveToEx(memDC, x1, y1, NULL);
		LineTo(memDC, point.x, point.y);
		LineTo(memDC, x2,y2);
	}
	else if (frame % 3 == 0)
	{
		Ellipse(memDC, point.x - DISTANCE, point.y - DISTANCE,
			point.x + DISTANCE, point.y + DISTANCE);
	}
	else
	{
		int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
		
		switch (tw)
		{
		case UP:
			x1 = point.x - DISTANCE;
			x2 = point.x + DISTANCE;
			y1 = point.y;
			y2 = point.y ;
			break;
		case DOWN:
			x1 = point.x + DISTANCE;
			x2 = point.x - DISTANCE;
			y1 = point.y ;
			y2 = point.y ;
			break;
		case LEFT:
			x1 = point.x ;
			x2 = point.x;
			y1 = point.y + DISTANCE;
			y2 = point.y - DISTANCE;
			break;
		case RIGHT:
			x1 = point.x ;
			x2 = point.x ;
			y1 = point.y - DISTANCE;
			y2 = point.y + DISTANCE;
			break;
		default:
			break;
		}
		Arc(memDC, point.x - DISTANCE, point.y - DISTANCE,
			point.x + DISTANCE, point.y + DISTANCE,
			x1, y1,
			x2, y2);
		MoveToEx(memDC, x1, y1, NULL);
		LineTo(memDC, point.x, point.y);
		LineTo(memDC, x2, y2);
	}
	frame++;//绘制下一帧
}

void PacMan::SetTwCommands(TWARDS command)
{
	twcommand = command;
}

void RedOne::MakeDecision(bool b)//松散型敌人行为模式函数
{
	int i = rand();//随机参数i
	if (b)//撞到墙壁，则改变方向
	{
		//逆时针转向
		if (i % 4 == 0)
		{
			tw == UP ? twcommand = LEFT : twcommand = UP;
		}
		else if (i % 3 == 0)
		{
			tw == DOWN ? twcommand = RIGHT : twcommand = DOWN;
		}
		else if (i % 2 == 0)
		{
			tw == RIGHT ? twcommand = UP : twcommand = RIGHT;
		}
		else
		{
			tw == LEFT ? twcommand = DOWN : twcommand = LEFT;
		}
		return;
	}
	//沿直线行走
	if (i % 4 == 0)
	{
		twcommand!= UP ? twcommand = DOWN : twcommand = UP;
	}
	else if (i % 3 == 0)
	{
		tw != DOWN ? twcommand = UP : twcommand = DOWN;
	}
	else if (i % 2 == 0)
	{
		tw != RIGHT ? twcommand = LEFT : twcommand = RIGHT;
	}
	else
	{
		tw != LEFT ? twcommand = RIGHT : twcommand = LEFT;
	}

}

void BlueOne::MakeDecision(bool b)//守卫型敌人行为模式函数
{
	const int DR = this->dRow - player->GetRow();//得到自身行列与”大嘴“所在行列的差
	const int DA = this->dArray - player->GetArray();

	//玩家在警戒范围内，进入追踪模式
	if (!b && DR == 0)//自身与大嘴在同一行
	{
		if (DA <= BLUE_ALERT && DA>0)//玩家在左侧警戒范围内
		{
			twcommand = LEFT;//向左移动
			return;
		}
		if (DA < 0 && DA >= -BLUE_ALERT)//右侧警戒范围内
		{
			twcommand = RIGHT;
			return;
		}
	}
	if (!b && DA == 0)//自身与大嘴在同一列
	{
		if (DR <= BLUE_ALERT && DR>0)//玩家在上方警戒范围内
		{
			twcommand = UP;
			return;
		}
		if (DR < 0 && DR >= -BLUE_ALERT)//下方
		{
			twcommand = DOWN;
			return;
		}
	}

	//不在追踪模式时与RED行为相同
	int i = rand();//随机参数i
	if (b)//撞到墙壁，则改变方向
	{
		//逆时针转向
		if (i % 4 == 0)
		{
			tw == UP ? twcommand = LEFT : twcommand = UP;
		}
		else if (i % 3 == 0)
		{
			tw == DOWN ? twcommand = RIGHT : twcommand = DOWN;
		}
		else if (i % 2 == 0)
		{
			tw == RIGHT ? twcommand = UP : twcommand = RIGHT;
		}
		else
		{
			tw == LEFT ? twcommand = DOWN : twcommand = LEFT;
		}
		return;
	}
	//沿直线行走
	if (i % 4 == 0)
	{
		twcommand != UP ? twcommand = DOWN : twcommand = UP;
	}
	else if (i % 3 == 0)
	{
		tw != DOWN ? twcommand = UP : twcommand = DOWN;
	}
	else if (i % 2 == 0)
	{
		tw != RIGHT ? twcommand = LEFT : twcommand = RIGHT;
	}
	else
	{
		tw != LEFT ? twcommand = RIGHT : twcommand = LEFT;
	}

}

void YellowOne::MakeDecision(bool b)//扰乱型敌人行为模式函数
{
	const int DR = this->dRow - player->GetRow();//得到自身行列与”大嘴“所在行列的差
	const int DA = this->dArray - player->GetArray();

	if (!b)
	{
		if (DR * DR > DA * DA)//与玩家相差的行数大于列数
		{
			if (DA > 0)//玩家在左侧警戒范围内
			{
				twcommand = LEFT;//向左移动
				return;
			}
			else if (DA < 0)//右侧警戒范围内
			{
				twcommand = RIGHT;
				return;
			}
		}

		else//与玩家相差的列数大于行数
		{
			if ( DR > 0)//玩家在上方警戒范围内
			{
				twcommand = UP;
				return;
			}
			if (DR < 0 )//下方
			{
				twcommand = DOWN;
				return;
			}
		}
	}

	//不在追踪模式时与RED行为相同
	int i = rand();//随机参数i
	if (b)//撞到墙壁，则改变方向
	{
		//逆时针转向
		if (i % 4 == 0)
		{
			tw == UP ? twcommand = LEFT : twcommand = UP;
		}
		else if (i % 3 == 0)
		{
			tw == DOWN ? twcommand = RIGHT : twcommand = DOWN;
		}
		else if (i % 2 == 0)
		{
			tw == RIGHT ? twcommand = UP : twcommand = RIGHT;
		}
		else
		{
			tw == LEFT ? twcommand = DOWN : twcommand = LEFT;
		}
		return;
	}
	//沿直线行走
	if (i % 4 == 0)
	{
		twcommand != UP ? twcommand = DOWN : twcommand = UP;
	}
	else if (i % 3 == 0)
	{
		tw != DOWN ? twcommand = UP : twcommand = DOWN;
	}
	else if (i % 2 == 0)
	{
		tw != RIGHT ? twcommand = LEFT : twcommand = RIGHT;
	}
	else
	{
		tw != LEFT ? twcommand = RIGHT : twcommand = LEFT;
	}

}