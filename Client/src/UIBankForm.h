//--------------------------------------------------------------
// ����:�û��������й�����
// ���˼��:�����������
//--------------------------------------------------------------

#ifndef UI_BANK_FORM_H
#define UI_BANK_FORM_H

#include "UIGlobalVar.h"
#include "NetProtocol.h"

namespace GUI
{
	class CForm;
	class CGoodsGrid;
	class CLabel;

	struct stNumBox;
     
	class CBankMgr : public CUIInterface
	{
	public:
		void		ShowBank(); // ��ʾ��������Ʒ
		CGoodsGrid* GetBankGoodsGrid() { return grdBank; } // ��ȡ����������

		bool		PushToBank(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem); // ����Ʒ��������
		bool		PopFromBank(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem); //���������ϳ���Ʒ
		bool		BankToBank(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem);  //����Ʒ��һ�������Ƶ���һ������

	protected:
		virtual bool Init(); // �û�����������Ϣ��ʼ��
		virtual void CloseForm(); //�رձ� 

	private:
		static void _MoveItemsEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);  // �ƶ������Ʒ
		static void _MoveAItemEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);  //�ƶ�������Ʒ
		static void _evtBankToBank(CGuiData *pSender,int nFirst, int nSecond, bool& isSwap);  //�����û����б��е��߻���
		static void _evtOnClose( CForm* pForm, bool& IsClose );//�ر����б�

	private:
		stNumBox*		m_pkNumberBox; // ѯ�ʸ���
		stNetBank		m_kNetBank;

		// ����
		CForm*			frmBank; // ���б�
		CGoodsGrid*		grdBank; // ���ӱ�
		CLabel*			labCharName;

	}; // end of class CBankMgr

} // end of namespace GUI

#endif // end of UI_BANK_FORM_H
