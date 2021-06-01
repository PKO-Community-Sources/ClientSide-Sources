
#include "stdafx.h"
#include "UIFindTeamForm.h"
#include "UIBoxForm.h"
#include "PacketCmd.h"
#include "GameApp.h"
#include "MapSet.h"
#include "UIMiniMapForm.h"

#include <jobtype.h>	// common ��
extern const char* g_szJobName[MAX_JOB_TYPE];


namespace GUI
{

	CFindTeamMgr::CFindTeamMgr()
	{
		m_dwLastTick = 0;
	}

	CFindTeamMgr::~CFindTeamMgr()
	{
	}


	bool CFindTeamMgr::Init()
	{
		//
		// Ѱ�����
		//
		frmFindTeam = CFormMgr::s_Mgr.Find("frmFindTeam");
		if(! frmFindTeam)
		{
			LG("gui", "frmFindTeam not found.");
			return false;
		}
		frmFindTeam->evtEntrustMouseEvent = _evtFindTeamMouseButton;

		labListPage = dynamic_cast<CLabelEx*>(frmFindTeam->Find("labListPage"));
		if(! labListPage)
		{
			LG("gui", "frmFindTeam:labListPage not found.");
			return false;
		}

		btnAddme = dynamic_cast<CTextButton*>(frmFindTeam->Find("btnAddme"));
		if(! btnAddme)
		{
			LG("gui", "frmFindTeam:btnAddme not found.");
			return false;
		}

		btnDelme = dynamic_cast<CTextButton*>(frmFindTeam->Find("btnDelme"));
		if(! btnDelme)
		{
			LG("gui", "frmFindTeam:btnDelme not found.");
			return false;
		}

		char szName[32] = {0};
		for(int i = 0; i < FINDTEAM_PAGE_SIZE; ++i)
		{
			sprintf(szName, "labName_%d", i);
			labName[i] = dynamic_cast<CLabelEx*>(frmFindTeam->Find(szName));
			if(! labName[i])
			{
				LG("gui", "frmFindTeam:%s not found.", szName);
				return false;
			}

			sprintf(szName, "labLevel_%d", i);
			labLevel[i] = dynamic_cast<CLabelEx*>(frmFindTeam->Find(szName));
			if(! labLevel[i])
			{
				LG("gui", "frmFindTeam:%s not found.", szName);
				return false;
			}

			sprintf(szName, "labJob_%d", i);
			labJob[i] = dynamic_cast<CLabelEx*>(frmFindTeam->Find(szName));
			if(! labJob[i])
			{
				LG("gui", "frmFindTeam:%s not found.", szName);
				return false;
			}

			sprintf(szName, "labPlace_%d", i);
			labPlace[i] = dynamic_cast<CLabelEx*>(frmFindTeam->Find(szName));
			if(! labPlace[i])
			{
				LG("gui", "frmFindTeam:%s not found.", szName);
				return false;
			}

			sprintf(szName, "btnSubmit_%d", i);
			btnSubmit[i] = dynamic_cast<CTextButton*>(frmFindTeam->Find(szName));
			if(! btnSubmit[i])
			{
				LG("gui", "frmFindTeam:%s not found.", szName);
				return false;
			}
		}

		return true;
	}


	void CFindTeamMgr::CloseForm()
	{
		frmFindTeam->SetIsShow(false);
	}


	void CFindTeamMgr::ShowFindTeamForm(bool bShow)
	{
		if(g_stUIMap.IsGuildWar())
		{
			frmFindTeam->SetIsShow(false);
			return;
		}

		frmFindTeam->SetIsShow(bShow);
	}


	void CFindTeamMgr::SetFindTeamPage(int nCurPage, int nPageNum)
	{
		m_nCurPage = nCurPage;
		m_nPageNum = nPageNum;

		char szPage[32] = {0};
		sprintf(szPage, "%d/%d", nCurPage, nPageNum);
		labListPage->SetCaption(szPage);
	}


	void CFindTeamMgr::AddFindTeamInfo(int nSeq, const char* szName, long nLevel, long nJob, const char* szPlace)
	{
		if(0 <= nSeq && nSeq < FINDTEAM_PAGE_SIZE)
		{
			labName[nSeq]->SetCaption(szName);
			labName[nSeq]->SetIsShow(true);

			char szLevel[32] = {0};
			sprintf(szLevel, "%d", nLevel);
			labLevel[nSeq]->SetCaption(szLevel);
			labLevel[nSeq]->SetIsShow(true);

			if(0 <= nJob && nJob < MAX_JOB_TYPE)
			{
				labJob[nSeq]->SetCaption(g_szJobName[nJob]);
				labJob[nSeq]->SetIsShow(true);
			}

			CMapInfo* pMapInfo = GetMapInfo(szPlace);
			if(pMapInfo)
			{
				labPlace[nSeq]->SetCaption(pMapInfo->szName);
				labPlace[nSeq]->SetIsShow(true);
			}
			else
			{
				labPlace[nSeq]->SetCaption(szPlace);
				labPlace[nSeq]->SetIsShow(true);
			}

			btnSubmit[nSeq]->SetIsShow(true);
		}
	}

	void CFindTeamMgr::RemoveTeamInfo()
	{
		for(int i = 0; i < FINDTEAM_PAGE_SIZE; ++i)
		{
			labName[i]->SetIsShow(false);
			labLevel[i]->SetIsShow(false);
			labJob[i]->SetIsShow(false);
			labPlace[i]->SetIsShow(false);
			btnSubmit[i]->SetIsShow(false);
		}
	}

	void CFindTeamMgr::SetOwnFindTeamState(bool bState)
	{
		if(bState)
		{
			btnAddme->SetIsEnabled(false);
			btnDelme->SetIsEnabled(true);
		}
		else
		{
			btnAddme->SetIsEnabled(true);
			btnDelme->SetIsEnabled(false);
		}
	}


	void CFindTeamMgr::FindTeamAsk(const char* szName)
	{
		m_strTeamLeader = szName;

		char szBuffer[256] = {0};
		sprintf(szBuffer, g_oLangRec.GetString(863), szName);
		CBoxMgr::ShowSelectBox(_evtFindTeamCheckEvent, szBuffer, true);
	}


	// Ѱ����ӽ��水ť��Ϣ����
	void CFindTeamMgr::_evtFindTeamMouseButton(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		string strName = pSender->GetName();

		if(strName == "btnLeftPage")
		{
			if(g_stUIFindTeam.m_nCurPage > 1)
			{
				CS_VolunteerList(g_stUIFindTeam.m_nCurPage - 1, FINDTEAM_PAGE_SIZE);
			}
		}
		else if(strName == "btnRightPage")
		{
			if(g_stUIFindTeam.m_nCurPage < g_stUIFindTeam.m_nPageNum)
			{
				CS_VolunteerList(g_stUIFindTeam.m_nCurPage + 1, FINDTEAM_PAGE_SIZE);
			}
		}
		else if(strName == "btnAddme")
		{
			// ��ӵ�Ѱ������б���
			CS_VolunteerAdd();
		}
		else if(strName == "btnDelme")
		{
			// ��Ѱ������б�������
			CS_VolunteerDel();
		}
		else if(strName.substr(0, 10) == "btnSubmit_")
		{
			DWORD dwCurTick = g_pGameApp->GetCurTick();
			if(g_stUIFindTeam.m_dwLastTick + 1000 * FINDTEAM_INTERVAL > dwCurTick)
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(883), FINDTEAM_INTERVAL, FINDTEAM_INTERVAL - (dwCurTick - g_stUIFindTeam.m_dwLastTick) / 1000);
				return;
			}
			g_stUIFindTeam.m_dwLastTick = dwCurTick;

			int nSeq = strName[strName.size() - 1] - '0';
			if(0 <= nSeq && nSeq < FINDTEAM_PAGE_SIZE)
			{
				// �����������
				if(g_stUIFindTeam.labName[nSeq]->GetIsShow())
				{
					CS_VolunteerSel(g_stUIFindTeam.labName[nSeq]->GetCaption());
				}
			}
		}
	}


	// �������ص�
	void CFindTeamMgr::_evtFindTeamCheckEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		CS_VolunteerAsr(nMsgType == CForm::mrYes, g_stUIFindTeam.m_strTeamLeader.c_str());
	}

}
