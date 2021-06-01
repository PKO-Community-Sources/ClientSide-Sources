//----------------------------------------------------------------------
// 名称:人物头上泡泡
// 作者:lh 2004-08-21
// 最后修改日期:2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uiGuidata.h"

class CCharacter;

namespace GUI
{
const int FACE_MAX = 50;
const int EVIL_MAX = 5;

class CGuiPic;
class CItemEx;

class CHeadSay
{
public:
    CHeadSay(CCharacter* p);

	bool isTeamMember;
	bool isGuildMember;
	
    static bool		Init();
    static bool		Clear();

	void			Reset()						{ _nShowTime=0; _nFaceTime=0;	}
    void			AddItem( CItemEx* obj );
    void			Render( D3DXVECTOR3& pos );
    void			RenderStateIcons(CCharacter* cha,int x,int y,float scale,float spacing,int rowSize);

    // 设置显示的血条:num血量，max总血量,attacknum被攻击的数字
    void			SetLifeNum(int num, int max);
    void			SetManaNum(int num, int max);
    void			SetIsShowLife( bool v )		{ _IsShowLife = v;		}
    void			SetIsShowMana( bool v )		{ _IsShowMana = v;		}
    bool			GetIsShowLife()				{ return _IsShowLife;	}
    bool			GetIsShowMana()				{ return _IsShowMana;	}

    void            SetIsShowName( bool v )     { _IsShowName = v;      }
    bool            GetIsShowName()             { return _IsShowName ;  } 
    void            SetNameColor( DWORD v )     { _dwNameColor=v;       }

    static void		SetMaxShowTime( int v )		{ _nMaxShowTime = v;	}

    static void		SetMaxShowLiftTime( int n )	{ _nMaxShowLifeTime = n;}
    static void		SetFaceFrequency( int n )	{ if(n>0) _nFaceFrequency=n;	}
    static CGuiPic* GetFacePic( unsigned int n ) { if( n>=FACE_MAX) return NULL; return &_pImgFace[n]; }
    static CGuiPic* GetShopPic( unsigned int n ) { if( n>=3) return NULL; return &_ImgShop[n]; }

    static CGuiPic* GetLifePic()			{ return _pImgLife;					}
    static CGuiPic* GetManaPic()			{ return _pImgMana;					}
    static CGuiPic* GetLeaderPic()			{ return _pImgTeamLeaderFlag;		}
    static CGuiPic* GetGuildLeaderPic()			{ return _pImgGuildLeaderFlag;		}

	static void		SetBkgColor( DWORD v )	{ _dwBkgColor=v;					}

	bool			SetFaceID( unsigned int faceid );
    int             GetFaceID(){ return  _nCurFaceID ;}      //得到当前的ID.by billy
    void            SetRenderScale(float f ) { _fScale  = f  ;      }

    void			SetName( const char* name );

	bool			InShop( int MouseX, int MouseY );

	static void		RenderText( const char* szShopName, int x, int y );

	void SetEvilLevel(short sMaxEnergy);
	void SetIsShowEvil(bool bShow);

private:
    static int		_nMaxShowTime;	// 最大能显示的时间

private:
    int				_nShowTime;		// 目前显示的时间
    float           _fScale ;       //显示的比例
    CItemEx *       _pObj;
    string          _str ;          //聊天时候显示在头像上面的文字信息
    CCharacter*		_pOwn;

private:	// 显示攻击效果时，血量变化
    static int			_nMaxShowLifeTime;	// 最大显示多长时间

    static CGuiPic*		_pImgLife;
    static CGuiPic*		_pImgMana;
    static CGuiPic*		_pImgTeamLeaderFlag;// 显示在队长头上的标志
    static CGuiPic*		_pImgGuildLeaderFlag;// 显示在队长头上的标志

    float				_fLifeW;
	int					_fCurHp;
	int					_fMxHp;
	
	float				_fManaW;
	int					_fCurSp;
	int					_fMxSp; 
	

    static unsigned int	_nFaceFrequency;

	static CGuiPic* _pImgFace;
	static CGuiPic* _pImgEvil;
	static DWORD	_dwBkgColor;

    int				_nFaceTime;		// 表情已经显示多长时间
    int             _nCurFaceID ;   //目前显示的表情ID号
    DWORD           _dwNameColor;

    CGuiPic*		_pCurFace;		// 目前显示的是那张表情
    unsigned int	_nCurFaceFrame;	// 目前显示的表情第几帧
    unsigned int	_nCurFaceCycle;

    bool			_IsShowLife;	// 是否显示血条
    bool			_IsShowMana;	// 是否显示血条
    bool            _IsShowName;    // 是否显示角色的名字。added by billy
    int				_nChaNameOffX;	// 玩家名字的X偏移

	// 名字的部分数
	enum {	
		PRENAME_SEP1_INDEX = 0,			//	(
		PRENAME_INDEX = 1,				//	前缀名
		PRENAME_SEP2_INDEX = 2,			//	)
		NAME_INDEX = 3,					//	角色名
		MOTTO_NAME_SEP1_INDEX = 4,		//	(
		MOTTO_NAME_INDEX = 5,			//	座右铭
		MOTTO_NAME_SEP2_INDEX = 6,		//	)
		BOAT_NAME_SEP1_INDEX = 7,		//	[
		BOAT_NAME_INDEX = 8,			//	船名
		BOAT_NAME_SEP2_INDEX = 9,		//	]

		NAME_PART_NUM=10,			//	名字的组成部分的数目
	};
	//名字前缀
	//名字显示方式 (前缀) ＋ 角色名 + (座右铭) ＋ [船名]
	static char s_sNamePart[NAME_PART_NUM][64];
	//前一列是前景色,后一列是阴影颜色
	static DWORD	s_dwNamePartsColors[NAME_PART_NUM][2];
	//全名
	static char s_szName[1024];
	//公会名
	static char s_szConsortiaNamePart[4][64];
	static char s_szConsortiaName[256];

	enum 
	{ 
		NAME_LENGTH = 64,
	};

private:	// 摆摊
	static CGuiPic	_ImgShop[3];	// 0为左，1为中可以拉伸的图片，2为右
	static int		_nShopFrameWidth;
	static int		_nShopFontYOff;
	void			_RenderShop( const char* szShopName, int x, int y );

	int	_nShopX0, _nShopY0, _nShopX1, _nShopY1;

	short _sEvilLevel;
	bool _IsShowEvil;
};

inline bool CHeadSay::InShop( int nMouseX, int nMouseY )
{
	return nMouseX > _nShopX0 && nMouseX < _nShopX1 && nMouseY > _nShopY0 && nMouseY < _nShopY1;
}

}
