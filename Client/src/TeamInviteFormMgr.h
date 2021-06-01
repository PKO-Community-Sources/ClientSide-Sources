#pragma once

class CTeamInviteFormMgr
{
public:
	typedef struct _sFormNode
	{
		DWORD id;
		string inviterName;
		CForm *pForm;
	}S_FormNode,*PS_FormNode;
	CTeamInviteFormMgr(void);
	~CTeamInviteFormMgr(void);
	static bool AddInviteForm(DWORD id,string inviterName);
	static bool RemoveInviteForm(DWORD id);
	static PS_FormNode GetInviteForm(DWORD id);
private:
	static vector<PS_FormNode> m_FormLink;
	static void _MainMousePlayerGroupEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
};

extern CTeamInviteFormMgr g_stTeamInviteFormMgr;
