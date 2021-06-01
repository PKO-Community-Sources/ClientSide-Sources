//------------------------------------------------------------------------
//	2005.4.27	Arcol	create this file
//------------------------------------------------------------------------


#include "stdafx.h"
#include "UIPage.h"
#include "netguild.h"
#include "UIFormMgr.h"
#include "UITextButton.h"
#include "GuildMemberData.h"
#include "GuildMembersMgr.h"
#include "RecruitMemberData.h"
#include "RecruitMembersMgr.h"
#include "UIBoxForm.h"
#include "UIListView.h"
#include "UILabel.h"
#include "UIEdit.h"
#include "UICheckbox.h"
#include "GameApp.h"
#include "Character.h"
#include "uiguildmgr.h"
#include "commfunc.h"
#include "uiBoatForm.h"
#include "ChaAttrType.h"
#include "SMallMap.h"
#include "UI3DCompent.h"
#include "NetChat.h"
#include "UIsystemform.h"
#include "GlobalVar.h"
#include "UITreeView.h"
#include "UIGraph.h"


CForm*	CUIGuildMgr::m_pGuildMgrForm=NULL;
CForm*	CUIGuildMgr::m_pGuildColForm=NULL;
CForm*	CUIGuildMgr::m_pGuildPermForm=NULL;
CLabelEx*	CUIGuildMgr::m_plabGuildName=NULL;
CLabelEx*	CUIGuildMgr::m_plabGuildMottoName=NULL;
CLabelEx*	CUIGuildMgr::m_plabGuildType=NULL;
CLabelEx*	CUIGuildMgr::m_plabGuildMaster=NULL;
CLabelEx*	CUIGuildMgr::m_plabGuildMemberCount=NULL;
CLabelEx*	CUIGuildMgr::m_plabGuildMoney=NULL;
CLabelEx*	CUIGuildMgr::m_plabGuildExperience=NULL;
CLabelEx*	CUIGuildMgr::m_plabGuildState=NULL;
CLabelEx*	CUIGuildMgr::m_plabGuildRemainTime=NULL;
//CLabelEx*	CUIGuildMgr::m_plabGuildRank=NULL;
CListView*	CUIGuildMgr::m_plstGuildMember=NULL;
CListView*	CUIGuildMgr::m_plstRecruitMember=NULL;
CPage*		CUIGuildMgr::m_ppgeClass=NULL;
CTextButton*	CUIGuildMgr::m_pbtnGuildMottoEdit=NULL;
CTextButton*	CUIGuildMgr::m_pbtnGuildDismiss=NULL;
CTextButton*	CUIGuildMgr::m_pbtnMemberRecruit=NULL;
CTextButton*	CUIGuildMgr::m_pbtnMemberRefuse=NULL;
CTextButton*	CUIGuildMgr::m_pbtnMemberKick=NULL;
CTextButton*	CUIGuildMgr::m_pbtnMemberPerms=NULL;
CTextButton*	CUIGuildMgr::m_pbtnHouse=NULL;
CTextButton*	CUIGuildMgr::m_pbtnColour=NULL;
CTextButton*	CUIGuildMgr::m_pbtnMemberYesPerms=NULL;



CForm*	CUIGuildMgr::m_pGuildMottoNameEditForm=NULL;
CEdit*	CUIGuildMgr::m_pedtGuildMottoName=NULL;
CTextButton*	CUIGuildMgr::m_pbtnGuildMottoFormOK=NULL;



CTextButton*	CUIGuildMgr::m_pbtnattrPR=NULL;
CTextButton*	CUIGuildMgr::m_pbtnattrMS=NULL;
CTextButton*	CUIGuildMgr::m_pbtnattrASPD=NULL;
CTextButton*	CUIGuildMgr::m_pbtnattrMX=NULL;
CTextButton*	CUIGuildMgr::m_pbtnattrDEF=NULL;
CTextButton*	CUIGuildMgr::m_pbtnattrHIT=NULL;
CTextButton*	CUIGuildMgr::m_pbtnattrFLEE=NULL;
CTextButton*	CUIGuildMgr::m_pbtnattrHPR=NULL;
CTextButton*	CUIGuildMgr::m_pbtnattrSPR=NULL;
CTextButton*	CUIGuildMgr::m_pbtnattrHP=NULL;
CTextButton*	CUIGuildMgr::m_pbtnattrSP=NULL;

CEdit* CUIGuildMgr::m_edtRed=NULL;
CEdit* CUIGuildMgr::m_edtGreen=NULL;
CEdit* CUIGuildMgr::m_edtBlue=NULL;
CTextButton*	CUIGuildMgr::m_btnConfCol=NULL;
CTextButton*	CUIGuildMgr::m_btnNext=NULL;
CTextButton*	CUIGuildMgr::m_btnPrev=NULL;
CTextButton*	CUIGuildMgr::m_btnDisCol=NULL;
CImage*	CUIGuildMgr::m_imgPreview=NULL;
CLabel*	CUIGuildMgr::m_gCircleCount=NULL;

CTreeView*	CUIGuildMgr::m_trvPerm=NULL;
string	CUIGuildMgr::attrName="";
int CUIGuildMgr::previewCounter=0;



CCharacter2D* CUIGuildMgr::pMob[3] = {0} ;

int questMobs[3] = {0,0,0} ;

CUIGuildMgr::CUIGuildMgr(void)
{
}

CUIGuildMgr::~CUIGuildMgr(void)
{
}

bool CUIGuildMgr::Init()
{
	FORM_LOADING_CHECK(m_pGuildMgrForm,"manage.clu","frmManage");
	FORM_LOADING_CHECK(m_pGuildColForm,"manage.clu","frmGuildColour");
	FORM_LOADING_CHECK(m_pGuildPermForm,"manage.clu","frmGuildPerm");
	m_pGuildMgrForm->evtBeforeShow=OnBeforeShow;

	FORM_CONTROL_LOADING_CHECK(m_plabGuildName,m_pGuildMgrForm,CLabelEx,"manage.clu","labName");
	FORM_CONTROL_LOADING_CHECK(m_plabGuildMottoName,m_pGuildMgrForm,CLabelEx,"manage.clu","labMaxim");
	//FORM_CONTROL_LOADING_CHECK(m_plabGuildType,m_pGuildMgrForm,CLabelEx,"manage.clu","labState");
	FORM_CONTROL_LOADING_CHECK(m_plabGuildMaster,m_pGuildMgrForm,CLabelEx,"manage.clu","labPeople");
	FORM_CONTROL_LOADING_CHECK(m_plabGuildMemberCount,m_pGuildMgrForm,CLabelEx,"manage.clu","labNum");
	FORM_CONTROL_LOADING_CHECK(m_plabGuildExperience,m_pGuildMgrForm,CLabelEx,"manage.clu","labExp");
	FORM_CONTROL_LOADING_CHECK(m_plabGuildMoney,m_pGuildMgrForm,CLabelEx,"manage.clu","labMen");
	//FORM_CONTROL_LOADING_CHECK(m_plabGuildState,m_pGuildMgrForm,CLabelEx,"manage.clu","labReason");
	//FORM_CONTROL_LOADING_CHECK(m_plabGuildRemainTime,m_pGuildMgrForm,CLabelEx,"manage.clu","labRemain");
	FORM_CONTROL_LOADING_CHECK(m_pbtnGuildMottoEdit,m_pGuildMgrForm,CTextButton,"manage.clu","btnMaxim");
	FORM_CONTROL_LOADING_CHECK(m_pbtnGuildDismiss,m_pGuildMgrForm,CTextButton,"manage.clu","btnSan");
	FORM_CONTROL_LOADING_CHECK(m_pbtnMemberRecruit,m_pGuildMgrForm,CTextButton,"manage.clu","btnYes");
	FORM_CONTROL_LOADING_CHECK(m_pbtnMemberRefuse,m_pGuildMgrForm,CTextButton,"manage.clu","btnNo");
	FORM_CONTROL_LOADING_CHECK(m_pbtnMemberKick,m_pGuildMgrForm,CTextButton,"manage.clu","btnkick");
	FORM_CONTROL_LOADING_CHECK(m_plstGuildMember,m_pGuildMgrForm,CListView,"manage.clu","lstNum");
	FORM_CONTROL_LOADING_CHECK(m_plstRecruitMember,m_pGuildMgrForm,CListView,"manage.clu","lstAsk");
	FORM_CONTROL_LOADING_CHECK(m_ppgeClass,m_pGuildMgrForm,CPage,"manage.clu","pgePublic");
	
	FORM_CONTROL_LOADING_CHECK(m_pbtnMemberYesPerms,m_pGuildPermForm,CTextButton,"manage.clu","btnYesPerm");
	FORM_CONTROL_LOADING_CHECK(m_pbtnMemberPerms,m_pGuildMgrForm,CTextButton,"manage.clu","btnperm");
	FORM_CONTROL_LOADING_CHECK(m_pbtnHouse,m_pGuildMgrForm,CTextButton,"manage.clu","btnHouse");
	FORM_CONTROL_LOADING_CHECK(m_pbtnColour,m_pGuildMgrForm,CTextButton,"manage.clu","btnColour");
	
	FORM_CONTROL_LOADING_CHECK(m_pbtnattrPR,m_pGuildMgrForm,CTextButton,"manage.clu","attrPRBtn");
	FORM_CONTROL_LOADING_CHECK(m_pbtnattrMS,m_pGuildMgrForm,CTextButton,"manage.clu","attrMSBtn");
	FORM_CONTROL_LOADING_CHECK(m_pbtnattrASPD,m_pGuildMgrForm,CTextButton,"manage.clu","attrASPDBtn");
	FORM_CONTROL_LOADING_CHECK(m_pbtnattrMX,m_pGuildMgrForm,CTextButton,"manage.clu","attrMXBtn");
	FORM_CONTROL_LOADING_CHECK(m_pbtnattrDEF,m_pGuildMgrForm,CTextButton,"manage.clu","attrDEFBtn");
	FORM_CONTROL_LOADING_CHECK(m_pbtnattrHIT,m_pGuildMgrForm,CTextButton,"manage.clu","attrHITBtn");
	FORM_CONTROL_LOADING_CHECK(m_pbtnattrFLEE,m_pGuildMgrForm,CTextButton,"manage.clu","attrFLEEBtn");
	FORM_CONTROL_LOADING_CHECK(m_pbtnattrHPR,m_pGuildMgrForm,CTextButton,"manage.clu","attrHPRBtn");
	FORM_CONTROL_LOADING_CHECK(m_pbtnattrSPR,m_pGuildMgrForm,CTextButton,"manage.clu","attrSPRBtn");
	FORM_CONTROL_LOADING_CHECK(m_pbtnattrHP,m_pGuildMgrForm,CTextButton,"manage.clu","attrHPBtn");
	FORM_CONTROL_LOADING_CHECK(m_pbtnattrSP,m_pGuildMgrForm,CTextButton,"manage.clu","attrSPBtn");

	m_pbtnattrPR->evtMouseClick = _GuildStatUpgrade;
	m_pbtnattrMS->evtMouseClick = _GuildStatUpgrade;
	m_pbtnattrASPD->evtMouseClick = _GuildStatUpgrade;	
	m_pbtnattrMX->evtMouseClick = _GuildStatUpgrade;	
	m_pbtnattrDEF->evtMouseClick = _GuildStatUpgrade;	
	m_pbtnattrHIT->evtMouseClick = _GuildStatUpgrade;	
	m_pbtnattrFLEE->evtMouseClick = _GuildStatUpgrade;	
	m_pbtnattrHPR->evtMouseClick = _GuildStatUpgrade;	
	m_pbtnattrSPR->evtMouseClick = _GuildStatUpgrade;	
	m_pbtnattrHP->evtMouseClick = _GuildStatUpgrade;
	m_pbtnattrSP->evtMouseClick = _GuildStatUpgrade;	
	
	m_pbtnGuildMottoEdit->evtMouseClick=_OnClickEditMottoName;
	m_pbtnGuildDismiss->evtMouseClick=_OnClickDismiss;
	m_pbtnMemberRecruit->evtMouseClick=_OnClickRecruit;
	m_pbtnMemberRefuse->evtMouseClick=_OnClickRefuse;
	m_pbtnMemberKick->evtMouseClick=_OnClickKick;
	m_ppgeClass->evtSelectPage=_OnClickSelectPage;
	m_pbtnMemberKick->SetIsEnabled(true);
	m_pbtnMemberRecruit->SetIsEnabled(false);
	m_pbtnMemberRefuse->SetIsEnabled(false);
	m_plabGuildName->SetIsCenter(true);
	m_plabGuildMottoName->SetIsCenter(true);
	//m_plabGuildType->SetIsCenter(true);
	m_plabGuildMaster->SetIsCenter(true);
	m_plabGuildMemberCount->SetIsCenter(true);
	m_plabGuildMoney->SetIsCenter(true);
	m_plabGuildExperience->SetIsCenter(true);
//	m_plabGuildState->SetIsCenter(true);
	
	m_pbtnMemberYesPerms->evtMouseClick=_OnClickConfirmPerm;
	m_pbtnMemberPerms->evtMouseClick=_OnClickPerm;
	m_pbtnHouse->evtMouseClick=_EnterGuildHouse;
	m_pbtnColour->evtMouseClick=_OnClickColour;
	m_pbtnMemberYesPerms->SetIsEnabled(true);
	m_pbtnMemberPerms->SetIsEnabled(true);
	m_pbtnHouse->SetIsEnabled(true);


	FORM_LOADING_CHECK(m_pGuildMottoNameEditForm,"manage.clu","frmEditMaxim");
	FORM_CONTROL_LOADING_CHECK(m_pedtGuildMottoName,m_pGuildMottoNameEditForm,CEdit,"manage.clu","edtMaxim");
	FORM_CONTROL_LOADING_CHECK(m_pbtnGuildMottoFormOK,m_pGuildMottoNameEditForm,CTextButton,"manage.clu","btnYes");
	m_pbtnGuildMottoFormOK->evtMouseClick=_OnClickMottoFormOK;
	
	
	//FORM_CONTROL_LOADING_CHECK(m_pGuildRank,m_pGuildMgrForm,CLabelEx,"manage.clu","labTurn");
	//FORM_CONTROL_LOADING_CHECK(m_pGuildList,m_pGuildMgrForm,CListView,"manage.clu","lstAsk");
	//m_pGuildList->GetList()->evtSelectChange=OnSelectChange;
	
	char buf[255];
	for (int i = 1;i<=3;i++){
		sprintf(buf,"d3dQuest%d",i);
		C3DCompent* p3D = dynamic_cast<C3DCompent*>( m_pGuildMgrForm->Find(buf) );
		p3D ->SetRenderEvent( _MobRenderEvent );
		
		RefreshMobModelPos(i-1);
	}
	
	m_pGuildMgrForm->evtMouseDragEnd = _OnDragEnd;
	
	
	FORM_CONTROL_LOADING_CHECK(m_edtRed,m_pGuildColForm,CEdit,"manage.clu","edtRed");
	FORM_CONTROL_LOADING_CHECK(m_edtGreen,m_pGuildColForm,CEdit,"manage.clu","edtGreen");
	FORM_CONTROL_LOADING_CHECK(m_edtBlue,m_pGuildColForm,CEdit,"manage.clu","edtBlue");
	FORM_CONTROL_LOADING_CHECK(m_btnConfCol,m_pGuildColForm,CTextButton,"manage.clu","btnConfCol");
	FORM_CONTROL_LOADING_CHECK(m_btnNext,m_pGuildColForm,CTextButton,"manage.clu","btnNext");
	FORM_CONTROL_LOADING_CHECK(m_btnPrev,m_pGuildColForm,CTextButton,"manage.clu","btnPrev");
	FORM_CONTROL_LOADING_CHECK(m_btnDisCol,m_pGuildColForm,CTextButton,"manage.clu","btnDisCol");
	FORM_CONTROL_LOADING_CHECK(m_imgPreview,m_pGuildColForm,CImage,"manage.clu","imgPreview");
	FORM_CONTROL_LOADING_CHECK(m_gCircleCount,m_pGuildColForm,CLabel,"manage.clu","gCircleCount");

	m_btnConfCol->evtMouseClick = _OnConfirmColour;
	m_btnDisCol->evtMouseClick = _OnConfirmRemoveColour;
	
	m_edtRed->evtKeyChar = _UpdateColourPreview;
	m_edtGreen->evtKeyChar = _UpdateColourPreview;
	m_edtBlue->evtKeyChar = _UpdateColourPreview;
	
	m_btnNext->evtMouseClick = _OnClickNext;
	m_btnPrev->evtMouseClick = _OnClickPrev;
	//m_edtRed->evtChange = _UpdateColourPreview;
	//m_edtGreen->evtChange = _UpdateColourPreview;
	//m_edtBlue->evtChange = _UpdateColourPreview;
	
	
	m_edtRed->SetIsDigit(true);
	m_edtGreen->SetIsDigit(true);
	m_edtBlue->SetIsDigit(true);
	
	//new permission form
	m_trvPerm = dynamic_cast<CTreeView*>(m_pGuildPermForm->Find("trvEditor"));
    m_trvPerm->evtMouseDown = _OnClickPermText;
	m_trvPerm->SetRowSpace(10);
	m_trvPerm->SetSelectColor(0);
	
	m_pGuildPermForm->evtEntrustMouseEvent = _evtPermFormMouseEvent;
	return true;
}

void CUIGuildMgr::_evtPermFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey){
	string strName = pSender->GetName();
	if(strName == "btnPredef"){
		//toggle all other predef buttons
		char buf[32];
		for(int i = 1;i<=6;i++){
			sprintf(buf,"btnPredef%d",i);
			CTextButton* btn = dynamic_cast<CTextButton*>(m_pGuildPermForm->Find(buf));
			btn->SetIsShow(!btn->GetIsShow());
		}
	}else if(strName == "btnPredef1"){//admin
		SetActivePerm(emGldPermMax);
	}else if(strName == "btnPredef2"){//leader
		SetActivePerm(12029);
	}else if(strName == "btnPredef3"){//architect
		SetActivePerm(3585);
	}else if(strName == "btnPredef4"){//banker
		SetActivePerm(541);
	}else if(strName == "btnPredef5"){//trusted
		SetActivePerm(3613);
	}else if(strName == "btnPredef6"){//player
		SetActivePerm(513);
	}		
}

void CUIGuildMgr::_OnClickPermText(CGuiData *pSender, int x, int y, DWORD key){
	CItemObj* permText = m_trvPerm->GetHitItem(x,y);
	if(permText->GetColor() != 0xFFFF0000){
		permText->SetColor(0xFFFF0000);
	}else{
		permText->SetColor(0xFF00FF00);
	}
}


void CUIGuildMgr::_OnClickConfirmPerm(CGuiData *pSender, int x, int y, DWORD key){
	m_trvPerm->GetScroll()->Reset();
	m_pGuildPermForm->Refresh();
	
	CGuildMemberData* pSelfData=CGuildMembersMgr::GetSelfData();
	int perm = (pSelfData->GetPerm()&emGldPermMgr);
	if ( pSelfData && perm==emGldPermMgr){
		CItemRow *pRow=m_plstGuildMember->GetList()->GetSelectItem();
		if (!pRow){
			CBoxMgr::ShowMsgBox( NULL, g_oLangRec.GetString(600), true );
			return;
		}
		
		CGuildMemberData* pMemberData=static_cast<CGuildMemberData*>(pRow->GetPointer());
		unsigned int perms = 0;
		
		int posX = m_pGuildPermForm->GetX()+30 ;
		int posY = m_pGuildPermForm->GetY()+25   ;
		
		for (int i = 1;i<=emGldPermNum;i++){
			int locY = posY + (i*22);
			CItemObj* permText = m_trvPerm->GetHitItem(posX,locY );
			if(permText && permText->GetColor() == 0xFF00FF00){
				perms += pow(2 ,i-1);
			}
		}

		CS_SetGuildPerms(pMemberData->GetID(),perms);
	}else{
		CBoxMgr::ShowMsgBox( NULL, "You do not have permissions to do this", true );
	}
}

void CUIGuildMgr::SetActivePerm(int perm){
	m_trvPerm->GetScroll()->Reset();
	m_pGuildPermForm->Refresh();
	int posX = m_pGuildPermForm->GetX()+30 ;
	int posY = m_pGuildPermForm->GetY()+25   ;
	for (int i = 1;i<=emGldPermNum;i++){
		int locY = posY + (i*22);
		CItemObj* permText = m_trvPerm->GetHitItem(posX,locY );
		if(permText){
			int permCheck = pow(2 ,i-1);
			if(perm & permCheck){
				permText->SetColor(0xFF00FF00);
			}else{
				permText->SetColor(0xFFFF0000);
			}	
		}
	}
}

void CUIGuildMgr::_OnClickPerm(CGuiData *pSender, int x, int y, DWORD key){	
	m_trvPerm->GetScroll()->Reset();
	m_pGuildPermForm->Refresh();

	CGuildMemberData* pSelfData=CGuildMembersMgr::GetSelfData();
	int perm = (pSelfData->GetPerm()&emGldPermMgr);
	if ( pSelfData && perm==emGldPermMgr){
		CItemRow *pRow=m_plstGuildMember->GetList()->GetSelectItem();
		if (!pRow){
			CBoxMgr::ShowMsgBox( NULL, "No user selected", true );
			return;
		}
				
		CGuildMemberData* pMemberData=static_cast<CGuildMemberData*>(pRow->GetPointer());
		SetActivePerm(pMemberData->GetPerm());
		m_pGuildPermForm->Show();
	}else{
		CBoxMgr::ShowMsgBox( NULL, "You do not have permissions to do this", true );
	}
}

/*void CUIGuildMgr::_OnClickPerm(CGuiData *pSender, int x, int y, DWORD key){
	CGuildMemberData* pSelfData=CGuildMembersMgr::GetSelfData();
	int perm = (pSelfData->GetPerm()&emGldPermMgr);
	if ( pSelfData && perm==emGldPermMgr)
	{
		CItemRow *pRow=m_plstGuildMember->GetList()->GetSelectItem();
		if (!pRow)
		{
			CBoxMgr::ShowMsgBox( NULL, "No user selected", true );
			return;
		}
		
		CForm* form = CFormMgr::s_Mgr.Find( "frmGuildPerm" );
		
		CGuildMemberData* pMemberData=static_cast<CGuildMemberData*>(pRow->GetPointer());
		
		for (int i = 1;i<=emGldPermNum;i++){
			char buf[6];
			sprintf(buf,"perm%d",i);
			CCheckBox	*permission = dynamic_cast<CCheckBox*>(form->Find(buf));
			int permCheck = pow(2 ,i-1);
			permission->SetIsChecked(pMemberData->GetPerm() & permCheck);
		}
		
		form->Show();
		
	}
	else
	{
		CBoxMgr::ShowMsgBox( NULL, "You do not have permissions to do this", true );
	}
}*/

/*
void CUIGuildMgr::_OnClickConfirmPerm(CGuiData *pSender, int x, int y, DWORD key){
	CGuildMemberData* pSelfData=CGuildMembersMgr::GetSelfData();
	int perm = (pSelfData->GetPerm()&emGldPermMgr);
	if ( pSelfData && perm==emGldPermMgr)
	{
		CItemRow *pRow=m_plstGuildMember->GetList()->GetSelectItem();
		if (!pRow)
		{
			CBoxMgr::ShowMsgBox( NULL, g_oLangRec.GetString(600), true );
			return;
		}
		
		CGuildMemberData* pMemberData=static_cast<CGuildMemberData*>(pRow->GetPointer());
		CForm* form = CFormMgr::s_Mgr.Find( "frmGuildPerm" );
		unsigned int perms = 0;
		for (int i = 1;i<=emGldPermNum;i++){
			char buf[6];
			sprintf(buf,"perm%d",i);
			CCheckBox	*permission = dynamic_cast<CCheckBox*>(form->Find(buf));
			if(permission->GetIsChecked()){
				perms += pow(2 ,i-1);
			}
		}
		//dont set perms until server confirms the value.
		//pMemberData->SetPerm(perms);
		CS_SetGuildPerms(pMemberData->GetID(),perms);
	}
	else
	{
		CBoxMgr::ShowMsgBox( NULL, "You do not have permissions to do this", true );
	}
}
*/



void CUIGuildMgr::_OnDragEnd(CGuiData *pSender, int x, int y, DWORD key ){
	for (int i = 0;i<3;i++){
		if(g_stUIGuildMgr.RefreshMobModelPos(i)){
			g_stUIGuildMgr.RefreshMobModel(i);
		}else{
			g_stUIGuildMgr.HideMobModel(i);
		}
	}
}

void CUIGuildMgr::HideMobModel(int index){
	RECT rt;
	rt.left = 0;
	rt.right =  0;
	rt.top =  0;
	rt.bottom =  0;
	pMob[index] = new CCharacter2D;
	pMob[index]->Create( rt );
}


void CUIGuildMgr::RefreshMobModel(int index){	
	if(pMob[index]){
		pMob[index]->LoadCha(questMobs[index]);
	}
}

bool CUIGuildMgr::RefreshMobModelPos(int index){	
	if(m_pGuildMgrForm ->GetLeft()+170 < 0 ||   m_pGuildMgrForm->GetTop()+100+(index)*87 <0
	|| (g_stUISystem.m_sysProp.m_videoProp.bPixel1024 && (m_pGuildMgrForm ->GetLeft()+170+32 > LARGE_RES_X ||  m_pGuildMgrForm->GetTop()+100+index*87+32> 768))
	|| (!g_stUISystem.m_sysProp.m_videoProp.bPixel1024 && (m_pGuildMgrForm ->GetLeft()+170+32 > SMALL_RES_X ||  m_pGuildMgrForm->GetTop()+100+index*87+32 > 600))
	){
		return false;
	}

	RECT rt;
	rt.left = m_pGuildMgrForm ->GetLeft()+170;
	rt.right = rt.left +32;
	rt.top = m_pGuildMgrForm->GetTop()+100+index*87;
	rt.bottom = rt.top+32;
	
	if(!pMob[index]){
		pMob[index] = new CCharacter2D;
	}
	pMob[index]->Create( rt );
	return true;
}

void CUIGuildMgr::_MobRenderEvent(C3DCompent *pSender, int x, int y){
	for(int i = 0;i<3;i++){
		pMob[i]->Render();
	}
}


void CUIGuildMgr::UpdateGuildQuests(LPRPACKET pk){
	char buf[255];
	for(int i = 1;i<=3;i++){
		int mob = pk.ReadLong();
		int required = pk.ReadLong();
		int current = pk.ReadLong();
		int reward  =pk.ReadLong();
		const char* title = pk.ReadString();
		
		sprintf(buf,"labQuest%dTitle",i);
		CLabel* labTitle = dynamic_cast<CLabel*>(m_pGuildMgrForm->Find(buf));
		labTitle->SetCaption(title);
		
		sprintf(buf,"labQuest%dDesc",i);
		CLabel* LabDesc = dynamic_cast<CLabel*>(m_pGuildMgrForm->Find(buf));
		CChaRecord* pRec = GetChaRecordInfo( mob );
		sprintf(buf,"Hunt %d %s. ",required, pRec->szName);
		LabDesc->SetCaption(buf);
		
		sprintf(buf,"labQuest%dReward",i);
		CLabel* LabReward = dynamic_cast<CLabel*>(m_pGuildMgrForm->Find(buf));
		sprintf(buf,"Reward : %d Guild EXP | [%d/%d]",reward,current,required);
		LabReward->SetCaption(buf);
		
		
		if(pMob[i-1]){
			pMob[i-1]->LoadCha(mob);
		}
		
		questMobs[i-1] = mob;
		
	}
}


void CUIGuildMgr::UpdateGuildPoints(int lv, int exp, int point){
	char buf[64];
	UpdateGuildLevel(lv);
	sprintf(buf,"%d",exp);
	m_plabGuildExperience->SetCaption(buf);
	sprintf(buf,"%d",point);
	m_plabGuildMoney->SetCaption(buf);
}

void CUIGuildMgr::UpdateGuildLevel(int lv){
	char buf[64];
	sprintf(buf,"Lv%d",lv);
	CLabel* LabLevel = dynamic_cast<CLabel*>(m_pGuildMgrForm->Find("labLevel"));
	LabLevel->SetCaption(buf);
}

void CUIGuildMgr::UpdateGuildAttr(LPRPACKET pk){
	char buf[64];
	
	int max[] = {5,10,10,20,20,20,20,30,30,100,100};
	int cost[] = {10,5,4,3,3,3,3,2,2,1,1};
	int mul[] = {1,5,5,5,5,5,5,1,1,50,50};
	
	char* btn[] = {"attrPRBtn","attrMSBtn","attrASPDBtn","attrMXBtn","attrDEFBtn","attrHITBtn","attrFLEEBtn","attrHPRBtn","attrSPRBtn","attrHPBtn","attrSPBtn"};
	
	for(int i = 0;i<11;i++){
		int lv = pk.ReadLong();
		
		sprintf(buf,"Val%d",i+1);
		CLabel* labValue = dynamic_cast<CLabel*>(m_pGuildMgrForm->Find(buf));
		
		sprintf(buf,"%d/%d",lv,max[i]);
		labValue->SetCaption(buf);
		
		sprintf(buf,"Total%d",i+1);
		CLabel* labTotal = dynamic_cast<CLabel*>(m_pGuildMgrForm->Find(buf));
		sprintf(buf,"+%d",mul[i]*lv);
		labTotal->SetCaption(buf);
		
		CTextButton* btnAttr = dynamic_cast<CTextButton*>(m_pGuildMgrForm->Find(btn[i]));
		sprintf(buf,"Cost: %d Gold Coins",cost[i]*(lv+1));		
		btnAttr->SetHint(buf);
		
		sprintf(buf,"Attr%d",i+1);
		CLabel* attrName = dynamic_cast<CLabel*>(m_pGuildMgrForm->Find(buf));
		sprintf(buf,"+%d per level",mul[i]);		
		attrName->SetHint(buf);
		
		sprintf(buf,"Max%d",i+1);		
		CLabel* labMax = dynamic_cast<CLabel*>(m_pGuildMgrForm->Find(buf));
		
		CGuildMemberData* pSelfData=CGuildMembersMgr::GetSelfData();
		int perm = (pSelfData->GetPerm()&emGldPermAttr);
	
		
		if(lv >= max[i]){
			labMax->SetIsShow(true);		
			btnAttr->SetIsShow(false);
		}else if ( pSelfData && perm==emGldPermAttr){
			labMax->SetIsShow(false);	
			btnAttr->SetIsShow(true);		
		}else{
			labMax->SetIsShow(false);	
			btnAttr->SetIsShow(false);
		}
		
	}
}

void CUIGuildMgr::_GuildStatUpgradeConfirm(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey){
	if( nMsgType!=CForm::mrYes ) {
		return;
	}
	int AttrID = 0;
	if( attrName ==  "attrPRBtn"){
		return CP_GuildAttr(ATTR_PDEF);
	}
	else if( attrName ==  "attrMSBtn"){
		return CP_GuildAttr(ATTR_MSPD);
	}
	else if( attrName ==  "attrASPDBtn"){
		return CP_GuildAttr(ATTR_ASPD);
	}
	else if( attrName ==  "attrMXBtn"){
		return CP_GuildAttr(ATTR_MXATK);
	}
	else if( attrName ==  "attrDEFBtn"){
		return CP_GuildAttr(ATTR_DEF);
	}
	else if( attrName ==  "attrHITBtn"){
		return CP_GuildAttr(ATTR_HIT);
	}
	else if( attrName ==  "attrFLEEBtn"){
		return CP_GuildAttr(ATTR_FLEE);
	}
	else if( attrName ==  "attrHPRBtn"){
		return CP_GuildAttr(ATTR_HREC);
	}
	else if( attrName ==  "attrSPRBtn"){
		return CP_GuildAttr(ATTR_SREC);
	}
	else if( attrName ==  "attrHPBtn"){
		return CP_GuildAttr(ATTR_MXHP);
	}
	else if( attrName ==  "attrSPBtn"){
		return CP_GuildAttr(ATTR_MXSP);
	}	
}

void CUIGuildMgr::_GuildStatUpgrade(CGuiData *pSender, int x, int y, DWORD key){
	char msg[64];
	attrName = pSender->GetName();
	sprintf(msg,"Are you sure?\n%s",pSender->GetHint().c_str());
	g_stUIBox.ShowSelectBox(_GuildStatUpgradeConfirm,msg);
}


void CUIGuildMgr::ShowForm()
{
	RefreshForm();
	m_pGuildMgrForm->nTag = 1;		// nTag==0时用于快捷键不显示
	m_pGuildMgrForm->Show();
}

void CUIGuildMgr::_OnClickEditMottoName(CGuiData *pSender, int x, int y, DWORD key)
{
	CGuildMemberData* pSelfData=CGuildMembersMgr::GetSelfData();
	int perm = (pSelfData->GetPerm()&emGldPermMotto);
	if ( pSelfData && perm==emGldPermMotto)
	{
		m_pedtGuildMottoName->SetCaption(CGuildData::GetGuildMottoName().c_str());
		m_pGuildMottoNameEditForm->ShowModal();
	}
	else
	{
		CBoxMgr::ShowMsgBox( NULL, g_oLangRec.GetString(594), true );
	}
}


void CUIGuildMgr::_OnClickDismiss(CGuiData *pSender, int x, int y, DWORD key)
{
	CGuildMemberData* pSelfData=CGuildMembersMgr::GetSelfData();
	
	if ( pSelfData && pSelfData->GetID() == CGuildData::GetGuildMasterID())
	{
		CBoxMgr::ShowPasswordBox(_OnPassDismiss, "Disband Guild");
	}
	else
	{
		CBoxMgr::ShowSelectBox( _OnClickLeave, g_oLangRec.GetString(596), true );
	}
}

void CUIGuildMgr::_OnClickRecruit(CGuiData *pSender, int x, int y, DWORD key)
{
	CGuildMemberData* pSelfData=CGuildMembersMgr::GetSelfData();
	int perm = (pSelfData->GetPerm()&emGldPermRecruit);
	if ( pSelfData && perm==emGldPermRecruit)
	{
		CItemRow *pRow=m_plstRecruitMember->GetList()->GetSelectItem();
		if (!pRow)
		{
			CBoxMgr::ShowMsgBox( NULL, g_oLangRec.GetString(597), true );
			return;
		}
		CRecruitMemberData* pMemberData=static_cast<CRecruitMemberData*>(pRow->GetPointer());
		CM_GUILD_APPROVE(pMemberData->GetID());
		m_plstRecruitMember->GetList()->Del(pRow);
		CRecruitMembersMgr::DelRecruitMember(pMemberData);
	}
	else
	{
		CBoxMgr::ShowMsgBox( NULL, g_oLangRec.GetString(598), true );
	}
}

void CUIGuildMgr::_OnClickRefuse(CGuiData *pSender, int x, int y, DWORD key)
{
	CGuildMemberData* pSelfData=CGuildMembersMgr::GetSelfData();
	int perm = (pSelfData->GetPerm()&emGldPermRecruit);
	if ( pSelfData && perm==emGldPermRecruit)
	{
		CItemRow *pRow=m_plstRecruitMember->GetList()->GetSelectItem();
		if (!pRow)
		{
			CBoxMgr::ShowMsgBox( NULL, g_oLangRec.GetString(597), true );
			return;
		}
		CRecruitMemberData* pMemberData=static_cast<CRecruitMemberData*>(pRow->GetPointer());
		CM_GUILD_REJECT(pMemberData->GetID());
		m_plstRecruitMember->GetList()->Del(pRow);
		CRecruitMembersMgr::DelRecruitMember(pMemberData);
	}
	else
	{
		CBoxMgr::ShowMsgBox( NULL, g_oLangRec.GetString(599), true );
	}
}

void CUIGuildMgr::_OnClickKick(CGuiData *pSender, int x, int y, DWORD key)
{
	CGuildMemberData* pSelfData=CGuildMembersMgr::GetSelfData();
	int perm = (pSelfData->GetPerm()&emGldPermKick);
	if ( pSelfData && perm==emGldPermKick)
	{
		CItemRow *pRow=m_plstGuildMember->GetList()->GetSelectItem();
		if (!pRow)
		{
			CBoxMgr::ShowMsgBox( NULL, g_oLangRec.GetString(600), true );
			return;
		}
		CGuildMemberData* pMemberData=static_cast<CGuildMemberData*>(pRow->GetPointer());
		string str=g_oLangRec.GetString(601)+pMemberData->GetName()+g_oLangRec.GetString(602);
		CBoxMgr::ShowSelectBox(_OnPassKick,str.c_str(),true);
	}
	else
	{
		CBoxMgr::ShowMsgBox( NULL, g_oLangRec.GetString(603), true );
	}
}


bool CUIGuildMgr::_UpdateColourPreview(CGuiData *pSender,char&key){
	CEdit* changedValue  = dynamic_cast<CEdit*>(pSender);
	char old[5];
	char buf[5];
	sprintf(old,"%s",changedValue->GetCaption());
	if(key == 0x8){
		
		int value = atoi(changedValue->GetCaption());
		sprintf(old,"%d",value);
		
		int i = 0;
		for(i = 0;i<3;i++){
			if(old[i] <0x30 || old[i] >0x39){
				old[i] = '\0';
				break;
			}
		}
		
		sprintf(buf,"%s",old);
		buf[i-1] = '\0';
		changedValue->SetCaption(buf);
	}else if(key>=0x30 && key<=0x39){
		
		for(int i = 0;i<3;i++){
			if(old[i] <0x30 || old[i] >0x39){
				old[i] = '\0';
			}
		}
		changedValue->SetCaption(old);
		
		if(strlen(changedValue->GetCaption()) >= 3){
			return false;
		}
		int value = min(255,atoi(changedValue->GetCaption())*10+(key-0x30));
		sprintf(old,"%d",value);
		for(int i = 0;i<3;i++){
			if(old[i] <0x30 || old[i] >0x39){
				old[i] = '\20';
			}
		}
		old[3] = '\0';
		changedValue->SetCaption(old);
	}else{
		int value = atoi(changedValue->GetCaption());
		sprintf(old,"%d",value);
		int len = strlen(old);
		
		for(int i = 0;i<3;i++){
			if(old[i] <0x30 || old[i] >0x39){
				old[i] = '\20';
			}
		}
		old[3] = '\0';
		changedValue->SetCaption(old);
	}
	
	int red = min(255,atoi(m_edtRed->GetCaption()));
	int green = min(255,atoi(m_edtGreen->GetCaption()));
	int blue = min(255,atoi(m_edtBlue->GetCaption()));

	changedValue->SetCaption(old);

	m_imgPreview->GetImage()->TintColour(red,green, blue );
	return false;
}

void CUIGuildMgr::_OnConfirmRemoveColour(CGuiData *pSender, int x, int y, DWORD key){
	CProCirculateCS* proCir = (CProCirculateCS *)g_NetIF->GetProCir();
	
	int red = atoi(m_edtRed->GetCaption());
	int green = atoi(m_edtGreen->GetCaption());
	int blue = atoi(m_edtBlue->GetCaption());
	proCir->GuildColour(D3DCOLOR_ARGB(0,red,green,blue),0);
}

void CUIGuildMgr::_OnClickNext(CGuiData *pSender, int x, int y, DWORD key){
	previewCounter++;
	int max =  atoi(m_gCircleCount->GetCaption());
	if(previewCounter >max){
		previewCounter = 0;
	}
	_UpdateCirclePreview();
}

void CUIGuildMgr::_UpdateCirclePreview(){
	char buf[64] ;
	sprintf(buf,"gCircle%d",previewCounter);
	CLabel* circle = dynamic_cast<CLabel*>(m_pGuildColForm->Find(buf));
	sprintf(buf,"texture/effect/guild_%s.tga",circle->GetCaption());
	m_imgPreview->GetImage()->LoadImage(buf,128,128,0,0,0);
}

void CUIGuildMgr::_OnClickPrev(CGuiData *pSender, int x, int y, DWORD key){
	previewCounter--;
	int max =  atoi(m_gCircleCount->GetCaption());
	if(previewCounter < 0){
		previewCounter = max;
	}
	_UpdateCirclePreview();
}

void CUIGuildMgr::_OnConfirmColour(CGuiData *pSender, int x, int y, DWORD key){
	int red = atoi(m_edtRed->GetCaption());
	int green = atoi(m_edtGreen->GetCaption());
	int blue = atoi(m_edtBlue->GetCaption());
	CProCirculateCS* proCir = (CProCirculateCS *)g_NetIF->GetProCir();
	char buf[64] ;
	sprintf(buf,"gCircleID%d",previewCounter);
	CLabel* circle = dynamic_cast<CLabel*>(m_pGuildColForm->Find(buf));
	int ID = atoi(circle->GetCaption());
	proCir->GuildColour(D3DCOLOR_ARGB(0,red,green,blue),ID);
}


void CUIGuildMgr::_OnClickColour(CGuiData *pSender, int x, int y, DWORD key){
	CCharacter* pCha = g_stUIBoat.GetHuman();
	int colour = pCha->getGuildCircleColour();
	
	int red = (colour&0xFF0000)/65536;
	int green = (colour&0xFF00)/256;
	int blue = colour&0xFF;
	
	char buf[3];
	sprintf(buf,"%d",red);
	m_edtRed->SetCaption(buf);
	sprintf(buf,"%d",green);
	m_edtGreen->SetCaption(buf);
	sprintf(buf,"%d",blue);
	m_edtBlue->SetCaption(buf);
	m_imgPreview->GetImage()->TintColour(red,green, blue );	
	int max =  atoi(m_gCircleCount->GetCaption());
	for(int i =0;i<=max;i++){
		char buf[64] ;
		sprintf(buf,"gCircleID%d",i);
		CLabel* circle = dynamic_cast<CLabel*>(m_pGuildColForm->Find(buf));
		int ID = atoi(circle->GetCaption());
		if (ID == pCha->getGuildIcon()){
			previewCounter = i;
			break;
		}
	}
	_UpdateCirclePreview();
	m_pGuildColForm->SetIsShow(true);
}

void CUIGuildMgr::_EnterGuildHouse(CGuiData *pSender, int x, int y, DWORD key){
	CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_ENTERGUILDHOUSE, NULL); 
}

void CUIGuildMgr::_OnClickMottoFormOK(CGuiData *pSender, int x, int y, DWORD key)
{
	string name=m_pedtGuildMottoName->GetCaption();
	if (!CTextFilter::IsLegalText(CTextFilter::NAME_TABLE, name)
		|| !IsValidName(name.c_str(), (unsigned short)name.length())
		)
	{
		g_pGameApp->MsgBox(g_oLangRec.GetString(51));
		return;
	}
	CM_GUILD_MOTTO(name.c_str());
	m_pGuildMottoNameEditForm->Hide();
}

void CUIGuildMgr::RefreshForm()
{
	RefreshAttribute();
	RefreshList();
}

void CUIGuildMgr::RefreshAttribute()
{
	char buf[50];
	m_plabGuildName->SetCaption(CGuildData::GetGuildName().c_str());
	m_plabGuildMaster->SetCaption(CGuildData::GetGuildMasterName().c_str());
	m_plabGuildMottoName->SetCaption(CGuildData::GetGuildMottoName().c_str());
	//m_plabGuildType->SetCaption((CGuildData::GetGuildType()==CGuildData::navy)?g_oLangRec.GetString(604):g_oLangRec.GetString(605));
	sprintf(buf,"%d/%d",CGuildData::GetMemberCount(),CGuildData::GetMaxMembers());
	m_plabGuildMemberCount->SetCaption(buf);
	_i64toa(CGuildData::GetExperence(),buf,10);
	m_plabGuildMoney->SetCaption(buf);
	_i64toa(CGuildData::GetMoney(),buf,10);
	m_plabGuildExperience->SetCaption(buf);
	CGuildData::eState state=CGuildData::GetGuildState();
	string strState;
	if (state==CGuildData::normal)
	{
		strState=g_oLangRec.GetString(606);
		//m_plabGuildState->SetCaption(strState.c_str());
		//m_plabGuildRemainTime->SetCaption("");
	}
	else
	{
		strState=g_oLangRec.GetString(607);
		if (state&CGuildData::money)
		{
			strState+=g_oLangRec.GetString(608);
		}
		if (state&CGuildData::repute)
		{
			strState+=g_oLangRec.GetString(609);
		}
		if (state&CGuildData::member)
		{
			strState+=g_oLangRec.GetString(610);
		}
		//m_plabGuildState->SetCaption(strState.c_str());
		__int64 remain=CGuildData::GetRemainTime();
		if (remain>1440)
		{
			sprintf(buf,g_oLangRec.GetString(611),remain/1440);
		}
		else if (remain>60)
		{
			sprintf(buf,g_oLangRec.GetString(612),remain/60);
		}
		else
		{
			sprintf(buf,g_oLangRec.GetString(613),remain);
		}
	//	m_plabGuildRemainTime->SetCaption(buf);
	}

	CGuildMemberData* pSelfData=CGuildMembersMgr::GetSelfData();
	//	Modify by alfred
	
	if ( pSelfData && pSelfData->GetID() == CGuildData::GetGuildMasterID()){
		m_pbtnGuildDismiss->GetImage()->LoadImage("texture/ui/SysBotton4.tga",41,19,0,0,76);
		m_pbtnGuildDismiss->GetImage()->LoadImage("texture/ui/SysBotton4.tga",41,19,1,0+41,76);
		m_pbtnGuildDismiss->GetImage()->LoadImage("texture/ui/SysBotton4.tga",41,19,2,0+82,76);
	}
	else //41, 19, 0, 76
	{
		m_pbtnGuildDismiss->GetImage()->LoadImage("texture/ui/SystemBotton3.tga",41,19,0,0,152);
		m_pbtnGuildDismiss->GetImage()->LoadImage("texture/ui/SystemBotton3.tga",41,19,1,0+41,152);
		m_pbtnGuildDismiss->GetImage()->LoadImage("texture/ui/SystemBotton3.tga",41,19,2,0+82,152);
	}
	m_pGuildMgrForm->Refresh();
}

void CUIGuildMgr::RefreshList()
{
	char buf[50];
	m_plstGuildMember->GetList()->GetItems()->Clear();
	for (DWORD i=0;i<CGuildMembersMgr::GetTotalGuildMembers();i++)
	{
		CGuildMemberData* pMemberData=CGuildMembersMgr::FindGuildMemberByIndex(i);
		if (!pMemberData) continue;
		CItemRow *pRow=m_plstGuildMember->GetList()->NewItem();
		CItem*	pMemberNameItem=new CItem(pMemberData->GetName().c_str(),COLOR_BLACK);
		CItem*	pMemberJobItem=new CItem(pMemberData->GetJob().c_str(),COLOR_BLACK);
		CItem*	pMemberLevelItem=new CItem(_i64toa(pMemberData->GetLevel(),buf,10),COLOR_BLACK);
		pRow->SetIndex(0,pMemberNameItem);
		pRow->SetIndex(1,pMemberJobItem);
		pRow->SetIndex(2,pMemberLevelItem);
		pRow->SetPointer(pMemberData);
	}

	m_plstRecruitMember->GetList()->GetItems()->Clear();
	for (DWORD i=0;i<CRecruitMembersMgr::GetTotalRecruitMembers();i++)
	{
		CRecruitMemberData* pMemberData=CRecruitMembersMgr::FindRecruitMemberByIndex(i);
		if (!pMemberData) continue;
		CItemRow *pRow=m_plstRecruitMember->GetList()->NewItem();
		CItem*	pMemberNameItem=new CItem(pMemberData->GetName().c_str(),COLOR_BLACK);
		CItem*	pMemberJobItem=new CItem(pMemberData->GetJob().c_str(),COLOR_BLACK);
		CItem*	pMemberLevelItem=new CItem(_i64toa(pMemberData->GetLevel(),buf,10),COLOR_BLACK);
		pRow->SetIndex(0,pMemberNameItem);
		pRow->SetIndex(1,pMemberJobItem);
		pRow->SetIndex(2,pMemberLevelItem);
		pRow->SetPointer(pMemberData);
	}
	m_pGuildMgrForm->Refresh();
}

void CUIGuildMgr::_OnClickSelectPage(CGuiData *pSender)
{
	int n=m_ppgeClass->GetIndex();
	if (n==0)	//成员列表
	{
		m_pbtnMemberKick->SetIsEnabled(true);
		m_pbtnMemberRecruit->SetIsEnabled(false);
		m_pbtnMemberRefuse->SetIsEnabled(false);

		CTextButton* chkSortName = dynamic_cast<CTextButton*>(m_pGuildMgrForm->Find("chkSortName"));
		CTextButton* chkSortClass = dynamic_cast<CTextButton*>(m_pGuildMgrForm->Find("chkSortClass"));
		CTextButton* chkSortLevel = dynamic_cast<CTextButton*>(m_pGuildMgrForm->Find("chkSortLevel"));
	}
	else if (n==1)	//申请人列表
	{
		m_pbtnMemberRecruit->SetIsEnabled(true);
		m_pbtnMemberRefuse->SetIsEnabled(true);
		m_pbtnMemberKick->SetIsEnabled(false);
	}
	else if (n==2)	//申请人列表
	{
		//guildbank
		
		m_pbtnMemberRecruit->SetIsEnabled(false);
		m_pbtnMemberRefuse->SetIsEnabled(false);
		m_pbtnMemberKick->SetIsEnabled(false);
		
		
		CGuildMemberData* pSelfData=CGuildMembersMgr::GetSelfData();
		CCompent* bankLocked = dynamic_cast<CCompent*>(m_pGuildMgrForm->Find("bankLocked"));
		CTextButton* btnGoldTake = dynamic_cast<CTextButton*>(m_pGuildMgrForm->Find("btngoldtake"));
		CTextButton* btnGoldPut = dynamic_cast<CTextButton*>(m_pGuildMgrForm->Find("btngoldput"));
		
		
		int perm = (pSelfData->GetPerm()&emGldPermViewBank);
		if(perm == emGldPermViewBank){
			bankLocked->SetIsShow(false);
			btnGoldTake->SetIsShow(true);
			btnGoldPut->SetIsShow(true);
			CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_REQUESTGUILDBANK, NULL); 
		}else{
			bankLocked->SetIsShow(true);
			btnGoldTake->SetIsShow(false);
			btnGoldPut->SetIsShow(false);
		}
	}
	else if (n==3){ //guild stats form
		CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_REQUESTGUILDATTR, NULL); 
	}
	else if (n==4){ //guild quest form
		CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_REQUESTGUILDQUEST, NULL); 
	}
}

void CUIGuildMgr::RemoveForm()
{
	m_pGuildMgrForm->Close();
}

void CUIGuildMgr::_OnPassDismiss(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if( nMsgType!=CForm::mrYes ) return;
	stPasswordBox* pBox = (stPasswordBox*)pSender->GetForm()->GetPointer();
	if( !pBox ) return;
	string str=pBox->edtPassword->GetCaption();
	if (str.length()>0)
	{
		CM_GUILD_DISBAND(str.c_str());
	}
}

void CUIGuildMgr::_OnClickLeave(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if( nMsgType==CForm::mrYes )
	{
		CM_GUILD_LEAVE();
	}
}

void CUIGuildMgr::_OnPassKick(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if( nMsgType!=CForm::mrYes ) return;
	CItemRow *pRow=m_plstGuildMember->GetList()->GetSelectItem();
	if (!pRow)
	{
		CBoxMgr::ShowMsgBox( NULL, g_oLangRec.GetString(614), true );
		return;
	}
	CGuildMemberData* pMemberData=static_cast<CGuildMemberData*>(pRow->GetPointer());
	CM_GUILD_KICK(pMemberData->GetID());
}

void CUIGuildMgr::OnBeforeShow(CForm* pForm,bool& IsShow)
{
	if( !pForm->nTag )
	{
		CM_GUILD_LISTTRYPLAYER();
		IsShow=false;
	}
	pForm->nTag=0;
}
