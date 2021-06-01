#include "StdAfx.h"
#include "uisystemform.h"
#include <stdio.h> 
#include <windows.h> 
#include "uiform.h"
#include "uicheckbox.h"
#include "Gameapp.h"
#include "uiformmgr.h"
#include "uiprogressbar.h"
#include "packetcmd.h"
#include "loginscene.h"
#include "cameractrl.h"
#include "Character.h"
#include "GameConfig.h"
#include "UIMinimapForm.h"
#include "uistartform.h"
#include "uiheadsay.h"
#include "netchat.h"
#include "smallmap.h"

#include "uiboatform.h"
#include "uiequipform.h"
#include "GlobalVar.h"
#include "scene.h"

#ifndef USE_DSOUND
#include "AudioThread.h"
extern CAudioThread g_AudioThread;
#endif

using namespace GUI;

extern bool g_IsShowStates;

//---------------------------------------------------------
// class CSystemProperties
//---------------------------------------------------------

/**
* 将系统的视频配置在游戏中生效.
* @return: success Return 0.
*/

CSystemProperties::SVideo::SVideo() :	
bFullScreen(false), bPixel1024(false), nTexture(0), nQuality(0),
bAnimation(false), bCameraRotate(false), bGroundMark(false), bDepth32(false)
{
}

int CSystemProperties::ApplyVideo()
{
	//video

	//bCameraRotate
	g_pGameApp->GetMainCam()->EnableRotation( m_videoProp.bCameraRotate );
	//bViewFar
	//g_pGameApp->GetMainCam()->EnableUpdown( m_videoProp.bViewFar ) ;//取消视野远近(Michael Chen 2005-04-22
	//nTexture-bGroundMark-nQuality
	g_pGameApp->GetCurScene()->SetTextureLOD(m_videoProp.nTexture);

	GetRender().SetIsChangeResolution(true);

	//尚未使用
	//bAnimation

	//bDepth32-bFullScreen-bPixel1024
	int width(0), height(0);
	if (m_videoProp.bPixel1024)
	{
		width = LARGE_RES_X;
		height = 768;
	}
	else
	{
		width = SMALL_RES_X;
		height = 600;
	}
	D3DFORMAT  format = m_videoProp.bDepth32 ? D3DFMT_D24X8 : D3DFMT_D16;

	MPIDeviceObject* dev_obj = g_Render.GetInterfaceMgr()->dev_obj;
	if(FAILED(dev_obj->CheckCurrentDeviceFormat(BBFI_DEPTHSTENCIL, format)))
	{
		format = D3DFMT_D16;
	}
	g_pGameApp->ChangeVideoStyle( width ,height ,format ,!m_videoProp.bFullScreen  );

	return 0;
}
/**
* 将系统音频配置在游戏中生效.
* @return: success Return 0.
*/
int CSystemProperties::ApplyAudio()
{
	//audio
	g_pGameApp->GetCurScene()->SetSoundSize( m_audioProp.nMusicEffect / 10.0f );			
	g_pGameApp->SetMusicSize( m_audioProp.nMusicSound / 10.0f  );
	g_pGameApp->mSoundManager->SetVolume(  m_audioProp.nMusicEffect / 10.0f  );

	return 0;

}
//-----------------------------------------------------------------------------
int CSystemProperties::ApplyGameOption()
{
	g_Config.SetMoveClient(m_gameOption.bRunMode);

	// Success
	return 0;
}


/**
* 将系统配置在游戏中生效.
* @return: success Return 0.
*          video failure Return -1.
*          audio failure Return -2.
*          gameOption failureboth Return -3.
*			other failure Return -4.
*/
int CSystemProperties::Apply()
{
	int nVideo = ApplyVideo();
	int nAudio = ApplyAudio();
	int nGameOption = ApplyGameOption();
	if (nVideo == 0 && nAudio == 0 && nGameOption == 0)
		return 0;
	if (nVideo != 0)
		return -1;
	if (nAudio != 0)
		return -2;
	if (nGameOption != 0)
		return -3;
	return -4;

}

/**
* The help function of reading the propties from the file(*.ini).
* @param: szIniFileName The name of ini file
* @return: 0 Success.
*          -1 File is not exist.
*          -2 File is destroyed.
*			-3 Filename is null or its length is zero.
*/
int CSystemProperties::readFromFile(const char * szIniFileName)
{
	//return 0;
#define DEFAULT_NUM -2

	if ((!szIniFileName) || (strlen(szIniFileName) == 0))
	{
		return -3;
	}

	int iTemp;
	// video
	iTemp = GetPrivateProfileInt("video", "texture", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_videoProp.nTexture = iTemp;

	iTemp = GetPrivateProfileInt("video", "animation", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_videoProp.bAnimation = int2bool(iTemp);

	iTemp = GetPrivateProfileInt("video", "cameraRotate", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_videoProp.bCameraRotate = int2bool(iTemp);

	iTemp = GetPrivateProfileInt("video", "groundMark", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_videoProp.bGroundMark = int2bool(iTemp);

	iTemp = GetPrivateProfileInt("video", "depth32", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_videoProp.bDepth32 = int2bool(iTemp);

	iTemp = GetPrivateProfileInt("video", "quality", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_videoProp.nQuality = iTemp;

	iTemp = GetPrivateProfileInt("video", "fullScreen", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_videoProp.bFullScreen = int2bool(iTemp);

	iTemp = GetPrivateProfileInt("video", "pixel1024", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_videoProp.bPixel1024 = int2bool(iTemp);

	// audio
	iTemp = GetPrivateProfileInt("audio", "musicSound", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_audioProp.nMusicSound = iTemp;

	iTemp = GetPrivateProfileInt("audio", "musicEffect", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_audioProp.nMusicEffect = iTemp;

	// gameOption
	iTemp = GetPrivateProfileInt("gameOption", "runMode", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_gameOption.bRunMode = int2bool(iTemp);

	iTemp = GetPrivateProfileInt("gameOption", "helpMode", 1, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_gameOption.bHelpMode = int2bool(iTemp);
	
	iTemp = GetPrivateProfileInt("gameOption", "apparel", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_gameOption.bAppMode = int2bool(iTemp);
	
	iTemp = GetPrivateProfileInt("gameOption", "effect", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_gameOption.bEffMode = int2bool(iTemp);
	
	iTemp = GetPrivateProfileInt("gameOption", "state", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_gameOption.bStateMode = int2bool(iTemp);

	// Add by lark.li 20080826 begin
	iTemp = GetPrivateProfileInt("startOption", "first", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_startOption.bFirst = int2bool(iTemp);
	// End

	// success
	return 0;
}
/**
* The help function of write the propties to the file(*.ini).
* @param: szIniFileName The name of ini file.
* @return: 0 Success.
*          -1 Error.
*			-2 File can not be created.
*			-3 Filename is null or its length is zero.
*/
int CSystemProperties::writeToFile(const char * szIniFileName)
{
#define OTHER_ERROR -1
#define ERROE_FILE_CANNT_CREAT -2

	if ((!szIniFileName) || (strlen(szIniFileName) == 0))
	{
		return -3;
	}

	// video
	if (!WriteInteger("video", "texture", m_videoProp.nTexture, szIniFileName))
	{
		FILE *fp;
		fp=fopen(szIniFileName, "wb");
		if( fp )
		{
			fclose(fp);
		}
		else
		{
			return ERROE_FILE_CANNT_CREAT;
		}
	}

	if (!WriteInteger("video", "animation", bool2int(m_videoProp.bAnimation), szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("video", "cameraRotate", bool2int(m_videoProp.bCameraRotate), szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("video", "groundMark", bool2int(m_videoProp.bGroundMark), szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("video", "depth32", bool2int(m_videoProp.bDepth32), szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("video", "quality", m_videoProp.nQuality, szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("video", "fullScreen", bool2int(m_videoProp.bFullScreen), szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("video", "pixel1024", bool2int(m_videoProp.bPixel1024), szIniFileName))
		return OTHER_ERROR;

	// audio
	if (!WriteInteger("audio", "musicSound", m_audioProp.nMusicSound, szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("audio", "musicEffect", m_audioProp.nMusicEffect, szIniFileName))
		return OTHER_ERROR;

	// gameOption
	//if (!WriteInteger("gameOption", "runMode", bool2int(m_gameOption.bRunMode), szIniFileName))
	if (!WriteInteger("gameOption", "runMode", bool2int(true), szIniFileName))
		return OTHER_ERROR;

	if (!WriteInteger("gameOption", "helpMode", bool2int(m_gameOption.bHelpMode), szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("gameOption", "apparel", bool2int(m_gameOption.bAppMode), szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("gameOption", "effect", bool2int(m_gameOption.bEffMode), szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("gameOption", "state", bool2int(m_gameOption.bStateMode), szIniFileName))
		return OTHER_ERROR;

	// Add by lark.li 20080826 begin
	if (!WriteInteger("startOption", "first", bool2int(m_startOption.bFirst), szIniFileName))
		return OTHER_ERROR;
	// End

	// Success
	return 0;
}

bool CSystemProperties::int2bool(int n)
{
	if (0 == n)
		return false;
	else 
		return true;
}

int	CSystemProperties::bool2int(bool b)
{
	return b?1:0;
}



//video界面的参数
static int     	g_nCbxTexture;
static int		g_nCbxMovie;
static int		g_nCbxCamera;
static int		g_nCbxView;
static int     	g_nCbxTrail;
static int     	g_nCbxColor;
static int     	g_nCbxSize;
static int     	g_nCbxModel;
static int     	g_bCbxQuality;
static float   	g_fPosMusic = -1.0f;           //环境音乐和即时音效
static float   	g_fPosMidi =  -1.0f;
static bool    	g_bChangeAudio = false;        //是否改变音频

//---------------------------------------------------------------------------
// class CVideoMgr
//---------------------------------------------------------------------------

CSystemMgr::CSystemMgr() 
: m_isLoad(false),frmSystem(0), frmAudio(0), proAudioMusic(0), proAudioMidi(0),
frmVideo(0), cbxTexture(0), cbxMovie(0), cbxCamera(0), cbxTrail(0), 
cbxColor(0), cbxSize(0), cbxModel(0), cbxQuality(0), frmAskRelogin(0),
frmAskExit(0), frmAskChange(0)
{}



void CSystemMgr::LoadCustomProp()
{
	//读取系统配置并应用(Michael Chen 2005-04-27)

	if (!m_isLoad)
	{
		if (m_sysProp.Load("user\\system.ini"))
		{
			//读取配置文件失败,用默认值填充
			m_sysProp.m_videoProp.nTexture=0;
			m_sysProp.m_videoProp.bAnimation=true;
			m_sysProp.m_videoProp.bCameraRotate=true;
			//m_sysProp.m_videoProp.bViewFar=true;      //取消视野远近(Michael Chen 2005-04-22
			m_sysProp.m_videoProp.bGroundMark=true;
			m_sysProp.m_videoProp.bDepth32=true;
			m_sysProp.m_videoProp.nQuality=0;
			m_sysProp.m_videoProp.bFullScreen=false;
			m_sysProp.m_videoProp.bPixel1024= true;//g_Render.GetScrWidth() > 1000 ? true : false;

			m_sysProp.m_audioProp.nMusicSound=static_cast<int>(10.0f* g_pGameApp->GetMusicSize());
			m_sysProp.m_audioProp.nMusicEffect=static_cast<int>(10.0f* g_pGameApp->GetCurScene()->GetSoundSize());

			m_sysProp.m_gameOption.bRunMode = true;
			m_sysProp.m_gameOption.bAppMode = true;
			m_sysProp.m_gameOption.bEffMode = true;
			m_sysProp.m_gameOption.bStateMode = false;

		}
	//	m_sysProp.m_gameOption.bRunMode = true;	//无论文件的设置如何，这一项都为true，临时
		m_isLoad = true;
	}
	CCharacter::SetIsShowShadow(m_sysProp.m_videoProp.bGroundMark);
	CCharacter::SetIsShowApparel(m_sysProp.m_gameOption.bAppMode);
	CCharacter::SetIsShowEffects(m_sysProp.m_gameOption.bEffMode);
	g_IsShowStates = m_sysProp.m_gameOption.bStateMode;
}
bool CSystemMgr::Init()
{

	frmSystem = _FindForm("frmSystem");//系统表单 
	if(!frmSystem ) 		return false;
	frmSystem->evtEntrustMouseEvent = _evtSystemFromMouseEvent;


	LoadCustomProp();       //读取用户自定义的配置

	///////////// Video系列
	frmVideo = _FindForm("frmVideo");
	if( !frmVideo )		return false;
	frmVideo->evtEntrustMouseEvent = _evtVideoFormMouseEvent;

	cbxTexture = ( CCheckGroup *)frmVideo->Find( "cbxTexture" ); //贴图精度
	if (! cbxTexture)	return Error( "msgui.clu界面<%s>上找不到控件<%s>", frmVideo->GetName(), "cbxTexture" );
	cbxTexture->SetActiveIndex(m_sysProp.m_videoProp.nTexture); //将配置文件的设置显示在控件上 Michael Chen 2005-04-22

	cbxMovie = ( CCheckGroup *)frmVideo->Find( "cbxMovie" ); //贴图精度
	if (! cbxMovie )	return Error( "msgui.clu界面<%s>上找不到控件<%s>", frmVideo->GetName(), "cbxMovie" );
	cbxMovie->SetActiveIndex(m_sysProp.m_videoProp.bAnimation ? 0 : 1); //将配置文件的设置显示在控件上 Michael Chen 2005-04-22

	cbxCamera = ( CCheckGroup *)frmVideo->Find( "cbxCamera" ); //贴图精度
	if (! cbxCamera )	return Error( "msgui.clu界面<%s>上找不到控件<%s>", frmVideo->GetName(), "cbxCamera" );
	cbxCamera->SetActiveIndex(m_sysProp.m_videoProp.bCameraRotate ? 0 : 1); //将配置文件的设置显示在控件上 Michael Chen 2005-04-22

	/** 取消视野远近(Michael Chen 2005-04-22
	cbxView = ( CCheckGroup *)frmVideo->Find( "cbxView" ); //贴图精度
	if (! cbxView )		return Error( "msgui.clu界面<%s>上找不到控件<%s>", frmVideo->GetName(), "cbxView" );
	cbxView->SetActiveIndex(m_sysProp.m_videoProp.bViewFar ? 0 : 1);    //将配置文件的设置显示在控件上 Michael Chen 2005-04-22
	*/

	cbxTrail = ( CCheckGroup *)frmVideo->Find( "cbxTrail" ); //贴图精度
	if (! cbxTrail )	return Error( g_oLangRec.GetString(45), frmVideo->GetName(), "cbxTrail" );
	cbxTrail->SetActiveIndex(m_sysProp.m_videoProp.bGroundMark ? 0 : 1);    //将配置文件的设置显示在控件上 Michael Chen 2005-04-22

	cbxColor = ( CCheckGroup *)frmVideo->Find( "cbxColor" ); //贴图精度
	if (! cbxColor )	return Error( g_oLangRec.GetString(45), frmVideo->GetName(), "cbxColor" );
	cbxColor->SetActiveIndex(m_sysProp.m_videoProp.bDepth32 ? 1 : 0);   //将配置文件的设置显示在控件上 Michael Chen 2005-04-22

	cbxSize = ( CCheckGroup *)frmVideo->Find( "cbxSize" ); //贴图精度
	if (! cbxSize )		return Error( g_oLangRec.GetString(45), frmVideo->GetName(), "cbxSize" );
	cbxSize->SetActiveIndex(m_sysProp.m_videoProp.bPixel1024 ? 1 : 0);  //将配置文件的设置显示在控件上 Michael Chen 2005-04-22

	cbxModel = ( CCheckGroup *)frmVideo->Find( "cbxModel" ); //贴图精度
	if (! cbxModel )	return Error( g_oLangRec.GetString(45), frmVideo->GetName(), "cbxModel" );	
	cbxModel->SetActiveIndex(m_sysProp.m_videoProp.bFullScreen ? 0 : 1);    //将配置文件的设置显示在控件上 Michael Chen 2005-04-22

	cbxQuality = ( CCheckGroup *)frmVideo->Find( "cbxQuality" ); //贴图精度
	if (! cbxQuality )	return Error( g_oLangRec.GetString(45), frmVideo->GetName(), "cbxQuality" );
	cbxQuality->SetActiveIndex(m_sysProp.m_videoProp.nQuality); //将配置文件的设置显示在控件上 Michael Chen 2005-04-22
	cbxQuality->evtSelectChange  = _evtVideoChangeChange;

	//////////Audio系列
	frmAudio = _FindForm("frmAudio");
	if( !frmAudio )         return false;
	frmAudio->evtEntrustMouseEvent = _evtAudioFormMouseEvent;

	proAudioMusic  = dynamic_cast<CProgressBar *>(frmAudio->Find("proAudioMusic"));    
	if( !proAudioMusic )		return Error( g_oLangRec.GetString(45), frmAudio->GetName(), "proAudioMusic" );
	proAudioMusic->SetPosition( static_cast<float>(m_sysProp.m_audioProp.nMusicSound) );    //将配置文件的设置显示在控件上 Michael Chen 2005-04-22
	proAudioMusic->evtMouseDown = _evtMainMusicMouseDown;

	proAudioMidi  = dynamic_cast<CProgressBar *>(frmAudio->Find("proAudioMidi"));    
	if( !proAudioMidi )			return Error( g_oLangRec.GetString(45), frmAudio->GetName(), "proAudioMidi" );
	proAudioMidi->SetPosition( static_cast<float>(m_sysProp.m_audioProp.nMusicEffect) );    //将配置文件的设置显示在控件上 Michael Chen 2005-04-22
	proAudioMidi->evtMouseDown = _evtMainMusicMouseDown;

	//////// GameOption系列
	frmGameOption = _FindForm("frmGame");
	if (!frmGameOption) return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "frmGame");
	frmGameOption->evtEntrustMouseEvent = _evtGameOptionFormMouseDown;
	frmGameOption->evtBeforeShow = _evtGameOptionFormBeforeShow;

	cbxRunMode = (CCheckGroup*)frmGameOption->Find("cbxRunmodel");
	if (!cbxRunMode) return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "cbxRunmodel");

	cbxLockMode = (CCheckGroup*)frmGameOption->Find("cbxLockmodel");
	if (!cbxLockMode) return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "cbxLockmodel");

	cbxHelpMode = (CCheckGroup*)frmGameOption->Find("cbxHelpmodel");
	if (!cbxHelpMode) return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "cbxHelpmodel");

	cbxAppMode = (CCheckGroup*)frmGameOption->Find("cbxAppmodel");
	if (!cbxAppMode) return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "cbxAppmodel");

	cbxEffMode = (CCheckGroup*)frmGameOption->Find("cbxEffmodel");
	if (!cbxEffMode) return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "cbxEffmodel");

	cbxStateMode = (CCheckGroup*)frmGameOption->Find("cbxStatemodel");
	if (!cbxStateMode) return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "cbxStatemodel");


	//////// 其它
	frmAskRelogin = _FindForm("frmAskRelogin");
	if( frmAskRelogin ) frmAskRelogin->evtEntrustMouseEvent = _evtAskReloginFormMouseDown;

	frmAskExit = _FindForm("frmAskExit");
	if( frmAskExit ) frmAskExit->evtEntrustMouseEvent = _evtAskExitFormMouseDown;

	frmAskChange = _FindForm("frmAskChange");
	if( frmAskChange ) frmAskChange->evtEntrustMouseEvent = _evtAskChangeFormMouseDown;

	return true;
}

void CSystemMgr::End()
{
	//将系统配置保存到文件(Michael Chen 2005-04-19)
	if (cbxTexture)
		m_sysProp.m_videoProp.nTexture = cbxTexture->GetActiveIndex();
	if (cbxMovie)
		m_sysProp.m_videoProp.bAnimation = cbxMovie->GetActiveIndex() == 0 ? true : false;
	if (cbxCamera)
		m_sysProp.m_videoProp.bCameraRotate = cbxCamera->GetActiveIndex() == 0 ? true : false;
	//m_sysProp.m_videoProp.bViewFar = cbxView->GetActiveIndex() == 0 ? true : false;取消视野远近(Michael Chen 2005-04-22
	if (cbxTrail)
		m_sysProp.m_videoProp.bGroundMark = cbxTrail->GetActiveIndex() == 0 ? true : false;
	if (cbxColor)
		m_sysProp.m_videoProp.bDepth32 = cbxColor->GetActiveIndex() == 0? false : true;
	if (cbxQuality)
		m_sysProp.m_videoProp.nQuality = cbxQuality->GetActiveIndex();
	if (cbxModel)
		m_sysProp.m_videoProp.bFullScreen = cbxModel->GetActiveIndex() == 0 ? true : false;
	if (cbxSize)
		m_sysProp.m_videoProp.bPixel1024 = cbxSize->GetActiveIndex() == 0 ? false : true;

	if (proAudioMusic)
		m_sysProp.m_audioProp.nMusicSound=static_cast<int>(proAudioMusic->GetPosition());
	if (proAudioMidi)
		m_sysProp.m_audioProp.nMusicEffect=static_cast<int>(proAudioMidi->GetPosition());

	if (cbxRunMode)
		m_sysProp.m_gameOption.bRunMode = cbxRunMode->GetActiveIndex() == 0 ? false : true;
	
	//if (cbxAppMode)
	//	m_sysProp.m_gameOption.bAppMode = cbxAppMode->GetActiveIndex() == 0 ? false : true;

	if (m_sysProp.Save("user\\system.ini"))
	{
		// error when save the system properties.
	}
	//end of modifying by Michael Chen
}

void CSystemMgr::_evtVideoChangeChange(CGuiData *pSender)
{
	CCheckGroup * g = dynamic_cast<CCheckGroup*>(pSender);
	if( !g ) return;

	if (g->GetActiveIndex()==0 )
	{
		g_stUISystem.cbxTexture->SetActiveIndex(0);
		g_stUISystem.cbxMovie->SetActiveIndex(0);
		g_stUISystem.cbxCamera->SetActiveIndex(0);
		//g_stUISystem.cbxView->SetActiveIndex(0);//取消视野远近(Michael Chen 2005-04-22
		g_stUISystem.cbxTrail->SetActiveIndex(0);
		g_stUISystem.cbxColor->SetActiveIndex(0);			
	}
	else if ( g->GetActiveIndex()==1)
	{
		g_stUISystem.cbxTexture->SetActiveIndex(1);
		g_stUISystem.cbxMovie->SetActiveIndex(0);
		g_stUISystem.cbxCamera->SetActiveIndex(1);
		//g_stUISystem.cbxView->SetActiveIndex(0);//取消视野远近(Michael Chen 2005-04-22
		g_stUISystem.cbxTrail->SetActiveIndex(0);
		g_stUISystem.cbxColor->SetActiveIndex(1);	
	}
	else if (g->GetActiveIndex()==2 )
	{
		g_stUISystem.cbxTexture->SetActiveIndex(2);
		g_stUISystem.cbxMovie->SetActiveIndex(1);
		g_stUISystem.cbxCamera->SetActiveIndex(1);
		//g_stUISystem.cbxView->SetActiveIndex(1);//取消视野远近(Michael Chen 2005-04-22
		g_stUISystem.cbxTrail->SetActiveIndex(1);
		g_stUISystem.cbxColor->SetActiveIndex(1);	
	}
}

void CSystemMgr::_evtVideoFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name = pSender->GetName();

	//frmVideo表单的处理
	if (name== "btnYes")						//直接关闭表单
	{			
		int  nTextureHigh = g_stUISystem.cbxTexture->GetActiveIndex();
		bool bMovieOn     = g_stUISystem.cbxMovie->GetActiveIndex()==0?true:false;
		bool bCameraOn    = g_stUISystem.cbxCamera->GetActiveIndex()==0?true:false;
		//bool bViewFar     = g_stUISystem.cbxView->GetActiveIndex()==0?true:false;//取消视野远近(Michael Chen 2005-04-22
		bool bTrailOn     = g_stUISystem.cbxTrail->GetActiveIndex()==0?true:false;
		CCharacter::SetIsShowShadow(bTrailOn);
		D3DFORMAT  format = g_stUISystem.cbxColor->GetActiveIndex()==0?D3DFMT_D24X8 : D3DFMT_D16;

		int  width =        g_stUISystem.cbxSize->GetActiveIndex()==0? SMALL_RES_X : LARGE_RES_X;
		int  height =       g_stUISystem.cbxSize->GetActiveIndex()==0? 600:768 ;

		bool bWindowed =    g_stUISystem.cbxModel->GetActiveIndex()==0?false:true;

		g_pGameApp->GetMainCam()->EnableRotation( bCameraOn );		
		g_stUISystem.m_sysProp.m_videoProp.bCameraRotate = bCameraOn;

		if(bCameraOn==false)
		{
			g_Render.EnableClearTarget(FALSE);
		}
		else
		{
			g_Render.EnableClearTarget(TRUE);
		}
		//g_pGameApp->GetMainCam()->EnableUpdown( bViewFar ) ;//取消视野远近(Michael Chen 2005-04-22
		g_pGameApp->GetCurScene()->SetTextureLOD(nTextureHigh);

		g_Config.m_bFullScreen = FALSE;
		if(! bWindowed)
		{
			width  = GetSystemMetrics(SM_CXSCREEN);
			height = GetSystemMetrics(SM_CYSCREEN);
			bWindowed = TRUE;
			g_Config.m_bFullScreen = TRUE;
		}

		GetRender().SetIsChangeResolution(true);

		MPIDeviceObject* dev_obj = g_Render.GetInterfaceMgr()->dev_obj;
		if(FAILED(dev_obj->CheckCurrentDeviceFormat(BBFI_DEPTHSTENCIL, format)))
		{
			format = D3DFMT_D16;
		}
		g_pGameApp->ChangeVideoStyle( width ,height ,format ,bWindowed  );
		pSender->GetForm()->SetIsShow(false);  

		g_stUISystem.frmSystem->SetIsShow(false);
		return;
	}
	else if (name == "btnNo" ||name=="btnClose" )       
	{
		g_stUISystem.cbxTexture->SetActiveIndex(g_nCbxTexture);
		g_stUISystem.cbxMovie->SetActiveIndex(g_nCbxMovie);
		g_stUISystem.cbxCamera->SetActiveIndex(g_nCbxCamera);
		//g_stUISystem.cbxView->SetActiveIndex(g_nCbxView);//取消视野远近(Michael Chen 2005-04-22
		g_stUISystem.cbxTrail->SetActiveIndex(g_nCbxTrail);
		g_stUISystem.cbxColor->SetActiveIndex(g_nCbxColor);
		g_stUISystem.cbxSize->SetActiveIndex(g_nCbxSize);
		g_stUISystem.cbxModel->SetActiveIndex(g_nCbxModel);
		g_stUISystem.cbxQuality->SetActiveIndex(g_bCbxQuality);

		pSender->GetForm()->SetIsShow(false);  
		return;
	}	 
}

void CSystemMgr::_evtSystemFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name = pSender->GetName();

	//frmSystem表单的处理
	if( name=="btnClose" || name == "btnNo" )
	{
		pSender->GetForm()->Close();
		return;
	}
	if (name== "btnChange") 
	{
		CForm* f = CFormMgr::s_Mgr.Find( "frmAskChange" );
		if( f ) 	f->SetIsShow(true);
		g_stUISystem.frmSystem->SetIsShow(false) ;
		return;
	}
	else if( name=="btnGame" )
	{
		CForm* f = CFormMgr::s_Mgr.Find( "frmGame" );
		if( f ) 	f->SetIsShow(true);
		g_stUISystem.frmSystem->SetIsShow(false) ;
		return;
	}
	else if (name == "btnRelogin")
	{
		if (g_TomServer.bEnable)
		{
			g_pGameApp->MsgBox( g_oLangRec.GetString(773) );
			return;
		}
		CForm* f = CFormMgr::s_Mgr.Find( "frmAskRelogin" );
		if( f ) 	f->SetIsShow(true);
		g_stUISystem.frmSystem->SetIsShow(false) ;
		return;
	}
	else if (name == "btnExit" )
	{
		CForm* f = CFormMgr::s_Mgr.Find( "frmAskExit" );
		if(f)  	f->SetIsShow(true);
		g_stUISystem.frmSystem->SetIsShow(false) ;
		return;
	}		
	else if (name == "btnAudio")
	{
		CForm *f = CFormMgr::s_Mgr.Find("frmAudio");
		if (!f)   return ;

		f->SetIsShow(true);
		if(g_fPosMusic<0.0f && g_fPosMusic<0.0f )
		{
			g_fPosMusic = g_stUISystem.proAudioMusic->GetPosition();				
			g_fPosMidi  = g_stUISystem.proAudioMidi->GetPosition();
			g_pGameApp->SetMusicSize( g_fPosMusic/10.0f);
			g_pGameApp->GetCurScene()->SetSoundSize( g_fPosMidi /10.0f );
			g_pGameApp->mSoundManager->SetVolume(  g_fPosMidi /10.0f   );

		}
		if(g_bChangeAudio)
		{
			g_fPosMusic = g_stUISystem.proAudioMusic->GetPosition();				
			g_fPosMidi  = g_stUISystem.proAudioMidi->GetPosition();
			g_pGameApp->SetMusicSize( g_fPosMusic/10.0f);
			g_pGameApp->GetCurScene()->SetSoundSize( g_fPosMidi /10.0f );
			g_pGameApp->mSoundManager->SetVolume(  g_fPosMidi /10.0f   );
		}							
		g_stUISystem.frmSystem->SetIsShow(false) ;
		return ;
	}
	else if (name == "btnVideo")
	{
		CForm *f = CFormMgr::s_Mgr.Find("frmVideo") ;
		if(!f)      return;

		f->SetIsShow(true);				
		g_nCbxTexture = g_stUISystem.cbxTexture->GetActiveIndex();
		g_nCbxMovie   = g_stUISystem.cbxMovie->GetActiveIndex();
		g_nCbxCamera  = g_stUISystem.cbxCamera->GetActiveIndex();
		//g_nCbxView    = g_stUISystem.cbxView->GetActiveIndex();//取消视野远近(Michael Chen 2005-04-22
		g_nCbxTrail   = g_stUISystem.cbxTrail->GetActiveIndex();
		g_nCbxColor   = g_stUISystem.cbxColor->GetActiveIndex();
		g_nCbxSize    = g_stUISystem.cbxSize->GetActiveIndex();
		g_nCbxModel   = g_stUISystem.cbxModel->GetActiveIndex();
		g_bCbxQuality = g_stUISystem.cbxQuality->GetActiveIndex();		
		g_stUISystem.frmSystem->SetIsShow(false) ;
		return ;
	}
}

void CSystemMgr::_evtAudioFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name=pSender->GetName();

	//frmAudio表单的处理
	if (name=="btnYes")						//直接关闭表单
	{
		g_fPosMusic = g_stUISystem.proAudioMusic->GetPosition();				
		g_fPosMidi  = g_stUISystem.proAudioMidi->GetPosition();
		g_pGameApp->SetMusicSize( g_fPosMusic/10.0f);
		g_pGameApp->GetCurScene()->SetSoundSize( g_fPosMidi /10.0f );
		g_pGameApp->mSoundManager->SetVolume(  g_fPosMidi /10.0f   );

		g_bChangeAudio = true;
		pSender->GetForm()->SetIsShow(false) ;  
		return  ;
	}
	else if (name=="btnNo")                   // 返回到原来的音量，并关闭表单
	{
		g_stUISystem.proAudioMusic->SetPosition(g_fPosMusic);	
		g_stUISystem.proAudioMidi->SetPosition(g_fPosMidi);
		g_pGameApp->SetMusicSize( g_fPosMusic/10.0f);
		g_pGameApp->GetCurScene()->SetSoundSize( g_fPosMidi /10.0f );
		g_pGameApp->mSoundManager->SetVolume(  g_fPosMidi /10.0f   );
		g_bChangeAudio = false;
		pSender->GetForm()->SetIsShow(false) ;  
		return;
	}	 
}

void CSystemMgr::_evtMainMusicMouseDown(CGuiData *pSender, int x, int y, DWORD key)
{
	CProgressBar *	proAudioMidi  = g_stUISystem.proAudioMidi;
	CProgressBar *  proAudioMusic = g_stUISystem.proAudioMusic;

	string name = pSender->GetName();
	float fPos;

	if ( stricmp ("frmAudio", pSender->GetForm()->GetName() ) == 0 )
	{
		if(name =="proAudioMusic")
		{
			fPos = 10.0f*(float)(x - proAudioMusic->GetLeft() - proAudioMusic->GetForm()->GetLeft() ) /(float)proAudioMusic->GetWidth() ;
			proAudioMusic->SetPosition( fPos);			
			proAudioMusic->Refresh();	
			g_pGameApp->SetMusicSize( fPos/10.0f  );

		}
		else if(name =="proAudioMidi" )
		{
			fPos = 10.0f* (float)(x - proAudioMidi->GetLeft() - proAudioMidi->GetForm()->GetLeft() ) /(float)proAudioMidi->GetWidth() ;
			proAudioMidi->SetPosition( fPos);			
			proAudioMidi->Refresh();
			g_pGameApp->mSoundManager->SetVolume( fPos/10.0f  );
			g_pGameApp->GetCurScene()->SetSoundSize( fPos/10.0f  );
		}
	}
}

void CSystemMgr::_evtAskReloginFormMouseDown(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name = pSender->GetName();
	pSender->GetForm()->SetIsShow(false);

	if (name== "btnYes") 
	{			
		g_ChaExitOnTime.Relogin();
		return;
	}
}

void CSystemMgr::_evtAskExitFormMouseDown(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name = pSender->GetName();
	pSender->GetForm()->SetIsShow(false);

	if (name== "btnYes") 
	{
		g_ChaExitOnTime.ExitApp();
	}
}

void CSystemMgr::_evtAskChangeFormMouseDown(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name = pSender->GetName();
	pSender->GetForm()->SetIsShow(false);

	if (name== "btnYes")	// 角色
	{
		g_ChaExitOnTime.ChangeCha();
	}
}

void CSystemMgr::_evtGameOptionFormMouseDown(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name = pSender->GetName();
	if (name!="btnYes") return;

	CCheckGroup *	pGroup  = g_stUISystem.cbxRunMode;
	if( pGroup )
	{
		g_stUISystem.m_sysProp.m_gameOption.bRunMode = pGroup->GetActiveIndex() == 0 ? false : true;
		g_stUISystem.m_sysProp.ApplyGameOption();
	}

	// 自动加锁状态
	pGroup = g_stUISystem.cbxLockMode;
	if(pGroup)
	{
		g_stUISystem.m_sysProp.m_gameOption.bLockMode = pGroup->GetActiveIndex() == 1 ? true : false;

		// 发送服务器
		CS_AutoKitbagLock(g_stUISystem.m_sysProp.m_gameOption.bLockMode);
	}

	// 显示帮助状态
	pGroup = g_stUISystem.cbxHelpMode;
	if(pGroup)
	{
		bool bHelpMode = pGroup->GetActiveIndex() == 1 ? true : false;
		g_stUISystem.m_sysProp.m_gameOption.bHelpMode = bHelpMode;

		if(! bHelpMode) g_stUIStart.ShowLevelUpHelpButton(bHelpMode);
		g_stUIStart.ShowInfoCenterButton(bHelpMode);

		::WritePrivateProfileString("gameOption", "helpMode", bHelpMode ? "1" : "0", "./user/system.ini");
	}
	
	pGroup = g_stUISystem.cbxAppMode;
	if(pGroup)
	{
		bool bAppMode = pGroup->GetActiveIndex() == 1 ? true : false;
		g_stUISystem.m_sysProp.m_gameOption.bAppMode = bAppMode;
		CCharacter::SetIsShowApparel(bAppMode);
		g_stUIStart.RefreshPet(); 
		//for each player in scene, re-render them.
		CGameScene* curScene  = g_pGameApp->GetCurScene();
		CCharacter* pCha = curScene->_pChaArray;
		for(int i = 0; i < curScene->_nChaCnt; i++){
			if( pCha->IsValid() && !pCha->IsNPC() && pCha->IsEnabled() ) {
				pCha->UpdataFace( pCha->GetPart() );
			}
			pCha++;
		}
		
		//update player portrait
		pCha = g_stUIBoat.GetHuman();
		static stNetTeamChaPart stTeamPart;	
		stTeamPart.Convert( pCha->GetPart() );
		g_stUIStart.GetMainCha()->UpdataFace( stTeamPart );
		
		::WritePrivateProfileString("gameOption", "apparel", bAppMode ? "1" : "0", "./user/system.ini");
	}
	
	pGroup = g_stUISystem.cbxEffMode;
	if(pGroup)
	{
		bool bEffMode = pGroup->GetActiveIndex() == 1 ? true : false;
		g_stUISystem.m_sysProp.m_gameOption.bEffMode = bEffMode;
		CCharacter::SetIsShowEffects(bEffMode);
		g_stUIStart.RefreshPet(); 
		//for each player in scene, re-render them.
		CGameScene* curScene  = g_pGameApp->GetCurScene();
		CCharacter* pCha = curScene->_pChaArray;
		for(int i = 0; i < curScene->_nChaCnt; i++){
			if( pCha->IsValid() && !pCha->IsNPC() && pCha->IsEnabled() ) {
				pCha->UpdataFace( pCha->GetPart() );
				pCha->RefreshSelfEffects();
			}
			pCha++;
		}
		
		::WritePrivateProfileString("gameOption", "effect", bEffMode ? "1" : "0", "./user/system.ini");
	}
	
	pGroup = g_stUISystem.cbxStateMode;
	if(pGroup)
	{
		bool bStateMode = pGroup->GetActiveIndex() == 1 ? true : false;
		g_stUISystem.m_sysProp.m_gameOption.bStateMode = bStateMode;
		g_IsShowStates = bStateMode;
		::WritePrivateProfileString("gameOption", "state", bStateMode ? "1" : "0", "./user/system.ini");
	}
}

void CSystemMgr::_evtGameOptionFormBeforeShow(CForm* pForm, bool& IsShow)
{
	CCheckGroup *	pGroup  = g_stUISystem.cbxRunMode;
	if( pGroup )
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bRunMode ? 1 : 0);

	pGroup = g_stUISystem.cbxLockMode;
	if( pGroup )
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bLockMode ? 1 : 0);

	pGroup = g_stUISystem.cbxHelpMode;
	if( pGroup )
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bHelpMode ? 1 : 0);
	
	pGroup = g_stUISystem.cbxAppMode;
	if( pGroup )
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bAppMode ? 1 : 0);
	
	pGroup = g_stUISystem.cbxEffMode;
	if( pGroup )
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bEffMode ? 1 : 0);
	pGroup = g_stUISystem.cbxStateMode;
	if( pGroup )
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bStateMode ? 1 : 0);
}


void CSystemMgr::CloseForm()
{
	g_ChaExitOnTime.Cancel();
}

void CSystemMgr::FrameMove(DWORD dwTime)
{
	g_ChaExitOnTime.FrameMove( dwTime );
}

//---------------------------------------------------------------------------
// class CChaExitOnTime
//---------------------------------------------------------------------------
namespace GUI
{
	CChaExitOnTime g_ChaExitOnTime;
};

CChaExitOnTime::CChaExitOnTime()
: _eOptionType(enumInit), _dwStartTime(0), _dwEndTime(0), _IsEnabled(false)
{
}

bool CChaExitOnTime::_IsTime()
{
	if( _eOptionType!=enumInit )
	{
		if( CGameApp::GetCurTick() > _dwStartTime + 60 * 1000 * 5 )
		{
			_eOptionType = enumInit;
			return false;
		}

		g_pGameApp->SysInfo( g_oLangRec.GetString(774) );
		return true;
	}

	return false;
}

void CChaExitOnTime::ChangeCha()
{
	if( _IsTime() ) return;

	_eOptionType = enumChangeCha;
	_dwStartTime = CGameApp::GetCurTick();

	_dwEndTime = 0;

	g_stUIMap.CloseRadar();	// 退出时关闭雷达箭头  add by Philip.Wu  2006-06-21
	CS_EndPlay();

#ifdef USE_DSOUND
	if( g_dwCurMusicID )
	{
		AudioSDL::get_instance()->stop( g_dwCurMusicID );
		g_dwCurMusicID = 0;
		Sleep( 60 );
	}
#endif

	if( !_IsEnabled )
	{
		TimeArrived();
	}
}

void CChaExitOnTime::ExitApp()
{
	if( _IsTime() ) return;

	_eOptionType = enumExitApp;
	_dwStartTime = CGameApp::GetCurTick();

	_dwEndTime = 0;
	CS_Logout();

	if( !_IsEnabled )
	{
		TimeArrived();
	}
}

void CChaExitOnTime::Relogin()
{
	if( _IsTime() ) return;

	_eOptionType = enumRelogin;
	_dwStartTime = CGameApp::GetCurTick();

	_dwEndTime = 0;

	g_stUIMap.CloseRadar();	// 退出时关闭雷达箭头  add by Philip.Wu  2006-06-21
	CS_Logout();

#ifdef USE_DSOUND
	if( g_dwCurMusicID )
	{
		AudioSDL::get_instance()->stop( g_dwCurMusicID );
		g_dwCurMusicID = 0;
		Sleep( 60 );
	}
#endif

	if( !_IsEnabled )
	{
		TimeArrived();
	}
}

void CChaExitOnTime::Cancel()
{
	if( !_IsEnabled ) return;

	if( _eOptionType==enumInit ) return;

	extern void CS_CancelExit();
	CS_CancelExit();

	_eOptionType = enumInit;
}

void CChaExitOnTime::FrameMove(DWORD dwTime)
{
	if( !_IsEnabled ) return;

	if( _eOptionType==enumInit ) return;

	if( _dwEndTime==0 ) return;

	if( dwTime < _dwEndTime ) 
	{
		static CTimeWork time(1000);
		if( time.IsTimeOut( dwTime ) )
		{
			g_pGameApp->ShowBigText( g_oLangRec.GetString(775), (_dwEndTime - dwTime)/1000 );
			return;
		}
	}
}

bool CChaExitOnTime::TimeArrived()
{
	switch( _eOptionType )
	{
	case enumChangeCha:
		{
			g_pGameApp->LoadScriptScene(enumLoginScene);
			g_pGameApp->SetLoginTime(0);

			CLoginScene* pScene = dynamic_cast<CLoginScene*>(g_pGameApp->GetCurScene());
			if( pScene ) 
			{
				if( g_NetIF->IsConnected() )
					pScene->ShowChaList();
				else
					//pScene->ShowRegionList();
					pScene->ShowLoginForm();
			}
		}
		break;
	case enumExitApp:
		{
			CS_Disconnect( DS_DISCONN );
			g_pGameApp->SetLoginTime(0);

			g_pGameApp->SetIsRun( false );
		}
		break;
	case enumRelogin:
		{
			CS_Disconnect( DS_DISCONN );
			g_pGameApp->SetLoginTime(0);

			g_pGameApp->LoadScriptScene(enumLoginScene);
			CLoginScene* pScene = dynamic_cast<CLoginScene*>(g_pGameApp->GetCurScene());
			if( pScene )
			{
				//pScene->ShowRegionList();
				pScene->ShowLoginForm();
			}
		}
		break;
	};

	if( _eOptionType!=enumInit ) 
	{
		_eOptionType = enumInit;	
		return true;
	}
	return false;
}

void CChaExitOnTime::NetStartExit( DWORD dwExitTime )
{
	_dwEndTime = CGameApp::GetCurTick() + dwExitTime;

	g_pGameApp->SysInfo( g_oLangRec.GetString(776), dwExitTime / 1000 );
}

void CChaExitOnTime::NetCancelExit()
{
	_eOptionType = enumInit;

	g_pGameApp->SysInfo( g_oLangRec.GetString(777) );
}

void CChaExitOnTime::Reset()		
{ 
	_eOptionType = enumInit;
}

