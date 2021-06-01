#pragma once

#define USE_DSOUND

#include "MPGameApp.h"
#include "script.h"
#include "CRCursorObj.h"
#include "RenderStateMgr.h"
#include "steadyframe.h"

#include "CameraCtrl.h"

#ifdef USE_DSOUND
#include "DSoundManager.h"
#include "DSoundInstance.h"
#endif

#include "AudioSDL.h"
#include "chastate.h"

extern DWORD g_dwCurMusicID;

namespace Ninja
{
	class Camera;
	struct SphereCoord;
	template < class T >
	class Controller;
}

class CSceneObj;

#define	  SHOWRSIZE		    40
#define   TIP_TEXT_NUM      128
#define   SHOW_TEXT_TIME	3000
#define   MAX_CQUEUE		7
#define   TIME_CQUEUE		3000

//#define TESTDEMO

namespace GUI
{
	class CTextHint;
	class CTextScrollHint;//Add by sunny.sun20080804
};

enum eSceneType
{
	enumLoginScene     = 0, // 登录场景
	enumWorldScene     = 1,	// 游戏场景
	enumSelectChaScene = 2,	// 选择人物
	enumCreateChaScene = 3, // 创建人物
	enumSceneEnd,
};

struct STipText
{
    char  szText[TIP_TEXT_NUM];
    DWORD dwBeginTime;
    BYTE  btAlpha;
};

class CSteadyFrame;
class CGameScene;
class CMPShadeMap;
class CPointTrack;
class CEffectObj;
class CActor;
class CDrawPointList;
class CCameraCtrl;
class CGameScene;
struct stSceneInitParam;

struct SAddSceneObj
{
    int nTypeID;
    int nPosX;
    int nPosY;
    int nHeightOff;
    int nAngle;
};

class CGameApp : public MPGameApp
{
public:
	CGameApp();
	~CGameApp();

    void    End();

    virtual void		MouseButtonDown(int nButton);
	virtual void		MouseButtonUp(int nButton);
	virtual void		MouseButtonDB(int nButton);
	virtual void		MouseMove(int nOffsetX, int nOffsetY);
	virtual void		MouseScroll(int nScroll);
	virtual void		MouseContinue(int nButton);
	virtual void		HandleKeyDown(DWORD dwKey);
    virtual void		LG_Config(const LGInfo& info);

	bool				LoadRes4();

    void				SetIsRun( bool v ) { if(v != _isRun) { _isRun = v; if(v == 0) { ::SendMessage(_hWnd, WM_DESTROY, 0, 0); } } }
    BOOL                IsRun() { return _isRun; }
	void				SetIsRenderTipText( bool v ){ _IsRenderTipText = v;		}
	bool				GetIsRenderTipText()		{ return _IsRenderTipText;	}
	
	void				InitAllTable();			// 读入所有的表格文本
	void				ReleaseAllTable();		// 释放所有的表格文本
	

	void	            HandleKeyContinue();
	bool	            HandleWindowMsg(DWORD dwMsg, DWORD dwParam1, DWORD dwParam2);

	void                ChangeVideoStyle( int width , int height ,D3DFORMAT format , bool bWindowed  );//by billy

	void	            EnableCameraFollow(BOOL bEnable)	{ _bCameraFollow = bEnable;		} 
	BOOL	            IsCameraFollow()					{ return _bCameraFollow;		}
    void                ResetGameCamera(int type = 0);
	void                ResetCamera();

	CPointTrack*		GetCameraTrack()					{ return _pCamTrack;			}

    void                HandleSuperKey();
    void                HandleContinueSuperKey();
    void                EnableSuperKey(BOOL bEnable)		{ _bEnableSuperKey = bEnable;	}
	BOOL                IsEnableSuperKey();              
    void                PlayMusic(int nMusicNo);
	void				PlaySound(int nSoundNo);

	void				SendMessage( DWORD dwTypeID, DWORD dwParam1=0, DWORD dwParam2=0 );

	bool				IsInit()							{ return _IsInit;				}

	bool				HasLogFile( const char* log_file, bool isOpen=true );

	void				Loading( int nFrame=40 );				// 参数为多少帧
    static void         Waiting( bool isWaiting=true );         // 显示出一个等待对话框,这其间不能操作UI
	static bool			IsMouseContinue(int nButton)		{ return _dwMouseDownTime[nButton]>12;	}

    void                AddTipText(const char *pszFormat, ...);
	void				SysInfo( const char *pszFormat, ... );
	void				ShowNotify( const char *szStr, DWORD dwColor );
	void				ShowNotify1( const char *szStr,int setnum, DWORD dwColor );//Add by sunny.sun20080804
	void				ShowHint( int x, int y, const char *szStr, DWORD dwColor );
	void				ShowStateHint( int x, int y, CChaStateMgr::stChaState stateData);

    static void         SetMusicSize( float fVol );         // 0~1,0静音景,1最大音量
    static float        GetMusicSize()                      { return (float)_nMusicSize / 128.0f;   }

    static void         MsgBox( const char *pszFormat, ... );

    void                ShowBigText( const char *pszFormat, ... );
    void                ShowMidText( const char *pszFormat, ... );
	void                ShowBottomText( unsigned int rgb, const char *pszFormat, ... );

	// 切换场景begin
	CGameScene*	CreateScene( stSceneInitParam* param );
	void		GotoScene( CGameScene* scene, bool isDelCurScene=true, bool IsShowLoading=true );		// 直接切换到另一个场景
	int			Run();
	static CGameScene*	GetCurScene()					{ return _pCurScene;	}	// 获得当前场景
	// 切换场景end

	void		CreateCharImg();

	void		RefreshLoadingProgress();
	
public:	// 脚本创建场景
	void		LoadScriptScene( eSceneType eType );
	void		LoadScriptScene( const char* script_file );

	bool		btest;

	int			ihei;
public:
	static DWORD		GetCurTick()					{ return _dwCurTick;	}
	void				SetTickCount(DWORD dwTick)		{ _dwCurTick = dwTick;  }
	void				SetFPSInterval( DWORD v );
	static DWORD        GetFrameFPS();

	static CSteadyFrame* GetFrame()						{ return _pSteady;		}

	// 根据文件名初始化当前场景，xuedong 2004.09.06 用于“生成障碍信息文件“
	BOOL				CreateCurrentScene(char *szMapName);

	CursorMgr*			GetCursor()						{ return &_stCursorMgr;	}

	CDrawPointList*		GetDrawPoints()					{ return _pDrawPoints;	}

	void				OnLostDevice();
	void				OnResetDevice();

	CCameraCtrl*		GetMainCam()					{ return _pMainCam;		} 
    static bool         IsMusicSystemValid()            { return _IsMusicSystemValid;       }

	CMPFont*			GetFont()						{return &g_CFont;		}
    RenderStateMgr*     GetRenderStateMgr()             { return _rsm;			}

	void				SetCameraPos(D3DXVECTOR3& pos,bool bRestoreCustom=true);	// bRestoreCustom参数为true，表示清除用户对镜头的改变

	void				SetStartMinimap(int  ix, int iy, int destx,int desty);
	CScriptMgr*			GetScriptMgr()					{ return &_stScriptMgr;				}
	void				ResetCaption();

	void				AutoTest();			// 自动化测试
	void				AutoTestInfo( const char *pszFormat, ... );		// 用于自动化测试时显示测试内容
	void				AutoTestUpdate();

	static bool			IsMouseInScene()				{ return _MouseInScene;				}

	//用于记录/读取登陆游戏的Tick时间
	static void SetLoginTime(DWORD _dwLoginTime)		{ m_dwLoginTime=_dwLoginTime;		}
	static DWORD GetLoginTime()							{ return m_dwLoginTime;				}

public:    
    list<SAddSceneObj*>  m_AddSceneObjList;
    DWORD                m_dwRenderUITime;
    DWORD                m_dwRenderSceneTime;
    DWORD                m_dwRenderScneObjTime;
    DWORD                m_dwRenderChaTime;
    DWORD                m_dwRenderEffectTime;
    DWORD                m_dwLoadingObjTime;
    DWORD                m_dwTranspObjTime;
    DWORD                m_dwRenderMMap;
    DWORD                m_dwPathFinding;
	
	BOOL			m_bRenderFlash;

#ifdef USE_DSOUND
	SoundManager*	mSoundManager;
	
	void			PlaySample( string SoundName );

#endif
	
protected:
	virtual BOOL        _Init();
	virtual void		_PreMouseRun( DWORD dwMouseKey );
    virtual void	    _FrameMove(DWORD dwTimeParam, bool camMove);		//ViM
	virtual void	    _Render();
    virtual void        _End();

	BOOL				_PrintScreen();
	BOOL				_CreateAviScreen();
	bool				_IsSceneOk() { return _pCurScene!=NULL; }
	BOOL				_CreateSmMap( MPTerrain* pTerr );
    void                _RenderTipText();
    void                _ShowLoading();

protected:
	
    BOOL			    _bCameraFollow;
    BOOL                _bEnableSuperKey;
	BOOL			    _bConnected;
    CPointTrack*		_pCamTrack;
	CDrawPointList*		_pDrawPoints;

    std::list<STipText*> _TipText;

	CCameraCtrl*		_pMainCam;

public:
    RenderStateMgr*     _rsm;
#if(defined USE_TIMERPERIOD)
    MPITimerPeriod*     _TimerPeriod;
#endif
	float	xp,xp1;
	float	yp,yp1;

	float	destxp;
	float	destyp;

protected:
	void	_SceneError( const char* info, CGameScene * p );
	void	_HandleMsg(DWORD dwTypeID, DWORD dwParam1, DWORD dwParam2);

	CGameScene*	_pStartScene;		// 第一个场景
	static CGameScene*	_pCurScene;	// 目前正在运行的场景
	static DWORD		_dwCurTick;

	int			_nSwitchScene;		// 大于零,正在切换场景

	bool		_isRun;		

	CScriptMgr	_stScriptMgr;
	CursorMgr	_stCursorMgr;

	bool		_IsRenderTipText;
	bool 		_IsRenderColourTest;

private:		// 用于脚本
	bool				_IsInit;
    DWORD               _dwGameThreadID;

	static DWORD		_dwMouseDownTime[2];

    static int          _nMusicSize;        // 背景音量
    bool                _IsUserEnabled;     // 是否接收用户输入

    static char         _szOutBuf[256];

	CMPFont				g_CFont;
    CMPFont             _MidFont;         // 在屏幕中央偏上的一个中等字体
    STipText            _stMidFont;
	CMPFont             _BottomFont;

	//int					_nCQueueIndex;
	//STipText*			_sCQueue[MAX_CQUEUE];
	//unsigned long 		_iCQueueColour[MAX_CQUEUE];
	queue< pair<STipText*, unsigned int> >	_qCQueueStrColour;

	//void UpdateColourQueue();
	void RenderColourQueue();

    static bool         _IsMusicSystemValid; 

private:        // 背景音乐的切换,经历三个阶段:1.当前音乐音量变小,2.为零时切换到新音乐,并且音乐变大,3.正常播放新音乐
    enum eBkgMusic { enumNoMusic, enumOldMusic, enumNewMusic, enumMusicPlay };
    eBkgMusic           _eSwitchMusic;
    int                 _nCurMusicSize;     // 切换音乐时的音量大小
    char                _szBkgMusic[256];   // 新音乐
    
	static CSteadyFrame*	_pSteady;
	static	bool			_MouseInScene;
	
	CTextHint			*_pNotify;
	CTextScrollHint		*_pNotify1;//Add by sunny.sun20080804
	DWORD				_dwNotifyTime;
	DWORD				_dwNotifyTime1;//Add by sunny.sun20080804
	
	int					SetNum;

	static DWORD		m_dwLoginTime;		//记录登陆游戏的Tick时间

	DWORD				_dwLoadingTick;		// 开始 Loading 的时间


public:



	Ninja::Camera*		GetNinjaCamera()	{ return _pNinjaCamera; }
//	Ninja::Camera*		_pNinjaCamera;
//	Ninja::Controller < D3DXVECTOR3 > *_ctrl;

	// Added by CLP
	private:
	Ninja::Camera*		_pNinjaCamera;
	Ninja::Controller < D3DXVECTOR3 > *_camera_target_ctrl;
	Ninja::Controller < Ninja::SphereCoord > *_camera_eye_ctrl;
};

inline void CGameApp::SendMessage( DWORD dwTypeID, DWORD dwParam1, DWORD dwParam2 )
{	
	int i=0;
	while( !::PostMessage( GetHWND(), dwTypeID, dwParam1, dwParam2 ) && i<=10 )
	{
		Sleep(50);
		i++;
	}
}

inline void	CGameApp::SetStartMinimap(int  ix, int iy, int destx,int desty)
{ 
	xp = (float)((ix / SHOWRSIZE) * SHOWRSIZE) + (SHOWRSIZE/2);
	yp = (float)((iy / SHOWRSIZE) * SHOWRSIZE) + (SHOWRSIZE/2);

	destxp = (float)xp + destx* SHOWRSIZE;
	destyp = (float)yp + desty* SHOWRSIZE;

	xp1 = xp;
	yp1 = yp;
}

inline DWORD CGameApp::GetFrameFPS()                   
{ 
	return CSteadyFrame::GetFPS();	
}

#define TipI(con, t1, t2) { if(con) {g_pGameApp->AddTipText(t1);} else { g_pGameApp->AddTipText(t2); } }
#define Tip(t)            { g_pGameApp->AddTipText(t);}

extern CGameApp* g_pGameApp;
extern bool	volatile	g_bLoadRes;
BOOL RenderHintFrame(const RECT* rc, DWORD color);
