#pragma once
#include "UIGlobalVar.h"


//2006-1-8 By Arcol:新的快捷聊天信息组件CCharMsg完成，此组件未使用也未经测试，因原结构比较混乱，建议使用此新组件
class CCharMsg
{
public:
	enum eChannel
	{
		CHANNEL_NONE			= 0,
		CHANNEL_ALL				= 1,
		CHANNEL_SIGHT			= 2,
		CHANNEL_PRIVATE			= 4,
		CHANNEL_WORLD			= 8,
		CHANNEL_TRADE			= 16,
		CHANNEL_TEAM			= 32,
		CHANNEL_GUILD			= 64,
		CHANNEL_SYSTEM			= 128,
		CHANNEL_PUBLISH			= 256,
		CHANNEL_SIDE			= 512,	// 阵营
		CHANNEL_VIP				= 1024,	
		CHANNEL_DISCORD			= 2048,	
	};

	struct sTextInfo
	{
		eChannel eTextChannel;
		string	 strWho;
		string	 strText;
		string	 strShowText;
		bool	 bSendTo;
		DWORD	dwColour;
	};

private:
	typedef list<sTextInfo> lstTextInfoType;
	struct sChannelInfo
	{
		eChannel					enumChannel;
		string						strChannelName;
		DWORD						dwChannelColor;
		string						strChannelCommand;
		DWORD						dwTotalMsg;
		lstTextInfoType::iterator	itFirstMsgPos;
	};


public:
	CCharMsg();
	~CCharMsg();

	static void		AddMsg(eChannel channel, string strWho, string strText, bool bSendTo = false,DWORD dwColour = 0xFFFFFFFF);
	static void		SetChannelColor(eChannel channel, DWORD dwColor);
	static DWORD	GetChannelColor(eChannel channel);
	static string	GetChannelCommand(WORD wChannelIndex);
	static string	GetChannelName(eChannel channel);
	static string	GetChannelShowName(eChannel channel);
	static WORD		GetChannelIndex(eChannel channel);
	static eChannel	GetChannelByIndex(WORD wChannelIndex);
	static WORD		GetTotalChannelCount();
	static void		ClearAllMsg();

	void			SetShowChannels(DWORD ecboShowChannels);
	bool			ModifyShowChannel(eChannel eShowChannel, bool bAddOrRemove = true, bool bShowTips = true);
	bool			IsShowChannel(eChannel channel);
	DWORD			GetShowChannels();

	bool			MoveToFirstMsg();
	bool			MoveToNextMsg();
	sTextInfo		GetMsgInfo();


private:


private:

	static const WORD			m_wTotalChannelsCount = 13;	// 添加阵营修改 10 -> 11
	static const DWORD			m_dwChannelBufferSize = 100;
	static sChannelInfo			m_sChannelInfo[m_wTotalChannelsCount];
	static lstTextInfoType		m_lstMsgLink;
	typedef list<CCharMsg*>		lstInstanceType;
	static lstInstanceType		m_lstThisInstanceLink;
	
	bool						m_bCurMsgAvailable;
	DWORD						m_ecboShowChannels;
	lstTextInfoType::iterator	m_itCurrentMsgPos;
};

class CCardCase
{
public:
	CCardCase(WORD wMaxLimit=10);
	~CCardCase();
	void	AddCard(string str);
	bool	RemoveCard(string str);
	void	ClearAll();
	int		GetTotalCount();
	bool	MoveToFirstCard();
	bool	MoveToNextCard();
	bool	MoveToLastCard();
	bool	MoveToPrevCard();
	string	GetCardInfo();

private:
	typedef list<string>	lstCardType;
	lstCardType				m_lstCardData;
	lstCardType::iterator	m_itCurrentCardPos;
	int						m_wMaxLimit;
};

namespace GUI
{

class CChannelSwitchForm : public CUIInterface
{
public:

	CChannelSwitchForm();
	static CChannelSwitchForm* GetInstance();
	~CChannelSwitchForm();
	void SwitchCheck();

protected:
	
	bool Init();
	static void EventLostFocus(CGuiData *pSender);
	static void EventPrivateCheckChange(CGuiData *pSender);
	static void EventSightCheckChange(CGuiData *pSender);
	static void EventSystemCheckChange(CGuiData *pSender);
	static void EventTeamCheckChange(CGuiData *pSender);
	static void EventGuildCheckChange(CGuiData *pSender);
	static void EventWorldCheckChange(CGuiData *pSender);
	static void EventTradeCheckChange(CGuiData *pSender);
	static void EventDiscordCheckChange(CGuiData *pSender);
	static void EventVIPCheckChange(CGuiData *pSender);

private:
	CForm		*m_frmChannelSwitch;
	CCheckBox	*m_chkPrivate;
	CCheckBox	*m_chkSight;
	CCheckBox	*m_chkSystem;
	CCheckBox	*m_chkTeam;
	CCheckBox	*m_chkGuild;
	CCheckBox	*m_chkWorld;
	CCheckBox	*m_chkTrade;
	CCheckBox	*m_chkVIP;
	CCheckBox	*m_chkDiscord;
};



class CCozeForm : public CUIInterface
{
	friend CChannelSwitchForm;
public:
	CCozeForm();
	~CCozeForm();
	static CCozeForm* GetInstance();

	void OnVIPMsg(string strName, string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnDiscordMsg(string strName, string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnSightMsg(string strName, string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnSightMsg(CCharacter* pChar, string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnPrivateMsg(string strFromName, string strToName, string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnWorldMsg(string strName, string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnTradeMsg(string strName, string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnTeamMsg(string strName, string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnTeamMsg(DWORD dwCharID, string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnGuildMsg(string strName, string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnSystemMsg(string strMsg);
	void OnSideMsg(string strName, string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnPublishMsg(string strName, string strMsg);
	void OnPublishMsg1(string strMsg,int setnum);//Add by sunny.sun20080804
	void OnPrivateNameSet(string strName);
	void OnResetAll();
	bool IsMouseOnList(int x, int y);
	void AddToEdit(string strData);

protected:
	bool Init();

	void SendMsg();
	void UpdatePages();
	void ResetPages();
	void ChangePrivatePlayerName(string strName);

	CCharMsg::eChannel	GetCmdFromMsg(string strMsg);

protected:
	static void EventPublishShowForm(CForm* pForm, bool& IsShow);
	static void EventPublishSendMsg(CGuiData *pSender);
	static bool EventGlobalKeyDownHandle(int& key);
	static void EventSendMsg(CGuiData *pSender);
	static bool EventEditMsg(CGuiData *pSender, int& key);
	static void EventMainListKeyDown(CGuiData *pSender, int x, int y, DWORD key);
	static void EventSendChannelSwitchClick(CGuiData *pSender, int x, int y, DWORD key);
	static void EventSendChannelChange(CGuiData *pSender);
	static void EventMainPageDragBegin(CGuiData *pSender, int x, int y, DWORD key);
	static void EventMainPageDragging(CGuiData *pSender, int x, int y, DWORD key);
	static void EventMainPageDragEnd(CGuiData *pSender, int x, int y, DWORD key);
	static void EventSystemPageDragBegin(CGuiData *pSender, int x, int y, DWORD key);
	static void EventSystemPageDragging(CGuiData *pSender, int x, int y, DWORD key);
	static void EventSystemPageDragEnd(CGuiData *pSender, int x, int y, DWORD key);
	
	static void EventDiscordListKeyDown(CGuiData *pSender, int x, int y, DWORD key);

	static void EventDiscordPageDragBegin(CGuiData *pSender, int x, int y, DWORD key);
	static void EventDiscordPageDragging(CGuiData *pSender, int x, int y, DWORD key);
	static void EventDiscordPageDragEnd(CGuiData *pSender, int x, int y, DWORD key);
	
	static void EventChannelSwitchCheck(CGuiData *pSender);
	static void EventCallingCardSwitchClick(CGuiData *pSender, int x, int y, DWORD key);
	//static void EventCallingCardLostFocus(CGuiData *pSender);
	static void EventCardSelected(CGuiData *pSender);
	static void EventFacePanelSwitchClick(CGuiData *pSender, int x, int y, DWORD key);
	//static void EventFacePanelLostFocus(CGuiData *pSender);
	static void EventFaceSelected(CGuiData *pSender);
	static void EventBrowPanelSwitchClick(CGuiData *pSender, int x, int y, DWORD key);
	static void EventBrowSelected(CGuiData *pSender);
	static void EventActionPanelSwitchClick(CGuiData *pSender, int x, int y, DWORD key);
	static void EventActionSelected(CGuiData *pSender);
    void ReplaceSpecialFace( string &strMsg ,const string & strReplace ,const string & strFace );//Add by sunny.sun 20080902

private:
	CForm			*m_frmMainChat;
	CForm			*m_frmPublish;

	CCombo			*m_cmbChannel;
	CEdit			*m_edtPublishMsg;
	CEdit			*m_edtMsg;

	CList			*m_lstMainPage;
	CList			*m_lstSystemPage;
	CList			*m_lstDiscordPage;
	CDragTitle		*m_drgMainPage;
	CDragTitle 		*m_drgSystemPage;
	CDragTitle 		*m_drgDiscordPage;
	CCheckBox		*m_chkChannelSwitch;
	CTextButton		*m_btnCallingCardSwitch;
	CList			*m_lstCallingCard;
	CTextButton		*m_btnFaceSwitch;
	CGrid			*m_grdFacePanel;
	CTextButton		*m_btnBrowSwitch;
	CGrid			*m_grdBrowPanel;
	CTextButton		*m_btnActionSwitch;
	CGrid			*m_grdActionPanel;

	CCharMsg::eChannel	m_eCurSelChannel;
	CCharMsg			m_cMainMsg;
	CCharMsg			m_cSystemMsg;
	CCharMsg			m_cDiscordMsg;
	CCardCase			m_cCallingCard;
	CCardCase			m_cSendMsgCard;

	bool				m_bSendMsgCardSwitch;
	int					m_nHeightBeforeDrag;
	int					m_nTopBeforeDrag;
	static const int	m_nMainPageMinHeight;
	static const int	m_nMainPageMaxHeight;
	static const int	m_nSystemPageMinHeight;
	static const int	m_nSystemPageMaxHeight;
};





















#if 0



// 与游戏中的聊天,即左下角游戏中的聊天
class CCozeMgr : public CUIInterface
{
	enum 
	{
		MAX_PAGE_SAY	= 10,		// 按up,down最大能翻出的行数
	};

	static const int DATA_RECORDS_SIZE = 200;
	static const int CHANNELS_COUNT = 9;	// 阵营频道添加, 8 -> 9

public:
	struct stChatColor
	{
		stChatColor() : road(0xFFFFFFFF), person(0xFFFFFFFF), team(0xFFFFFFFF), guild(0xFFFFFFFF), world(0xFFFFFFFF), system(0xFFFFFFFF), trade(0xFFFFFFFF), gm(0xFFFFFFFF, side(0xFFFFFFFF)) {}

		DWORD	road;		//a）	公聊
		DWORD	person;		//b）	私聊
		DWORD	team;		//c）	队聊
		DWORD	guild;		//d）	团聊
		DWORD   world;		//e）	广播
		DWORD   system;		//f）	系统
		DWORD   trade;		//g）	交易
		DWORD	gm;			//h）	GM
		DWORD	side;
	};

	enum eChannel
	{
		CHANNEL_ROAD,
		CHANNEL_PRIVATE,
		CHANNEL_TEAM,
		CHANNEL_GUILD,
		CHANNEL_WORLD,
		CHANNEL_SYSTEM,
		CHANNEL_TRADE,
		CHANNEL_SIDE		// 阵营频道添加
	};

	struct sTextInfo
	{
		eChannel eTextChannel;
		string stName;
		string stText;
		DWORD dwColor;
	};


	static bool		IsMouseOnSayList( int x, int y );

	static void		OnGmSay( const char* name, const char* say );
	static void     OnSystemSay( const char* say );							// 系统信息
	static void		OnRoadSay( CCharacter* p, const char* say );
	static void		OnWorldSay( const char* name, const char* say );
	static void		OnTradeSay( const char* name, const char* say );
	static void		OnTeamSay(DWORD cha_id, const char* say );
	static void		OnGuildSay( const char* name, const char* say );
	static void		OnPrivateSay( const char* srcName, const char* dstName,const char* say );
	static void		SetPrivateName(const char* srcName);
	static void		ResetAll();

	static CGrid*   GetFaceGrid()		{ return	grdFace;	}

	void		AddInput( const char* str );
	static void AddNameToCard(string strName);
	static bool IsForbidChannel(eChannel channel);

	stChatColor		chatColor;

protected:
	bool Init();
    void End();
	void CloseForm();

private:
	void		NextChatState();

	static void _evtGmForm( CForm* pForm, bool& IsShow );
	static void _evtSay(CGuiData *pSender);			// 发送聊天
	static void _evtGmSay(CGuiData *pSender);
	static bool _evtSayKeyDown(CGuiData *pSender, int& key);
	static void _evtChannelSelectChange(CGuiData *pSender);
	static void _evtCardSelectChange(CGuiData *pSender);
	static void _evtShowChannelList(CGuiData *pSender, int x, int y, DWORD key);
	static void _evtForbidCheck(CGuiData *pSender);
	static void _evtCardClick(CGuiData *pSender, int x, int y, DWORD key);
	
	static void _MainOnLstKeyDownEvent(CGuiData *pSender, int x, int y, DWORD key);
	static bool _MainOnSayKeyDownEvent( int& key );

	static void _DragBeginOnSystemTitle(CGuiData *pSender, int x, int y, DWORD key);
	static void _DragMoveOnSystemTitle(CGuiData *pSender, int x, int y, DWORD key);
	static void _DragEndOnSystemTitle(CGuiData *pSender, int x, int y, DWORD key);

	static void _DragBeginOnDiscordTitle(CGuiData *pSender, int x, int y, DWORD key);
	static void _DragMoveOnDiscordTitle(CGuiData *pSender, int x, int y, DWORD key);
	static void _DragEndOnDiscordTitle(CGuiData *pSender, int x, int y, DWORD key);

	
	static void _DragBeginOnSay(CGuiData *pSender, int x, int y, DWORD key);
	static void _DragMoveOnSay(CGuiData *pSender, int x, int y, DWORD key);
	static void _DragEndOnSay(CGuiData *pSender, int x, int y, DWORD key);

	static void _evtSelectBudChange(CGuiData *pSender) ;       //包子选择发生变化
	static void _evtHideSelf(CGuiData *pSender);	// 隐藏自己

	static void _evtForbidFormLost(CGuiData *pSender);
	static void _evtForbidSystemChange(CGuiData *pSender);



	//static void ShowLstOnSay(string str , DWORD color);
	static void PrintToList(eChannel channel, string name, string str, DWORD color);

private:
	CForm*			frmMainChat; 
	CForm*			frmGM; 
	CForm*			frmForbid; 

	static CCheckBox* m_pPrivateCheck;
	static CCheckBox* m_pSightCheck;
	static CCheckBox* m_pSystemCheck;
	static CCheckBox* m_pTeamCheck;
	static CCheckBox* m_pGuildCheck;
	static CCheckBox* m_pWorldCheck;
	static CCheckBox* m_pTradeCheck;

	
	//static CCheckBox*	m_p

	static CList*	lstOnSay;
	static CList*	lstOnSystemSay;
	static CList*	lstOnDiscordSay;
	CList*			lstChannel;
	static CGrid*	grdFace;

	CDragTitle*		drpTitle;
	CDragTitle*		drpTitleSystem;
	CDragTitle*		drpTitleDiscord;
	static CEdit*	edtSay;
	static CEdit*	edtGmSay;
	static CCheckBox* m_pForbidCheck;
	static CTextButton* m_pbtnCard;
	static CList* m_pCardList;


	static CCombo*	m_cmbChannel;

private:
	string		oldSay[MAX_PAGE_SAY];			// 以前说的话

	int		_nOldSayRow;		// 上一次说话时记录的行号,用于上下键直接显示以前打入的话
	int		_nSelectSayRow;

	int		_nBeginOnSayY;		// 拖动前lstOnSay的Y位置
	int		_nBeginOnSayHeight;	// 拖动前lstOnSay的高
	int		_nDragSystemMinY, _nDragSystemMaxY;
	int		_nDragMinY, _nDragMaxY;
	int		_nDragDiscordMinY, _nDragDiscordMaxY;

	static eChannel m_CurSelectChannel;
	static string m_CurCommand;
	static int m_TextPoint[CHANNELS_COUNT][2];
	static sTextInfo m_TextRecords[CHANNELS_COUNT][DATA_RECORDS_SIZE];
	static vector<string> m_strCardListData;
};
#endif

}

