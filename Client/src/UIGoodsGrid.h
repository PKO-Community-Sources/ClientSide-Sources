//----------------------------------------------------------------------
// ����:������
// ����:lh 2004-10-27
// ��;:��������ÿһ������,װ�������϶�,ÿһװ���¶������ִ������
// ����޸�����:
//----------------------------------------------------------------------
#pragma once
#include "UICommandCompent.h"
#include "UIScroll.h"

namespace GUI
{
 
typedef void (*GuiThrowItemEvent)(CGuiData *pSender,int id,bool& isThrow);
typedef void (*GuiSwapItemEvent)(CGuiData *pSender,int nFirst, int nSecond, bool& isSwap);
typedef void (*GuiDragInGridEvent)(CGuiData *pSender,CCommandObj* pItem,int nGridID,bool& isAccept);
typedef void (*GuiRMouseGridEvent)(CGuiData *pSender,CCommandObj* pItem,int nGridID);

class CCommandObj;
class CGoodsGrid : public CCommandCompent
{
public:
	enum eShowStyle
	{
		enumSmall,		// ������ʾͼ�귽ʽ
		enumSale,		// ��ʾ�۳�
		enumOwnDef		// �Զ�����ʾ
	};

	CGoodsGrid(CForm& frmOwn);
	CGoodsGrid(const CGoodsGrid& rhs);
	CGoodsGrid& operator=(const CGoodsGrid& rhs);
	~CGoodsGrid();
	GUI_CLONE(CGoodsGrid)

	virtual void	Init();
	virtual void	Render();
	virtual void	Refresh();
	virtual bool	MouseRun( int x, int y, DWORD key );
	virtual bool	MouseScroll( int nScroll );
	virtual eAccept	SetCommand( CCommandObj* p, int x, int y );
	virtual void	Reset();

	virtual bool	IsHandleMouse()					{ return true;			}
	virtual CGuiPic*	GetImage()					{ return _pImage;		}
	virtual void		SetMargin( int left, int top, int right, int bottom );

	virtual void		SetAlpha( BYTE alpha );
	virtual void		DragRender();
    virtual CCompent*   GetHintCompent( int x, int y );
    virtual void        RenderHint( int x, int y );

    virtual UseComandEvent  GetUseCommantEvent()    { return evtUseCommand; }

	CScroll*		GetScroll()						{ return _pScroll;		}

	CGuiPic*		GetUnitImage()					{ return _pUnit;		}
	void			SetIsHint( bool v )				{ _IsShowHint = v;		}

    void            Clear();

public:
	void			SetShowStyle( eShowStyle v )	{ _eShowStyle=v; 		}
 
	bool			SetContent( int nRow, int nCol );
	void			SetSpace( int x, int y );
	void			SetUnitSize( int w, int h );

	int				GetFreeIndex();
    void            GetFreeIndex( int* nFree, int& nCount, int nSize );     // nFreeΪint����,nCountΪ�����ж��ٸ�ֵ��Ч,nSizeΪnFree�Ļ������
	bool			DelItem( int nIndex );
	bool			SetItem( unsigned int nIndex, CCommandObj* pItem );
	bool			SwapItem( unsigned int nFirst, unsigned int nSecond );

public:
	int				GetDragIndex()			{ return _nDragIndex;	}
    int             GetCol()                { return _nCol;         }
	int				GetRow()				{ return _nRow;			}

	CCommandObj*	GetItem( unsigned int nIndex );
	int				GetMaxNum()				{ return _nMaxNum;		}
	int				GetCurNum()				{ return _nCurNum;		}

    int             FindCommand( CCommandObj* p );  // ���ش�����ɹ�,ʧ�ܷ���-1 

	void			SetItemValid( bool v );

	int				GetFirstShow()			{ return _nFirst;		}

	int				GetEmptyGridCount();	// ����пո���Ӹ���
	int				GetUsedGridCount();		// �������ʹ�ø��Ӹ���

public:	// �¼�
    GuiThrowItemEvent   evtThrowItem;
    GuiSwapItemEvent    evtSwapItem;
    GuiDragInGridEvent	evtBeforeAccept;
	GuiRMouseGridEvent	evtRMouseEvent;

    UseComandEvent      evtUseCommand;

private:
	void			_Copy( const CGoodsGrid& rhs );
	void			_SetSelf();
	void			_ClearItem();
	int				_GetHitItem( int x, int y );		// ����-1ʧ�ܣ����򷵻�����

protected:
	static void		_OnScrollChange(CGuiData *pSender) {
		((CGoodsGrid*)(pSender->GetParent()))->_OnScrollChange();
	}
	void			_OnScrollChange();

protected:
	CGuiPic*		_pImage;					// ����
	CScroll*		_pScroll;
	CGuiPic*		_pUnit;						// ��Ԫ��ͼƬ

	CCommandObj**	_pItems;

protected:
	eShowStyle		_eShowStyle;

	int				_nUnitHeight, _nUnitWidth;	// ��Ԫ��� 
	int				_nSpaceX, _nSpaceY;			// ��Ԫ���

	int				_nRow, _nCol;				// ��ʾ������
	int				_nMaxNum;					// ������,���е�������

	// ����Χ�߿�ľ���
	int				_nLeftMargin;
	int				_nTopMargin;
	int				_nRightMargin;
	int				_nBottomMargin;

private:
	static	void	_DragEnd(CGuiData *pSender, int x, int y, DWORD key)	{
		((CGoodsGrid*)(pSender))->_DragEnd( x, y, key );
	}
	void			_DragEnd( int x, int y, DWORD key );

private:
	int				_nStartX, _nStartY;
	int				_nPageShowNum;				// һҳ����ʾ�ĸ���
	int				_nTotalW, _nTotalH;

	int				_nFirst, _nLast;			// ��ʾʱ�ĵ�һ�������һ��

	CCommandObj*	_pDragItem;
	int				_nDragIndex;				// �϶�ʱ������
	int				_nDragOffX, _nDragOffY;
	int				_nDragRow,	_nDragCol;

	int				_nCurNum;					// ��ǰ��Ʒ����
	bool			_IsShowHint;

private:
	static int 		_nTmpX, _nTmpY,  _nTmpRow,	_nTmpCol;

private:
	int _nTmpIndex;

};

// ��������
inline void CGoodsGrid::SetSpace( int x, int y )	
{ 
	if( x >= 0 )	_nSpaceX=x; 
	if( y >= 0 ) 	_nSpaceY=y;	
}

inline void CGoodsGrid::SetUnitSize( int w, int h )	
{
	if( w>0 ) _nUnitWidth=w;
	if( h>0 ) _nUnitHeight=h; 
}

inline CCommandObj* CGoodsGrid::GetItem( unsigned int nIndex ) 
{ 
	if(nIndex>=(unsigned int)_nMaxNum) 
		return NULL; 
	return _pItems[nIndex];	
}

}
