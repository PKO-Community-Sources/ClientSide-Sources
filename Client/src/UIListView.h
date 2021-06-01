//----------------------------------------------------------------------
// ����:�����б�ؼ�
// ����:lh 2004-08-02
// ���˼��:CList+������
// ����޸�����:2004-10-09
//----------------------------------------------------------------------

#pragma once
#include "uilist.h"

namespace GUI
{

class CListView : public CCompent
{
public:
	enum eStyle
	{
		eSimpleTitle=0,		// �򵥱�ͷ,����һ��ͼƬ
		eWindowTitle,		// ��windowsһ���ı�ͷ,��Ҫ����ÿһ����ͷ��ͼƬ
		eNoTitle,			// û�б�����
	};

	class CListTitle	// �б�ͷ���
	{
	public:
		CListTitle( CListView* pList ) : _pList(pList) {}
		virtual ~CListTitle(){}
		virtual void		Init() {}
		virtual void		SetColumnWidth( unsigned int nCol, unsigned int width ) {}
		virtual void		Render() {}
		virtual void		Refresh() {}
		virtual bool		MouseRun( int x, int y, DWORD key ) {return false;	}
		virtual void		SetParent( CListView* p ) {}
		virtual CImage*		GetImage( int index )				{return NULL;	}
		virtual void		SetAlpha( BYTE alpha ){}

	public:
		int		GetColumnWidth( unsigned int nCol ) { return _pList->GetList()->GetItems()->GetColumnWidth(nCol); }

	protected:
		CListView*			_pList;

	};

	class CImageTitle : public CListTitle		// �򵥷��,��һ������
	{
	public:
		CImageTitle( CListView* pList ) : CListTitle(pList), _pImage(new CImage(*pList->GetForm())) {}

		virtual void Init(){ 
			_pImage->SetSize( _pList->GetWidth(), _pList->GetColumnHeight() ); 
			_pImage->evtMouseDown = CListView::OnColumnClick;
		}
		virtual void		Render()								{ _pImage->Render();					}
		virtual void		Refresh()								{ _pImage->Refresh();					}
		virtual bool		MouseRun( int x, int y, DWORD key )		{ return _pImage->MouseRun(x,y,key);	}
		virtual void		SetParent( CListView* p )				{ _pImage->SetParent(p);				}
		virtual CImage*		GetImage( int index )					{ return _pImage;						}
		virtual void		SetAlpha( BYTE alpha )					{ _pImage->SetAlpha(alpha);				}

	private:
		CImage*				_pImage;			// �򵥷��ʱ����ʾ��Title����

	};

	class CWindowsTitle : public CListTitle		// Windows���б����
	{
	public:
		CWindowsTitle( CListView* pList );
		virtual void		Init();
		virtual void		SetColumnWidth( unsigned int nCol, unsigned int width );

		virtual void		Render()								{ _pColumn->Render();				}
		virtual void		Refresh()								{ _pColumn->Refresh();				}
		virtual bool		MouseRun( int x, int y, DWORD key )		{return _pColumn->MouseRun(x,y,key);}
		virtual void		SetParent( CListView* p )				{ _pColumn->SetParent(p);			}
		virtual CImage*		GetImage( int index ) { return dynamic_cast<CImage*>(_pColumn->GetIndex( index )); }
		virtual void		SetAlpha( BYTE alpha )					{ _pColumn->SetAlpha(alpha);		}

	private:
		static void			_InitColumnPos( CCompent* pThis, unsigned int index );

	private:
		CContainer*			_pColumn;

	};

public:
	CListView(CForm& frmOwn, int nCol, eStyle eTitle );
	CListView( const CListView& rhs );
	CListView& operator=(const CListView& rhs);
	virtual ~CListView(void);
	GUI_CLONE(CListView)

	virtual void		Init();
	virtual void		Render();
	virtual void		Refresh();
	virtual bool		MouseRun( int x, int y, DWORD key );
	virtual bool		MouseScroll( int nScroll );
	virtual void		SetAlpha( BYTE alpha );

	virtual bool		IsHandleMouse()					{ return true;			}

	virtual void		OnActive()	{ CCompent::OnActive(); _pList->OnActive();	}
	virtual void		OnLost()	{ CCompent::OnLost(); _pList->OnLost();		}
	virtual bool		OnKeyDown( int key );
	virtual bool		SetShowRow( int n );			// ������ʾʱ���и�,��ı�List�ܸ߶�
	virtual void		SetMargin( int left, int top, int right, int bottom );

public:
	CList*				GetList()						{ return _pList;			}
	CListTitle*			GetTitle()						{ return _pTitle;			}
	CImage*				GetColumnImage(int index) {	return _pTitle->GetImage(index);}

	void				SetColumnHeight(unsigned int v) { _nColumnHeight = v;		}
	unsigned int		GetColumnHeight()				{ return _nColumnHeight;	}
	static	void		OnColumnClick(CGuiData *pSender, int x, int y, DWORD key) {
		((CListView*)(pSender->GetParent()->GetParent()))->_ColumnClick( pSender );
	}

	CItemRow*			AddItemRow();
	bool				UpdateItemObj( int nRow, int nCol, CItemObj* pObj );
	CItemObj*			GetItemObj( int nRow, int nCol );

public:
	GuiEvent			evtColumnClick;				// ������б�ͷ

private:
	void				_ColumnClick( CGuiData* pSender ){	if( evtColumnClick ) evtColumnClick(pSender); }

	void				_SetSelf(const CListView& rhs);

protected:
	CListTitle*			_pTitle;
	eStyle				_eTitle;			// ��ͷ���:ΪeSimpleTitle��ʹ��_pImage,����ʹ��_pColumn

	CList*				_pList;
	unsigned int		_nColumnHeight;		// �б�ͷ�߶�

};

}
