#pragma once

class CGuildMemberData
{
public:
	CGuildMemberData(void);
	~CGuildMemberData(void);
	void	SetID(DWORD dwID)	{ m_dwID=dwID; }
	DWORD	GetID()	{ return m_dwID;}
	void	SetName(string strName)	{ m_strName=strName; }
	string	GetName()	{ return m_strName; }
	void	SetMottoName(string strMottoName)	{ m_strMottoName=strMottoName; }
	string	GetMottoName()	{ return m_strMottoName; }
	void	SetIcon(DWORD dwIcon)	{ m_dwIcon=dwIcon; }
	DWORD	GetIcon()	{ return m_dwIcon;}
	void	SetJob(string strJob)	{ m_strJob=strJob; }
	string GetJob()	{ return m_strJob; }
	void SetLevel(DWORD	dwLv)	{ m_dwLv=dwLv; }
	DWORD GetLevel()	{ return m_dwLv; }
	void SetOnline(bool bOnline)	{ m_bOnline=bOnline; }
	bool IsOnline()	{ return m_bOnline; }
	void SetManager(bool bMgr)	{ m_bMgr=bMgr; }
	bool IsManager()	{ return m_bMgr; }
	void SetPointer(void* pointer)	{ m_pPointer=pointer; }
	void* GetPointer()	{ return m_pPointer; }
	
	void SetPerm( int	perm)	{ m_cPerm=perm; }
	unsigned int GetPerm()	{ return m_cPerm; }

private:
	DWORD	m_dwID;
	string	m_strName;
	string	m_strMottoName;
	string	m_strJob;
	DWORD	m_dwLv;	
	bool	m_bOnline;
	bool	m_bMgr;
	DWORD	m_dwIcon;
	void*	m_pPointer;
	unsigned int			m_cPerm;

	
};
