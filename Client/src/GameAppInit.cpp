#include "Stdafx.h"

#include "GameApp.h"
#include "cameractrl.h"
#include "Character.h"
#include "SceneObj.h"
#include "SceneItem.h"
#include "SceneObjFile.h"

#include "MPEditor.h"
#include "Scene.h"
#include "GameConfig.h"
#include "EffectObj.h"
#include "Track.h"
#include "MPFont.h"
#include "SmallMap.h"
#include "UIFormMgr.h" 
#include "script.h"
#include "GlobalVar.h"
#include "DrawPointList.h"
#include "PacketCmd.h"
#include "UIRender.h"
#include "UIsystemform.h"
#include "SteadyFrame.h"
#include "UICozeForm.h"
#include "GameMovie.h"

#ifndef USE_DSOUND
#include "AudioThread.h"
extern CAudioThread g_AudioThread;
#endif

extern CGameMovie g_GameMovie;

#ifdef TESTDEMO
#include "TestDemo.h"
#endif


DWORD		CGameApp::_dwMouseDownTime[2]	= { 0 };
int			CGameApp::_nMusicSize			= 64;
char		CGameApp::_szOutBuf[256]		= { 0 };
bool		CGameApp::_IsMusicSystemValid	= false;
CGameScene*	CGameApp::_pCurScene			= NULL;
DWORD		CGameApp::_dwCurTick			= 0;
DWORD		CGameApp::m_dwLoginTime			= 0;

const BYTE verifyName[]={0xf8,0x05,0x1a,0xe4,0x98,0x5e,0xb8,0x9e};

const BYTE verifyDialog[]={0xf0, 0xdc, 0xea, 0x7b, 0x40, 0xeb, 0xc4, 0x47};

static  CSteadyFrame steady;
CSteadyFrame* CGameApp::_pSteady = &steady;

extern void LimitCurrentProc();

CGameApp::CGameApp()
:_bEnableSuperKey(FALSE),
 _pCamTrack(NULL),
 _IsRenderTipText(false),
 _IsInit(false),
 _nSwitchScene(1),
 _IsUserEnabled(true),
 _eSwitchMusic(enumNoMusic), 
 _nCurMusicSize(1)
{
	LimitCurrentProc();
	_pDrawPoints = new CDrawPointList;
	_pMainCam = new CCameraCtrl;
  
	xp = SHOWRSIZE / 2;
	yp = SHOWRSIZE / 2;

    _rsm = 0;

    memset( _szBkgMusic, 0, sizeof(_szBkgMusic) );

#if(defined USE_TIMERPERIOD)
    _TimerPeriod = 0;
#endif

	btest = false;
	ihei = 0;

	_pNotify = new CTextHint;

	_pNotify->SetHintIsCenter( false );
	_pNotify->SetFixWidth( 430 );
	_pNotify->SetBkgColor( 0x60000000 );
	_pNotify->SetIsHeadShadow( false );

	//Add by sunny.sun20080804
	//Begin
	_pNotify1 = new CTextScrollHint;
	_pNotify1->SetFixWidth( 430 );
	_pNotify1->SetBkgColor( 0x60000000 );
	//End
	_dwNotifyTime = 0;	
	_dwNotifyTime1 = 0;

	//_ctrl = new Ninja::LinearController < D3DXVECTOR3 >;
	//_pNinjaCamera = new Ninja::Camera( _ctrl );

	// Added by CLP
	_camera_target_ctrl = new Ninja::LinearController < D3DXVECTOR3 >;
	_camera_eye_ctrl = new Ninja::LinearController < Ninja::SphereCoord >;
	_pNinjaCamera = new Ninja::Camera( _camera_target_ctrl, _camera_eye_ctrl );

#ifdef USE_DSOUND
	mSoundManager = NULL;
#endif
}


CGameApp::~CGameApp()
{
	delete _pNotify;

#ifdef USE_DSOUND
	delete mSoundManager;
#endif

    SAFE_DELETE( _pMainCam );
	SAFE_DELETE( _pNinjaCamera );
	SAFE_DELETE( _camera_target_ctrl );
	SAFE_DELETE( _camera_eye_ctrl );
}

void CGameApp::End()
{
	_IsInit = false;

	delete _pDrawPoints;

	SAFE_DELETE( _pCamTrack );

	CNavigationBar::g_cNaviBar.Clear();

	_stCursorMgr.ClearMemory();
    if( _pCurScene )
    {
        _pCurScene->_Clear();
        _pCurScene->_ClearMemory();
        delete _pCurScene;
        _pCurScene = NULL;
    }

	CGameScene::_ClearScene();

    // CCharacterSet *pCharSet = CCharacterSet::I();
	// SAFE_DELETE( pCharSet );

    //CSceneItemSet* pItemSet = CSceneItemSet::I();
    //SAFE_DELETE( pItemSet );

	ReleaseAllTable();

	_stScriptMgr.Clear();

    SAFE_DELETE( _pMainCam );    
	
    g_CFont.ReleaseFont();
    _MidFont.ReleaseFont();
	_BottomFont.ReleaseFont();

    g_CEffBox.ReleaseBox();
    CPathBox.ReleaseBox();

	CGuiFont::s_Font.Clear();

    SAFE_DELETE(_rsm);

	// Add by lark.li 20080923 begin
	_pSteady->Exit();
	// End

#ifdef TESTDEMO
    ReleaseTestDemo();
#endif

    MPGameApp::End();
}


BOOL CGameApp::_Init()
{	
	CCozeForm::GetInstance();
	//InitAllTable();

#ifdef _LUA_GAME
	extern HINSTANCE g_hInstance;
	extern void CreateScriptDebugWindow(HINSTANCE hInst, HWND hParent);
	CreateScriptDebugWindow(g_hInstance, g_pGameApp->GetHWND());
#endif

	if(!LoadTerrainSet("scripts/table/TerrainInfo", FALSE))
        return 0;

	// 在初始化资源表格后，初始化资源 -- Michael Chen
	if(!LoadResourceSet("scripts/table/ResourceInfo", g_Config.m_nMaxResourceNum, FALSE))
		return 0;
	if (!LoadResource() || !LoadRes2() /*|| !LoadRes3()*/)
	{
		return 0;
	}

	if( g_Config.m_bEditor  )
	{
		if( !LoadRes3() )
			return 0;
	}

	_IsInit = false;

	{ // init loading res mt flag
        lwIByteSet* res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
        res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, g_Config.m_bMThreadRes);
        res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0 );//g_Config.m_bMThreadRes);

        // tex encoder
        res_bs->SetValue(OPT_RESMGR_TEXENCODE, 1);
    }

    { // init loading helper object instance flag
        lwIOptionMgr* opt_mgr = g_Render.GetInterfaceMgr()->sys->GetOptionMgr();
        opt_mgr->SetByteFlag(OPTION_FLAG_CREATEHELPERPRIMITIVE, g_Config.m_bEditor);
    }

	//_IsMusicSystemValid = ::mus_mgr_init( g_Config.m_bEnableMusic!=0 );	// music
#ifdef USE_DSOUND
	if( mSoundManager == NULL )
		mSoundManager = new DSoundManager( GetHWND() );
#endif
    AudioSDL::get_instance()->init();

	_IsMusicSystemValid = true;
    if( !_IsMusicSystemValid && g_Config.m_bEnableMusic!=0 )
    {
        LG( "init", g_oLangRec.GetString(65) );
    }

	_bConnected = FALSE;
	
	// Console系统使用的回调函数
	extern const char* ConsoleCallback(const char *pszCmd);
	_pConsole->SetCmdFN(ConsoleCallback);

	g_Render.EnablePrint(INFO_GAME, TRUE);

	// #define MPFONT_BOLD        0x0001
	// #define MPFONT_ITALIC      0x0002
	// #define MPFONT_UNLINE      0x0004

#ifdef USE_RENDER
	g_CFont.CreateFont(&g_Render,const_cast<char*>(g_oLangRec.GetString(66)), 12);
    _MidFont.CreateFont(&g_Render,const_cast<char*>(g_oLangRec.GetString(67)), 16, 3, MPFONT_BOLD);
	_BottomFont.CreateFont(&g_Render,const_cast<char*>(g_oLangRec.GetString(67)), 12, 3, MPFONT_BOLD);
#else
	g_CFont.CreateFont(g_Render.GetDevice(),const_cast<char*>(g_oLangRec.GetString(66)), 12);
    _MidFont.CreateFont(&g_Render,const_cast<char*>(g_oLangRec.GetString(67)), 16, 3, MPFONT_BOLD);
	_BottomFont.CreateFont(&g_Render,const_cast<char*>(g_oLangRec.GetString(67)), 12, 3, MPFONT_BOLD);
#endif
	g_CFont.BindingRes(&ResMgr);
    _MidFont.BindingRes(&ResMgr);
	_BottomFont.BindingRes(&ResMgr);

	//SIZE sizes;
	//g_CFont.GetTextSize("IDE\nokdote\n村城国工件副书记 三等分",&sizes);

    memset( _stMidFont.szText, 0, sizeof(_stMidFont.szText) );
    _stMidFont.dwBeginTime = 0;

	if( !_stScriptMgr.Init() )
	{
		LG("init", g_oLangRec.GetString(68));
		return FALSE;
	}
   
	string curr_ver = __TIME__;
	curr_ver += " "; curr_ver += __DATE__;
	// cout << curr_ver.c_str() << endl;
    
   	_pCamTrack = new CPointTrack;
	
	InitAllTable();

    GetCursor()->InitMemory();

    extern void InitPoseData();
    InitPoseData();
    
#if 1
    // by lsh
    // 必需在CSceneObjInfo载入后调用
    extern void LoadResModelBuf(MPIResourceMgr* res_mgr);
    MPTimer t;
    t.Begin();
    LoadResModelBuf(g_Render.GetInterfaceMgr()->res_mgr);
    DWORD res_t = t.End();
#endif
	
	extern bool UIMainInit( CFormMgr* pSender );
	CFormMgr::s_Mgr.AddFormInit( UIMainInit );				// 读入脚本后,初始化时事件

	if( !CFormMgr::s_Mgr.Init( g_pGameApp->GetHWND() ) )    // by lh 执行完成CLU_LoadScript后调用
	{
		LG("init", g_oLangRec.GetString(69));
		return FALSE;
	}

	GetRender().RegisterFunc();

	CFormMgr::s_Mgr.SetEnabled( true );

	if( !_stScriptMgr.LoadScript() )
	{
		LG("init", g_oLangRec.GetString(70));
		return FALSE;
	}

	g_NetIF	= new NetIF;
	g_Editor.Init(1);

	// 装载初始场景
	LoadScriptScene( (eSceneType)g_Config.m_nCreateScene );

	//LoadRes4();

    GetRender().Init();

#ifdef FLOAT_INVALID
	int i = _controlfp(0,0);
	i &= ~(EM_ZERODIVIDE | EM_OVERFLOW | EM_INVALID);
	_controlfp(i, MCW_EM);
#endif

	SetFocus(GetHWND());

    if(g_Config.m_bEditor) SetIsRenderTipText(true);

    _rsm = new RenderStateMgr;
    _rsm->Init(g_Render.GetInterfaceMgr()->dev_obj);

    _IsInit = true;
    
	ResetCaption();

	if( !CGameScene::_InitScene() )
	{
		LG( "init", "msgCGameScene::_InitScene() return false" );
		return false;
	}

#if(defined USE_TIMERPERIOD)
    extern void CALLBACK __timer_period_proc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

    //DWORD fps = (DWORD)((1.0f / 30.0f) * 1000);
    DWORD fps = _dwFPSInterval;
    HWND hwnd = g_pGameApp->GetHWND();
    MPGUIDCreateObject((LW_VOID**)&_TimerPeriod, LW_GUID_TIMERPERIOD);
    _TimerPeriod->SetEvent(fps, fps, __timer_period_proc, (DWORD_PTR)hwnd, TIME_PERIODIC|TIME_CALLBACK_FUNCTION);
#endif
	
    lwIOptionMgr* opt_mgr = g_Render.GetInterfaceMgr()->sys->GetOptionMgr();
#if(defined OPT_CULL_1)
#pragma message("-------------Primitive Culling Opened-------------")
    opt_mgr->SetByteFlag(OPTION_FLAG_CULLPRIMITIVE_MODEL, 1);
#else
#pragma message("-------------Primitive Culling Closed-------------")
    opt_mgr->SetByteFlag(OPTION_FLAG_CULLPRIMITIVE_MODEL, 0);
#endif

	//LPD3DXEFFECT peff,peff2;
	//LPD3DXBUFFER	pbuff;
	//D3DXCreateEffectFromFile(g_Render.GetDevice(),"shader\\dx8\\eff.fx",&peff,NULL);
	//peff->GetCompiledEffect(&pbuff);
	//if(FAILED(D3DXCreateEffect(g_Render.GetDevice(),pbuff->GetBufferPointer(),pbuff->GetBufferSize(),&peff2,NULL)))
	//{
	//	INT EF = 0;
	//}

    // 设置玩家自定义的音频设置(Modify by Michael Chen 2005-04-27)
    if ( !g_stUISystem.m_isLoad)
    {
        g_stUISystem.LoadCustomProp();
    }
	g_pGameApp->GetCurScene()->SetTextureLOD(
		g_stUISystem.m_sysProp.m_videoProp.nTexture);
	GetRender().SetIsChangeResolution(true);

    g_stUISystem.m_sysProp.ApplyAudio();

	// 设置玩家自定义的游戏设置(Modify by Michael Chen 2006-01-17)
	g_stUISystem.m_sysProp.ApplyGameOption();

#ifdef TESTDEMO
    InitTestDemo();
#endif

	//std::list<int> listtest;
	//listtest.push_back(1);
	//listtest.push_back(1);
	//listtest.push_back(1);
	//listtest.push_back(1);
	//listtest.push_back(1);

	//listtest.remove(NULL);

	// 记录主线程ID，方便以后调试
	//DWORD id = GetCurrentThreadId();
	//LG( "threadid", "%d:%s\n", id, "WinMain" );		

	//_pConsole->Show(TRUE);

	return TRUE;
}


void CGameApp::_End()
{
    CFormMgr::s_Mgr.SetEnabled( false );

    extern bool UIClear();
    UIClear();

	SAFE_DELETE( _pCurScene );

    LG("end", "NetIF release start\n");
    SAFE_DELETE( g_NetIF );
    LG("end", "NetIF release end\n");

	CFormMgr::s_Mgr.Clear();

	UnloadResourceSet();
    UnloadTerrainSet();

    //::mus_mgr_exit();	// music
#ifdef USE_DSOUND
	if( g_dwCurMusicID )
	{
		AudioSDL::get_instance()->stop( g_dwCurMusicID );
		g_dwCurMusicID = 0;
		Sleep( 60 );
	}
#endif

    AudioSDL::get_instance()->release();

#if(defined USE_TIMERPERIOD)
    if(_TimerPeriod)
    {
        _TimerPeriod->KillEvent();
        SAFE_RELEASE(_TimerPeriod);
    }
#endif
}

void	CGameApp::OnLostDevice()
{
	 _pDrawPoints->OnLostDevice();

}
void	CGameApp::OnResetDevice()
{
	_pDrawPoints->OnResetDevice();
}

#ifdef USE_DSOUND

void CGameApp::PlaySample( string SoundName )
{
	int SoundChannel = mSoundManager->LoadSound( SoundName );
	if( SoundChannel == -1 )
		return;
	
	SoundInstance* aSoundInstance = mSoundManager->GetSoundInstance( SoundChannel );
	if( aSoundInstance )
		aSoundInstance->Play( false, true );
}

#endif

