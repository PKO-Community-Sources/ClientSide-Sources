//------------------------------------------------------------------------
//	2005.4.25	Arcol	create this file
//------------------------------------------------------------------------

#include "stdafx.h"
#include "GuildData.h"
#include "guildlistdata.h"
#include ".\guildlistmgr.h"


void SwapData(vector<CGuildListData*>::reference swap1, vector<CGuildListData*>::reference swap2)
{
	CGuildListData* temp=swap1;
	swap1=swap2;
	swap2=temp;
}

vector<CGuildListData*> CGuildListMgr::m_pGuildList;
CGuildData::eType	CGuildListMgr::m_eType=CGuildData::pirate;

CGuildListMgr::CGuildListMgr(void)
{
}

CGuildListMgr::~CGuildListMgr(void)
{
	ResetAll();
}

void CGuildListMgr::AddGuild(CGuildListData* m_pGuild)
{
	m_pGuild->SetGuildType(m_eType);
	m_pGuildList.push_back(m_pGuild);
}

bool CGuildListMgr::DelGuild(CGuildListData* m_pGuild)
{
	vector <CGuildListData*>::iterator Iter;
	for (Iter=m_pGuildList.begin();Iter!=m_pGuildList.end();Iter++)
	{
		if ((*Iter)==m_pGuild)
		{
			//delete (*Iter);
			SAFE_DELETE(*Iter); // UI��������
			m_pGuildList.erase(Iter);
			return true;
		}
	}
	return false;
}

bool CGuildListMgr::DelGuildByID(DWORD dwID)
{
	vector <CGuildListData*>::iterator Iter;
	for (Iter=m_pGuildList.begin();Iter!=m_pGuildList.end();Iter++)
	{
		if ((*Iter)->GetGuildID()==dwID)
		{
			//delete (*Iter);
			SAFE_DELETE(*Iter); // UI��������
			m_pGuildList.erase(Iter);
			return true;
		}
	}
	return false;
}

bool CGuildListMgr::DelGuildByName(string strName)
{
	vector <CGuildListData*>::iterator Iter;
	for (Iter=m_pGuildList.begin();Iter!=m_pGuildList.end();Iter++)
	{
		if ((*Iter)->GetGuildName()==strName)
		{
			//delete (*Iter);
			SAFE_DELETE(*Iter); // UI��������
			m_pGuildList.erase(Iter);
			return true;
		}
	}
	return false;
}

CGuildListData* CGuildListMgr::FindGuildByID(DWORD dwID)
{
	vector <CGuildListData*>::iterator Iter;
	for (Iter=m_pGuildList.begin();Iter!=m_pGuildList.end();Iter++)
	{
		if ((*Iter)->GetGuildID()==dwID)
		{
			return (*Iter);
		}
	}
	return NULL;
}

CGuildListData* CGuildListMgr::FindGuildByName(string strName)
{
	vector <CGuildListData*>::iterator Iter;
	for (Iter=m_pGuildList.begin();Iter!=m_pGuildList.end();Iter++)
	{
		if ((*Iter)->GetGuildName()==strName)
		{
			return (*Iter);
		}
	}
	return NULL;
}

CGuildListData* CGuildListMgr::FindGuildByIndex(DWORD dwIndex)
{
	if (dwIndex>=GetTotalGuilds()) return NULL;
	return m_pGuildList[dwIndex];
}

DWORD CGuildListMgr::GetTotalGuilds()
{
	return static_cast<DWORD>(m_pGuildList.size());
}

void CGuildListMgr::ResetAll()
{
	vector <CGuildListData*>::iterator Iter;
	while (m_pGuildList.size()>0)
	{
		Iter=m_pGuildList.begin();
		CGuildListData* pNode=*Iter;
		//delete pNode;
		SAFE_DELETE(pNode); // UI��������
		m_pGuildList.erase(Iter);
	}
}

void CGuildListMgr::SortGuildsByName(bool bInc)
{
	for (int i=0;i<int(m_pGuildList.size())-1;i++)
	{
		for (int j=i+1;j<int(m_pGuildList.size());j++)
		{
			if (m_eType==CGuildData::navy)
			{
				if ( (m_pGuildList[i]->GetGuildID() >m_pGuildList[j]->GetGuildID() && bInc) ||
					(m_pGuildList[i]->GetGuildID()<m_pGuildList[j]->GetGuildID() && !bInc) )
				{
					//CGuildListData* temp=m_pGuildList[i];
					//m_pGuildList[i]=m_pGuildList[j];
					//m_pGuildList[j]=temp;
					SwapData(m_pGuildList[i],m_pGuildList[j]);
				}
			}
			else
			{
				if ( (m_pGuildList[i]->GetGuildName()>m_pGuildList[j]->GetGuildName() && bInc) ||
					(m_pGuildList[i]->GetGuildName()<m_pGuildList[j]->GetGuildName() && !bInc) )
				{
					SwapData(m_pGuildList[i],m_pGuildList[j]);
				}
			}
		}
	}
}

void CGuildListMgr::SortGuildsByMemberCount(bool bInc)
{
	for (int i=0;i<int(m_pGuildList.size())-1;i++)
	{
		for (int j=i+1;j<int(m_pGuildList.size());j++)
		{
			if (m_eType==CGuildData::navy)
			{
				if ( (m_pGuildList[i]->GetMembers() >m_pGuildList[j]->GetMembers() && bInc) ||
					(m_pGuildList[i]->GetMembers()<m_pGuildList[j]->GetMembers() && !bInc) )
				{
					SwapData(m_pGuildList[i],m_pGuildList[j]);
				}
			}
		}
	}
	//vector <CGuildListData*>::iterator Iter1;
	//vector <CGuildListData*>::iterator Iter2;
	//for (Iter1=m_pGuildList.begin();Iter1+1!=m_pGuildList.end();Iter1++)
	//{
	//	Iter2=Iter1;
	//	do {
	//		Iter2++;
	//		if ( ((*Iter1)->GetMembers()>(*Iter2)->GetMembers() && bInc) ||
	//			((*Iter1)->GetMembers()<(*Iter2)->GetMembers() && !bInc) )
	//		{
	//			SwapIter(Iter1,Iter2);
	//		}
	//	} while(Iter2!=m_pGuildList.end());
	//}
}

void CGuildListMgr::SortGuildsByExperience(bool bInc)
{
	for (int i=0;i<int(m_pGuildList.size())-1;i++)
	{
		for (int j=i+1;j<int(m_pGuildList.size());j++)
		{
			if (m_eType==CGuildData::navy)
			{
				if ( (m_pGuildList[i]->GetExperence() >m_pGuildList[j]->GetExperence() && bInc) ||
					(m_pGuildList[i]->GetExperence()<m_pGuildList[j]->GetExperence() && !bInc) )
				{
					SwapData(m_pGuildList[i],m_pGuildList[j]);
				}
			}
		}
	}
	//vector <CGuildListData*>::iterator Iter1;
	//vector <CGuildListData*>::iterator Iter2;
	//for (Iter1=m_pGuildList.begin();Iter1+1!=m_pGuildList.end();Iter1++)
	//{
	//	Iter2=Iter1;
	//	do {
	//		Iter2++;
	//		if ( ((*Iter1)->GetExperence()>(*Iter2)->GetExperence() && bInc) ||
	//			((*Iter1)->GetExperence()<(*Iter2)->GetExperence() && !bInc) )
	//		{
	//			SwapIter(Iter1,Iter2);
	//		}
	//	} while(Iter2!=m_pGuildList.end());
	//}
}

void CGuildListMgr::SetType(CGuildData::eType type)
{
	m_eType=type;
}

CGuildData::eType CGuildListMgr::GetType()
{
	return m_eType;
}
