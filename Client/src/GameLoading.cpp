
#include "Stdafx.h"
#include <process.h>
#include "gameloading.h"
#include "gameapp.h"
#include "resource.h"

#include "GameConfig.h"
#include "UIsystemform.h"

#include "GlobalVar.h"

extern HINSTANCE g_hInstance;

extern CGameMovie*	m_pMovie;


bool DrawBMPFile(RECT rect, LPTSTR pszFile, HDC hDC) 
{ 
	HDC hdcCompatible = CreateCompatibleDC(hDC); 

	HBITMAP   hBitmap;  
	//×°ÔØÍ¼Æ¬  
	hBitmap=(HBITMAP)LoadImage(NULL,pszFile,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);  

	if(!hBitmap)
		return false;  

	BITMAP   bm;  
	GetObject(hBitmap,   sizeof(BITMAP),   &bm);  

	HBITMAP old = (HBITMAP)SelectObject(hdcCompatible, hBitmap);
	
	BitBlt(hDC, rect.right - bm.bmWidth, rect.bottom - bm.bmHeight,bm.bmWidth,bm.bmHeight,hdcCompatible,0,0,SRCCOPY);

	SelectObject(hdcCompatible, old);

	DeleteObject(hBitmap);
	DeleteObject(hdcCompatible);

	return true;
}

LRESULT CALLBACK LoadingProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int i = 0;
	switch(uMsg)
	{
	case WM_INITDIALOG:
		{
		}  break;
	//case WM_GETMINMAXINFO:
		//{
		//	MINMAXINFO* pInfo = (MINMAXINFO*)lParam;

		//	int w = GetSystemMetrics(SM_CXSCREEN);
		//	int h = GetSystemMetrics(SM_CYSCREEN);

		//	pInfo->ptMaxSize.x = w;
		//	pInfo->ptMaxSize.y = h;
		//	break;
		//}
	case WM_CHAR:
		if(!g_stUISystem.m_sysProp.m_startOption.bFirst)
		{
			if(!GameLoading::Init()->m_bWait)
			{
				if(wParam == 27)
				{
					GameLoading::Init()->m_bShow = false;

					PostQuitMessage(0);
				}
			}
		}
		break;
	case WM_GRAPHNOTIFY:
		// Get all the events

		m_pMovie->HandleGraphEvent();
		break;
	case WM_CLOSE:
		{
			GameLoading::Init()->m_bShow = false;

			PostQuitMessage(0);
		}  break;
	case WM_PAINT:
		{
			RECT rc;
			GetClientRect(hwndDlg, &rc);

			PAINTSTRUCT ps; 
			HDC hdc = BeginPaint(hwndDlg, &ps); 

			int height = rc.bottom - rc.top - (rc.right - rc.left) * 9.0 / 16.0;

			RECT rect;
			rect.left = rc.left;
			rect.right = rc.right;
			rect.top = rc.top;
			rect.bottom = rc.top + height / 2.0 + 1;

			FillRect(hdc, &rect,(HBRUSH)(COLOR_WINDOWTEXT));

			rect.left = rc.left;
			rect.right = rc.right;
			rect.top = rc.bottom - height / 2.0 - 1;
			rect.bottom = rc.bottom;

			FillRect(hdc, &rect,(HBRUSH)(COLOR_WINDOWTEXT));

			if(!g_stUISystem.m_sysProp.m_startOption.bFirst)
			{
				if(!GameLoading::Init()->m_bWait)
				{
					DrawBMPFile(rc,"memo.bmp", hdc);
				}
			}
			EndPaint(hwndDlg, &ps);
			
			return FALSE;
		}
	case WM_ERASEBKGND:
		{
			return FALSE;
		}
	default:
		return DefWindowProc(hwndDlg, uMsg, wParam, lParam);
	}
	return FALSE;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= LoadingProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KOP));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOWTEXT);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpszClassName	= "Loading";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

unsigned __stdcall LoadingThread(void* param)
{
//#ifdef _DEBUG
//	return 0;
//#endif

    char buff[MAX_PATH] = {0};
    int idx = 0;
    srand((unsigned)time(0));
    GameLoading* load = (GameLoading*)param;

	m_pMovie = new CGameMovie();

	ATOM ret = MyRegisterClass(g_hInstance);
   
    int nSizeW = GetSystemMetrics(SM_CXFULLSCREEN);
	int nSizeH = GetSystemMetrics(SM_CYFULLSCREEN);
	int w = LARGE_RES_X;//g_Config.m_nMovieW;
	int h = 768;//g_Config.m_nMovieH;

	//if(w < 0 && h < 0)
	{
		if(g_stUISystem.m_sysProp.m_videoProp.bFullScreen)
		{
			nSizeW = GetSystemMetrics(SM_CXSCREEN);
			nSizeH = GetSystemMetrics(SM_CYSCREEN);

			w = nSizeW;
			h = nSizeH;
		}
		else if(g_stUISystem.m_sysProp.m_videoProp.bPixel1024)
		{
			w = LARGE_RES_X;
			h = 768;
		}
		else
		{
			w = SMALL_RES_X;
			h = 600;
		}
	}

	///////////////////
	DWORD dwWindowStyle = WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX; 
	
    if(g_stUISystem.m_sysProp.m_videoProp.bFullScreen) // Î±È«ÆÁ
    {
        dwWindowStyle = WS_VISIBLE|WS_POPUP|WS_CLIPCHILDREN;
    }

    RECT rc; SetRect( &rc, 0, 0, w, h);
	AdjustWindowRect( &rc, dwWindowStyle, FALSE );
    
    int nFrameSize   = GetSystemMetrics(SM_CYSIZEFRAME);
	int nCaptionSize = GetSystemMetrics(SM_CXSIZE);

    int nWindowWidth = rc.right - rc.left;
    int nWindowHeight = rc.bottom - rc.top; // nScrHeight + nFrameSize + nCaptionSize;

	if(g_stUISystem.m_sysProp.m_videoProp.bFullScreen)
	{
		load->m_hLoading =  CreateWindowEx(WS_EX_TOPMOST, "Loading", "Loading", WS_POPUP /*& ~WS_TILED*/  , 
			0, 0 ,  nSizeW, nSizeH, NULL, NULL, g_hInstance, NULL);
	}
	else
	{
		load->m_hLoading =  CreateWindowEx(WS_EX_TOPMOST, "Loading", "Loading", WS_POPUP /*& ~WS_TILED*/  , 
		//load->m_hLoading =  CreateWindow("Loading", "Loading", WS_POPUP /*& ~WS_TILED*/  , 
			(nSizeW - nWindowWidth) / 2.0, (nSizeH - nWindowHeight) / 2.0 + nCaptionSize + nFrameSize - 1, nWindowWidth, nWindowHeight, NULL, NULL, g_hInstance, NULL);
	}


	DWORD error = ::GetLastError();

    if(!load->m_hLoading)
    {
        return 1;
    }

	::SetWindowLong(load->m_hLoading, GWL_EXSTYLE, WS_EX_TOOLWINDOW); 
	//SetWindowPos(load->m_hLoading, HWND_TOP, (nSizeW - nWindowWidth) / 2, (nSizeH - nWindowHeight) / 2 + nCaptionSize + nFrameSize - 1, nWindowWidth, nWindowHeight, SWP_NOMOVE| WS_EX_TOPMOST);


	//SetWindowPos(&CWnd::wndTopMost,0,0,0,0,  SWP_NOMOVE|SWP_NOSIZE);
	m_pMovie->Play(load->m_hLoading, "first.wmv");
    ShowWindow(load->m_hLoading, SW_SHOW);
	UpdateWindow(load->m_hLoading);
	
	GameLoading::Init()->Active();

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	m_pMovie->Cleanup();

    return 0;
}

GameLoading* GameLoading::_instance = NULL;

GameLoading* GameLoading::Init(string param)
{
	if(_instance == NULL)
	{
		_instance = new GameLoading(param);
	}

	return _instance;
}

GameLoading::GameLoading(string param)
{
	m_bShow = true;
	m_bWait = true;

	if(param.find("editor")!=-1 || param.find("table_bin")!=-1)
	{
		m_bShow = false;
	}
}


GameLoading::~GameLoading()
{

}


void GameLoading::Create()
{
	if(!m_bShow)
		return;

    unsigned tid;
    _beginthreadex(0, 0, LoadingThread, this, 0, &tid);
}


void GameLoading::Close()
{
	if(!m_bShow)
		return;

	m_bWait = false;

	RECT rc;
	GetClientRect(m_hLoading, &rc);


	int height = rc.bottom - rc.top - (rc.right - rc.left) * 9.0 / 16.0;

	RECT rect;
	rect.left = rc.left;
	rect.right = rc.right;
	rect.top = rc.bottom - height / 2.0 - 1;
	rect.bottom = rc.bottom;

	::InvalidateRect(m_hLoading, &rect, TRUE);

	::UpdateWindow(m_hLoading);

	//if(g_stUISystem.m_sysProp.m_startOption.bFirst)
	{
		while(m_pMovie->IsPlaying())
		{
			::Sleep(1);
		}
	}

    PostMessage(m_hLoading, WM_CLOSE, 0, 0);
    //DestroyWindow(m_hLoading);

	g_stUISystem.m_sysProp.m_startOption.bFirst = false;

	HWND hwnd = g_pGameApp->GetHWND();
	SetForegroundWindow (hwnd);
}

bool GameLoading::Active()
{
	if(GameLoading::Init()->m_bShow)
	{
		::SendMessage(m_hLoading, WM_ACTIVATE, TRUE, TRUE);
		::UpdateWindow(m_hLoading);
		return true;
	}
	//::SetActiveWindow(m_hLoading);

	return false;
}