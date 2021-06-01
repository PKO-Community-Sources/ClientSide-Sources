#pragma once


#include "uiglobalvar.h"
#include "uiform.h"
#include "uiformmgr.h"
#include "uilabel.h"
#include "uitextbutton.h"
#include "uicheckbox.h"
#include "uiimage.h"
#include "uicommand.h"
#include "uifastcommand.h"


namespace GUI
{

	class CSpiritMgr : public CUIInterface
	{
	public:
		CSpiritMgr(void);
		~CSpiritMgr(void);

		static const int NO_USE = -1;
		static const int SPIRIT_MARRY_TYPE = 6;			// �����ں����ͣ�������������ã�

		static const int SPIRIT_MARRY_CELL_COUNT = 3;	// ���Ӹ���
		static const int SPIRIT_MARRY_ITEM = 0;			// ��ħ��ʵ
		static const int SPIRIT_MARRY_ONE  = 1;			// ����1
		static const int SPIRIT_MARRY_TWO  = 2;			// ����2

		static const int ERNIE_IMAGE_COUNT = 5;			// �ϻ���ͼƬ��
		static const int ERNIE_SPEED       = 50;		// ��ת�ٶȣ����룩
		static const int ERNIE_COIN_COUNT  = 5;			// 5 ������Ӳ��
		static const int ERNIE_EMPTY_COUNT = 5;			// 5 �������ո�

		void ClearAllCommand();
		void ShowMarryForm(bool bShow = true);
		void ShowErnieForm(bool bShow = true);

		void UpdateErnieNumber(short nNum, short nID1, short nID2, short nID3);	// �����ϻ���
		void UpdateErnieString(const char* szText);		// ������ʾ����
		void ShowErnieHighLight();

		int GetType() {  return SPIRIT_MARRY_TYPE;	}	// ����

	protected:
		virtual bool Init();
		virtual void CloseForm();
		virtual void FrameMove(DWORD dwTime);


	private:

		//
		//  �����ں�
		//
		CForm*		    frmSpiritMarry;
		CLabel*			labMoneyShow;
		CTextButton*	btnForgeYes;
		COneCommand*    cmdSpiritMarry[SPIRIT_MARRY_CELL_COUNT];

		int             m_iSpiritItemPos[SPIRIT_MARRY_CELL_COUNT];

		void		PushItem(int iIndex, CItemCommand& rItem);
		void		PopItem(int iIndex);
		void		SetSpiritUI();

		bool		IsValidSpiritItem(CItemCommand& rItem);	// �Ƿ��ǺϷ��Ķ�ħ��ʵ
		bool		IsValidSpirit(CItemCommand& rItem);		// �Ƿ��ǺϷ��ľ��飨LV > 20��

		void		SendSpiritMarryProtocol();

		static void _evtDragMarryItem(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);	// �����ħ��ʵ
		static void _evtDragMarryOne(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);	// ���뾫��1
		static void _evtDragMarryTwo(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);	// ���뾫��2

		static void _evtMainMouseButton(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);// �������ť�¼�
		static void _evtCloseMarryForm(CForm* pForm, bool& IsClose);	// �رմ����¼�

	private:

		//
		//  �����ϻ���
		//
		CForm*			frmSpiritErnie;
		CImage*			imgLine[9][ERNIE_IMAGE_COUNT];
		CCheckBox*		chkSetmoney[3];
		CTextButton*	btnStop[3];
		COneCommand*	cmdItem[9];
		CTextButton*	btnStart;

		CLabelEx*		labUsemoney[3];
		CLabelEx*		labLastshow1;
		CLabelEx*		labLastshow2;

		bool			m_bIsRunning[3];

		DWORD			m_dwLastTickCount;
		int				m_nCurrImage;

		void			AddTigerItem(short nNum, short sItemID);
		void			ClearTigerItem(void);
		void			ErnieHightLight(int nNum, bool b = true);

		static void _evtErnieMouseButton(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);// �������ť�¼�
		static void _evtCloseErnieForm(CForm* pForm, bool& IsClose);	// �رմ����¼�

	};

}

