//----------------------------------------------------------------------
// ����:��̬����
// ����:lh 2004-07-19
// ����޸�����:2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uiguidata.h"

namespace GUI
{

// ͼԪ
class CGraph : public CItemObj
{
public:
	CGraph(char* file, int w, int h, int sx, int sy, int maxframe) : nTag(0) {
		_pImage = new CGuiPic(NULL, maxframe);
		_pImage->LoadAllImage( file, w, h, sx, sy );
	}
	CGraph( const CGuiPic& rhs ) {
		_pImage = new CGuiPic( rhs );
	}
	CGraph( const CGraph& rhs )	: CItemObj(rhs), _pImage(new CGuiPic(*rhs._pImage)), nTag(rhs.nTag)	{}
	CGraph& operator=(const CGraph& rhs) { CItemObj::operator=(rhs) ;*_pImage=*rhs._pImage; nTag=rhs.nTag; return *this;}
	CGraph() : nTag(0)		{ _pImage = new CGuiPic(NULL, 1);	}
	~CGraph()				{ SAFE_DELETE(_pImage); } // UI�������� //delete _pImage;					}
	ITEM_CLONE(CGraph)

	CGraph(unsigned int frame) : nTag(0)	{_pImage = new CGuiPic(NULL, frame);	}

	// Grid�ӿ�
	virtual void	Render( int x, int y )	{ _pImage->Render(x, y);	    }

    virtual int     GetWidth()				{ return _pImage->GetWidth();	}
    virtual int     GetHeight()				{ return _pImage->GetHeight();	}

	void		Next()					{ _pImage->Next();			}
	void		Reset()					{ _pImage->SetFrame(0);		}

	// �ⲿ�ӿ�
	CGuiPic*	GetImage()				{ return _pImage;			}

	int			nTag;

protected:
	CGuiPic*	_pImage;

};

// ��hint��ͼԪ,���ڱ༭�����нڵ�
class CHintGraph : public CGraph
{
public:
    CHintGraph( char* file, int w, int h, int sx, int sy, int maxframe );
    CHintGraph(unsigned int maxframe);

    virtual bool    HasHint()           { return !_strHint.empty();     }
    virtual void    RenderHint( int x, int y );

public:
    void    SetHint( const char* str )  { _strHint = str;               }

protected:
    string  _strHint;

};

// ���������ͷ��
class CTextGraph : public CHintGraph
{
public:
    CTextGraph( char* file, int w, int h, int sx, int sy, int maxframe );
    CTextGraph(unsigned int maxframe);
    ~CTextGraph();
	virtual DWORD    GetColor()        { return _color;                        }
	virtual void	Render( int x, int y );
	virtual void	SetString( const char* str ){ _strName=str;				}

	const char*     GetString()					{ return _strName.c_str();	}
	virtual void	SetColor(DWORD color)		{ _color=color;				}

	void			SetPointer( void* v )		{ _pVoid = v;					}
	void*			GetPointer()				{ return _pVoid;				}

protected:
    string			_strName;
	DWORD			_color;

	void*			_pVoid;

};

// �������Ϳؼ��ı��ڵ�
class CNoteGraph : public CHintGraph
{
public:
    CNoteGraph( unsigned int maxframe );

	virtual void	Render( int x, int y );

	virtual void	SetString( const char* str ){ _strName=str;				}
	const char*     GetString()					{ return _strName.c_str();	}

	virtual void	SetColor(DWORD color)		{ _color=color;				}

	void			SetTextX( int v )			{ _nTextX = v;				}	
	void			SetTextY( int v )			{ _nTextY = v;				}	

protected:
    string			_strName;
	DWORD			_color;
	int				_nTextX, _nTextY;	// ���ֵ�X,Yƫ��

};

}
