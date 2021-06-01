#include "Stdafx.h"
#include "GameApp.h"
#include "MPEditor.h"
#include "Scene.h"
#include "FindPath.h"
#include "UIFormMgr.h" 
#include "GlobalVar.h"
#include "DrawPointList.h"
#include "SmallMap.h"
#include "Character.h"
#include "UICursor.h"
#include "Track.h"
#include "cameractrl.h"
#include "SceneObj.h"

#ifdef TESTDEMO
#include "TestDemo.h"
#endif
Ctemp tmap;


struct _RHFFVF
{
    lwVector4 pos;
    DWORD dif;
};

BOOL RenderHintFrame(const RECT* rc, DWORD color)
{
    //color = 0xffff0000;

    DWORD rs_lgt;
    DWORD rs_vc;
    _RHFFVF vert[5];
    vert[0].pos = lwVector4((float)rc->left, (float)rc->top, 0, 1.0f);
    vert[1].pos = lwVector4((float)rc->right, (float)rc->top, 0, 1.0f);
    vert[2].pos = lwVector4((float)rc->right, (float)rc->bottom, 0, 1.0f);
    vert[3].pos = lwVector4((float)rc->left, (float)rc->bottom, 0, 1.0f);
    vert[4].pos = vert[0].pos;
    vert[0].dif = vert[1].dif = vert[2].dif = vert[3].dif = vert[4].dif = color;
    MPIResourceMgr* res_mgr = g_Render.GetInterfaceMgr()->res_mgr;
    MPIDeviceObject* dev_obj = g_Render.GetInterfaceMgr()->dev_obj;
    lwIDynamicStreamMgr* dns_mgr = res_mgr->GetDynamicStreamMgr();
    dev_obj->GetRenderState(D3DRS_LIGHTING, &rs_lgt);
    dev_obj->GetRenderState(D3DRS_COLORVERTEX, &rs_vc);
    dev_obj->SetRenderStateForced(D3DRS_LIGHTING, 0);
    dev_obj->SetRenderStateForced(D3DRS_COLORVERTEX, 1);
  
    lwMaterial mtl;
    memset(&mtl, 0, sizeof(mtl));
    mtl.amb.a = mtl.amb.r = mtl.amb.g = mtl.amb.b = 1.0f;
    dev_obj->SetMaterial(&mtl);

    dev_obj->SetTexture(0, NULL);

    if(LW_FAILED(dns_mgr->DrawPrimitive(D3DPT_LINESTRIP, 4, vert, sizeof(_RHFFVF), (D3DFORMAT)(D3DFVF_XYZRHW|D3DFVF_DIFFUSE))))
    {
        dev_obj->SetRenderState(D3DRS_LIGHTING, rs_lgt);
        dev_obj->SetRenderState(D3DRS_COLORVERTEX, rs_vc);
        return 0;
    }

    dev_obj->SetRenderState(D3DRS_LIGHTING, rs_lgt);
    dev_obj->SetRenderState(D3DRS_COLORVERTEX, rs_vc);
    return 1;
};

//CMinimap* minimap = NULL;
// Game SDK ��ܺ���, ���û����
void CGameApp::_Render()
{
	ResMgr.RestoreEffect();

	if(IsEnableSpSmMap())
	{
		_CreateSmMap( _pCurScene->GetTerrain() );
	}

	if(btest)
	{
		CreateCharImg();
		return;
	}


	// Scene�Ļ���------------------------------------------------------------
	MPTimer tScene; tScene.Begin();
	_pCurScene->_Render();
	m_dwRenderSceneTime = tScene.End();

#ifdef TESTDEMO
	g_pTestDemo->Render();
#endif
	// ��Χ���ߵ���Ⱦ(��Scene���, ��Ҫ�ڽ���֮ǰ)-------------------------------------------
	_pDrawPoints->Reader();
	g_Editor.Render();
	g_Render.RenderAllLines();

	// ����ؼ�����Ⱦ-----------------------------------------------------------------------
	MPTimer tUI; tUI.Begin();

	_stCursorMgr.Render();
	_pCurScene->_RenderUI();

	// С��ͼ��Ⱦ----------------------------------------------------------------------------
	CCharacter *pMainCha =_pCurScene->GetMainCha();
	MPTimer mpt; mpt.Begin();
	if( CGameScene::_bShowMinimap && CGameScene::_pSmallMap )
	{
		if(pMainCha)
		{
			CGameScene::_pSmallMap->MoveTo( GetMainCam()->m_RefPos,-GetMainCam()->m_vDir,\
				GetMainCam()->m_fAngle,GetMainCam()->m_fxy/7,pMainCha->getYaw());
			CGameScene::_pSmallMap->Render();
		}
	}
	m_dwRenderMMap = mpt.End();

	CFormMgr::s_Mgr.Render();
	CFormMgr::s_Mgr.RenderHint( GetMouseX(), GetMouseY() );
	CCursor::I()->Render();  

	if( _dwNotifyTime>GetCurTick() )
	{
		_pNotify->Render();
	}

	if(_dwNotifyTime1>GetCurTick() )
	{
		_pNotify1->Render();//Add by sunny.sun20080804
	}
	m_dwRenderUITime = tUI.End();

	ResMgr.RestoreEffect();


	MPTimer mt; mt.Begin();

	if(CGameScene::_pBigMap)
		CGameScene::_pBigMap->Render();

	// ��Ļ��ʾ����Ⱦ-----------------------------------------------------------------------
	if(_IsRenderTipText) _RenderTipText();

	//if(!_qCQueueStrColour.empty())
	//{
		//pair <STipText*, unsigned int> curpair;
		//curpair = _qCQueueStrColour.back();
		//STipText* tmp = curpair.first;
		//if (curpair.second >= TIME_CQUEUE & _IsRenderColourTest)

		if (_IsRenderColourTest)
			RenderColourQueue();
	//}

	if( _stMidFont.dwBeginTime > GetCurTick() )
	{
		_stMidFont.btAlpha = (BYTE)(255.0f * (float)(_stMidFont.dwBeginTime - GetCurTick()) / (float)SHOW_TEXT_TIME);

		DWORD dwAlpha = _stMidFont.btAlpha << 24;
		DWORD dwColor = dwAlpha | 0x00f01000; 

		SIZE size;
		_MidFont.GetTextSize( _stMidFont.szText, &size );        
		_MidFont.DrawText(_stMidFont.szText, (GetWindowWidth() - size.cx) / 2, (GetWindowHeight() - size.cy) / 3, dwColor);
	}

	//������Ļ, ��Ҫ�����-------------------------------------------------------------
	if(IsEnableSpAvi())  
		_CreateAviScreen();
}
