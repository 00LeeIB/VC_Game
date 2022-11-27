// GameProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "GameProject.h"

using namespace std;

Concurrency::concurrent_vector<RECT> v;
Concurrency::concurrent_vector<RECT> v_enemy;
Concurrency::concurrent_vector<ENEMY_Status> v_enemy_status;


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

ME_Status ME;

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
wchar_t Rangestr[10];
wchar_t AttackSpeedstr[10];
wchar_t Defensestr[10];
wchar_t HPregenstr[10];
wchar_t Levelstr[10];
wchar_t button[3][10];



bool keyLayout[256];

void MoveCalc(HWND hWnd);

CRITICAL_SECTION g_cs;

bool ME_AttackDelay = true;
bool ENEMY_AttackDelay = true;
bool ENEMY_Thread_End = false;
bool GameStop = false;
bool LvUp_check[7];

int GameTimeInt;
int vcnt;
int ENEMYvcnt;
int g_dx, g_dy;
int g_i;
int enemy_vcnt;
int Stat_Up;



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

    srand((unsigned int)time(NULL));

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


    if (GameLine.left > ME_RECT.left)
    {
        ME_RECT.left = GameLine.left;
        ME_RECT.right = GameLine.left + 20;
    }
    if (GameLine.top > ME_RECT.top)
    {
        ME_RECT.top = GameLine.top;
        ME_RECT.bottom = GameLine.top + 20;
    }
    if (GameLine.right < ME_RECT.right)
    {
        ME_RECT.left = GameLine.right - 20;
        ME_RECT.right = GameLine.right;
    }
    if (GameLine.bottom < ME_RECT.bottom)
    {
        ME_RECT.top = GameLine.bottom - 20;
        ME_RECT.bottom = GameLine.bottom;
    }
}

DWORD WINAPI ME_Attack(LPVOID param) {
    HWND hWnd = (HWND)param;
    RECT Attack_ENEMY;

    Attack_ENEMY.left = ME_RECT.left + 5;
    Attack_ENEMY.right = ME_RECT.right - 5;
    Attack_ENEMY.top = ME_RECT.top + 5;
    Attack_ENEMY.bottom = ME_RECT.bottom - 5;

    v.push_back(Attack_ENEMY);
    int tcnt = vcnt;
    vcnt++;

    int dx = g_dx;
    int dy = g_dy;

    int t_i = g_i;

    while (1) {
        if (GameStop == true)
            continue;
        if (v_enemy.at(t_i).left == 0) {
            v.at(tcnt).left = 0;
            v.at(tcnt).right = 0;
            v.at(tcnt).top = 0;
            v.at(tcnt).bottom = 0;
            ExitThread(0);
        }
            
        if (v_enemy.at(t_i).left < v.at(tcnt).left) {
            v.at(tcnt).left -= 5;
            v.at(tcnt).right -= 5;
        }
        else if (v_enemy.at(t_i).left > v.at(tcnt).left) {
            v.at(tcnt).left += 5;
            v.at(tcnt).right += 5;
        }
        else {
        }

        if (v_enemy.at(t_i).top < v.at(tcnt).top) {
            v.at(tcnt).top -= 5;
            v.at(tcnt).bottom -= 5;
        }
        else if (v_enemy.at(t_i).top > v.at(tcnt).top) {
            v.at(tcnt).top += 5;
            v.at(tcnt).bottom += 5;
        }
        else {
        }
        Sleep(10);

    }

    ExitThread(0);
    return 0;
}

DWORD WINAPI ME_Range(LPVOID param) {
    HWND hWnd = (HWND)param;
    while (1) {
        if (GameStop == true)
            continue;
        ME_RANGE_RECT.left = ME_RECT.left - ME.Range;
        ME_RANGE_RECT.right = ME_RECT.right + ME.Range;
        ME_RANGE_RECT.top = ME_RECT.top - ME.Range;
        ME_RANGE_RECT.bottom = ME_RECT.bottom + ME.Range;

        if (ME_AttackDelay == true) {
            for (int i = 0; i < v_enemy.size(); i++)
            {
                if (true == collisionEllipseCheck(v_enemy.at(i),ME_RANGE_RECT)) {
                    if (ME_AttackDelay == true) {
                        ME_AttackDelay = false;
                        SetTimer(hWnd, ME_ATTACK_DELAY, ME.AttackSpeed, NULL);

                        int enemy_x = v_enemy.at(i).right + v_enemy.at(i).left / 2;
                        int enemy_y = v_enemy.at(i).bottom + v_enemy.at(i).top / 2;

                        int me_x = ME_RECT.left + ME_RECT.right / 2;
                        int me_y = ME_RECT.top + ME_RECT.bottom / 2;

                        g_dx = enemy_x - me_x;
                        g_dy = enemy_y - me_y;

                        g_i = i;

                        DWORD tid;
                        CreateThread(NULL, 0, ME_Attack, hWnd, 0, &tid);
                    }
                }
            }
        }

    }
    return 0;
}



DWORD WINAPI ENEMY_control(LPVOID param) {
    HWND hWnd = (HWND)param;

    RECT ENEMY_RECT;
    ENEMY_Status ENEMY;

    srand((unsigned int)time(NULL));

    ENEMY.Level = GameTimeInt / 5;
    ENEMY.TotalHP = GameTimeInt / 5 + 1;
    ENEMY.HP = ENEMY.TotalHP;
    ENEMY.Power = GameTimeInt / 5 + 1;
    ENEMY.Speed = GameTimeInt / 10 + 2;
    

    //ENEMY 크기 설정
    ENEMY_RECT.left = 20 + rand() % 500;
    ENEMY_RECT.top = 70 + rand() % 500;
    ENEMY_RECT.right = ENEMY_RECT.left + 20;
    ENEMY_RECT.bottom = ENEMY_RECT.top + 20;

    v_enemy_status.push_back(ENEMY);
    v_enemy.push_back(ENEMY_RECT);
    int enemy_tcnt = enemy_vcnt;
    enemy_vcnt++;

    while (1) {
        if (GameStop == true)
            continue;
        if (ME_RECT.left < v_enemy.at(enemy_tcnt).left)
        {
            v_enemy.at(enemy_tcnt).left -= ENEMY.Speed;
            v_enemy.at(enemy_tcnt).right -= ENEMY.Speed;
        }
        else if(ME_RECT.left > v_enemy.at(enemy_tcnt).left)
        {
            v_enemy.at(enemy_tcnt).left += ENEMY.Speed;
            v_enemy.at(enemy_tcnt).right += ENEMY.Speed;
        }
        else {

        }
        if (ME_RECT.top < v_enemy.at(enemy_tcnt).top)
        {
            v_enemy.at(enemy_tcnt).top -= ENEMY.Speed;
            v_enemy.at(enemy_tcnt).bottom -= ENEMY.Speed;
        }
        else if (ME_RECT.top > v_enemy.at(enemy_tcnt).top)
        {
            v_enemy.at(enemy_tcnt).top += ENEMY.Speed;
            v_enemy.at(enemy_tcnt).bottom += ENEMY.Speed;
        }
        else {

        }

        if (true == collisionEllipseCheck(ME_RECT, v_enemy.at(enemy_tcnt))) { //피격
            if (ENEMY_AttackDelay == true) {
                ENEMY_AttackDelay = false;
                if (ENEMY.Power > ME.Defense) 
                    ME.HP = ME.HP - ENEMY.Power + ME.Defense;
                SetTimer(hWnd, ENEMY_ATTACK_DELAY, 1000, NULL);

                if (ME.HP <= 0) {
                    ENEMY_Thread_End = true;
                    break;
                }
                wsprintfW(HPstr, L"%d / %d", ME.HP, ME.TotalHP);
                InvalidateRect(hWnd, nullptr, true);
            }
            else {
            }
        } 

        for (int i = 0; i < v.size(); i++)
        {
            if (true == collisionEllipseCheck(v_enemy.at(enemy_tcnt), v.at(i))) {//타격
                ENEMY.HP -= ME.Power;
                v.at(i).left = 0;
                v.at(i).right = 0;
                v.at(i).top = 0;
                v.at(i).bottom = 0;
            }
            
        }

        if (ENEMY.HP <= 0) {
            v_enemy.at(enemy_tcnt).left = 0;
            v_enemy.at(enemy_tcnt).right = 0;
            v_enemy.at(enemy_tcnt).top = 0;
            v_enemy.at(enemy_tcnt).bottom = 0;
            ME.Exp++;
            ExitThread(0);
        }

        if (ENEMY_Thread_End == true) {
            EnterCriticalSection(&g_cs);
            
            KillTimer(hWnd, HP_REGENERATE);
            KillTimer(hWnd, SCREEN_RESET);
            KillTimer(hWnd, ENEMY_THREAD_CREATE);
            KillTimer(hWnd, ENEMY_ATTACK_DELAY);

            MessageBox(hWnd, GameTimestr, L"종료", MB_OK);
            ExitThread(0);
            LeaveCriticalSection(&g_cs);
            return 0;
        }

        Sleep(40);
    }

    ExitThread(0);
    return 0;
}

#define IDM_BTN_CLICK1   991
#define IDM_BTN_CLICK2   992
#define IDM_BTN_CLICK3   993
#define IDM_BTN_CLICK4   994
#define IDM_BTN_CLICK5   995
#define IDM_BTN_CLICK6   996
#define IDM_BTN_CLICK7   997

HWND g_button[3];
int RandomStat[7][3] = { 0, };
int checked;

void abc(HWND hWnd) {
    GameStop = false;
    SetTimer(hWnd, HP_REGENERATE, 2000, NULL);
    SetTimer(hWnd, SCREEN_RESET, 20, NULL);
    SetTimer(hWnd, ENEMY_THREAD_CREATE, 1000, NULL);
    SetTimer(hWnd, GAME_TIME, 1000, NULL);
    DestroyWindow(g_button[0]);
    DestroyWindow(g_button[1]);
    DestroyWindow(g_button[2]);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    switch (message)
    {
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDM_BTN_CLICK1:
            for (int i = 0; i < 3; i++) {
                if (RandomStat[0][i] != 0)
                    ME.TotalHP += RandomStat[0][i];
            }
            abc(hWnd);
            break;
        case IDM_BTN_CLICK2:
            for (int i = 0; i < 3; i++) {
                if (RandomStat[1][i] != 0)
                    ME.HPregen += RandomStat[1][i];
            }
            abc(hWnd);
            break;
        case IDM_BTN_CLICK3:
            for (int i = 0; i < 3; i++) {
                if (RandomStat[2][i] != 0)
                    ME.Power += RandomStat[2][i];
            }
            abc(hWnd);
            break;
        case IDM_BTN_CLICK4:
            for (int i = 0; i < 3; i++) {
                if (RandomStat[3][i] != 0)
                    ME.AttackSpeed -= RandomStat[3][i];
            }
            abc(hWnd);
            break;
        case IDM_BTN_CLICK5:
            for (int i = 0; i < 3; i++) {
                if (RandomStat[4][i] != 0)
                    ME.Speed += RandomStat[4][i];
            }
            abc(hWnd);
            break;
        case IDM_BTN_CLICK6:
            for (int i = 0; i < 3; i++) {
                if (RandomStat[5][i] != 0)
                    ME.Range += RandomStat[5][i];
            }
            abc(hWnd);
            break;
        case IDM_BTN_CLICK7:
            for (int i = 0; i < 3; i++) {
                if (RandomStat[6][i] != 0)
                    ME.Defense += RandomStat[6][i];
            }
            abc(hWnd);
            break;


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
        ME.Level = 0;
        ME.HP = 10;
        ME.TotalHP = 10;
        ME.HPregen = 1;
        ME.Power = 3;
        ME.AttackSpeed = 1000;
        ME.Speed = 3;
        ME.Range = 100;
        ME.Defense = 0;
        ME.Exp = 0;
        ME.SumExp = 1;
        
        
        //타이머 설정
        SetTimer(hWnd, HP_REGENERATE, 1000, NULL);
        SetTimer(hWnd, SCREEN_RESET, 20, NULL);
        SetTimer(hWnd, ENEMY_THREAD_CREATE, 1000, NULL);
        SetTimer(hWnd, GAME_TIME, 1000, NULL);

        //ME 기본스텟 텍스트 설정
        wsprintfW(Levelstr, L"%d", ME.Level);
        wsprintfW(HPstr, L"%d / %d", ME.HP, ME.TotalHP);
        wsprintfW(HPregenstr, L"%d", ME.HPregen);
        wsprintfW(Powerstr, L"%d", ME.Power);
        wsprintfW(AttackSpeedstr, L"%d", ME.AttackSpeed);
        wsprintfW(Speedstr, L"%d", ME.Speed);
        wsprintfW(Rangestr, L"%d", ME.Range);
        wsprintfW(Defensestr, L"%d", ME.Defense);
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
                ME.HP = ME.HP + ME.HPregen;
                if (ME.HP > ME.TotalHP)
                    ME.HP = ME.TotalHP;
            }
        }
        if (wParam == SCREEN_RESET) {
            wsprintfW(Levelstr, L"%d", ME.Level);
            wsprintfW(HPstr, L"%d / %d", ME.HP, ME.TotalHP);
            wsprintfW(HPregenstr, L"%d", ME.HPregen);
            wsprintfW(Powerstr, L"%d", ME.Power);
            wsprintfW(AttackSpeedstr, L"%d", ME.AttackSpeed);
            wsprintfW(Speedstr, L"%d", ME.Speed);
            wsprintfW(Rangestr, L"%d", ME.Range);
            wsprintfW(Defensestr, L"%d", ME.Defense);
            wsprintfW(Expstr, L"%d / %d", ME.Exp, ME.SumExp);

            if (ME.Exp >= ME.SumExp) {
                ME.Level++;
                ME.Exp = 0;
                ME.SumExp += 1;
                ME.HP = ME.TotalHP;

                GameStop = true;

                KillTimer(hWnd, HP_REGENERATE);
                KillTimer(hWnd, SCREEN_RESET);
                KillTimer(hWnd, ENEMY_THREAD_CREATE);
                KillTimer(hWnd, GAME_TIME);

                for (int i = 0; i < 7; i++) {
                    LvUp_check[i] = true;
                }

                for (int i = 0; i < 7; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        RandomStat[i][j] = 0;
                    }
                    
                }

                for (int i = 0; i < 3; i++)
                {
                    while (1) {
                        Stat_Up = rand() % 7;
                        if (ME.AttackSpeed <= 300 && Stat_Up == 3) {
                            continue;
                        }
                        if (LvUp_check[Stat_Up] == true)
                            break;
                    }
                    switch (Stat_Up)
                    {
                    case 0:
                        RandomStat[0][i] = rand() % 5 + 3;
                        wsprintfW(button[i], L"체력+%d",RandomStat[0][i]);
                        LvUp_check[0] = false;
                        break;
                    case 1:
                        RandomStat[1][i] = rand() % 3 + 2;
                        wsprintfW(button[i], L"체력재생+%d", RandomStat[1][i]);
                        LvUp_check[1] = false;
                        break;
                    case 2:
                        RandomStat[2][i] = rand() % 3 + 1;
                        wsprintfW(button[i], L"데미지+%d", RandomStat[2][i]);
                        LvUp_check[2] = false;
                        break;
                    case 3:
                        RandomStat[3][i] = (rand() % 3) * 50 + 200;
                        wsprintfW(button[i], L"공격속도+%d", RandomStat[3][i]);
                        LvUp_check[3] = false;
                        break;
                    case 4:
                        RandomStat[4][i] = rand() % 5 + 1;
                        wsprintfW(button[i], L"이동속도+%d", RandomStat[4][i]);
                        LvUp_check[4] = false;
                        break;
                    case 5:
                        RandomStat[5][i] = (rand() % 3) * 10 + 40;
                        wsprintfW(button[i], L"사거리+%d", RandomStat[5][i]);
                        LvUp_check[5] = false;
                        break;
                    case 6:
                        RandomStat[6][i] = rand() % 3 + 1;
                        wsprintfW(button[i], L"방어구+%d", RandomStat[6][i]);
                        LvUp_check[6] = false;
                        break;
                    }

                    g_button[i] = CreateWindow(L"button", button[i], WS_CHILD | WS_VISIBLE, 100 + i * 120, 310, 100, 50, hWnd, (HMENU)(991 + Stat_Up), hInst, NULL);
                    
                }
                
            }
            InvalidateRect(hWnd, nullptr, true);
        }
        if (wParam == ENEMY_THREAD_CREATE) {
            DWORD tid;
            CreateThread(NULL, 0, ENEMY_control, hWnd, 0, &tid);

        }
        if (wParam == ENEMY_ATTACK_DELAY) {
            ENEMY_AttackDelay = true;
            KillTimer(hWnd, ENEMY_ATTACK_DELAY);
        }
        if (wParam == ME_ATTACK_DELAY) {
            ME_AttackDelay = true;
            KillTimer(hWnd, ME_ATTACK_DELAY);
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

            //ME_Range 선 그리기
            my_pen = CreatePen(PS_NULL, 1, RGB(255, 0, 0));
            os_pen = (HPEN)SelectObject(hdc, my_pen);
            Ellipse(hdc, ME_RANGE_RECT.left, ME_RANGE_RECT.top, ME_RANGE_RECT.right, ME_RANGE_RECT.bottom);
            SelectObject(hdc, os_pen);
            DeleteObject(my_pen);


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
            my_brush = CreateSolidBrush(RGB(0, 255, 0));
            os_brush = (HBRUSH)SelectObject(hdc, my_brush);

            //나중에 다른 case문으로 변경할내용
            LONG EXP2Bar = ((ME.Exp * 100) / ME.SumExp) + 20;

            Rectangle(hdc, ExpBar.left, ExpBar.top, EXP2Bar, ExpBar.bottom);
            SelectObject(hdc, os_brush);
            DeleteObject(my_brush);


            //Stat 텍스트 출력
            int y = 100;
            TextOut(hdc, 570, y, L"Level", 5);
            TextOut(hdc, 570, y + 30, L"체력", 2);
            TextOut(hdc, 570, y + 60, L"체력재생", 4);
            TextOut(hdc, 570, y + 90, L"데미지", 3);
            TextOut(hdc, 570, y + 120, L"공격속도", 4);
            TextOut(hdc, 570, y + 150, L"이동속도", 4);
            TextOut(hdc, 570, y + 180, L"사거리", 3);
            TextOut(hdc, 570, y + 210, L"방어구", 3);
            

            //Stat
            TextOut(hdc, 720, y, Levelstr, lstrlen(Levelstr));
            TextOut(hdc, 720, y + 30, HPstr, lstrlen(HPstr));
            TextOut(hdc, 720, y + 60, HPregenstr, lstrlen(HPregenstr));
            TextOut(hdc, 720, y + 90, Powerstr, lstrlen(Powerstr));
            TextOut(hdc, 720, y + 120, AttackSpeedstr, lstrlen(AttackSpeedstr));
            TextOut(hdc, 720, y + 150, Speedstr, lstrlen(Speedstr));
            TextOut(hdc, 720, y + 180, Rangestr, lstrlen(Rangestr));
            TextOut(hdc, 720, y + 210, Defensestr, lstrlen(Defensestr));
            
            //GameTime
            TextOut(hdc, 500, 50, GameTimestr, lstrlen(GameTimestr));

            //ME 선 그리기
            Ellipse(hdc, ME_RECT.left, ME_RECT.top, ME_RECT.right, ME_RECT.bottom);

            //ME_Attack 선그리기
            for (int i = 0; i < v.size(); i++)
            {
                Ellipse(hdc, v.at(i).left, v.at(i).top, v.at(i).right, v.at(i).bottom);
            }
            
            //ENEMY 선그리기
            for (int i = 0; i < v_enemy_status.size(); i++)
            {
                switch (v_enemy_status.at(i).Level) //레벨에 맞는 색
                {
                case 0:
                    my_brush = CreateSolidBrush(RGB(255, 0, 0));
                    break;
                case 1:
                    my_brush = CreateSolidBrush(RGB(255, 140, 0));
                    break;
                case 2:
                    my_brush = CreateSolidBrush(RGB(255, 255, 0));
                    break;
                case 3:
                    my_brush = CreateSolidBrush(RGB(0, 128, 0));
                    break;
                case 4:
                    my_brush = CreateSolidBrush(RGB(0, 0, 255));
                    break;
                case 5:
                    my_brush = CreateSolidBrush(RGB(75, 0, 130));
                    break;
                case 6:
                    my_brush = CreateSolidBrush(RGB(128, 0, 128));
                    break;
                default:
                    my_brush = CreateSolidBrush(RGB(256, 256, 256));
                    break;
                }
                os_brush = (HBRUSH)SelectObject(hdc, my_brush);
                Ellipse(hdc, v_enemy.at(i).left, v_enemy.at(i).top, v_enemy.at(i).right, v_enemy.at(i).bottom);
                SelectObject(hdc, os_brush);
                DeleteObject(my_brush);
            }


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
