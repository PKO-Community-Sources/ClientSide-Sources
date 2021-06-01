

#pragma once

#include "uiglobalvar.h"
#include "uiform.h"
#include "uiformmgr.h"
#include "uilabel.h"
#include "uitextbutton.h"
#include "uifastcommand.h"
#include <string>


namespace GUI
{

	class CFindTeamMgr : public CUIInterface
	{
	public:
		CFindTeamMgr();
		~CFindTeamMgr();

		void ShowFindTeamForm(bool bShow = true);
		bool IsShowFom(){return frmFindTeam->GetIsShow();}	//	Add by alfred.shi 20080902
		void SetFindTeamPage(int nCurPage, int nPageNum);
		void AddFindTeamInfo(int nSeq, const char* szName, long nLevel, long nJob, const char* szPlace);
		void RemoveTeamInfo();
		void SetOwnFindTeamState(bool bState);
		void FindTeamAsk(const char* szName);

		static const int FINDTEAM_PAGE_SIZE = 10;	// Ѱ�����һҳ��ʾ����
		static const int FINDTEAM_INTERVAL  = 60;	// ����������ʱ�䣺60��

	protected:
		virtual bool Init();
		virtual void CloseForm();

	private:

		//
		// Ѱ�����
		//
		CForm*		frmFindTeam;

		CLabelEx*		labName[FINDTEAM_PAGE_SIZE];	// ��ɫ����
		CLabelEx*		labLevel[FINDTEAM_PAGE_SIZE];	// ��ɫ�ȼ�
		CLabelEx*		labJob[FINDTEAM_PAGE_SIZE];		// ��ɫְҵ
		CLabelEx*		labPlace[FINDTEAM_PAGE_SIZE];	// ��ɫ���ڵ�
		CTextButton*	btnSubmit[FINDTEAM_PAGE_SIZE];	// �ύ��ť

		CTextButton*	btnAddme;
		CTextButton*	btnDelme;
		CLabelEx*		labListPage;		// ��ʾҳ��

		int				m_nCurPage;			// ��ǰҳ
		int				m_nPageNum;			// ��ҳ��
		DWORD			m_dwLastTick;		// ��һ��������ӵ�ʱ��
		std::string		m_strTeamLeader;	// ����Ķӳ�

		static void _evtFindTeamMouseButton(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);// �������ť�¼�
		static void	_evtFindTeamCheckEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey); // �������

	};

}

