// 吃豆人.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "吃豆人.h"
#include"GObject.h"
#include"GMap.h"
#include <atlstr.h>
#include "recording.h"

#define WLENGTH 3000
#define WHIGHT 3000
#define STAGE_COUNT 3//最大关卡数
#define MB_OKA 5

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
PacMan* p;
Gobject* e1;
Gobject* e2;
Gobject* e3;
Gobject* e4;
Record* record;
// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, HWND& hWnd)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 1000, 1000, nullptr, nullptr, hInstance, nullptr);//6、7个参数表示窗口横向、纵向大小

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

bool IsPlaying()//判断是否在播放
{
	// char RefStr[10];
	CString RefStr;
	if (mciSendString(L"status bgm", RefStr.GetBuffer(10), 10, 0) == 0)
	{
		if (RefStr.Find(L"playing") >= 0 || RefStr.Find(L"播放") >= 0)
			return true;
	}
	return false;
}

void ResetGobjects()
{
	p->SetPosition(P_ROW, P_ARRAY);
	e1->SetPosition(E_ROW, E_ARRAY);
	e2->SetPosition(E_ROW, E_ARRAY);
	e3->SetPosition(E_ROW, E_ARRAY);
	e4->SetPosition(E_ROW, E_ARRAY);
}

//定义一个函数模板，在模板中传入指针变量即可对它所指向的堆内存进行回收
template<class T>
void Realese(T t)
{
	if (t != NULL)
		delete t;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此处放置代码。
	int s_n = 0;//进行到的关卡数
	p = new PacMan(P_ROW, P_ARRAY);
	e1 = new RedOne(E_ROW, E_ARRAY);
	e2 = new RedOne(E_ROW, E_ARRAY);
	e3 = new BlueOne(E_ROW, E_ARRAY);
	e4 = new YellowOne(E_ROW, E_ARRAY);
	record = new Record();
	Gmap* MapArray[STAGE_COUNT] = { new Stage_1(),new Stage_1(), new Stage_1() };//存储关卡地图
	Gobject::pStage = MapArray[s_n];
	Enermy::player = p;//敌人追踪的玩家为p
	TCHAR score[] = _T("score:");
	TCHAR tip[] = _T("提示：");
	TCHAR tip_1[] = _T("键盘“↑”控制向上运动");
	TCHAR tip_2[] = _T("键盘“↓”控制向下运动");
	TCHAR tip_3[] = _T("键盘“←”控制向左运动");
	TCHAR tip_4[] = _T("键盘“→”控制向右运动");
	TCHAR score_i[2] = { 0 };//存储分数值
	TCHAR time[] = _T("time:");//存储时间
	TCHAR time_s[] = _T("s");
	TCHAR time_i[10] = { 0 };
	DWORD t = 0;
	int Time;

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:

	//test
	//const int BUFSIZE = 4096;
	//DWORD dwReadSize = 0;
	//char ch_buffer[BUFSIZE];//缓冲区
	//record->Output_Recording(ch_buffer,&dwReadSize);
	//HWND hwnd;
	//hwnd = CreateWindow(szWindowClass, L"历史记录", WS_OVERLAPPEDWINDOW,
	//	CW_USEDEFAULT, 0, 1000, 1000, nullptr, nullptr, hInstance, nullptr);
	//HDC hdc_record = GetDC(hwnd);
	//ShowWindow(hwnd, nCmdShow);
	//UpdateWindow(hwnd);
	//for (int i = 0,y=0; i < dwReadSize; i++)
	//{
	//	TCHAR line_record[100] = { 0 };//行缓冲区，用于存储每一行的字符
	//	int j = 0;
	//	while (ch_buffer[i] != '\n')
	//	{
	//		line_record[j] = ch_buffer[i];
	//		j++;
	//		i++;
	//	}
	//	TextOut(hdc_record, 0, y, line_record, _tcslen(line_record));
	//	y += 30;
	//	for (int k = 0; k < 100; k++) line_record[k] = '\0';
	//}

	HWND hWnd;

	if (!InitInstance(hInstance, nCmdShow, hWnd))
	{
		return FALSE;
	}
	DWORD StartTime = GetTickCount64();//计时开始
	HDC hdc = GetDC(hWnd);
	Gobject::pStage->DrawMap(hdc);//绘制地图
	TextOut(hdc, 700, 570, tip, _tcslen(tip));
	TextOut(hdc, 700, 600, tip_1, _tcslen(tip_1));
	TextOut(hdc, 700, 630, tip_2, _tcslen(tip_2));
	TextOut(hdc, 700, 660, tip_3, _tcslen(tip_3));
	TextOut(hdc, 700, 690, tip_4, _tcslen(tip_4));
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

	MSG msg;

	mciSendString(TEXT("open 游戏音乐.wma alias bgm"), NULL, 0, NULL);
	mciSendString(TEXT("play bgm repeat"), NULL, 0, NULL);
	//创建音乐播放控制按钮
	CreateWindow(
		TEXT("button"), TEXT("音乐暂停"),
		WS_CHILD | WS_VISIBLE | BS_LEFT | BS_AUTOCHECKBOX,
		750, 400, 85, 26,
		hWnd, (HMENU)9, hInst, NULL
	);

	//创建游戏记录控制按钮
	CreateWindow(
		TEXT("button"), TEXT("游戏记录"),
		WS_CHILD | WS_VISIBLE | BS_LEFT | BS_AUTOCHECKBOX,
		750, 500, 85, 26,
		hWnd, (HMENU)9, hInst, NULL
	);

	// 主消息循环:
	while (p->GetTw() != OVER && s_n < STAGE_COUNT)
	{
		//HDC hdc = GetDC(hWnd);

		//int state = IsDlgButtonChecked(hWnd, 9);//判断按钮选中状态
		//if (state == BST_CHECKED)//按钮被选中
		//{
		//    if (IsPlaying() == true)
		//    {
		//        continue;
		//    }
		//    else
		//    {
		//        mciSendString(TEXT("play bgm repeat"), NULL, 0, NULL);
		//    }
		//
		//
		//}
		//else if (state == BST_UNCHECKED)//按钮未被选中
		//{
		//   mciSendString(TEXT("close bgm"), NULL, 0, NULL);
		//}

		TextOut(hdc, 700, 0, score, _tcslen(score));//输出得分情况
		wsprintf(score_i, _T("%d"), p->pStage->score);
		TextOut(hdc, 750, 0, score_i, _tcslen(score_i));//输出得分情况
		if (p->Win())//玩家胜利
		{
			s_n++;
			ResetGobjects();
			if (s_n < STAGE_COUNT)
			{
				mciSendString(TEXT("pause bgm "), NULL, 0, NULL);
				mciSendString(TEXT("open 胜利.wav alias victory"), NULL, 0, NULL);
				mciSendString(TEXT("play victory"), NULL, 0, NULL);
				MessageBoxA(hWnd, "恭喜过关！", "吃豆子提示", MB_OKA);
				Gobject::pStage = MapArray[++s_n];
				RECT screenRect;
				screenRect.top = 0;
				screenRect.left = 0;
				screenRect.right = WLENGTH;
				screenRect.bottom = WHIGHT;
				::FillRect(hdc, &screenRect, CreateSolidBrush(RGB(255, 255, 255)));
				Gobject::pStage->DrawMap(hdc);
			}
			continue;
		}
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (GetAsyncKeyState(VK_UP) & 0x8000)//w控制大嘴向上
			p->SetTwCommands(UP);
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)//s控制大嘴向下
			p->SetTwCommands(DOWN);
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)//a控制大嘴向左
			p->SetTwCommands(LEFT);
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)//d控制大嘴向右
			p->SetTwCommands(RIGHT);
		else
		{
			if (GetTickCount64() - t > 58)
			{
				HDC hdc = GetDC(hWnd);
				e1->action();
				e2->action();
				e3->action();
				e4->action();
				p->action();

				Gobject::pStage->DrawPeas(hdc);
				e1->DrawBlank(hdc);
				e2->DrawBlank(hdc);
				e3->DrawBlank(hdc);
				e4->DrawBlank(hdc);
				p->DrawBlank(hdc);
				e1->Draw(hdc);
				e2->Draw(hdc);
				e3->Draw(hdc);
				e4->Draw(hdc);
				p->Draw(hdc);
				DeleteDC(hdc);
				t = GetTickCount64();
			}
		}
		DWORD EndTime = GetTickCount64();//计时结束
		TextOut(hdc, 700, 30, time, _tcslen(time));//输出用时情况
		Time = (EndTime - StartTime) / 1000;
		wsprintf(time_i, _T("%d"), Time);
		TextOut(hdc, 740, 30, time_i, _tcslen(time_i));
		TextOut(hdc, 770, 30, time_s, _tcslen(time_s));
	}

	Realese(e1);
	Realese(e2);
	Realese(e3);
	Realese(e4);
	for (int i = 0; i < STAGE_COUNT; i++)
	{
		Realese(MapArray[i]);
	}
	if (p->GetTw() == OVER)
	{
		mciSendString(TEXT("close bgm "), NULL, 0, NULL);
		mciSendString(TEXT("open 游戏失败音效.wma alias defeat"), NULL, 0, NULL);
		mciSendString(TEXT("play defeat"), NULL, 0, NULL);
		MessageBoxA(hWnd, "出师未捷", "吃豆子提示", MB_OKA);
	}
	else
	{
		mciSendString(TEXT("close bgm "), NULL, 0, NULL);
		mciSendString(TEXT("open 胜利.wav alias victory"), NULL, 0, NULL);
		mciSendString(TEXT("play victory"), NULL, 0, NULL);
		MessageBoxA(hWnd, "恭喜你赢得胜利", "吃豆子提示", MB_OKA);
	}
	Realese(p);

	return (int)msg.wParam;
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	//wcex.cbSize = 100;

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case 9:
		{
			int state = IsDlgButtonChecked(hWnd, 9);//判断按钮选中状态
			if (state == BST_CHECKED)//按钮被选中
			{
				mciSendString(TEXT("pause bgm"), NULL, 0, NULL);
			}
			else if (state == BST_UNCHECKED)//按钮未被选中
			{
				if (IsPlaying() == true)
				{
					;
				}
				else
				{
					mciSendString(TEXT("play bgm repeat"), NULL, 0, NULL);
				}
			}
			break;
		}
		case 10:
		{
			int state = IsDlgButtonChecked(hWnd, 9);//判断按钮选中状态
			HWND hwnd;
			if (state == BST_CHECKED)//按钮被选中
			{
				const int BUFSIZE = 4096;
				DWORD dwReadSize = 0;
				char ch_buffer[BUFSIZE];//缓冲区
				record->Output_Recording(ch_buffer, &dwReadSize);
				
				hwnd = CreateWindow(szWindowClass, L"历史记录", WS_OVERLAPPEDWINDOW,
					CW_USEDEFAULT, 0, 1000, 1000, nullptr, nullptr, GetModuleHandle, nullptr);
				HDC hdc_record = GetDC(hwnd);
				ShowWindow(hwnd, SW_SHOWNORMAL);
				UpdateWindow(hwnd);
				for (int i = 0, y = 0; i < dwReadSize; i++)
				{
					TCHAR line_record[100] = { 0 };//行缓冲区，用于存储每一行的字符
					int j = 0;
					while (ch_buffer[i] != '\n')
					{
						line_record[j] = ch_buffer[i];
						j++;
						i++;
					}
					TextOut(hdc_record, 0, y, line_record, _tcslen(line_record));
					y += 30;
					for (int k = 0; k < 100; k++) line_record[k] = '\0';
				}
			}
			else if (state == BST_UNCHECKED)//按钮未被选中
			{
				//CloseWindow(hwnd);
			}
		}
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);

		}
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		::exit(0);
		break;
	}
	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}
	return 0;
}

	// “关于”框的消息处理程序。
	INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);
		switch (message)
		{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
		}
		return (INT_PTR)FALSE;
	}