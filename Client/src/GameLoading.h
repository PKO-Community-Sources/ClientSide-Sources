
#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include "GameMovie.h"

#define     MAX_LOADING_IMAGES       9

class GameLoading
{
    GameLoading(string param);
public:
    virtual ~GameLoading();

    virtual void Create();
    virtual void Close();

	bool Active();

public:

    HWND        m_hLoading;

	bool		m_bShow;
	bool		m_bWait;

	static GameLoading* Init(string param = "");
private:
	static GameLoading* _instance;
};


