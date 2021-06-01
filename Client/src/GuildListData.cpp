//------------------------------------------------------------------------
//	2005.4.25	Arcol	create this file
//------------------------------------------------------------------------

#include "stdafx.h"
#include ".\guildlistdata.h"

CGuildListData::CGuildListData(void)
{
	m_dwID=0;
	m_strName="";
	m_strMottoName="";
	m_strMasterName="";
	m_dwMembers=0;
	m_i64Exp=0;
	m_eType=CGuildData::pirate;
}

CGuildListData::CGuildListData(DWORD dwID,string strName,string strMottoName,string strMasterName,DWORD dwMemberCount,__int64 i64Exp)
{
	SetGuildID(dwID);
	SetGuildName(strName);
	SetGuildMottoName(strMottoName);
	SetGuildMasterName(strMasterName);
	SetExperence(i64Exp);
	SetMembers(dwMemberCount);
	
	SetGuildType(CGuildData::pirate);
}

CGuildListData::~CGuildListData(void)
{
}
