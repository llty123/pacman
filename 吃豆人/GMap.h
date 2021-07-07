#pragma once
#include"framework.h"
#include <list>
#include<WinUser.h>
#include <windef.h>

#define MAPLENTH 19//��ͼ��С
#define P_ROW 10
#define P_ARRAY 9
#define E_ROW 8
#define E_ARRAY 9


using std::list;

class Gmap
{
protected:
	static int LD;//�ϰ���ߴ�
	static int PD;//���Ӱ뾶
	void InitOP();//����˫�����ֵ�λ��û�ж��ӳ���
	bool map_data[MAPLENTH][MAPLENTH];//�ϰ����߼���ͼ����
	bool pea_map_data[MAPLENTH][MAPLENTH];//�����߼���ͼ����
	COLORREF color;
public:
	void DrawMap(HDC& hdc);//���Ƶ�ͼ
	void DrawPeas(HDC& hdc);//���ƶ���
	//virtual ~Gmap();
	Gmap()
	{
		color = RGB(255, 0, 0);
	}

	friend class Gobject;//Gobject��������Է���
	friend class PacMan;//�������족���ʶ��ӵ�ͼ
};

//��һ��
class Stage_1 :public Gmap
{
private:
	bool static initData[MAPLENTH][MAPLENTH];
	

public:
	Stage_1();
};



