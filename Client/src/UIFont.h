//----------------------------------------------------------------------
// ����:���ڸ���������ʾ�ײ�
// ����:lh 2004-07-15
// ����޸�����:2004-10-20
//----------------------------------------------------------------------
#pragma once
#include "UIRender.h"
#include "compcommand.h"
#include "MPFont.h"

namespace GUI
{
const DWORD COLOR_BLACK = 0xff000000;
const DWORD COLOR_WHITE = 0xffffffff;
const DWORD COLOR_RED = 0xffff0000;
const DWORD COLOR_GREEN = 0xff00ff00;
const DWORD COLOR_BLUE = 0xff0000ff;
const DWORD COLOR_GRAY = 0xff808080;
const DWORD COLOR_CHAR_NAME = COLOR_WHITE;
const DWORD COLOR_SHIP_NAME = 0xff03fdbd;
const DWORD COLOR_SHIP_SEPARATOR = 0xff16074f;
#define SCROLLNEEDSPACE		60

enum ALLIGN{eAlignTop,eAlignCenter,eAlignBottom};

class CGuiFont
{
public:
	CGuiFont();
	~CGuiFont();

	// ����nStyle: 0x0001-����,0x0002-б��,0x0004-���»���
	int		CreateFont( char* font, int size800, int size1024, DWORD dwStyle=0 );
	bool	Init()							{ return !_stfonts.empty();				}
	bool	Clear();

	void	SetScreen( int nScrWidth, int nScrHeight );
	void	SetIndex( unsigned int n );
	void	Render( const char* str, int x, int y, DWORD color ,float size = 1.0f);					// ������ʾ		
	void    RenderScale(const char *str, int x ,int y , DWORD color , float scale);  //������ʾ
	void	BRender( const char* str, int x, int y, DWORD color, DWORD shadow );	// ��Ӱ��ʾ
	void	Render3d( const char* str, D3DXVECTOR3& pos, DWORD color=COLOR_RED );

	void    FrameRender( const char* str, int x, int y );
	void    TipRender( const char* str, int x, int y );

	int		GetWidth( const char* );
	int		GetHeight( const char* );
	void	GetSize( const char* str, int& w, int &h );					// �Ѿ��Զ���������Ļת��
	bool	GetSize( unsigned int dwIndex, const char* str, int& w, int &h );

	void	Begin();
	void	Draw(const char* str, int x, int y, DWORD color );
	void	End();

public:
	CMPFont*		GetFont( unsigned int index=0 );
	void			Render( unsigned int nIndex, const char* str, int x, int y, DWORD color ,float size = 1.0f);
	void			BRender( unsigned int nIndex, const char* str, int x, int y, DWORD color, DWORD shadow );	// ��Ӱ��ʾ

public:		
	static CGuiFont	s_Font;

private:
	typedef vector<CMPFont*> fonts;
	fonts			_fonts;
	CMPFont*		_pFont;
	unsigned int	_nMaxFont;

	struct stFont
	{
		string	strFont;
		DWORD	dwStyle;
		int		size800;
		int		size1024;
	};
	typedef vector<stFont>	stfont;
	stfont		_stfonts;

private:
	SIZE		_size;

};


// ��ʾ�ɾ��е�hint list
// ʹ�÷���:
// 1.Clear���ԭ�е������б�
// 2.PushHint,AddHintHeight,������б�
// 3.ReadyForHint( x, y ) - ׼�����Ƕ���ʾ
// 4.Render - ��ʾhint
class CTextHint
{
public:
	void	Clear();
	void	ReadyForHint( int x, int y );
	void	ReadyForHintGM(int x, int y);//Add by sunny.sun 20080912
	void	Render();

    void    PushHint( const char* str, DWORD color=COLOR_WHITE, int height=5, int font=0, int index = -1, bool shadow = false, DWORD scolor = 0 ); // height:�۳��ָߺ�ĸ߶�
    void    AddHintHeight( int height=6 );													// ���һ������

	int		WriteText( const char* file );		// ����ǰhintд���ı�������д�������

public:
    struct stHint 
    {
        stHint( const char* str, DWORD c, int h, int f, bool sh, unsigned int sc)
			: hint(str), color(c), height(h), offx(0), font(f), width(0), shadow(sh), scolor(sc) {}

        string  hint;
        DWORD   color;
        int     height; 
        int     offx;
        int     width;
		int		font;
		bool	shadow;
		unsigned int scolor;
    };

	CTextHint();

	// ��������Hint�Ƿ����
    void    SetHintIsCenter( bool v )		{ _IsCenter = v;			}
	bool	GetHintIsCenter()				{ return _IsCenter;			}

	void	SetIsHeadShadow( bool v )		{ _IsHeadShadow = v;		}
	bool	GetIsHeadShadow()				{ return _IsHeadShadow;		}
	
	stHint*	GetHint( int v )				{ return _hint[v];			}
	stHint*	GetHintRelated( int i, int j )	{ return _hint_related[i][j];	}
	int		GetCount()						{ return (int)_hint.size();	}

	void	SetFixWidth( int w );
	int		GetFixWidth()					{ return _nFixWidth;		}

	void	SetBkgColor( DWORD color )		{ _dwBkgColor = color;		}
	DWORD	GetBkgColor()					{ return _dwBkgColor;		}

	void	SetIsShowFrame( bool v )		{ _IsShowFrame = v;			}
	bool	GetIsShowFrame()				{ return _IsShowFrame;		}

private:
    typedef vector<stHint*> hints;
    hints    _hint;
	hints	_hint_related[enumEQUIP_NUM];

private:
	enum eStyle
	{
		enumFontSize,
		enumFixWidth,
	};
	eStyle		_eStyle;
	int			_nFixWidth;

    bool		_IsCenter;
	bool		_IsHeadShadow;
	bool		_IsShowFrame;

    int			_nHintW, _nHintH;
	int			_nRelatedHintW[enumEQUIP_NUM];
	int			_nRelatedHintH[enumEQUIP_NUM];
    int			_nStartX, _nStartY;
	DWORD		_dwBkgColor;

};

//Add by sunny.sun20080804
//Begin
class CTextScrollHint
{
public:
	void	Clear();
	void	ReadyForHint( int x, int y,int SetNum);
	void	Render();

    void    PushHint( const char* str, DWORD color=COLOR_WHITE, int height=5, int font=0 ); // height:�۳��ָߺ�ĸ߶�
public:
    struct stHint 
    {
        stHint( const char* str, DWORD c, int h, int f ) : hint(str), color(c), height(h), offx(0), font(f), width(0) {}

        string  hint;
        DWORD   color;
        int     height; 
        int     offx;
        int     width;
		int		font;
    };

	CTextScrollHint();

	void	SetFixWidth( int w );	
	void	SetBkgColor( DWORD color )		{ _dwBkgColor = color;		}
private:
    typedef vector<stHint*> hints;
    hints    _hint;
	int		index;
	string	CopyHints;
	string	SpaceLength;
	int		num;
	int		RenderScrollNum;
	int		SetScrollNum;
	int		m;
private:
	enum eStyle
	{
		enumFontSize,
		enumFixWidth,
	};
	eStyle		_eStyle;
	int			_nFixWidth;

    bool		_IsCenter;
	bool		_IsHeadShadow;
	bool		_IsShowFrame;

    int			_nHintW, _nHintH;
    int			_nStartX, _nStartY;
	DWORD		_dwBkgColor;

};
//End
inline void	CGuiFont::Begin()
{
	
	_pFont->Begin();
}
inline void	CGuiFont::Draw(const char* str, int x, int y, DWORD color )
{
	
	GetRender().DrawConvert( x, y );
	_pFont->Draw((char*)str, x, y, color);

}
inline void	CGuiFont::End()
{
	_pFont->End();
}

inline void CGuiFont::Render( unsigned int nIndex, const char* str, int x, int y, DWORD color ,float size)
{
	if( nIndex<_nMaxFont) 
	{		
		GetRender().DrawConvert( x, y );
		_fonts[nIndex]->DrawText((char*)str, x, y, color, size );
	}
}

inline void CGuiFont::BRender( unsigned int nIndex, const char* str, int x, int y, DWORD color, DWORD shadow )
{
	if( nIndex<_nMaxFont) 
	{		
		GetRender().DrawConvert( x, y );
        _fonts[nIndex]->DrawTextShadow( (char*)str, x+1, y+1, x, y, shadow, color );
	}
}

inline void CGuiFont::BRender( const char* str, int x, int y, DWORD color, DWORD shadow )
{
	GetRender().DrawConvert( x, y );
   _pFont->DrawTextShadow( (char*)str, x+1, y+1, x, y, shadow, color );
}

inline void CGuiFont::Render( const char* str, int x, int y, DWORD color,float size )
{
	GetRender().DrawConvert( x, y );

	_pFont->DrawText((char*)str, x, y, color, size);
}

inline  void CGuiFont::RenderScale(const char *str, int x ,int y , DWORD color , float scale)
{
	GetRender().DrawConvert( x, y );
	_pFont->DrawText((char*)str, x, y, color, scale);
}

inline void CGuiFont::Render3d( const char* str, D3DXVECTOR3& pos, DWORD color )
{
	_pFont->Draw3DText((char*)str, pos, color );
}

inline int CGuiFont::GetWidth( const char* str )
{
	_pFont->GetTextSize( const_cast<char*>(str), &_size );
	return _size.cx;
}

inline int CGuiFont::GetHeight( const char* str )
{
	_pFont->GetTextSize( const_cast<char*>(str), &_size );
	return _size.cy;
}

inline void CGuiFont::GetSize( const char* str, int& w, int &h )
{
	_pFont->GetTextSize( const_cast<char*>(str), &_size );

	h = (int)( _size.cy );
	w = (int)( _size.cx );
}

inline bool CGuiFont::GetSize( unsigned int dwIndex, const char* str, int& w, int &h )
{
	if( dwIndex>=_nMaxFont) return false;
	
	_fonts[dwIndex]->GetTextSize( const_cast<char*>(str), &_size );

	h = (int)( _size.cy );
	w = (int)( _size.cx );
	return true;
}

inline void CGuiFont::SetIndex( unsigned int n )		
{ 
	if( n<_nMaxFont) _pFont = _fonts[n]; 
}

inline CMPFont*	CGuiFont::GetFont( unsigned int index ) 
{ 
	if( index<_nMaxFont) return _fonts[index]; return NULL;
}

// ��������
inline void CTextHint::PushHint( const char* str, DWORD color, int height, int font, int index, bool shadow, DWORD scolor )
{ 
    if( str && strlen(str)>0 )
	{
		if (index == -1)
			_hint.push_back( new stHint(str, color, height, font, shadow, scolor) );
		else
			_hint_related[index].push_back( new stHint(str, color, height, font, shadow, scolor) );
	}
}

inline void CTextHint::AddHintHeight( int height )
{
    if( !_hint.empty() )
    {
        _hint.back()->height += height;
    }
}

inline void CTextHint::SetFixWidth( int w )			
{
	if( w<=0 )
	{
		_eStyle = enumFontSize;
	}
	else
	{
		_nFixWidth = w;
		_eStyle = enumFixWidth;
	}
}
// ��������//Add by sunny.sun20080804
//Begin
inline void CTextScrollHint::PushHint( const char* str, DWORD color, int height, int font )       
{ 
    if( str && strlen(str)>0 ) _hint.push_back( new stHint(str, color, height, font) ); 
}

inline void CTextScrollHint::SetFixWidth( int w )			
{
	if( w<=0 )
	{
		_eStyle = enumFontSize;
	}
	else
	{
		_nFixWidth = w;
		_eStyle = enumFixWidth;
	}
}
//End
}
