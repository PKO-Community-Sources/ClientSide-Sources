//----------------------------------------------------------------------
// ����:�̶���ͼ��ʽ�б�
// ����:lh 2004-10-09
// ����޸�����:2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"
#include "Tools.h"

namespace GUI
{

// �̶���С��List,ʵ���Ͻ���һ������+ѡ���,ѡ��ʱ�ƶ�ѡ�������λ��
class CFixList : public CCompent
{
public:
	CFixList(CForm& frmOwn);
	CFixList( const CFixList& rhs );
	CFixList& operator=(const CFixList& rhs);
	~CFixList();
	GUI_CLONE(CFixList)

	virtual void		Render();
	virtual void		Refresh();
	virtual bool		MouseRun( int x, int y, DWORD key );
	virtual bool		MouseScroll( int nScroll );
	virtual bool		SetMaxNum( int n );
	virtual bool		IsHandleMouse()				{ return true;					}

	void				SetMargin( int left, int top, int right, int bottom );
	void				SetString( int n, const char* str );
	const char*			GetString( int n );
	int					GetSelIndex()				{ return _lmtSel.GetCur();		}

	CGuiPic*			GetImage()					{ return _pImage;				}
	CGuiPic*			GetSelectImage()			{ return _pSelect;				}

	void				SetRowHeight( int v )		{ if( v>0 ) _nRowHeight = v;	}
	void				SetRowSpace( int v )		{ if( v>=0 ) _nRowSpace = v;	}

public:	// �¼�
	GuiEvent			evtSelectChange;				// ѡ�����˱仯
	GuiMouseEvent		evtListMouseDown;			// ������б������������

private:
	void				_SetSelf( const CFixList& rhs );
	void				_RefreshSelY();

protected:
	int					_nLeftMargin;	// ����Χ�߿�ľ���
	int					_nTopMargin;
	int					_nRightMargin;
	int					_nBottomMargin;

	int					_nRowHeight;	// �и�
	int					_nRowSpace;		// �о�

	typedef vector<string> vstring;
	vstring				_vstString;

	CGuiPic*			_pImage;		// ����ͼ
	CGuiPic*			_pSelect;		// ѡ���
	int					_nSelX, _nSelY;
	CLimit<int>			_lmtSel;		// ��ǰѡ��

	int					_nHitX1, _nHitY1, _nHitX2, _nHitY2;

};

// ��;:��ÿһ���Ͽɱ�ʶ�Ƿ��Ѿ�ѡ��
class CCheckFixList : public CFixList
{
public:
	CCheckFixList(CForm& frmOwn);
	CCheckFixList( const CCheckFixList& rhs );
	CCheckFixList& operator=(const CCheckFixList& rhs);
	~CCheckFixList();
	GUI_CLONE(CCheckFixList)

	virtual bool			SetMaxNum( int n );
	virtual void			Render();

	void					SetCheckMargin( int left, int top )	{ _nCheckLeftMargin=left; _nCheckTopMargin=top; }
	CGuiPic*				GetCheckImage()						{ return _pImgCheck;							}
	CGuiPic*				GetUnCheckImage()					{ return _pImgUnCheck;							}
	void					SetIsCheck( int index, bool v );
	bool					GetIsCheck( int index );

private:
	void					_SetSelf( const CCheckFixList& rhs );

	static void				_OnCheckDown(CGuiData *pSender, int x, int y, DWORD key) {
		((CCheckFixList*)(pSender))->_OnCheckDown();
	}
	void					_OnCheckDown()				{_checks[_lmtSel.GetCur()] = !_checks[_lmtSel.GetCur()];}

protected:
	typedef vector<bool>	vbool;
	vbool					_checks;

	int						_nCheckLeftMargin, _nCheckTopMargin;	// ��ʾCheck����߾�;ඥ���߾�

	CGuiPic*				_pImgCheck;			// ѡ��
	CGuiPic*				_pImgUnCheck;		// δѡ��

};

// ��������
inline 	void CFixList::SetMargin( int left, int top, int right, int bottom ) 
{ 
	_nLeftMargin = left;
	_nTopMargin = top;
	_nRightMargin= right;
	_nBottomMargin = bottom; 
}

inline 	void CFixList::SetString( int n, const char* str )
{ 
	if( n>0 && n<=_lmtSel.GetMax() )
	{
		_vstString[n] = str; 
	}
}

inline void	CFixList::_RefreshSelY() 
{ 
	_nSelY = GetSelIndex() * (_nRowHeight+_nRowSpace) + _nHitY1; 
}

inline void CCheckFixList::SetIsCheck( int index, bool v )
{ 
	if(index>=0 && index<=_lmtSel.GetMax()) 
		_checks[index]=v; 
}

inline bool CCheckFixList::GetIsCheck( int index )
{ 
	if(index>=0 && index<=_lmtSel.GetMax()) 
		return _checks[index]; 
	return false;
}

}
