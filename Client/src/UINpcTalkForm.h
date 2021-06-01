#pragma once
#include "UIGlobalVar.h"
#include "NetProtocol.h"
class	CCharacter2D;
//struct	stNetChangeChaPart;

namespace GUI
{

// 与NPC交谈
class CNpcTalkMgr : public CUIInterface
{
public:
	void			ShowFuncPage( BYTE byFuncPage , BYTE byCount,BYTE byMisNum ,const NET_FUNCPAGE& FuncArray, DWORD dwNpcID ) ;      //显示 NPC功能页

	void			AddHelpInfo( const NET_HELPINFO &Info );
	void			ShowTalkPage( const char *content, BYTE command, DWORD npcID );
	void			CloseTalk( DWORD dwNpcID );
	void				RefreshNpcModel();
	bool				RefreshNpcModelPos();
	void				HideNpcModel();
	DWORD			GetNpcId();

	static void     SetTalkStyle( BYTE bit )	{ _byTalkStyle = bit ;  }

protected:
	virtual bool Init();
    virtual void End();
	virtual void FrameMove(DWORD dwTime);
	virtual void LoadingCall();
	virtual void SwitchMap();

	void	CloseForm();
	void    ShowHelpInfo( const NET_HELPINFO& Info );

	static void _MainMouseNPCEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _evtMemSelectChange(CGuiData *pSender);

private:
	CForm*		frmNPCchat;
	CMemo*		memCtrl;
	
private:
	static BYTE		_byTalkStyle;		// 交易的类型
	static CCharacter2D* pNpc;
	static C3DCompent* p3D ;
	static void _NpcRenderEvent(C3DCompent *pSender, int x, int y);
	static void _OnDragEnd(CGuiData *pSender, int x, int y, DWORD key );
	bool		m_bIsNpcTalk;

	typedef list<NET_HELPINFO> HELP_LIST;
	HELP_LIST	m_HelpInfoList;

};

}

