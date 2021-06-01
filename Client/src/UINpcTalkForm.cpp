#include "StdAfx.h"
#include "uinpctalkform.h"
#include "uiequipform.h"
#include "uigoodsgrid.h"
#include "uiitemcommand.h"
#include "uipage.h"
#include "packetcmd.h"
#include "gameapp.h"
#include "scene.h"
#include "character.h"
#include "uiboxform.h"
#include "uiedit.h"
#include "STNpcTalk.h"
#include "uiformmgr.h"
#include "uimemo.h"
#include "worldscene.h"
#include <shellapi.h>

#include "NetChat.h"
#include "SMallMap.h"
#include "UI3DCompent.h"
#include "UIsystemform.h"
#include "GlobalVar.h"
#include "UILabel.h"
#include "AreaRecord.h"

using namespace GUI;

static BYTE		_byIndex = - 1; 
static BYTE		_byPage = - 1;       
static DWORD	_npcID = -1;       //npc��id ��
static BYTE		_byCmd = 0;

BYTE CNpcTalkMgr::_byTalkStyle = 0;

CCharacter2D* CNpcTalkMgr::pNpc = NULL;
C3DCompent* CNpcTalkMgr::p3D = NULL;

//---------------------------------------------------------------------------
// class CNpcTalkMgr
//---------------------------------------------------------------------------
bool CNpcTalkMgr::Init()
{
	m_bIsNpcTalk = false;
	CFormMgr &mgr = CFormMgr::s_Mgr;

	//��ʼ��npc�Ի���
	frmNPCchat = _FindForm("frmNPCchat");   // ���߱�
	if ( !frmNPCchat ) return false;	
	
	frmNPCchat->evtEntrustMouseEvent = _MainMouseNPCEvent;
	frmNPCchat->evtMouseDragEnd = _OnDragEnd;

	frmNPCchat->Refresh();
	memCtrl = dynamic_cast<CMemo*> (frmNPCchat->Find("memCtrl")) ;
	if( !memCtrl ) return Error(g_oLangRec.GetString(45), frmNPCchat->GetName(), "memCtrl");	
	memCtrl->evtSelectChange = _evtMemSelectChange;
	
	p3D = dynamic_cast<C3DCompent*>( frmNPCchat->Find("d3dNpc") );
	p3D->SetRenderEvent( _NpcRenderEvent );
	RECT rt;
	rt.left = p3D->GetX();
	rt.right = p3D->GetX2();
	rt.top = p3D->GetY();
	rt.bottom = p3D->GetY2();
	pNpc = new CCharacter2D;
	pNpc->Create( rt );
			
	return true;
}

void CNpcTalkMgr::End()
{
	SAFE_DELETE(pNpc);
}

bool CNpcTalkMgr::RefreshNpcModelPos(){	
	if(p3D->GetX() < 0 ||  p3D->GetY() <0
	|| (g_stUISystem.m_sysProp.m_videoProp.bPixel1024 && (p3D->GetX2() > LARGE_RES_X ||  p3D->GetY2() > 768))
	|| (!g_stUISystem.m_sysProp.m_videoProp.bPixel1024 && (p3D->GetX2() > SMALL_RES_X ||  p3D->GetY2() > 600))
	){
		return false;
	}

	RECT rt;
	rt.left = p3D->GetX();
	rt.right = p3D->GetX2();
	rt.top = p3D->GetY();
	rt.bottom = p3D->GetY2();
	if(!pNpc){
		pNpc = new CCharacter2D;
	}
	pNpc->Create( rt );
	return true;
}



void CNpcTalkMgr::HideNpcModel(){
	RECT rt;
	rt.left = 0;
	rt.right =  0;
	rt.top =  0;
	rt.bottom =  0;
	pNpc = new CCharacter2D;
	pNpc->Create( rt );
}

void CNpcTalkMgr::RefreshNpcModel(){	
	CCharacter* pNpcCha = CGameApp::GetCurScene()->SearchByID( _npcID );
	if(pNpcCha && pNpc){
		static stNetTeamChaPart stNpcPart;		
		stNpcPart.Convert( pNpcCha->GetPart() );	
		pNpc->UpdataFace( stNpcPart );
		pNpc->LoadCha(pNpcCha->getMobID());
		
		if ( frmNPCchat ){
			CLabel * labNpcName = dynamic_cast<CLabel *>( frmNPCchat->Find("npcName") );
			labNpcName->SetCaption(pNpcCha->getName());
		}
	}else{
		CLabel * labNpcName = dynamic_cast<CLabel *>( frmNPCchat->Find("npcName") );
		labNpcName->SetCaption("System");
	}
}

void CNpcTalkMgr::_NpcRenderEvent(C3DCompent *pSender, int x, int y)
{
	pNpc->Render();
	
}

void CNpcTalkMgr::_OnDragEnd(CGuiData *pSender, int x, int y, DWORD key ){
	if(g_stUINpcTalk.RefreshNpcModelPos()){
		g_stUINpcTalk.RefreshNpcModel();
	}else{
		g_stUINpcTalk.HideNpcModel();
	}
}

DWORD CNpcTalkMgr::GetNpcId()
{
	return _npcID;
}

void CNpcTalkMgr::ShowFuncPage( BYTE byFuncPage , BYTE byCount,BYTE byMisNum, const NET_FUNCPAGE& FuncArray, DWORD dwNpcID ) 
{
	m_bIsNpcTalk = true;

	//ɾ��Lst�ؼ�ԭ����Item
	if( !memCtrl ) return;

	
	
	
    memCtrl->Init();
	memCtrl->reset();

	
	
	memCtrl->SetCaption( FuncArray.szTalk );

	if ( byCount >0 )   //��ȡѡ������
	{
		memCtrl->SetIsHaveItem(true);			
		memCtrl->SetItemRowNum( byCount);
		for (int i =0 ; i<byCount ; i++)
		{
			if( FuncArray.FuncItem[i].szFunc[0]=='@' )
			{
				string item = &FuncArray.FuncItem[i].szFunc[1];
				size_t pos = item.find( "http" );
				if( pos==string::npos )
				{
					memCtrl->AddItemRowContent( i, item.c_str() );
				}
				else
				{
					string http = item.substr( pos, item.length() );
					item = item.substr( 0, pos );
					memCtrl->AddItemRowContent( i, item.c_str(), http.c_str() );
				}
			}
			else
			{
				memCtrl->AddItemRowContent( i, FuncArray.FuncItem[i].szFunc );
			}
		}			
	}
	if ( byMisNum >0 )
	{
		memCtrl->SetIsHaveMis(true);		
		memCtrl->SetMisRowNum( byMisNum);
		for (int i =0 ; i<byMisNum ; i++){
			int index = FuncArray.MisItem[i].byState; // 0-255
			int y = (index/16) * 16;
			int x = (index%16) * 16;
			CGraph *p = new CGraph("texture/ui/corsairs/missionIcon.png", 16,16,x,y,10);
			memCtrl->AddIcon( i, p);
			memCtrl->AddMisRowContent( i , FuncArray.MisItem[i].szMis);
		}			
	}
	memCtrl->ProcessCaption();
	
	if ( frmNPCchat )
	{
		frmNPCchat->Show();
		_byIndex  = -1;
	}

	_byPage = byFuncPage;
	_npcID = dwNpcID;
	
	
	
	if(RefreshNpcModelPos()){
		RefreshNpcModel();
	}else{
		//Hide it
		HideNpcModel();
	}
	
	CCharacter* pMain = CGameScene::GetMainCha();
	int nArea = CGameApp::GetCurScene()->GetTerrain()->GetTile( pMain->GetCurX()/100, pMain->GetCurY()/100 )->getIsland();
	char buf[64];
	
	if( nArea){
		sprintf(buf,"texture/ui/corsairs/npcBackdrop/%d.tga",nArea);
	}else{
		sprintf(buf,"texture/ui/corsairs/npcBackdrop/%d.tga",0);
	}
	CCompent* imgNpcBackDrop = dynamic_cast<CCompent*>(g_stUINpcTalk.frmNPCchat->Find("imgNpcBackDrop"));
	if(imgNpcBackDrop){
		 //https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c/25450408#25450408 # 3
		if(GetFileAttributes(buf) == INVALID_FILE_ATTRIBUTES){
			imgNpcBackDrop->GetImage()->LoadImage("texture/ui/corsairs/npcBackdrop/0.tga",55, 44, 0, 0, 0);
		}else{
				imgNpcBackDrop->GetImage()->LoadImage(buf,55, 44, 0, 0, 0);
		}
	}
	
	
	return;
}

void CNpcTalkMgr::CloseForm()			
{ 
	if( m_bIsNpcTalk && frmNPCchat->GetIsShow() )
	{
		frmNPCchat->Close();	
	}
}

void CNpcTalkMgr::_MainMouseNPCEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name = pSender->GetName();

	if( name=="btnNo"  || name == "btnClose" )  //������˳���ť,��رոñ�
	{		
		pSender->GetForm()->Close();
		pSender->GetForm()->Find("memCtrl")->SetCaption("");			
		return;
	}		
	return;
}

void CNpcTalkMgr::_evtMemSelectChange(CGuiData *pSender)
{
	CMemo* memo = dynamic_cast<CMemo*>(pSender);
	if( !memo ) return;

	if (!memo->GetIsHaveItem() &&!memo->GetIsHaveMis() )
		return;

	int nMis= -1;
	int nItem = -1;

	nMis = memo->GetSelectMis();
	nItem = memo->GetSelectItem();

	if( nMis==-1 && nItem ==-1 )
		return;

	if ( nMis >=0  )
	{
		_byTalkStyle = 1;
		_byIndex =nMis ;
	}

	if (nItem >=0 )
	{
		_byTalkStyle = 0; 
		_byIndex =nItem ;
	}	
	
	switch( _byTalkStyle )
	{
	case 0:
		{
			string item, itemex;
			memo->GetSelectItemText( item, itemex );
			if( itemex.empty() )
			{
				CS_SelFunction( _npcID, _byPage, _byIndex) ;
			}
			else
			{
				::ShellExecute( NULL, "open", 
					itemex.c_str(),
					NULL, NULL, SW_SHOW);
			}
		}
		break;
	case 1:
		CS_SelMission( _npcID, _byIndex );
	
		break;
	case 2:
		CS_SelMissionFunc( _npcID, _byPage, _byIndex) ;
		break;
	}			
	pSender->GetForm()->Close();
}

void CNpcTalkMgr::FrameMove(DWORD dwTime)
{	return;
	static CTimeWork time(100);
	if( !time.IsTimeOut( dwTime ) ) return;

	if( frmNPCchat && !frmNPCchat->GetIsShow() )
	{
		if( m_HelpInfoList.size() > 0 )
		{
			HELP_LIST::iterator pos = m_HelpInfoList.begin();
			if( pos == m_HelpInfoList.end() )
				return;

			ShowHelpInfo( *pos );
			m_HelpInfoList.erase( pos );
		}
	}
}

void CNpcTalkMgr::LoadingCall()
{
}

void CNpcTalkMgr::SwitchMap()
{
	if( !(dynamic_cast<CWorldScene*>( CGameApp::GetCurScene() )) ) return;

	// ����
	static bool IsFirstWorldScene = true;
	if( IsFirstWorldScene )
	{
		IsFirstWorldScene = false;
		FILE* fp = fopen( "./scripts/txt/HelpInfo.tx", "rt" );
		if( fp )
		{
			if( fseek(fp, 0, SEEK_END) == 0 )
			{
				long size = ftell( fp );
				fseek(fp, 0, SEEK_SET);

				NET_HELPINFO Info;
				memset( &Info, 0, sizeof(NET_HELPINFO) );
				//fread(Info.szDesp, size > ROLE_MAXNUM_DESPSIZE - 1 ? ROLE_MAXNUM_DESPSIZE - 1 : size, 1, fp);
				fread(Info.szDesp, size > HELPINFO_DESPSIZE - 1 ? HELPINFO_DESPSIZE - 1 : size, 1, fp);
				Info.byType = mission::MIS_HELP_DESP;
				AddHelpInfo( Info );			
			}
			fclose( fp );
		}
		return;
	}
}

void CNpcTalkMgr::ShowHelpInfo(const NET_HELPINFO &Info)
{
	if( Info.byType == mission::MIS_HELP_DESP || Info.byType == mission::MIS_HELP_IMAGE )
	{
		m_bIsNpcTalk = false;
		if ( memCtrl )
		{
			memCtrl->Init();
			memCtrl->SetCaption( Info.szDesp );
			memCtrl->ProcessCaption();
			if( frmNPCchat )
			{
				frmNPCchat->Show();
			}
			else
			{

			}
		}
		else
		{
		}			
	}
	else if( Info.byType == mission::MIS_HELP_SOUND )
	{
		//Info.sID;
	}
	else
	{
		return ;
	}
	return;
}

void CNpcTalkMgr::AddHelpInfo(const NET_HELPINFO &Info)
{
	m_HelpInfoList.push_back( Info );
}

void CNpcTalkMgr::ShowTalkPage( const char *content, BYTE command, DWORD npcID )
{
	if ( memCtrl )
	{
		memCtrl->Init();
		memCtrl->SetCaption( content );
		memCtrl->ProcessCaption();
	}
	if ( frmNPCchat )
	{
		m_bIsNpcTalk = true;
		frmNPCchat->Show();
	}

	_byCmd = command;
	_npcID = npcID;
}

void CNpcTalkMgr::CloseTalk( DWORD dwNpcID )
{
	if( frmNPCchat )
	{
		frmNPCchat->Close();
		memCtrl->SetCaption("") ;
	}
}
