// LogvwrDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CLogvwrDlg �Ի���
class CLogPacket;
class CLogTypeData;
class CGplTypeData;
class CGplViewDlg;
class CLogvwrDlg : public CDialog
    {
    // ����
public:
    CLogvwrDlg(CWnd* pParent = NULL);   // ��׼���캯��
	~CLogvwrDlg();

    // �Ի�������
    enum {IDD = IDD_LOGVWR_DIALOG};

	// ���˵�����ָ��
	CMenu* m_pMainMenu;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��


    // ʵ��
protected:
    HICON m_hIcon;

    // ���ɵ���Ϣӳ�亯��
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();

    // �������Ϣӳ�亯��
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    DECLARE_MESSAGE_MAP()

private:
	// �Ҽ������˵�����
	CMenu m_PopMenu;

    // ����������ʾԪ�ص�����
    CStatic m_Container;

	// ��ǰ����Ĵ���
	CWnd* m_pActiveWnd;

    // ��ʾ����LOG���б�
    CListCtrl m_LogList;

	// ��Ÿ������LOG��ListCtrl��������ʶ��
	std::vector<CLogTypeData*> m_LGData;

	// ��ʾ����GPL�ĶԻ���
	CGplViewDlg* m_pGplDlg;

	// ��Ÿ������GPL��������
	std::vector<CGplTypeData*> m_GPLData;

	// ���ԡ������ť
    CButton m_btnTestLog;
    CButton m_btnClearLog;

    // ��ʾ���������ֹ�����ͬ������
    CCriticalSection m_CS;

    // LOG������
    CLogPacket* m_pLogPacket;

    // �������а�����
    DWORD m_dwPacketCnt;

public:
    afx_msg void OnBnClickedClearlog();
    afx_msg void OnBnClickedTestlog();
	afx_msg void OnFileTest();
	afx_msg void OnViewAll();
	afx_msg void OnNMRclickLoglist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkLoglist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnViewGpl();
	BOOL HaveThisLGType(const char* szTypeName, CLogTypeData** ppltd);
	BOOL HaveThisGPLType(const char* szTypeName, CGplTypeData** ppgtd);
	BOOL SwitchView(CWnd& ActiveWnd);
	BOOL SwitchView(CString& strActiveWnd);
    void ToggleTopMost();
	afx_msg void OnViewTest();
	afx_msg void OnViewClear();
	afx_msg void OnViewPlaymp3();
	afx_msg void OnViewStopmp3();
	afx_msg void OnViewPlaywave();
	afx_msg void OnTimer(UINT nIDEvent);
	};
