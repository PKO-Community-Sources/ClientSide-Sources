#include "stdafx.h"
#include "LoginScene.h"

#include "GameApp.h"
#include "Character.h"
#include "SceneObj.h"
#include "UiFormMgr.h"
#include "UiTextButton.h"
#include "CharacterAction.h"
#include "SceneItem.h"
#include "ItemRecord.h"
#include "PacketCmd.h"
#include "GameConfig.h"

#include "Character.h"
#include "caLua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "UIRender.h"
#include "UIEdit.h"
#include "UILabel.h"
#include "uiformmgr.h"
#include "uitextbutton.h"
#include "uilabel.h"
#include "uiprogressbar.h"
#include "uiscroll.h"
#include "uilist.h"
#include "uicombo.h"
#include "uiimage.h"
#include "UICheckBox.h"
#include "uiimeinput.h"
#include "uigrid.h"
#include "uilistview.h"
#include "uipage.h"
#include "uitreeview.h"
#include "uiimage.h"
#include "UILabel.h"
#include "RenderStateMgr.h"
#include "cameractrl.h"
#include "UIListView.h"

#include "UIMemo.h"
#include "caLua.h"

#include "Connection.h"
#include "ServerSet.h"
#include "GameAppMsg.h"


#include "UI3DCompent.h"
#include "UIForm.h"
#include "UITemplete.h"
#include "commfunc.h"
#include "uiboxform.h"
#include "bill.h"

#include "uisystemform.h"

#include <shellapi.h>

//#define USE_STATUS		// 此协议与大波的网络有冲突，关闭 lh by 2006-3-22

#ifdef USE_STATUS
#include "UdpClient.h"

using namespace client_udp;
#endif

#pragma comment( lib, "shell32.lib" )

#include "xmlwriter.h"

//#ifdef KOP_TOM.
TOM_SERVER	g_TomServer;
//#endif
Cooperate   g_cooperate;    //  add by jampe


bool registerLogin = false;

char autoLoginName[32];
char autoLoginPassword[32];
bool useAutoLogin;

bool useModelMode = false;
bool modelMode = false;
char modelLook[8192];


int CLoginScene::nSelectChaType  = 0; // 保存选中角色的信息
int CLoginScene::nSelectChaPart[enumEQUIP_PART_NUM] = { 0 };

CForm*			CLoginScene::frmServer   = NULL;
CForm*			CLoginScene::frmRegion   = NULL;
CForm*			CLoginScene::frmAccount  = NULL;
CForm*          CLoginScene::frmLOGO     = NULL;
CForm*          CLoginScene::frmAnnounce = NULL;
CForm*          CLoginScene::frmKeyboard = NULL;	// add by Philip.Wu  软键盘界面  2006-06-05
CForm*          CLoginScene::frmRegister = NULL;	// add by Philip.Wu  软键盘界面  2006-06-05
CForm*          CLoginScene::frmPathLogo = NULL;

CCheckBox*      CLoginScene::chkID       = NULL;

CList*			CLoginScene::lstRegion[NUM_REGIN_LIST];
CListView*		CLoginScene::lstServer[NUM_SERVR_LIST];

CEdit*			CLoginScene::edtID       = NULL;
CEdit*          CLoginScene::edtPassword = NULL;
CEdit*			CLoginScene::edtFocus    = NULL;	// add by Philip.Wu  光标激活的编辑框  2006-06-06
CCheckBox*      CLoginScene::chkShift    = NULL;	// add by Philip.Wu  软键盘上的 Shift  2006-06-09

CImage*         CLoginScene::imgLogo1    = NULL;
CImage*         CLoginScene::imgLogo2    = NULL;
CCharacter*         CLoginScene::modelCha    = 0;


//static			CCharacterBuilder* __character_scene = 0;
static			CCharacter*  _pCntCha[3] = { 0 };
static			CCharacter*  pPxCha[2] = { 0 };  //登陆场景的3个螃蟹
static			CCharacter * _pSelectCha = 0;

static	int		iSelectIndex = 0;				//鼠标点中的3D控件的序号
static	int		iRotateCha      = 360;			// 在创建角色的时候，点击左右按钮旋转
static	int		iMiniRotateCha  = 360;

static CLoginScene* g_login_scene = 0;

static void	_GoBack(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	CLoginScene* pLogin = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if( pLogin )
	{
		pLogin->ShowLoginForm();
	}
}

#define MAX_SERVER_NUM	8

extern char* GetFlashDir( int UpLayers );

CLoginScene::CLoginScene(stSceneInitParam& param):
	CGameScene(param),
	_eState(enumInit), 
    m_bPasswordError(false), 
	m_sPassport("nobill"), 
	m_sUsername(""), 
	m_sPassword(""),
	IsLoad(false)
{
	//LG( "scene memory", "CLoginScene Create\n" );
	//_loadtex_flag = 9; _loadmesh_flag = 9;
}

CLoginScene::~CLoginScene()
{
	LG( "scene memory", "CLoginScene Destroy\n" );

	for (int i =0 ; i<3 ; i++)
		_pCntCha[i] = NULL ;
	_pSelectCha = NULL;
}

bool CLoginScene::_Init()
{
	
    g_login_scene = this;
    _IsUseSound = false;
    _eState = enumInit;

    /*{ // save loading res mt flag
        lwIByteSet* res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
        _loadtex_flag = res_bs->GetValue(OPT_RESMGR_LOADTEXTURE_MT);
        _loadmesh_flag = res_bs->GetValue(OPT_RESMGR_LOADMESH_MT);
        res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
        res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0);
    }*/

	if ( !CGameScene::_Init() )
	{
		return false;
	}
    { // save loading res mt flag, and resume these flags in _Clear() before this scene destoried.
        lwIByteSet* res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
        _loadtex_flag = res_bs->GetValue(OPT_RESMGR_LOADTEXTURE_MT);
        _loadmesh_flag = res_bs->GetValue(OPT_RESMGR_LOADMESH_MT);
        res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
        res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0);
    }

	//SetFocus( g_pGameApp->GetHWND()) ;
	//SetFocus( g_pGameApp->GetHWND()) ;

    _bEnableCamDrag    = TRUE;
    MPTimer tInit; 
    tInit.Begin();

	//static bool IsLoad = false;
	static CGuiPic LoginPic;
	if( !IsLoad )
	{
		IsLoad = true;
		pObj = AddSceneObj(435);

		if(pObj)
		{
			pObj->SetCullingFlag(0);
			// position of the scene
			pObj->setPos(0,0);
			pObj->setYaw(180);

			DWORD num = pObj->GetPrimitiveNum();
			for(DWORD i = 0; i < num; i++)
			{
				pObj->GetPrimitive(i)->SetState(STATE_TRANSPARENT, 0);
				pObj->GetPrimitive(i)->SetState(STATE_UPDATETRANSPSTATE, 0);
			}
			pObj->PlayDefaultAnimation();
		}
	}

	g_Render.SetClip(g_Config.fnear, g_Config.ffar);

    CCameraCtrl *pCam = g_pGameApp->GetMainCam();
    if(pCam)
    {
        g_pGameApp->EnableCameraFollow(TRUE);
        pCam->m_EyePos.x = 103.749f;
        pCam->m_EyePos.y = 150.923f;
        pCam->m_EyePos.z = 320.982f;

        pCam->m_RefPos.x = 0.034f;
        pCam->m_RefPos.y = -294.137f;
        pCam->m_RefPos.z = 0.868f;

    }
    g_Render.SetWorldViewFOV(Angle2Radian(70.0f));
    g_Render.SetWorldViewAspect(1.33f);
    g_Render.SetClip(1.0f, 2000.0f);

    g_Render.LookAt(pCam->m_EyePos, pCam->m_RefPos);
    g_Render.SetCurrentView(MPRender::VIEW_WORLD);
    MPIDeviceObject* dev_obj = g_Render.GetInterfaceMgr()->dev_obj;

    //SetupVertexFog(dev_obj, 0, 0, D3DCOLOR_XRGB(28, 221, 246), D3DFOG_EXP2, 1, 0.0006f);
	//SetupVertexFog(dev_obj, 0, 0, D3DCOLOR_XRGB(86, 209, 246), D3DFOG_EXP2, 1, 0.0006f);

    g_Render.SetRenderStateForced(D3DRS_LIGHTING, 0);
    g_Render.SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	/////////////////////////////////////

	m_sUsername = "player";
	m_sPassword = "";

	if (!_InitUI())
	{
		LG("login_ini", g_oLangRec.GetString(168));

		return false;
	}

#ifdef USE_STATUS
	static bool isInitStatus = false;
	if (!isInitStatus)
	{
		isInitStatus = true;

		GetRegionMgr()->Init( g_pGameApp->GetHWND(), APP_STATUS );
		int nCount  = CServerSet::I()->GetLastID() + 1;
		for (int i(0); i<nCount; i++)
		{
			if( CServerGroupInfo* pGate = GetServerGroupInfo( i ) )
			{
				for( int k(0); k<pGate->cValidGateCnt; k++ )
				{
					const char * pszRegion = pGate->szRegion;
					const char * pszGroup = pGate->szDataName;
					const char * pszGate = pGate->szGateIP[k];
					if( GetRegionMgr()->Add( pszRegion, pszGroup, pszGate ) )
					{
						LG( "all_gate", "%s, %s, %s\n", pszRegion, pszGroup, pszGate );
					}
				}
			}
		}
	}
#endif

	CFormMgr::s_Mgr.SetEnabled(TRUE);    
   	_pMainCha = NULL;

	// 删除自动更新程序的复本
	char szUpdateFileName[] = "_Update.exe";
	SetFileAttributes(szUpdateFileName, FILE_ATTRIBUTE_NORMAL);
  	DeleteFile(szUpdateFileName);

	PlayWhalePose();
	ShowLoginForm();

	static bool isFirst = true;
	if (g_TomServer.bEnable && isFirst )
	{
		isFirst = false;
		m_sUsername = g_TomServer.szUser;
		m_sPassword = g_TomServer.szPassword;
		m_sPassport = g_TomServer.szPassport;
		_Connect();
		return true;
	}
	
	if(useModelMode){
		CForm* frmModel = CFormMgr::s_Mgr.Find( "frmModel" );
		frmAccount->Hide();
		frmModel->Show();
		C3DCompent* ui3dCha;
		if(!modelMode){
			ui3dCha = (C3DCompent*)frmModel->Find( "ui3dCha" );
			frmModel->Find( "iconBack" )->SetIsShow(false);
		}else{
			ui3dCha = (C3DCompent*)frmModel->Find( "ui3dIcon" );
			frmModel->Find( "cardBack" )->SetIsShow(false);
		}
		if( ui3dCha ){
			stNetChangeChaPart pLook;
			memset(&pLook, 0, sizeof(pLook));
			string strLook(modelLook);			
			if(Strin2LookData(&pLook, strLook)){
				modelCha = AddCharacter(pLook.sTypeID);
				modelCha->SetIsForUI( 1 );
				modelCha->UpdataFace( pLook );
			}
            ui3dCha->SetRenderEvent( __cha_render_event );
			
			
        }
		return true;
	}
	
	if (useAutoLogin){
		//prevent issues on logout/change char.
		useAutoLogin = false;
		m_sUsername = autoLoginName;
		m_sPassword = autoLoginPassword;
		_Connect();
	}


	return true;
}

void CLoginScene::__cha_render_event(C3DCompent *pSender, int x, int y){
	g_Render.GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	g_Render.GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    g_Render.LookAt( D3DXVECTOR3( 11.0f, 36.0f, 10.0f ), D3DXVECTOR3( 8.70f, 12.0f, 8.0f ), MPRender::VIEW_3DUI );
    y +=100;
    MPMatrix44 old_mat = *modelCha->GetMatrix();
    
	if(!modelMode){
		modelCha->SetUIYaw(160);
		modelCha->SetUIScaleDis(5.0f );
	}else{
		modelCha->SetUIYaw(140);
		modelCha->SetUIScaleDis(1.7f );
		int chatype = modelCha->GetPart().sTypeID;
		if(chatype==1){
			y+=140;
		}else if(chatype==2){
			y+=240;
		}else if(chatype==3){
			y+=140;
		}else if(chatype==4){
			y+=20;
		}
	}
    modelCha->RenderForUI(x, y);
    modelCha->SetMatrix(&old_mat);
    g_Render.SetTransformView(&g_Render.GetWorldViewMatrix());
	
	
	char szMD5[33] = {0};
	md5string(modelLook, szMD5);
		
	char file[32];
	if(!modelMode){
		sprintf(file,"./player/%s.png",szMD5);
	}else{
		sprintf(file,"./icon/%s.png",szMD5);
	}
	g_Render.CaptureScreen(file);
	g_ChaExitOnTime.ExitApp();
}

void CLoginScene::PlayWhalePose()
{
}

bool CLoginScene::_Clear()
{
    if ( !CGameScene::_Clear() )
    {
        return false;
    }

//    g_Render.SetRenderState(D3DRS_FOGENABLE, 0);
    { // reset loading res mt flag
        if(_loadtex_flag != 9 && _loadmesh_flag != 9)
        {
            lwIByteSet* res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
            res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, _loadtex_flag);
            res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, _loadmesh_flag);
        }
    }

    g_Render.SetClip(1.F, 1000.0f);

    return 1;
}

void CLoginScene::ShowLoginForm()
{
	chkID->SetIsChecked( m_bSaveAccount );
	edtID->SetCaption( m_sSaveAccount.c_str());
	edtPassword->SetCaption("");
	frmAccount->Show();

	// add by Philip.Wu  2006-07-03  显示输入框时同时显示软键盘
	frmKeyboard->SetIsShow(false);
	imgLogo1->SetIsShow(true);
	imgLogo2->SetIsShow(true);

	if (m_sSaveAccount == "")
	{
		edtID->SetActive(edtID);
	}
	else
	{
		edtPassword->SetActive(edtPassword);
	}
}

void CLoginScene::_FrameMove(DWORD dwTimeParam)
{
    //CGameScene::_FrameMove(dwTimeParam);
	int x = g_pGameApp->GetMouseX();
	int y = g_pGameApp->GetMouseY();
	GetRender().ScreenConvert( x, y );

/*
	if (frmServer->GetIsShow())
	{
#ifdef USE_STATUS
		CLoginScene* pkScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
		if (!pkScene) return;

		GetRegionMgr()->FrameMove( dwTimeParam );

		int iRegionIndex = pkScene->GetCurSelRegionIndex();
		const char* szServerGroupName;
		CGraph* pGraph(0);
		int iServIconIndex(0);

		static int nFlash = -5;
		nFlash++;
		bool IsFlash = false;
		if( nFlash>=5 ) 
		{
			nFlash=-5;
		}
		for( int i = 0; i< GetCurServerGroupCnt(iRegionIndex); i++ )
		{
			szServerGroupName = GetCurServerGroupName(iRegionIndex, i);
			iServIconIndex = pkScene->GetServIconIndex( GetRegionMgr()->GetGroupNum(szServerGroupName) );

			if (i%2 == 0)
				pGraph = dynamic_cast<CGraph*>(lvServer0->GetItemObj(i/2, IMAGE_INDEX));
			else
				pGraph = dynamic_cast<CGraph*>(lvServer1->GetItemObj(i/2, IMAGE_INDEX));
			if ( pGraph)
				pGraph->GetImage()->SetFrame(iServIconIndex);

			if( nFlash>=0 )
				pGraph->GetImage()->SetAlpha( 150 );
			else
				pGraph->GetImage()->SetAlpha( 255 );
		}
#endif
	}

	if (frmRegion->GetIsShow())
	{
		for( int index = 0; index < NUM_REGIN_LIST; index ++ )
		{
			if( !lstRegion[index]->InRect(x, y) )
				lstRegion[index]->GetItems()->GetSelect()->SetNoSelect();
		}
	}
*/
	if( _eState==enumConnect )
    {
        switch( g_NetIF->GetConnStat() )
        {
		case Connection::CNST_CONNECTING:
			return ;
        case Connection::CNST_INVALID:
        case Connection::CNST_FAILURE: 
            {
				if (g_TomServer.bEnable)
				{
					MessageBox( 0, g_oLangRec.GetString(169), "Info", 0 );
					g_pGameApp->SetIsRun( false );
					return;
				}

                // 返回到选择服务器场景
	             _eState = enumInit;
                CGameApp::Waiting(false);

				ShowKeyboard(false);
				//frmRegion->SetIsShow(false);
				frmAccount->SetIsShow(false);
				g_stUIBox.ShowMsgBox( _GoBack, g_oLangRec.GetString(169) );
            }
            return;
        case Connection::CNST_CONNECTED:
            CGameApp::Waiting(false);

			//连接成功,发送登陆消息
			_Login();
			return;
		case Connection::CNST_TIMEOUT:
            _eState = enumInit;
			g_pGameApp->SendMessage(APP_NET_DISCONNECT,1000);
			return;
        }
        return;
    }
}

#include "GlobalVar.h"

void CLoginScene::_Render()
{
/*	static bool IsLoad = false;
	static CGuiPic LoginPic;
	if( !IsLoad )
	{
		LoginPic.LoadImage( "texture/ui/new_login.jpg", 1198, 768, 0, 0, 0, 1.F, 1.F );
		IsLoad = true;
	}
	LoginPic.SetScale( 0, GetRender().GetScreenWidth(), GetRender().GetScreenHeight() );
	LoginPic.Render( 0, 0 );
*/
	//CGuiFont::s_Font.TipRender(VERSION_NUM, 3*(strlen(VERSION_NUM)+1)+20, GetRender().GetScreenHeight()-30 );

    MPTimer mpt;
    mpt.Begin();
    //CGameScene::_Render();
    MPIDeviceObject* dev_obj = g_Render.GetInterfaceMgr()->dev_obj;
    RenderStateMgr* rsm = g_pGameApp->GetRenderStateMgr();
	DWORD dwOldState;

	rsm->BeginScene();

    D3DLIGHTX env_light;
    D3DLIGHTX env_light_old;
    memset(&env_light, 0, sizeof(env_light));
    env_light.Type = D3DLIGHT_DIRECTIONAL;

    env_light.Direction.x = -1.0f;
    env_light.Direction.y = -1.0f;
    env_light.Direction.z = -0.5f;
    D3DXVec3Normalize((D3DXVECTOR3*)&env_light.Direction, (D3DXVECTOR3*)&env_light.Direction);

    MPDwordByte4 c;
    c.b[3] = 0xff;
    c.b[2] = 185;
    c.b[1] = 36;
    c.b[0] = 54;
    env_light.Diffuse.r = (float)(c.b[2] / 255.0f);
    env_light.Diffuse.g = (float)(c.b[1] / 255.0f);
    env_light.Diffuse.b = (float)(c.b[0] / 255.0f);

    g_Render.GetLight(0, &env_light_old);
    g_Render.SetLight(0, &env_light);
	dev_obj->SetRenderState(D3DRS_LIGHTING, dwOldState);
    g_Render.SetLight(0, &env_light_old);

    rsm->BeginSceneObject();
	if (pObj)
		pObj->FrameMove(0);

	// shade islands
    dev_obj->GetRenderState(D3DRS_LIGHTING, &dwOldState);
    dev_obj->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (pObj)
		pObj->Render();
    rsm->EndSceneObject();

    rsm->BeginTranspObject();
    lwUpdateSceneTransparentObject();
    rsm->EndTranspObject();
    rsm->EndScene();

    //dev_obj->SetRenderState(D3DRS_FOGENABLE, FALSE); // sharpen colour

/*
#ifdef USE_STATUS
	if (frmServer->GetIsShow())
	{
		CGraph* pGraph = NULL;
		CItemRow* pRow = NULL;
		if( pRow = lvServer0->GetList()->GetItems()->GetSelect()->GetItem() )
		{
			pGraph = dynamic_cast<CGraph*>( pRow->GetBegin() );
		}
		else if( pRow = lvServer1->GetList()->GetItems()->GetSelect()->GetItem() )
		{
			pGraph = dynamic_cast<CGraph*>( pRow->GetBegin() );
		}		
		if( pGraph )
		{
			pGraph->GetImage()->SetAlpha( 255 );
			int n = pGraph->GetImage()->GetFrame();
			char szBuf[256] = { 0 };
			switch( n )
			{
			case 0: 
				sprintf( szBuf, g_oLangRec.GetString(170), pRow->GetIndex(1)->GetString() );
				break;
			case 1:
				sprintf( szBuf, g_oLangRec.GetString(171), pRow->GetIndex(1)->GetString() );
				break;
			case 2: 
				sprintf( szBuf, g_oLangRec.GetString(172), pRow->GetIndex(1)->GetString() );
				break;
			case 3:
				sprintf( szBuf, g_oLangRec.GetString(173), pRow->GetIndex(1)->GetString() );
				break;
			}
			
			g_pGameApp->ShowHint( g_pGameApp->GetMouseX(), g_pGameApp->GetMouseY(), szBuf, COLOR_GREEN );
		}
	}
#endif */
}


void CLoginScene::LoadingCall()
{
	if(g_dwCurMusicID != 1)	g_pGameApp->PlayMusic( 1 );
}

//-----------------
// 界面相关Routines
//-----------------
void CLoginScene::CallbackUIEvent_LoginScene( CCompent *pSender, int state, int x, int y, DWORD key)
{
	string strName = (const char*)pSender->GetName();
	CLoginScene* pScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if( !pScene )
		return;

	if ( stricmp("frmAnnounce", pSender->GetForm()->GetName())==0 )
	{
		if ( strName == "btnYes")
		{
			//关闭通告表单
			pSender->GetForm()->SetIsShow(false);
/*
			//更新游戏区域列表并显示游戏区域选择表单
			pScene->InitRegionList();
			frmRegion->SetIsShow(true);
			//frmServer->Show();
		}
	}
	else if ( stricmp ("frmServer" , pSender->GetForm()->GetName() ) == 0 )
	{
		if(strName=="btnYes")
		{         
            pSender->GetForm()->Hide();						
            pScene->LoginFlow();//选定了服务器	  
		}
		else if(strName=="btnNo")
		{
			g_pGameApp->SetIsRun( false );
			return;
*/
		}
	}
	else if ( stricmp ("frmAccount" , pSender->GetForm()->GetName() ) == 0 )
	{
		if(strName=="btnYes")
		{	
			pScene->LoginFlow();
		}
		else if(strName=="btnNo")
		{
			if( g_NetIF->IsConnected() )
			{
				CS_Disconnect(DS_DISCONN);				
			}
			pSender->GetForm()->Hide();
			return;
		}
	}
}

void CLoginScene::_evtServerFrm(CCompent *pSender, int state, int x, int y, DWORD key)
{
	CLoginScene* pkScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if (!pkScene) return;

	string strName = (const char*)pSender->GetName();
	if (strName == "btnNo")
	{
		pSender->GetForm()->SetIsShow(false);

		frmRegion->SetIsShow(true);
	}
}

void CLoginScene::_evtRegionFrm(CCompent *pSender, int state, int x, int y, DWORD key)
{
	CLoginScene* pkScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if (!pkScene) return;

	string strName = (const char*)pSender->GetName();
	if (strName == "btnNo")
	{
		//关闭服务器列表表单,显示通告表单
		pSender->GetForm()->SetIsShow(false);

		g_pGameApp->SetIsRun( false );
		return;
	}

}

void CLoginScene::_evtLoginFrm(CCompent *pSender, int state, int x, int y, DWORD key)
{
	CLoginScene* pkScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if (!pkScene) return;

	string strName = (const char*)pSender->GetName();
	if(strName=="btnYes")
	{
		// 先关闭软键盘
		if(frmKeyboard->GetIsShow())
		{
			frmKeyboard->SetIsShow(false);

			imgLogo1->SetIsShow(true);
			imgLogo2->SetIsShow(true);
		}

		// 连接服务器
		registerLogin = false;
		pkScene->LoginFlow();
	}
	else if(strName=="btnNo")
	{
		// 先关闭软键盘
		if(frmKeyboard->GetIsShow())
		{
			frmKeyboard->SetIsShow(false);

			imgLogo1->SetIsShow(true);
			imgLogo2->SetIsShow(true);
		}

		//关闭连接
		if( g_NetIF->IsConnected() )
		{
			CS_Disconnect(DS_DISCONN);				
		}
		//关闭登陆表单,显示上一次显示的服务器列表表单
		pSender->GetForm()->SetIsShow(false);
		//pkScene->InitServerList(pkScene->GetCurSelRegionIndex());
		//frmServer->SetIsShow(true);
		g_pGameApp->SetIsRun( false );

		return;
	}
	else if(strName == "btnKeyboard" || strName == "btnRegNo")
	{
		bool bShow = frmRegister->GetIsShow();
		frmRegister->SetIsShow(!frmRegister->GetIsShow());
		CEdit* edtRegID = dynamic_cast<CEdit*>(frmRegister->Find( "edtRegID" ));
		CEdit* edtRegPassword = dynamic_cast<CEdit*>(frmRegister->Find( "edtRegPassword" ));
		CEdit* edtRegPassword2 = dynamic_cast<CEdit*>(frmRegister->Find( "edtRegPassword2" ));
		CEdit* edtRegEmail = dynamic_cast<CEdit*>(frmRegister->Find( "edtRegEmail" ));
		edtRegID->SetCaption("");
		edtRegPassword->SetCaption("");
		edtRegPassword2->SetCaption("");
		edtRegEmail->SetCaption("");
	}else if(strName == "btnRegYes"){
		registerLogin = true;
		pkScene->LoginFlow();
	}
}

//-----------------------------------------------------------------------------
void CLoginScene::_evtServerFrmBeforeShow(CForm* pForm, bool& IsShow)
{
#ifdef USE_STATUS
	CLoginScene* pkScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if (!pkScene) return;

	GetRegionMgr()->EnterRegion(GetCurRegionName(pkScene->GetCurSelRegionIndex()));
#endif

}
//-----------------------------------------------------------------------------
void CLoginScene::_evtServerFrmOnClose(CForm* pForm, bool& IsClose)
{
#ifdef USE_STATUS
	CLoginScene* pkScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if (!pkScene) return;

	pkScene->m_szSelServIp = NULL;
	CRegionInfo* pRegion = GetRegionMgr()->GetActiovRegion();
	if( pRegion )
	{
		CGroupInfo* pGroup = pRegion ->Find( GetCurServerGroupName(pkScene->m_iCurSelRegionIndex,pkScene->m_iCurSelServerIndex) );
		if (pGroup)
		{
			CGateInfo* pGate = pGroup->GetMinGate();
			if (pGate)
			{
				pkScene->m_szSelServIp = pGate->szName;
			}
		}
	}
	if (!pkScene->m_szSelServIp)
		pkScene->m_szSelServIp = SelectGroupIP(pkScene->m_iCurSelRegionIndex, pkScene->m_iCurSelServerIndex);

	GetRegionMgr()->ExitRegion();
#endif
}

void CLoginScene::_evtServerLDBDown(CGuiData *pSender, int x, int y, DWORD key)
{
	CLoginScene* pkScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if (!pkScene) return;

	CList* pkServerList = dynamic_cast<CList*>(pSender);
	if (!pkServerList) return;

	if (pkServerList == lstServer[0]->GetList())
	{
		pkScene->SetCurSelServerIndex(pkServerList->GetItems()->GetSelect()->GetIndex() * 2);
	}
	else if (pkServerList == lstServer[1]->GetList())
	{
		pkScene->SetCurSelServerIndex(pkServerList->GetItems()->GetSelect()->GetIndex() * 2 + 1);
	}

	if (g_cooperate.code)
	{
		pkScene->LoginFlow();
	}

	if (key & Mouse_LDown)
	{
		pSender->GetForm()->Hide();
		pkScene->ShowLoginForm();
	}
}

void CLoginScene::_evtRegionLDBDown(CGuiData *pSender, int x, int y, DWORD key)
{
	CLoginScene* pkScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if (!pkScene) return;

	CList* pkRegionList = dynamic_cast<CList*>(pSender);
	if (!pkRegionList) return;

	if (pkRegionList == lstRegion[0])
	{
		pkScene->SetCurSelRegionIndex(pkRegionList->GetItems()->GetSelect()->GetIndex() * 2);
	}
	else if (pkRegionList == lstRegion[1])
	{
		pkScene->SetCurSelRegionIndex(pkRegionList->GetItems()->GetSelect()->GetIndex() * 2 + 1);
	}

	if (key & Mouse_LDown)
	{
		pSender->GetForm()->SetIsShow(false);
		pkScene->InitServerList(pkScene->GetCurSelRegionIndex());
		frmServer->SetIsShow(true);
	}
}

void  CLoginScene::_evtEnter(CGuiData *pSender)			// added by billy 
{
    CLoginScene* pScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
    if( !pScene )     return;
    pScene->LoginFlow();
}

void CLoginScene::InitRegionList()
{
	lstRegion[0]->GetItems()->Clear();
	lstRegion[1]->GetItems()->Clear();

	CServerSet* server_set = CServerSet::I();
	for (int i = 0; i < server_set->m_nRegionCnt; i++)
	{
		lstRegion[i % 2]->Add(server_set->m_szRegionName[i]);
	}

	SetCurSelRegionIndex(0);
	CListItems *items = lstRegion[0]->GetItems();
	if (!items)
		return;
	items->Select(GetCurSelRegionIndex());
}

void CLoginScene::InitServerList(int nRegionNo)
{
	lstServer[0]->GetList()->GetItems()->Clear();
	lstServer[1]->GetList()->GetItems()->Clear();

	for (int i = 0; i < GetCurServerGroupCnt(nRegionNo); i++)
	{
		CItemRow* item_row = lstServer[i % 2]->AddItemRow();
		if (item_row)
		{
			CItem* v7 = new CItem(GetCurServerGroupName(nRegionNo, i), COLOR_BLACK);
			v7->SetColor(lstServer[1]->GetList()->GetFontColor());

			CGraph* v10 = new CGraph(*imgServerIcons->GetImage());

			item_row->SetIndex(0, v10);
			item_row->SetIndex(1, v7);
		}
	}

	SetCurSelServerIndex(0);
	CListItems *items = lstServer[0]->GetList()->GetItems();
	if (!items)
		return;
	items->Select(GetCurSelServerIndex());
	lstServer[0]->Refresh();
	lstServer[1]->Refresh();
}

BOOL CLoginScene::_InitUI()
{/*
	frmServer = CFormMgr::s_Mgr.Find( "frmServer" );	
	if(!frmServer) return false;
	frmServer->evtEntrustMouseEvent = _evtServerFrm;
#ifdef USE_STATUS
	frmServer->evtBeforeShow = _evtServerFrmBeforeShow;
	frmServer->evtClose = _evtServerFrmOnClose;
#endif
	lstServer[0] = dynamic_cast<CListView*>(frmServer->Find("lvServer0"));
	if (!lstServer[0]) return false;
	lstServer[0]->GetList()->evtListMouseDown = _evtServerLDBDown;

	lstServer[1] = dynamic_cast<CListView*>(frmServer->Find("lvServer1"));
	if (!lstServer[1]) return false;
	lstServer[1]->GetList()->evtListMouseDown = _evtServerLDBDown;

	imgServerIcons = dynamic_cast<CImage*>(frmServer->Find("imgServerIcon0"));
	if (!imgServerIcons) return false;
	imgServerIcons->SetIsShow(false);

	frmRegion = CFormMgr::s_Mgr.Find("frmArea");
	if (!frmRegion) return false;
	frmRegion->evtEntrustMouseEvent = _evtRegionFrm;

	lstRegion[0] = dynamic_cast<CList*>(frmRegion->Find("lstRegion0"));
	if (!lstRegion[0]) return false;
	lstRegion[0]->evtListMouseDown = _evtRegionLDBDown;

	lstRegion[1] = dynamic_cast<CList*>(frmRegion->Find("lstRegion1"));
	if (!lstRegion[1]) return false;
	lstRegion[1]->evtListMouseDown = _evtRegionLDBDown;

	InitRegionList();
	if (!g_TomServer.bEnable)
		frmRegion->SetIsShow(true);
*/
	frmPathLogo = CFormMgr::s_Mgr.Find("frmPathLogo");
	if(! frmPathLogo) return false;
	frmPathLogo->SetIsShow(false);

	frmAccount = CFormMgr::s_Mgr.Find( "frmAccount" );
	if(!frmAccount) return false;
	frmAccount->SetIsShow(false);
	frmAccount->evtEntrustMouseEvent = _evtLoginFrm;
    
	chkID  =( CCheckBox *)frmAccount->Find( "chkID" );
	m_bSaveAccount = false;
	if(!chkID)  return false;

	char szChkID[128] ={0};
	string strChkID;
	ifstream inCheck("user\\checkid.txt");
	if( inCheck.is_open())
	{
		while(!inCheck.eof())
		{
			inCheck.getline(szChkID, 128);
			strChkID = szChkID ;
			int nCheck = Str2Int(strChkID);
			m_bSaveAccount = (nCheck ==1)?true:false;
			chkID->SetIsChecked( m_bSaveAccount );
		}		
	}
	else
	{
		m_bSaveAccount = true;
		chkID->SetIsChecked( m_bSaveAccount );
	}

	edtID = dynamic_cast<CEdit*>(frmAccount->Find( "edtID" ));
	
	if(!edtID )    return false;
	m_sSaveAccount = "";

	char szName[128]= {0};
	ifstream in("user\\username.txt");

	_bAutoInputAct = FALSE;
	if(in.is_open())
	{
		while(!in.eof())
		{
			in.getline(szName, 128);
		}		
		_bAutoInputAct = TRUE;
	}
	m_sSaveAccount = string(szName);
	edtID->SetCaption( m_sSaveAccount.c_str());

	if( edtID )
	{
		edtID->evtEnter = _evtEnter;
		edtID->SetIsWrap(true);

	}
	
	
	
	edtPassword = dynamic_cast<CEdit*>(frmAccount->Find( "edtPassword" ));
	if( edtPassword )
	{
		edtPassword->SetCaption("");
		edtPassword->SetIsPassWord( true );
		edtPassword->SetIsWrap(true);
		edtPassword->evtEnter = _evtEnter;
	}

	frmKeyboard = CFormMgr::s_Mgr.Find("frmKeyboard");
	if (!frmKeyboard) return false;
	frmKeyboard->Hide();
	
	frmRegister = CFormMgr::s_Mgr.Find("frmRegister");
	if (!frmRegister) return false;
	frmRegister->Hide();
	frmRegister->evtEntrustMouseEvent = _evtLoginFrm;
	
	CEdit* edtRegPassword = dynamic_cast<CEdit*>(frmRegister->Find( "edtRegPassword" ));
	CEdit* edtRegPassword2 = dynamic_cast<CEdit*>(frmRegister->Find( "edtRegPassword2" ));
	edtRegPassword->SetIsPassWord( true );
	edtRegPassword2->SetIsPassWord( true );

	
	chkShift = ( CCheckBox *)frmKeyboard->Find("chkShift");
	if(!chkShift) return false;

	frmKeyboard->evtEntrustMouseEvent = _evtKeyboardFromMouseEvent;

	edtID->evtActive       = _evtAccountFocus;
	edtPassword->evtActive = _evtAccountFocus;

	imgLogo1 = (CImage*) frmAccount->Find("imgLogo1");
	if(!imgLogo1) return false;

	imgLogo2 = (CImage*) frmAccount->Find("imgLogo2");
	if(!imgLogo2) return false;

	return TRUE;
}

void CLoginScene::CloseNewChaFrm()
{
}

bool CLoginScene::IsValidCheckChaName(const char *name)
{
	if( !::IsValidName( name, (unsigned short)strlen(name) ) )
	{
		g_pGameApp->MsgBox(g_oLangRec.GetString(51));
		return false;
	}
	return true;

	const char* s = name ;
	int len = (int)strlen(s) ;
	bool bOk  = true;

	for ( int i = 0; i< len ; i++)
	{
		if ( s[i]&0x80 )
		{
			if (!(s[i]==-93) )  //用于处理是否是双字节的字母
			{
				i++;
			}
			else
			{
				bOk = false;
				i++;
				break;
			}
		}
		else
		{
			if (  !( isdigit(s[i])||isalpha(s[i]) ) )
			{
				bOk = false;			
				break;
			}
		}	
	}

	if (!bOk )
		g_pGameApp->MsgBox( g_oLangRec.GetString(52));

	return bOk;
}

bool CLoginScene::_CheckAccount()
{
	// 本地检查用户名和密码
	if(registerLogin){
		CEdit* edtRegPassword = dynamic_cast<CEdit*>(frmRegister->Find( "edtRegPassword" ));
		CEdit* edtRegPassword2 = dynamic_cast<CEdit*>(frmRegister->Find( "edtRegPassword2" ));
		CEdit* edtRegEmail = dynamic_cast<CEdit*>(frmRegister->Find( "edtRegEmail" ));

		if(strcmp(edtRegPassword->GetCaption(),edtRegPassword2->GetCaption())!=0){
			g_pGameApp->MsgBox("Two passwords do not match.");
			return false;
		}else{
			return true;
		}
	}
	
	if (strlen(edtID->GetCaption()) == 0)
	{
		g_pGameApp->MsgBox(g_oLangRec.GetString(174));
		return false;
	}
	if(!IsValidCheckChaName(edtID->GetCaption()))
		return false;

	if (strlen(edtPassword->GetCaption()) <= 4)
	{
		g_pGameApp->MsgBox(g_oLangRec.GetString(175));
		return false;
	}

	// 保存用户名
	SaveUserName(*chkID, *edtID);

	m_sUsername = edtID->GetCaption();
	m_sPassword = edtPassword->GetCaption();

	return true;
}

bool CLoginScene::_Bill()
{
	//收费流程

    if (g_Config.m_IsBill)
	{
		static CPAI cpai;
        int ret = cpai.Login(edtID->GetCaption(), edtPassword->GetCaption());
		if (10000 == ret)
		{
			m_sPassport = cpai.GetPassport();
			return true;
		}
        else if(10004 == ret)
        {
            CBoxMgr::ShowMsgBox( NULL, g_oLangRec.GetString(176), true );
        }
        else
        {
            std::string ret = cpai.LastError();
            if(ret == "[Player_Missing]")
            {
                CBoxMgr::ShowMsgBox( NULL, g_oLangRec.GetString(177), true );
            }
            else if(ret == "[Player_Failure]")
            {
                CBoxMgr::ShowMsgBox( NULL, g_oLangRec.GetString(178), true );
            }
            else
            {
                CBoxMgr::ShowMsgBox( NULL, "PAI: Unknown Error. !", true );
            }
        }
		return false;
	}
	m_sPassport = "nobill";
	return true;
}

void CLoginScene::_Connect()
{
	CGameApp::Waiting(true);

	//PlayWhalePose();	//鲸鱼动画改为初始化时就播放(Michael Chen 2005-06-03)

	_eState = enumConnect;	

	if (g_TomServer.bEnable)
	{
		CS_Connect(g_TomServer.szServerIP.c_str(), g_TomServer.nPort, g_Config.m_nConnectTimeOut);
		return;
	}

	LG("connect", g_oLangRec.GetString(179), m_iCurSelRegionIndex, m_iCurSelServerIndex);
	//int nSelRegionNo = 0;
	//int nNO = lstServer->GetItems()->GetSelect()->GetIndex();

	const char *pszSelectGateIP(0);

#ifdef USE_STATUS
	//得到服务器人数最少的Gate的Ip
	CRegionInfo* pRegion = GetRegionMgr()->GetActiovRegion();
	if( pRegion )
	{
		CGroupInfo* pGroup = pRegion ->Find( GetCurServerGroupName(m_iCurSelRegionIndex,m_iCurSelServerIndex) );
		if (pGroup)
		{
			CGateInfo* pGate = pGroup->GetMinGate();
			if (pGate)
			{
				pszSelectGateIP = pGate->szName;
			}
		}
	}
#endif

	//如果没有得到随机选择Gate的Ip
	//if (!pszSelectGateIP) pszSelectGateIP = SelectGroupIP(m_iCurSelRegionIndex, m_iCurSelServerIndex );
	if (!pszSelectGateIP) pszSelectGateIP = SelectGroupIP(0, 0);

	if (!pszSelectGateIP)
	{
		//LG("connect", g_oLangRec.GetString(180), m_iCurSelRegionIndex, m_iCurSelServerIndex);
		LG("connect", g_oLangRec.GetString(180), 0, 0);
	}
	else
	{
		CS_Connect(pszSelectGateIP, 1973, g_Config.m_nConnectTimeOut );
	}
//#endif

}


void CLoginScene::LoginFlow()
{
    ////////////////////////////////////////
    //
    //  By Jampe
    //  合作商用户名密码处理
    //  2006/5/19
    //
    switch(g_cooperate.code)
    {
    case COP_OURGAME:
    case COP_SINA:
    case COP_CGA:
        {
            m_sUsername = g_cooperate.uid;
            m_sPassword = g_cooperate.pwd;
        }  break;
    case 0:
    default:
        {
	        if (!_CheckAccount()){
		        return;
			}
        }   break;
    }
    //  end
	if (!_Bill()){
		return;
	}
	_Connect();
}

void CLoginScene::Select( int num, NetChaBehave chabehave[] )
{
}

void CLoginScene::NewCha()
{
}

void CLoginScene::DelCha()
{
}

void CLoginScene::_Login()
{
    _eState = enumAccount;
	if (registerLogin){
		CEdit* edtRegID = dynamic_cast<CEdit*>(frmRegister->Find( "edtRegID" ));
		CEdit* edtRegPassword = dynamic_cast<CEdit*>(frmRegister->Find( "edtRegPassword" ));
		CEdit* edtRegEmail = dynamic_cast<CEdit*>(frmRegister->Find( "edtRegEmail" ));
		CS_Register(edtRegID->GetCaption(), edtRegPassword->GetCaption(),edtRegEmail->GetCaption());
		registerLogin = false;
		CGameApp::Waiting();
	}else if( m_sUsername.size() != 0 && m_sPassword.size() != 0 ){
		CS_Login( m_sUsername.c_str(), m_sPassword.c_str(), m_sPassport.c_str() );
		CGameApp::Waiting();
    }
}

void CLoginScene::SaveUserName(CCheckBox& chkID, CEdit& edtID)
{
	//创建新文件夹
	if (!CreateDirectory("user", NULL)) 
	{ 
	} 

	m_bSaveAccount = chkID.GetIsChecked();
	m_sSaveAccount = string(edtID.GetCaption());

	//写文件
	FILE *fchk =fopen("user\\checkid.txt","wb");
	if ( fchk )
	{
		fwrite( m_bSaveAccount?"1":"0" , strlen("1"), 1,  fchk); 
		fclose(fchk);             
	}

	FILE *fp = fopen("user\\username.txt", "wb");
	if( fp )
	{
		if (m_bSaveAccount)
			fwrite(m_sSaveAccount.c_str() , m_sSaveAccount.size(), 1, fp);
		else 
			fwrite("" ,0,1, fp); 

		fclose(fp);
	}
}


void CLoginScene::BeginPlay()
{
}

void  CLoginScene::_evtVerErrorFrm(CCompent *pSender, int nMsgType, int x, int y, DWORD key)
{
	g_pGameApp->SetIsRun( false );

	if( nMsgType!=CForm::mrYes )
	{
		// 弹开一个网页
		if( strlen( g_Config.m_szVerErrorHTTP )==0 )
			return;

		/*	2008-10-15	close!
		::ShellExecute( NULL, "open", 
			g_Config.m_szVerErrorHTTP,
			NULL, NULL, SW_SHOW);
		*/
		return;
	}

	// 自动更新
	extern bool g_IsAutoUpdate;
	g_IsAutoUpdate = true;
}

void CLoginScene::Error( int error_no, const char* error_info )
{
    CGameApp::Waiting( false );
    LG( "error", "%s Error, Code:%d, Info: %s", error_info, error_no, g_GetServerError(error_no) );

	if( ERR_MC_VER_ERROR==error_no && !g_TomServer.bEnable )
	{		
		CBoxMgr::ShowSelectBox( _evtVerErrorFrm, g_oLangRec.GetString(181), true );
		return;
	}

    g_pGameApp->MsgBox( "%s", g_GetServerError(error_no) );
}

void CLoginScene::ReSetNewCha()
{
	_pSelectCha = CGameApp::GetCurScene()->AddCharacter(CLoginScene::nSelectChaType );
	for(int i = 0; i <enumEQUIP_PART_NUM; i++)
	{
		_pSelectCha->ChangePart(i, CLoginScene::nSelectChaPart[i]);		
	}	
}

void CLoginScene::ShowChaList()
{
	if( frmAccount )
    {
        frmAccount->Hide();
    }	
	if( frmAnnounce )
    {
        frmAnnounce->Hide();
    }
    if( frmServer )
    {
		frmServer->Hide();
	}
}

void CLoginScene::ShowServerList()
{
	CS_Disconnect(DS_DISCONN);

	if( frmKeyboard)		// add by Philip.Wu  2006-07-21
		ShowKeyboard(false);

	if( frmAccount )
    {
        frmAccount->Hide();
    }	
	if( frmAnnounce )
    {
        frmAnnounce->Hide();
    }
    if( frmServer )
    {
		frmServer->Show();
	}
}

void CLoginScene::ShowRegionList()
{
	CS_Disconnect(DS_DISCONN);

	if( frmKeyboard)		// add by Philip.Wu  2006-06-05
		ShowKeyboard(false);

	if( frmAccount )
		frmAccount->SetIsShow(false);

	if( frmAnnounce )
		frmAnnounce->SetIsShow(false);

	if( frmServer )
		frmServer->SetIsShow(false);

	InitRegionList();
	
	if (frmRegion)
		frmRegion->SetIsShow(true);
}

int  CLoginScene::GetServIconIndex(int iNum)
{
	if( iNum < 0 ) return 0;
	if( iNum>ServIconNum ) return ServIconNum;
	return iNum;
}


// add by Philip.Wu  2006-06-05
// 软键盘的鼠标点击按钮事件
void CLoginScene::_evtKeyboardFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if(!edtFocus) return;

	CLoginScene* pLoginScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if(!pLoginScene) return;

	string strText = edtFocus->GetCaption();
	string strName = pSender->GetName();
	if(strName.size() <= 0) return;

	// 按键消息的处理
	if(strName == "btnClose" || strName == "btnYes")	// 关闭软键盘
	{
		if(frmKeyboard->GetIsShow())
		{
			ShowKeyboard(false);
		}
	}
	else if(strName == "btnDel")	// 删除最后一个字符
	{
		if(strText.size() > 0)
		{
			strText.resize(strText.size() - 1);
			edtFocus->SetCaption(strText.c_str());
		}
	}
	else if(strName == "chkShift")	// 大小转换
	{
		
	}
	else if(strName == "btnOther101")	// 第一排特别字符键
	{
		strText += '~';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther102")
	{
		strText += '!';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther103")
	{
		strText += '@';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther104")
	{
		strText += '#';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther105")
	{
		strText += '$';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther106")
	{
		strText += '%';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther107")
	{
		strText += '^';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther108")
	{
		strText += '&';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther109")
	{
		strText += '*';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther110")
	{
		strText += '(';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther111")
	{
		strText += ')';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther112")
	{
		strText += '_';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther113")
	{
		strText += '+';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther114")
	{
		strText += '|';
		edtFocus->SetCaption(strText.c_str());
	}

	else if(strName == "btnOther201")	// 第二排特别字符键
	{
		strText += '`';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther202")
	{
		strText += '-';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther203")
	{
		strText += '=';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther204")
	{
		strText += '\\';
		edtFocus->SetCaption(strText.c_str());
	}

	else if(strName == "btnOther301")	// 第三排特别字符键
	{
		strText += '{';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther302")
	{
		strText += '}';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther303")
	{
		strText += '[';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther304")
	{
		strText += ']';
		edtFocus->SetCaption(strText.c_str());
	}

	else if(strName == "btnOther401")	// 第四排特别字符键
	{
		strText += ':';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther402")
	{
		strText += '\"';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther403")
	{
		strText += ';';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther404")
	{
		strText += '\'';
		edtFocus->SetCaption(strText.c_str());
	}

	else if(strName == "btnOther501")	// 第五排特别字符键
	{
		strText += '<';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther502")
	{
		strText += '>';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther503")
	{
		strText += '?';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther504")
	{
		strText += ',';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther505")
	{
		strText += '.';
		edtFocus->SetCaption(strText.c_str());
	}
	else if(strName == "btnOther506")
	{
		strText += '/';
		edtFocus->SetCaption(strText.c_str());
	}

	else	// 功能键全部排除后，剩下的是输入字符键按下
	{
		char cAdd = strName.at(strName.size() - 1);

		// 判断是否是数字或字母（暂时偷懒方式处理）
		if( ('0' <= cAdd && cAdd <= '9') )
		{
			strText += cAdd;
			edtFocus->SetCaption(strText.c_str());
		}
		else if( 'A' <= cAdd && cAdd <= 'Z')
		{
			if(chkShift->GetIsChecked())
			{
				// 大写
				strText += cAdd;
			}
			else
			{
				// 小写
				strText += cAdd + 32;
			}
			edtFocus->SetCaption(strText.c_str());
		}
	}
}


// add by Philip.Wu  2006-06-07
// 编辑框激活事件（保存下激活的编辑框）
void CLoginScene::_evtAccountFocus(CGuiData* pSender)
{
	CEdit* edtTemp = dynamic_cast<CEdit*>(pSender);

	if(edtTemp)
	{
		edtFocus = edtTemp;
	}
}


void CLoginScene::ShowKeyboard(bool bShow)
{
	frmKeyboard->SetIsShow(bShow);

	imgLogo1->SetIsShow(!bShow);
	imgLogo2->SetIsShow(!bShow);
}


void CLoginScene::ShowPathLogo(int isShow)
{
	frmPathLogo->SetIsShow(isShow ? true : false);
}