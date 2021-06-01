
#pragma once


#include <windows.h>
#include <dshow.h>
#pragma comment (lib,"Strmiids.lib")

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			\
	{							\
		if(p)					\
		{						\
			(p)->Release();		\
			(p) = 0;			\
		}						\
	}							
#endif


#define WM_GRAPHNOTIFY WM_USER + 1024


// ����Ƶ����
class CGameMovie
{
public:
	CGameMovie(void);
	virtual ~CGameMovie(void);

public:

	// ����
	bool Play(HWND hWnd, TCHAR* pszFileName);

	// ֹͣ����
	void Stop(void);

	// �Ƿ����ڲ���
	bool IsPlaying(void);

	// ���
	void Cleanup(void);

	// ������Ĵ�����
	HRESULT GetLastError(void) { return _hResult; }

	IMediaEventEx* GetEvent() { return _pEvent; }

	void HandleGraphEvent();

private:

	HRESULT _hResult;
	HWND	_hWnd;

	IGraphBuilder*	_pGraph;
	IMediaControl*	_pControl;
	IVideoWindow*	_pVidWin;
	IMediaPosition*	_pPosition;

	IMediaEventEx* _pEvent;
};

