//----------------------------------------------------------------------
// 名称:输入框
// 作者:lh 2004-07-08
// 设计思想:可支持文字,图象,图象部分未完成
// 最后修改日期:2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uiCompent.h"
#include "uiForm.h"

namespace GUI
{

class CTextButton;
class CEdit : public CCompent
{
public:
	CEdit(CForm& frmOwn);
	CEdit( const CEdit& rhs );
	CEdit& operator=(const CEdit& rhs);
	virtual ~CEdit(void);
	GUI_CLONE(CEdit)

	virtual void	Init();
	virtual void	Refresh();
	virtual void	SetAlpha( BYTE alpha );
	virtual void	OnActive();
	virtual void	OnLost();

	// 放置于消息循环中，接收键盘消息
	bool OnKeyDown( int key );		
	bool OnChar( char c );

public:	// Get,Set
	static MPTexRect* GetCursorImage()	{ return &_CursorImage;		}

	CGuiPic*		GetImage() { return _pImage; }

	bool			GetIsPassWord()		{ return _bIsPassWord;		}
	bool            GetIsParseText()    { return _bParseText ;     } 
	bool			GetIsDigit()		{ return _bIsDigit;			}
	bool			GetIsMulti()		{ return _bIsMulti;			}
	bool			GetIsWrap()			{ return _bIsWrap;			}

	void			SetIsMulti( bool v ){ _bIsMulti = v;			}
	void			SetIsPassWord( bool v ) { _bIsPassWord = v;		}
	void            SetIsParseText(bool  v ) { _bParseText = v;     } 
	void			SetIsDigit( bool v ){ _bIsDigit = v;			}
	void			SetIsWrap( bool v ) { _bIsWrap = v;				}

	int				GetFontHeight()		{ return _nFontHeight;		}
	void			SetFontHeight( bool v ) { _nFontHeight = v;		}

	int				GetMaxNum()			{ return _nMaxNum;			}
	void			SetMaxNum( int v )	{ _nMaxNum = v;				}

	int             GetMaxVisible()     {return  _nMaxNumVisible;   }
	void            SetMaxNumVisible(int v) { _nMaxNumVisible = v;     }

	int				GetMaxLineNum()		{ return _nMaxLineNum;		}
	void			SetMaxLineNum( int v );
	void			SetCaption( const char * str );
	const char *	GetCaption()		{ return _str.c_str();		}
	void			ReplaceSel( const char * str, BOOL bCanUndo=TRUE );

	virtual void	SetTextColor( DWORD color );
	DWORD			GetTextColor()		{ return _color;			}

	void			SetCursorColor( DWORD  v )	{ _nCursorColor = v;}

	void			SetEnterButton( CTextButton* pButton )	{ _pEnterButton=pButton;	}

public:
	GuiEvent			evtEnter;		// 简单文本按回车时执行的事件
	GuiKeyDownEvent		evtKeyDown;
	GuiKeyCharEvent		evtKeyChar;
    GuiEvent            evtChange;      // caption发生变化

public:
	void			Render();
	static bool		InitCursor( const char* szFile );

	static int		GetCursorX()	{ return _nCursorX;		}
	static int		GetCursorY()	{ return _nCursorY;		}

	void			RefreshText();
	void			RefreshCursor();
	void			ClearText();

protected:
	bool			_IsCursorInHZ( long l, char * s );		// 判断光标是否在一个汉字中间
	void			ShowFocus();		// 显示光标
	void			CorrectCursor();

	// 与剪贴板有关的操作
	void			_Copy();	
	void			_Paste();

	void			_Cut();
	void			_Delete();			// 删除已选择的字符串

	void			_UpdataLines();		// 重新调整显示文字，包括硬回车和软回车

	bool			_isdigit( char c )	{ return (c>='0' && c<='9') || c==VK_BACK || c==VK_RETURN || c==VK_DELETE;	}

private:
	// void		_RefreshCursorPos();	// 根据光标所在行列，计算光标显示的位置
	void        _GetCursorPos(int nCurPos);       
	void		_Copy( const CEdit& rhs );

private:
	static MPTexRect	_CursorImage;	// 光标贴图信息

	static int			_nCursorFlashCount;
	static bool			_bCursorIsShow;
	static int	_nCursorX, _nCursorY;	// 光标应该显示的位置

	CGuiPic*			_pImage;

	DWORD				_color;

	int					_nLeftMargin;
	int					_nTopMargin;

	bool		        _bParseText;	// 是否需要解析图元

	CTextButton*		_pEnterButton;	// 响应回车的按钮

protected:
	string		_str;
	string      _strVisible ; 

	int			_nMaxNum;			// 最大字数
	int         _nMaxNumVisible ;      //控件一次可以容纳的字符数目

	bool		_bIsPassWord;		// 是否密码显示
	bool		_bIsMulti;			// 是否多行输入
	bool		_bIsDigit;			// 是否仅能输入数字
	bool		_bIsWrap;
	int			_nOffset;

protected:		// 多行内部变量	
	int			_nFontHeight;		// 行高，即换到下行的高度
	int			_nMaxLineNum;		// 最多行数，包括硬回车和软回车

protected:		// 光标定位
	int			_nCursorRow;		// 所在行数
	int			_nCursorCol;		// 所在列数,从第0列到length长度列
	int			_nCursorFirstCol;
	int			_nCursorSecondCol;

	DWORD		_nCursorColor;
	DWORD		_nCursorHeight;

};

// 内联函数
inline void	CEdit::SetMaxLineNum( int v ) 
{ 
	if( v > 1 ) {
		_nMaxLineNum = v;
		SetIsMulti( true );
	}
}

}
