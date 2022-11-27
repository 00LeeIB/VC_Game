// GameProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "GameProject.h"
#include<ctime>
#include<cmath>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


RECT GameLine;
RECT StatLine;
RECT ME_RECT;
RECT ME_RANGE_RECT;

class Status {
public:
    //스텟
    int HP;
    int TotalHP;
    int Power;
    double AttackSpeed;
    int Speed;
    int SumExp;
    int Exp;
    int Range;

};

Status ME;

//나의 체력 바 생성
RECT HPBar;

//나의 경험치 바 생성
RECT ExpBar;

//스텟 출력용 문자열
wchar_t HPstr[10];
wchar_t Powerstr[10];
wchar_t Speedstr[10];
wchar_t Expstr[10];
wchar_t GameTimestr[10];


bool keyLayout[256];

void MoveCalc(HWND hWnd);

CRITICAL_SECTION g_cs;

bool AttackDelay = true;
bool ENEMY_Thread_End = false;
int GameTimeInt = 0;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GAMEPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMEPROJECT));

    MSG msg;


    bool done = FALSE;

    //Message Loop
    while (!done)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
                if (msg.message == WM_QUIT)
                    done = TRUE;
                else
                {
                    //TranslateMessage(&msg);
                    DispatchMessage(&msg);
                    if (msg.wParam == SCREEN_RESET)
                        MoveCalc(msg.hwnd);
                }

            }

        }
        else {}
            //RenderTest();  //메세지가 없는 경우는 backbone형태로 이 함수가 수행됨.
        
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMEPROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GAMEPROJECT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 900, 650, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

void MoveCalc(HWND hWnd) {
    if (keyLayout[VK_LEFT] == keyLayout[VK_RIGHT]) {

    }
    else if (keyLayout[VK_LEFT]) {
            ME_RECT.left -= ME.Speed;
            ME_RECT.right -= ME.Speed;
    }
    else {
            ME_RECT.left += ME.Speed;
            ME_RECT.right += ME.Speed;
    }

    if (keyLayout[VK_UP] == keyLayout[VK_DOWN]) {
    }
    else if (keyLayout[VK_UP]) {
            ME_RECT.top -= ME.Speed;
            ME_RECT.bottom -= ME.Speed;
    }
    else {
            ME_RECT.top += ME.Speed;
            ME_RECT.bottom += ME.Speed;
    }


    if (20 > ME_RECT.left)
    {
        ME_RECT.left = 20;
        ME_RECT.right = 40;
    }
    if (70 > ME_RECT.top)
    {
        ME_RECT.top = 70;
        ME_RECT.bottom = 90;
    }
    if (520 < ME_RECT.right)
    {
        ME_RECT.right = 520;
        ME_RECT.left = 500;
    }
    if (570 < ME_RECT.bottom)
    {
        ME_RECT.bottom = 570;
        ME_RECT.top = 550;
    }
}
RECT Attack_ENEMY;

DWORD WINAPI ME_Attack(LPVOID param) {
    while (1) {
        if (Attack_ENEMY.right == NULL) {
            //공격X
        }
        else {
            //공격
        }
    }
    return 0;
}

DWORD WINAPI ENEMY_control(LPVOID param) {
    HWND hWnd = (HWND)param;

    RECT ENEMY_RECT;
    Status ENEMY;

    HDC hdc = GetDC(hWnd);

    srand((unsigned int)time(NULL));

    ENEMY.TotalHP = GameTimeInt / 10 + 1;
    ENEMY.Power = GameTimeInt / 10 + 1;
    ENEMY.Speed = GameTimeInt / 20 + 1;
    ENEMY.HP = ENEMY.TotalHP;

    //ENEMY 크기 설정
    ENEMY_RECT.left = 20 + rand() % 500;
    ENEMY_RECT.top = 70 + rand() % 500;
    ENEMY_RECT.right = ENEMY_RECT.left + 20;
    ENEMY_RECT.bottom = ENEMY_RECT.top + 20;


    while (1) {
        if (ME_RECT.left < ENEMY_RECT.left)
        {
            ENEMY_RECT.left -= 1;
            ENEMY_RECT.right -= 1;
        }
        else if(ME_RECT.left > ENEMY_RECT.left)
        {
            ENEMY_RECT.left += 1;
            ENEMY_RECT.right += 1;
        }
        else {

        }
        if (ME_RECT.top < ENEMY_RECT.top)
        {
            ENEMY_RECT.top -= 1;
            ENEMY_RECT.bottom -= 1;
        }
        else if (ME_RECT.top > ENEMY_RECT.top)
        {
            ENEMY_RECT.top += 1;
            ENEMY_RECT.bottom += 1;
        }
        else {

        }
        Rectangle(hdc, ENEMY_RECT.left, ENEMY_RECT.top, ENEMY_RECT.right, ENEMY_RECT.bottom);

        //테스트용 체력표시 
        wchar_t test[10];
        wsprintfW(test, L"%d", ENEMY.HP);
        TextOut(hdc, ENEMY_RECT.left, ENEMY_RECT.top-10, test, lstrlen(test));
        //

        RECT is;
        if (true == IntersectRect(&is, &ME_RECT, &ENEMY_RECT)) {
            if (AttackDelay == true) {
                ME.HP = ME.HP - ENEMY.Power;
                AttackDelay = false;
                SetTimer(hWnd, ATTACK_DELAY_TIMER, 1000, NULL);

                if (ME.HP <= 0) {
                    EnterCriticalSection(&g_cs);
                    ENEMY_Thread_End = true;
                    LeaveCriticalSection(&g_cs);
                    break;
                }
                wsprintfW(HPstr, L"%d / %d", ME.HP, ME.TotalHP);
                InvalidateRect(hWnd, nullptr, true);
            }
            else {
            }
        } //ENEMY_RECT 가 ME_RANGE 에 들어왔을 때
        if (ENEMY_RECT.right <= ME_RANGE_RECT.right && ENEMY_RECT.left >= ME_RANGE_RECT.left
            && ENEMY_RECT.bottom <= ME_RANGE_RECT.bottom && ENEMY_RECT.top >= ME_RANGE_RECT.top) {
            ExitThread(0);
        }

        if (ENEMY_Thread_End == true) {
            EnterCriticalSection(&g_cs);
            
            ReleaseDC(hWnd, hdc);
            KillTimer(hWnd, HP_REGENERATE);
            KillTimer(hWnd, SCREEN_RESET);
            KillTimer(hWnd, ENEMY_THREAD_CREATE);
            KillTimer(hWnd, ATTACK_DELAY_TIMER);

            MessageBox(hWnd, L"종료", L"사망", MB_OK);
            ExitThread(0);
            LeaveCriticalSection(&g_cs);
            return 0;
        }

        Sleep(10);
    }
    ReleaseDC(hWnd, hdc);
    ExitThread(0);
    return 0;
}


DWORD WINAPI ME_Range(LPVOID param) {
    HWND hWnd = (HWND)param;


    while (1) {

        ME_RANGE_RECT.left = ME_RECT.left - ME.Range;
        ME_RANGE_RECT.right = ME_RECT.right + ME.Range;
        ME_RANGE_RECT.top = ME_RECT.top - ME.Range;
        ME_RANGE_RECT.bottom = ME_RECT.bottom + ME.Range;


    }

    return 0;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    switch (message)
    {
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ID_LEFT:
            keyLayout[VK_LEFT] = 1;
            break;
        case ID_RIGHT:
            keyLayout[VK_RIGHT] = 1;
            break;
        case ID_DOWN:
            keyLayout[VK_DOWN] = 1;
            break;
        case ID_UP:
            keyLayout[VK_UP] = 1;
            break;
        }
    }
        break;
    case WM_CREATE:
    {
        //GameLine 크기 설정 20~520, 70~570
        GameLine.left = 20;
        GameLine.top = 70;
        GameLine.right = 520;
        GameLine.bottom = 570;

        //StatLine 크기 설정 550~800, 70~570
        StatLine.left = 550;
        StatLine.top = 70;
        StatLine.right = 800;
        StatLine.bottom = 570;

        //HPBar 설정 20~120, 10~30
        HPBar.left = 20;
        HPBar.top = 10;
        HPBar.right = 120;
        HPBar.bottom = 30;

        //ExpBar 설정 20~120, 40~60
        ExpBar.left = 20;
        ExpBar.top = 40;
        ExpBar.right = 120;
        ExpBar.bottom = 60;

        //ME 크기 설정
        ME_RECT.left = 260;
        ME_RECT.top = 260;
        ME_RECT.right = 280;
        ME_RECT.bottom = 280;

        //ME 기본스텟 설정
        ME.HP = 3;
        ME.TotalHP = 10;
        ME.Power = 1;
        ME.AttackSpeed = 1;
        ME.Speed = 3;
        ME.SumExp = 5;
        ME.Exp = 0;
        ME.Range = 100;
        
        //타이머 설정
        SetTimer(hWnd, HP_REGENERATE, 2000, NULL);
        SetTimer(hWnd, SCREEN_RESET, 20, NULL);
        SetTimer(hWnd, ENEMY_THREAD_CREATE, 1000, NULL);
        SetTimer(hWnd, GAME_TIME, 1000, NULL);

        //ME 기본스텟 텍스트 설정
        wsprintfW(HPstr, L"%d / %d", ME.HP, ME.TotalHP);
        wsprintfW(Powerstr, L"%d", ME.Power);
        wsprintfW(Speedstr, L"%d", ME.Speed);
        wsprintfW(Expstr, L"%d / %d", ME.Exp, ME.SumExp);

        InitializeCriticalSection(&g_cs);

        CreateThread(NULL, 0, ME_Range, hWnd, 0, NULL);


    }
        break;
    case WM_TIMER:
    {
        if (wParam == GAME_TIME) {
            wsprintfW(GameTimestr, L"%d", GameTimeInt);
            GameTimeInt++;
            
        }
        if (wParam == HP_REGENERATE) {
            if(ME.HP==ME.TotalHP){}
            else {
                ME.HP++;
                wsprintfW(HPstr, L"%d / %d", ME.HP, ME.TotalHP);
                
            }
        }
        if (wParam == SCREEN_RESET) {
            InvalidateRect(hWnd, nullptr, true);
        }
        if (wParam == ENEMY_THREAD_CREATE) {
            DWORD tid;
            CreateThread(NULL, 0, ENEMY_control, hWnd, 0, &tid);

        }
        if (wParam == ATTACK_DELAY_TIMER) {
            AttackDelay = true;
            KillTimer(hWnd, ATTACK_DELAY_TIMER);
        }


    }
        break;

    case WM_KEYDOWN:
    {
        //keyLayout[wParam] = 1;
    }
        break;
    case WM_KEYUP:
    {
        keyLayout[wParam] = 0;
    }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            HBRUSH my_brush, os_brush;
            HPEN my_pen, os_pen;

            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            //GameLine 선 그리기
            Rectangle(hdc, GameLine.left, GameLine.top, GameLine.right, GameLine.bottom);

            //StatLine 선 그리기
            Rectangle(hdc, StatLine.left, StatLine.top, StatLine.right, StatLine.bottom);

           
            //HPBar 막대 그리기
            Rectangle(hdc, HPBar.left, HPBar.top, HPBar.right, HPBar.bottom);
            my_brush = CreateSolidBrush(RGB(255, 0, 0));
            os_brush = (HBRUSH)SelectObject(hdc, my_brush);

            //나중에 다른 case문으로 변경할내용
            LONG HP2Bar = ((ME.HP * 100) / ME.TotalHP) + 20;

            Rectangle(hdc, HPBar.left, HPBar.top, HP2Bar, HPBar.bottom);
            SelectObject(hdc, os_brush);
            DeleteObject(my_brush);


            //ExpBar 막대 그리기
            Rectangle(hdc, ExpBar.left, ExpBar.top, ExpBar.right, ExpBar.bottom);

            //Stat 텍스트 출력
            TextOut(hdc, 570, 100, L"체력", 2);
            TextOut(hdc, 570, 130, L"데미지", 3);
            TextOut(hdc, 570, 160, L"이동속도", 4);
            TextOut(hdc, 570, 190, L"경험치", 3);


            //Stat
            TextOut(hdc, 720, 100, HPstr, lstrlen(HPstr));
            TextOut(hdc, 720, 130, Powerstr, lstrlen(Powerstr));
            TextOut(hdc, 720, 160, Speedstr, lstrlen(Speedstr));
            TextOut(hdc, 720, 190, Expstr, lstrlen(Expstr));

            //GameTime
            TextOut(hdc, 500, 50, GameTimestr, lstrlen(GameTimestr));

            Rectangle(hdc, ME_RANGE_RECT.left, ME_RANGE_RECT.top, ME_RANGE_RECT.right, ME_RANGE_RECT.bottom);

            //ME 선 그리기
            Rectangle(hdc, ME_RECT.left, ME_RECT.top, ME_RECT.right, ME_RECT.bottom);

            

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
    {
        
        DeleteCriticalSection(&g_cs);
        PostQuitMessage(0);
    }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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
