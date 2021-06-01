
// ��һ�����ص�Window Edit�ؼ�����������������
// ����windows�ؼ���������ֵ����,ɾ��,�˸�,����,ճ��,���λ�ü���ȷ����Ĵ���
class CInputBox 
{

public:


	CInputBox::CInputBox()
	:_bAccountMode(FALSE), _hEdit(0)
	{

	}

	HWND    GetEditWindow()						{ return _hEdit;  }
	void	SetEditWindow(HWND hEdit)			{ _hEdit = hEdit; }
	void	HandleWindowMsg(DWORD dwMsgType, DWORD dwParam1, DWORD dwParam2);
	
	
	void	SetAccountMode(BOOL bAccount);		// ����Ϊ�ʺ�����ģʽ, ���ε��ո�, ���ŵ�
	void	SetPasswordMode(BOOL bPassword);	// ����Ϊ����ģʽ, �õ�*���ַ���
	void	SetDigitalMode(BOOL bDigitalMode);	// ����Ϊ����ģʽ, ֻ������������
	void	SetMaxNum(int nMaxNum);				// ��������������������
	void	SetMultiMode(BOOL bMultiLine);		// ����Ϊ����ģʽ	
	void	SetSel(int nStartChar, int nEndChar);


	void	SetCursorTail();					// ��������õ�����β��
	char*	RefreshText();
	int		RefreshCursor();
	void	ClearText();
	char*	SetText(const char *pszText);
	void	ReplaceSel(const char *pszRplText, BOOL bCanUndo = TRUE);	//�滻��ǰѡ������ - added by Arcol

protected:

	char	_szText[1024];	// ���������
	int		_nCursorPos;	// ��������ִ��е�λ��
	BOOL	_bAccountMode;	// �Ƿ����ʺ�ģʽ
	HWND	_hEdit;
};


inline void CInputBox::SetMaxNum(int nMaxNum)
{
	::SendMessage(_hEdit, EM_LIMITTEXT, nMaxNum, 0);
}

inline char* CInputBox::RefreshText()
{
	GetWindowText(_hEdit, _szText, 1000); 
	return _szText;
}

inline char* CInputBox::SetText(const char *pszText)
{
	SetWindowText(_hEdit, pszText);
	strcpy(_szText, pszText);
	return _szText;
}

inline void CInputBox::ClearText()
{
	strcpy(_szText, "");
	SetWindowText(_hEdit, "");
}

inline int CInputBox::RefreshCursor()
{
	::SendMessage(_hEdit, EM_GETSEL, (WPARAM)&_nCursorPos, 0);
	return _nCursorPos;
}

inline void CInputBox::SetCursorTail()
{
	int nLen = (int)(strlen(_szText));
	::SendMessage(_hEdit, EM_SETSEL, nLen, nLen);
}

inline void CInputBox::SetSel(int nStartChar, int nEndChar)
{
	::SendMessage(_hEdit, EM_SETSEL, nStartChar, nEndChar);
}

inline void CInputBox::HandleWindowMsg(DWORD dwMsgType, DWORD dwParam1, DWORD dwParam2)
{
	// ����Ϸ���ڵ�windows�ļ������봫�ݵ�edit�ؼ�
	// if(!(dwMsgType==WM_KEYDOWN || dwMsgType==WM_CHAR)) return;

	// if(dwMsgType==WM_IME_NOTIFY || dwMsgType==WM_IME_STARTCOMPOSITION) return;

	if(dwMsgType==WM_KEYDOWN || dwMsgType==WM_KEYUP)
	{
		if(dwParam1==VK_UP) return; // ���ε�UP��, ���ⴥ��Edit�ؼ��Ķ�Ӧ����
		// ::SendMessage(_hEdit, WM_KEYDOWN, dwParam1, dwParam2);
	}
	else if(dwMsgType==WM_CHAR)
	{
		if(_bAccountMode)
		{
			if(dwParam1==' ' || dwParam1==',') return;
		}
	}
	::SendMessage(_hEdit, dwMsgType, dwParam1, dwParam2);
}

inline void CInputBox::ReplaceSel(const char *pszRplText, BOOL bCanUndo)
{
	::SendMessage(_hEdit, EM_REPLACESEL, (WPARAM)bCanUndo, (LPARAM)(LPCTSTR)pszRplText);
}

extern CInputBox g_InputBox;
