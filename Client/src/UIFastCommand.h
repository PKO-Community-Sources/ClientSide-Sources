//----------------------------------------------------------------------
// 名称:快捷命令控件
// 作者:lh 2004-11-02
// 用途:用于容纳一个快捷命令，仅只有一个指向外部实体的快捷方式
// 最后修改日期:
//----------------------------------------------------------------------

#pragma once
#include "UICommandCompent.h"

namespace GUI
{
typedef void (*GuiComEvent)(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);

class CCommandObj;
class CFastCommand : public CCommandCompent
{
public:

	bool topBar;

	CFastCommand(CForm& frmOwn);
	CFastCommand(const CFastCommand& rhs);
	CFastCommand& operator=(const CFastCommand& rhs);
	~CFastCommand();
	GUI_CLONE(CFastCommand)

	virtual void		Render();
	virtual bool		MouseRun( int x, int y, DWORD key );
	virtual eAccept		SetCommand( CCommandObj* p, int x, int y );
    virtual void		DragRender();
    virtual CCompent*   GetHintCompent( int x, int y );
    virtual void        RenderHint( int x, int y );

	static void			DelCommand( CCommandObj* p );

	static CFastCommand*	GetFastCommand( int index );		// 查找对应的快捷控件
	static CFastCommand*	FintFastCommand( CCommandObj* p,bool checkSecondary = false );		// 查找对应的快捷控件
	static void				FindFastCommandIndexes( CCommandObj* p,int arr[36] );
	void Exec(bool rightclick = false);
    CCommandObj*		GetCommand()					{ return _pCommand;		}
    CCommandObj*		GetCommand2()					{ return _pCommand2;		}
    void                DelCommand()                    { if( _pCommand ) DelCommand(_pCommand);    }
    void                AddCommand( CCommandObj* p );
    void                AddCommand2( CCommandObj* p );

public:
    GuiComEvent         evtChange;      // 快捷栏发生变化

protected:
    void				_SetSelf();
    static	void		_DragEnd(CGuiData *pSender, int x, int y, DWORD key)	{
        ((CFastCommand*)(pSender))->_DragEnd( x, y, key );
    }
    void				_DragEnd( int x, int y, DWORD key );

protected:
	CCommandObj*		_pCommand;
	CCommandObj*		_pCommand2;

	typedef vector<CFastCommand*> fasts;
	static fasts		_fast;

};

// 用于容纳一个命令实体
class COneCommand : public CCommandCompent
{
public:
	enum eShowStyle
	{
		enumSmall,		// 正常显示图标方式
		enumSale,		// 显示售出
	};
	COneCommand(CForm& frmOwn);
	COneCommand(const COneCommand& rhs);
	COneCommand& operator=(const COneCommand& rhs);
	~COneCommand();
	GUI_CLONE(COneCommand)

	virtual void		Render();
	virtual bool		MouseRun( int x, int y, DWORD key );
	virtual void		DragRender();
	virtual eAccept		SetCommand( CCommandObj* p, int x, int y );
    virtual CCompent*   GetHintCompent( int x, int y );
    virtual void        RenderHint( int x, int y );

	CCommandObj*		GetCommand()					{ return _pCommand;		}
    void                DelCommand();

    void                AddCommand( CCommandObj* p );

    virtual UseComandEvent  GetUseCommantEvent()        { return evtUseCommand; }

	void				SetActivePic( CGuiPic* p )		{ _pActive=p;			}
	void				SetIsShowActive( bool v )		{ _IsShowActive = v;	}
	void				SetShowStyle( eShowStyle v )	{ _eShowStyle=v;		}

public:
	GuiComEvent			evtBeforeAccept;
    GuiComEvent         evtThrowItem;

    UseComandEvent      evtUseCommand;

protected:
	void				_SetSelf();
	void				_Copy( const COneCommand& rhs );
	static	void		_DragEnd(CGuiData *pSender, int x, int y, DWORD key)	{
		((COneCommand*)(pSender))->_DragEnd( x, y, key );
	}
	void				_DragEnd( int x, int y, DWORD key );

protected:
	CCommandObj*		_pCommand;
	eShowStyle			_eShowStyle;
	bool				_IsShowActive;

    CGuiPic*			_pActive;			// 激活图象

};

// 内联函数


}
