// GameProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "GameProject.h"


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

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
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



RECT GameLine;
RECT StatLine;
RECT ME_RECT;

class Status {
public:
    //스텟
    int HP;
    int TotalHP;
    int Power;
    int Speed;
    int SumExp;
    int Exp;

};

Status ME;

//나의 체력 바 생성
RECT HPBar;

//나의 경험치 바 생성
RECT ExpBar;

//스텟 출력용 문자열
wchar_t HPstr[2];
wchar_t Powerstr[2];
wchar_t Speedstr[2];
wchar_t Expstr[2];


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
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
        ME.TotalHP = 5;
        ME.Power = 1;
        ME.Speed = 5;
        ME.SumExp = 5;
        ME.Exp = 0;
        

        //ME 기본스텟 텍스트 설정
        wsprintfW(HPstr, L"%d / %d", ME.HP, ME.TotalHP);
        wsprintfW(Powerstr, L"%d", ME.Power);
        wsprintfW(Speedstr, L"%d", ME.Speed);
        wsprintfW(Expstr, L"%d / %d", ME.Exp,ME.SumExp);

    }
        break;
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_LEFT:
            ME_RECT.left -= ME.Speed;
            ME_RECT.right -= ME.Speed;
            break;
        case VK_RIGHT:
            ME_RECT.left += ME.Speed;
            ME_RECT.right += ME.Speed;
            break;
        case VK_UP:
            ME_RECT.top -= ME.Speed;
            ME_RECT.bottom -= ME.Speed;
            break;
        case VK_DOWN:
            ME_RECT.top += ME.Speed;
            ME_RECT.bottom += ME.Speed;
            break;
        }

        InvalidateRect(hWnd, NULL, true);
    }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            HBRUSH my_brush, os_brush;

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


            //ME 선 그리기
            Rectangle(hdc, ME_RECT.left, ME_RECT.top, ME_RECT.right, ME_RECT.bottom);





            



            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        
        PostQuitMessage(0);
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
