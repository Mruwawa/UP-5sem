// Lab2_kamera.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab2_kamera.h"
#include <vfw.h>
#include <atlimage.h>
#include <string>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
const int Button_Photo = 11;
const int Button_StartRecord = 12;
const int Button_StopRecord = 13;
const int Button_Cam1 = 14;
const int Button_Cam2 = 15;

HWND preview;
HWND preview2;

// Forward declarations of functions included in this code module:
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

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB2KAMERA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB2KAMERA));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB2KAMERA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB2KAMERA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   HWND buttonPhoto = CreateWindowW(
	   L"BUTTON",
	   L"Take a photo",
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
	   5, 5,
	   200, 100,
	   hWnd,
	   (HMENU)Button_Photo,
	   hInstance,
	   nullptr
   );

   
   HWND buttonStartRecord = CreateWindowW(
	   L"BUTTON",
	   L"Start Recording",
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
	   5, 110,
	   200, 100,
	   hWnd,
	   (HMENU)Button_StartRecord,
	   hInstance,
	   nullptr
   );
   
   HWND buttonStopRecord = CreateWindowW(
	   L"BUTTON",
	   L"Stop Recording",
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
	   5, 215,
	   200, 100,
	   hWnd,
	   (HMENU)Button_StopRecord,
	   hInstance,
	   nullptr
   );

   HWND buttonCamera1 = CreateWindowW(
	   L"BUTTON",
	   L"Connect camera 1",
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
	   5, 320,
	   200, 100,
	   hWnd,
	   (HMENU)Button_Cam1,
	   hInstance,
	   nullptr
   );

   HWND buttonCamera2 = CreateWindowW(
	   L"BUTTON",
	   L"Connect camera 2",
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
	   5, 425,
	   200, 100,
	   hWnd,
	   (HMENU)Button_Cam2,
	   hInstance,
	   nullptr
   );

   preview = capCreateCaptureWindow(
	   L"Preview 1",
	   WS_VISIBLE | WS_CHILD,
	   210, 5,
	   700, 500,
	   hWnd,
	   (int)0
   );

   preview2 = capCreateCaptureWindow(
	   L"Preview 2",
	   WS_VISIBLE | WS_CHILD,
	   1010, 5,
	   700, 500,
	   hWnd,
	   (int)0
   );

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case Button_Photo:
			{
				capGrabFrame(preview);
				capEditCopy(preview);

				OpenClipboard(preview);
				HBITMAP hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
				CloseClipboard();

				CImage image;
				image.Attach(hBitmap);
				image.Save(_T("photo.bmp"));
				SendMessage(preview, WM_CAP_SET_PREVIEW, true, 0L);
			}
				break;

			case Button_StartRecord:
			{
				
				capFileAlloc(preview, (1024 * 1024 * 5));
				capFileSetCaptureFile(preview, _T("video.avi"));
				
				CAPTUREPARMS params;

				capCaptureGetSetup(preview, &params, sizeof(CAPTUREPARMS));
				params.fYield = true;
				params.fAbortLeftMouse = false;
				params.fAbortRightMouse = false;
				params.vKeyAbort = VK_ESCAPE;
				//params.fCaptureAudio = false; //jakby ktos odpial mikrofon
		
				capCaptureSetSetup(preview, &params, sizeof(CAPTUREPARMS));
				

				capCaptureSequence(preview);

				//SendMessage(preview, WM_CAP_SEQUENCE, 0, 0L);
				
			}
			break;

			case Button_StopRecord:
			{
				capCaptureStop(preview);
			}
			break;
			case Button_Cam1: {
				capDriverConnect(preview, 0);

				SendMessage(preview, WM_CAP_SET_PREVIEWRATE, 60, 0L);
				SendMessage(preview, WM_CAP_SET_PREVIEW, true, 0L); 
			}
			break;
			case Button_Cam2: {
				capDriverConnect(preview2, 0);

				SendMessage(preview2, WM_CAP_SET_PREVIEWRATE, 60, 0L);
				SendMessage(preview2, WM_CAP_SET_PREVIEW, true, 0L);
			}
							  
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
            // TODO: Add any drawing code that uses hdc here...
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

// Message handler for about box.
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
