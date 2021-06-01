#pragma once
#include "UIGlobalVar.h"
#include "NetProtocol.h"
#include "UIMissionForm.h"

namespace GUI
{
#define MISLOG_REFRESH_TIME				2000	// ������־ˢ��ʱ��

class CTreeNodeObj;
class CMisLogForm : public CUIInterface
{
public:
	CMisLogForm();
	~CMisLogForm();

	void MisLogList( const NET_MISLOG_LIST& List );
	void MissionLog( WORD wMisID, const NET_MISPAGE& page );
	void MisLogState( WORD wMisID, BYTE byState );
	void MisClear( WORD wMisID );
	void MisAddLog( WORD wMisID, BYTE byState );
	void MisRefresh();

protected:
	bool Init();
    void End();

	void GetMisData( WORD wMisID, BYTE& byType, char szBuf[], USHORT sBufLen );
	BOOL AddNode( WORD wMisID, BYTE byState, BYTE& byType );
	void ClearAllNode();

private:
	// ���ڴ�����Ϣ����
	static void _MouseEvent( CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey );
	static void _MouseDown( CGuiData *pSender, int x, int y, DWORD key );
	static void	_Show( CGuiData *pSender );
	// ȷ���Ƿ��ж�����
	static void _evtBreakYesNoEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

	CForm*		m_pForm;

	// ������־�ؼ�
	CTreeView*	m_pMisTree;
	CMemoEx*	m_pMisInfo;
	
	// �������ͽڵ�
	CTreeNodeObj*	m_pNormal;
	CTreeNodeObj*	m_pHistory;
	CTreeNodeObj*	m_pGuild;
	CTreeNodeObj*	m_pInvalid;

	// ������־��Ϣ
	NET_MISLOG_LIST m_LogList;

	// ��ǰ����˵��
	WORD  m_wMisID;
	DWORD m_dwUpdateTick;
};

}

