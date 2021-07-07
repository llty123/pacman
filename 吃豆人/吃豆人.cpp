// 吃豆人.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "吃豆人.h"
#include"GObject.h"
#include"GMap.h"

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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, HWND &hWnd)
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
    Gmap* MapArray[STAGE_COUNT] = { new Stage_1(),new Stage_1(), new Stage_1() };//存储关卡地图
    Gobject::pStage = MapArray[s_n];
    Enermy::player = p;//敌人追踪的玩家为p
    TCHAR score[] = _T("score：");

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    DWORD t=0;
    HWND hWnd;
    
    if (!InitInstance (hInstance, nCmdShow,hWnd))
    {
        return FALSE;
    }

    HDC hdc = GetDC(hWnd);
    Gobject::pStage->DrawMap(hdc);//绘制地图
    
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;
    
    // 主消息循环:
    while (p->GetTw()!=OVER&&s_n<STAGE_COUNT)
    {
        //HDC hdc = GetDC(hWnd);
       
        TextOut(hdc, 700, 0, score, _tcslen(score));//输出得分情况
        if (p->Win())//玩家胜利
        {
            
            s_n++;
            ResetGobjects();
            if (s_n < STAGE_COUNT)
            {
                MessageBoxA(hWnd , "恭喜过关！","吃豆子提示",MB_OKA);
                Gobject::pStage = MapArray[++s_n];
                RECT screenRect;
                screenRect.top = 0;
                screenRect.left = 0;
                screenRect.right= WLENGTH;
                screenRect.bottom = WHIGHT;
                ::FillRect(hdc, &screenRect, CreateSolidBrush(RGB(255, 255, 255)));
               Gobject::pStage->DrawMap(hdc);
            }
            continue;
        }
        if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        
        if (GetAsyncKeyState(VK_UP)&0x8000)//w控制大嘴向上
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
        MessageBoxA(hWnd, "出师未捷", "吃豆子提示", MB_OKA);

    }
    else
    {
        MessageBoxA(hWnd, "恭喜你赢得胜利", "吃豆子提示", MB_OKA);
    }
    Realese(p);
    


    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        ::exit(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
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
