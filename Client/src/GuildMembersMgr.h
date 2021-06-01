#pragma once

class CGuildMembersMgr
{
public:
	CGuildMembersMgr(void);
	~CGuildMembersMgr(void);
	static void	AddGuildMember(CGuildMemberData* pGuildMember);
	static bool	DelGuildMember(CGuildMemberData* pGuildMember);
	static bool	DelGuildMemberByID(DWORD dwID);
	static bool	DelGuildMemberByName(string strName);
	static CGuildMemberData*	GetSelfData();
	static CGuildMemberData*	FindGuildMemberByID(DWORD dwID);
	static CGuildMemberData*	FindGuildMemberByName(string strName);
	static CGuildMemberData*	FindGuildMemberByIndex(DWORD dwIndex);
	static DWORD	GetTotalGuildMembers();
	static void	ResetAll();

private:
	static vector<CGuildMemberData*> m_pGuildMembers;
};
