#include "stdafx.h"
#include "timerRandGraph_ump40.h"
#include "ClassQueue.h"

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HDC g_hdc;

Queue<int> g_queue(40);

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
ATOM MyRegisterClass(HINSTANCE hInstance);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	// 윈도 클래스 등록
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;

    // 기본 메시지 루프입니다.
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TIMERRANDGRAPHUMP40));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TIMERRANDGRAPHUMP40);
    wcex.lpszClassName  = L"ump40_class";
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(L"ump40_class", L"this_is_ump40_class", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hdc = GetDC(hWnd);

   srand((unsigned int)time(NULL));
   
   // 타이머 설정, 300
   SetTimer(hWnd, FIRST_TIMER, RAND_TIME, NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// WndProc
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_TIMER:
	{
		switch (wParam)
		{
		case FIRST_TIMER:
		{
			if (g_queue.IsFull())
			{
				int getValue = 0;
				g_queue.Get(&getValue);
			}

			int randValue = rand() % 500;
			g_queue.Put(randValue);

			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
		}
	}
	break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
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
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.

			int firstValue = 0;
			g_queue.Peek(&firstValue, 0);

			if (!g_queue.IsEmpty())
			{
				MoveToEx(hdc, 0, firstValue, NULL);
			}

			for (int i = 1; i < g_queue.Count(); i++)
			{
				int peekValue = 0;
				g_queue.Peek(&peekValue, i);

				LineTo(hdc, i * 30, peekValue);
			}

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		KillTimer(hWnd, FIRST_TIMER);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}