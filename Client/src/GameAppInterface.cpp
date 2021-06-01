#include "Stdafx.h"

#include "GameApp.h"
#include "GameConfig.h"

#include "SceneObjSet.h"
#include "EffectSet.h"
#include "MusicSet.h"

#include "Character.h"
#include "MPEditor.h"
#include "Scene.h"

#include "MPFont.h"
#include "SmallMap.h"
#include "LoginScene.h"
#include "WorldScene.h"

#include "GlobalVar.h"
#include "PlaySound.h"
#include "PacketCmd.h"
#include "Track.h"
#include "STStateObj.h"
#include "Actor.h"
#include "UIGlobalVar.h"
#include "UIFormMgr.h" 
#include "uilabel.h"
#include "uitextbutton.h"
#include "UIEditor.h"
#include "UITemplete.h"
#include "UIImage.h"
#include "uicozeform.h"
#include "uinpctalkform.h"
#include "uiboxform.h"
#include "uiprogressbar.h"

#include "SceneObjSet.h"
#include "EffectSet.h"
#include "CharacterPoseSet.h"
#include "Mapset.h"
#include "MusicSet.h"
#include "CharacterRecord.h"
#include "ChaCreateSet.h"
#include "ServerSet.h"
#include "ItemRecord.h"
#include "SkillRecord.h"
#include "AreaRecord.h"
#include "RenderStateMgr.h"
#include "notifyset.h"
#include "SkillStateRecord.h"
#include "ChatIconSet.h"
#include "ItemTypeSet.h"
#include "forgerecord.h"
#include "EventRecord.h"
#include "EventSoundSet.h"
#include "createchascene.h"
#include "SelectChaScene.h"
#include "SteadyFrame.h"
#include "uistartform.h"
#include "cameractrl.h"
#include "shipset.h"

#include "gameloading.h"

#include "StringLib.h"
#include "ItemPreSet.h"
#include "HairRecord.h"
#include "ItemRefineSet.h"
#include "ItemRefineEffectSet.h"
#include "StoneSet.h"
#include "SceneItem.h"
#include "ElfSkillSet.h"
#include "GameMovie.h"

extern CGameMovie g_GameMovie;

#ifndef USE_DSOUND
#include "AudioThread.h"
extern CAudioThread g_AudioThread;
#endif

extern DWORD g_dwCurMusicID;

static bool IsExistFile( const char* file )
{
	HANDLE hFile = CreateFile(file,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if( INVALID_HANDLE_VALUE == hFile ) 
    {
		return false;
    }
    else
    {
        CloseHandle(hFile);
		return true;
	}
}

static void __timer_frame(DWORD time)
{
    g_pGameApp->FrameMove(time);
}
static void __timer_render(DWORD time)
{
    g_pGameApp->Render();
}

void CALLBACK __timer_period_proc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    HWND hwnd = (HWND)dwUser;
    ::SendMessage(hwnd, WM_USER_TIMER, 0, 0);
}

void __timer_period_render()
{
    if(g_pGameApp->IsRun())
    {
        DWORD Tick = timeGetTime();
        if((Tick - g_pGameApp->GetCurTick()) > 18)
        {
            g_pGameApp->SetTickCount(Tick);
            g_pGameApp->FrameMove(Tick);
            g_pGameApp->Render();
        }
    }
}

int CGameApp::Run()
{
	#define R_FAIL  -1;
	#define R_OK 0;

	_dwLoadingTick = g_pGameApp->GetCurTick();
 
	MSG msg;
	memset( &msg, 0, sizeof(msg) );

#if(defined USE_TIMERPERIOD)

    while(GetMessage(&msg, NULL, 0, 0)) 
	{		
        TranslateMessage(&msg);
        DispatchMessage(&msg);
	}
#else
	_dwCurTick = 0;

#if(defined USE_INDIVIDUAL_TIMER)
    MPITimer* timer = 0;
    MPGUIDCreateObject((LW_VOID**)&timer, LW_GUID_TIMER);
    timer->SetTimer(0, __timer_frame, 1.0f/30);
    timer->SetTimer(1, __timer_render, 1.0f/30);
#endif

	//ResetGameCamera(0);

	//CCameraCtrl *pCam = GetMainCam();
	//MPTerrain *pTerr = GetCurScene()->GetTerrain();
	//CCharacter *pCha = _pCurScene->GetMainCha();
	//if(pCha)
	//{
	//	D3DXVECTOR3 vecCha = pCha->GetPos();

	//	_pCamTrack->Reset(vecCha.x,vecCha.y,vecCha.z);
	//	pCam->SetFollowObj(vecCha);
	//	pCam->FrameMove(0);
	//	g_Render.SetWorldViewFOV(Angle2Radian(pCam->m_ffov));
	//	g_Render.LookAt(pCam->m_EyePos, pCam->m_RefPos);
	//	g_Render.SetCurrentView(MPRender::VIEW_WORLD);
	//}

	_isRun = true;

	if( !_pSteady->Init() ) return -1;

	while( _isRun ) 	
	{
        if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
			continue;
		}

		if( _pCurScene )
		{
			//if(g_GameMovie.IsPlaying())	// 视频播放特殊处理
			//{
			//	if(GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			//	{
			//		g_GameMovie.Cleanup();
			//	}
			//	continue;
			//}
			//else
			//{
			//	g_GameMovie.Cleanup();
			//}

#if(defined USE_INDIVIDUAL_TIMER)
            timer->OnTimer();
#else       

			g_NetIF->PeekPacket(1);	// 修正客户端CPU占用率100%
			if( _pSteady->Run() )
			{
				LG( "frame", "time:%u\n", GetTickCount() - _dwCurTick );

	            g_Render.GetInterfaceMgr()->tp_loadres->SetPoolEvent(TRUE);
				_dwCurTick = _pSteady->GetTick();
				//_FrameMoveOnce( _dwCurTick );
				FrameMove( _dwCurTick );
				Render();

				if( _nSwitchScene>0 )
				{
					_ShowLoading();
				}

				_pSteady->End();
	            g_Render.GetInterfaceMgr()->tp_loadres->SetPoolEvent(FALSE);
			}
#endif
		}
	}

#if(defined USE_INDIVIDUAL_TIMER)
    timer->Release();
#endif
#endif
	return (int) msg.wParam;
}

void CGameApp::AutoTestUpdate()
{
	if( !_pCurScene ) return;
	if( !_isRun ) exit(-1);

	MSG msg;
	memset( &msg, 0, sizeof(msg) );
	int i=0;
	for( ;; )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
			continue;
		}

		g_Render.GetInterfaceMgr()->tp_loadres->SetPoolEvent(TRUE);
		_dwCurTick = ::GetTickCount();
		FrameMove( _dwCurTick );
		Render();
	    g_Render.GetInterfaceMgr()->tp_loadres->SetPoolEvent(FALSE);

		if( i>1 ) return;
		i++;
	}
}

void CGameApp::GotoScene( CGameScene* scene, bool isDelCurScene, bool IsShowLoading )
{
	if( !scene ) 
	{
        MsgBox( g_oLangRec.GetString(71) );
		return;
	}  

    if( _pCurScene && !_pCurScene->_Clear() )
    {
        _SceneError( g_oLangRec.GetString(72), _pCurScene );
        SetIsRun( false );
        return;
    }

    if( isDelCurScene && _pCurScene && _pCurScene!=scene )
    {
        delete _pCurScene;
    }
    _pCurScene = scene;

	CFormMgr::s_Mgr.SetEnabled( true );
	CFormMgr::s_Mgr.ResetAllForm();
	CFormMgr::s_Mgr.SwitchTemplete( _pCurScene->GetInitParam()->nUITemplete );

    if( !_pCurScene->_Init() )
    {
        _SceneError( g_oLangRec.GetString(73), _pCurScene );
        SetIsRun( false );
        return;
    }

	if( IsShowLoading )
	{
		_dwLoadingTick = g_pGameApp->GetCurTick();

		static bool isFirstWorld = false;
		if( !isFirstWorld && dynamic_cast<CWorldScene*>(scene) )
		{
			// 第一次切换到游戏场景是较慢,等待时间要加长
			isFirstWorld = true;
			Loading(100);
		}
		else
		{
			Loading();
		}
	}
}

void CGameApp::ResetGameCamera(int type)
{
	CCameraCtrl *pCam = GetMainCam();
	pCam->SetModel(type);
	//if(type==0)
	//{
	//	pCam->m_stDist = g_Config.m_fminxy;
	//	pCam->m_enDist = g_Config.m_fmaxxy;
	//	pCam->m_stHei = g_Config.m_fminHei;
	//	pCam->m_enHei = g_Config.m_fmaxHei;
	//	pCam->m_stFov = g_Config.m_fminfov;
	//	pCam->m_enFov = g_Config.m_fmaxfov;

	//	pCam->m_fxy = g_Config.m_fmaxxy;
	//	pCam->m_fz  = g_Config.m_fmaxHei;
	//	pCam->m_ffov = g_Config.m_fmaxfov;
	//}else
	//{
	//	pCam->m_stDist = g_Config.m_fminxy2;
	//	pCam->m_enDist = g_Config.m_fmaxxy2;
	//	pCam->m_stHei = g_Config.m_fminHei2;
	//	pCam->m_enHei = g_Config.m_fmaxHei2;
	//	pCam->m_stFov = g_Config.m_fminfov2;
	//	pCam->m_enFov = g_Config.m_fmaxfov2;

	//	pCam->m_fxy = g_Config.m_fmaxxy2;
	//	pCam->m_fz  = g_Config.m_fmaxHei2;
	//	pCam->m_ffov = g_Config.m_fmaxfov2;

	//}
	//pCam->m_fstep1 = 1.0f;

	//pCam->m_fResetRotatVel = g_Config.m_fCameraVel;
	//pCam->m_fResetRotatAccl = g_Config.m_fCameraAccl;

	//////pCam->SetFollowObj(_pCurScene->GetMainCha()->GetPos());
	//GetMainCam()->InitAngle(0.5235987f + 0.0872664f);
	////GetMainCam()->ResetCamera(0.5235987f + 0.0872664f);
	//g_Render.SetWorldViewFOV(Angle2Radian( GetMainCam()->m_ffov));
}
	
void    CGameApp::ResetCamera()
{
	CCameraCtrl *pCam = g_pGameApp->GetMainCam();
	GetMainCam()->ResetCamera(0.5235987f + 0.0872664f);
	//g_Render.SetWorldViewFOV(Angle2Radian( GetMainCam()->m_cameractrl.m_ffov));
}

void	CGameApp::CreateCharImg()
{
	static int id = 1;
	static	float fTime = *ResMgr.GetDailTime();

	CCharacter *pMainCha =_pCurScene->GetMainCha();

	fTime += *ResMgr.GetDailTime();
	bool be = false;
	CCharacter *pCha = NULL;
	CChaRecord* pInfo = GetChaRecordInfo( id );

	D3DXVECTOR3 veye(pMainCha->GetPos());
	D3DXVECTOR3 vlookat = veye;

	veye.y += 10;
	veye.z += 3.5f;

	vlookat.z += 1.5f;


	if(fTime > 1.0f)
	{
		pCha = _pCurScene->AddCharacter(id);
		if(pCha)
		{
			pCha->PlayPose( 1, PLAY_PAUSE );
			pCha->setYaw( 180 );

			pCha->setPos(pMainCha->GetCurX(),pMainCha->GetCurY());

			//if(pInfo->chTerritory == 1)
			//	pCha->SetHieght(1.6f);
			pCha->FrameMove(0);


			veye = pCha->GetPos();
			vlookat = veye;

			veye.y += 12;
			veye.z += 3.5f;

			vlookat.z += 1.5f;

		}else
		{
			id++;
			if(id > 670)
			{
				btest  = false;
				pMainCha->SetHide(FALSE);
			}
			fTime = 0;
			return;
		}

		be = true;
		fTime = 0;
	}
	pMainCha->SetHide(TRUE);
	g_Render.LookAt(veye, vlookat);
	g_Render.SetCurrentView(MPRender::VIEW_WORLD);

	_pCurScene->_Render();

	if(be)
	{
		char szPath[255]; 
		char fileName[255]; 
		sprintf(szPath, "screenshot/cha");				
		Util_MakeDir(szPath);

		sprintf(fileName,"%s/%s.bmp", szPath, pInfo->szDataName);

		g_Render.CaptureScreen(fileName);

		if(pCha)
			pCha->SetValid(FALSE);
		id++;
	}
	if(id > 670)
	{
		btest  = false;
		pMainCha->SetHide(FALSE);
	}
}

////////////////////////
BOOL	CGameApp::_CreateSmMap( MPTerrain* pTerr )
{
	static	bool isnext = true;

	static	float fTime = *ResMgr.GetDailTime();

	if( !_pCurScene->GetMainCha() ) return FALSE;

	fTime += *ResMgr.GetDailTime();
	if(fTime > 0.5f)
	{
		if(isnext)
		{
			pTerr->SetShowSize(SHOWRSIZE + 5,SHOWRSIZE + 5);
			_pCurScene->GetMainCha()->setPos((int)xp * 100, (int)yp * 100);
			SetCameraPos(_pCurScene->GetMainCha()->GetPos());
			isnext = false;
			fTime = 0;
			return TRUE;
		}
		bool	bSea = true;
		fTime = 0;


		if(!isnext)
		{
			MPTile *TileList;
			int se;
			int e = (int)xp - SHOWRSIZE/2;
			se = e;
			int w = (int)yp - SHOWRSIZE/2;
			for(; w <  int(yp + SHOWRSIZE/2); w++)
			{
				for(; e < int(xp + SHOWRSIZE/2);e++)
				{
					TileList = pTerr->GetTile(e, w);
					if(!TileList->IsDefault())
					{
						bSea = false;
						goto skip;
					}
				}
				e = se;
			}
skip:

			if( int(xp / SHOWRSIZE) >= (destxp /SHOWRSIZE))
			{
				xp = xp1;
				yp += SHOWRSIZE;
				if(int(yp / SHOWRSIZE) >= destyp /SHOWRSIZE)
				{
					MessageBox(NULL, g_oLangRec.GetString(74),"INFO",0);
					xp = xp1;
					yp = yp1;

					_pCurScene->GetMainCha()->setPos((int)xp * 100,(int)yp * 100);
					SetCameraPos(_pCurScene->GetMainCha()->GetPos());

					EnableSprintSmMap(FALSE);
				}
				isnext = true;
				return TRUE;
			}else
			{
				xp += SHOWRSIZE;
				isnext = true;
			}
			fTime = 0;
		}
		if(bSea)
			return TRUE;//如果全部是海，不生成。

		D3DXMATRIX matView, matProj;
		D3DXVECTOR3 vEyePt = _pCurScene->GetMainCha()->GetPos();
		vEyePt.z = SHOWRSIZE;
		D3DXVECTOR3 vLookatPt = vEyePt;
		vLookatPt.z = 0;
		D3DXVECTOR3 vUpVec = D3DXVECTOR3(0,-1,0);
		D3DXMatrixOrthoLH(&matProj, SHOWRSIZE, 
			SHOWRSIZE, 0.0f, 1000.0f);
		g_Render.SetTransformProj(&matProj);
		char fileName[64];

		D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
		g_Render.SetTransformView(&matView);

		pTerr->SetShowCenter(vEyePt.x, vEyePt.y);
		pTerr->SetShowSize(SHOWRSIZE + 5,SHOWRSIZE + 5);
		_pCurScene->RenderSMallMap();

		char szPath[255]; 
		sprintf(szPath, "texture/minimap/%s",_pCurScene->GetTerrainName());				
		Util_MakeDir(szPath);
		sprintf(fileName,"%s/sm_%d_%d.bmp", szPath, 
			int(vEyePt.x / SHOWRSIZE),int(vEyePt.y / SHOWRSIZE));

		g_Render.CaptureScreen(fileName);

		LPDIRECT3DTEXTURE8		pTex = NULL;
		D3DXCreateTextureFromFileEx(g_Render.GetDevice(),
			fileName, //文件名
			256, //文件宽，这里设为自动
			256, //文件高，这里设为自动
			1, //需要多少级mipmap，这里设为1
			0, //此纹理的用途
			D3DFMT_UNKNOWN, //自动检测文件格式
			D3DPOOL_MANAGED, //由DXGraphics管理
			D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR|D3DX_FILTER_BOX, //纹理过滤方法
			D3DX_FILTER_NONE, //mipmap纹理过滤方法
			0x00000000, //透明色颜色
			NULL, //读出的图像格式存储在何变量中
			NULL, //读出的调色板存储在何变量中
			&pTex);//要创建的纹理
		D3DXSaveTextureToFile(fileName,D3DXIFF_BMP,pTex,NULL);
		SAFE_RELEASE(pTex);
	}
	return TRUE;
}

BOOL CGameApp::_PrintScreen()
{
	if(IsEnableSpAvi()) 
    {
        _CreateAviScreen();
    }
    else
    {
        static int g_nScreenCap = 0;
	    char fileName[64];
        Util_MakeDir("screenshot\\");

		char pszName[64];

		int nidx = 0;
		while(1)
		{
			sprintf(pszName,"screenshot\\%d\\",nidx);
			if(_access(pszName,0)== -1)
			{
				Util_MakeDir(pszName);
				break;
			}
		}
		sprintf(fileName,"%scap%05d.bmp",pszName, g_nScreenCap);
		g_Render.CaptureScreen(fileName);

		char szTip[64];
		sprintf(szTip, g_oLangRec.GetString(75), fileName);
        Tip(szTip);
        g_nScreenCap++;
	    EnableSprintScreen(FALSE);
	}
    return TRUE;
}

BOOL	CGameApp::_CreateAviScreen()
{
	/*static int g_nAviCnt = 0;
	char fileName[64];
    Util_MakeDir("screenshot/");

	sprintf(fileName,"screenshot/cap%05d.bmp",g_nAviCnt);
	g_Render.CaptureScreen(fileName);
	g_nAviCnt++;*/
	return TRUE;
}

CGameScene*	CGameApp::CreateScene( stSceneInitParam* param )
{
	if( !param ) return NULL;

	if( param->nTypeID>=enumSceneEnd ) return NULL;

	CGameScene * scene = NULL;
	switch( param->nTypeID )
	{
        case enumLoginScene:        scene=new CLoginScene(*param);       break;
        case enumSelectChaScene:    scene=new CSelectChaScene(*param);   break;
        case enumCreateChaScene:    scene=new CCreateChaScene(*param);   break;
	    case enumWorldScene:
        {
            scene = new CWorldScene(*param);
            break;
        }
	}

	if( scene && scene->_CreateMemory() )
		return scene;

	return NULL;
}

BOOL CGameApp::CreateCurrentScene(char *szMapName)
{
	stSceneInitParam stInit;
	stInit.nTypeID = enumLoginScene;
	stInit.strName = g_oLangRec.GetString(76);
	stInit.strMapFile = szMapName;
	stInit.nMaxEff = 300;
	stInit.nMaxCha = 300;
	stInit.nMaxObj = 400;
	stInit.nMaxItem = 400;
	CGameScene * s = g_pGameApp->CreateScene( &stInit );
	if (!s)
		return FALSE;
	_pCurScene = s;
	return TRUE;
}

void CGameApp::_RenderTipText()
{     
if( !_TipText.empty() )
	{
		//屏幕上的文字提示(渐消)
		int nTextY = 220;	    
        SIZE sz;
		for(list<STipText*>::iterator it = _TipText.begin(); it!=_TipText.end(); it++)
		{
			STipText *pText = (*it);
			if(GetCurTick() - pText->dwBeginTime > 3000)
			{
				if(pText->btAlpha > 10) pText->btAlpha-=10;
				if(pText->btAlpha < 10) pText->btAlpha = 0;
			}
			DWORD dwAlpha = pText->btAlpha << 24;
			DWORD dwColor = dwAlpha | 0x00f01000; 

            g_CFont.GetTextSize( pText->szText, &sz );
			g_CFont.DrawText(pText->szText, ( GetWindowWidth() - sz.cx ) / 2, nTextY, dwColor);
			nTextY+=24;
		}

		if(_TipText.size() > 0)
		{
			STipText *pText = _TipText.front();
			if(pText) 
			{
				if(pText->btAlpha <=0) _TipText.pop_front();
			}
		}
	}
}

void CGameApp::PlayMusic(int nMusicNo)
{
    if ( !CGameApp::IsMusicSystemValid() ) return;
    
    //SetBkMusicNotify(_hWnd, WM_MUSICEND);
    CMusicInfo *pInfo = NULL;
    //
    //if(nMusicNo==0) // 顺序播放
    //{
    //    static g_dwPlayMusicNo = 0;
    //    g_dwPlayMusicNo++;
    //    CMusicInfo *pInfo = GetMusicInfo(g_dwPlayMusicNo);
    //    if( !pInfo || strlen(pInfo->szDataName)==0 )
    //    {
    //        g_dwPlayMusicNo = 1; // 回到第一首
    //    }
    //    nMusicNo = g_dwPlayMusicNo;
    //}
    if(nMusicNo>=1) // 指定编号播放
    {
        pInfo = GetMusicInfo(nMusicNo);
        if( pInfo && strlen(pInfo->szDataName)==0)
        {
            pInfo = NULL;
        }
    }

    if( !pInfo ) 
    {
        _szBkgMusic[0] = '\0';
        _eSwitchMusic = enumOldMusic;
        return;
    }

    switch( _eSwitchMusic )
    {
    case enumNoMusic:
        {
            strncpy( _szBkgMusic, pInfo->szDataName, sizeof(_szBkgMusic) );

            _eSwitchMusic=enumNewMusic;
			DWORD OldMusicID = g_dwCurMusicID;
			g_dwCurMusicID = AudioSDL::get_instance()->get_resID(_szBkgMusic, TYPE_MP3);
			AudioSDL::get_instance()->volume(g_dwCurMusicID, _nCurMusicSize);
#ifndef USE_DSOUND
            g_AudioThread.play(g_dwCurMusicID, true);
#else
			if( g_dwCurMusicID && ( OldMusicID != g_dwCurMusicID ) )
			{
				AudioSDL::get_instance()->stop( OldMusicID );
				AudioSDL::get_instance()->play( g_dwCurMusicID, true );
			}
#endif

        }
        break;
    case enumOldMusic:
        {
            strncpy( _szBkgMusic, pInfo->szDataName, sizeof(_szBkgMusic) );
        }
        break;
    case enumNewMusic:
    case enumMusicPlay:
        if( strncmp( _szBkgMusic, pInfo->szDataName, sizeof(_szBkgMusic) )!=0 )
        {
            strncpy( _szBkgMusic, pInfo->szDataName, sizeof(_szBkgMusic) );
            _eSwitchMusic = enumOldMusic;
        }
        break;
    }
}

void CGameApp::PlaySound(int nSoundNo)
{
    if ( !CGameApp::IsMusicSystemValid() ) return;

	if( nSoundNo == -1 ) return;

	CMusicInfo *pInfo = GetMusicInfo(nSoundNo);
	if( !pInfo || pInfo->nType!=1 ) return;

#ifdef USE_DSOUND
	PlaySample( pInfo->szDataName );
#else
	ulong musid = AudioSDL::get_instance()->get_resID(pInfo->szDataName, TYPE_WAV);
	AudioSDL::get_instance()->volume(musid, (int)CGameScene::_fSoundSize);
    g_AudioThread.play(musid, false);
#endif
}

void CGameApp::SetMusicSize( float fVol )
{
    if ( !CGameApp::IsMusicSystemValid() ) return;

    if( fVol>=0.0 && fVol<=1.0 )
    {
        _nMusicSize = (int)(fVol * 128.0f);

        //::bkg_snd_vol( _nMusicSize );
		AudioSDL::get_instance()->volume(g_dwCurMusicID, _nMusicSize);
    }
}

static bool WaitModalForm(CGuiData *pSender, char& key )
{
    if( key==VK_RETURN )
    {
        CForm* frmWaiting = dynamic_cast<CForm*>(pSender);
        if( frmWaiting )
        {
            frmWaiting->Close();
            return true;
        }
    }
    return false;
}

void CGameApp::MsgBox( const char *pszFormat, ... )
{
    va_list list;
    va_start(list, pszFormat);           
    vsprintf(_szOutBuf, pszFormat, list);
    va_end(list);    

	g_stUIBox.ShowMsgBox( NULL, _szOutBuf );
}

void CGameApp::ShowBigText( const char *pszFormat, ... )
{
    va_list list;
    va_start(list, pszFormat);           
    vsprintf(_szOutBuf, pszFormat, list);
    va_end(list);    
    
	g_stUIStart.ShowBigText( _szOutBuf );
}

void CGameApp::ShowMidText( const char *pszFormat, ... )
{
    va_list list;
    va_start(list, pszFormat);           
    vsprintf(_szOutBuf, pszFormat, list);
    va_end(list);    

    strncpy( _stMidFont.szText, _szOutBuf, sizeof(_stMidFont.szText) );
    _stMidFont.dwBeginTime = GetCurTick() + SHOW_TEXT_TIME;
    _stMidFont.btAlpha     = 255;

    //g_stUICoze.OnSystemSay( _szOutBuf );
	CCozeForm::GetInstance()->OnSystemMsg(_szOutBuf);
}

void CGameApp::ShowBottomText( unsigned int rgb, const char *pszFormat, ... )
{
    va_list list;
    va_start(list, pszFormat);           
    vsprintf(_szOutBuf, pszFormat, list);
    va_end(list);   

	STipText* tmpSize = new STipText;

	strncpy( tmpSize->szText, _szOutBuf, sizeof(tmpSize->szText) );
	tmpSize->dwBeginTime = GetCurTick() + TIME_CQUEUE;
	tmpSize->btAlpha = 255;

	if (_qCQueueStrColour.size() >= MAX_CQUEUE)
	{
		pair <STipText*, unsigned int> curpair;
		curpair = _qCQueueStrColour.front();
		_qCQueueStrColour.pop();
	}
	_qCQueueStrColour.push(make_pair(tmpSize, rgb));
	_IsRenderColourTest = true;
	//CCozeForm::GetInstance()->OnSystemMsg(_szOutBuf);
}

void CGameApp::RenderColourQueue()
{
	if (_qCQueueStrColour.empty())
	{
		return;
	}
	// peek the first one
	pair <STipText*, unsigned int> curpair;
	curpair = _qCQueueStrColour.back();

	// check if time is up
	STipText* tmp = curpair.first;
	if (GetCurTick() > tmp->dwBeginTime + TIME_CQUEUE)
	{
		// remove it
		//MessageBoxA(NULL, "done", "title", 0);
		_IsRenderColourTest = false;
		_qCQueueStrColour.pop();
		delete tmp;
	}
	else
	{
		// render
		queue< pair<STipText*, unsigned int> > tmpQueue = _qCQueueStrColour;
		int i;
		for (i = 0; i < MAX_CQUEUE; i++)
		{
			if (tmpQueue.empty())
			{
				break;
			}

			curpair = tmpQueue.front();
			STipText* tmp = curpair.first;
			if (GetCurTick() < tmp->dwBeginTime + TIME_CQUEUE)
			{
				//tmp->btAlpha = (BYTE)(255.0f * (float)(tmp->dwBeginTime - GetCurTick()) / (float)TIME_CQUEUE);

				DWORD dwAlpha = tmp->btAlpha << 24;
				DWORD dwColor = dwAlpha | curpair.second;

				SIZE size;
				_BottomFont.GetTextSize(tmp->szText, &size);
				/*_BottomFont.DrawText(tmp->szText,
									(GetWindowWidth()-size.cx) / 2,
									((floor(GetWindowHeight() * 2.2)) - size.cy) / 3 - 18 * i,
									//18 * i + ((GetWindowHeight() * 2) - size.cy) / 3,
									dwColor);*/

				int x = (GetWindowWidth()-size.cx) / 2;
				int y = (floor(GetWindowHeight() * 2.025) - size.cy) / 3 + 18 * i;
				_BottomFont.DrawTextShadow(tmp->szText, x+1, y+1, x, y, D3DXCOLOR(0,0,0,1), (DWORD)dwColor ); //0xff808000
			}
			else
			{
				tmpQueue.pop();
				_qCQueueStrColour = tmpQueue;
			}
			tmpQueue.pop();
			//_IsRenderColourTest = false;
		}
		//_IsRenderColourTest = false;
	}
}

void CGameApp::AddTipText(const char *pszFormat, ...)
{
    va_list list;
    va_start(list, pszFormat);           
    vsprintf(_szOutBuf, pszFormat, list);
    va_end(list);    

    STipText *pText = new STipText;
    strncpy( pText->szText, _szOutBuf, TIP_TEXT_NUM );
    pText->dwBeginTime = GetTickCount();
    pText->btAlpha     = 255;
    _TipText.push_back(pText);
}

void CGameApp::ShowStateHint( int x, int y, CChaStateMgr::stChaState stateData){
	char szTitle[32];
	sprintf(szTitle,"Lv%d %s",stateData.chStateLv,stateData.pInfo->szName);
	_pNotify->SetFixWidth( 0 );
	_dwNotifyTime = GetCurTick() + 100;
	_pNotify->Clear();
	_pNotify->PushHint( szTitle, COLOR_WHITE );
	if (!stateData.pInfo->szDesc)
		_pNotify->PushHint( stateData.pInfo->szDesc, 0xFF000000|stateData.pInfo->lColour );
	if(stateData.lTimeRemaining > 0){
		int minutes = stateData.lTimeRemaining/60;
		int seconds = stateData.lTimeRemaining%60;
		char szTime[32];
		sprintf(szTime,"Time Remaining: %ds",stateData.lTimeRemaining);
		//sprintf(szTime,"Time Remaining: %d:%02d",minutes,seconds);
		_pNotify->PushHint( szTime, COLOR_WHITE );
	}
	_pNotify->ReadyForHint( x, y );
}

void CGameApp::ShowHint( int x, int y, const char *szStr, DWORD dwColor )
{
	_pNotify->SetFixWidth( 0 );

	_dwNotifyTime = GetCurTick() + 100;
	_pNotify->Clear();
	_pNotify->PushHint( szStr, dwColor );
	_pNotify->ReadyForHint( x, y );


	//disabled by billy 20/06/2018
	//Add by sunny.sun20080804
	//_pNotify1->SetFixWidth( 0 );
	//_dwNotifyTime1 = GetCurTick() + 100;
	//_pNotify1->Clear();
	//_pNotify1->PushHint( szStr, dwColor );
	//SetNum = 1;
	//_pNotify1->ReadyForHint( x, y,SetNum);
	//End
}

void CGameApp::ShowNotify( const char *szStr, DWORD dwColor )
{
	_pNotify->SetFixWidth( 430 );

	_dwNotifyTime = GetCurTick() + 120000;

	_pNotify->Clear();
	static bool titleFlag;
	titleFlag=false;			//允许特殊标题
	string text=szStr;
	while (!text.empty())
	{
		int n=1;
		string strSub=text.substr(0,1);
		for (;n<72;n++)
		{
			if (n>=(int)text.length())
			{
				break;
			}
			if (text[n]==':' && titleFlag)
			{
				titleFlag=false;
				n++;
				break;
			}
			strSub=text.substr(0,n);
		}
		string str=CutFaceText(text,n);
		_pNotify->PushHint( str.c_str(), dwColor );
	}
	//_pNotify->ReadyForHint( 170, 5 );
	_pNotify->ReadyForHintGM( 170, 5 );
}

//Add by sunny.sun20080804
//Begin
void CGameApp::ShowNotify1( const char *szStr,int setnum, DWORD dwColor )
{
	_pNotify1->SetFixWidth( 430 );

	_dwNotifyTime1 = GetCurTick() + 120000;

	_pNotify1->Clear();

	static bool titleFlag;
	titleFlag=false;			//允许特殊标题
	string text=szStr;

	_pNotify1->PushHint( text.c_str(), dwColor );
	_pNotify1->ReadyForHint( 170, 100,setnum);
}
//End

void CGameApp::AutoTestInfo( const char *pszFormat, ... )
{
    va_list list;
    va_start(list, pszFormat);           
    vsprintf(_szOutBuf, pszFormat, list);
    va_end(list);

    string info = _szOutBuf;
	ShowMidText( info.c_str() );
	info += "\n";
	LG( "autotest", info.c_str() );

	FrameMove( GetTickCount() );
	Render();
}

void CGameApp::SysInfo( const char *pszFormat, ... )
{
    va_list list;
    va_start(list, pszFormat);           
    vsprintf(_szOutBuf, pszFormat, list);
    va_end(list);

    string info = _szOutBuf;
    //g_stUICoze.OnSystemSay( info.c_str() );
	CCozeForm::GetInstance()->OnSystemMsg(info.c_str());
}

void CGameApp::Waiting( bool isWaiting )
{
    static CForm* frmWaiting = NULL;
    if( !frmWaiting )
    {
        frmWaiting = CFormMgr::s_Mgr.Find( "frmConnect", enumSwitchSceneForm );
		if( frmWaiting )
		{
			frmWaiting->SetIsEnabled(false);
		}
    }

    if( frmWaiting )
    {
        if ( isWaiting )
        {
            CCursor::I()->SetCursor( CCursor::stWait );
            frmWaiting->ShowModal();
        }
        else
        {
            CCursor::I()->SetCursor( CCursor::stNormal );
            frmWaiting->Close();
        }
    }
}

void	CGameApp::SetCameraPos(D3DXVECTOR3& pos,bool bRestoreCustom)
{
	CCameraCtrl *pCam = GetMainCam();

	//GetCameraTrack()->Reset(pos.x,pos.y,pos.z);

	pCam->InitPos(pos.x,pos.y,pos.z,bRestoreCustom);

	pCam->Update();

	pCam->SetFollowObj(pCam->m_vCurPos);

	pCam->FrameMove(0);

	g_Render.SetWorldViewFOV(Angle2Radian(pCam->m_ffov));
	g_Render.LookAt(pCam->m_EyePos, pCam->m_RefPos);
	g_Render.SetCurrentView(MPRender::VIEW_WORLD);
}

// 用Loading
static CImage* flashProgress[32] = { 0 };
static CForm* frmLoading = NULL;
static bool bChangeFormLoginToSelCha  = false;

void CGameApp::_ShowLoading()
{
    _nSwitchScene--;

    const int LOAD_DELAY = 10;
    if( GetCurScene()->GetTerrain() && _nSwitchScene>LOAD_DELAY )
    {
        CCharacter *pMain = CGameScene::GetMainCha(); 
        if(pMain)
        {
            // LG("load", "camera pos = %.1f  %.1f  %.1f\n", _pMainCam->m_vCurPos.x, _pMainCam->m_vCurPos.y, _pMainCam->m_vCurPos.z);

			float fX = (float)pMain->GetCurX() / 100.0f;
            float fY = (float)pMain->GetCurY() / 100.0f;
            // LG("load", "main char pos = [%.2f %.2f]\n", fX, fY);
            float fHeight = GetCurScene()->GetGridHeight(fX, fY);
            // LG("load", "terrain height = %.1f\n", fHeight);
			bool isOK = false;
            if( fHeight > 0.001f)
            {
				isOK = true;
            }
			else
			{
				MPTerrain* pTer = GetCurScene()->GetTerrain();
				if( pTer && pTer->GetTileRegionAttr( (int)fX, (int)fY )!=0 )
				{
					isOK = true;
				}
			}
			if( isOK )
			{
                _nSwitchScene = LOAD_DELAY;
				pMain->setPos( pMain->GetCurX(), pMain->GetCurY() );

                // LG("load", "loading disappear!\n");
				//GetMainCam()->InitModel(pMain->IsBoat() ? 3 : 0, &D3DXVECTOR3(fX,fY,fHeight));
				//CCameraCtrl *pCam = g_pGameApp->GetMainCam();
				//MPTerrain *pTerr = GetTerrain();

				//D3DXVECTOR3 vecCha = pCha->GetPos();
				//vecCha.z = GetGridHeight(vecCha.x,vecCha.y);

				//pCam->InitModel(pCha->IsBoat() ? 3 : 0,&vecCha);
				//pCam->SetBufVel( pCha->getMoveSpeed(), nChaID);
				//pCam->FrameMove(0);
				//pCam->SetViewTransform();
			}
        }
    }

    if( frmLoading )
    {
        for ( int i=0; i<32; i++ ) 
        {
            if( !flashProgress[i] ) continue;
            flashProgress[i]->SetIsShow(false);
            if ( i == 32 - _nSwitchScene/4 )
            {
                flashProgress[i]->SetPos( 0, (int)(GetRender().GetScreenHeight()  * 0.9f)  );				
                flashProgress[i]->Refresh();

                flashProgress[i]->SetIsShow(true);
            }
        }

		// LOADING进度条
		RefreshLoadingProgress();
		//float fPos = (g_pGameApp->GetCurTick() - _dwLoadingTick) / 10000.0f;
		//SetLoadingProgressPos(fPos <= 1.0f ? fPos : 1.0f);
    }
    if( _nSwitchScene<=0 )
    {
        _IsUserEnabled = true;
        SetInputActive(true);
        //snd_enable( true );

        GetCurScene()->LoadingCall();
		CUIInterface::All_LoadingCall();

		if( frmLoading ) 
		{
			frmLoading->Close();

			//  关闭loading界面
			GameLoading::Init()->Active();
			GameLoading::Init()->Close();


			//HWND hwnd = g_pGameApp->GetHWND();
			//SetForegroundWindow (hwnd);
		}
    }
}


void CGameApp::RefreshLoadingProgress()
{
	const int PROGRESS_CRYCLE  = 1000;			// 一个轮循（毫秒）
	const int PROGRESS_WIDTH   = GetWindowWidth()-200; // the length of the loading bar
	const int PROGRESS_X       = 100;
	const int PROGRESS_Y       = ( GetWindowHeight() == SMALL_RES_Y ) ? 418 : 567;

	//const int PROGRESS_WIDTH   = 523 * GetWindowWidth()  / 800;
	//const int PROGRESS_X       = 132 * GetWindowWidth()  / 800;
	//const int PROGRESS_Y       = 447 * GetWindowHeight() / 600;

	// 起始 （X，Y） = （132, 545）

	CForm* frmLoading = CFormMgr::s_Mgr.Find( "frmLoading", enumSwitchSceneForm );
	if(frmLoading)
	{
		CImage* imgLoading = dynamic_cast<CImage*>(frmLoading->Find("imgLoading"));
		if(imgLoading)
		{
			float fPrecent = ((GetCurTick() - _dwLoadingTick) % PROGRESS_CRYCLE) / (float)(PROGRESS_CRYCLE);	// 在当前进度条的百分比
			int nX = (int)((PROGRESS_WIDTH - imgLoading->GetWidth()) * fPrecent + PROGRESS_X);
			int nY = PROGRESS_Y;

			imgLoading->SetPos(nX, nY);
			imgLoading->Refresh();
		}

		CLabelEx* labLoad =  dynamic_cast<CLabelEx*>(frmLoading->Find( "labLoad" ));
		if( labLoad )
		{
			int x = labLoad->GetLeft();
			int y = ( GetWindowHeight() == SMALL_RES_Y ) ? 523 : 679;
			labLoad->SetPos( x, y );
		}
	}
}


void CGameApp::ResetCaption()
{
	extern short g_sClientVer;
	char szSpace[250] = "";
	int nSpace = 40;
	int nWidth = GetRender().GetScreenWidth();
	if(nWidth==LARGE_RES_X) 
	{
		nSpace = 140;
	}
   
	for(int i = 0; i < nSpace; i++) szSpace[i] = ' ';
	szSpace[nSpace] = '\0';
   	
	char szCaption[350]; sprintf(szCaption, CLIENT_NAME " - " VERSION_NAME, (float)(g_sClientVer % 1000) / 100, szSpace ); // , curr_ver.c_str());
	SetCaption(szCaption);
}

void CGameApp::Loading( int nFrame )
{
	_IsUserEnabled = false;

	//snd_enable( false );
	_nSwitchScene = nFrame;
	SetInputActive(false);

	if (!frmLoading )
	{
		frmLoading = CFormMgr::s_Mgr.Find( "frmLoading", enumSwitchSceneForm );
	}
	if (!frmLoading )
	{
		return;
	}
	frmLoading->SetSize( GetRender().GetScreenWidth()  ,GetRender().GetScreenHeight() );	
	frmLoading->Refresh(); 
	frmLoading->ShowModal();


	int width = (GetRender().GetScreenWidth()==LARGE_RES_X) ? (LARGE_RES_X/4) : (SMALL_RES_X/4);
	int height= (GetRender().GetScreenHeight()==LARGE_RES_Y) ? (LARGE_RES_Y/3) : (SMALL_RES_Y/3);//width;
	if(g_Config.m_bFullScreen)
	{
		// 伪全屏处理
		width  = GetSystemMetrics(SM_CXSCREEN) / 4;
		
		height = GetSystemMetrics(SM_CYSCREEN) / 3;
		height += 32;
	}


	CImage  *imgLoading = NULL;
    char szBuf[8];
    std::string sImgFile = "imgLoading";
    for (int i=1; i<=12; ++i)
    {
        imgLoading = NULL;
        
        imgLoading = dynamic_cast<CImage *>(frmLoading->Find((sImgFile + itoa(i, szBuf, 10)).c_str()))  ;
        if (imgLoading)
        {
            imgLoading->SetPos(((i-1)%4)*width, ((i-1)/4)*height);
            imgLoading->SetSize( width , height);
            imgLoading->Refresh();
        }
    }
}


void CGameApp::InitAllTable()
{
	BOOL bBinary = FALSE; // 此变量已改为全局g_bBinaryTable控制

    CSceneObjSet *pObjSet = new CSceneObjSet(0, g_Config.m_nMaxSceneObjType);
	pObjSet->LoadRawDataInfo("scripts/table/sceneobjinfo", bBinary);

    CEffectSet* pEffSet = new CEffectSet(0, g_Config.m_nMaxEffectType);
	pEffSet->LoadRawDataInfo("scripts/table/sceneffectinfo", bBinary);

	CShadeSet* pShadeSet = new CShadeSet(0, g_Config.m_nMaxEffectType);
	pShadeSet->LoadRawDataInfo("scripts/table/shadeinfo", bBinary);

	CEventSoundSet* pEventSoundSet = new CEventSoundSet( 0, 30 );
	pEventSoundSet->LoadRawDataInfo("scripts/table/eventsound", bBinary);

    CMusicSet* pMusicSet = new CMusicSet(0, 500);
    pMusicSet->LoadRawDataInfo("scripts/table/musicinfo", bBinary);

    CPoseSet* pPoseSet = new CPoseSet(0, 100);
    pPoseSet->LoadRawDataInfo("scripts/table/characterposeinfo", bBinary);

    CChaCreateSet* pChaCreateSet = new CChaCreateSet(0, 60);
    pChaCreateSet->LoadRawDataInfo("scripts/table/selectcha", bBinary);

	CSkillRecordSet* pSkillSet = new CSkillRecordSet( 0, 500 );
	pSkillSet->LoadRawDataInfo("scripts/table/skillinfo", bBinary);

    CMapSet* pMapSet = new CMapSet(0, 100);
    pMapSet->LoadRawDataInfo("scripts/table/mapinfo", bBinary);

	CChaRecordSet* pChaSetAttrib = new CChaRecordSet(0, 2500);
	pChaSetAttrib->LoadRawDataInfo("scripts/table/Characterinfo", bBinary);

    CItemRecordSet* pItemSet = new CItemRecordSet(0, g_Config.m_nMaxItemType);
    pItemSet->LoadRawDataInfo("scripts/table/iteminfo", bBinary);

    CEventRecordSet* pEvent = new CEventRecordSet( 0,10 );
    pEvent->LoadRawDataInfo("scripts/table/objevent", bBinary);

    CAreaSet* pArea = new CAreaSet( 0, 300 );
    pArea->LoadRawDataInfo("scripts/table/AreaSet", bBinary);

    CServerSet* pServer = new CServerSet( 0, 100 );

	if(g_bBinaryTable)
	{
		////////////////////////////////////////
		//
		//  By Jampe
		//  合作商ServerSet处理
		//  2006/5/19
		//
		switch(g_cooperate.code)
		{
		case COP_OURGAME:
			{
				pServer->LoadRawDataInfo("scripts/table/ServerSet2", bBinary);
			}  break;
		case COP_SINA:
			{
				pServer->LoadRawDataInfo("scripts/table/ServerSet3", bBinary);
			}  break;
        case COP_CGA:
            {
                pServer->LoadRawDataInfo("scripts/table/ServerSet4", bBinary);
            }  break;
		case 0:
		default:
			//CLanguageRecord t;
			//t.MadeBinFile("./scripts/table/serverset.bin", "./scripts/table/serverset.txt");

			pServer->LoadRawDataInfo("scripts/table/ServerSet", bBinary);
		}
		//  end
	}
	else
	{
		// add by Philip.Wu  2006-06-12  修改 makebin 不能完整生成全部的 serverset.txt （策划需求）
		pServer->LoadRawDataInfo("scripts/table/ServerSet", bBinary);

		// Delete by lark.li 20080305
		// add by Philip.Wu  2006-07-31  多语言统一后的字符串资源生成 BIN 文件
		//g_oLangRec.MadeBinFile("./scripts/table/StringSet.bin", "./scripts/table/StringSet.txt");
	}

    CNotifySet* pNotify = new CNotifySet( 0, 100 );
    pNotify->LoadRawDataInfo("scripts/table/notifyset", bBinary);

	// Modify by lark.li 20080822 begin
    //CSkillStateRecordSet* pState = new CSkillStateRecordSet( 0, 240 );
	CSkillStateRecordSet* pState = new CSkillStateRecordSet( 0, 300 );
	// End

    pState->LoadRawDataInfo("scripts/table/skilleff", bBinary);

    CChatIconSet* pIcon = new CChatIconSet( 0, 100 );
    pIcon->LoadRawDataInfo("scripts/table/chaticons", bBinary);

	CItemTypeSet* pItemType = new CItemTypeSet( 0, 100 );
    pItemType->LoadRawDataInfo("scripts/table/itemtype", bBinary);

	CItemPreSet* pItemPre = new CItemPreSet( 0, 100 );
    pItemPre->LoadRawDataInfo("scripts/table/itempre", bBinary);

	CForgeRecordSet *pRecordSet = new CForgeRecordSet( 1, ROLE_MAXNUM_FORGE );
	pRecordSet->LoadRawDataInfo( "scripts/table/forgeitem", bBinary );

	xShipSet *pShipSet = new xShipSet(0, 120);
	pShipSet->LoadRawDataInfo("scripts/table/shipinfo", bBinary);

	xShipPartSet *pShipItem = new xShipPartSet(0, 500);
	pShipItem->LoadRawDataInfo("scripts/table/shipiteminfo", bBinary);

	CHairRecordSet *pHair = new CHairRecordSet(0, 500);
	pHair->LoadRawDataInfo("scripts/table/hairs", bBinary);

	// txt
	MPTerrainSet *pTerrainSet = new MPTerrainSet(0, 100);		
	pTerrainSet->LoadRawDataInfo("scripts/table/TerrainInfo", bBinary);
		
	CEff_ParamSet* pEffSetp = new CEff_ParamSet(0, 100);
	pEffSetp->LoadRawDataInfo("scripts/table/MagicSingleinfo", bBinary);

	CGroup_ParamSet* pGroupSet = new CGroup_ParamSet(0, 10);
	pGroupSet->LoadRawDataInfo("scripts/table/MagicGroupInfo", bBinary);

	CItemRefineSet* pItemRefineSet = new CItemRefineSet(0, g_Config.m_nMaxItemType);
	pItemRefineSet->LoadRawDataInfo("scripts/table/ItemRefineInfo", bBinary);

	CItemRefineEffectSet* pItemRefineEffectSet = new CItemRefineEffectSet(0, 5000);
	pItemRefineEffectSet->LoadRawDataInfo("scripts/table/ItemRefineEffectInfo", bBinary);

	CStoneSet* pStoneSet = new CStoneSet(0, 100);
	pStoneSet->LoadRawDataInfo("scripts/table/StoneInfo", bBinary);

	MPResourceSet* pResourceSet = new MPResourceSet(0, g_Config.m_nMaxResourceNum);
	pResourceSet->LoadRawDataInfo("scripts/table/ResourceInfo", bBinary);

	CElfSkillSet* pElfSkillSet = new CElfSkillSet(0, 100);
	pElfSkillSet->LoadRawDataInfo("scripts/table/ElfSkillInfo", bBinary);
}

void CGameApp::ReleaseAllTable()
{
	CMusicSet* pMusicSet =  CMusicSet::I();
	SAFE_DELETE( pMusicSet );

	CPoseSet* pPoseSet = CPoseSet::I();
	SAFE_DELETE( pPoseSet );

	CChaCreateSet* pChaCreateSet =CChaCreateSet::I();
	SAFE_DELETE( pChaCreateSet );


    CSceneObjSet* pSceneObjSet = CSceneObjSet::I();
    SAFE_DELETE( pSceneObjSet );

    CEffectSet* pEffectSet = CEffectSet::I();
    SAFE_DELETE( pEffectSet );

	CShadeSet* pShadeSet = CShadeSet::I();
	SAFE_DELETE( pShadeSet );

	CSkillRecordSet* pSkillSet = CSkillRecordSet::I();
	SAFE_DELETE( pSkillSet );

    CMapSet* pMapSet = CMapSet::I();
    SAFE_DELETE( pMapSet);

	CEventSoundSet* pEventSoundSet = CEventSoundSet::I();
	SAFE_DELETE( pEventSoundSet );

    CEventRecordSet* pEvent = CEventRecordSet::I();
    SAFE_DELETE( pEvent );    

    CServerSet* pServe = CServerSet::I();
    SAFE_DELETE( pServe );    

    CNotifySet* pNotify = CNotifySet::I();
    SAFE_DELETE( pNotify );    

    CSkillStateRecordSet* pState = CSkillStateRecordSet::I();
    SAFE_DELETE( pState );    

	CChatIconSet* pIcon = CChatIconSet::I();
    SAFE_DELETE( pIcon );    

	CItemTypeSet* pItemType = CItemTypeSet::I();
    SAFE_DELETE( pItemType );    
	
	CItemPreSet *pItemPreSet = CItemPreSet::I();
	SAFE_DELETE( pItemPreSet );	

	CForgeRecordSet *pRecordSet = CForgeRecordSet::I();
    SAFE_DELETE( pRecordSet );  

	xShipSet *pShipSet = xShipSet::I();
	SAFE_DELETE( pShipSet );

	xShipPartSet *pShipItem = xShipPartSet::I();
	SAFE_DELETE( pShipItem );

	CHairRecordSet *pHair = CHairRecordSet::I();
	SAFE_DELETE( pHair );

	CItemRefineSet *pRefine = CItemRefineSet::I();
	SAFE_DELETE( pRefine );

	CStoneSet *pStone = CStoneSet::I();
	SAFE_DELETE( pStone );

	CElfSkillSet* pElfSkillSet = CElfSkillSet::I();
	SAFE_DELETE( pElfSkillSet );

	//MPResourceSet *pResourceSet = MPResourceSet::GetInstancePtr();
	//SAFE_DELETE(pResourceSet);
}

bool CGameApp::LoadRes4()
{
	int test = 0;
	char szBone[128];
	CCharacter* pChar = this->GetCurScene()->_GetFirstInvalidCha();
	for( int i = 1; i < 1200; i++ )
	{
		CChaRecord* pInfo = GetChaRecordInfo( i );
		if( pInfo && (pInfo->chCtrlType == 1 || pInfo->chCtrlType == 2) )
		{
			sprintf( szBone, "%04d.lab", pInfo->sModel );
			pChar->InitBone( szBone );
			test ++;
		}
	}
	return true;
}

void LoadResModelBuf(MPIResourceMgr* res_mgr)
{   
    MPIResBufMgr* buf_mgr = res_mgr->GetResBufMgr();
    MPIPathInfo* path_info = res_mgr->GetSysGraphics()->GetSystem()->GetPathInfo();
    const char* model_path = path_info->GetPath(PATH_TYPE_MODEL_SCENE);

    char path[LW_MAX_PATH];
    LW_HANDLE handle;

 
    CSceneObjInfo* obj_info;
    const DWORD model_num = 500;
    for(DWORD i = 0; i < model_num; i++)
    {
        if((obj_info = GetSceneObjInfo(i)) == 0)
            continue;

        if(stricmp(obj_info->szDataName, "sl-bd026-05.lmo") == 0)
        {
            int x = 0;
        }
        handle = obj_info->nID;
        sprintf(path, "%s%s", model_path, obj_info->szDataName);

        if(LW_FAILED(buf_mgr->RegisterModelObjInfo(handle, path)))
        {
            LG("init", "msgcannot find model file: %s", path);
            continue;
        }
    }    
}

void CGameApp::SetFPSInterval( DWORD v )		
{ 
	if(v>0) _pSteady->SetFPS(v);	
}

void CGameApp::_SceneError( const char* info, CGameScene * p )
{
    if( p )
    {
        MsgBox( g_oLangRec.GetString(78), p->GetInitParam()->strName.c_str(), info );
    }
    else
    {
        MsgBox( g_oLangRec.GetString(79), info );
    }
}

bool CGameApp::HasLogFile( const char* log_file, bool isOpen )
{
	// 检查是否有gui
	string file;
	GetLGDir( file );
	file += "/";
	file += log_file;
	file += ".log";
	FILE *fp = fopen( file.c_str(), "r" );
	if( fp )
	{
		fclose( fp );

		if( isOpen )
		{
			char buf[256] = { 0 };
			sprintf( buf, "notepad %s", file.c_str() );
			::WinExec( buf, SW_SHOWNORMAL );
		}
		return true;
	}
	return false;
}

void LimitCurrentProc()
{
    HANDLE hCurrentProcess = GetCurrentProcess();
    
    // Get the processor affinity mask for this process
    DWORD_PTR dwProcessAffinityMask = 0;
    DWORD_PTR dwSystemAffinityMask = 0;
    
    if( GetProcessAffinityMask( hCurrentProcess, &dwProcessAffinityMask, &dwSystemAffinityMask ) != 0 && dwProcessAffinityMask )
    {
        // Find the lowest processor that our process is allows to run against
        DWORD_PTR dwAffinityMask = ( dwProcessAffinityMask & ((~dwProcessAffinityMask) + 1 ) );

        // Set this as the processor that our thread must always run against
        // This must be a subset of the process affinity mask
        HANDLE hCurrentThread = GetCurrentThread();
        if( INVALID_HANDLE_VALUE != hCurrentThread )
        {
            SetThreadAffinityMask( hCurrentThread, dwAffinityMask );
            CloseHandle( hCurrentThread );
        }
    }

    CloseHandle( hCurrentProcess );
}

extern const char* ConsoleCallback( const char* );
void CGameApp::AutoTest()
{
	char szBuf[256] = { 0 };
	sprintf( szBuf, "testeffect 0 5000 1" );
	ConsoleCallback( szBuf );
	sprintf( szBuf, "testskilleffect 0 5000 1" );
	ConsoleCallback( szBuf );

	CGameScene* pScene = GetCurScene();
	if( !pScene ) return;

	D3DXVECTOR3 TestPos;
	int nTestX = 0;
	int nTestY = 0;
	if( CGameScene::GetMainCha() )
	{
		TestPos = CGameScene::GetMainCha()->GetPos();
		TestPos.x -= 3.0f;

		nTestX = CGameScene::GetMainCha()->GetCurX() - 300;
		nTestY = CGameScene::GetMainCha()->GetCurY();
	}

	for (int i(0); i<1; i++)
	{
		// 正在测试所有特效
		{
			AutoTestInfo( g_oLangRec.GetString(80) );
			int nCount = CEffectSet::I()->GetLastID() + 1;
			CEffectObj* pEffect = NULL;
			CMagicInfo* pInfo = NULL;
			for( int i=0; i<nCount; i++ )
			{
				pInfo = GetMagicInfo(i);
				if(!pInfo) continue;

				pEffect = pScene->GetFirstInvalidEffObj();
				if( !pEffect ) continue;

				if( !pEffect->Create( i ) )
				{
					AutoTestInfo( g_oLangRec.GetString(81), i );
					continue;
				}

				pEffect->Emission( -1, &TestPos, NULL );
				pEffect->SetValid( TRUE );

				AutoTestUpdate();

				pEffect->SetValid( FALSE );
			}
		}

	}
	// 自动创建所有角色,然后删除,用于检查角色,角色动作等合法性
	{
		AutoTestInfo( g_oLangRec.GetString(82) );

		int nCount = CChaRecordSet::I()->GetLastID() + 1;
		CCharacter *pCha = NULL;
		for( int i=0; i<nCount; i++ )
		{
			CChaRecord* pInfo = GetChaRecordInfo( i );
			if( !pInfo ) continue;

			pCha = pScene->AddCharacter( i );
			if( !pCha )
			{
				AutoTestInfo( g_oLangRec.GetString(83), i );
				continue;
			}

		    pCha->setPos( nTestX, nTestY );
			AutoTestUpdate();
			pCha->SetValid( FALSE );
		}
	}

	// 自动测试头发表begin
	{
		AutoTestInfo( g_oLangRec.GetString(84) );

		CCharacter* pHairCha[4] = { NULL };
		int nMax = 4;
		for( int i=0; i<nMax; i++ )
		{
			pHairCha[i] = pScene->AddCharacter( i + 1 );
			pHairCha[i]->setPos( nTestX, nTestY + i * 100 - 200 );
		}
		int nCount = CHairRecordSet::I()->GetLastID() + 1;
		CHairRecord* pHair = NULL;
		for( int i=0; i<nCount; i++ )
		{
			pHair = GetHairRecordInfo( i );
			if( !pHair ) continue;

			for( int j=0; j<nMax; j++ )
			{
				if( pHair->IsChaUse[j] )
				{
					if( !pHairCha[j]->ChangePart( enumEQUIP_HEAD, pHair->dwItemID ) )
						SysInfo( g_oLangRec.GetString(85), i, j+1, pHair->dwItemID );

					for( int k=0; k<pHair->GetFailItemNum(); k++ )
					{
						if( !pHairCha[j]->ChangePart( enumEQUIP_HEAD, pHair->dwFailItemID[k] ) )
							SysInfo( g_oLangRec.GetString(86), i, j+1, pHair->dwFailItemID[k] );
					}
					AutoTestUpdate();
				}
			}
		}
		for( int i=0; i<nMax; i++ )
		{
			pHairCha[i]->SetValid( FALSE );
		}
		// end
	}

	// 正在测试所有特效
	{
		AutoTestInfo( g_oLangRec.GetString(80) );
		int nCount = CEffectSet::I()->GetLastID() + 1;
		CEffectObj* pEffect = NULL;
		CMagicInfo* pInfo = NULL;
		for( int i=0; i<nCount; i++ )
		{
			pInfo = GetMagicInfo(i);
			if(!pInfo) continue;

			pEffect = pScene->GetFirstInvalidEffObj();
			if( !pEffect ) continue;

			if( !pEffect->Create( i ) )
			{
				AutoTestInfo( g_oLangRec.GetString(81), i );
				continue;
			}
			
			pEffect->Emission( -1, &TestPos, NULL );
			pEffect->SetValid( TRUE );

			AutoTestUpdate();

			pEffect->SetValid( FALSE );
		}
	}

	// 检查所有道具
	{
		int x = 0;
		int y = 0;
		string file;
		if( CGameScene::GetMainCha() )
		{
			x = CGameScene::GetMainCha()->GetCurX() - 300;
			y = CGameScene::GetMainCha()->GetCurY();
		}

		AutoTestInfo( g_oLangRec.GetString(87) );
		CItemRecord* pInfo = NULL;
		CSceneItem *pItem = NULL;
		CMagicInfo* pEffectInfo = NULL;
		int nEffectID = 0;
		int nCount = CItemRecordSet::I()->GetLastID() + 1;
		for( int i=0; i<nCount; i++ )
		{
			pInfo = GetItemRecordInfo( i );
			if( !pInfo ) continue;

			for( int j=0; j<5; j++ )
			{
				if( strcmp( pInfo->chModule[j], "0")!=0 )
				{
					file = "model/item/";
					file += pInfo->chModule[j];
					file += ".lgo";
					if( !IsExistFile( file.c_str() ) )
					{
						file = "model/character/";
						file += pInfo->chModule[j];
						file += ".lgo";
						if( !IsExistFile( file.c_str() ) )
						{
							if( j==0 )
							{
								AutoTestInfo( g_oLangRec.GetString(88), i, pInfo->szName, pInfo->chModule[0] );
							}
							else
							{
								AutoTestInfo( g_oLangRec.GetString(89), i, pInfo->szName, j, pInfo->chModule[j] );
							}
						}
					}
				}
			}

			if( strcmp( pInfo->szICON, "0" )!=0 )
			{
				if( !IsExistFile( pInfo->GetIconFile() ) )
					AutoTestInfo( g_oLangRec.GetString(90), i, pInfo->szName, pInfo->szICON );
			}

			nEffectID = pInfo->sDrap;
			if( nEffectID>0 )
			{
				pEffectInfo = GetMagicInfo(nEffectID);
				if( !pEffectInfo )
				{
					AutoTestInfo( g_oLangRec.GetString(91), i, pItem->GetItemInfo()->szName, nEffectID );
				}
			}
			for( int j=0; j<pInfo->sEffNum; j++ )
			{
				nEffectID = pInfo->sEffect[j][0];
				if( nEffectID>0 )
				{
					pEffectInfo = GetMagicInfo(nEffectID);
					if( !pEffectInfo )
					{
						AutoTestInfo( g_oLangRec.GetString(92), i, pItem->GetItemInfo()->szName, nEffectID );
					}
				}
			}
			nEffectID = pInfo->sItemEffect[0];
			if( nEffectID>0 )
			{
				pEffectInfo = GetMagicInfo(nEffectID);
				if( !pEffectInfo )
				{
					AutoTestInfo( g_oLangRec.GetString(93), i, pItem->GetItemInfo()->szName, nEffectID );
				}
			}
			nEffectID = pInfo->sAreaEffect[0];
			if( nEffectID>0 )
			{
				pEffectInfo = GetMagicInfo(nEffectID);
				if( !pEffectInfo )
				{
					AutoTestInfo( g_oLangRec.GetString(94), i, pItem->GetItemInfo()->szName, nEffectID );
				}
			}

		    pItem = pScene->AddSceneItem( i, 0 );
			if( !pItem )
			{
				AutoTestInfo( g_oLangRec.GetString(95), i, pInfo->szName );
				continue;
			}

		    pItem->setPos( nTestX, nTestY );
			AutoTestUpdate();

			pItem->SetValid( FALSE );
		}
	}

	// 正在测试精炼索引表...
	{
		AutoTestInfo( g_oLangRec.GetString(96) );
		CItemRefineInfo* pRefine = NULL;
		int nCount = CItemRefineSet::I()->GetLastID() + 1;
		int nEffectID = 0;
		CItemRefineEffectInfo* pEffectInfo = NULL;
		for( int i=0; i<nCount; i++ )
		{
			pRefine = GetItemRefineInfo( i );
			if( !pRefine ) continue;

			for( int k=0; k<ITEM_REFINE_NUM; k++ )
			{
				nEffectID = pRefine->Value[k];
				if( nEffectID > 0 )
				{
					pEffectInfo = GetItemRefineEffectInfo( nEffectID );
					if( !pEffectInfo )
					{
						AutoTestInfo( g_oLangRec.GetString(97), i, pRefine->szDataName, nEffectID );
					}
				}
			}
		}
	}

	// 正在测试精炼效果表...
	{
		AutoTestInfo( g_oLangRec.GetString(98) );
		int nCount = CItemRefineEffectSet::I()->GetLastID() + 1;
		CItemRefineEffectInfo* pInfo = NULL;
		CMagicInfo* pEffectInfo = NULL;
		int nEffectNum;
		int nEffectID;
		for( int i=0; i<nCount; i++ )
		{
			pInfo = GetItemRefineEffectInfo( i );
			if( !pInfo ) continue;
			
			// 4个角色
			for( int k=0; k<4; k++ )
			{
				nEffectNum = pInfo->GetEffectNum( k );
				for( int j=0; j<nEffectNum; j++ )
				{
					if( pInfo->sEffectID[k][j]<=0 ) continue;

					for( int level = 0; level<4; level++ )
					{
						nEffectID = pInfo->sEffectID[k][j] * 10 + level;
						pEffectInfo = GetMagicInfo(nEffectID);
						if( !pEffectInfo )
						{
							AutoTestInfo( g_oLangRec.GetString(99), i, pInfo->szDataName, nEffectID );
						}
					}
				}
			}
		}
	}

	// 检测是否有道具表错误
	{
		AutoTestInfo( g_oLangRec.GetString(100) );

		g_pGameApp->HasLogFile( "iteminfoerror" );
	}

	g_pGameApp->HasLogFile( "autotest" );
}

void CGameApp::LG_Config(const LGInfo& info)
{
	LGInfo in = info;
	if( g_Config.m_bEditor ) 
	{
		in.bMsgBox=true;
		in.bEnableAll = true;
	}

	MPGameApp::LG_Config( in );

    if(in.bCloseAll)
    {
        ::LG_CloseAll();
    }
    ::LG_SetEraseMode(in.bEraseMode);
    ::LG_SetDir(in.dir);
    ::LG_EnableAll(in.bEnableAll);
    ::LG_EnableMsgBox(in.bMsgBox);
}

