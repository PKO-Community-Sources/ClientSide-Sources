// Logvwr.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"       // ������


// CLogvwrApp:
// �йش����ʵ�֣������ Logvwr.cpp
//

class CLogvwrApp : public CWinApp
    {
public:
    CLogvwrApp();

    // ��д
public:
    virtual BOOL InitInstance();

    // ʵ��

    DECLARE_MESSAGE_MAP()
    };

extern CLogvwrApp theApp;

class CLogUtil;
extern CLogUtil theLog;
