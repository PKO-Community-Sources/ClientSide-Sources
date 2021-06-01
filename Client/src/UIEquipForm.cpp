#include "StdAfx.h"
#include "uiequipform.h"
#include "uiform.h"
#include "uiskilllist.h"
#include "packetcmd.h"
#include "Scene.h"
#include "Character.h"
#include "uifastcommand.h"
#include "uigoodsgrid.h"
#include "NetProtocol.h"
#include "gameapp.h"
#include "uiitemcommand.h"
#include "uilabel.h"
#include "tools.h"
#include "uitradeform.h"
#include "uiboxform.h"
#include "packetcmd.h"
#include "netprotocol.h"
#include "uiboxform.h"
#include "uiboatform.h"
#include "chastate.h"
#include "SkillStateRecord.h"
#include "ProCirculate.h"
#include "stpose.h"
#include "uiboxform.h"
#include "StringLib.h"
#include "UICheckBox.h"
#include "UIDoublePwdForm.h"
#include "UIStoreForm.h"

#include "GlobalVar.h"

using namespace GUI;

static char szBuf[32] = { 0 };

CGuiPic	CEquipMgr::_imgCharges[enumEQUIP_NUM];
CEquipMgr::SSplitItem CEquipMgr::SSplit;
int CEquipMgr::lIMP =0;


extern CDoublePwdMgr g_stUIDoublePwd;

//---------------------------------------------------------------------------
// class CMainMgr
//---------------------------------------------------------------------------
bool CEquipMgr::Init()
{
	CForm *frmMain800 = _FindForm("frmMain800");

	///////////技能系列 
	frmSkill = _FindForm("frmSkill");
	if(!frmSkill) return false;
	frmSkill->evtShow = _evtSkillFormShow;

    lstFightSkill = dynamic_cast<CSkillList*>( frmSkill->Find("lstSkill") );
    if( !lstFightSkill ) return Error( g_oLangRec.GetString(45), frmSkill->GetName(), "lstSkill" );
	lstFightSkill->evtUpgrade = _evtSkillUpgrade;

    lstLifeSkill = dynamic_cast<CSkillList*>( frmSkill->Find("lstSkillW") );
    if( !lstLifeSkill ) return Error( g_oLangRec.GetString(45), frmSkill->GetName(), "lstSkillW" );
	lstLifeSkill->evtUpgrade = _evtSkillUpgrade;

    lstSailSkill = dynamic_cast<CSkillList*>( frmSkill->Find("lstSkillS") );
    if( !lstSailSkill ) return Error( g_oLangRec.GetString(45), frmSkill->GetName(), "lstSkillS" );
	lstSailSkill->evtUpgrade = _evtSkillUpgrade;
	lstSailSkill->SetIsShowUpgrade(false);

    labPoint = dynamic_cast<CLabel *>(frmSkill->Find("labPoint"));
    if (!labPoint) return Error( g_oLangRec.GetString(45), frmSkill->GetName(), "labPoint");

    labPointLife = dynamic_cast<CLabel *>(frmSkill->Find("labPoint1"));
    if (!labPointLife) return Error( g_oLangRec.GetString(45), frmSkill->GetName(), "labPoint1");

	frmInv = _FindForm("frmInv");
	
	if( !frmInv ) 		return false;  
	frmInv->evtShow = _evtEquipFormShow;   
	frmInv->evtClose = _evtEquipFormClose;
	frmInv->evtEntrustMouseEvent = _evtItemFormMouseEvent;
	
	grdItem = dynamic_cast<CGoodsGrid*>(frmInv->Find("grdItem"));
	if( !grdItem ) return Error( g_oLangRec.GetString(45), frmInv->GetName(), "grdItem" );

	lblGold = dynamic_cast<CLabel*>(frmInv->Find("labItemgoldnumber"));
	if( !lblGold ) return Error( g_oLangRec.GetString(45), frmInv->GetName(), "labItemgoldnumber" );
	
    GetGoodsGrid()->evtThrowItem = evtThrowItemEvent;
    GetGoodsGrid()->evtSwapItem = evtSwapItemEvent;
    GetGoodsGrid()->evtBeforeAccept = _evtDragToGoodsEvent;
	GetGoodsGrid()->evtRMouseEvent = _evtRMouseGridEvent;

	imgLock = dynamic_cast<CImage*>(frmInv->Find("imgLock"));
	if(! imgLock)return Error( g_oLangRec.GetString(45), frmInv->GetName(), "imgLock" );

	imgUnLock = dynamic_cast<CImage*>(frmInv->Find("imgUnLock"));
	if(! imgUnLock)return Error( g_oLangRec.GetString(45), frmInv->GetName(), "imgUnLock" );

	SetIsLock(false);	// 默认不锁定


	///////////////快捷键系列
	CForm* frmFast = _FindForm("frmFast");
	if( !frmFast ) return false;

	// 上下换页按钮
    CTextButton* btnFastUp = dynamic_cast<CTextButton*>( frmFast->Find("btnFastUp") );
    if( !btnFastUp ) return Error( g_oLangRec.GetString(45), frmMain800->GetName(), "btnFastUp" );
	btnFastUp->evtMouseClick = _evtButtonClickEvent;

    CTextButton* btnFastDown = dynamic_cast<CTextButton*>( frmFast->Find("btnFastDown") );
    if( !btnFastUp ) return Error( g_oLangRec.GetString(45), frmMain800->GetName(), "btnFastDown" );
	btnFastDown->evtMouseClick = _evtButtonClickEvent;

	CForm* frmFast2 = _FindForm("frmFast2");
	if( !frmFast2 ) return false;

	_pFastCommands = new CFastCommand*[ SHORT_CUT_NUM ];   
    memset( _pFastCommands, 0, sizeof(CFastCommand*)*SHORT_CUT_NUM );
    char szName[30] = { 0 };

	for( DWORD j=0; j<SHORT_CUT_NUM; j++ )
    {
		if (j >= 0 && j < MAX_FAST_COL)
		{
			sprintf( szName, "fscMainF%d", j );
			_pFastCommands[ j ] = dynamic_cast<CFastCommand*>(frmFast->Find(szName));
			if( _pFastCommands[ j ] )
			{
				_pFastCommands[ j ]->nTag = j;
				_pFastCommands[ j ]->evtChange = _evtFastChange;
			}
		}
		else if (j >= MAX_FAST_COL*2)
		{
			sprintf( szName, "fscMainF2%d", j -12);
			_pFastCommands[ j ] = dynamic_cast<CFastCommand*>(frmFast2->Find(szName));
			if( _pFastCommands[ j ] )
			{
				_pFastCommands[ j ]->nTag = j;
				_pFastCommands[ j ]->evtChange = _evtFastChange;
				_pFastCommands[ j]->topBar = true;
			}
		}
    }

    int nIndex = 0;
    for( DWORD i=1; i<MAX_FAST_ROW-1; i++ ){
        for( DWORD j=0; j<MAX_FAST_COL; j++ ){
          
                nIndex = i * MAX_FAST_COL + j;
                _pFastCommands[ nIndex ] = new CFastCommand( *_pFastCommands[j] );
				
				_pFastCommands[ nIndex ]->AddForm();
				
                _pFastCommands[ nIndex ]->nTag = nIndex;
				_pFastCommands[ nIndex]->evtChange = _evtFastChange;
			
		}
	}
	

    
	//_pActiveFastLabel = dynamic_cast<CLabel*>(frmMain800->Find( "labFast" ) );
    _ActiveFast( 0 );

	COneCommand* cmdLost = dynamic_cast<COneCommand*>(frmInv->Find("cmdLost"));
    cmdLost->evtBeforeAccept = _evtEquipEvent;

	/////////////// 装备栏
	
	memset( cnmEquip, 0, sizeof(cnmEquip) );
	cnmEquip[enumEQUIP_HEAD] = dynamic_cast<COneCommand*>(frmInv->Find("cmdArmet"));
	cnmEquip[enumEQUIP_BODY] = dynamic_cast<COneCommand*>(frmInv->Find("cmdBody"));
	cnmEquip[enumEQUIP_GLOVE] = dynamic_cast<COneCommand*>(frmInv->Find("cmdGlove"));
	cnmEquip[enumEQUIP_SHOES] = dynamic_cast<COneCommand*>(frmInv->Find("cmdShoes"));
	cnmEquip[enumEQUIP_LHAND] = dynamic_cast<COneCommand*>(frmInv->Find("cmdLeftHand"));
	cnmEquip[enumEQUIP_RHAND] = dynamic_cast<COneCommand*>(frmInv->Find("cmdRightHand"));
	cnmEquip[enumEQUIP_NECK] = dynamic_cast<COneCommand*>(frmInv->Find("cmdNecklace"));
	cnmEquip[enumEQUIP_HAND1] = dynamic_cast<COneCommand*>(frmInv->Find("cmdCirclet1"));
	cnmEquip[enumEQUIP_HAND2] = dynamic_cast<COneCommand*>(frmInv->Find("cmdCirclet2"));
	cnmEquip[enumEQUIP_Jewelry1] = dynamic_cast<COneCommand*>(frmInv->Find("cmdJewelry1"));
	cnmEquip[enumEQUIP_Jewelry2] = dynamic_cast<COneCommand*>(frmInv->Find("cmdJewelry2"));
	cnmEquip[enumEQUIP_Jewelry3] = dynamic_cast<COneCommand*>(frmInv->Find("cmdJewelry3"));
	cnmEquip[enumEQUIP_Jewelry4] = dynamic_cast<COneCommand*>(frmInv->Find("cmdJewelry4"));
	cnmEquip[enumEQUIP_WING] = dynamic_cast<COneCommand*>(frmInv->Find("cmdWing"));
	
	cnmEquip[enumEQUIP_CLOAK] = dynamic_cast<COneCommand*>(frmInv->Find("cmdCloak"));
	cnmEquip[enumEQUIP_FAIRY] = dynamic_cast<COneCommand*>(frmInv->Find("cmdPet"));
	cnmEquip[enumEQUIP_REAR	] = dynamic_cast<COneCommand*>(frmInv->Find("cmdRearPet"));
	cnmEquip[enumEQUIP_MOUNT] = dynamic_cast<COneCommand*>(frmInv->Find("cmdMount"));
	
	
	cnmEquip[enumEQUIP_HEADAPP ] = dynamic_cast<COneCommand*>(frmInv->Find("cmdArmetApp"));
	cnmEquip[enumEQUIP_FACEAPP ] = dynamic_cast<COneCommand*>(frmInv->Find("cmdFaceApp"));
	cnmEquip[enumEQUIP_BODYAPP ] = dynamic_cast<COneCommand*>(frmInv->Find("cmdBodyApp"));
	cnmEquip[enumEQUIP_GLOVEAPP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdGloveApp"));
	cnmEquip[enumEQUIP_SHOESAPP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdShoesApp"));
	cnmEquip[enumEQUIP_FAIRYAPP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdPetApp"));
	cnmEquip[enumEQUIP_GLOWAPP ] = dynamic_cast<COneCommand*>(frmInv->Find("cmdGlowApp"));
	cnmEquip[enumEQUIP_DAGGERAPP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdDaggerApp"));
	cnmEquip[enumEQUIP_GUNAPP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdGunApp"));
	cnmEquip[enumEQUIP_SWORD1APP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdSword1App"));
	cnmEquip[enumEQUIP_GREATSWORDAPP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdGreatSwordApp"));
	cnmEquip[enumEQUIP_STAFFAPP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdStaffApp"));
	cnmEquip[enumEQUIP_BOWAPP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdBowApp"));
	cnmEquip[enumEQUIP_SWORD2APP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdSword2App"));
	cnmEquip[enumEQUIP_SHIELDAPP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdShieldApp"));

	for( int i=0; i<enumEQUIP_NUM; i++ )
    {
        if( cnmEquip[i] ) 
        {
            cnmEquip[i]->evtBeforeAccept = _evtEquipEvent;
            //cnmEquip[i]->evtMouseClick = _UnequipPart;
			//cnmEquip[i]->evtThrowItem = _evtThrowEquipEvent;	// 屏蔽直接将装备丢弃

			cnmEquip[i]->SetActivePic( &_imgCharges[i] );
        }
    }

	int nTotalSkill = CSkillRecordSet::I()->GetLastID() + 1;
	CSkillRecord *pInfo;
	for( int i=0; i<nTotalSkill; i++ )
	{
		pInfo=GetSkillRecordInfo( i );
		if( pInfo )
		{
			if( pInfo->nStateID ) _cancels.push_back( pInfo );
		}
	}

	int nTotalState = CSkillStateRecordSet::I()->GetLastID() + 1;
	CSkillStateRecord* pState;
	for( int i=0; i<nTotalState; i++ )
	{
		pState = GetCSkillStateRecordInfo( i );
		if( pState )
		{
			if( pState->sChargeLink>=0 && pState->sChargeLink<enumEQUIP_NUM )
			{
				_charges.push_back( pState );
			}
		}
	}
	
	frmItemSpy = _FindForm("frmItemSpy");   // 道具表单
	frmItemSpy->evtShow = _evtSpyFormShow;
	frmItemSpy->evtClose = _evtSpyFormClose;
	if( !frmItemSpy ) 		return false;  
	//frmItemSpy->evtShow = _evtItemFormShow;   
	//frmItemSpy->evtClose = _evtItemFormClose;
	//frmItemSpy->evtEntrustMouseEvent = _evtItemFormMouseEvent;
	
	/////////////// 装备栏
	memset( cnmEquipSpy, 0, sizeof(cnmEquipSpy) );
	cnmEquipSpy[enumEQUIP_HEAD] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdArmet"));
	cnmEquipSpy[enumEQUIP_BODY] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdBody"));
	cnmEquipSpy[enumEQUIP_GLOVE] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdGlove"));
	cnmEquipSpy[enumEQUIP_SHOES] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdShoes"));
	cnmEquipSpy[enumEQUIP_LHAND] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdLeftHand"));
	cnmEquipSpy[enumEQUIP_RHAND] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdRightHand"));
	cnmEquipSpy[enumEQUIP_NECK] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdNecklace"));
	cnmEquipSpy[enumEQUIP_HAND1] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdCirclet1"));
	cnmEquipSpy[enumEQUIP_HAND2] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdCirclet2"));
	cnmEquipSpy[enumEQUIP_Jewelry1] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdJewelry1"));
	cnmEquipSpy[enumEQUIP_Jewelry2] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdJewelry2"));
	cnmEquipSpy[enumEQUIP_Jewelry3] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdJewelry3"));
	cnmEquipSpy[enumEQUIP_Jewelry4] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdJewelry4"));
	cnmEquipSpy[enumEQUIP_WING] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdWing"));
		
	cnmEquipSpy[enumEQUIP_CLOAK] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdCloak"));
	cnmEquipSpy[enumEQUIP_FAIRY] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdPet"));
	cnmEquipSpy[enumEQUIP_REAR	] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdRearPet"));
	cnmEquipSpy[enumEQUIP_MOUNT] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdMount"));
	
	
	C3DCompent* ui3dEqSpyCha = (C3DCompent*)frmItemSpy->Find( "ui3dEqSpyCha" );
	if( ui3dEqSpyCha ){
		ui3dEqSpyCha->SetRenderEvent( _EqSpyRenderEvent );
	}
	
	CTextButton* btnLeft3d = (CTextButton*)frmItemSpy->Find("btnLeft3d");
	if (!btnLeft3d){
		Error(g_oLangRec.GetString(45), 
			frmItemSpy->GetName(), "btnLeft3d");
		return false;
	}
	btnLeft3d->evtMouseClick = _RotateSpyLeft;
	btnLeft3d->evtMouseDownContinue = _RotateSpyLeftContinue;

	CTextButton* btnRight3d = (CTextButton*)frmItemSpy->Find("btnRight3d");
	if (!btnRight3d){
		Error(g_oLangRec.GetString(45), 
			frmItemSpy->GetName(), "btnRight3d");
		return false;
	}
	btnRight3d->evtMouseClick = _RotateSpyRight;
	btnRight3d->evtMouseDownContinue = _RotateSpyRightContinue;
	
	
	

	C3DCompent* ui3dCha = (C3DCompent*)frmInv->Find( "ui3dCha" );
	if( ui3dCha ){
		ui3dCha->SetRenderEvent( _ChaRenderEvent );
	}
	
	CTextButton* btnChaLeft3d = (CTextButton*)frmInv->Find("btnChaLeft3d");
	btnChaLeft3d->evtMouseClick = _RotateChaLeft;
	btnChaLeft3d->evtMouseDownContinue = _RotateChaLeftContinue;

	CTextButton* btnChaRight3d = (CTextButton*)frmInv->Find("btnChaRight3d");
	btnChaRight3d->evtMouseClick = _RotateChaRight;
	btnChaRight3d->evtMouseDownContinue = _RotateChaRightContinue;
	

	CCheckBox* chkSortInv = dynamic_cast<CCheckBox*>(frmInv->Find("chkSortInv"));
	chkSortInv->evtCheckChange = _showSortOptions;
	
	
	btnSortPrice = (CTextButton*)frmInv->Find("btnSortPrice");
	btnSortPrice->evtMouseClick = _ClickSortInv;
	
	btnSortAlpha = (CTextButton*)frmInv->Find("btnSortAlpha");
	btnSortAlpha->evtMouseClick = _ClickSortInv;
	
	btnSortType = (CTextButton*)frmInv->Find("btnSortType");
	btnSortType->evtMouseClick = _ClickSortInv;
	
	btnSortID = (CTextButton*)frmInv->Find("btnSortID");
	btnSortID->evtMouseClick = _ClickSortInv;
	
	
	CTextButton* btnGoldStore =(CTextButton*) frmInv->Find("btnGoldStore");
	btnGoldStore->evtMouseClick = _ClickGoldStore;
	

	
	return true;
}

void CEquipMgr::_ClickGoldStore(CGuiData *pSender, int x, int y, DWORD key ){
	g_stUIBox.ShowNumberBox( _evtGoldStoreEvent, g_stUIBoat.GetHuman()->getGameAttr()->get(ATTR_GD), g_oLangRec.GetString(781), true );
}

void CEquipMgr::_evtGoldStoreEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey){
	if( nMsgType!=CForm::mrYes ) return;

	stNumBox* pBox = (stNumBox*)pSender->GetForm()->GetPointer();
	if( !pBox ) return;

	CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_GOLDSTORE, (void*)pBox->GetNumber()); 
}

void CEquipMgr::_ClickSortInv(CGuiData *pSender, int x, int y, DWORD key ){
	string name = pSender->GetName();
	//CForm* frmInv = _FindForm("frmInv");
	CCheckBox* chkSortDir =dynamic_cast<CCheckBox*>( g_stUIEquip.frmInv->Find("chkSortDir"));
	int dir = (int)chkSortDir->GetIsChecked();
	int sort = 0;
	if( name == "btnSortPrice"){
		sort = 0;
	}else if( name == "btnSortAlpha"){
		sort = 1;
	}else if( name == "btnSortType"){
		sort = 2;
	}else if( name == "btnSortID"){
		sort = 3;
	}
	CS_InvSort(sort,dir);
}

void CEquipMgr::_showSortOptions(CGuiData *pSender){
	g_stUIEquip.btnSortPrice->SetIsShow(!g_stUIEquip.btnSortPrice->GetIsShow());
	g_stUIEquip.btnSortAlpha->SetIsShow(!g_stUIEquip.btnSortAlpha->GetIsShow());
	g_stUIEquip.btnSortType->SetIsShow(!g_stUIEquip.btnSortType->GetIsShow());
	g_stUIEquip.btnSortID->SetIsShow(!g_stUIEquip.btnSortID->GetIsShow());
}

void CEquipMgr::_UnequipPart(CGuiData *pSender, int x, int y, DWORD key){
	if( !CGameScene::GetMainCha() ) return;

	string name = pSender->GetName();
	int linkID = 0;
	if(name == "eqhelm"){
		linkID = 0;
	}else if(name == "eqboot"){
		linkID = 4;
	}else if(name == "eqglove"){
		linkID = 3;
	}else if(name == "eqbody"){
		linkID = 2;
	}else{
		return;
	}
	
    stNetItemUnfix item;
	item.chLinkID = linkID;
	item.sGridID = 0;

	if( g_stUIBoat.GetHuman()==CGameScene::GetMainCha() )
	{
		CActor* pActor = g_stUIBoat.GetHuman()->GetActor();
		CEquipState* pState = new CEquipState( pActor );
		pState->SetUnfixData( item );
		pActor->SwitchState( pState );
	}
	else
	{
	    CS_BeginAction( g_stUIBoat.GetHuman(), enumACTION_ITEM_UNFIX, (void*)&item );
	}
}


void CEquipMgr::End()
{
	//RefreshServerShortCut(); Human已经无效
}

void CEquipMgr::_evtSkillUpgrade(CSkillList *pSender, CSkillCommand* pSkill)
{
	if(!pSkill) return;

    int nSkillID = pSkill->GetSkillID();
	g_NetIF->GetProCir()->SkillUpgrade( nSkillID, 1 );
	return;
}

void CEquipMgr::LoadingCall()
{
	CCharacter* pMain = CGameScene::GetMainCha();
	if ( pMain )
	{
		int nJob = pMain->getGameAttr()->get(ATTR_JOB);

		for( int i=0; i<4; i++ )
		{
			CSkillList* pSkillList = GetSkillList( i );
			if(!pSkillList) continue;

			int nCount = pSkillList->GetCount();
			CSkillCommand* pObj = NULL;
			for( int j=0; j<nCount; j++ )
			{
				pObj = pSkillList->GetCommand( j );
				if( pObj )
				{
					pObj->GetSkillRecord()->Refresh( nJob );
				}
			}
		}
	}
}

void CEquipMgr::SynSkillBag(DWORD dwCharID, stNetSkillBag *pSSkillBag)
{
    CCharacter * pCha = g_stUIBoat.GetHuman();
	if( !pCha || pCha->getAttachID()!=dwCharID) 
	{
        LG( "protocol", g_oLangRec.GetString(547) );
        return;
	}

	int nJob = pCha->getGameAttr()->get(ATTR_JOB);
	int nCount = pSSkillBag->SBag.GetCount();
	SSkillGridEx* pSBag = pSSkillBag->SBag.GetValue();
    switch( pSSkillBag->chType )
    {
    case enumSYN_SKILLBAG_INIT: // 技能栏初始化（发送全部的技能信息）
        {
			lstSailSkill->Clear();
			lstFightSkill->Clear();
			lstLifeSkill->Clear();

			_skills.clear();

            CSkillRecord* pInfo = NULL;
            CSkillCommand* tmp = NULL;
			for( int i=0; i<nCount; i++ )
            {
                pInfo = GetSkillRecordInfo( pSBag[i].sID );
                if( !pInfo )
                {
                    LG( "protocol", g_oLangRec.GetString(548), pSBag[i].sID );
                    continue;
                }
                pInfo->GetSkillGrid() = pSBag[i];
				pInfo->Refresh( nJob );
				if( !pInfo->IsShow() ) continue;

                tmp = new CSkillCommand( pInfo );
                GetSkillList( pInfo->chFightType )->AddCommand( tmp );

				_skills.push_back( pInfo );
            }
        }
        break;
    case enumSYN_SKILLBAG_ADD:  // 增加技能（只发送新增加的技能的信息）
        {
            CSkillRecord* pInfo = NULL;
            CSkillCommand* tmp = NULL;
            for( int i=0; i<nCount; i++ )
            {
                pInfo = GetSkillRecordInfo( pSBag[i].sID );
                if( !pInfo )
                {
                    LG( "protocol", g_oLangRec.GetString(549), pSBag[i].sID );
                    continue;
                }
                pInfo->GetSkillGrid() = pSBag[i];
				pInfo->Refresh( nJob );
				if( !pInfo->IsShow() ) continue;

                tmp = new CSkillCommand( pInfo );

                GetSkillList( pInfo->chFightType )->AddCommand( tmp );
				_skills.push_back( pInfo );
            }
        }
        break;
    case enumSYN_SKILLBAG_MODI: // 修改技能
        {
            CSkillRecord* pInfo = NULL;
            CSkillCommand* tmp = NULL;
            for( int i=0; i<nCount; i++ )
            {
                pInfo = GetSkillRecordInfo( pSBag[i].sID );
                if( !pInfo )
                {
                    LG( "protocol", g_oLangRec.GetString(550), pSBag[i].sID );
                    continue;
                }
                pInfo->GetSkillGrid() = pSBag[i];
				pInfo->Refresh( nJob );
				if( !pInfo->IsShow() ) continue;

                if( pSBag[i].chLv==0 && !GetSkillList( pInfo->chFightType )->DelSkill( pSBag[i].sID ) )
                {
                    LG( "protocol", g_oLangRec.GetString(551), pSBag[i].sID );
                    continue;
                }
            }        
        }
        break;
    default:
        LG( "protocol", g_oLangRec.GetString(552), pSSkillBag->chType );
        return;
    }


	lstSailSkill->Refresh();
	lstFightSkill->Refresh();
	lstLifeSkill->Refresh();
}

CSkillRecord* CEquipMgr::FindSkill( int nID )
{
	for( vskill::iterator it=_skills.begin(); it!=_skills.end(); it++ )
		if( (*it)->nID==nID )
			return *it;

	return NULL;
}

//defItemShortCutType
void CEquipMgr::FastChange(int nIndex, short sGridID, char chType,bool update){
	if(update){
		_pFastCommands[ nIndex ]->AddCommand( GetGoodsGrid()->GetItem(sGridID) );
	}else{
		stNetShortCutChange param;
		memset( &param, 0, sizeof(param) );
		param.chIndex = nIndex;
		param.chType = chType;
		param.shyGrid = sGridID;
		
		/*
		if(_pFastCommands[ nIndex ]->GetCommand2()){
			param.shyGrid2 = _pFastCommands[ nIndex ]->GetCommand2()->nTag;
		}
		*/
		
		CS_BeginAction( g_stUIBoat.GetHuman(), enumACTION_SHORTCUT, (void*)&param );
		stNetShortCut& SCut = _stShortCut;
		SCut.chType[nIndex] = chType;
		SCut.byGridID[nIndex] = sGridID;
	}
}

void CEquipMgr::_evtFastChange(CGuiData *pSender, CCommandObj* pItem, bool& isAccept)
{	
	CGoodsGrid* pGrid = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
	if( pGrid && pGrid!=g_stUIEquip.GetGoodsGrid() ) return;

    CFastCommand* pFast = dynamic_cast<CFastCommand*>(pSender);
    if( !pFast ) return;
   
    isAccept = true;

    char chType = 0;
    short sGridID = 0;
    int nIndex = pFast->nTag;
	if( !g_stUIEquip._GetCommandShortCutType( pItem, chType, sGridID ) )
	{
		isAccept = false;
		g_pGameApp->SysInfo( g_oLangRec.GetString(553), pItem->GetName() );
		return;
	}
	g_stUIEquip.FastChange( nIndex,  sGridID,  chType);
}


void CEquipMgr::UpdataEquipData( const stNetChangeChaPart& SPart, CCharacter* pCha )
{
	// ID不等于零时,更新数值
	CItemCommand* pItem = NULL;
	for( int i=0; i<enumEQUIP_NUM; i++ )
	{
		// Modify by lark.li 20080818 begin
		//if( SPart.SLink[i].sID==0 ) continue;
		if( cnmEquip[i] == NULL || SPart.SLink[i].sID==0 ) continue;
		// End

        pItem = dynamic_cast<CItemCommand*>(cnmEquip[i]->GetCommand());
        if( pItem && pItem->GetItemInfo()->lID==SPart.SLink[i].sID )
        {
			SItemGrid& Item = pItem->GetData();
			Item.bValid = SPart.SLink[i].bValid;
			Item.sEndure[0] = SPart.SLink[i].sEndure[0];
			Item.sEnergy[0] = SPart.SLink[i].sEnergy[0];
        }
		else
		{
			LG( "error", g_oLangRec.GetString(554),( pItem ? pItem->GetItemInfo()->lID : 0), SPart.SLink[i].sID );
		}
	}
}

void CEquipMgr::UpdataEquip( const stNetChangeChaPart& SPart, CCharacter* pCha )
{     
    // 默认装备不占装备栏
    memcpy( &stEquip, &SPart, sizeof(SPart) );
    for( int i=0; i<enumEQUIP_PART_NUM; i++ )
    {
        if( stEquip.SLink[i].sID==pCha->GetDefaultChaInfo()->sSkinInfo[i] )
            stEquip.SLink[i].sID = 0;
    }
    //for( int i=enumEQUIP_PART_NUM; i<enumEQUIP_NUM; i++ )
    //{
    //    if( stEquip.SLink[i].sID==enumEQUIP_BOTH_HAND )
    //        stEquip.SLink[i].sID = 0;
    //}

    // 更新UI装备栏
    for( int i=0; i<enumEQUIP_NUM; i++ )
    {
        _UpdataEquip( stEquip.SLink[i], i );
    }
}

void CEquipMgr::UpdataEquipSpy( const stNetChangeChaPart& SPart, CCharacter* pCha )
{     
    // 默认装备不占装备栏
	eqSpyTarget = pCha;
	
	CLabel* labTitle = (CLabel*)frmItemSpy->Find( "labTitle" );
	char buf[64];
	sprintf(buf,"EqSpy - %s",pCha->getName());
	labTitle->SetCaption(buf);
	
	
    memcpy( &stEquipSpy, &SPart, sizeof(SPart) );
    for( int i=0; i<enumEQUIP_PART_NUM; i++ )
    {
        if( stEquipSpy.SLink[i].sID==pCha->GetDefaultChaInfo()->sSkinInfo[i] )
            stEquipSpy.SLink[i].sID = 0;
    }
    for( int i=0; i<enumEQUIP_NUM; i++ )
    {
        _UpdataEquipSpy( stEquipSpy.SLink[i], i );
    }
	
	frmItemSpy->Show();
	
	
}

bool CEquipMgr::_UpdataEquipSpy( SItemGrid& Item, int nLink )
{
    if( !cnmEquipSpy[nLink] ) return false;

    int nItemID = Item.sID;
    if( nItemID>0 && nItemID!=enumEQUIP_BOTH_HAND )
    {        
        CItemCommand* pItem = dynamic_cast<CItemCommand*>(cnmEquipSpy[nLink]->GetCommand());
        if( pItem && pItem->GetItemInfo()->lID==nItemID )
        {
	        pItem->SetData( Item );
            return true;
        }

        CItemRecord* pInfo = GetItemRecordInfo( nItemID );
        if( !pInfo )
        {
            return false;
        }

        pItem = new CItemCommand( pInfo );
        pItem->SetData( Item );
        cnmEquipSpy[nLink]->AddCommand( pItem );
    }
    else
    {
        cnmEquipSpy[nLink]->DelCommand();
    }
    return true;
}


void CEquipMgr::RenderSpy(int x,int y){
	if( !eqSpyTarget || !spyModel ) {
		return;
	}
	RenderModel(x,y,eqSpyTarget, spyModel,eqSpyTargetRotate,refreshSpyModel);
	refreshSpyModel = false;
}

void CEquipMgr::SwitchMap(){
	chaModel = 0;
	eqSpyTarget = 0;
	spyModel = 0;
	refreshChaModel = true;
	refreshSpyModel = true;
}
	
void CEquipMgr::RenderModel(int x, int y, CCharacter* original, CCharacter* model, int rotation,bool refresh){
	
	g_Render.LookAt( D3DXVECTOR3( 11.0f, 36.0f, 10.0f ), D3DXVECTOR3( 8.70f, 12.0f, 8.0f ), MPRender::VIEW_3DUI );
	
	MPMatrix44 matrix = *model->GetMatrix();
	
	model->SetUIYaw(180 + rotation);

	int typeID = model->getTypeID();
/*
	if (typeID == 3){
		model->SetUIScaleDis(12.0f * g_Render.GetScrWidth()/SMALL_RES_X );
	}else if( typeID ==1 ){
		model->SetUIScaleDis(12.0f * g_Render.GetScrWidth()/SMALL_RES_X );
	}else if(typeID == 4){
		model->SetUIScaleDis(12.0f* g_Render.GetScrWidth()/SMALL_RES_X );//7
	}else if(typeID == 2){
		model->SetUIScaleDis(12.0f* g_Render.GetScrWidth()/SMALL_RES_X );
	}*/
	model->SetUIScaleDis(15.0f* g_Render.GetScrWidth()/SMALL_RES_X );
	if(!model->IsSameEquip(original->GetPart()) || refresh){
		model->UpdataFace(original->GetPart());
	}
	model->RenderForUI(x, y);
	model->SetMatrix(&matrix);
	
	g_Render.SetTransformView(&g_Render.GetWorldViewMatrix());
	
	model->PlayPose( 1, PLAY_LOOP_SMOOTH );
	
}

void CEquipMgr::RenderCha(int x,int y){
	RenderModel(x,y,g_stUIBoat.GetHuman(), chaModel,chaModelRotate,refreshChaModel);
	refreshChaModel = false;
}




void CEquipMgr::RotateCha(eDirectType enumDirect){
	chaModelRotate += 180;
	chaModelRotate += -((int)(enumDirect)) * 15;
	chaModelRotate = (chaModelRotate + 360) % 360;
	chaModelRotate -= 180;
}

void CEquipMgr::_RotateChaLeft(CGuiData *sender, int x, int y, DWORD key ){
	g_stUIEquip.RotateCha(LEFT);
}
	
	//~ ==================================================================
void CEquipMgr::_RotateChaRight(CGuiData *sender, int x, int y, DWORD key ){
	g_stUIEquip.RotateCha(RIGHT);
}

void CEquipMgr::_RotateChaLeftContinue(CGuiData *sender ){
	g_stUIEquip.RotateCha(LEFT);
}

void CEquipMgr::_RotateChaRightContinue(CGuiData *sender ){
	g_stUIEquip.RotateCha(RIGHT);
}



void CEquipMgr::RotateSpy(eDirectType enumDirect){
	eqSpyTargetRotate += 180;
	eqSpyTargetRotate += -((int)(enumDirect)) * 15;
	eqSpyTargetRotate = (eqSpyTargetRotate + 360) % 360;
	eqSpyTargetRotate -= 180;
}

void CEquipMgr::_EqSpyRenderEvent( C3DCompent *pSender, int x, int y){
	g_stUIEquip.RenderSpy(x, y);
}
void CEquipMgr::_ChaRenderEvent( C3DCompent *pSender, int x, int y){
	g_stUIEquip.RenderCha(x, y);
}

void CEquipMgr::_RotateSpyLeft(CGuiData *sender, int x, int y, DWORD key ){
	g_stUIEquip.RotateSpy(LEFT);
}
	
	//~ ==================================================================
void CEquipMgr::_RotateSpyRight(CGuiData *sender, int x, int y, DWORD key ){
	g_stUIEquip.RotateSpy(RIGHT);
}

//~ ==================================================================
void CEquipMgr::_RotateSpyLeftContinue(CGuiData *sender ){
	g_stUIEquip.RotateSpy(LEFT);
}

//~ ==================================================================
void CEquipMgr::_RotateSpyRightContinue(CGuiData *sender ){
	g_stUIEquip.RotateSpy(RIGHT);
}

bool CEquipMgr::_UpdataEquip( SItemGrid& Item, int nLink )
{
    if( !cnmEquip[nLink] ) return false;

    int nItemID = Item.sID;
    if( nItemID>0 && nItemID!=enumEQUIP_BOTH_HAND )
    {        
        CItemCommand* pItem = dynamic_cast<CItemCommand*>(cnmEquip[nLink]->GetCommand());
        if( pItem && pItem->GetItemInfo()->lID==nItemID )
        {
	        pItem->SetData( Item );
            return true;
        }

        CItemRecord* pInfo = GetItemRecordInfo( nItemID );
        if( !pInfo )
        {
            LG( "UpdataEquip", g_oLangRec.GetString(555), nItemID );
            return false;
        }

        pItem = new CItemCommand( pInfo );
        pItem->SetData( Item );
        cnmEquip[nLink]->AddCommand( pItem );
    }
    else
    {
        cnmEquip[nLink]->DelCommand();
    }
	
    return true;
}

bool CEquipMgr::_GetCommandShortCutType( CCommandObj* pItem, char& chType, short& sGridID )
{
	chType = 0;
	sGridID = 0;

	if( !pItem ) return true;

    if( g_stUIEquip.GetGoodsGrid() && pItem->GetParent()==g_stUIEquip.GetGoodsGrid() )
    {
        chType = defItemShortCutType;
        sGridID = pItem->GetIndex();
    }
    else if( g_stUIEquip.lstFightSkill && g_stUIEquip.lstFightSkill==pItem->GetParent() )
    {
        CSkillCommand* pSkill = dynamic_cast<CSkillCommand*>(pItem);

		if( pSkill->GetSkillRecord()->chType==2 )
		{
			return false;
		}

        if( pSkill ) 
        {
            chType = defSkillFightShortCutType;
            sGridID = pSkill->GetSkillID();
        }
    }
    else if( g_stUIEquip.lstLifeSkill && g_stUIEquip.lstLifeSkill==pItem->GetParent() )
    {
        CSkillCommand* pSkill = dynamic_cast<CSkillCommand*>(pItem);
        if( pSkill ) 
        {
            chType = defSkillLifeShortCutType;
            sGridID = pSkill->GetSkillID();
		}
    }
	else if(g_stUIEquip.lstSailSkill && g_stUIEquip.lstSailSkill == pItem->GetParent())
	{
        CSkillCommand* pSkill = dynamic_cast<CSkillCommand*>(pItem);
        if( pSkill ) 
        {
            chType = defSkillSailShortCutType;
            sGridID = pSkill->GetSkillID();
		}
	}

	return true;
}

int CEquipMgr::RefreshServerShortCut()
{
	int nCount = 0;
	stNetShortCut tmp;
	memset( &tmp, 0, sizeof(tmp) );
	for( int i=0; i<SHORT_CUT_NUM; i++ )
	{
		if( _pFastCommands[i] )
		{
			_GetCommandShortCutType( _pFastCommands[i]->GetCommand(), tmp.chType[i], tmp.byGridID[i] );
		}
	}

	stNetShortCutChange param;
	for( int i=0; i<SHORT_CUT_NUM; i++ )
	{
		if( (tmp.chType[i]!=_stShortCut.chType[i]) || (tmp.byGridID[i]!=_stShortCut.byGridID[i]) )
		{
			param.chIndex = i;
			param.chType = tmp.chType[i];
			param.shyGrid = tmp.byGridID[i];
			CS_BeginAction( g_stUIBoat.GetHuman(), enumACTION_SHORTCUT, (void*)&param );
			nCount++;

			LG( "shortcut", "Index:%d, Type:%d, GridID:%d\n", param.chIndex, param.chType, param.shyGrid );
		}
	}
	memcpy( &_stShortCut, &tmp, sizeof(tmp) );
	LG( "shortcut", "Total:%d\n\n", nCount );
	return nCount;
}

void CEquipMgr::UpdateShortCut( stNetShortCut& stShortCut ){
    memcpy( &_stShortCut, &stShortCut, sizeof(_stShortCut) );
   if( _pFastCommands )
   {
       for( unsigned int i=0; i<SHORT_CUT_NUM; i++ )
       {
           if( _pFastCommands[i] ) _pFastCommands[i]->DelCommand();
       }
   }

	int nIndex = 0;
	CGoodsGrid* getGoods = GetGoodsGrid();
	for( DWORD i=0; i<SHORT_CUT_NUM; i++ ) { 
		if( _pFastCommands[ i ] ){	
			if( getGoods && stShortCut.chType[i]==defItemShortCutType ){
				_pFastCommands[ i ]->AddCommand( getGoods->GetItem(stShortCut.byGridID[i]) );
			}
			else if (lstFightSkill && stShortCut.chType[i]==defSkillFightShortCutType ){
				_pFastCommands[ i ]->AddCommand( lstFightSkill->FindSkill( stShortCut.byGridID[i] ) );
			}
			else if (lstLifeSkill && stShortCut.chType[i]==defSkillLifeShortCutType ){
				_pFastCommands[ i ]->AddCommand( lstLifeSkill->FindSkill( stShortCut.byGridID[i] ) );				
			}
			else if (lstSailSkill && stShortCut.chType[i]==defSkillSailShortCutType ){
				_pFastCommands[ i ]->AddCommand( lstSailSkill->FindSkill( stShortCut.byGridID[i] ) );		
			}
		}
	}
}

void CEquipMgr::DelFastCommand( CCommandObj* pObj )
{
	if( pObj && pObj->GetIsFast() )
	{
		CFastCommand* pFast = CFastCommand::FintFastCommand( pObj ,true);
		if( pFast )
		{
			int nIndex = pFast->nTag;
			stNetShortCutChange param;
			memset( &param, 0, sizeof(param) );
			param.chIndex = nIndex;
			CS_BeginAction( g_stUIBoat.GetHuman(), enumACTION_SHORTCUT, (void*)&param );

			// 服务器接收到后无返回,客户端自己改变
			stNetShortCut& SCut = _stShortCut;
			SCut.chType[nIndex] = 0;
			SCut.byGridID[nIndex] = 0;	
			pFast->DelCommand();			
		}				
	}
}

void CEquipMgr::_evtButtonClickEvent( CGuiData *pSender, int x, int y, DWORD key)
{
	string name=pSender->GetName();
    if( name=="btnFastUp" )
    {
        g_stUIEquip._ActiveFast( g_stUIEquip._nFastCur-1 );
        return;
    }
    else if( name=="btnFastDown" )
    {
        g_stUIEquip._ActiveFast( g_stUIEquip._nFastCur+1 );
        return;
    }
}

void CEquipMgr::_evtEquipEvent(CGuiData *pSender, CCommandObj* pItem, bool& isAccept)
{
    isAccept = false;

    CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
    if( pGood!=g_stUIEquip.GetGoodsGrid() ) return;

    CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
    if( !pItemCommand ) return;

    CGameScene* pScene = g_pGameApp->GetCurScene();
    if( !pScene ) return;

    CCharacter* pCha = pScene->GetMainCha();
    if( !pCha ) return;

    COneCommand* pCom = dynamic_cast<COneCommand*>(pSender);
    if( !pCom ) return;

	if( pCom->nTag==999 )
	{
		// 直接丢弃
		g_stUIEquip._nLostGridID = g_stUIEquip.grdItem->GetDragIndex();
		g_stUIBox.ShowSelectBox( _evtLostYesNoEvent, g_oLangRec.GetString(556), true );
	}
	else
	{
		//Add by sunny.sun 20080902
		/*CItemRecord * _pItem = pItemCommand->GetItemInfo();
		if( _pItem->sType == 8 || _pItem->sType == 13 || _pItem->sType == 31 || _pItem->sType == 36 || 
			_pItem->sType == 41 || _pItem->sType == 42 || _pItem->sType == 47 || _pItem->sType == 48 ||
			_pItem->sType == 57 || _pItem->sType == 58 || _pItem->sType == 65)
		{
			g_pGameApp->MsgBox("道具使用方式错误");
			return;
		}*/
		//End
		stNetUseItem info;
		info.sGridID = g_stUIEquip.grdItem->GetDragIndex();
		CS_BeginAction( g_stUIBoat.GetHuman(), enumACTION_ITEM_USE, &info );
	}
}

void CEquipMgr::_evtLostYesNoEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if( nMsgType!=CForm::mrYes ) return;

	stNetDelItem info;
	info.sGridID = g_stUIEquip._nLostGridID;
	CS_BeginAction( g_stUIBoat.GetHuman(), enumACTION_ITEM_DELETE, &info );
}

void CEquipMgr::_evtThrowEquipEvent(CGuiData *pSender, CCommandObj* pItem, bool& isThrow)
{
    isThrow = false;

    CGameScene* pScene = CGameApp::GetCurScene();
    if( !pScene ) return;

    CCharacter* pMain = CGameScene::GetMainCha();
    if( !pMain ) return;

    COneCommand* pCom = dynamic_cast<COneCommand*>(CDrag::GetParent());
    if( pCom && pCom->nTag>0 )
    {
		int x = (int)(pScene->GetMouseMapX() * 100.0f);
		int y = (int)(pScene->GetMouseMapY() * 100.0f);

		if( !g_stUIEquip._GetThrowPos( x, y ) ) return;

		stUnfix& unfix = g_stUIEquip._sUnfix;
		unfix.Reset();

		unfix.nGridID = -2;
		unfix.nLink = pCom->nTag;
		unfix.nX = x;
		unfix.nY = y;
		unfix.pItem = pItem;
		g_stUIEquip._StartUnfix( unfix );
    }
}

bool CEquipMgr::ExecFastKey( int key ){ 
    if( key==VK_TAB || ( key==VK_OEM_6 && ::GetAsyncKeyState(VK_CONTROL) ) )
    {
        _ActiveFast( _nFastCur+1 );
        return true;
    }
	
    key -= VK_F1;
	
    if ( key<0 || key>(int)11 ) {
		return false;
	}
	
	if (! CFormMgr::s_Mgr.GetEnableHotKey()) {
		return false;
	}

    
	//check if we are using the top bar
	if(g_pGameApp->IsShiftPress() ){
		key = 2*MAX_FAST_COL+key;
	}else{
		key = _nFastCur*MAX_FAST_COL+key;
	}
	
	if( _pFastCommands[key]){
		_pFastCommands[key]->Exec();
		return true;
	}
	return false;
}

void CEquipMgr::_ActiveFast( int num )
{
    if( num>=(int)2 ) 
    {
        num = 0;
    }

    if( num<0 ) 
    {
        num = 2 - 1;
    }

    int count = (int)SHORT_CUT_NUM;
    for( int i=0; i<count; i++ )
    {
        if( _pFastCommands[i] )
        {
            _pFastCommands[i]->SetIsShow( false );
        }
    }

    count = (num+1)*MAX_FAST_COL;
    for( int i=num*MAX_FAST_COL; i<count; i++ )
    {
        if( _pFastCommands[i] )
        {
            _pFastCommands[i]->SetIsShow( true );
        }
    }
    _nFastCur = num; 
	
	count =3*MAX_FAST_COL;
    for( int i=2*MAX_FAST_COL; i<count; i++ )
    {
        if( _pFastCommands[i] )
        {
            _pFastCommands[i]->SetIsShow( true );
        }
    }
    //_nFastCur = num;

	//sprintf( szBuf, "%d", _nFastCur + 1 );
	//_pActiveFastLabel->SetCaption( szBuf );
}

void CEquipMgr::evtSwapItemEvent(CGuiData *pSender,int nFirst, int nSecond, bool& isSwap)
{
    isSwap = false;
    CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());

	CCharacter* pSelf = g_stUIBoat.FindCha( pGood );
	if( !pSelf ) return;

	if( pSelf->IsBoat() && pSelf!=CGameScene::GetMainCha() )
	{
		g_pGameApp->SysInfo( g_oLangRec.GetString(557) );
		return;
	}

	CItemCommand* pItem = dynamic_cast<CItemCommand*>( pGood->GetItem( nSecond ) );
	CItemCommand* pTarget = dynamic_cast<CItemCommand*>( pGood->GetItem( nFirst ) );
	if (pItem && !pItem->GetIsValid())
	{
		return;
	}
	if (pTarget && !pTarget->GetIsValid())
	{
		return;
	}


	if( g_pGameApp->IsShiftPress() )
	{
		if( pItem && pItem->GetTotalNum()>1 )
		{
			if( !pTarget || pTarget->GetItemInfo()->nID==pItem->GetItemInfo()->nID )
			{
				SSplit.nFirst = nFirst;
				SSplit.nSecond = nSecond;
				SSplit.pSelf = pSelf;
				CBoxMgr::ShowNumberBox( SSplitItem::_evtSplitItemEvent, pItem->GetTotalNum() );
				return;
			}
		}
	}

	stNetItemPos info;
	info.sSrcGridID = nSecond;
	info.sSrcNum = 0;
	info.sTarGridID = nFirst;
	CS_BeginAction( pSelf, enumACTION_ITEM_POS, (void*)&info );
}

void CEquipMgr::SSplitItem::_evtSplitItemEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if( nMsgType!=CForm::mrYes ) return;

	stNumBox* pBox = (stNumBox*)pSender->GetForm()->GetPointer();
	if( !pBox ) return;

	stNetItemPos info;
	info.sSrcGridID = SSplit.nSecond;
	info.sSrcNum = pBox->GetNumber();
	info.sTarGridID = SSplit.nFirst;
	CS_BeginAction( SSplit.pSelf, enumACTION_ITEM_POS, (void*)&info );
}

bool CEquipMgr::_GetThrowPos( int& x, int& y )
{
    CCharacter* pCha = CGameScene::GetMainCha();
    if( !pCha ) return false;

    GetDistancePos( pCha->GetCurX(), pCha->GetCurY(), x, y, 100, x, y );
    if( CGameApp::GetCurScene()->GetIsBlockWalk(pCha, x, y) )
    {
        x = pCha->GetCurX();
        y = pCha->GetCurY();
    }
    return true;
}

void CEquipMgr::evtThrowItemEvent(CGuiData *pSender,int id,bool& isThrow)
{
    isThrow = false;

    if( !CGameApp::GetCurScene() )  return;

    CCharacter* pMain = CGameScene::GetMainCha();
    if( !pMain ) return;

    CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());

    CCommandObj* obj = pGood->GetItem(id); 

	CCharacter* pSelf = g_stUIBoat.FindCha( pGood );
	if( !pSelf ) return;

	// add by Philip.Wu  2006-07-05
	// 判定道具是否合法，防止道具被锁后丢弃再捡起又可用的 BUG
	if(!obj->GetIsValid()) return;

    int x = (int)(CGameApp::GetCurScene()->GetMouseMapX() * 100.0f);
    int y = (int)(CGameApp::GetCurScene()->GetMouseMapY() * 100.0f);

	if( !g_stUIEquip._GetThrowPos( x, y ) ) return;

	stThrow& sthrow = g_stUIEquip._sThrow;
	sthrow.nX = x;
	sthrow.nY = y;
	sthrow.nGridID = id;
	sthrow.pSelf = pSelf;

	//如果是船长证明
	CItemCommand* pItem = dynamic_cast<CItemCommand*>(obj);
	if ( pItem && pItem->GetItemInfo()->sType==43 )
	{
		stSelectBox* pBox = g_stUIBox.ShowSelectBox(_evtThrowBoatDialogEvent,
			g_oLangRec.GetString(558), 
			true);
		if (pBox)
		{
			pBox->pointer = &sthrow;
			return;
		}
	}

    if( obj->GetIsPile() && obj->GetTotalNum()>1 )
    {
		stNumBox* pBox = g_stUIBox.ShowNumberBox( _evtThrowDialogEvent, obj->GetTotalNum() );
		if( pBox )
		{
			pBox->pointer = &sthrow;
	        return;
		}
    }

	g_stUIEquip._SendThrowData( sthrow );
}

void CEquipMgr::_evtThrowDialogEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if( nMsgType!=CForm::mrYes ) return;

	stNumBox* pBox = (stNumBox*)pSender->GetForm()->GetPointer();
	if( !pBox ) return;

	stThrow* p = (stThrow*)pBox->pointer;
	if( !p ) return;

	g_stUIEquip._SendThrowData( *p, pBox->GetNumber() );
}

void CEquipMgr::_evtThrowBoatDialogEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if( nMsgType!=CForm::mrYes ) return;

	stSelectBox* pBox = (stSelectBox*)pSender->GetForm()->GetPointer();
	if( !pBox ) return;

	stThrow* p = (stThrow*)pBox->pointer;
	if( !p ) return;

	g_stUIEquip._SendThrowData( *p, 1 );
}

void CEquipMgr::_SendThrowData( const stThrow& sthrow, int nThrowNum )
{
	if( sthrow.pSelf->IsBoat() && sthrow.pSelf!=CGameScene::GetMainCha() )
	{
		g_pGameApp->SysInfo( g_oLangRec.GetString(557) );
		return;
	}

    stNetItemThrow item;
    item.lNum = nThrowNum;
    item.sGridID = sthrow.nGridID;
	item.lPosX = sthrow.nX;
    item.lPosY = sthrow.nY;
	CS_BeginAction( sthrow.pSelf, enumACTION_ITEM_THROW, (void*)&item );
}

void CEquipMgr::_evtSkillFormShow(CGuiData *pSender)
{
	g_stUIEquip.RefreshUpgrade();
}

void CEquipMgr::UpdateIMP(int IMP){
	lIMP = IMP;
	//CForm* frmInv = _FindForm("frmInv");
	CLabel* lblIMP = dynamic_cast<CLabel*>(frmInv->Find("labItemIMPnumber"));
	char szBuf[16];
	sprintf( szBuf, "%s", StringSplitNum( IMP )  );
	lblIMP->SetCaption( szBuf );
}

void CEquipMgr::FrameMove(DWORD dwTime)
{
	static CTimeWork time(100);
	if( time.IsTimeOut( dwTime ) )
	{
		CCharacter* pCha = g_stUIBoat.GetHuman();
		if( !pCha ) return;

		SGameAttr* pGameAttr = pCha->getGameAttr();
		if( frmSkill->GetIsShow() )
		{
			// 显示剩余技能点数
			sprintf( szBuf, "%d", pGameAttr->get(ATTR_TP) );
			labPoint->SetCaption( szBuf );

			sprintf( szBuf, "%d", pGameAttr->get(ATTR_LIFETP) );
			labPointLife->SetCaption( szBuf );

			RefreshUpgrade();
		}


		CCharacter* pMainCha = CGameScene::GetMainCha();
		if( pMainCha )
		{
			// 刷新技能激活框
			CSkillCommand::GetActiveImage()->Next();

			CChaStateMgr* pState = pMainCha->GetStateMgr();

			for( vskill::iterator it=_cancels.begin(); it!=_cancels.end(); it++ )
				(*it)->SetIsActive( pState->HasSkillState( (*it)->nStateID ) );


			if(frmInv->GetIsShow()){
				sprintf( szBuf, "%s", StringSplitNum( pGameAttr->get(ATTR_GD) )  );
				lblGold->SetCaption( szBuf );
				
				for( int i=0; i<enumEQUIP_NUM; i++ ) 
					_imgCharges[i].Next();
				
				CItemCommand* pItem = NULL;
				for( states::iterator it=_charges.begin(); it!=_charges.end(); it++ )
				{
					if( COneCommand* pCmd = cnmEquip[(*it)->sChargeLink] )
					{
						pItem = dynamic_cast<CItemCommand*>( pCmd->GetCommand() );
						if( pItem && pItem->GetItemInfo()->sType==29 )
						{
							pCmd->SetIsShowActive( pState->HasSkillState( (*it)->chID ) );
						}
						else
						{
							pCmd->SetIsShowActive( false );
						}
					}
				}
			}
			
		}
	}
}

void CEquipMgr::RefreshUpgrade()
{
	CForm * f = frmSkill;
	if( !f->GetIsShow() ) return;

	CCharacter* pCha = CGameScene::GetMainCha();
	if( !pCha )			return;

	SGameAttr* pAttr = pCha->getGameAttr();

	lstFightSkill->SetIsShowUpgrade(pAttr->get(ATTR_TP)>0);
	lstLifeSkill->SetIsShowUpgrade(pAttr->get(ATTR_LIFETP)>0);

	RefreshSkillJob( pAttr->get(ATTR_JOB) );
}

void CEquipMgr::RefreshSkillJob( int nJob )
{
	int count = lstFightSkill->GetCount();
	CSkillCommand* tmp = NULL;
	CSkillList* pList = NULL;
	for( int j=0; j<2; j++ )
	{
		pList = GetSkillList( j );
		for( int i=0; i<count; i++ )
		{
			tmp = pList->GetCommand(i);
			if( tmp )
			{
				tmp->GetSkillRecord()->Refresh( nJob );
			}
		}
	}
}

void CEquipMgr::UnfixToGrid( CCommandObj* pItem, int nGridID, int nLink )
{

	_sUnfix.Reset();

	_sUnfix.nLink = nLink;
	_sUnfix.nGridID = nGridID;
	_sUnfix.pItem = pItem;

	_StartUnfix( _sUnfix );
}
/*
void CEquipMgr::_evtItemFormClose( CForm *pForm, bool& IsClose )
{
	if ( g_stUITrade.IsTrading() ){
		IsClose = false;
	}
	
	if(g_stUIEquip.chkLinkEqForm->GetIsChecked()&& g_stUIEquip.frmEquip->GetIsShow()){
		g_stUIEquip.frmEquip->Hide();
	}
}*/

void CEquipMgr::_evtEquipFormClose( CForm *pForm, bool& IsClose ){
	if ( g_stUITrade.IsTrading() ){
		IsClose = false;
	}
	if (g_stUIEquip.chaModel){
		g_stUIEquip.chaModel->SetValid(false);
		g_stUIEquip.refreshChaModel = true;
	}
}

void CEquipMgr::_evtSpyFormClose( CForm *pForm, bool& IsClose ){
	if (g_stUIEquip.spyModel){
		g_stUIEquip.spyModel->SetValid(false);
	}
}

void CEquipMgr::_evtSpyFormShow(CGuiData *pSender){
	int typeID = g_stUIEquip.eqSpyTarget->getTypeID();
	
	g_stUIEquip.spyModel = CGameApp::GetCurScene()->AddCharacter(typeID );

	lwIByteSet* res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
	BYTE loadtex_flag = res_bs->GetValue(OPT_RESMGR_LOADTEXTURE_MT);
	BYTE loadmesh_flag = res_bs->GetValue(OPT_RESMGR_LOADMESH_MT);
	res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
	res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0);
	res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, loadtex_flag);
	res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, loadmesh_flag);
	g_stUIEquip.spyModel->SetIsForUI(true);
    g_stUIEquip.spyModel->EnableAI(false);
	g_stUIEquip.spyModel->SetHide(true);
	g_stUIEquip.spyModel->GetActor()->SetSleep();
	g_stUIEquip.refreshSpyModel = true;
	
}

void CEquipMgr::_evtEquipFormShow(CGuiData *pSender){
	CCharacter* pCha = g_stUIBoat.GetHuman();
	int typeID = pCha->getTypeID();
	g_stUIEquip.chaModel = CGameApp::GetCurScene()->AddCharacter(typeID );

	lwIByteSet* res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
	BYTE loadtex_flag = res_bs->GetValue(OPT_RESMGR_LOADTEXTURE_MT);
	BYTE loadmesh_flag = res_bs->GetValue(OPT_RESMGR_LOADMESH_MT);
	res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
	res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0);
	res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, loadtex_flag);
	res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, loadmesh_flag);
	g_stUIEquip.chaModel->SetIsForUI(true);
    g_stUIEquip.chaModel->EnableAI(false);
	g_stUIEquip.chaModel->SetHide(true);
	g_stUIEquip.chaModel->GetActor()->SetSleep();
	g_stUIEquip.refreshChaModel = true;
}

/*
void CEquipMgr::_evtItemFormShow(CGuiData *pSender)
{
	if(g_stUIStore.GetStoreForm()->GetIsShow())
	{
		g_stUIEquip.frmItem->SetIsShow(false);
	}
	if(g_stUIEquip.chkLinkEqForm->GetIsChecked()&& !g_stUIEquip.frmEquip->GetIsShow()){
		g_stUIEquip.frmEquip->Show();
	}		
}*/

void CEquipMgr::_SendUnfixData(const stUnfix& unfix, int nUnfixNum)
{
	if( !CGameScene::GetMainCha() ) return;

	if( unfix.nLink==-1 ) return;

    stNetItemUnfix item;
	item.chLinkID = unfix.nLink;
	item.sGridID = unfix.nGridID;
	item.lPosX = unfix.nX;
	item.lPosY = unfix.nY;
	
	if( g_stUIBoat.GetHuman()==CGameScene::GetMainCha() )
	{
		CActor* pActor = g_stUIBoat.GetHuman()->GetActor();
		CEquipState* pState = new CEquipState( pActor );
		pState->SetUnfixData( item );
		pActor->SwitchState( pState );
	}
	else
	{
	    CS_BeginAction( g_stUIBoat.GetHuman(), enumACTION_ITEM_UNFIX, (void*)&item );
	}
}

void CEquipMgr::_evtUnfixDialogEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if( nMsgType!=CForm::mrYes ) return;

	stNumBox* pBox = (stNumBox*)pSender->GetForm()->GetPointer();
	if( !pBox ) return;

	stUnfix* p = (stUnfix*)pBox->pointer;
	if( !p ) return;

	g_stUIEquip._SendUnfixData( *p, pBox->GetNumber() );
}

void CEquipMgr::_StartUnfix( stUnfix& unfix )
{
	CCommandObj* pItem = unfix.pItem;
	if( !pItem ) return;
	
	if( unfix.nGridID==-2 )
	{
        //if( !pItem->IsAllowThrow() ) 
        //{
        //    g_pGameApp->SysInfo( "特殊物品不能丢弃" );
        //    return;
        //}

		if( !_GetThrowPos( unfix.nX, unfix.nY ) ) return;		
	}


	if( pItem->GetIsPile() && pItem->GetTotalNum()>1 )
	{
		stNumBox* pBox = g_stUIBox.ShowNumberBox( _evtUnfixDialogEvent, pItem->GetTotalNum() );
		if( pBox )
		{
			pBox->pointer = &unfix;
	        return;
		}
	}

	_SendUnfixData( unfix );
}

CItemCommand* CEquipMgr::GetEquipItem( unsigned int nLink )
{ 
	if( nLink<enumEQUIP_NUM && cnmEquip[nLink] )
		return dynamic_cast<CItemCommand*>(cnmEquip[nLink]->GetCommand());
	return NULL;
}

bool CEquipMgr::IsEquipCom( COneCommand* pCom )
{
	return frmInv==pCom->GetForm() ;
}

CGuiPic* CEquipMgr::GetChargePic( unsigned int n )	
{ 
	if( n<enumEQUIP_NUM )
		return &_imgCharges[n];		
	return NULL;
}

void CEquipMgr::_evtRMouseGridEvent(CGuiData *pSender,CCommandObj* pItem,int nGridID)
{
	if( !g_stUIBoat.GetHuman() ) return;

	CItemCommand* pItemCommand = dynamic_cast<CItemCommand*>(pItem);
	if( !pItemCommand || pItemCommand->GetItemInfo()->sType!=43 )
	{
		pItem->ExecRightClick();
		return;
	}

	stNetItemInfo info;
	info.chType = mission::VIEW_CHAR_BAG;
	info.sGridID = nGridID;
	CS_BeginAction( g_stUIBoat.GetHuman(), enumACTION_ITEM_INFO, &info );	
}

void CEquipMgr::_evtRepairEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	CS_ItemRepairAnswer( nMsgType==CForm::mrYes );
}

void CEquipMgr::ShowRepairMsg( const char* pItemName, long lMoney )
{
	char szBuf[255] = { 0 };
	sprintf( szBuf, g_oLangRec.GetString(559), pItemName, lMoney );
	g_stUIBox.ShowSelectBox( _evtRepairEvent, szBuf, true );	
}


void CEquipMgr::CloseAllForm()
{
	if(frmInv && frmInv->GetIsShow())
	{
		frmInv->SetIsShow(false);
	}

	if(frmSkill && frmSkill->GetIsShow())
	{
		frmSkill->SetIsShow(false);
	}
}


// 获得道具在当前背包中总的个数
int CEquipMgr::GetItemCount(int nID)
{
	int nRet = 0;
	for(int i = 0; i < grdItem->GetMaxNum(); ++i)
	{
		CItemCommand* pItem = dynamic_cast<CItemCommand*>(grdItem->GetItem(i));
		if(pItem && pItem->GetItemInfo())
		{
			if(pItem->GetItemInfo()->lID == nID)
			{
				nRet += pItem->GetTotalNum();
			}
		}
	}

	return nRet;
}



///////////////////////////////////////////////////////////////////////////////////
//
//  二次密码相关
//


// 锁定图片鼠标点击事件
extern	bool	_wait_select_lock_item_state_;

void CEquipMgr::_evtItemFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string strName = pSender->GetName();
	
	if(strName == "btnLock")
	{
		if(g_stUIEquip.GetIsLock())
		{
			// 解锁
			g_stUIDoublePwd.SetType(CDoublePwdMgr::PACKAGE_UNLOCK);
			g_stUIDoublePwd.ShowDoublePwdForm();
		}
		else
		{
			CBoxMgr::ShowSelectBox(_CheckLockMouseEvent, g_oLangRec.GetString(824), true);
		}
	}else if(strName == "btnLockItem"){
		CCursor::I()->SetCursor(	CCursor::stPick	);
		_wait_select_lock_item_state_	=	true;
	}
}


void CEquipMgr::SetIsLock(bool bLock)
{
	imgLock->SetIsShow(bLock);
	imgUnLock->SetIsShow(! bLock);
}


bool CEquipMgr::GetIsLock()
{
	return imgLock->GetIsShow();
}


// 加锁 MSGBOX 确认
void CEquipMgr::_CheckLockMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
    if( nMsgType == CForm::mrYes ) 
    {
		CS_LockKitbag();
    }
}
