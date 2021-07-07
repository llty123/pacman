#include "GObject.h"
#include"framework.h"

PacMan* Enermy::player = NULL;
Gmap* Gobject::pStage = NULL;
bool Gobject::Achive()//�ж������Ƿ񵽴��߼�����λ��
{
	int n = (point.x - pStage->LD / 2) % pStage->LD;
	int k = (point.y - pStage->LD / 2) % pStage->LD;
	bool i = (n == 0 && k == 0);//���������뷽�������غ�
		return i;
}

int Gobject::PtTransfrom(int k)//����������Ļ�ϵ�����ת��Ϊ��������
{
	return (k - (pStage->LD) / 2) / pStage->LD;
}

void Gobject::AchieveCtrl()//�����߼�����������
{
	if (Achive())
	{
		dArray = PtTransfrom(point.x);//������
		dRow = PtTransfrom(point.y);//������
	}
}

bool Gobject::Collision()//�߼���ײ���
{
	bool b = false;
	AchieveCtrl();//������������

	//�ж�ָ�����Ч��
	if (dArray<0 || dRow<0 || dArray>MAPLENTH || dRow>MAPLENTH)
	{
		b = true;//ָ����Ч
	}
	else if (Achive())
	{
		switch (twcommand)//�ж��н��ķ���
		{
		case LEFT:
			if (dArray > 0 && !pStage->map_data[dRow][dArray - 1])//�ж���һ�������Ƿ���ͨ��
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

		if (!b)//b=falseִ������
		{
			tw = twcommand;//ûײǽ��ָ��ɹ�
		}
	}
	//����������������
	mX = point.x;
	mY = point.y;
	int MAX = pStage->LD * MAPLENTH + pStage->LD / 2;
	int MIN = pStage->LD / 2;
	//�ж��н�����
	switch (tw)
	{
	case LEFT:
		if (dArray > 0 && !pStage->map_data[dRow][dArray - 1])//�ж���һ�������Ƿ���ͨ��
		{
			b = true;
			break;//ײǽ��
		}
		if (point.x < MIN)//���������ͼ��Χ�����ӵ�ͼ��һ�߳���
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

void Gobject::DrawBlank(HDC& hdc)//����һ�����α�������ǰһ֡��ͼ�񸲸�
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

void PacMan::AchieveCtrl()//����Զ��Ӻ���ʵ��
{
	Gobject::AchieveCtrl();//ʵ������������С�������Ĺ���
	if (Achive())
	{
		if (dRow >= 0 && dRow < MAPLENTH && dArray >= 0 && dArray < MAPLENTH)//�����ڵ�ͼ��Χ��
		{
			if (pStage->pea_map_data[dRow][dArray])//ԭ���ĵط��ж���
			{
				pStage->pea_map_data[dRow][dArray] = false;//�����ӱ�Ǹ�Ϊ���ޡ�
			}
		}
	}
}

bool PacMan::Win()//�ж�����Ƿ�Ӯ����Ϸ
{
	for (int i = 0; i <= MAPLENTH; i++)
	{
		for (int j = 0; j <= MAPLENTH; j++)
		{
			if (pStage->pea_map_data[i][j] == true)//��ͼ�ϻ��ж���
				return false;
		}
	}
	return true;
}

void PacMan::Over()//������ķ���ΪOVERʱ����ʾ�����ѽ�����ץס
{
	tw = OVER;
}

POINT PacMan::GetPos()//�ṩ����λ��
{
	return point;
}

void Enermy::Catch()//����ץ��������ж�����
{
	int DX = point.x - player->GetPos().x;//��������ҵľ���
	int DY = point.y - player->GetPos().y;

	if ((-RD < DX && DX < RD) && (-RD < DY && DY < RD))//���������ͼ�����ص�
		player->Over();
}

void Enermy::action()//�������˵���Ϊ
{
	bool b = Collision();
	MakeDecision(b);
	Catch();
}

void Enermy::Draw(HDC& hdc)//���Ƶ���
{
	HPEN pen = ::CreatePen(0, 0, color);
	HPEN oldpen = (HPEN)SelectObject(hdc, pen);
	Arc(hdc, point.x - DISTANCE, point.y - DISTANCE, point.x + DISTANCE, point.y + DISTANCE, point.x + DISTANCE, point.y, point.x - DISTANCE, point.y);//���ư�Բ�ε�ͷ
	int const LEGLENTH = (DISTANCE / LEGCOUNTS);
	//����֡������������Ȳ�
	if (frame % 2 == 0)
	{
		MoveToEx(hdc, point.x - DISTANCE, point.y, NULL);//��������
		LineTo(hdc, point.x - DISTANCE, point.y + DISTANCE - LEGLENTH);
		MoveToEx(hdc, point.x + DISTANCE, point.y, NULL);
		LineTo(hdc, point.x + DISTANCE, point.y + DISTANCE - LEGLENTH);
		//�������һ����Ȳ�
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
		MoveToEx(hdc, point.x - DISTANCE, point.y, NULL);//��������
		LineTo(hdc, point.x - DISTANCE, point.y + DISTANCE);
		MoveToEx(hdc, point.x + DISTANCE, point.y, NULL);
		LineTo(hdc, point.x + DISTANCE, point.y + DISTANCE);
		//�������һ����Ȳ�
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

	//���ݷ�������۾�
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
	frame++;//׼��������һ֡
	SelectObject(hdc, oldpen);
	DeleteObject(pen);
	return;
}

void PacMan::Draw(HDC& memDC)//���ơ������족
{
	if (tw == OVER)
	{

	}
	else if (frame % 2 == 0)//����֡�͵ڶ�֡����
	{
		int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
		int offsetX = DISTANCE / 2 + D_OFFSET;//�����ҵĽ���
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
	frame++;//������һ֡
}

void PacMan::SetTwCommands(TWARDS command)
{
	twcommand = command;
}

void RedOne::MakeDecision(bool b)//��ɢ�͵�����Ϊģʽ����
{
	int i = rand();//�������i
	if (b)//ײ��ǽ�ڣ���ı䷽��
	{
		//��ʱ��ת��
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
	//��ֱ������
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

void BlueOne::MakeDecision(bool b)//�����͵�����Ϊģʽ����
{
	const int DR = this->dRow - player->GetRow();//�õ����������롱���조�������еĲ�
	const int DA = this->dArray - player->GetArray();

	//����ھ��䷶Χ�ڣ�����׷��ģʽ
	if (!b && DR == 0)//�����������ͬһ��
	{
		if (DA <= BLUE_ALERT && DA>0)//�������ྯ�䷶Χ��
		{
			twcommand = LEFT;//�����ƶ�
			return;
		}
		if (DA < 0 && DA >= -BLUE_ALERT)//�Ҳྯ�䷶Χ��
		{
			twcommand = RIGHT;
			return;
		}
	}
	if (!b && DA == 0)//�����������ͬһ��
	{
		if (DR <= BLUE_ALERT && DR>0)//������Ϸ����䷶Χ��
		{
			twcommand = UP;
			return;
		}
		if (DR < 0 && DR >= -BLUE_ALERT)//�·�
		{
			twcommand = DOWN;
			return;
		}
	}

	//����׷��ģʽʱ��RED��Ϊ��ͬ
	int i = rand();//�������i
	if (b)//ײ��ǽ�ڣ���ı䷽��
	{
		//��ʱ��ת��
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
	//��ֱ������
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

void YellowOne::MakeDecision(bool b)//�����͵�����Ϊģʽ����
{
	const int DR = this->dRow - player->GetRow();//�õ����������롱���조�������еĲ�
	const int DA = this->dArray - player->GetArray();

	if (!b)
	{
		if (DR * DR > DA * DA)//���������������������
		{
			if (DA > 0)//�������ྯ�䷶Χ��
			{
				twcommand = LEFT;//�����ƶ�
				return;
			}
			else if (DA < 0)//�Ҳྯ�䷶Χ��
			{
				twcommand = RIGHT;
				return;
			}
		}

		else//���������������������
		{
			if ( DR > 0)//������Ϸ����䷶Χ��
			{
				twcommand = UP;
				return;
			}
			if (DR < 0 )//�·�
			{
				twcommand = DOWN;
				return;
			}
		}
	}

	//����׷��ģʽʱ��RED��Ϊ��ͬ
	int i = rand();//�������i
	if (b)//ײ��ǽ�ڣ���ı䷽��
	{
		//��ʱ��ת��
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
	//��ֱ������
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