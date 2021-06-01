#pragma once

class CGuildData
{
public:
	enum eType{navy,pirate};
	enum eState{normal=0,money=1,member=2,repute=4};
	struct sGuildData
	{
		DWORD	guild_id;
		string	guild_name;
		string	guild_motto_name;
		CGuildData::eType	guild_type;
		string	guild_master_name;
		__int64	guild_money;
		__int64	guild_experience;
		DWORD	guild_member_count;
		DWORD	guild_member_max;
		eState	guild_state;
		DWORD	guild_remain_time;
	};
	CGuildData(void);
	~CGuildData(void);

	static void LoadData(const sGuildData data);
	static void	SetGuildID(const DWORD dwID)	{ m_dwID=dwID; }
	static DWORD	GetGuildID()	{ return m_dwID; }
	static void	SetGuildName(const string strName)	{ m_strName=strName; }
	static string	GetGuildName()	{ return m_strName; }
	static void	SetGuildMottoName(const string strName)	{ m_strMottoName=strName; }
	static string	GetGuildMottoName()	{ return m_strMottoName; }
	static void	SetGuildType(const eType type)	{ m_eType=type; }
	static eType	GetGuildType()	{ return m_eType; }
	static void	SetGuildState(const eState state)	{ m_eState=state; }
	static eState	GetGuildState()	{ return m_eState; }
	static void	SetGuildLevel(const DWORD dwLv)	{ m_dwLv=dwLv; }
	static DWORD	GetGuildLevel()	{ return m_dwLv; }
	static void	SetGuildMasterID(const DWORD dwID)	{ m_dwMasterID=dwID; }
	static DWORD	GetGuildMasterID()	{ return m_dwMasterID; }
	static void	SetGuildMasterName(const string strMasterName)	{ m_strMasterName=strMasterName; }
	static string	GetGuildMasterName()	{ return m_strMasterName; }
	static void	SetExperence(const __int64 i64Exp)	{ m_i64Exp=i64Exp; }
	static __int64	GetExperence()	{ return m_i64Exp; }
	static void	SetMemberCount(const DWORD dwCount)	{ m_dwMemberCount=dwCount; }
	static DWORD	GetMemberCount()	{ return m_dwMemberCount; }
	static void	SetMaxMembers(const DWORD dwCount)	{ m_dwMaxMembers=dwCount; }
	static DWORD	GetMaxMembers()	{ return m_dwMaxMembers; }
	static void	SetMoney(const __int64 i64Money)	{ m_i64Money=i64Money; }
	static __int64	GetMoney()	{ return m_i64Money; }
	static void	SetRemainTime(const __int64 i64RemainTime)	{ m_i64RemainTime=i64RemainTime; }
	static __int64	GetRemainTime()	{ return m_i64RemainTime; }
	static void	Reset();

private:
	static DWORD	m_dwID;
	static string	m_strName;
	static string	m_strMottoName;
	static eType	m_eType;
	static eState	m_eState;
	static DWORD	m_dwLv;
	static DWORD	m_dwMasterID;
	static string	m_strMasterName;
	static __int64	m_i64Exp;
	static DWORD	m_dwMemberCount;
	static DWORD	m_dwMaxMembers;
	static __int64	m_i64Money;
	static __int64	m_i64RemainTime;
};
