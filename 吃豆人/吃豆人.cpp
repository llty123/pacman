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
//按钮的宏
#define MAX_LOADSTRING 100
#define GAME_RECORDING 1
#define RECORDING_BACK 2
#define MUSIC		   3
#define BEGIN_GAME	   4
#define GAME_BACK      5
#define QUIT		   6
#define GAME_QUIT	   7

#define MAX_LOADSTRING 100

// 全局变量:
WNDPROC  oldProc = NULL;
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
LRESULT CALLBACK    WndProc_main(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndProc_record(HWND, UINT, WPARAM, LPARAM);
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

	hWnd = CreateWindowW(L"GAME", L"游戏界面", WS_OVERLAPPEDWINDOW,
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

	//主菜单窗口类
	WNDCLASSEXW wmain;

	wmain.cbSize = sizeof(WNDCLASSEX);

	wmain.style = CS_HREDRAW | CS_VREDRAW;
	wmain.lpfnWndProc = WndProc_main;
	wmain.cbClsExtra = 0;
	wmain.cbWndExtra = 0;
	wmain.hInstance = hInstance;
	wmain.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
	wmain.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wmain.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wmain.lpszMenuName = MAKEINTRESOURCEW(IDC_MY);
	wmain.lpszClassName = L"MAIN";
	wmain.hIconSm = LoadIcon(wmain.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassExW(&wmain);

	//历史记录窗口类
	WNDCLASSEXW wrecord;

	wrecord.cbSize = sizeof(WNDCLASSEX);

	wrecord.style = CS_HREDRAW | CS_VREDRAW;
	wrecord.lpfnWndProc = WndProc_record;
	wrecord.cbClsExtra = 0;
	wrecord.cbWndExtra = 0;
	wrecord.hInstance = hInstance;
	wrecord.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
	wrecord.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wrecord.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wrecord.lpszMenuName = MAKEINTRESOURCEW(IDC_MY);
	wrecord.lpszClassName = L"RECORD";
	wrecord.hIconSm = LoadIcon(wrecord.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassExW(&wrecord);

	int s_n = 0;//进行到的关卡数
	BOOL begin_game = FALSE;//是否已经开始游戏
	p = new PacMan(P_ROW, P_ARRAY);
	e1 = new RedOne(E_ROW, E_ARRAY);
	e2 = new RedOne(E_ROW, E_ARRAY);
	e3 = new BlueOne(E_ROW, E_ARRAY);
	e4 = new YellowOne(E_ROW, E_ARRAY);
	record = new Record();
	Gmap* MapArray[STAGE_COUNT] = { new Stage_3(),new Stage_2(), new Stage_1() };//存储关卡地图
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
	DWORD t = 0, StartTime, EndTime;
	int Time;
	const int BUFSIZE = 4096;
	DWORD dwReadSize = 0, dwWrittenSize = 0;//读写缓存区的大小
	char ch_buffer[BUFSIZE];//读缓冲区
	char wr_buffer[BUFSIZE];//写缓存区
	//判断窗口可见性的变量
	BOOL main_visible = TRUE, game_visible = FALSE, record_visible = FALSE;
	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

	MSG msg;
	

	
	//创建记录窗口
	HWND hwnd_record;
	hwnd_record = CreateWindow(L"RECORD", L"历史记录", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 1000, 1000, nullptr, nullptr, hInstance, nullptr);
	HDC hdc_record = GetDC(hwnd_record);
	ShowWindow(hwnd_record, SW_HIDE);
	UpdateWindow(hwnd_record);
	//创建主菜单窗口
	HWND hwnd_main;
	hwnd_main = CreateWindow(L"MAIN", L"主菜单", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 1000, 1000, nullptr, nullptr, hInstance, nullptr);
	HDC hdc_main = GetDC(hwnd_main);
	ShowWindow(hwnd_main, SW_SHOWNORMAL);
	UpdateWindow(hwnd_main);

	//创建游戏界面
	HWND hWnd;
	if (!InitInstance(hInstance, nCmdShow, hWnd))
	{
		return FALSE;
	}
	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);
	HDC hdc = GetDC(hWnd);

	//创建历史记录窗口
	record->getlocaltime(record->localtime);
	record->Output_Recording(ch_buffer, &dwReadSize);


	
	Gobject::pStage->DrawMap(hdc);//绘制地图
	

	mciSendString(TEXT("open 游戏音乐.wma alias bgm"), NULL, 0, NULL);
	mciSendString(TEXT("play bgm repeat"), NULL, 0, NULL);
	

	// 主消息循环:
	while (1)
	{
		//获得窗口可见性
		main_visible = IsWindowVisible(hwnd_main);
		game_visible = IsWindowVisible(hWnd);
		record_visible = IsWindowVisible(hwnd_record);

		if (main_visible == TRUE)//如果窗口可见
		{
			;
		}
		else
		{
			;
		}

		if (game_visible == TRUE)
		{
			begin_game = TRUE;
			StartTime = GetTickCount64();//计时开始

			while (p->GetTw() != OVER && s_n < STAGE_COUNT)
			{
				TextOut(hdc, 700, 0, score, _tcslen(score));//输出得分情况
				wsprintf(score_i, _T("%d"), p->pStage->score);
				TextOut(hdc, 750, 0, score_i, _tcslen(score_i));//输出得分情况

				if (p->Win())//玩家胜利
				{
					s_n++;
					ResetGobjects();
					if (s_n < STAGE_COUNT)
					{
						//播放胜利音效
						mciSendString(TEXT("pause bgm "), NULL, 0, NULL);
						mciSendString(TEXT("open 胜利.wav alias victory"), NULL, 0, NULL);
						mciSendString(TEXT("play victory"), NULL, 0, NULL);
						////录入游戏记录
						//record->score = p->pStage->score;
						//record->time_use = Time;
						//record->Write_Recording(wr_buffer, &dwWrittenSize, record);
						MessageBoxA(hWnd, "恭喜过关！", "吃豆子提示", MB_OK);
						Gobject::pStage = MapArray[s_n];
						RECT screenRect;
						screenRect.top = 0;
						screenRect.left = 0;
						screenRect.right = WLENGTH;
						screenRect.bottom = WHIGHT;
						::FillRect(hdc, &screenRect, CreateSolidBrush(RGB(255, 255, 255)));
						Gobject::pStage->DrawMap(hdc);
						if (IsPlaying() == FALSE)
						{
							mciSendString(TEXT("play bgm repeat"), NULL, 0, NULL);
						}
						mciSendString(TEXT("close victory"), NULL, 0, NULL);
					}
					continue;
				}//end_if (p->Win())//玩家胜利
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
				}//end_else
				EndTime = GetTickCount64();//计时结束
				TextOut(hdc, 700, 30, time, _tcslen(time));//输出用时情况
				Time = (EndTime - StartTime) / 1000;//游戏时长
				wsprintf(time_i, _T("%d"), Time);
				TextOut(hdc, 740, 30, time_i, _tcslen(time_i));
				TextOut(hdc, 770, 30, time_s, _tcslen(time_s));
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}//end_while (p->GetTw() != OVER && s_n < STAGE_COUNT)



			if (p->GetTw() == OVER)
			{
				//播放游戏失败音效
				mciSendString(TEXT("close bgm "), NULL, 0, NULL);
				mciSendString(TEXT("open 游戏失败音效.wma alias defeat"), NULL, 0, NULL);
				mciSendString(TEXT("play defeat"), NULL, 0, NULL);
				//录入游戏记录
				record->score = p->pStage->score;
				record->time_use = Time;
				record->Write_Recording(wr_buffer, &dwWrittenSize, record);
				MessageBoxA(hWnd, "哎呀，失败了\n下次加油吧", "吃豆子提示", MB_OK);
				goto END_GAME;
			}//end_if (p->GetTw() == OVER)
			else
			{
				//录入游戏记录
				record->score = p->pStage->score;
				record->time_use = Time;
				record->Write_Recording(wr_buffer, &dwWrittenSize, record);
				//播放游戏胜利音效
				mciSendString(TEXT("close bgm "), NULL, 0, NULL);
				mciSendString(TEXT("open 胜利.wav alias victory"), NULL, 0, NULL);
				mciSendString(TEXT("play victory"), NULL, 0, NULL);
				MessageBoxA(hWnd, "恭喜你赢得胜利", "吃豆子提示", MB_OK);
				goto END_GAME;
			}

		}//end_if (game_visible == TRUE)
		else if(begin_game==TRUE&&game_visible==FALSE)
		{
			begin_game = FALSE;
			//重置游戏
			//UpdateWindow(hWnd);
		}

		if (record_visible == TRUE)
		{
			HDC hdc_record = GetDC(hwnd_record);

			TCHAR line_record[100] = { 0 };//行缓冲区，用于存储每一行的字符
			
				record->Output_Recording(ch_buffer, &dwReadSize);
				for (int i = 0, y = 0; i < dwReadSize; i++)
				{
					int j = 0;
					while (ch_buffer[i] != '\r')
					{
						line_record[j] = ch_buffer[i];
						j++;
						i++;
					}
					i++;
					TextOut(hdc_record, 0, y, line_record, _tcslen(line_record));
					y += 30;
					for (int k = 0; k < 100; k++) line_record[k] = '\0';
				}
			
		}
		else
		{
			;
			//MessageBoxA(hWnd, "历史记录不可见", "吃豆子提示", MB_OK);
		}
		
		
		
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		
		
	}//end_while (1)
	
	
	
	
END_GAME :	
	Realese(e1);
	Realese(e2);
	Realese(e3);
	Realese(e4);
	Realese(p);
	for (int i = 0; i < STAGE_COUNT; i++)
	{
		Realese(MapArray[i]);
	}
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY);
	wcex.lpszClassName = L"GAME";
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

//游戏界面回调函数
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
		case GAME_QUIT:
		{
			int result=MessageBoxA(hWnd, "此时退出将不会保存游戏记录！\n是否要退出？", "吃豆人提示", MB_YESNO| MB_ICONEXCLAMATION);
			switch (result)
			{
			case IDYES: ::exit(0);//退出程序
			case IDNO: break;
			}
			
			break;
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
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		

	
		
		Gobject::pStage->DrawMap(hdc);//绘制地图

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_CREATE:
	{
		//创建退出按钮
		CreateWindow(
			TEXT("button"), TEXT("退出游戏"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			700, 400, 100, 50,
			hWnd, (HMENU)GAME_QUIT, hInst, NULL
		);
		//显示提示信息
		CreateWindow(
			TEXT("static"), 
			TEXT("提示：\n键盘“↑”控制向上运动\n键盘“↓”控制向下运动\n键盘“←”控制向左运动\n键盘“→”控制向右运动"),
			WS_CHILD | WS_VISIBLE | SS_LEFT,
			700, 500, 200, 200,
			hWnd, (HMENU)99, hInst, NULL
		);
		
		break;
	}
	case WM_DESTROY:
	{
		HWND hwnd_record = FindWindow(L"RECORD", L"历史记录");
		HWND hwnd_game = FindWindow(L"GAME", L"游戏界面");
		HWND hwnd_main = FindWindow(L"MAIN", L"主菜单");
		DestroyWindow(hwnd_record);

		DestroyWindow(hwnd_main);
		DestroyWindow(hwnd_game);
		PostQuitMessage(0);
		::exit(0);//退出程序
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
//主菜单窗口回调函数
LRESULT CALLBACK WndProc_main(HWND hwnd_main, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case MUSIC://背景音乐控制
		{
			int state = IsDlgButtonChecked(hwnd_main, MUSIC);//判断按钮选中状态
			if (state == BST_CHECKED)//按钮被选中
			{
				MessageBoxA(hwnd_main, "音乐暂停", "吃豆子提示", MB_OK| MB_ICONERROR);
				mciSendString(TEXT("pause bgm"), NULL, 0, NULL);
			}
			else if (state == BST_UNCHECKED)//按钮未被选中
			{
				MessageBoxA(hwnd_main, "音乐播放", "吃豆子提示", MB_OK);
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
		case GAME_RECORDING://历史记录
		{
			HWND hwnd_record = FindWindow(L"RECORD", L"历史记录");
			ShowWindow(hwnd_record, SW_RESTORE);

			UpdateWindow(hwnd_record);

			HWND hwnd_game = FindWindow(L"GAME", L"游戏界面");
			ShowWindow(hwnd_game, SW_HIDE);

			UpdateWindow(hwnd_game);
			ShowWindow(hwnd_main, SW_HIDE);

			UpdateWindow(hwnd_main);
			break;
		}
		case BEGIN_GAME://开始游戏
		{
			HWND hwnd_game = FindWindow(L"GAME", L"游戏界面");
			ShowWindow(hwnd_game, SW_RESTORE);

			UpdateWindow(hwnd_game);

			HWND hwnd_record = FindWindow(L"RECORD", L"历史记录");
			ShowWindow(hwnd_record, SW_HIDE);

			UpdateWindow(hwnd_record);
			ShowWindow(hwnd_main, SW_HIDE);

			UpdateWindow(hwnd_main);
			break;
		}
		case QUIT://退出游戏
		{
			int result = MessageBoxA(hwnd_main, "是否退出游戏？", "吃豆人提示", MB_YESNO | MB_ICONEXCLAMATION);
			switch (result)
			{
			case IDYES: ::exit(0);//退出程序
			case IDNO: break;
			}
		}
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd_main, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hwnd_main);
			break;
		default:
			return DefWindowProc(hwnd_main, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd_main, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		HDC  hMemDC = CreateCompatibleDC(hdc);
		HBITMAP hBmp = (HBITMAP)LoadImage(NULL, L"background.bmp", IMAGE_BITMAP, 800, 800, LR_LOADFROMFILE);
		SelectObject(hMemDC, hBmp);

		SetStretchBltMode(hdc, HALFTONE);


		StretchBlt(hdc, 0, 0, 1000, 1000, hMemDC, 0, 0, 800, 800, SRCCOPY);
		DeleteObject(hBmp);
		DeleteDC(hMemDC);
		EndPaint(hwnd_main, &ps);
	}
	break;
	case WM_CREATE:
	{
		//创建游戏记录控制按钮
		CreateWindow(
			TEXT("button"), TEXT("游戏记录"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			450, 500, 100, 50,
			hwnd_main, (HMENU)GAME_RECORDING, hInst, NULL
		);
		
		//创建开始游戏按钮
		CreateWindow(
			TEXT("button"), TEXT("开始游戏"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			450, 400, 100, 50,
			hwnd_main, (HMENU)BEGIN_GAME, hInst, NULL
		);
		//创建退出按钮
		CreateWindow(
			TEXT("button"), TEXT("退出游戏"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			450, 600, 100, 50,
			hwnd_main, (HMENU)QUIT, hInst, NULL
		);
		//创建音乐播放控制按钮
		CreateWindow(
			TEXT("button"), TEXT("音乐暂停"),
			WS_CHILD | WS_VISIBLE | BS_LEFT | BS_AUTOCHECKBOX,
			450, 700, 100, 26,
			hwnd_main, (HMENU)MUSIC, hInst, NULL
		);
		//输出游戏图片

		HWND hBtn = CreateWindow(
			TEXT("BuTtOn"), TEXT(" "),
			WS_CHILD | WS_VISIBLE | BS_BITMAP,
			300, 50, 400, 300,
			hwnd_main, (HMENU)100, hInst, NULL
		);
		HBITMAP  hBmp = (HBITMAP)LoadImage(NULL, L"pacman.bmp",
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		LRESULT  ret = SendMessage(hBtn, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp);
		break;
	}
	case WM_DESTROY:
	{
		HWND hwnd_record = FindWindow(L"RECORD", L"历史记录");
		HWND hwnd_game = FindWindow(L"GAME", L"游戏界面");
		HWND hwnd_main = FindWindow(L"MAIN", L"主菜单");
		DestroyWindow(hwnd_record);
		DestroyWindow(hwnd_game);
		DestroyWindow(hwnd_main);
		PostQuitMessage(0);
		::exit(0);//退出程序
		break;
	}
	default:
		return DefWindowProc(hwnd_main, message, wParam, lParam);
	}
	return 0;
}
//过程记录窗口回调函数
LRESULT CALLBACK WndProc_record(HWND hwnd_record, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case RECORDING_BACK:
		{
			HWND hwnd_main = FindWindow(L"MAIN", L"主菜单");
			ShowWindow(hwnd_main, SW_RESTORE);

			UpdateWindow(hwnd_main);
			ShowWindow(hwnd_record, SW_HIDE);

			UpdateWindow(hwnd_record);
			break;
		}
		
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd_record, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hwnd_record);
			break;
		default:
			return DefWindowProc(hwnd_record, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd_record, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		HDC  hMemDC = CreateCompatibleDC(hdc);
		HBITMAP hBmp = (HBITMAP)LoadImage(NULL, L"bg_record.bmp", IMAGE_BITMAP, 800, 800, LR_LOADFROMFILE);
		SelectObject(hMemDC, hBmp);

		SetStretchBltMode(hdc, HALFTONE);

		
		StretchBlt(hdc, 0, 0, 1000, 1000, hMemDC, 0, 0, 800, 800, SRCCOPY);
		DeleteObject(hBmp);
		DeleteDC(hMemDC);

		EndPaint(hwnd_record, &ps);
	}
	break;
	case WM_CREATE:
	{
		//记录窗口创建返回控制按钮
		CreateWindow(
			TEXT("button"), TEXT("返回主菜单"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			750, 715, 100, 50,
			hwnd_record, (HMENU)RECORDING_BACK, hInst, NULL
		);
		break;
	}
	case WM_DESTROY:
	{
		HWND hwnd_record = FindWindow(L"RECORD", L"历史记录");
		HWND hwnd_game = FindWindow(L"GAME", L"游戏界面");
		HWND hwnd_main = FindWindow(L"MAIN", L"主菜单");

		DestroyWindow(hwnd_game);
		DestroyWindow(hwnd_main);
		DestroyWindow(hwnd_record);
		PostQuitMessage(0);
		::exit(0);//退出程序
		break;
	}
	default:
		return DefWindowProc(hwnd_record, message, wParam, lParam);
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