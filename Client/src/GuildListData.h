#pragma once

#include "GuildData.h"

class CGuildListData
{
public:
	CGuildListData(void);
	CGuildListData(DWORD dwID,string strName,string strMottoName,string strMasterName,DWORD dwMemberCount,__int64 i64Exp);
	~CGuildListData(void);
	void	SetGuildID(DWORD dwID)	{ m_dwID=dwID; }
	DWORD	GetGuildID()	{ return m_dwID; }
	void	SetGuildName(string strName)	{ m_strName=strName; }
	string	GetGuildName()	{ return m_strName; }
	void	SetGuildMottoName(string strMottoName)	{ m_strMottoName=strMottoName; }
	string	GetGuildMottoName()	{ return m_strMottoName; }
	void	SetGuildType(CGuildData::eType type)	{ m_eType=type; }
	CGuildData::eType	GetGuildType()	{ return m_eType; }
	void	SetGuildMasterName(string strMasterName)	{ m_strMasterName=strMasterName; }
	string	GetGuildMasterName()	{ return m_strMasterName; }
	void	SetExperence(__int64 i64Exp)	{ m_i64Exp=i64Exp; }
	__int64	GetExperence()	{ return m_i64Exp; }
	void	SetMembers(DWORD dwMemberCount)	{ m_dwMembers=dwMemberCount; }
	DWORD	GetMembers()	{ return m_dwMembers; }

private:
	DWORD	m_dwID;
	string	m_strName;
	string	m_strMottoName;
	string	m_strMasterName;
	DWORD	m_dwMembers;
	__int64	m_i64Exp;
	CGuildData::eType	m_eType;
};
