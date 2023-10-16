// USBKamera4.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "USBKamera4.h"
#include <string>
#include <vfw.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
const int BUTTON1_ID = 15;
const int BUTTON2_ID = 16;
const int BUTTON3_ID = 17;
std::string text1 = "tekst w okienku";
std::string fileName = "CHUJ.AVI";
std::string fileNamePhoto = "zdjecie.bmp";

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);
PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);

wchar_t* GetWCSString(char* text);
HWND hwdpCapture;
HWND hWnd;

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
	LoadStringW(hInstance, IDC_USBKAMERA4, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_USBKAMERA4));

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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_USBKAMERA4));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_USBKAMERA4);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

	hWnd = CreateWindowW(szWindowClass, L"Tytul okienka", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	HWND hwndButton1 =
		CreateWindowW(
			L"BUTTON", //klasa
			L"NAGRYWAJ", //tekst na buttonie
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			5, //x
			5, //y
			200, //width
			100, //height
			hWnd, //parent
			(HMENU)BUTTON1_ID, //id buttona
			hInstance,
			nullptr);

	HWND hwndButton2 =
		CreateWindowW(
			L"BUTTON", //klasa
			L"ZROB COS", //tekst na buttonie
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			210, //x
			5, //y
			200, //width
			100, //height
			hWnd, //parent
			(HMENU)BUTTON2_ID, //id buttona
			hInstance,
			nullptr);

	HWND hwndButton3 =
		CreateWindowW(
			L"BUTTON", //klasa
			L"ZROB ZDJECIE", //tekst na buttonie
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			415, //x
			5, //y
			200, //width
			100, //height
			hWnd, //parent
			(HMENU)BUTTON3_ID, //id buttona
			hInstance,
			nullptr);

	hwdpCapture = capCreateCaptureWindow(
		L"Podglad", //window name
		WS_CHILD | WS_VISIBLE,
		400, 200, 600, 400,
		(HWND)hWnd,
		(int)0);

	//wysylanie mesedza
	//laczenie sie ze sterownikiem
	SendMessage(hwdpCapture, WM_CAP_DRIVER_CONNECT, 0, 0L);

	SendMessage(hwdpCapture, WM_CAP_SET_PREVIEWRATE, (WPARAM)(INT)(60), 0);
	SendMessage(hwdpCapture, WM_CAP_SET_PREVIEW, (WPARAM)(BOOL)(true), 0);
	//zaczynanie nagrywania




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
			//case BN_CLICKED:
			//	switch (wmId)
			//	{
		case BUTTON1_ID:
		{
			//zacznij nagranie


			std::wstring stemp1 = std::wstring(fileName.begin(), fileName.end());
			LPCWSTR sw = stemp1.c_str();

			capFileAlloc(hwdpCapture, (1024L * 1024L * 5));
			SendMessage(hwdpCapture, WM_CAP_FILE_SET_CAPTURE_FILE, 0, (LPARAM)(LPTSTR)(sw));

			CAPTUREPARMS  captureParams;

			capCaptureGetSetup(hwdpCapture, &captureParams, sizeof(CAPTUREPARMS));

			captureParams.fYield = true;
			captureParams.fAbortLeftMouse = false;
			captureParams.fAbortRightMouse = true;
			captureParams.vKeyAbort = VK_ESCAPE;
			captureParams.dwRequestMicroSecPerFrame = 33332;
			//captureParams.wStepCaptureAverageFrames = 30;
			//captureParams.prev

			capCaptureSetSetup(hwdpCapture, &captureParams, sizeof(CAPTUREPARMS));

			//SendMessage(hwdpCapture, WM_CAP_SET_SEQUENCE_SETUP, );
			SendMessage(hwdpCapture, WM_CAP_SEQUENCE, 0, 0L);

			//UpdateWindow(hWnd);
		}
		break;

		case BUTTON2_ID:
		{
			//if (text1 == "ZMIENIONE")
				//text1 = "TEST";
			//else
				//text1 = "ZMIENIONE";

			//InvalidateRect(hWnd, nullptr, true);

			capCaptureStop(hwdpCapture);
		}
		break;
		case BUTTON3_ID:
		{
			//if (text1 == "ZMIENIONE")
				//text1 = "TEST";
			//else
				//text1 = "ZMIENIONE";

			//InvalidateRect(hWnd, nullptr, true);

			//capCaptureStop(hwdpCapture);\

			//HWND camhwnd;
			HDC hdc;
			HDC hdcMem;
			PAINTSTRUCT ps;
			HBITMAP hbm = NULL;
			RECT rc;

			capGrabFrame(hwdpCapture);
			capEditCopy(hwdpCapture);

			//Copy the clipboard image data to a HBITMAP object called hbm
			hdc = BeginPaint(hwdpCapture, &ps);
			hdcMem = CreateCompatibleDC(hdc);
			if (hdcMem != NULL)
			{
				if (OpenClipboard(hwdpCapture))
				{
					hbm = (HBITMAP)GetClipboardData(CF_BITMAP);
					SelectObject(hdcMem, hbm);
					GetClientRect(hwdpCapture, &rc);
					CloseClipboard();
				}
			}
			//Save hbm to a .bmp file called Frame.bmp
			PBITMAPINFO pbi = CreateBitmapInfoStruct(hWnd, hbm);

			CreateBMPFile(hWnd, (LPTSTR)L"photo.bmp", pbi, hbm, hdcMem);

			SendMessage(hwdpCapture, WM_CAP_DRIVER_CONNECT, 0, 0);
			SendMessage(hwdpCapture, WM_CAP_SET_SCALE, true, 0);
			SendMessage(hwdpCapture, WM_CAP_SET_PREVIEWRATE, 66, 0);
			SendMessage(hwdpCapture, WM_CAP_SET_PREVIEW, true, 0);
		}
		break;
		//}
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
		std::wstring stemp = std::wstring(text1.begin(), text1.end());
		LPCWSTR sw = stemp.c_str();

		auto textWindw = TextOut(hdc, 700, 10, sw, wcslen(sw));

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

wchar_t* GetWCSString(char* text)
{
	// Calculate the length of the wide character string
	int wideStrLength = MultiByteToWideChar(CP_ACP, 0, text, -1, NULL, 0);

	// Allocate memory for the wide character string
	wchar_t* myWideString = new wchar_t[wideStrLength];

	// Convert the narrow character string to wide character string
	MultiByteToWideChar(CP_ACP, 0, text, -1, myWideString, wideStrLength);

	return myWideString;
}


void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC)
{
	HANDLE hf;                  // file handle
	BITMAPFILEHEADER hdr;       // bitmap file-header
	PBITMAPINFOHEADER pbih;     // bitmap info-header
	LPBYTE lpBits;              // memory pointer
	DWORD dwTotal;              // total count of bytes
	DWORD cb;                   // incremental count of bytes
	BYTE* hp;                   // byte pointer
	DWORD dwTmp;

	pbih = (PBITMAPINFOHEADER)pbi;
	lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

	if (!lpBits)
	{
		MessageBox(hwnd, L"GlobalAlloc", L"Error", MB_OK);
	}
	// Retrieve the color table (RGBQUAD array) and the bits
	// (array of palette indices) from the DIB.
	if (!GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS))
	{
		MessageBox(hwnd, L"GetDIBits", L"Error", MB_OK);
	}
	// Create the .BMP file.
	hf = CreateFile(pszFile, GENERIC_READ | GENERIC_WRITE, (DWORD)0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	if (hf == INVALID_HANDLE_VALUE)
	{
		MessageBox(hwnd, L"CreateFile", L"Error", MB_OK);
	}

	hdr.bfType = 0x4d42;  // File type designator "BM" 0x42 = "B" 0x4d = "M"
// Compute the size of the entire file.
	hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;
	// Compute the offset to the array of color indices.
	hdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD);
	// Copy the BITMAPFILEHEADER into the .BMP file.
	if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER), (LPDWORD)&dwTmp, NULL))
	{
		MessageBox(hwnd, L"WriteFileHeader", L"Error", MB_OK);
	}
	// Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
	if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD), (LPDWORD)&dwTmp, NULL))
	{
		MessageBox(hwnd, L"WriteInfoHeader", L"Error", MB_OK);
	}
	// Copy the array of color indices into the .BMP file.
	dwTotal = cb = pbih->biSizeImage;
	hp = lpBits;
	if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL))
	{
		MessageBox(hwnd, L"WriteFile", L"Error", MB_OK);
	}
	// Close the .BMP file.
	if (!CloseHandle(hf))
	{
		MessageBox(hwnd, L"CloseHandle", L"Error", MB_OK);
	}

	// Free memory.
	GlobalFree((HGLOBAL)lpBits);
}

PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{
	BITMAP bmp;
	PBITMAPINFO pbmi;
	WORD cClrBits;
	// Retrieve the bitmap color format, width, and height.
	if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
	{
		MessageBox(hwnd, L"GetObject", L"Error", MB_OK);
	}
	// Convert the color format to a count of bits.
	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else cClrBits = 32;

	// Allocate memory for the BITMAPINFO structure. (This structure
	// contains a BITMAPINFOHEADER structure and an array of RGBQUAD
	// data structures.)

	if (cClrBits != 24)
	{
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << cClrBits));
	}
	// There is no RGBQUAD array for the 24-bit-per-pixel format.
	else
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));

	// Initialize the fields in the BITMAPINFO structure.
	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	if (cClrBits < 24)
	{
		pbmi->bmiHeader.biClrUsed = (1 << cClrBits);
	}
	// If the bitmap is not compressed, set the BI_RGB flag.
	pbmi->bmiHeader.biCompression = BI_RGB;

	// Compute the number of bytes in the array of color
	// indices and store the result in biSizeImage.
	// For Windows NT, the width must be DWORD aligned unless
	// the bitmap is RLE compressed. This example shows this.
	// For Windows 95/98/Me, the width must be WORD aligned unless the
	// bitmap is RLE compressed.
	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8 * pbmi->bmiHeader.biHeight;
	// Set biClrImportant to 0, indicating that all of the
	// device colors are important.
	pbmi->bmiHeader.biClrImportant = 0;

	return pbmi; //return BITMAPINFO
}