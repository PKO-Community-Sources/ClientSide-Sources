#pragma once

class CRecruitMemberData
{
public:
	CRecruitMemberData(void);
	~CRecruitMemberData(void);
	void	SetID(DWORD dwID)	{ m_dwID=dwID; }
	DWORD	GetID()	{ return m_dwID;}
	void	SetName(string strName)	{ m_strName=strName; }
	string	GetName()	{ return m_strName; }
	void	SetJob(string strJob)	{ m_strJob=strJob; }
	string GetJob()	{ return m_strJob; }
	void SetLevel(DWORD	dwLv)	{ m_dwLv=dwLv; }
	DWORD GetLevel()	{ return m_dwLv; }

private:
	DWORD	m_dwID;
	string	m_strName;
	string	m_strJob;
	DWORD	m_dwLv;
};
