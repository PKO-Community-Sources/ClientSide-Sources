
#pragma once


#include "UIFormMgr.h"
#include "UIForm.h"
#include "UIEdit.h"
#include "UILabel.h"
#include "UITextButton.h"

#include "UIGlobalVar.h"
#include "NetProtocol.h"

#include "LoginScene.h"
#include "SelectChaScene.h"


namespace GUI
{
	class CForm;
	class CEdit;
	class CTextButton;

	// �����������
	class CDoublePwdMgr : public CUIInterface	
	{
	public:

		CDoublePwdMgr();

		// ��ʾ������
		void ShowCreateForm();

		// ��ʾ�޸ı�
		void ShowAlterForm();

		// ��ʾ�����
		void ShowDoublePwdForm();

		// �ر�ȫ����
		void CloseAllForm();

		// ��ñ��Ƿ�����ʾ
		bool GetIsShowCreateForm()    { return frmDoublePwdCreate->GetIsShow(); }
		bool GetIsShowAlterForm()     { return frmDoublePwdAlter->GetIsShow();  }
		bool GetIsShowDoublePwdForm() { return frmDoublePwd->GetIsShow();       }

		// ����������������ִ������
		void SetType(int nType) { m_nType = nType; }
		int  GetType()          { return  m_nType; }

		static const int MC_REQUEST	= -1;
		static const int DELETE_CHARACTOR	= 1;	// ɾ����ǰ��ɫ
		static const int PACKAGE_UNLOCK		= 2;	// ��������
		static const int STORE_OPEN_ASK		= 3;	// �̳Ǵ�
		static const int ITEM_UNLOCK		= 4;	// ���߽���  ning.yan 2008-11-10
		static const int SHOW_EXCHANGEFORM	= 5;

	protected:

		virtual bool Init();
		virtual void CloseForm();

		// ��ʾ�������������
		void ShowDoublePwdKeyboardForm();

		// ���þ۽�������༭��
		void SetFocusEditBox(CEdit* edt) { edtFocusEditBox = edt; }

		// ����ɾ����ɫ��Ϣ
		void SendDeleteCharactor();

		// ���ͱ���������Ϣ
		void SendPackageUnlock();

		// ���͵��߽�����Ϣ
		void SendItemUnlock(); //add by ning.yan 2008-11-11

		// �����̳Ǵ���Ϣ
		void SendPackageStoreOpen();

		void SendGameRequest();

		// ������̰�ť�����������
		void RandomInputButton();

	private:

		// �������������
		CForm*      frmDoublePwdCreate;
		CEdit*		edtDoublePwdCreate;
		CEdit*      edtDoublePwdCreateRetry;

		// �޸Ķ��������
		CForm*      frmDoublePwdAlter;
		CEdit*		edtDoublePwdAlterOld;
		CEdit*		edtDoublePwdAlterNew;
		CEdit*		edtDoublePwdAlterNewRetry;

		// ������������
		CForm*      frmDoublePwd;
		CEdit*      edtDoublePwd;

		// ����������������̱�
		CForm*      frmDoublePwdInput;

		// ������������ʱ����ʾ
		CForm*		frmDoublePwdInfo;

		// ��������༭��
		CEdit*		edtFocusEditBox;

		// ����
		int         m_nType;

	private:

		// ���������������ť�¼�
		static void _evtCreateFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

		// �޸Ķ����������ť�¼�
		static void _evtAlterFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

		// ��������������ť�¼�
		static void _evtDoublePwdFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

		// ����������������̱���ť�¼�
		static void _evtInputFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

		// �༭�򼤻��¼��������¼���ı༭��
		static void _evtEditFocus(CGuiData* pSender);

		// �رմ����¼�
		static void _evtFormClose(CForm* pForm, bool& IsClose );

		// ���������Ƿ�Ϸ�
		static bool IsPwdValid(const char* szStr);

	}; // end of class CDoublePwdMgr

} // end of namespace GUI


