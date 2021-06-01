#pragma once
#include "uiglobalvar.h"
#include "GuildData.h"
#include "UIPage.h"
#include "PacketCmd.h"
#include "procirculate.h"

class	CCharacter2D;

namespace GUI
{

class CUIGuildMgr :
	public CUIInterface
{
public:
	CUIGuildMgr(void);
	~CUIGuildMgr(void);
	static void ShowForm();
	static void RefreshList();
	static void RefreshAttribute();
	static void RefreshForm();
	static void RemoveForm();
	static void UpdateGuildQuests(LPRPACKET pk);
	static void UpdateGuildAttr(LPRPACKET pk);
	static void UpdateGuildPoints(int lv, int exp, int point);
	static void UpdateGuildLevel(int lv);
	static void SetActivePerm(int perm);
protected:
	virtual bool Init();
	

private:
	static CForm*		m_pGuildMgrForm;
	static CForm*		m_pGuildColForm;
	static CForm*		m_pGuildPermForm;
	static CLabelEx*	m_plabGuildName;
	static CLabelEx*	m_plabGuildMottoName;
	static CLabelEx*	m_plabGuildType;
	static CLabelEx*	m_plabGuildMaster;
	static CLabelEx*	m_plabGuildMemberCount;
	static CLabelEx*	m_plabGuildExperience;
	static CLabelEx*	m_plabGuildMoney;
	static CLabelEx*	m_plabGuildState;
	static CLabelEx*	m_plabGuildRemainTime;
	//static CLabelEx*	m_plabGuildRank;
	static CListView*	m_plstGuildMember;
	static CListView*	m_plstRecruitMember;
	static CTextButton*	m_pbtnGuildMottoEdit;
	static CTextButton*	m_pbtnGuildDismiss;
	static CTextButton*	m_pbtnMemberRecruit;
	static CTextButton*	m_pbtnMemberRefuse;
	static CTextButton*	m_pbtnMemberKick;
	static CTextButton*	m_pbtnMemberPerms;
	static CTextButton*	m_pbtnHouse;
	static CTextButton*	m_pbtnColour;
	static CTextButton*	m_pbtnMemberYesPerms;
	
	//guild attr button
	static CTextButton*	m_pbtnattrPR;
	static CTextButton*	m_pbtnattrMS;
	static CTextButton*	m_pbtnattrASPD;
	static CTextButton*	m_pbtnattrMX;
	static CTextButton*	m_pbtnattrDEF;
	static CTextButton*	m_pbtnattrHIT;
	static CTextButton*	m_pbtnattrFLEE;
	static CTextButton*	m_pbtnattrHPR;
	static CTextButton*	m_pbtnattrSPR;
	static CTextButton*	m_pbtnattrHP;
	static CTextButton*	m_pbtnattrSP;
	
	static CTextButton*	m_btnConfCol;
	static CTextButton*	m_btnNext;
	static CTextButton*	m_btnPrev;
	static CTextButton*	m_btnDisCol;
	
	static CTreeView*		m_trvPerm;
	
	static CEdit*	m_edtRed;
	static CEdit*	m_edtGreen;
	static CEdit*	m_edtBlue;
	static CImage* m_imgPreview;
	
	static CLabel*	m_gCircleCount;
	
	
	//models for guild quest
	static CCharacter2D* pMob[3];
	//static int questMobs[3];
	static void _MobRenderEvent(C3DCompent *pSender, int x, int y);
	
	
	static CPage*		m_ppgeClass;
	static void _OnClickEditMottoName(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnClickDismiss(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnClickRecruit(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnClickRefuse(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnClickKick(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnClickPerm(CGuiData *pSender, int x, int y, DWORD key);
	static void _EnterGuildHouse(CGuiData *pSender, int x, int y, DWORD key);
	
	static void _OnClickColour(CGuiData *pSender, int x, int y, DWORD key);
	static bool _UpdateColourPreview(CGuiData *pSender,char&key);
	//static void _UpdateColourPreview(CGuiData *pSender);
	static void _OnClickNext(CGuiData *pSender, int x, int y, DWORD key);
	static void _UpdateCirclePreview();
	static void _OnClickPrev(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnConfirmColour(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnConfirmRemoveColour(CGuiData *pSender, int x, int y, DWORD key);
	
	static void _OnClickConfirmPerm(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnClickSelectPage(CGuiData *pSender);
	static void _OnClickLeave(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _OnPassKick(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _OnPassDismiss(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _GuildStatUpgrade(CGuiData *pSender, int x, int y, DWORD key);
	static void _GuildStatUpgradeConfirm(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	
	static void _OnDragEnd(CGuiData *pSender, int x, int y, DWORD key );
	static void HideMobModel(int index);
	static void RefreshMobModel(int index);
	static bool RefreshMobModelPos(int index);

	static void _OnClickPermText(CGuiData *pSender, int x, int y, DWORD key);
	static void _evtPermFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

	static string attrName;
	
	
	static CForm*		m_pGuildMottoNameEditForm;
	static CEdit*		m_pedtGuildMottoName;
	static CTextButton*	m_pbtnGuildMottoFormOK;
	static void _OnClickMottoFormOK(CGuiData *pSender, int x, int y, DWORD key);

	static void OnBeforeShow(CForm* pForm,bool& IsShow);

	static previewCounter;
};

}
