//----------------------------------------------------------------------
// ����:����ִ�п������
// ����:lh 2004-10-28
//----------------------------------------------------------------------
#pragma once

#include "uicompent.h"

namespace GUI
{
#define defCommandDefaultIndex	-1

class CCommandObj : public CItemObj
{
public:
	CCommandObj() : _nFast(0), nTag(0), _pParent(NULL) ,_bTrade(false), _nIndex(defCommandDefaultIndex) {}
	virtual ~CCommandObj();
    ITEM_CLONE(CCommandObj)

    bool	        Exec();         // �û�ѡ��ʱִ��
	bool	        ExecRightClick();         // �û�ѡ��ʱִ��

	virtual bool	MouseDown(){ return false;	} //��갴��
	bool	GetCanDrag();
	virtual	void	SaleRender( int x, int y, int nWidth, int nHeight )	{}
	virtual void	OwnDefRender( int x, int y, int nWidth, int nHeight ) {}
	virtual void	RenderEnergy(int x, int y)	{}

    void            SetParent( CCompent* p )    { _pParent=p;   }
    CCompent*       GetParent()     { return _pParent;          }
    
    static  bool    UserExec();     // ���û�ѡ���,�ȴ��û�����ִ��
    static  void    Cancel()        { _pCommand=NULL;           }
	static  CCommandObj* GetReadyCommand()	{ return  _pCommand;}

    virtual bool    UseCommand(bool value = false);   // ʹ��
	virtual bool	StartCommand()	{ return false;	}

    virtual int     GetTotalNum()   { return 1;     }
    virtual void    SetTotalNum( int num ){}

    virtual bool    IsDragFast()    { return true;  }   // �Ƿ�����϶�Ϊ��ݷ�ʽ

    virtual bool    IsAllowThrow()  { return false; }   // �Ƿ��ʶ���
    virtual bool    IsAllowEquip()  { return false; }   // ��װ��

	virtual void    SetIsSolid( bool v ) {}
	virtual bool    GetIsSolid()    { return true;  }

	virtual void    SetIsValid( bool v ){}
	virtual bool    GetIsValid()	{ return false; }

    virtual bool    GetIsPile()     { return false; }
    virtual int     GetPrice()      { return 0;     }

    virtual const char* GetName()   { return "unknown";    }
    virtual bool    IsAllowUse()    { return true;  }   // ����ʹ��

    // hint
    virtual bool    HasHint()       { return true;  }
	virtual void    ReadyForHint( int x, int y, CCompent* pCompent );
    virtual void    RenderHint( int x, int y );

	// �����Զ����ַ���
	void			SetOwnDefText(const char* pszText) { _OwnDefText = pszText; }
	const char*		GetOwnDefText()	{ return _OwnDefText.c_str(); }

	static CTextHint& GetHints()	{ return  _hints;}

public:
    void		SetIsFast( bool v );                    // �Ƿ��ݷ�ʽ
    bool        GetIsFast()             { return _nFast>0;  }

    void        SetIndex( int nIndex )  { _nIndex = nIndex; }
    int         GetIndex()              { return _nIndex;   }

	int			nTag;       // ���ڼ�¼Item��Skill��ScriptID

protected:
    virtual bool    IsAtOnce()      { return true;  }   // ����ֱ��ʹ��
    virtual bool    ReadyUse()      { return false; }   // ׼��ʹ��,��Ӧ�ڲ�������ʹ��ʱ
    virtual void    Error(){}

    virtual void    AddHint( int x, int y ) {}

protected:
    static CCommandObj*     _pCommand;      // ��ǰ�û����ڲ����ļ���

private:
    bool    _Exec();

private:
	int	        _nFast;	    // �ڿ�����ϵļ���,����Ѿ����ڵĿ������,���˳�ʱҪ����һ��֪ͨ
    CCompent*   _pParent;   // �����ؼ�
	bool        _bTrade;
    int         _nIndex;

protected:
    void    PushHint( const char* str, DWORD color=COLOR_WHITE, int height=5, int font=0, int index = -1, bool shadow = false, DWORD scolor = 0); // height:�۳��ָߺ�ĸ߶�
    void    AddHintHeight( int height=6 );
	void    SetHintIsCenter( bool v );

	static CTextHint _hints;

	string	_OwnDefText;

};

// ��������
inline void CCommandObj::PushHint( const char* str, DWORD color, int height, int font, int index, bool shadow, DWORD scolor )
{ 
	_hints.PushHint( str, color, height, font, index, shadow, scolor );
}

inline void CCommandObj::AddHintHeight( int height )
{
	_hints.AddHintHeight( height );
}

inline void CCommandObj::SetHintIsCenter( bool v )   
{ 
	_hints.SetHintIsCenter( v );    
}

}
