#include "StdAfx.h"
#include "uiCozeform.h"
#include "uiformmgr.h"
#include "uiform.h"
#include "uicheckbox.h"
#include "uitextbutton.h"
#include "uilist.h"
#include "uigrid.h"
#include "uiedit.h"
#include "UITeam.h"
#include "UIChat.h"
#include "UICombo.h"
#include "gameapp.h"
#include "character.h"
#include "netchat.h"
#include "packetcmd.h"
#include "uiheadsay.h"
#include "uitextparse.h"
#include "StringLib.h"
#include "UIBoatForm.h"
#include "UIGlobalVar.h"
#include "UIMiniMapForm.h"


//const WORD					CCharMsg::m_wTotalChannelsCount;
//const DWORD					CCharMsg::m_dwChannelBufferSize;
CCharMsg::lstTextInfoType	CCharMsg::m_lstMsgLink;
CCharMsg::sChannelInfo		CCharMsg::m_sChannelInfo[m_wTotalChannelsCount]=
{
	{CHANNEL_NONE,		"",							0xFFFFFFFF,	"",		0,},				//CHANNEL_NONE
	{CHANNEL_ALL,		g_oLangRec.GetString(493),	0xFFFFFFFF,	"",		0,},				//CHANNEL_ALL
	{CHANNEL_SIGHT,		g_oLangRec.GetString(494),	0xFFFFFFFF,	"",		0,},				//CHANNEL_SIGHT
	{CHANNEL_PRIVATE,	g_oLangRec.GetString(495),	0xFFFFFFFF,	"@",	0,},				//CHANNEL_PRIVATE
	{CHANNEL_WORLD,		g_oLangRec.GetString(496),	0xFFFFFFFF,	"*",	0,},				//CHANNEL_WORLD
	{CHANNEL_TRADE,		g_oLangRec.GetString(497),	0xFFFFFFFF,	"^",	0,},				//CHANNEL_TRADE
	{CHANNEL_TEAM,		g_oLangRec.GetString(299),	0xFFFFFFFF,	"!",	0,},				//CHANNEL_TEAM
	{CHANNEL_GUILD,		g_oLangRec.GetString(468),	0xFFFFFFFF,	"%",	0,},				//CHANNEL_GUILD
	{CHANNEL_SYSTEM,	g_oLangRec.GetString(498),	0xFFFFFFFF,	"",		0,},				//CHANNEL_SYSTEM
	{CHANNEL_PUBLISH,	g_oLangRec.GetString(499),	0xFFFFFFFF,	"",		0,},				//CHANNEL_PUBLISH
	{CHANNEL_SIDE,		g_oLangRec.GetString(932),	0xFFFFFFFF,	"|",	0,},				//CHANNEL_SIDE
	{CHANNEL_VIP,		"VIP",						0XFFCEB02B,	"$",	0,},				//CHANNEL_SIDE
	{CHANNEL_DISCORD,	"Discord",					0XFFFF45FF,	"`",	0,},				//CHANNEL_SIDE
};

CCharMsg::lstInstanceType	CCharMsg::m_lstThisInstanceLink;

CCharMsg::CCharMsg()
{
	m_ecboShowChannels=CHANNEL_ALL|CHANNEL_SIGHT|CHANNEL_PRIVATE|CHANNEL_WORLD|CHANNEL_TRADE|CHANNEL_TEAM|CHANNEL_GUILD|CHANNEL_SYSTEM|CHANNEL_PUBLISH|CHANNEL_SIDE|CHANNEL_VIP|CHANNEL_DISCORD;
	m_bCurMsgAvailable=false;
	m_itCurrentMsgPos=m_lstMsgLink.end();
	m_lstThisInstanceLink.push_back(this);
}

CCharMsg::~CCharMsg()
{
	m_lstThisInstanceLink.remove(this);
}

WORD CCharMsg::GetChannelIndex(eChannel channel)
{
	DWORD dwChannel=(DWORD)channel;
	for (WORD i=1; i<=m_wTotalChannelsCount; i++)
	{
		if (dwChannel&1)
			return i;
		dwChannel=dwChannel>>1;
	}
	return 0;
}

void CCharMsg::SetChannelColor(eChannel channel, DWORD dwColor)
{
	m_sChannelInfo[GetChannelIndex(channel)].dwChannelColor=dwColor;
}

DWORD CCharMsg::GetChannelColor(eChannel channel)
{
	return m_sChannelInfo[GetChannelIndex(channel)].dwChannelColor;
}

string CCharMsg::GetChannelCommand(WORD wChannelIndex)
{
	return m_sChannelInfo[wChannelIndex].strChannelCommand;
}

string CCharMsg::GetChannelName(eChannel channel)
{
	return m_sChannelInfo[GetChannelIndex(channel)].strChannelName;
}

string CCharMsg::GetChannelShowName(eChannel channel)
{
	return "["+m_sChannelInfo[GetChannelIndex(channel)].strChannelName+"]";
}

CCharMsg::eChannel CCharMsg::GetChannelByIndex(WORD wChannelIndex)
{
	return m_sChannelInfo[wChannelIndex].enumChannel;
}

WORD CCharMsg::GetTotalChannelCount()
{
	return m_wTotalChannelsCount;
}

void CCharMsg::AddMsg(eChannel channel, string strWho, string strText, bool bSendTo,DWORD dwcolour)
{
	CTeam* pTeam = g_stUIChat.GetTeamMgr()->Find(enumTeamBlocked);
	for (int i = 0; i < pTeam->GetCount(); i++)
	{
		CMember* pMember = pTeam->GetMember(i);
		if (pMember->GetName() == strWho)
		{
			return;
		}
	}

	sTextInfo sText;
	if (channel == CHANNEL_PRIVATE)
	{
		if (bSendTo)
		{
			sText.strShowText=GetChannelShowName(channel)+"<To "+strWho+">:";
		}
		else
		{
			sText.strShowText=GetChannelShowName(channel)+"<From "+strWho+">:";
		}
	}
	else
	{
		sText.strShowText=GetChannelShowName(channel)+strWho;
		if (!strWho.empty())
		{
			sText.strShowText+=":";
		}
	}
	sText.strShowText+=strText;
	sText.eTextChannel=channel;
	sText.strText=strText;
	sText.bSendTo=bSendTo;
	sText.strWho=strWho;
	sText.dwColour = dwcolour;

	WORD wChannelIndex=GetChannelIndex(channel);
	if (m_sChannelInfo[wChannelIndex].dwTotalMsg >= m_dwChannelBufferSize)
	{
		//��������
		lstTextInfoType::iterator iterRemoveObject=m_sChannelInfo[wChannelIndex].itFirstMsgPos;
		while(m_sChannelInfo[wChannelIndex].itFirstMsgPos != m_lstMsgLink.end())
		{
			m_sChannelInfo[wChannelIndex].itFirstMsgPos++;
			sTextInfo sTempText=*(m_sChannelInfo[wChannelIndex].itFirstMsgPos);
			if (wChannelIndex == GetChannelIndex(sTempText.eTextChannel))
			{
				break;
			}
		}
		if (iterRemoveObject != m_lstMsgLink.end())
		{
			for (lstInstanceType::iterator it=m_lstThisInstanceLink.begin(); it!=m_lstThisInstanceLink.end(); it++)
			{
				if (iterRemoveObject == (*it)->m_itCurrentMsgPos)
				{
					(*it)->m_bCurMsgAvailable=false;
					if ((*it)->m_itCurrentMsgPos == m_lstMsgLink.begin())
					{
						(*it)->m_itCurrentMsgPos = m_lstMsgLink.end();
					}
					else
					{
						(*it)->m_itCurrentMsgPos--;
					}
				}
			}
			m_lstMsgLink.erase(iterRemoveObject);
		}
		m_sChannelInfo[wChannelIndex].dwTotalMsg--;
	}
	
	m_lstMsgLink.push_back(sText);

	if (m_sChannelInfo[wChannelIndex].dwTotalMsg++ == 0)
	{
		m_sChannelInfo[wChannelIndex].itFirstMsgPos = m_lstMsgLink.end()--;
	}
}

void CCharMsg::ClearAllMsg()
{
	m_lstMsgLink.clear();
	for (WORD i=0; i<m_wTotalChannelsCount; i++)
	{
		m_sChannelInfo[i].dwTotalMsg = 0;
		m_sChannelInfo[i].itFirstMsgPos = m_lstMsgLink.end();
	}
	for (lstInstanceType::iterator it=m_lstThisInstanceLink.begin(); it!=m_lstThisInstanceLink.end(); it++)
	{
		(*it)->m_bCurMsgAvailable=false;
		(*it)->m_itCurrentMsgPos = m_lstMsgLink.end();
	}
}

void CCharMsg::SetShowChannels(DWORD ecboShowChannels)
{
	m_ecboShowChannels=ecboShowChannels;
}

bool CCharMsg::ModifyShowChannel(eChannel eShowChannel, bool bAddOrRemove, bool bShowTips)
{
	bool bOrgShow=(m_ecboShowChannels&eShowChannel)?true:false;
	if (bOrgShow != bAddOrRemove)
	{
		if (bAddOrRemove)
		{
			SetShowChannels(m_ecboShowChannels|eShowChannel);
			if (bShowTips)
			{
				CCozeForm::GetInstance()->OnSystemMsg(GetChannelName(eShowChannel)+g_oLangRec.GetString(500));
			}
		}
		else
		{
			SetShowChannels(m_ecboShowChannels&~eShowChannel);
			if (bShowTips)
			{
				CCozeForm::GetInstance()->OnSystemMsg(GetChannelName(eShowChannel)+g_oLangRec.GetString(501));
			}
		}
		return true;
	}
	return false;
}

bool CCharMsg::IsShowChannel(eChannel channel)
{
	return (m_ecboShowChannels&channel)?true:false;
}

DWORD CCharMsg::GetShowChannels()
{
	return m_ecboShowChannels;
}

bool CCharMsg::MoveToFirstMsg()
{
	m_itCurrentMsgPos = m_lstMsgLink.begin();
	m_bCurMsgAvailable = !m_lstMsgLink.empty();
	if (!m_bCurMsgAvailable) return false;
	if ((*m_itCurrentMsgPos).eTextChannel&m_ecboShowChannels)
	{
		return true;
	}
	else
	{
		return MoveToNextMsg();
	}
}

bool CCharMsg::MoveToNextMsg()
{
	if (m_itCurrentMsgPos == m_lstMsgLink.end())
	{
		if (m_lstMsgLink.empty())
		{
			return false;
		}
		m_itCurrentMsgPos = m_lstMsgLink.begin();
		if ((*m_itCurrentMsgPos).eTextChannel&m_ecboShowChannels)
		{
			m_bCurMsgAvailable=true;
			return true;
		}
	}

	lstTextInfoType::iterator iterTemp=m_itCurrentMsgPos;
	while(++iterTemp != m_lstMsgLink.end())
	{
		sTextInfo sTempText=*(iterTemp);
		if (sTempText.eTextChannel&m_ecboShowChannels)
		{
			m_bCurMsgAvailable=true;
			m_itCurrentMsgPos = iterTemp;
			return true;
		}
	}
	return false;
}

CCharMsg::sTextInfo CCharMsg::GetMsgInfo()
{
	sTextInfo sText;
	sText.eTextChannel=CHANNEL_ALL;
	sText.strWho=g_oLangRec.GetString(2);
	sText.strText=g_oLangRec.GetString(502);
	sText.strShowText=g_oLangRec.GetString(502);
	sText.bSendTo=false;
	if (m_bCurMsgAvailable && m_itCurrentMsgPos != m_lstMsgLink.end())
	{
		sText=*m_itCurrentMsgPos;
	}
	return sText;
}

CCardCase::CCardCase(WORD wMaxLimit)
{
	m_wMaxLimit=wMaxLimit;
	MoveToFirstCard();
}

CCardCase::~CCardCase()
{
}

void CCardCase::AddCard(string str)
{
	if (str.empty()) return;

	for (lstCardType::iterator it=m_lstCardData.begin();it!=m_lstCardData.end();it++)
	{
		if ((*it)==str)
		{
			m_lstCardData.erase(it);
			m_lstCardData.push_back(str);
			return;
		}
	}
	
	m_lstCardData.push_back(str);
	if ((WORD)m_lstCardData.size()>m_wMaxLimit)
	{
		m_lstCardData.erase(m_lstCardData.begin());
	}
}

bool CCardCase::RemoveCard(string str)
{
	for (lstCardType::iterator it=m_lstCardData.begin();it!=m_lstCardData.end();it++)
	{
		if ((*it)==str)
		{
			m_lstCardData.erase(it);
			return true;
		}
	}
	return false;
}

void CCardCase::ClearAll()
{
	m_lstCardData.clear();
	MoveToFirstCard();
}

int	CCardCase::GetTotalCount()
{
	return (int)m_lstCardData.size();
}

bool CCardCase::MoveToFirstCard()
{
	m_itCurrentCardPos=m_lstCardData.begin();
	if (GetTotalCount()==0)
	{
		return false;
	}
	return true;
}

bool CCardCase::MoveToNextCard()
{
	lstCardType::iterator iterTemp=m_itCurrentCardPos;
	if (++iterTemp == m_lstCardData.end())
	{
		return false;
	}
	m_itCurrentCardPos=iterTemp;
	return true;
}

bool CCardCase::MoveToLastCard()
{
	m_itCurrentCardPos=m_lstCardData.end();
	if (GetTotalCount()==0)
	{
		return false;
	}
	--m_itCurrentCardPos;
	return true;
}

bool CCardCase::MoveToPrevCard()
{
	if (m_itCurrentCardPos==m_lstCardData.begin())
	{
		return false;
	}
	--m_itCurrentCardPos;
	return true;
}

string CCardCase::GetCardInfo()
{
	if (GetTotalCount()==0)
	{
		return "";
	}

	return *m_itCurrentCardPos;
}



using namespace GUI;


CChannelSwitchForm::CChannelSwitchForm()
{
}

CChannelSwitchForm::~CChannelSwitchForm()
{
}

extern CChannelSwitchForm g_stUIChannelSwitch;
CChannelSwitchForm* CChannelSwitchForm::GetInstance()
{
	//static CChannelSwitchForm s_ChannelSwitchForm;
	return (&g_stUIChannelSwitch);
}

void CChannelSwitchForm::SwitchCheck()
{
	CCozeForm *pCozeForm=CCozeForm::GetInstance();
	if (m_frmChannelSwitch->GetIsShow())
	{
		pCozeForm->m_chkChannelSwitch->SetIsChecked(false);
		m_frmChannelSwitch->SetIsShow(false);
	}
	else
	{
		CChannelSwitchForm::GetInstance()->m_chkPrivate->SetIsChecked(pCozeForm->m_cMainMsg.IsShowChannel(CCharMsg::CHANNEL_PRIVATE));
		CChannelSwitchForm::GetInstance()->m_chkSight->SetIsChecked(pCozeForm->m_cMainMsg.IsShowChannel(CCharMsg::CHANNEL_SIGHT));
		CChannelSwitchForm::GetInstance()->m_chkSystem->SetIsChecked(pCozeForm->m_lstSystemPage->GetIsShow());
		CChannelSwitchForm::GetInstance()->m_chkTeam->SetIsChecked(pCozeForm->m_cMainMsg.IsShowChannel(CCharMsg::CHANNEL_TEAM));
		CChannelSwitchForm::GetInstance()->m_chkGuild->SetIsChecked(pCozeForm->m_cMainMsg.IsShowChannel(CCharMsg::CHANNEL_GUILD));
		CChannelSwitchForm::GetInstance()->m_chkWorld->SetIsChecked(pCozeForm->m_cMainMsg.IsShowChannel(CCharMsg::CHANNEL_WORLD));
		CChannelSwitchForm::GetInstance()->m_chkTrade->SetIsChecked(pCozeForm->m_cMainMsg.IsShowChannel(CCharMsg::CHANNEL_TRADE));
		CChannelSwitchForm::GetInstance()->m_chkVIP->SetIsChecked(pCozeForm->m_cMainMsg.IsShowChannel(CCharMsg::CHANNEL_VIP));
		CChannelSwitchForm::GetInstance()->m_chkDiscord->SetIsChecked(pCozeForm->m_cMainMsg.IsShowChannel(CCharMsg::CHANNEL_DISCORD));

		pCozeForm->m_chkChannelSwitch->SetIsChecked(true);
		m_frmChannelSwitch->SetIsShow(true);
	}
}

bool CChannelSwitchForm::Init()
{
	FORM_LOADING_CHECK(m_frmChannelSwitch,"main.clu","frmForbid");
	m_frmChannelSwitch->evtLost=EventLostFocus;
	FORM_CONTROL_LOADING_CHECK(m_chkPrivate,m_frmChannelSwitch,CCheckBox,"main.clu","chkPersonal");
	FORM_CONTROL_LOADING_CHECK(m_chkSight,m_frmChannelSwitch,CCheckBox,"main.clu","chkNear");
	FORM_CONTROL_LOADING_CHECK(m_chkSystem,m_frmChannelSwitch,CCheckBox,"main.clu","chkSystem");
	FORM_CONTROL_LOADING_CHECK(m_chkTeam,m_frmChannelSwitch,CCheckBox,"main.clu","chkTeam");
	FORM_CONTROL_LOADING_CHECK(m_chkGuild,m_frmChannelSwitch,CCheckBox,"main.clu","chkGuild");
	FORM_CONTROL_LOADING_CHECK(m_chkWorld,m_frmChannelSwitch,CCheckBox,"main.clu","chkWorld");
	FORM_CONTROL_LOADING_CHECK(m_chkTrade,m_frmChannelSwitch,CCheckBox,"main.clu","chkTrade");
	FORM_CONTROL_LOADING_CHECK(m_chkDiscord,m_frmChannelSwitch,CCheckBox,"main.clu","chkDiscord");
	FORM_CONTROL_LOADING_CHECK(m_chkVIP,m_frmChannelSwitch,CCheckBox,"main.clu","chkVIP");
	
	m_chkPrivate->evtCheckChange=EventPrivateCheckChange;
	m_chkSight->evtCheckChange=EventSightCheckChange;
	m_chkSystem->evtCheckChange=EventSystemCheckChange;
	m_chkTeam->evtCheckChange=EventTeamCheckChange;
	m_chkGuild->evtCheckChange=EventGuildCheckChange;
	m_chkWorld->evtCheckChange=EventWorldCheckChange;
	m_chkTrade->evtCheckChange=EventTradeCheckChange;
	m_chkDiscord->evtCheckChange=EventDiscordCheckChange;
	m_chkVIP->evtCheckChange=EventVIPCheckChange;

	return true;
}

void CChannelSwitchForm::EventLostFocus(CGuiData *pSender)
{
	CCozeForm *pCozeForm=CCozeForm::GetInstance();
	pCozeForm->m_chkChannelSwitch->SetIsChecked(false);
	pCozeForm->m_chkChannelSwitch->MouseRun(pCozeForm->m_chkChannelSwitch->GetX()-1,pCozeForm->m_chkChannelSwitch->GetY()-1,0);
	CChannelSwitchForm::GetInstance()->m_frmChannelSwitch->SetIsShow(false);
}

void CChannelSwitchForm::EventPrivateCheckChange(CGuiData *pSender)
{
	CCozeForm *pCozeForm=CCozeForm::GetInstance();
	pCozeForm->m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_PRIVATE, CChannelSwitchForm::GetInstance()->m_chkPrivate->GetIsChecked());
	pCozeForm->ResetPages();
}

void CChannelSwitchForm::EventSightCheckChange(CGuiData *pSender)
{
	CCozeForm *pCozeForm=CCozeForm::GetInstance();
	pCozeForm->m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_SIGHT, CChannelSwitchForm::GetInstance()->m_chkSight->GetIsChecked());
	pCozeForm->ResetPages();
}

void CChannelSwitchForm::EventSystemCheckChange(CGuiData *pSender)
{
	CCozeForm *pCozeForm=CCozeForm::GetInstance();
	bool bCheck=CChannelSwitchForm::GetInstance()->m_chkSystem->GetIsChecked();
	pCozeForm->m_lstSystemPage->SetIsShow(bCheck);
	pCozeForm->m_drgSystemPage->SetIsShow(bCheck);
	if (bCheck)
	{
		pCozeForm->OnSystemMsg(g_oLangRec.GetString(503));
	}
	else
	{
		pCozeForm->OnSystemMsg(g_oLangRec.GetString(504));
	}
	pCozeForm->ResetPages();
}

void CChannelSwitchForm::EventTeamCheckChange(CGuiData *pSender)
{
	CCozeForm *pCozeForm=CCozeForm::GetInstance();
	pCozeForm->m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_TEAM, CChannelSwitchForm::GetInstance()->m_chkTeam->GetIsChecked());
	pCozeForm->ResetPages();
}

void CChannelSwitchForm::EventGuildCheckChange(CGuiData *pSender)
{
	CCozeForm *pCozeForm=CCozeForm::GetInstance();
	pCozeForm->m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_GUILD, CChannelSwitchForm::GetInstance()->m_chkGuild->GetIsChecked());
	pCozeForm->ResetPages();
}

void CChannelSwitchForm::EventWorldCheckChange(CGuiData *pSender)
{
	CCozeForm *pCozeForm=CCozeForm::GetInstance();
	pCozeForm->m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_WORLD, CChannelSwitchForm::GetInstance()->m_chkWorld->GetIsChecked());
	pCozeForm->ResetPages();
}

void CChannelSwitchForm::EventTradeCheckChange(CGuiData *pSender)
{
	CCozeForm *pCozeForm=CCozeForm::GetInstance();
	pCozeForm->m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_TRADE, CChannelSwitchForm::GetInstance()->m_chkTrade->GetIsChecked());
	pCozeForm->ResetPages();
}

void CChannelSwitchForm::EventVIPCheckChange(CGuiData *pSender)
{
	CCozeForm *pCozeForm=CCozeForm::GetInstance();
	pCozeForm->m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_VIP, CChannelSwitchForm::GetInstance()->m_chkVIP->GetIsChecked());
	pCozeForm->ResetPages();
}

void CChannelSwitchForm::EventDiscordCheckChange(CGuiData *pSender)
{
	CCozeForm *pCozeForm=CCozeForm::GetInstance();
	bool bCheck=CChannelSwitchForm::GetInstance()->m_chkDiscord->GetIsChecked();
	pCozeForm->m_lstDiscordPage->SetIsShow(bCheck);
	pCozeForm->m_drgDiscordPage->SetIsShow(bCheck);
	
	
	CList* pList=pCozeForm->m_lstSystemPage;
	
	if (bCheck){//off->on
		pCozeForm->OnSystemMsg("Opened Discord Message Channel");
		pList->SetPos(pList->GetLeft(),  pCozeForm->m_lstDiscordPage->GetTop()- pList->GetHeight());
	}else{//on->off
		pCozeForm->OnSystemMsg("Closed Discord Message Channel");
		pList->SetPos(pList->GetLeft(),  pCozeForm->m_lstDiscordPage->GetBottom()- pList->GetHeight());
	}
	pList->Refresh();
	pCozeForm->m_drgSystemPage->SetPos(pList->GetLeft(), pList->GetTop() - 4);
	pCozeForm->m_drgSystemPage->Refresh();
	pCozeForm->ResetPages();
}






const int	CCozeForm::m_nMainPageMinHeight=70;
const int	CCozeForm::m_nMainPageMaxHeight=280;
const int	CCozeForm::m_nSystemPageMinHeight=55;
const int	CCozeForm::m_nSystemPageMaxHeight=240;

CCozeForm::CCozeForm() : m_cCallingCard(10), m_cSendMsgCard(10), m_bSendMsgCardSwitch(false)
{
	m_eCurSelChannel=CCharMsg::CHANNEL_SIGHT;
}

CCozeForm::~CCozeForm()
{
}


extern CCozeForm g_stUICozeForm;
CCozeForm* CCozeForm::GetInstance()
{
	//static CCozeForm s_CozeForm;
	return &g_stUICozeForm;
}

void CCozeForm::OnVIPMsg(string strName, string strMsg,DWORD dwColour){
	CCharMsg::AddMsg(CCharMsg::CHANNEL_VIP, strName, strMsg,false,dwColour);	
	UpdatePages();
}

void CCozeForm::OnDiscordMsg(string strName, string strMsg,DWORD dwColour){
	CCharMsg::AddMsg(CCharMsg::CHANNEL_DISCORD, strName, strMsg,false,dwColour);	
	UpdatePages();
}

void CCozeForm::OnSightMsg(string strName, string strMsg,DWORD dwColour)
{
	CCharMsg::AddMsg(CCharMsg::CHANNEL_SIGHT, strName, strMsg,false,dwColour);	
	UpdatePages();
}

void CCozeForm::OnSightMsg(CCharacter* pChar, string strMsg,DWORD dwColour)
{
	if (!pChar) return;

	std::string sender = pChar->getHumanName();
	if (g_stUIChat.GetTeamMgr()->Find(enumTeamBlocked)->FindByName(sender.c_str()))
		return;
	
	char buf[10];
	//for (int i=0; i<=9; i++)
	for (int i=0; i<=50; i++)//Modify by sunny.sun 20080902
	{
		sprintf(buf, "***%d", i);
		if (strMsg==buf)
		{
			pChar->GetHeadSay()->SetFaceID(i);
			return;
		}
	}
	
	//Add by sunny.sun 20080902
	//Begin
	const string pcnsenderface = "#21";//�������ղ������ַ���
	const string preplaceface = "#99";//�滻���͵��ַ���
	ReplaceSpecialFace(strMsg,pcnsenderface,preplaceface);//�滻�ַ���
	//End
	CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
	OnSightMsg(pChar->IsPlayer() ? pChar->getHumanName() : pChar->getName(), strMsg,dwColour);

	string str = string(pChar->IsPlayer() ? pChar->getHumanName() : pChar->getName())+": "+strMsg;// 需要修�? ning.yan 2008-11-07
	CItemEx *item = new CItemEx(str.c_str(), CCharMsg::GetChannelColor(CCharMsg::CHANNEL_SIGHT));
	if (strlen(str.c_str()) > 32)  //�������?32���ַ�����������?
	{
		item->SetIsMultiLine(true);
		//item->ProcessString((int)strlen(pChar->getHumanName())+1);
		item->ProcessString((int)strlen(pChar->IsPlayer() ? pChar->getHumanName() : pChar->getName())+1);
	}
	if (str.find("#") != -1)
	{
		item->SetIsParseText(true);	
	}
	pChar->GetHeadSay()->AddItem(item);
}

void CCozeForm::OnPrivateMsg(string strFromName, string strToName, string strMsg,DWORD dwColour)
{
	if( !CGameScene::GetMainCha() ) return;

	CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
	string strHumanName=CGameScene::GetMainCha()->getHumanName();
	if (strHumanName==strFromName)
	{
		if (strMsg!="{x}*")
		{
			CCharMsg::AddMsg(CCharMsg::CHANNEL_PRIVATE, strToName, strMsg, true,dwColour);
			m_cCallingCard.AddCard(strToName);
		}
	}
	else
	{
		if (strMsg=="{x}*")
		{
			OnSystemMsg(g_oLangRec.GetString(505)+strFromName+g_oLangRec.GetString(506));
		}
		else
		{
			CCharMsg::AddMsg(CCharMsg::CHANNEL_PRIVATE, strFromName, strMsg, false,dwColour);
			if (m_cMainMsg.IsShowChannel(CCharMsg::CHANNEL_PRIVATE))
			{
				m_cCallingCard.AddCard(strFromName);
			}
			else
			{
				string strContent="{x}*";
				CS_Say2You(strFromName.c_str(), strContent.c_str());
			}
		}
	}
	UpdatePages();
}

void CCozeForm::OnWorldMsg(string strName, string strMsg,DWORD dwColour)
{
	CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
	CCharMsg::AddMsg(CCharMsg::CHANNEL_WORLD, strName, strMsg,false,dwColour);
	UpdatePages();
}

void CCozeForm::OnTradeMsg(string strName, string strMsg,DWORD dwColour)
{
	CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
	CCharMsg::AddMsg(CCharMsg::CHANNEL_TRADE, strName, strMsg,false,dwColour);
	UpdatePages();
}

void CCozeForm::OnTeamMsg(string strName, string strMsg,DWORD dwColour)
{
	CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
	CCharMsg::AddMsg(CCharMsg::CHANNEL_TEAM, strName, strMsg,false,dwColour);
	UpdatePages();
}

void CCozeForm::OnTeamMsg(DWORD dwCharID, string strMsg,DWORD dwColour)
{
	if( !CGameScene::GetMainCha() ) return;

	string strName;
	if ( dwCharID!=CGameScene::GetMainCha()->getHumanID() )
	{
		CTeam *pTeam=g_stUIChat.GetTeamMgr()->Find(enumTeamGroup);
		if (!pTeam) return;
		CMember *pMember=pTeam->Find(dwCharID);
		if (!pMember) return;
		strName=pMember->GetName();
	}
	else
	{
		strName=CGameScene::GetMainCha()->getHumanName();
	}
	OnTeamMsg(strName, strMsg,dwColour);
}

void CCozeForm::OnGuildMsg(string strName, string strMsg,DWORD dwColour)
{
	CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
	CCharMsg::AddMsg(CCharMsg::CHANNEL_GUILD, strName, strMsg,false,dwColour);
	UpdatePages();
}

void CCozeForm::OnSystemMsg(string strMsg)
{
	if (GuildSysInfo)
		CCharMsg::SetChannelColor(CCharMsg::CHANNEL_SYSTEM, 0xFF00FFFF);
	else
		CCharMsg::SetChannelColor(CCharMsg::CHANNEL_SYSTEM, 0xFFFFFF00);

	CCharMsg::AddMsg(CCharMsg::CHANNEL_SYSTEM, "", strMsg);
	UpdatePages();
}

void CCozeForm::OnSideMsg(string strName, string strMsg,DWORD dwColour)
{
	CCharMsg::AddMsg(CCharMsg::CHANNEL_SIDE, strName, strMsg,false,dwColour);
	UpdatePages();
}

void CCozeForm::OnPublishMsg(string strName, string strMsg)
{
	string str;
	str=g_oLangRec.GetString(507)+string(strMsg);
	g_pGameApp->ShowNotify(str.c_str(), CCharMsg::GetChannelColor(CCharMsg::CHANNEL_PUBLISH));
}
//Add by sunny.sun20080804
void CCozeForm::OnPublishMsg1(string strMsg,int setnum)
{
	string str;
	str=string(strMsg);
	g_pGameApp->ShowNotify1(str.c_str(),setnum, CCharMsg::GetChannelColor(CCharMsg::CHANNEL_PUBLISH));
}
//End
void CCozeForm::OnPrivateNameSet(string strName)
{
	if (strName.empty()) return;

	string strCurCmd=CCharMsg::GetChannelCommand(CCharMsg::GetChannelIndex(CCharMsg::CHANNEL_PRIVATE))+strName+" ";
	m_edtMsg->SetCaption(strCurCmd.c_str());
	m_edtMsg->SetActive(m_edtMsg);

	m_cCallingCard.AddCard(strName);
}

void CCozeForm::OnResetAll()
{
	m_chkChannelSwitch->SetIsChecked(false);
	m_lstCallingCard->SetIsShow(false);
	m_grdFacePanel->SetIsShow(false);
	m_grdBrowPanel->SetIsShow(false);
	m_grdActionPanel->SetIsShow(false);

	m_eCurSelChannel=CCharMsg::CHANNEL_SIGHT;
	m_cmbChannel->GetList()->GetItems()->Select(0);
	m_edtMsg->SetCaption("");
	m_lstMainPage->GetItems()->Clear();
	m_lstSystemPage->GetItems()->Clear();
	m_cMainMsg.ClearAllMsg();
	m_cSystemMsg.ClearAllMsg();
	m_cCallingCard.ClearAll();
	m_cSendMsgCard.ClearAll();
	m_bSendMsgCardSwitch=false;
}

bool CCozeForm::IsMouseOnList(int x, int y)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	if (pThis->m_lstSystemPage->GetScroll()->IsNormal())
	{
		GetRender().ScreenConvert(x, y);
		if(pThis->m_lstSystemPage->GetScroll()->InRect(x, y)) return false;
	}
	if (pThis->m_lstMainPage->GetScroll()->IsNormal())
	{
		GetRender().ScreenConvert(x, y);
		if(pThis->m_lstMainPage->GetScroll()->InRect(x, y)) return false;
	}
	return true;
}

void CCozeForm::AddToEdit(string strData)
{
	CEdit* pEdit=dynamic_cast<CEdit*>(CCompent::GetActive());
	if (!pEdit)
	{
		if (!m_edtMsg) return;
		pEdit = m_edtMsg;
	}

	string strMsg=pEdit->GetCaption();
	strMsg+=strData;
	if ((int)strlen(strMsg.c_str()) > pEdit->GetMaxNum()) return;
	pEdit->SetCaption(strMsg.c_str());
}
//Add by sunny.sun 20080902
//Begin
//��strMsg�е�strFace�滻ΪstrReplace
void CCozeForm::ReplaceSpecialFace( string &strMsg, const string & strReplace, const string & strFace )
{
	int  nPos=strMsg.find(strFace);

	while (nPos>=0)
	{
		strMsg.replace(nPos,strFace.length(),strReplace,0,strReplace.length());
		nPos=strMsg.find(strFace,nPos+(strReplace.length()));
	}
}
//End

bool CCozeForm::Init()
{
	m_frmMainChat = _FindForm("frmMain800");
	if( !m_frmMainChat ) return false;
	CFormMgr::s_Mgr.AddKeyDownEvent( EventGlobalKeyDownHandle );

	FORM_LOADING_CHECK(m_frmPublish, "main.clu", "frmGM");
	m_frmPublish->evtBeforeShow=EventPublishShowForm;

	FORM_CONTROL_LOADING_CHECK(m_edtPublishMsg, m_frmPublish, CEdit, "coze.clu", "edtGMSay");
	m_edtPublishMsg->SetIsWrap(true);
	m_edtPublishMsg->evtEnter=EventPublishSendMsg;

	FORM_CONTROL_LOADING_CHECK(m_edtMsg,m_frmMainChat,CEdit,"main.clu","edtSay");
	m_edtMsg->SetIsWrap(true);
	m_edtMsg->SetIsParseText(true);
	m_edtMsg->evtEnter=EventSendMsg;
	m_edtMsg->evtKeyDown=EventEditMsg;

	FORM_CONTROL_LOADING_CHECK(m_cmbChannel,m_frmMainChat,CCombo,"main.clu","cboChannel");
	m_cmbChannel->evtSelectChange=EventSendChannelChange;
	m_cmbChannel->evtMouseClick=EventSendChannelSwitchClick;
	m_cmbChannel->GetList()->GetItems()->Select(0); 

	FORM_CONTROL_LOADING_CHECK(m_lstMainPage,m_frmMainChat,CList,"main.clu","lstOnSay");
	m_lstMainPage->SetIsChangeColor(false);
	m_lstMainPage->SetMouseAction( enumMA_Drill );
	m_lstMainPage->SetRowHeight(16);
	m_lstMainPage->evtListMouseDB=EventMainListKeyDown;

	FORM_CONTROL_LOADING_CHECK(m_lstSystemPage,m_frmMainChat,CList,"main.clu","lstOnSaySystem");
	m_lstSystemPage->SetIsChangeColor(false);
	m_lstSystemPage->SetMouseAction( enumMA_Drill );
	m_lstSystemPage->SetRowHeight(16);
	m_cSystemMsg.SetShowChannels(CCharMsg::CHANNEL_SYSTEM);
	
	FORM_CONTROL_LOADING_CHECK(m_lstDiscordPage,m_frmMainChat,CList,"main.clu","lstOnSayDiscord");
	m_lstDiscordPage->SetIsChangeColor(false);
	m_lstDiscordPage->SetMouseAction( enumMA_Drill );
	m_lstDiscordPage->SetRowHeight(16);
	m_lstDiscordPage->evtListMouseDB=EventDiscordListKeyDown;

	m_cDiscordMsg.SetShowChannels(CCharMsg::CHANNEL_DISCORD);
	
	FORM_CONTROL_LOADING_CHECK(m_drgMainPage,m_frmMainChat,CDragTitle,"main.clu","drpTitle");
	m_drgMainPage->SetIsShowDrag(false);
	m_drgMainPage->GetDrag()->SetYare(0);
	m_drgMainPage->GetDrag()->SetDragInCursor(CCursor::stVertical);
	m_drgMainPage->GetDrag()->evtMouseDragBegin = EventMainPageDragBegin;
	m_drgMainPage->GetDrag()->evtMouseDragMove = EventMainPageDragging;
	m_drgMainPage->GetDrag()->evtMouseDragEnd = EventMainPageDragEnd;
	
	FORM_CONTROL_LOADING_CHECK(m_drgSystemPage,m_frmMainChat,CDragTitle,"main.clu","drpTitleSystem");
	m_drgSystemPage->SetIsShowDrag(false);
	m_drgSystemPage->GetDrag()->SetYare(0);
	m_drgSystemPage->GetDrag()->SetDragInCursor(CCursor::stVertical);
	m_drgSystemPage->GetDrag()->evtMouseDragBegin = EventSystemPageDragBegin;
	m_drgSystemPage->GetDrag()->evtMouseDragMove = EventSystemPageDragging;
	m_drgSystemPage->GetDrag()->evtMouseDragEnd = EventSystemPageDragEnd;	
	
	FORM_CONTROL_LOADING_CHECK(m_drgDiscordPage,m_frmMainChat,CDragTitle,"main.clu","drpTitleDiscord");
	m_drgDiscordPage->SetIsShowDrag(false);
	m_drgDiscordPage->GetDrag()->SetYare(0);
	m_drgDiscordPage->GetDrag()->SetDragInCursor(CCursor::stVertical);
	m_drgDiscordPage->GetDrag()->evtMouseDragBegin = EventDiscordPageDragBegin;
	m_drgDiscordPage->GetDrag()->evtMouseDragMove = EventDiscordPageDragging;
	m_drgDiscordPage->GetDrag()->evtMouseDragEnd = EventDiscordPageDragEnd;
	
	FORM_CONTROL_LOADING_CHECK(m_chkChannelSwitch,m_frmMainChat,CCheckBox,"main.clu","chkChatOn");
	m_chkChannelSwitch->SetIsChecked(false);
	m_chkChannelSwitch->evtCheckChange=EventChannelSwitchCheck;

	FORM_CONTROL_LOADING_CHECK(m_btnCallingCardSwitch,m_frmMainChat,CTextButton,"main.clu","btnCard");
	m_btnCallingCardSwitch->evtMouseClick=EventCallingCardSwitchClick;		//switch

	FORM_CONTROL_LOADING_CHECK(m_lstCallingCard,m_frmMainChat,CList,"main.clu","lstCard");
	//m_lstCallingCard->evtLost=EventCallingCardLostFocus; -arcol ��Ч�¼�
	m_lstCallingCard->evtSelectChange=EventCardSelected;
	m_lstCallingCard->SetIsChangeColor(false);

	FORM_CONTROL_LOADING_CHECK(m_btnFaceSwitch,m_frmMainChat,CTextButton,"main.clu","btnChatFace");
	m_btnFaceSwitch->evtMouseClick=EventFacePanelSwitchClick;		//switch

	FORM_CONTROL_LOADING_CHECK(m_grdFacePanel,m_frmMainChat,CGrid,"main.clu","grdFace");
	//m_grdFacePanel->evtLost=EventFacePanelLostFocus;
	m_grdFacePanel->evtSelectChange=EventFaceSelected;

	FORM_CONTROL_LOADING_CHECK(m_btnBrowSwitch,m_frmMainChat,CTextButton,"main.clu","btnBrow");
	m_btnBrowSwitch->evtMouseClick=EventBrowPanelSwitchClick;		//switch

	FORM_CONTROL_LOADING_CHECK(m_grdBrowPanel,m_frmMainChat,CGrid,"main.clu","grdHeart");
	m_grdBrowPanel->evtSelectChange=EventBrowSelected;

	FORM_CONTROL_LOADING_CHECK(m_btnActionSwitch,m_frmMainChat,CTextButton,"main.clu","btnAction");
	m_btnActionSwitch->evtMouseClick=EventActionPanelSwitchClick;		//switch

	FORM_CONTROL_LOADING_CHECK(m_grdActionPanel,m_frmMainChat,CGrid,"main.clu","grdAction");
	m_grdActionPanel->evtSelectChange=EventActionSelected;


	m_frmMainChat->SetIsShow(true);
	CChannelSwitchForm::GetInstance();

	m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_SYSTEM, false, false);
	ResetPages();
	return true;
}

CCharMsg::eChannel CCozeForm::GetCmdFromMsg(string strMsg)
{
	string strCmd;
	strCmd.push_back(strMsg[0]);
	for (WORD i=0; i<CCharMsg::GetTotalChannelCount(); i++)
	{
		if (strCmd==CCharMsg::GetChannelCommand(i))
		{
			return CCharMsg::GetChannelByIndex(i);
		}
	}
	return CCharMsg::CHANNEL_NONE;
}

void CCozeForm::SendMsg()
{
	CCharacter* pChar = CGameScene::GetMainCha();
	if( !pChar ) return;

	string strMsg = m_edtMsg->GetCaption();
	if (strMsg.empty() || strMsg.length() == count( strMsg.begin( ), strMsg.end( ), ' '))
	{
		m_lstSystemPage->OnKeyDown(VK_END);
		m_lstMainPage->OnKeyDown(VK_END);
		return;
	}

	if(g_stUIMap.IsPKSilver() && pChar->getGMLv() <= 0)
	{
		g_pGameApp->SysInfo(g_oLangRec.GetString(901)); // �Ҷ��������н�ֹ������Ϣ
		return;
	}

	string strCurCmd="";
	CCharMsg::eChannel enumChannel=GetCmdFromMsg(strMsg);
	if (enumChannel == CCharMsg::CHANNEL_PRIVATE)
	{
		int p = (int)(strMsg.find(" ")) ;
		if (p != -1)
		{	
			string strPersonName=strMsg.substr(1, p-1) ;//˽�Ķ���
			string strContent=strMsg.substr(p+1);
			if (!strContent.empty())
			{
				//CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strContent);
				if (!strPersonName.empty() && strPersonName.length()<=37)
				{
					m_cSendMsgCard.AddCard(strContent);
					m_bSendMsgCardSwitch=false;
					if (m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_PRIVATE))
					{
						ResetPages();
					}
					strCurCmd=CCharMsg::GetChannelCommand(CCharMsg::GetChannelIndex(CCharMsg::CHANNEL_PRIVATE))+strPersonName+" ";

					CCharacter * pMain = CGameScene::GetMainCha();
					if(pMain && (pMain->IsBoat() || pMain->getGameAttr()->get(ATTR_LV) >= 9))
					{
						CS_Say2You(strPersonName.c_str(), strContent.c_str());
					}
					else
					{
						// ��ɫ�ȼ�9�����½�ֹ��������˽��
						g_pGameApp->SysInfo(g_oLangRec.GetString(868));
					}

					m_edtMsg->SetCaption(strCurCmd.c_str());
				}
			}
		}
		return;
	}
	else if (enumChannel == CCharMsg::CHANNEL_NONE)
	{
		enumChannel=m_eCurSelChannel;
	}
	else
	{
		strMsg.erase(0, 1);
		if (strMsg.empty()) return;
	}
	m_cSendMsgCard.AddCard(strMsg);
	m_bSendMsgCardSwitch=false;

	switch(enumChannel)
	{
	case CCharMsg::CHANNEL_SIGHT:
		{
			static string preStr="";
			static DWORD preTime=0;
			if (preStr==strMsg && GetTickCount()-preTime<1000)
			{
				OnSystemMsg(g_oLangRec.GetString(508));
			}
			else
			{
				if (m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_SIGHT))
				{
					ResetPages();
				}
				CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
				//Add by sunny.sun 20080902
				//Begin
				const string pcnsenderface = "#21";
				const string preplaceface = "#99";
				ReplaceSpecialFace(strMsg,preplaceface,pcnsenderface);
				//End
				CS_Say(strMsg.c_str());
				preStr=strMsg;
				preTime=GetTickCount();
			}
			break;
		}
	case CCharMsg::CHANNEL_WORLD:
		{
			CCharacter* pChar=g_stUIBoat.GetHuman();
			if (!pChar || pChar->getGameAttr()->get(ATTR_LV)<10)
			{
				OnSystemMsg(g_oLangRec.GetString(509));
			}
			else	// modify by Philip.Wu  2006-06-09  ���� 10 ������ʹ������Ƶ��
			{
				if (pChar->getGMLv() == 99 || GetTickCount()-g_pGameApp->GetLoginTime()>60000)
				{
					if (m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_WORLD))
					{
						ResetPages();
					}
					CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
					CS_Say2All(strMsg.c_str());
				}
				else
				{
					OnSystemMsg(g_oLangRec.GetString(510));
				}
			}
			break;
		}
	case CCharMsg::CHANNEL_TRADE:
		{
			CCharacter* pChar=g_stUIBoat.GetHuman();
			if (!pChar || pChar->getGameAttr()->get(ATTR_LV)<10)
			{
				OnSystemMsg(g_oLangRec.GetString(511));
			}
			else	// modify by Philip.Wu  2006-06-09  ���� 10 ������ʹ��ó��Ƶ��
			{
				if (pChar->getGMLv() == 99 ||  GetTickCount()-g_pGameApp->GetLoginTime()>60000)
				{
					if (m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_TRADE))
					{
						ResetPages();
					}
					CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
					CS_Say2Trade(strMsg.c_str());
				}
				else
				{
					OnSystemMsg(g_oLangRec.GetString(512));
				}
			}
			break;
		}
	case CCharMsg::CHANNEL_TEAM:
		{
			CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
			if (!strMsg.empty())
			{
				CTeam *pTeam=g_stUIChat.GetTeamMgr()->Find(enumTeamGroup);
				if (!pTeam || pTeam->GetCount()==0)
				{
					OnSystemMsg(g_oLangRec.GetString(513));
				}
				else
				{
					if (m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_TEAM))
					{
						ResetPages();
					}
					CS_Say2Team(strMsg.c_str());
				}
			}
			break;
		}
	case CCharMsg::CHANNEL_GUILD:
		{
			if (m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_GUILD))
			{
				ResetPages();
			}
			CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
			CS_Say2Guild(strMsg.c_str());
			break;
		}	
	case CCharMsg::CHANNEL_VIP:
		{
			if (m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_VIP))
			{
				ResetPages();
			}
			CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
			CS_Say2VIP(strMsg.c_str());
			break;
		}
	case CCharMsg::CHANNEL_DISCORD:
		{
			if (m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_DISCORD))
			{
				ResetPages();
			}
			CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
			CS_Say2Dis(strMsg.c_str());
			break;
		}
	case CCharMsg::CHANNEL_SYSTEM:
		{
			break;
		}
	case CCharMsg::CHANNEL_PUBLISH:
		{
			break;
		}
	case CCharMsg::CHANNEL_SIDE:
		{
			CS_Say2Camp(strMsg.c_str());
			break;
		}
	default:
		{
			OnSystemMsg(g_oLangRec.GetString(514));
			return;
		}
	}
	strCurCmd=CCharMsg::GetChannelCommand(CCharMsg::GetChannelIndex(enumChannel));
	m_edtMsg->SetCaption(strCurCmd.c_str());
}

void CCozeForm::UpdatePages()
{
	CList	*page = m_lstMainPage;
	
	bool bIndentFlag;
	CListItems* pListItems;
	while (m_cMainMsg.MoveToNextMsg())
	{
		
		
		CCharMsg::sTextInfo msg=m_cMainMsg.GetMsgInfo();
		
		switch(msg.eTextChannel){
			case CCharMsg::CHANNEL_DISCORD:{
				page = m_lstDiscordPage;
				break;
			}
			default:{
				page = m_lstMainPage;
				break;
			}
		}
		
		
		string strMsg=msg.strShowText;
		bIndentFlag=false;
		
		bool highlight = false;
		int nPos = (int)strMsg.find("]");
		int nameEnd = (int)strMsg.find(":");
		if(nPos>0 && msg.strWho != "" && nameEnd>0){
			CCharacter* main =  g_stUIBoat.GetHuman();
			int namePos = strMsg.find(main->getName());
			if(namePos>nameEnd){
				highlight= true;
			}
		}
					
					
		while (!strMsg.empty())
		{
			if (bIndentFlag)
			{
				strMsg="      "+strMsg;
			}
			string strShowText=CutFaceText(strMsg,44);
			
			CItemEx* pItem=new CItemEx(strShowText.c_str(), CCharMsg::GetChannelColor(msg.eTextChannel));
			pItem->SetAlignment(eAlignCenter);
			pItem->SetHeight(18);
			pItem->SetIsParseText(true);
			pItem->SetItemName(msg.strWho.c_str());
			if (!bIndentFlag){
				int nPos = (int)strShowText.find("]");
				int nameEnd = (int)strShowText.find(":");
				if(nPos>0){
					pItem->AddHighlightText(0, nPos+1,0xFFFFFF00);
					if(msg.strWho != "" && nameEnd>0){
						if((int)strShowText.find("[GM]")==nPos+1){
							pItem->AddHighlightText(nPos+1,4,0xFFa89525);
						}
						
						pItem->AddHighlightText(nPos+1, nameEnd-nPos,msg.dwColour);
					}
				}
			}
			if(highlight){
				pItem->SetHighlighted(0x80607CA0);
			}
			
			page->NewItem()->SetBegin(pItem);
			bIndentFlag=true;
		}
	};
	pListItems=page->GetItems();
	for (int i=pListItems->GetCount()-200; i>0; i--)
	{
		pListItems->Del(pListItems->GetItem(0));
	}

	static bool IsPressScroll = false;
	IsPressScroll = false;
	if( g_pGameApp->IsMouseButtonPress( 0 ) )
	{
		if( page->GetScroll()->IsNormal() )
		{
			int x = g_pGameApp->GetMouseX();
			int y = g_pGameApp->GetMouseY();
			GetRender().ScreenConvert(x, y);
			if( page->GetScroll()->InRect(x, y) ) 
			{
				IsPressScroll = true;
			}
		}
	}
	else
	{
		if( page->GetScroll()->IsNormal() )
		{
			CStep &step=page->GetScroll()->GetStep();
			if (step.GetPosition() > step.GetMin() && step.GetPosition()+5 < step.GetMax())
			{
				IsPressScroll = true;
			}
		}
	}

	if( IsPressScroll )
		page->Refresh();	
	else
		page->OnKeyDown(VK_END);
	
	while (m_cSystemMsg.MoveToNextMsg())
	{
		CCharMsg::sTextInfo msg=m_cSystemMsg.GetMsgInfo();
		string strMsg=msg.strShowText;
		bIndentFlag=false;
		while (!strMsg.empty())
		{
			if (bIndentFlag)
			{
				strMsg="      "+strMsg;
			}
			string strShowText=CutFaceText(strMsg,44);
			CItemEx* pItem=new CItemEx(strShowText.c_str(), CCharMsg::GetChannelColor(msg.eTextChannel));
			pItem->SetAlignment(eAlignCenter);
			pItem->SetHeight(18);
			pItem->SetIsParseText(true);
			pItem->SetItemName(msg.strWho.c_str());
			if (!bIndentFlag)
			{
				pItem->SetHasHeadText(8, 0xFFFFFF00);
			}
			m_lstSystemPage->NewItem()->SetBegin(pItem);
			bIndentFlag=true;
		}
	};
	pListItems=m_lstSystemPage->GetItems();
	for (int i=pListItems->GetCount()-200; i>0; i--)
	{
		pListItems->Del(pListItems->GetItem(0));
	}
	m_lstSystemPage->Refresh();

	IsPressScroll = false;
	if( g_pGameApp->IsMouseButtonPress( 0 ) )
	{
		if( m_lstSystemPage->GetScroll()->IsNormal() )
		{
			int x = g_pGameApp->GetMouseX();
			int y = g_pGameApp->GetMouseY();
			GetRender().ScreenConvert(x, y);
			if( !m_lstSystemPage->GetScroll()->InRect(x, y) ) 
			{
				IsPressScroll = true;
			}
		}
	}
	else
	{
		if( page->GetScroll()->IsNormal() )
		{
			CStep &step=page->GetScroll()->GetStep();
			if (step.GetPosition() > step.GetMin() && step.GetPosition()+5 < step.GetMax())
			{
				IsPressScroll = true;
			}
		}
	}


	if( IsPressScroll )
		m_lstSystemPage->Refresh();	
	else
		m_lstSystemPage->OnKeyDown(VK_END);

	if (GuildSysInfo)
		GuildSysInfo = !GuildSysInfo;
}

void CCozeForm::ResetPages()
{
	bool bIndentFlag;
	m_lstMainPage->GetItems()->Clear();
	m_lstDiscordPage->GetItems()->Clear();
	if (m_cMainMsg.MoveToFirstMsg())
	{
		CCharMsg::sTextInfo msg=m_cMainMsg.GetMsgInfo();
		string strMsg=msg.strShowText;
		bIndentFlag=false;
		
		CList* page = m_lstMainPage;
		switch(msg.eTextChannel){
			case CCharMsg::CHANNEL_DISCORD:{
				page = m_lstDiscordPage;
				break;
			}
			default:{
				page = m_lstMainPage;
				break;
			}
		}
		
		while (!strMsg.empty())
		{
			if (bIndentFlag)
			{
				strMsg="      "+strMsg;
			}
			string strShowText=CutFaceText(strMsg,44);
			CItemEx* pItem=new CItemEx(strShowText.c_str(), CCharMsg::GetChannelColor(msg.eTextChannel));
			pItem->SetAlignment(eAlignCenter);
			pItem->SetHeight(18);
			pItem->SetIsParseText(true);
			pItem->SetItemName(msg.strWho.c_str());
			if (!bIndentFlag)
			{
				pItem->SetHasHeadText(6, 0xFFFFFF00);
			}
			page->NewItem()->SetBegin(pItem);
			bIndentFlag=true;
		}
	}

	m_lstSystemPage->GetItems()->Clear();
	if (m_cSystemMsg.MoveToFirstMsg())
	{
		CCharMsg::sTextInfo msg=m_cSystemMsg.GetMsgInfo();
		string strMsg=msg.strShowText;
		bIndentFlag=false;
		while (!strMsg.empty())
		{
			if (bIndentFlag)
			{
				strMsg="      "+strMsg;
			}
			string strShowText=CutFaceText(strMsg,44);
			CItemEx* pItem=new CItemEx(strShowText.c_str(), CCharMsg::GetChannelColor(msg.eTextChannel));
			pItem->SetAlignment(eAlignCenter);
			pItem->SetHeight(18);
			pItem->SetIsParseText(true);
			pItem->SetItemName(msg.strWho.c_str());
			if (!bIndentFlag)
			{
				pItem->SetHasHeadText(6, 0xFFFFFF00);
			}
			m_lstSystemPage->NewItem()->SetBegin(pItem);
			bIndentFlag=true;
		}
	}
	UpdatePages();
}

void CCozeForm::ChangePrivatePlayerName(string strName)
{
	if (strName.empty())
	{
		return;
	}
	
	string strChat=m_edtMsg->GetCaption();
	string strPrivateCmd=CCharMsg::GetChannelCommand(CCharMsg::GetChannelIndex(CCharMsg::CHANNEL_PRIVATE));
	int nLTrim=0;
	if (strChat.find(CCharMsg::GetChannelCommand(CCharMsg::GetChannelIndex(CCharMsg::CHANNEL_WORLD)))==0 ||
		strChat.find(CCharMsg::GetChannelCommand(CCharMsg::GetChannelIndex(CCharMsg::CHANNEL_TRADE)))==0 ||
		strChat.find(CCharMsg::GetChannelCommand(CCharMsg::GetChannelIndex(CCharMsg::CHANNEL_TEAM)))==0  || 
		strChat.find(CCharMsg::GetChannelCommand(CCharMsg::GetChannelIndex(CCharMsg::CHANNEL_GUILD)))==0
		)
	{
		nLTrim=1;
	}
	else if (strChat.find(strPrivateCmd)==0)
	{
		int nPos=(int)(strChat.find(" "));
		if (nPos>0)
			nLTrim = nPos + 1;
		else
			nLTrim = (int) strChat.length();
	}
	if (nLTrim>0 && nLTrim<=(int)strChat.length())
	{
		strChat=strChat.substr(nLTrim,strChat.length()-nLTrim);
	}
	strChat=strPrivateCmd+strName+" "+strChat;
	if ((int)strChat.length()>m_edtMsg->GetMaxNum())
	{
		strChat=strPrivateCmd+strName+" ";
	}
	m_edtMsg->SetCaption(strChat.c_str());
}

void CCozeForm::EventPublishShowForm(CForm* pForm, bool& IsShow)
{
	CCharacter* pCharacter=CGameScene::GetMainCha();
	if(!pCharacter) return;

	if(pCharacter->getGMLv()==0)
	{
		IsShow=false;
	}
}

void CCozeForm::EventPublishSendMsg(CGuiData *pSender)
{
	CCharacter* pCharacter=CGameScene::GetMainCha();
	if(!pCharacter) return;

	//����?�ǿո�����ʾ
	CCozeForm *pThis=CCozeForm::GetInstance();
	string strMsg=pThis->m_edtPublishMsg->GetCaption() ;
	if (strMsg.empty() || strMsg.length() == count( strMsg.begin( ), strMsg.end( ), ' '))
	{
		return;
	}

	CS_GM1Say(strMsg.c_str());
	pThis->m_edtPublishMsg->SetCaption("");
}

bool CCozeForm::EventGlobalKeyDownHandle(int& key)
{
	if(g_pGameApp->IsCtrlPress())
	{
		WORD wFaceID=key-'0';
		if (wFaceID>=0 && wFaceID<=9)
		{
			CCharacter* pCharacter=CGameScene::GetMainCha();
			if (pCharacter)
			{
				char lpszBuf[20];
				sprintf(lpszBuf, "***%d", wFaceID);
				pCharacter->GetHeadSay()->SetFaceID(wFaceID);
				CS_Say(lpszBuf);
			}
		}
	}
	return false;
}

void CCozeForm::EventSendMsg(CGuiData *pSender)
{
	CCozeForm::GetInstance()->SendMsg();
}

bool CCozeForm::EventEditMsg(CGuiData *pSender, int& key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	if (key == VK_PRIOR || key == VK_NEXT)
	{
		pThis->m_lstMainPage->OnKeyDown(key);
		return true;
	}
	else if (key == VK_UP || key == VK_DOWN)
	{
		if (!pThis->m_bSendMsgCardSwitch)
		{
			if (pThis->m_cSendMsgCard.MoveToLastCard())
			{
				pThis->m_bSendMsgCardSwitch=true;
				pThis->m_edtMsg->SetCaption(pThis->m_cSendMsgCard.GetCardInfo().c_str());
				return true;
			}
		}
		else
		{
			if (key == VK_UP)
			{
				if (pThis->m_cSendMsgCard.MoveToPrevCard())
				{
					pThis->m_edtMsg->SetCaption(pThis->m_cSendMsgCard.GetCardInfo().c_str());
					return true;
				}
			}
			else
			{
				if (pThis->m_cSendMsgCard.MoveToNextCard())
				{
					pThis->m_edtMsg->SetCaption(pThis->m_cSendMsgCard.GetCardInfo().c_str());
					return true;
				}
			}
		}
	}
	return false;
}

void CCozeForm::EventDiscordListKeyDown(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	if( key & Mouse_RDown )
	{
		CItemRow* pItemRow=pThis->m_lstDiscordPage->GetSelectItem();
		if (!pItemRow) return;
		CItemEx* pItemEx=dynamic_cast<CItemEx*>(pItemRow->GetBegin());
		if (!pItemEx) return;
		string strName=pItemEx->GetItemName();
		CCharacter* pCharacter=CGameScene::GetMainCha();
		if (pCharacter)
		{
			if (strName==pCharacter->getHumanName())
			{
				return;
			}
		}
		pThis->ChangePrivatePlayerName(strName);
	}
}

void CCozeForm::EventMainListKeyDown(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	if( key & Mouse_RDown )
	{
		CItemRow* pItemRow=pThis->m_lstMainPage->GetSelectItem();
		if (!pItemRow) return;
		CItemEx* pItemEx=dynamic_cast<CItemEx*>(pItemRow->GetBegin());
		if (!pItemEx) return;
		string strName=pItemEx->GetItemName();
		CCharacter* pCharacter=CGameScene::GetMainCha();
		if (pCharacter)
		{
			if (strName==pCharacter->getHumanName())
			{
				return;
			}
		}
		pThis->ChangePrivatePlayerName(strName);
	}
}

void CCozeForm::EventSendChannelSwitchClick(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	pThis->m_lstCallingCard->SetIsShow(false);
	pThis->m_grdFacePanel->SetIsShow(false);
	pThis->m_grdBrowPanel->SetIsShow(false);
	pThis->m_grdActionPanel->SetIsShow(false);
}

void CCozeForm::EventSendChannelChange(CGuiData *pSender)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	string str=pThis->m_cmbChannel->GetText();
	if (str==CCharMsg::GetChannelName(CCharMsg::CHANNEL_SIGHT))
	{
		pThis->m_eCurSelChannel=CCharMsg::CHANNEL_SIGHT;
	}
	else if (str==CCharMsg::GetChannelName(CCharMsg::CHANNEL_TEAM))
	{
		pThis->m_eCurSelChannel=CCharMsg::CHANNEL_TEAM;
	}
	else if (str==CCharMsg::GetChannelName(CCharMsg::CHANNEL_GUILD))
	{
		pThis->m_eCurSelChannel=CCharMsg::CHANNEL_GUILD;
	}
	else if (str==CCharMsg::GetChannelName(CCharMsg::CHANNEL_TRADE))
	{
		pThis->m_eCurSelChannel=CCharMsg::CHANNEL_TRADE;
	}
	else if (str==CCharMsg::GetChannelName(CCharMsg::CHANNEL_WORLD))
	{
		pThis->m_eCurSelChannel=CCharMsg::CHANNEL_WORLD;
	}
	else if (str==CCharMsg::GetChannelName(CCharMsg::CHANNEL_SIDE))
	{
		pThis->m_eCurSelChannel=CCharMsg::CHANNEL_SIDE;
	}
	else if (str==CCharMsg::GetChannelName(CCharMsg::CHANNEL_VIP))
	{
		pThis->m_eCurSelChannel=CCharMsg::CHANNEL_VIP;
	}
	else if (str==CCharMsg::GetChannelName(CCharMsg::CHANNEL_DISCORD))
	{
		pThis->m_eCurSelChannel=CCharMsg::CHANNEL_DISCORD;
	}
	else
	{
		pThis->m_eCurSelChannel=CCharMsg::CHANNEL_NONE;
	}
	pThis->m_edtMsg->SetCaption(CCharMsg::GetChannelCommand(CCharMsg::GetChannelIndex(pThis->m_eCurSelChannel)).c_str());
}

void CCozeForm::EventMainPageDragBegin(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	pThis->m_nHeightBeforeDrag=pThis->m_lstMainPage->GetHeight();
	pThis->m_nTopBeforeDrag=pThis->m_lstMainPage->GetTop();
}

void CCozeForm::EventMainPageDragging(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	int nDragTop=pThis->m_lstMainPage->GetDrag()->GetDragY();
	int nHeight=pThis->m_nHeightBeforeDrag - nDragTop;
	if (m_nMainPageMinHeight <= nHeight && nHeight <= m_nMainPageMaxHeight)
	{
		CList *pList=pThis->m_lstMainPage;
		pList->SetPos(pList->GetLeft(), pThis->m_nTopBeforeDrag + nDragTop);
		pList->SetSize(pList->GetWidth(), nHeight);
		pList->Init();
		pList->Refresh();
		pList->GetItems()->SetFirstShowRow(pList->GetItems()->GetCount() - pList->GetItems()->GetShowCount());
		pList->Refresh();

		pThis->m_drgMainPage->SetPos(pList->GetLeft(), pList->GetTop() - 4);
		pThis->m_drgMainPage->Refresh();

		pList=pThis->m_lstDiscordPage;
		pList->SetPos(pList->GetLeft(), pThis->m_nTopBeforeDrag + nDragTop - pList->GetHeight());
		pList->Init();
		pList->Refresh();
		pList->GetItems()->SetFirstShowRow(pList->GetItems()->GetCount() - pList->GetItems()->GetShowCount());
		pList->Refresh();

		pThis->m_drgDiscordPage->SetPos(pList->GetLeft(), pList->GetTop() - 4);
		pThis->m_drgDiscordPage->Refresh();
		
		
		int h = pList->GetHeight();
		pList=pThis->m_lstSystemPage;
		pList->SetPos(pList->GetLeft(), pThis->m_nTopBeforeDrag + nDragTop - pList->GetHeight() - h);
		pList->Init();
		pList->Refresh();
		pList->GetItems()->SetFirstShowRow(pList->GetItems()->GetCount() - pList->GetItems()->GetShowCount());
		pList->Refresh();

		
		
		pThis->m_drgSystemPage->SetPos(pList->GetLeft(), pList->GetTop() - 4);
		pThis->m_drgSystemPage->Refresh();
	}
}

void CCozeForm::EventMainPageDragEnd(CGuiData *pSender, int x, int y, DWORD key)
{
	EventMainPageDragging(pSender, x, y, key);
	CCozeForm *pThis=CCozeForm::GetInstance();
	CList *pList=pThis->m_lstMainPage;
	pThis->m_drgMainPage->SetPos(pList->GetLeft(), pList->GetTop() - 4);
	pThis->m_drgMainPage->Refresh();

	pList=pThis->m_lstSystemPage;
	pThis->m_drgSystemPage->SetPos(pList->GetLeft(), pList->GetTop() - 4);
	pThis->m_drgSystemPage->Refresh();
}

void CCozeForm::EventSystemPageDragBegin(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	pThis->m_nHeightBeforeDrag=pThis->m_lstSystemPage->GetHeight();
	pThis->m_nTopBeforeDrag=pThis->m_lstSystemPage->GetTop();
}

void CCozeForm::EventSystemPageDragging(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	int nDragTop=pThis->m_lstSystemPage->GetDrag()->GetDragY();
	int nHeight=pThis->m_nHeightBeforeDrag - nDragTop;
	if (m_nSystemPageMinHeight <= nHeight && nHeight <= m_nSystemPageMaxHeight)
	{
		CList *pList=pThis->m_lstSystemPage;
		pList->SetPos(pList->GetLeft(), pThis->m_nTopBeforeDrag + nDragTop);
		pList->SetSize(pList->GetWidth(), nHeight);
		pList->Init();
		pList->Refresh();
		pList->GetItems()->SetFirstShowRow(pList->GetItems()->GetCount() - pList->GetItems()->GetShowCount());
		pList->Refresh();

		pThis->m_drgSystemPage->SetPos(pList->GetLeft(), pList->GetTop() - 4);
		pThis->m_drgSystemPage->Refresh();
		
		/*
		pList=pThis->m_lstDiscordPage;
		pList->SetPos(pList->GetLeft(), pThis->m_nTopBeforeDrag + nDragTop- pList->GetHeight());
		pList->Init();
		pList->Refresh();
		pList->GetItems()->SetFirstShowRow(pList->GetItems()->GetCount() - pList->GetItems()->GetShowCount());
		pList->Refresh();

		
		pThis->m_drgDiscordPage->SetPos(pList->GetLeft(), pList->GetTop() - 4);
		pThis->m_drgDiscordPage->Refresh();
		
		*/
	}
}

void CCozeForm::EventSystemPageDragEnd(CGuiData *pSender, int x, int y, DWORD key)
{
	EventSystemPageDragging(pSender, x, y, key);
	CCozeForm *pThis=CCozeForm::GetInstance();
	CList *pList=pThis->m_lstSystemPage;
	pThis->m_drgSystemPage->SetPos(pList->GetLeft(), pList->GetTop() - 4);
	pThis->m_drgSystemPage->Refresh();
}








void CCozeForm::EventDiscordPageDragBegin(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	pThis->m_nHeightBeforeDrag=pThis->m_lstDiscordPage->GetHeight();
	pThis->m_nTopBeforeDrag=pThis->m_lstDiscordPage->GetTop();
}

void CCozeForm::EventDiscordPageDragging(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	int nDragTop=pThis->m_lstDiscordPage->GetDrag()->GetDragY();
	int nHeight=pThis->m_nHeightBeforeDrag - nDragTop;
	if (m_nSystemPageMinHeight <= nHeight && nHeight <= m_nSystemPageMaxHeight)
	{
		CList *pList=pThis->m_lstDiscordPage;
		pList->SetPos(pList->GetLeft(), pThis->m_nTopBeforeDrag + nDragTop);
		pList->SetSize(pList->GetWidth(), nHeight);
		pList->Init();
		pList->Refresh();
		pList->GetItems()->SetFirstShowRow(pList->GetItems()->GetCount() - pList->GetItems()->GetShowCount());
		pList->Refresh();

		pThis->m_drgDiscordPage->SetPos(pList->GetLeft(), pList->GetTop() - 4);
		pThis->m_drgDiscordPage->Refresh();
		
		pList=pThis->m_lstSystemPage;
		pList->SetPos(pList->GetLeft(), pThis->m_nTopBeforeDrag + nDragTop- pList->GetHeight());
		pList->Init();
		pList->Refresh();
		pList->GetItems()->SetFirstShowRow(pList->GetItems()->GetCount() - pList->GetItems()->GetShowCount());
		pList->Refresh();

		
		pThis->m_drgSystemPage->SetPos(pList->GetLeft(), pList->GetTop() - 4);
		pThis->m_drgSystemPage->Refresh();
		
		
	}
}

void CCozeForm::EventDiscordPageDragEnd(CGuiData *pSender, int x, int y, DWORD key)
{
	EventDiscordPageDragging(pSender, x, y, key);
	CCozeForm *pThis=CCozeForm::GetInstance();
	CList *pList=pThis->m_lstDiscordPage;
	pThis->m_drgDiscordPage->SetPos(pList->GetLeft(), pList->GetTop() - 4);
	pThis->m_drgDiscordPage->Refresh();
}










void CCozeForm::EventChannelSwitchCheck(CGuiData *pSender)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	pThis->m_lstCallingCard->SetIsShow(false);
	pThis->m_grdFacePanel->SetIsShow(false);
	pThis->m_grdBrowPanel->SetIsShow(false);
	pThis->m_grdActionPanel->SetIsShow(false);
	pThis->m_cmbChannel->GetList()->SetIsShow(false);

	CChannelSwitchForm::GetInstance()->SwitchCheck();
}

void CCozeForm::EventCallingCardSwitchClick(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();

	pThis->m_lstCallingCard->GetItems()->Clear();
	if (pThis->m_cCallingCard.MoveToFirstCard())
	{
		do
		{
			pThis->m_lstCallingCard->Add(pThis->m_cCallingCard.GetCardInfo().c_str());
		} while(pThis->m_cCallingCard.MoveToNextCard());
	}

	CItemRow *pRow=pThis->m_lstCallingCard->Add(g_oLangRec.GetString(515));
	pRow->GetBegin()->SetColor(0xFF7F7F3F);
	pThis->m_lstCallingCard->SetPointer(pRow);
	pThis->m_lstCallingCard->Refresh();

	pThis->m_lstCallingCard->SetIsShow(!pThis->m_lstCallingCard->GetIsShow());
	pThis->m_grdFacePanel->SetIsShow(false);
	pThis->m_grdBrowPanel->SetIsShow(false);
	pThis->m_grdActionPanel->SetIsShow(false);
	pThis->m_cmbChannel->GetList()->SetIsShow(false);

}

//void CCozeForm::EventCallingCardLostFocus(CGuiData *pSender)
//{
//	CCozeForm *pThis=CCozeForm::GetInstance();
//	pThis->m_lstCallingCard->SetIsShow(false);
//	pThis->m_lstCallingCard->MouseRun(pThis->m_lstCallingCard->GetX()-1,pThis->m_lstCallingCard->GetY()-1,0);
//}

void CCozeForm::EventCardSelected(CGuiData *pSender)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	pThis->m_lstCallingCard->SetIsShow(false);
	CItemRow *pRow=pThis->m_lstCallingCard->GetSelectItem();
	if (!pRow)
	{
		return;
	}
	string strName=pRow->GetBegin()->GetString();
	if (strName.empty())
	{
		return;
	}
	if (pThis->m_lstCallingCard->GetPointer()==pRow)
	{
		pThis->m_cCallingCard.ClearAll();
		return;
	}

	pThis->ChangePrivatePlayerName(strName);
}

void CCozeForm::EventFacePanelSwitchClick(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	pThis->m_grdFacePanel->SetIsShow(!pThis->m_grdFacePanel->GetIsShow());
	pThis->m_lstCallingCard->SetIsShow(false);
	pThis->m_grdBrowPanel->SetIsShow(false);
	pThis->m_grdActionPanel->SetIsShow(false);
	pThis->m_cmbChannel->GetList()->SetIsShow(false);
}

//void CCozeForm::EventFacePanelLostFocus(CGuiData *pSender)
//{
//	CCozeForm *pThis=CCozeForm::GetInstance();
//	pThis->m_grdFacePanel->SetIsShow(false);
//	pThis->m_grdFacePanel->MouseRun(pThis->m_grdFacePanel->GetX()-1, pThis->m_grdFacePanel->GetY()-1, 0);
//}

void CCozeForm::EventFaceSelected(CGuiData *pSender)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	pThis->m_grdFacePanel->SetIsShow(false);

	if ((int)strlen(pThis->m_edtMsg->GetCaption())>pThis->m_edtMsg->GetMaxNum()-3)
	{
		return;
	}

	if (pThis->m_grdFacePanel->GetSelect())
	{
		pThis->m_edtMsg->SetActive(pThis->m_edtMsg);
		char lpszFace[10];
		sprintf(lpszFace, "#%02d", pThis->m_grdFacePanel->GetSelectIndex());
		pThis->m_edtMsg->ReplaceSel(lpszFace);
	}
}

void CCozeForm::EventBrowPanelSwitchClick(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	pThis->m_grdBrowPanel->SetIsShow(!pThis->m_grdBrowPanel->GetIsShow());
	pThis->m_lstCallingCard->SetIsShow(false);
	pThis->m_grdFacePanel->SetIsShow(false);
	pThis->m_grdActionPanel->SetIsShow(false);
	pThis->m_cmbChannel->GetList()->SetIsShow(false);
}

void CCozeForm::EventBrowSelected(CGuiData *pSender)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	CCharacter* pCharacter=CGameScene::GetMainCha();
	if(!pCharacter) return;
	pThis->m_grdBrowPanel->SetIsShow(false);

	if (pThis->m_grdBrowPanel->GetSelect())
	{
		int nIndex = pThis->m_grdBrowPanel->GetSelectIndex();
		pCharacter->GetHeadSay()->SetFaceID(nIndex);
		char lpszFaceID[10] = {0} ;
		sprintf(lpszFaceID , "***%d" ,nIndex);	
		CS_Say(lpszFaceID);		
	}
}

void CCozeForm::EventActionPanelSwitchClick(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	pThis->m_grdActionPanel->SetIsShow(!pThis->m_grdActionPanel->GetIsShow());
	pThis->m_lstCallingCard->SetIsShow(false);
	pThis->m_grdFacePanel->SetIsShow(false);
	pThis->m_grdBrowPanel->SetIsShow(false);
	pThis->m_cmbChannel->GetList()->SetIsShow(false);
}

void CCozeForm::EventActionSelected(CGuiData *pSender)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	CCharacter* pCharacter=CGameScene::GetMainCha();
	if(!pCharacter) return;
	pThis->m_grdActionPanel->SetIsShow(false);

	CGraph * graph=pThis->m_grdActionPanel->GetSelect();
	//int nIndex = pThis->m_grdActionPanel->GetSelectIndex();
	if(graph)
	{		
		//pCharacter->GetActor()->PlayPose(graph->nTag, true, true);
		pCharacter->GetActor()->PlayPose(graph->nTag, true, true);
	}
}












#if 0

#define CARD_LIST_MAX 15


//static string  strPersonObject;     // ˽�ĵĶ���
CList* CCozeMgr::lstOnSay=NULL;
CList* CCozeMgr::lstOnSystemSay=NULL;
CList* CCozeMgr::lstOnDiscordSay=NULL;
CCombo*	CCozeMgr::m_cmbChannel=NULL;
CGrid*	CCozeMgr::grdFace=NULL;
CEdit*	CCozeMgr::edtSay=NULL;
CEdit*	CCozeMgr::edtGmSay=NULL;
CCheckBox* CCozeMgr::m_pForbidCheck=NULL;
CTextButton* CCozeMgr::m_pbtnCard=NULL;
CList* CCozeMgr::m_pCardList=NULL;
vector<string> CCozeMgr::m_strCardListData;

CCheckBox* CCozeMgr::m_pPrivateCheck=NULL;
CCheckBox* CCozeMgr::m_pSightCheck=NULL;
CCheckBox* CCozeMgr::m_pSystemCheck=NULL;
CCheckBox* CCozeMgr::m_pTeamCheck=NULL;
CCheckBox* CCozeMgr::m_pGuildCheck=NULL;
CCheckBox* CCozeMgr::m_pWorldCheck=NULL;
CCheckBox* CCozeMgr::m_pTradeCheck=NULL;


CCozeMgr::eChannel CCozeMgr::m_CurSelectChannel=CCozeMgr::CHANNEL_ROAD;
string CCozeMgr::m_CurCommand="";
int CCozeMgr::m_TextPoint[CHANNELS_COUNT][2]={0};
CCozeMgr::sTextInfo CCozeMgr::m_TextRecords[CHANNELS_COUNT][DATA_RECORDS_SIZE]={CHANNEL_ROAD,"","",0};

//---------------------------------------------------------------------------
// class CCozeMgr
//---------------------------------------------------------------------------
bool CCozeMgr::Init()
{
	CCozeForm::GetInstance();
	frmMainChat = _FindForm("frmMain800");
	grdFace = dynamic_cast<CGrid*>(frmMainChat->Find("grdFace"));
	if( !grdFace ) return Error( g_oLangRec.GetString(45), frmMainChat->GetName(), "grdFace" );
	return true;					

	//Arcol:Test-

	_nSelectSayRow = 0;
	_nOldSayRow = 0;
	for (int i=0;i<MAX_PAGE_SAY;i++)
	{
		g_stUICoze.oldSay[i]="";
	}

	//
	CForm* frmMain800 = _FindForm("frmMain800");

	CFormMgr &mgr = CFormMgr::s_Mgr;
	frmMainChat = frmMain800;//_FindForm("frmMainChat");
	if( !frmMainChat ) return false;
	
	frmMainChat->SetIsShow(true);
	//frmMainChat->SetPos( frmMainChat->GetLeft(), frmMain800->GetTop() );
	//frmMainChat->SetStyle(CForm::fsBottom);

	edtSay = dynamic_cast<CEdit*>(frmMainChat->Find("edtSay"));
	if( !edtSay ) return Error(g_oLangRec.GetString(516), frmMainChat->GetName(), "edtSay");
	edtSay->SetIsWrap(true);
	edtSay->evtEnter = _evtSay;
	edtSay->evtKeyDown = _evtSayKeyDown;

	lstOnSystemSay = dynamic_cast<CList*>(frmMainChat->Find("lstOnSaySystem"));
	if( !lstOnSystemSay )	return Error( g_oLangRec.GetString(516), frmMainChat->GetName(), "lstOnSaySystem" );
	lstOnSystemSay->evtListMouseDB = _MainOnLstKeyDownEvent;
	lstOnSystemSay->SetIsChangeColor(false);
	lstOnSystemSay->SetMouseAction( enumMA_Drill );
	
	lstOnDiscordSay = dynamic_cast<CList*>(frmMainChat->Find("lstOnSayDiscord"));
	if( !lstOnDiscordSay )	return Error( g_oLangRec.GetString(516), frmMainChat->GetName(), "lstOnSayDiscord" );
	lstOnDiscordSay->evtListMouseDB = _MainOnLstKeyDownEvent;
	lstOnDiscordSay->SetIsChangeColor(false);
	lstOnDiscordSay->SetMouseAction( enumMA_Drill );

	lstOnSay = dynamic_cast<CList*>(frmMainChat->Find("lstOnSay"));
	if( !lstOnSay )	return Error( g_oLangRec.GetString(516), frmMainChat->GetName(), "lstOnSay" );
	lstOnSay->evtListMouseDB = _MainOnLstKeyDownEvent;
	lstOnSay->SetIsChangeColor(false);
	lstOnSay->SetMouseAction( enumMA_Drill );
	
	lstChannel = dynamic_cast<CList*>(frmMainChat->Find("lstChannel"));
	if( !lstChannel ) return Error( g_oLangRec.GetString(516), frmMainChat->GetName(), "lstChannel" );
	lstChannel->evtSelectChange = _evtChannelSelectChange;

	mgr.AddKeyDownEvent( _MainOnSayKeyDownEvent );

	drpTitleDiscord = dynamic_cast<CDragTitle*>(frmMainChat->Find("drpTitleDiscord"));
	if( !drpTitleDiscord ) return Error( g_oLangRec.GetString(516), frmMainChat->GetName(), "drpTitleDiscord" );
	drpTitleDiscord->SetIsShowDrag(false);
	
	drpTitleSystem = dynamic_cast<CDragTitle*>(frmMainChat->Find("drpTitleSystem"));
	if( !drpTitleSystem ) return Error( g_oLangRec.GetString(516), frmMainChat->GetName(), "drpTitleSystem" );
	drpTitleSystem->SetIsShowDrag(false);
	
	drpTitle = dynamic_cast<CDragTitle*>(frmMainChat->Find("drpTitle"));
	if( !drpTitle ) return Error( g_oLangRec.GetString(516), frmMainChat->GetName(), "drpTitle" );
	drpTitle->SetIsShowDrag(false);

	//CTextButton *btnChat = dynamic_cast<CTextButton*>(frmMainChat->Find("btnChat"));
	//if( !btnChat ) return Error( "coze.clu����<%s>���Ҳ����ؼ�<%s>", frmMainChat->GetName(), "btnChat" );
	//btnChat->evtMouseClick=_evtShowChannelList;

	//CTextButton *btnChat = dynamic_cast<CTextButton*>(frmMainChat->Find("btnChat"));
	//if( !btnChat ) return Error( "coze.clu����<%s>���Ҳ����ؼ�<%s>", frmMainChat->GetName(), "btnChat" );
	//btnChat->evtMouseClick=_evtShowChannelList;

	m_cmbChannel=dynamic_cast<CCombo*>(frmMainChat->Find("cboChannel"));
	if( !m_cmbChannel ) return Error( g_oLangRec.GetString(516), frmMainChat->GetName(), "cboChannel" );
	m_cmbChannel->evtSelectChange=_evtChannelSelectChange;
	m_cmbChannel->GetList()->GetItems()->Select(0);


	m_pForbidCheck=dynamic_cast<CCheckBox*>(frmMainChat->Find("chkChatOn"));
	if( !m_pForbidCheck ) return Error( g_oLangRec.GetString(516), frmMainChat->GetName(), "chkChatOn" );
	m_pForbidCheck->SetIsChecked(false);
	m_pForbidCheck->evtCheckChange=_evtForbidCheck;

	FORM_CONTROL_LOADING_CHECK(m_pbtnCard,frmMainChat,CTextButton,"coze.clu","btnCard");
	m_pbtnCard->evtMouseClick=_evtCardClick;

	FORM_CONTROL_LOADING_CHECK(m_pCardList,frmMainChat,CList,"coze.clu","lstCard");
	m_pCardList->evtSelectChange=_evtCardSelectChange;
	m_pCardList->SetIsChangeColor(false);


	FORM_LOADING_CHECK(frmGM,"coze.clu","frmGM");
	frmGM->evtBeforeShow=_evtGmForm;

	//Add by sunny.sun20080804
	FORM_LOADING_CHECK(Arena,"coze.clu","Arena");
	Arena->evtBeforeShow=_evtGmForm;
	//End

	FORM_LOADING_CHECK(frmForbid,"main.clu","frmForbid");
	frmForbid->evtLost=_evtForbidFormLost;
	FORM_CONTROL_LOADING_CHECK(m_pPrivateCheck,frmForbid,CCheckBox,"main.clu","chkPersonal");
	FORM_CONTROL_LOADING_CHECK(m_pSightCheck,frmForbid,CCheckBox,"main.clu","chkNear");
	FORM_CONTROL_LOADING_CHECK(m_pSystemCheck,frmForbid,CCheckBox,"main.clu","chkSystem");
	FORM_CONTROL_LOADING_CHECK(m_pTeamCheck,frmForbid,CCheckBox,"main.clu","chkTeam");
	FORM_CONTROL_LOADING_CHECK(m_pGuildCheck,frmForbid,CCheckBox,"main.clu","chkGuild");
	FORM_CONTROL_LOADING_CHECK(m_pWorldCheck,frmForbid,CCheckBox,"main.clu","chkWorld");
	FORM_CONTROL_LOADING_CHECK(m_pTradeCheck,frmForbid,CCheckBox,"main.clu","chkTrade");
	m_pSystemCheck->evtCheckChange=_evtForbidSystemChange;


	FORM_CONTROL_LOADING_CHECK(edtGmSay,frmGM,CEdit,"coze.clu","edtGMSay");
	edtGmSay->SetIsWrap(true);
	edtGmSay->evtEnter = _evtGmSay;
	//Add by sunny.sun20080804
	FORM_CONTROL_LOADING_CHECK(edtArenaSay,Arena,CEdit,"coze.clu","edtArenaSay");
	edtArenaSay->SetIsWrap(true);
	edtArenaSay->evtEnter = _evtGmSay;	
	//End
	
	CDrag *pDrag = drpTitleSystem->GetDrag();
	pDrag->evtMouseDragBegin = _DragBeginOnSystemTitle;
	pDrag->evtMouseDragMove = _DragMoveOnSystemTitle;
	pDrag->evtMouseDragEnd = _DragEndOnSystemTitle;
	pDrag->SetYare(0);
	pDrag->SetDragInCursor( CCursor::stVertical );
	//_nDragMinY = 70;
	//_nDragMaxY = 280;
	
	pDrag = drpTitleDiscord->GetDrag();
	pDrag->evtMouseDragBegin = _DragBeginOnDiscordTitle;
	pDrag->evtMouseDragMove = _DragMoveOnDiscordTitle;
	pDrag->evtMouseDragEnd = _DragEndOnDiscordTitle;
	pDrag->SetYare(0);
	pDrag->SetDragInCursor( CCursor::stVertical );

	pDrag = drpTitle->GetDrag();
	pDrag->evtMouseDragBegin = _DragBeginOnSay;
	pDrag->evtMouseDragMove = _DragMoveOnSay;
	pDrag->evtMouseDragEnd = _DragEndOnSay;
	pDrag->SetYare(0);
	pDrag->SetDragInCursor( CCursor::stVertical );
	_nDragSystemMinY=55;
	_nDragSystemMaxY=280;
	_nDragMinY = 70;
	_nDragMaxY = 280;
	
	_nDragDiscordMinY = 55;
	_nDragDiscordMaxY = 280;
	
	grdFace = dynamic_cast<CGrid*>(frmMainChat->Find("grdFace"));
	if( !grdFace ) return Error( g_oLangRec.GetString(45), frmMainChat->GetName(), "grdFace" );
	grdFace->evtSelectChange =_evtSelectBudChange;   
	grdFace->evtLost = _evtHideSelf;
	
	g_stUICoze.lstOnSay->SetRowHeight(16);
	g_stUICoze.lstOnSystemSay->SetRowHeight(16);
	g_stUICoze.lstOnDiscordSay->SetRowHeight(16);


	return true;
}

void CCozeMgr::End()
{
}

bool CCozeMgr::IsMouseOnSayList( int x, int y )
{
	if( CCompent::GetLastMouseCompent()==lstOnSay )
	{		
		if( lstOnSay->GetScroll()->IsNormal() )
		{
			GetRender().ScreenConvert( x, y );
			if( lstOnSay->GetScroll()->InRect( x, y ) )
				return false;
		}

		return true;
	}
	return false;
}

void CCozeMgr::_evtSay(CGuiData *pSender)
{
	CCharacter* pCha = CGameScene::GetMainCha();
	if( !pCha ) return;

	//����?�ǿո�����ʾ	
	const char* s = g_stUICoze.edtSay->GetCaption() ;
	int len =(int) strlen( s );
	int i = 0;
	for ( ; i <len  ;i++)
	{
		if 	(s[i]!= 32 )  //32�ǿո��ascii��
			break;
	}	
	if (i == len )
		return ;

	string strChat =  g_stUICoze.edtSay->GetCaption();
	g_stUICoze.oldSay[g_stUICoze._nOldSayRow++] = g_stUICoze.edtSay->GetCaption();
	g_stUICoze._nOldSayRow%=MAX_PAGE_SAY;
	g_stUICoze._nSelectSayRow = g_stUICoze._nOldSayRow;
	int p = (int)(strChat.find(" ") ) ;
	if ( strChat.find ("@") ==0 && p != -1)   //˽��
	{	
		string strPersonName=strChat.substr(1, p-1) ;//˽�Ķ���
		string strPersonContent	 = strChat.substr( p+1, strChat.length() - p -1 );
		if (strPersonContent.length()>0)
		{
			//FilterText(strPersonContent,&g_stUIChat._strFilterTxt);
			//CTextFilter::Filter(CTextFilter::DIALOG_TABLE,strPersonContent);
			if (!strPersonName.empty() && strPersonName.length()<=37)
			{
				m_CurCommand="@"+strPersonName+" ";
				CS_Say2You( strPersonName.c_str() ,strPersonContent.c_str() );
			}
		}
		///////////����lstOnSay����ʾ
		//string str = pCha->getName();
		//str += ":";
		//str += strPersonContent ;
		//ShowLstOnSay( str, g_stUICoze.chatColor.person );
		//g_stUICoze.lstOnSay->OnKeyDown(VK_END);

		//g_stUICoze.oldSay[g_stUICoze._nOldSayRow] = g_stUICoze.edtSay->GetCaption();
		//++g_stUICoze._nOldSayRow;
		//if( g_stUICoze._nOldSayRow >= MAX_PAGE_SAY )
		//	g_stUICoze._nOldSayRow=0;

		//g_stUICoze._nSelectSayRow = g_stUICoze._nOldSayRow;
		//g_stUICoze.edtSay->SetCaption("");
		//strPersonObject = "";

		///////////����lstOnSay����ʾ
		//str = pCha->getName();
		//str += ":";
		//str += g_stUICoze.edtSay->GetCaption() ;
		//ShowLstOnSay( str, g_stUICoze.chatColor.road );
		//g_stUICoze.lstOnSay->OnKeyDown(VK_END);

		////////////////////////////////////////////////////

		//g_stUICoze.oldSay[g_stUICoze._nOldSayRow] = g_stUICoze.edtSay->GetCaption();
		//++g_stUICoze._nOldSayRow;
		//if( g_stUICoze._nOldSayRow >= MAX_PAGE_SAY )
		//	g_stUICoze._nOldSayRow=0;

		//g_stUICoze._nSelectSayRow = g_stUICoze._nOldSayRow;


		//string  strheadSay;
		//string  strName ;
		//strName = pCha->getName() ;   //��Ϸ������
		//strName = strName + ":" ;
		//strheadSay =strName  +  g_stUICoze.edtSay->GetCaption();
		//CItemEx * item_headSay = new CItemEx(strheadSay.c_str() , g_stUICoze.chatColor.road );

		//if ( strheadSay.size() > 32 )  //�������?16���ַ�����������?
		//{
		//	item_headSay->SetIsMultiLine(true );
		//	item_headSay->ProcessString( (int) strName.size());
		//}
		//if (  strheadSay.find ("#")!= -1 )
		//	item_headSay->SetIsParseText(true);

		//item_headSay->SetColor( g_stUICoze.chatColor.road );
		//pCha->GetHeadSay()->AddItem( item_headSay );	
		//g_stUICoze.edtSay->SetCaption("");

		//return;
	}	
	else
	{
		//FilterText(strChat,&g_stUIChat._strFilterTxt);
		if (strChat.find("*")==0)
		{
			if (GetTickCount()-g_pGameApp->GetLoginTime()>60000)
			{
				CTextFilter::Filter(CTextFilter::DIALOG_TABLE,strChat);
				if (strChat.length()>1) CS_Say2All( strChat.substr(1).c_str() );
			}
			else
			{
				OnSystemSay(g_oLangRec.GetString(510));
			}
		}
		else if (strChat.find("^")==0)
		{
			if (GetTickCount()-g_pGameApp->GetLoginTime()>60000)
			{
				CTextFilter::Filter(CTextFilter::DIALOG_TABLE,strChat);
				if (strChat.length()>1) CS_Say2Trade( strChat.substr(1).c_str() );
			}
			else
			{
				OnSystemSay(g_oLangRec.GetString(512));
			}
		}
		else if (strChat.find("%")==0)
		{
			CTextFilter::Filter(CTextFilter::DIALOG_TABLE,strChat);
			if (strChat.length()>1) CS_Say2Guild( strChat.substr(1).c_str() );
			//g_pGameApp->SysInfo("��Ƶ����δ����,������ʹ������ϵͳ�еĹ�����������Ϣ");
		}
		else if (strChat.find("!")==0)
		{
			CTextFilter::Filter(CTextFilter::DIALOG_TABLE,strChat);
			if (strChat.length()>1)
			{
				CTeam *pTeam=g_stUIChat.GetTeamMgr()->Find(enumTeamGroup);
				if (!pTeam || pTeam->GetCount()==0)
				{
					PrintToList(CHANNEL_TEAM,"",g_oLangRec.GetString(517),g_stUICoze.chatColor.team);
				}
				else
					CS_Say2Team( strChat.substr(1).c_str() );
			}
		}
		else
		{
			// 2005.5.27 ��������������Ƶ������·��Ƶ�������������ظ��ķ��ԣ�����Ϊ8��
			// 2005.6.2 �޸�����������Ƶ������·��Ƶ�������������ظ��ķ��ԣ�����Ϊ1��
			static string preStr="";
			static DWORD preTime=0;
			if (preStr==strChat && GetTickCount()-preTime<1000)
			{
				OnSystemSay(g_oLangRec.GetString(518));
			}
			else
			{
				CTextFilter::Filter(CTextFilter::DIALOG_TABLE,strChat);
				CS_Say( strChat.c_str() );
				preStr=strChat;
				preTime=GetTickCount();
			}
		}
		string str=m_cmbChannel->GetText();
		if (str==g_oLangRec.GetString(496))
		{
			m_CurCommand="*";
		}
		else if (str==g_oLangRec.GetString(497))
		{
			m_CurCommand="^";
		}
		else if (str==g_oLangRec.GetString(468))
		{
			m_CurCommand="%";
		}
		else if (str==g_oLangRec.GetString(519))
		{
			m_CurCommand="!";
		}
		else if (str==g_oLangRec.GetString(493))
		{
			m_CurCommand="";
		}

		//string str=m_cmbChannel->GetText();
		//if (str=="����")
		//{
		//	CS_Say2All( g_stUICoze.edtSay->GetCaption() );
		//}
		//else if (str=="����")
		//{
		//	CS_Say2All( g_stUICoze.edtSay->GetCaption() );
		//}
		//else if (str=="����")
		//{
		//	CS_Say( g_stUICoze.edtSay->GetCaption() );
		//}
		//else if (str=="����")
		//{
		//	//CS_Say( g_stUICoze.edtSay->GetCaption() );
		//}
		//else if (str=="����")
		//{
		//	CTeam *pTeam=g_stUIChat.GetTeamMgr()->Find(enumTeamGroup);
		//	if (!pTeam || pTeam->GetCount()==0)
		//	{
		//		PrintToList(CHANNEL_TEAM,"","����ʧ��:��Ŀǰ��û�����?",g_stUICoze.chatColor.team);
		//		return;
		//	}
		//	CS_Say2Team( g_stUICoze.edtSay->GetCaption() );
		//}
		//else
		//{
		//	CS_Say( g_stUICoze.edtSay->GetCaption() );
		//}
	}
	g_stUICoze.edtSay->SetCaption(m_CurCommand.c_str());
}

bool CCozeMgr::_evtSayKeyDown(CGuiData *pSender, int& key)
{
	m_pCardList->SetIsShow( false );
	grdFace->SetIsShow( false );
	switch( key )
	{
	case VK_DOWN:
		{
			++g_stUICoze._nSelectSayRow;
			if( g_stUICoze._nSelectSayRow>=MAX_PAGE_SAY )
				g_stUICoze._nSelectSayRow = 0;
		}
		break;
	case VK_UP:
		{
			--g_stUICoze._nSelectSayRow;
			if( g_stUICoze._nSelectSayRow < 0 )
				g_stUICoze._nSelectSayRow = MAX_PAGE_SAY - 1;
		}
		break;
	default: return false;
	}
	g_stUICoze.edtSay->SetCaption( g_stUICoze.oldSay[g_stUICoze._nSelectSayRow].c_str() );
	return true;
}

void CCozeMgr::_evtChannelSelectChange(CGuiData *pSender)
{
	string str=m_cmbChannel->GetText();
	if (str==g_oLangRec.GetString(493))
	{
		m_CurSelectChannel=CHANNEL_ROAD;
		m_CurCommand="";
	}
	else if (str==g_oLangRec.GetString(519))
	{
		m_CurSelectChannel=CHANNEL_TEAM;
		m_CurCommand="!";
	}
	else if (str==g_oLangRec.GetString(468))
	{
		m_CurSelectChannel=CHANNEL_GUILD;
		m_CurCommand="%";
	}
	else if (str==g_oLangRec.GetString(497))
	{
		m_CurSelectChannel=CHANNEL_TRADE;
		m_CurCommand="^";
	}
	else if (str==g_oLangRec.GetString(496))
	{
		m_CurSelectChannel=CHANNEL_WORLD;
		m_CurCommand="*";
	}
	else if (str == g_oLangRec.GetString(932))
	{
		m_CurSelectChannel=CHANNEL_SIDE;
		m_CurCommand="|";
	}else if (str == "Discord")
	{
		m_CurSelectChannel=CHANNEL_DISCORD;
		m_CurCommand="`";
	}else if (str == "VIP")
	{
		m_CurSelectChannel=CHANNEL_VIP;
		m_CurCommand="$";
	}

	g_stUICoze.edtSay->SetCaption(m_CurCommand.c_str());
	g_stUICoze.lstOnSystemSay->GetItems()->Clear();
	g_stUICoze.lstOnSay->GetItems()->Clear();
	for (int i=m_TextPoint[m_CurSelectChannel][0];i!=m_TextPoint[m_CurSelectChannel][1];i=(i+1)%DATA_RECORDS_SIZE)
	{
		if ((m_CurSelectChannel==CHANNEL_ROAD && IsForbidChannel(m_TextRecords[m_CurSelectChannel][i].eTextChannel)) ||
			(m_TextRecords[m_CurSelectChannel][i].eTextChannel==CHANNEL_PRIVATE && IsForbidChannel(CHANNEL_PRIVATE)))
		{
			continue;
		}
		string text=m_TextRecords[m_CurSelectChannel][i].stText;
		bool indentFlag=false;
		while (!text.empty())
		{
			if (indentFlag)
			{
				text="      "+text;
			}
			string str=CutFaceText(text,44);
			CItemRow* pItemRow;
			if (m_TextRecords[m_CurSelectChannel][i].eTextChannel==CHANNEL_SYSTEM)
			{
				pItemRow=g_stUICoze.lstOnSystemSay->NewItem();
			}else if (m_TextRecords[m_CurSelectChannel][i].eTextChannel==CHANNEL_DISCORD)
			{
				pItemRow=g_stUICoze.lstOnDiscordSay->NewItem();
			}
			else
			{
				pItemRow=g_stUICoze.lstOnSay->NewItem();
			}
			CItemEx* pItem=new CItemEx(str.c_str(),m_TextRecords[m_CurSelectChannel][i].dwColor);
			pItem->SetAlignment(eAlignCenter);
			pItem->SetHeight(18);
			pItem->SetItemName(m_TextRecords[m_CurSelectChannel][i].stName.c_str());
			pItem->SetIsParseText(true);
			if (!indentFlag)
			{
				pItem->SetHasHeadText(6,0xFFFFFF00);
			}
			pItemRow->SetBegin(pItem);
			indentFlag=true;
		}
	}
	if( CCompent::GetActive()!=lstOnSay->GetScroll() ) lstOnSay->OnKeyDown(VK_END);
	if( CCompent::GetActive()!=lstOnSystemSay->GetScroll() ) lstOnSystemSay->OnKeyDown(VK_END);
	if( CCompent::GetActive()!=lstOnDiscordSay->GetScroll() ) lstOnDiscordSay->OnKeyDown(VK_END);
	g_stUICoze.lstOnSystemSay->Refresh();
	g_stUICoze.lstOnSay->Refresh();
	g_stUICoze.lstOnDiscordSay->Refresh();

	//CList* p = dynamic_cast<CList*>(pSender);
	//if( !p ) return;

	//CItemRow* selectItem = p->GetSelectItem();

	//int nSelect = p->GetItems()->GetIndex(selectItem);

	//switch(nSelect)
	//{
	//case 0:
	//	g_stUICoze.edtSay->SetCaption("");
	//	break;
	//case 1:
	//	g_stUICoze.edtSay->SetCaption("@");
	//	break;
	//case 2:
	//	g_stUICoze.edtSay->SetCaption("!");
	//	break;
	//case 3:
	//	g_stUICoze.edtSay->SetCaption("^");
	//	break;
	//case 4:
	//	g_stUICoze.edtSay->SetCaption("$");
	//	break;
	//}
	//p->SetIsShow(false);
}

bool CCozeMgr::_MainOnSayKeyDownEvent( int& key )
{
	if( g_pGameApp->IsCtrlPress() )
	{
		CCharacter* pCha = CGameScene::GetMainCha();
		if( !pCha ) return false;

		pCha->GetHeadSay()->SetFaceID( key-'0');

		unsigned int id = key-'0' ;	

		if ( id<0 || id > 9)
			return false;
		
		char szFaceID[10] = {0} ;
		sprintf( szFaceID , "***%d" ,id);	
		CS_Say( szFaceID);
		return false;
	}

	if( key==VK_UP || key==VK_DOWN ) return false;
	if( key==VK_LEFT || key==VK_RIGHT ) return false;

	g_stUICoze.lstOnSay->OnKeyDown( key );
	return false;
}


void CCozeMgr::_DragBeginOnSystemTitle(CGuiData *pSender, int x, int y, DWORD key)
{
	g_stUICoze._nBeginOnSayHeight = g_stUICoze.lstOnSystemSay->GetHeight();
	g_stUICoze._nBeginOnSayY = g_stUICoze.lstOnSystemSay->GetTop();
}

void CCozeMgr::_DragMoveOnSystemTitle(CGuiData *pSender, int x, int y, DWORD key)
{
	int height = g_stUICoze._nBeginOnSayHeight - g_stUICoze.lstOnSystemSay->GetDrag()->GetDragY();
	if( height >= g_stUICoze._nDragSystemMinY && height <= g_stUICoze._nDragSystemMaxY )
	{
		CList *lst = g_stUICoze.lstOnSystemSay;
		lst->SetPos( lst->GetLeft(), g_stUICoze._nBeginOnSayY + g_stUICoze.lstOnSystemSay->GetDrag()->GetDragY() );		
		lst->SetSize( lst->GetWidth(), height );
		lst->Refresh();

		CDragTitle* title = g_stUICoze.drpTitleSystem ;
		title->SetPos( lst->GetLeft(), lst->GetTop() -4 );
		title->Refresh() ;

		CListItems *items = g_stUICoze.lstOnSystemSay->GetItems();
		items->SetFirstShowRow( items->GetCount() - items->GetShowCount() );
	}

	g_stUICoze.lstOnSystemSay->Init();
	g_stUICoze.lstOnSystemSay->Refresh();
}

void CCozeMgr::_DragEndOnSystemTitle(CGuiData *pSender, int x, int y, DWORD key)
{
	_DragMoveOnSystemTitle( pSender,  g_stUICoze.drpTitleSystem->GetLeft(), y, key );
	g_stUICoze.drpTitleSystem->SetPos( g_stUICoze.lstOnSystemSay->GetLeft(), g_stUICoze.lstOnSystemSay->GetTop() -4 );
	g_stUICoze.drpTitleSystem->Refresh();
}

void CCozeMgr::_DragBeginOnSay(CGuiData *pSender, int x, int y, DWORD key)
{
	g_stUICoze._nBeginOnSayHeight = g_stUICoze.lstOnSay->GetHeight();
	g_stUICoze._nBeginOnSayY = g_stUICoze.lstOnSay->GetTop();
//	g_stUICoze._DragMoveOnSay( pSender, x, y, key );
}

void CCozeMgr::_DragEndOnSay(CGuiData *pSender, int x, int y, DWORD key)
{
	_DragMoveOnSay( pSender,  g_stUICoze.drpTitle->GetLeft(), y, key );
	g_stUICoze.drpTitle->SetPos( g_stUICoze.lstOnSay->GetLeft(), g_stUICoze.lstOnSay->GetTop() -4 );
	g_stUICoze.drpTitle->Refresh();
	//g_stUICoze.lstOnSay->Init();
	//g_stUICoze.lstOnSay->Refresh();
}

void CCozeMgr::_DragMoveOnSay(CGuiData *pSender, int x, int y, DWORD key)
{
	int height = g_stUICoze._nBeginOnSayHeight - g_stUICoze.drpTitle->GetDrag()->GetDragY();
	if( height >= g_stUICoze._nDragMinY && height <= g_stUICoze._nDragMaxY )
	{
		CList *lst = g_stUICoze.lstOnSystemSay;
		lst->SetPos( lst->GetLeft(), g_stUICoze._nBeginOnSayY + g_stUICoze.drpTitle->GetDrag()->GetDragY() - lst->GetHeight() );		
		lst->Refresh();

		CDragTitle* title = g_stUICoze.drpTitleSystem ;
		title->SetPos( lst->GetLeft(), lst->GetTop() -4 );
		title->Refresh() ;

		lst = g_stUICoze.lstOnSay;
		lst->SetPos( lst->GetLeft(), g_stUICoze._nBeginOnSayY + g_stUICoze.drpTitle->GetDrag()->GetDragY() );		
		lst->SetSize( lst->GetWidth(), height );
		lst->Refresh();

		title = g_stUICoze.drpTitle ;
		title->SetPos( lst->GetLeft(), lst->GetTop() -4 );
		title->Refresh() ;

		CListItems *items = g_stUICoze.lstOnSay->GetItems();
		items->SetFirstShowRow( items->GetCount() - items->GetShowCount() );
	}
	g_stUICoze.lstOnSystemSay->Init();
	g_stUICoze.lstOnSystemSay->Refresh();

	g_stUICoze.lstOnSay->Init();
	g_stUICoze.lstOnSay->Refresh();
}






void CCozeMgr::_DragBeginOnDiscordTitle(CGuiData *pSender, int x, int y, DWORD key)
{
	g_stUICoze._nBeginOnSayHeight = g_stUICoze.lstOnDiscordSay->GetHeight();
	g_stUICoze._nBeginOnSayY = g_stUICoze.lstOnDiscordSay->GetTop();
}

void CCozeMgr::_DragEndOnDiscordTitle(CGuiData *pSender, int x, int y, DWORD key)
{
	_DragMoveOnSay( pSender,  g_stUICoze.drpTitleDiscord->GetLeft(), y, key );
	g_stUICoze.drpTitleDiscord->SetPos( g_stUICoze.lstOnDiscordSay->GetLeft(), g_stUICoze.lstOnDiscordSay->GetTop() -4 );
	g_stUICoze.drpTitleDiscord->Refresh();
}

void CCozeMgr::_DragMoveOnDiscordTitle(CGuiData *pSender, int x, int y, DWORD key)
{
	int height = g_stUICoze._nBeginOnSayHeight - g_stUICoze.drpTitleDiscord->GetDrag()->GetDragY();
	if( height >= g_stUICoze._nDragDiscordMinY && height <= g_stUICoze._nDragDiscordMaxY )
	{
		CList *lst = g_stUICoze.lstOnDiscordSay;
		lst->SetPos( lst->GetLeft(), g_stUICoze._nBeginOnSayY + g_stUICoze.drpTitleDiscord->GetDrag()->GetDragY() - lst->GetHeight() );		
		lst->Refresh();

		CDragTitle* title = g_stUICoze.drpTitleDiscord;
		title->SetPos( lst->GetLeft(), lst->GetTop() -4 );
		title->Refresh() ;

		lst = g_stUICoze.lstOnSayDiscord;
		lst->SetPos( lst->GetLeft(), g_stUICoze._nBeginOnSayY + g_stUICoze.drpTitleDiscord->GetDrag()->GetDragY() );		
		lst->SetSize( lst->GetWidth(), height );
		lst->Refresh();

		title = g_stUICoze.drpTitleDiscord ;
		title->SetPos( lst->GetLeft(), lst->GetTop() -4 );
		title->Refresh() ;

		CListItems *items = g_stUICoze.lstOnSayDiscord->GetItems();
		items->SetFirstShowRow( items->GetCount() - items->GetShowCount() );
	}
	g_stUICoze.lstOnDiscordSay->Init();
	g_stUICoze.lstOnDiscordSay->Refresh();

	g_stUICoze.lstOnDiscordSay->Init();
	g_stUICoze.lstOnDiscordSay->Refresh();
}




void CCozeMgr::_evtSelectBudChange(CGuiData *pSender)
{
	pSender->SetIsShow( false );

	CGrid *p = dynamic_cast<CGrid*>(pSender);
	if( !p ) return;

	if ((int)(strlen( g_stUICoze.edtSay->GetCaption()))>g_stUICoze.edtSay->GetMaxNum()-3 )
		return;

	CGraph * r = p->GetSelect();
	int nIndex = p->GetSelectIndex();
    if( r )
	{
		g_stUICoze.edtSay->SetIsParseText(true);
		g_stUICoze.edtSay->SetActive( g_stUICoze.edtSay);
		//string sSource = g_stUICoze.edtSay->GetCaption();
		char pszChar[10] = {0};
		if ( nIndex <10)
			sprintf( pszChar ,"#0%d",nIndex);
		else
			sprintf( pszChar ,"#%d",nIndex);
		string sAdd   = pszChar;   //��Ҫ����ͼԪ����ƥ��
		//string sDes = sSource +sAdd;
		g_stUICoze.edtSay->ReplaceSel(sAdd.c_str());
		//g_stUICoze.edtSay->SetCaption(sDes.c_str());	
	}		
}

void CCozeMgr::_evtForbidFormLost(CGuiData *pSender)
{
	//if (m_pForbidCheck->GetActive()!=m_pForbidCheck)
	//{
	//	g_stUICoze.frmForbid->SetIsShow(false);
	//	g_stUICoze.m_pForbidCheck->SetIsChecked(false);
	//}
	g_stUICoze.m_pForbidCheck->SetIsChecked(false);
	g_stUICoze._evtForbidCheck( g_stUICoze.m_pForbidCheck );
	g_stUICoze.m_pForbidCheck->MouseRun( g_stUICoze.m_pForbidCheck->GetX() - 1, g_stUICoze.m_pForbidCheck->GetY() - 1, 0 );
}

void CCozeMgr::_evtForbidSystemChange(CGuiData *pSender)
{
	if (g_stUICoze.m_pSystemCheck->GetIsChecked())
	{
		lstOnSystemSay->SetIsShow(false);
		g_stUICoze.drpTitleSystem->SetIsShow(false);
	}
	else
	{
		lstOnSystemSay->SetIsShow(true);
		g_stUICoze.drpTitleSystem->SetIsShow(true);
	}
}

void CCozeMgr::_evtHideSelf(CGuiData *pSender)
{
	pSender->SetIsShow( false );	
}

void CCozeMgr::_evtForbidCheck(CGuiData *pSender)
{
	g_stUICoze.frmForbid->SetIsShow( g_stUICoze.m_pForbidCheck->GetIsChecked() );

	//CCheckBox *pChecker=dynamic_cast<CCheckBox*>(pSender);
	//if (!pChecker) return;
	//bool bChecked=pChecker->GetIsChecked();
	//g_stUICoze.frmForbid->SetIsShow(bChecked);

	//CP_RefuseToMe(bChecked);
	//if( bChecked )
	//	g_pGameApp->ShowMidText( "���Ѿ��ص������﹦�ܣ����˽�����������������˽�ģ��ٴε�����" );
	//else
	//	g_pGameApp->ShowMidText( "���Ѿ��������﹦�ܣ��ٴε����ر�" );
}

//void CCozeMgr::ShowLstOnSay(string str , DWORD color) 
//{			
//	//CItemRow* p1 ;
//	//CItemRow* p2 ;
//	//CItemEx *item_lstOnSay1;
//	//CItemEx *item_lstOnSay2;
//
//	while (!str.empty())
//	{
//		string text;
//		text=CutFaceText(str,46);
//		CItemRow* pItemRow=g_stUICoze.lstOnSay->NewItem();
//		CItemEx* pItem=new CItemEx(text.c_str(),color);
//		pItem->SetAlignment(eAlignCenter);
//		pItem->SetHeight(18);
//		if (text.find ("#") !=-1)	
//		{
//			pItem->SetIsParseText(true);
//		}
//		pItemRow->SetBegin(pItem);
//	}
//	if( CCompent::GetActive()!=lstOnSay->GetScroll() ) lstOnSay->OnKeyDown(VK_END);
//	g_stUICoze.lstOnSay->Refresh();
//
//
//
//	//if ( str.length() > 40 )
//	//{
//	//	const char * s = str.c_str() ;
//	//	int len = (int )  str.length() ;
//	//	
//	//	string str1 = GetSelfString( str , 40 ,true) ;
//	//	string str2 = GetSelfString( str, len - 40 , false );
//
//	//	p1 = g_stUICoze.lstOnSay->NewItem();
//	//	item_lstOnSay1 = new CItemEx(  str1.c_str(), color );
//	//	item_lstOnSay1->SetAlignment(eAlignCenter);
//	//	item_lstOnSay1->SetHeight( 18);
//	//	if (  str.find ("#") !=-1)	
//	//	{
//	//		item_lstOnSay1->SetIsParseText(true);
//	//	}
//
//	//	p1->SetBegin(item_lstOnSay1);		
//	//			
//	//	p2 = g_stUICoze.lstOnSay->NewItem();
//	//	item_lstOnSay2 = new CItemEx( str2.c_str(), color);
//	//	item_lstOnSay2->SetAlignment(eAlignCenter);
//	//	item_lstOnSay2->SetHeight( 18);
//	//
//	//	if (  str.find ("#") !=-1)
//	//	{
//	//		item_lstOnSay2->SetIsParseText(true);
//	//	}
//	//	p2->SetBegin(item_lstOnSay2);
//	//}
//	//else
//	//{
//	//	p1 = g_stUICoze.lstOnSay->NewItem();
//	//	item_lstOnSay1 = new CItemEx(  str.c_str(), color );
//	//	item_lstOnSay1->SetAlignment(eAlignCenter);
//	//	item_lstOnSay1->SetHeight(18);
//
//	//	if (  str.find ("#") !=-1)	
//	//	{
//	//		item_lstOnSay1->SetIsParseText(true);
//	//	}
//	//	p1->SetBegin(item_lstOnSay1);
//	//}
//	//if( CCompent::GetActive()!=lstOnSay ) lstOnSay->OnKeyDown(VK_END);
//	////g_stUICoze.lstOnSay->OnKeyDown(VK_END);
//	//g_stUICoze.lstOnSay->Refresh();
//}

void CCozeMgr::OnWorldSay( const char* name, const char* say ) //�������˷��㲥��Ϣ:CS_Say2All
{
	CCozeForm::GetInstance()->OnWorldMsg(name, say);
	return;										//Arcol:Test-

	/////////����lstOnSay����ʾ
	//string str = name;
	//str += ":";
	//str += say ;
	

	//string str=m_cmbChannel->GetText();
	//DWORD color;
	//if (str=="����")
	//{
	//	str="[����]"+string(name)+":"+say;
	//	color=g_stUICoze.chatColor.trade;
	//}
	//else
	//{
	//	str="[����]"+string(name)+":"+say;
	//	color=g_stUICoze.chatColor.world;
	//}

	string filter=say;
	CTextFilter::Filter(CTextFilter::DIALOG_TABLE,filter);
	PrintToList(CHANNEL_WORLD,name,filter,g_stUICoze.chatColor.world);

	//ShowLstOnSay( str, g_stUICoze.chatColor.world );
//	if( CCompent::GetActive()!=lstOnSay ) 
//		lstOnSay->OnKeyDown(VK_END);

}

void CCozeMgr::OnTradeSay( const char* name, const char* say )
{
	CCozeForm::GetInstance()->OnTradeMsg(name, say);
	return;										//Arcol:Test-

	string filter=say;
	CTextFilter::Filter(CTextFilter::DIALOG_TABLE,filter);
	PrintToList(CHANNEL_TRADE,name,filter,g_stUICoze.chatColor.trade);

	//ShowLstOnSay( str, g_stUICoze.chatColor.world );
	//	if( CCompent::GetActive()!=lstOnSay ) 
	//		lstOnSay->OnKeyDown(VK_END);

}

void CCozeMgr::OnPrivateSay( const char* srcName, const char* dstName,const char* say )   //˽�� 
{
	CCozeForm::GetInstance()->OnPrivateMsg(srcName, dstName, say);
	return;										//Arcol:Test-

	if( !CGameScene::GetMainCha() ) return;

	CCharacter* pCha = CGameScene::GetMainCha();
	string selfName=pCha->getHumanName();
	string name;
	string str;
	string filter=say;
	CTextFilter::Filter(CTextFilter::DIALOG_TABLE,filter);
	if (selfName==srcName)
	{
		name=string(dstName);
		str="<To "+name+">:"+filter;
	}
	else
	{
		name=string(srcName);
		str="<From "+name+">:"+filter;
	}
	AddNameToCard(name);
	PrintToList(CHANNEL_PRIVATE,name,str,g_stUICoze.chatColor.person);
	//ShowLstOnSay( str, g_stUICoze.chatColor.person );
	//if( CCompent::GetActive()!=lstOnSay ) lstOnSay->OnKeyDown(VK_END);
}

void CCozeMgr::OnSystemSay( const char* say )
{
	CCozeForm::GetInstance()->OnSystemMsg(say);
	return;										//Arcol:Test-

	PrintToList(CHANNEL_SYSTEM,"",say,g_stUICoze.chatColor.system);

	// CItemRow* p = lstOnSay->Add( "ϵͳ��Ϣ" );
	// p->GetIndex(0)->SetColor( g_stUIMain.chatColor.world );	

//	ShowLstOnSay( say, g_stUICoze.chatColor.world );
//	CItem *item = new CItem( say, g_stUIMain.chatColor.world );
//	p->SetBegin( item );

//	if( CCompent::GetActive()!=lstOnSay ) 
	lstOnSay->OnKeyDown(VK_END);	
}

void CCozeMgr::OnRoadSay( CCharacter* c, const char* say )
{
	CCozeForm::GetInstance()->OnSightMsg(c, say);
	return;										//Arcol:Test-

	if ( strcmp( say ,"***0")== 0 )
	{		c->GetHeadSay()->SetFaceID( 0);  return ;  }	
	else if ( strcmp( say ,"***1")== 0 )
	{		c->GetHeadSay()->SetFaceID( 1);  return ;  }
	else if ( strcmp( say ,"***2")== 0 )
	{		c->GetHeadSay()->SetFaceID( 2);  return ;  }
	else if ( strcmp( say ,"***3")== 0 )
	{		c->GetHeadSay()->SetFaceID( 3);  return ;  }
	else if ( strcmp( say ,"***4")== 0 )
	{		c->GetHeadSay()->SetFaceID( 4);  return ;  }
	else if ( strcmp( say ,"***5")== 0 )
	{		c->GetHeadSay()->SetFaceID( 5);  return ;  }
	else if ( strcmp( say ,"***6")== 0 )
	{		c->GetHeadSay()->SetFaceID( 6);  return ;  }
	else if ( strcmp( say ,"***7")== 0 )
	{		c->GetHeadSay()->SetFaceID( 7);  return ;  }
	else if ( strcmp( say ,"***8")== 0 )
	{		c->GetHeadSay()->SetFaceID( 8);  return ;  }
	else if ( strcmp( say ,"***9")== 0 )
	{		c->GetHeadSay()->SetFaceID( 9);  return ;  }
	/////////����lstOnSay����ʾ
	//string str = c->getName();
	//str += ":";
	//str += say ;
	
	string filter=say;
	CTextFilter::Filter(CTextFilter::DIALOG_TABLE,filter);
	PrintToList( CHANNEL_ROAD,c->getHumanName(),filter,g_stUICoze.chatColor.road );

	//ShowLstOnSay( str, g_stUICoze.chatColor.road );
	//if( CCompent::GetActive()!=lstOnSay ) 
	//	lstOnSay->OnKeyDown(VK_END);	



	//�Լ�ͷ���ϵ�������ʾ
	string str = string(c->getHumanName())+":"+filter;
	CItemEx *item = new CItemEx( str.c_str(), g_stUICoze.chatColor.road );
	
	if ( strlen( str.c_str()) > 32 )  //�������?32���ַ�����������?
	{
		item->SetIsMultiLine(true );
		item->ProcessString((int)strlen(c->getHumanName()) +1 );
	}			
		
	if (  str.find ("#") !=-1)	
		item->SetIsParseText(true);	
	c->GetHeadSay()->AddItem( item );
}

void CCozeMgr::_evtShowChannelList(CGuiData *pSender, int x, int y, DWORD key)
{
	//lstChannel->SetisShow
}

void CCozeMgr::OnTeamSay(DWORD cha_id, const char* say )
{
	CCozeForm::GetInstance()->OnTeamMsg(cha_id, say);
	return;				//Arcol:Test-

	if( !CGameScene::GetMainCha() ) return;

	string name;
	if ( cha_id!=CGameScene::GetMainCha()->getHumanID() )
	{
		CTeam *pTeam=g_stUIChat.GetTeamMgr()->Find(enumTeamGroup);
		if (!pTeam) return;
		CMember *pMember=pTeam->Find(cha_id);
		if (!pMember) return;
		name=pMember->GetName();
	}
	else
	{
		name=CGameScene::GetMainCha()->getHumanName();
	}
	string filter=say;
	CTextFilter::Filter(CTextFilter::DIALOG_TABLE,filter);
	PrintToList(CHANNEL_TEAM,name,filter,g_stUICoze.chatColor.team);
}

bool CCozeMgr::IsForbidChannel(eChannel channel)
{
	switch(channel)
	{
	case CHANNEL_ROAD:
		return m_pSightCheck->GetIsChecked();
	case CHANNEL_PRIVATE:
		return m_pPrivateCheck->GetIsChecked();
	case CHANNEL_TEAM:
		return m_pTeamCheck->GetIsChecked();
	case CHANNEL_GUILD:
		return m_pGuildCheck->GetIsChecked();
	case CHANNEL_WORLD:
		return m_pWorldCheck->GetIsChecked();
	case CHANNEL_SYSTEM:
		//ϵͳ��Ϣ��Զ�����ˣ�ֻ���ش���
		//return m_pSystemCheck->GetIsChecked();
		return false;
	case CHANNEL_TRADE:
		return m_pTradeCheck->GetIsChecked();
	}
	return false;
}

void CCozeMgr::PrintToList(eChannel channel,string name, string str, DWORD color)
{
	return;						//Arcol:Test-

	string strPrint="";
	switch(channel)
	{
	case CHANNEL_ROAD:
		strPrint=g_oLangRec.GetString(520)+name+":"+str;
		break;
	case CHANNEL_TEAM:
		if (name.empty())
		{
			strPrint=g_oLangRec.GetString(521)+str;
		}
		else
		{
			strPrint=g_oLangRec.GetString(521)+name+":"+str;
		}
		break;
	case CHANNEL_GUILD:
		if (name.empty())
		{
			strPrint=g_oLangRec.GetString(522)+str;
		}
		else
		{
			strPrint=g_oLangRec.GetString(522)+name+":"+str;
		}
		break;
	case CHANNEL_TRADE:
		strPrint=g_oLangRec.GetString(523)+name+":"+str;
		break;
	case CHANNEL_WORLD:
		strPrint=g_oLangRec.GetString(524)+name+":"+str;
		break;
	case CHANNEL_PRIVATE:
		strPrint=g_oLangRec.GetString(525)+str;
		break;
	case CHANNEL_SYSTEM:
		if (name.empty())
		{
			strPrint=g_oLangRec.GetString(526)+str;
		}
		else
		{
			strPrint=g_oLangRec.GetString(526)+name+":"+str;
		}
		break;
	}
	if (channel==CHANNEL_SYSTEM || channel==CHANNEL_PRIVATE)
	{
		for (int i=CHANNEL_PRIVATE;i<=CHANNEL_TRADE;i++)
		{
			m_TextRecords[i][m_TextPoint[i][1]].eTextChannel=channel;
			m_TextRecords[i][m_TextPoint[i][1]].stText=strPrint;
			m_TextRecords[i][m_TextPoint[i][1]].stName=name;
			m_TextRecords[i][m_TextPoint[i][1]].dwColor=color;
			m_TextPoint[i][1]=(m_TextPoint[i][1]+1)%DATA_RECORDS_SIZE;
			if (m_TextPoint[i][1]==m_TextPoint[i][0])
			{
				m_TextPoint[i][0]=(m_TextPoint[i][0]+1)%DATA_RECORDS_SIZE;
			}
		}
	}
	else if ( channel!=CHANNEL_ROAD )
	{
		m_TextRecords[channel][m_TextPoint[channel][1]].eTextChannel=channel;
		m_TextRecords[channel][m_TextPoint[channel][1]].stText=strPrint;
		m_TextRecords[channel][m_TextPoint[channel][1]].stName=name;
		m_TextRecords[channel][m_TextPoint[channel][1]].dwColor=color;
		m_TextPoint[channel][1]=(m_TextPoint[channel][1]+1)%DATA_RECORDS_SIZE;
		if (m_TextPoint[channel][1]==m_TextPoint[channel][0])
		{
			m_TextPoint[channel][0]=(m_TextPoint[channel][0]+1)%DATA_RECORDS_SIZE;
		}
	}
	m_TextRecords[CHANNEL_ROAD][m_TextPoint[CHANNEL_ROAD][1]].eTextChannel=channel;
	m_TextRecords[CHANNEL_ROAD][m_TextPoint[CHANNEL_ROAD][1]].stText=strPrint;
	m_TextRecords[CHANNEL_ROAD][m_TextPoint[CHANNEL_ROAD][1]].stName=name;
	m_TextRecords[CHANNEL_ROAD][m_TextPoint[CHANNEL_ROAD][1]].dwColor=color;
	m_TextPoint[CHANNEL_ROAD][1]=(m_TextPoint[CHANNEL_ROAD][1]+1)%DATA_RECORDS_SIZE;
	if (m_TextPoint[CHANNEL_ROAD][1]==m_TextPoint[CHANNEL_ROAD][0])
	{
		m_TextPoint[CHANNEL_ROAD][0]=(m_TextPoint[CHANNEL_ROAD][0]+1)%DATA_RECORDS_SIZE;
	}

	//if (m_CurSelectChannel==channel || m_CurSelectChannel==CHANNEL_ROAD || channel==CHANNEL_SYSTEM || channel==CHANNEL_PRIVATE))
	if ( (m_CurSelectChannel!=CHANNEL_ROAD && (m_CurSelectChannel==channel || channel==CHANNEL_SYSTEM || (channel==CHANNEL_PRIVATE && !IsForbidChannel(CHANNEL_PRIVATE)))) ||
		 (m_CurSelectChannel==CHANNEL_ROAD && !IsForbidChannel(channel)) )
	{
		bool indentFlag=false;
		while (!strPrint.empty())
		{
			if (indentFlag)
			{
				strPrint="      "+strPrint;
			}
			string text=CutFaceText(strPrint,44);
			CItemRow* pItemRow;
			if (channel==CHANNEL_SYSTEM)
			{
				pItemRow=g_stUICoze.lstOnSystemSay->NewItem();
			}
			else if (m_TextRecords[m_CurSelectChannel][i].eTextChannel==CHANNEL_DISCORD)
			{
				pItemRow=g_stUICoze.lstOnDiscordSay->NewItem();
			}
			else
			{
				pItemRow=g_stUICoze.lstOnSay->NewItem();
			}
			CItemEx* pItem=new CItemEx(text.c_str(),color);
			pItem->SetAlignment(eAlignCenter);
			pItem->SetHeight(18);
			pItem->SetItemName(name.c_str());
			pItem->SetIsParseText(true);
			if (!indentFlag)
			{
				pItem->SetHasHeadText(6,0xFFFFFF00);
			}
			pItemRow->SetBegin(pItem);
			indentFlag=true;
		}

		CListItems* pListItems=g_stUICoze.lstOnSystemSay->GetItems();
		if (pListItems->GetCount()>200)
		{
			pListItems->Del(pListItems->GetItem(0));
		}
		if( CCompent::GetActive()!=lstOnSystemSay->GetScroll() ) lstOnSystemSay->OnKeyDown(VK_END);
		g_stUICoze.lstOnSystemSay->Refresh();

		pListItems=g_stUICoze.lstOnSay->GetItems();
		if (pListItems->GetCount()>200)
		{
			pListItems->Del(pListItems->GetItem(0));
		}
		if( CCompent::GetActive()!=lstOnSay->GetScroll() ) lstOnSay->OnKeyDown(VK_END);
		g_stUICoze.lstOnSay->Refresh();
	}
}

void CCozeMgr::_MainOnLstKeyDownEvent(CGuiData *pSender, int x, int y, DWORD key)
{
	if( key & Mouse_RDown )
	{
		CItemRow* pItemRow=lstOnSay->GetSelectItem();
		if (!pItemRow) return;
		CItemEx* pItemEx=dynamic_cast<CItemEx*>(pItemRow->GetBegin());
		if (!pItemEx) return;
		string str=pItemEx->GetItemName();
		if (str.empty()) return;
		//string str=pItemEx->GetString();
		//if (!GetNameFormString(str,str)) return;
		m_CurCommand="@"+str+" ";
		g_stUICoze.edtSay->SetCaption(m_CurCommand.c_str());
	}
}

void CCozeMgr::SetPrivateName(const char* srcName)
{
	if (!strlen(srcName)) return;
	m_CurCommand="@"+string(srcName)+" ";
	g_stUICoze.edtSay->SetCaption(m_CurCommand.c_str());
	CCompent::SetActive(g_stUICoze.edtSay);
	AddNameToCard(srcName);
}

void CCozeMgr::CloseForm()
{
	if ( grdFace )
	{
		grdFace->SetIsShow( false );
	}
	if ( m_pCardList )
	{
		 m_pCardList->SetIsShow( false );
	}
	if ( frmForbid )
	{
		frmForbid->SetIsShow( false );
	}

}

void CCozeMgr::ResetAll()
{
	return;										//Arcol:Test-

	g_stUICoze._nSelectSayRow = 0;
	g_stUICoze._nOldSayRow = 0;
	int i=0;
	for (;i<MAX_PAGE_SAY;i++)
	{
		g_stUICoze.oldSay[i]="";
	}
	m_CurSelectChannel=CHANNEL_ROAD;
	m_cmbChannel->GetList()->GetItems()->Select(0);
	m_CurCommand="";
	for (i=0;i<CHANNELS_COUNT;i++)
	{
		for (int j=0;j<DATA_RECORDS_SIZE;j++)
		{
			m_TextRecords[i][j].stName=m_TextRecords[i][j].stText="";
			m_TextRecords[i][j].dwColor=0;
		}
	}
	for (i=CHANNEL_ROAD;i<CHANNEL_TRADE;i++)
	{
		m_TextPoint[i][1]=m_TextPoint[i][0]=0;
	}
	m_pForbidCheck->SetIsChecked(false);
	g_stUICoze.lstOnSay->GetItems()->Clear();
	g_stUICoze.frmMainChat->Refresh();
}

void CCozeMgr::AddInput( const char* str )
{
	CEdit* pEdit = dynamic_cast<CEdit*>( CCompent::GetActive() );
	if( !pEdit )
	{
		if( !edtSay ) return;

		pEdit = edtSay;
	}

	string name = pEdit->GetCaption();
	name += str;
	UINT len=pEdit->GetMaxNum();
	if (strlen(name.c_str())>len)
	{
		return;
	}
	pEdit->SetCaption( name.c_str() );
}

void CCozeMgr::_evtCardClick(CGuiData *pSender, int x, int y, DWORD key)
{
	m_pCardList->GetItems()->Clear();
	for (vector<string>::reverse_iterator it=m_strCardListData.rbegin();it!=m_strCardListData.rend();it++)
	{
		m_pCardList->Add((*it).c_str());
	}
	CItemRow *pRow=m_pCardList->Add(g_oLangRec.GetString(515));
	pRow->GetBegin()->SetColor(0xFF7F7F3F);
	m_pCardList->SetPointer(pRow);
	m_pCardList->Refresh();
	m_pCardList->SetIsShow(!m_pCardList->GetIsShow());
}

void CCozeMgr::_evtCardSelectChange(CGuiData *pSender)
{
	CList *pList=dynamic_cast<CList*>(pSender);
	pList->SetIsShow(false);
	CItemRow *pRow=pList->GetSelectItem();
	if (!pRow) return;
	string strName=pRow->GetBegin()->GetString();
	if (strName.length()==0) return;
	if (strName==g_oLangRec.GetString(515) && pList->GetPointer()==pRow)
	{
		m_strCardListData.clear();
		return;
	}
	string strChat=edtSay->GetCaption();
	int nLTrim=0;
	if (strChat[0]=='*' || strChat[0]=='^' || strChat[0]=='!' || strChat[0]=='*' || strChat[0]=='%')
	{
		nLTrim=1;
	}
	else if ( strChat[0]=='@' )
	{
		nLTrim = (int)(strChat.find(" ")) +1;
	}
	if (nLTrim>0 && nLTrim<=(int)strChat.length())
	{
		strChat=strChat.substr(nLTrim,strChat.length()-nLTrim);
	}
	strChat="@"+strName+" "+strChat;
	if (strChat.length()>80)
	{
		strChat="@"+strName+" ";
	}
	edtSay->SetCaption(strChat.c_str());
}

void CCozeMgr::AddNameToCard(string strName)
{
	if (strName.length()==0) return;
	for (vector<string>::iterator it=m_strCardListData.begin();it!=m_strCardListData.end();it++)
	{
		if ((*it)==strName)
		{
			m_strCardListData.erase(it);
			m_strCardListData.push_back(strName);
			return;
		}
	}
	m_strCardListData.push_back(strName);
	if (m_strCardListData.size()>CARD_LIST_MAX)
	{
		m_strCardListData.erase(m_strCardListData.begin());
	}
}

void CCozeMgr::_evtGmForm( CForm* pForm, bool& IsShow )
{
	CCharacter* pMain = CGameScene::GetMainCha();
	if( !pMain ) return;

	if(pMain->getGMLv()==0)
	{
		IsShow=false;
	}
}


void CCozeMgr::_evtGmSay(CGuiData *pSender)
{
	CCharacter* pCha = CGameScene::GetMainCha();
	if( !pCha ) return;
	
	//����?�ǿո�����ʾ	
	const char* s = g_stUICoze.edtGmSay->GetCaption() ;
	int len =(int) strlen( s );
	int i = 0;
	for ( ; i <len  ;i++)
	{
		if 	(s[i]!= 32 )  //32�ǿո��ascii��
			break;
	}	
	if (i == len )
		return ;


	CS_GM1Say(s);
	g_stUICoze.edtGmSay->SetCaption("");
}

void CCozeMgr::OnGmSay( const char* name, const char* say )
{
	string str;
	//str="[ϵͳ����]"+string(name)+":"+string(say);
	str=g_oLangRec.GetString(507)+string(say);
	//str=string(say);
	g_pGameApp->ShowNotify(str.c_str(),g_stUICoze.chatColor.gm);
}

void CCozeMgr::OnGuildSay( const char* name, const char* say )
{
	CCozeForm::GetInstance()->OnGuildMsg(name, say);
	return;				//Arcol:Test-

	string filter=say;
	CTextFilter::Filter(CTextFilter::DIALOG_TABLE,filter);
	PrintToList(CHANNEL_GUILD,name,filter,g_stUICoze.chatColor.guild);
}



#endif
