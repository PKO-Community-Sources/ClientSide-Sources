#include "StdAfx.h"
#include "UIMisLogForm.h"
#include "uiform.h"
#include "UITreeView.h"
#include "UIMemo.h"
#include "PacketCmd.h"
#include "UIItem.h"
#include "Script.h"
#include "lua_platform.h"
#include "caLua.h"
#include "uiboxform.h"
#include "uiboatform.h"
#include "MapSet.h"
#include "FindPath.h"
#include "UIStartForm.h"
#include "UIList.h"
using namespace GUI;

extern lua_State*	g_pLuaState;

//---------------------------------------------------------------------------
// class CMisLogForm
//---------------------------------------------------------------------------
CMisLogForm::CMisLogForm()
{
	m_pMisTree = NULL;
	m_pMisInfo = NULL;
	m_pNormal = NULL;
	m_pHistory = NULL;
	m_pGuild = NULL;
	m_pInvalid = NULL;
	m_wMisID = 0xFFFF;
	m_dwUpdateTick = 0;
}

CMisLogForm::~CMisLogForm()
{
}

bool CMisLogForm::Init()
{
	m_pForm = _FindForm( "frmMission" );
	if( !m_pForm )
	{
		LG( "gui", g_oLangRec.GetString(721) );
		return false;
	}

	m_pForm->evtEntrustMouseEvent = _MouseEvent;
	m_pForm->evtShow = _Show;
	m_pMisTree = dynamic_cast<CTreeView*>(m_pForm->Find( "trvMission" ));

	if( !m_pMisTree )
	{
		return Error( g_oLangRec.GetString(473), m_pForm->GetName(), "trvMission" );
	}

	m_pMisTree->evtMouseDown = _MouseDown;
	m_pMisTree->SetSelectColor( 0xFF00FFFF );
	m_pMisTree->SetColSpace( 1 );

	m_pMisInfo = dynamic_cast<CMemoEx*>(m_pForm->Find( "memMiss" ));
	if( !m_pMisInfo )
	{
		return Error( g_oLangRec.GetString(473), m_pForm->GetName(), "memMiss" );
	}
	m_pMisInfo->SetRowNum(64);
	m_pMisInfo->Refresh();
	CTextButton* pButton = dynamic_cast<CTextButton*>(m_pForm->Find( "btnBreak" ));
	if( pButton )
	{
		pButton->SetIsEnabled( false );
	}

	return true;
}

void CMisLogForm::End()
{

}

void CMisLogForm::_Show( CGuiData *pSender )
{
	if( stricmp( "frmMission", pSender->GetName() ) ==  0 )
	{
		g_stUIMisLog.MisRefresh();
	}
}

void CMisLogForm::_MouseEvent( CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey )
{
	string strName = pSender->GetName();
	if( stricmp( "frmMission", pSender->GetForm()->GetName() ) ==  0 )
	{
		// ������˳���ť,��رոñ�
		if( strName == "btnNo"  || strName == "btnClose" )
		{	
			pSender->GetForm()->Close();						
			return;
		}
		else if( strName == "btnBreak" )
		{
			// ȡ��һ������
			CTreeNodeObj* pNode = g_stUIMisLog.m_pMisTree->GetSelectNode();
			if( pNode == NULL ) return;
			
			char szData[128];
			BYTE byType = 0;
			sprintf( szData,g_oLangRec.GetString(722), g_stUIMisLog.m_wMisID );

			// ��ȡ������Ϣ
			g_stUIMisLog.GetMisData( g_stUIMisLog.m_wMisID, byType, szData, 32 );

			char szBuf[256] = { 0 };
			//if( byType==mission::MIS_TREENODE_RAND )
			//{
			//	int nMoney = 100;
			//	if( CCharacter* pMain = g_stUIBoat.GetHuman() )
			//	{
			//		nMoney += pMain->getGameAttr()->get(ATTR_LV) * pMain->getGameAttr()->get(ATTR_LV);
			//	}
			//	sprintf( szBuf, "�ж�[%s]\n���۳���Ǯ[%d$]?", szData, nMoney );
			//}
			//else
			//{
				sprintf( szBuf, g_oLangRec.GetString(723), szData );
			//}


			stSelectBox* pBox = CBoxMgr::ShowSelectBox( _evtBreakYesNoEvent, szBuf, true );
			pBox->dwTag = pNode->GetTag();
		}
	}
}

void CMisLogForm::_evtBreakYesNoEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if( nMsgType!=CForm::mrYes ) return;

	stSelectBox* pBox = (stSelectBox*)pSender->GetForm()->GetPointer();
	if( !pBox ) return;

	CS_MisClear( (WORD)pBox->dwTag );
}

void CMisLogForm::_MouseDown( CGuiData *pSender, int x, int y, DWORD key )
{
	CTreeView* pTree = dynamic_cast<CTreeView*>(pSender);
	if( !pTree ) return;

	CTreeNodeObj* pNode = pTree->GetHitNode( x, y );
	if( pNode )
	{
		DWORD dwData = pNode->GetTag();
		if( dwData == -1 )
		{
			return;
		}

		CTextButton* pButton = dynamic_cast<CTextButton*>(g_stUIMisLog.m_pForm->Find( "btnBreak" ));
		if( pButton )
		{
			pButton->SetIsEnabled( true );
		}
		else
		{
			pButton->SetIsEnabled( false );
		}
		DWORD dwTick = GetTickCount();
		if( dwTick - g_stUIMisLog.m_dwUpdateTick > MISLOG_REFRESH_TIME || dwData != g_stUIMisLog.m_wMisID )
		{
			CS_MisLogInfo( (WORD)dwData );
			g_stUIMisLog.m_dwUpdateTick = dwTick;
		}
	}
}

void CMisLogForm::MisRefresh()
{
	DWORD dwTick = GetTickCount();
	if( dwTick - m_dwUpdateTick > MISLOG_REFRESH_TIME )
	{
		if( m_wMisID != 0xFFFF )
		{
			CS_MisLogInfo( m_wMisID );
			m_dwUpdateTick = dwTick;
		}		
	}
}

BOOL CMisLogForm::AddNode( WORD wMisID, BYTE byState, BYTE& byType )
{
	USHORT sMaxNum = 28;
	char szData[128];
	byType = 0;
	sprintf( szData,g_oLangRec.GetString(722), wMisID );
	CTreeNodeObj* pNode = m_pMisTree->GetRootNode();

	// ��ȡ������Ϣ
	GetMisData( wMisID, byType, szData, 32 );
	
	// �ü�������
	USHORT sNum = 0;
	char* pszTemp = szData;
	while( pszTemp[0] )
	{
		// �ж��Ƿ�һ��GBK
		BOOL bFlag1 = 0x81 <= (BYTE)pszTemp[0] && (BYTE)pszTemp[0] <= 0xFE;
		BOOL bFlag2 = (0x40 <= (BYTE)pszTemp[1] && (BYTE)pszTemp[1] <= 0x7E) || (0x7E <= (BYTE)pszTemp[1] && (BYTE)pszTemp[1] <= 0xFE);
		if( bFlag1 && bFlag2 )
		{			
			if( sNum + 2 > 18 )
			{
				szData[sNum] = 0;
				break;
			}
			sNum += 2;
			pszTemp += 2;
		}
		else
		{
			if( sNum > 18 )
			{
				szData[sNum] = 0;
				break;
			}
			sNum++;
			pszTemp++;
		}
			
	}

	string strData = "<p";
	strData +=szData;
	strData += ">";
	strData += "<r";
	int nLen = sMaxNum - (int)strlen(szData) - 6;
	for( int i = 0; i < nLen; i++ )
	{
		strData += ".";
	}
	strData += ">";

	if( byState == ROLE_MIS_PENDING_FLAG )
	{
		strData += g_oLangRec.GetString(724);
	}
	else if( byState == ROLE_MIS_COMPLETE_FLAG )
	{
		strData += g_oLangRec.GetString(725);
	}
	else if( byState == ROLE_MIS_FAILURE_FALG )
	{
		strData += g_oLangRec.GetString(726);
	}
	else
	{
		strData += g_oLangRec.GetString(727);
	}

	// ����������Ϣ������ӵķ���ڵ���
	CColorItem* pItem = new CColorItem;
	pItem->SetString( strData.c_str() );
	CTreeNodeObj* pTreeNode = new CTreeNode( m_pMisTree, pItem );
	DWORD dwData = wMisID;
	pTreeNode->SetTag( dwData );

	// ������ʾ�ڵ�����
	if( byType == mission::MIS_TREENODE_NORMAL )
	{
		if( m_pNormal == NULL )
		{
			CItem* pTemp = new CItem( g_oLangRec.GetString(728), COLOR_BLACK );
			m_pNormal = new CTreeNode( m_pMisTree, pTemp );
			m_pNormal->SetTag( -1 );
			pNode->AddNode( m_pNormal );
		}
		m_pNormal->AddNode( pTreeNode );
	}
	else if( byType == mission::MIS_TREENODE_HISTORY )
	{
		if( m_pHistory == NULL )
		{
			CItem* pTemp = new CItem( g_oLangRec.GetString(729), COLOR_BLACK );
			m_pHistory = new CTreeNode( m_pMisTree, pTemp );
			m_pHistory->SetTag( -1 );
			pNode->AddNode( m_pHistory );
		}
		m_pHistory->AddNode( pTreeNode );
	}
	else if( byType == mission::MIS_TREENODE_GUILD )
	{
		if( m_pGuild == NULL )
		{
			CItem* pTemp = new CItem( g_oLangRec.GetString(730), COLOR_BLACK );
			m_pGuild = new CTreeNode( m_pMisTree, pTemp );
			m_pGuild->SetTag( -1 );
			pNode->AddNode( m_pGuild );
		}
		m_pGuild->AddNode( pTreeNode );
	}
	else
	{
		if( m_pInvalid == NULL )
		{
			CItem* pTemp = new CItem( g_oLangRec.GetString(731), COLOR_BLACK );
			m_pInvalid = new CTreeNode( m_pMisTree, pTemp );
			m_pInvalid->SetTag( -1 );
			pNode->AddNode( m_pInvalid );
		}

		m_pInvalid->AddNode( pTreeNode );
	}

	m_pMisTree->Refresh();

	return TRUE;
}

void CMisLogForm::ClearAllNode()
{
	m_pMisTree->ClearAllNode();
	m_pNormal = NULL;
	m_pHistory = NULL;
	m_pGuild = NULL;
	m_pInvalid = NULL;
	m_pMisInfo->Init();
}

void CMisLogForm::MisLogList( const NET_MISLOG_LIST& List )
{
	ClearAllNode();

	memcpy( &m_LogList, &List, sizeof(NET_MISLOG_LIST) );

	for( BYTE i = 0; i < m_LogList.byNumLog; i++ )
	{
		AddNode( m_LogList.MisLog[i].wMisID, m_LogList.MisLog[i].byState, m_LogList.MisLog[i].byType );
	}

	if( List.byNumLog > 0 )
	{
		CS_MisLogInfo( List.MisLog[0].wMisID );
	}
}

void CMisLogForm::MissionLog( WORD wMisID, const NET_MISPAGE& page )
{
	if( m_pMisInfo == NULL )
	{
		return;
	}
	
	m_pMisInfo->Init();
	m_pMisInfo->SetMisPage( page );
	m_pMisInfo->SetIsShow(true);
	m_wMisID = wMisID;
}

void CMisLogForm::GetMisData( WORD wMisID, BYTE& byType, char szBuf[], USHORT sBufLen )
{
	typedef struct RetType
	{
		BYTE byRet;
		BYTE byType;
		char* pszName;
	} RetType;

	RetType* pRet = (RetType*)CLU_CallScriptFunction( "GetMisData", "uchar,uchar,char*", "ushort", wMisID );
	if( pRet == NULL )
	{
		return;
	}
	
	if( pRet->byRet != LUA_TRUE )
	{
		return;
	}
	
	byType = pRet->byType;
	if( pRet->pszName )
	{
		strncpy( szBuf, pRet->pszName, sBufLen - 1 );
	}

	CLU_DllFree( pRet );
}

void CMisLogForm::MisClear( WORD wMisID )
{
	int nIndex = -1;
	for( int i = 0; i < m_LogList.byNumLog; i++ )
	{
		if( m_LogList.MisLog[i].wMisID == wMisID )
		{		
			nIndex = i;
			break;
		}
	}

	if( nIndex == -1 )
	{
		g_pGameApp->MsgBox( g_oLangRec.GetString(732), wMisID );
		return;
	}

	CTreeNodeObj* pNode = NULL;
	if( m_LogList.MisLog[nIndex].byType == mission::MIS_TREENODE_NORMAL )
	{
		pNode = m_pNormal;
	}
	else if( m_LogList.MisLog[nIndex].byType == mission::MIS_TREENODE_HISTORY )
	{
		pNode = m_pHistory;
	}
	else if( m_LogList.MisLog[nIndex].byType == mission::MIS_TREENODE_GUILD )
	{
		pNode = m_pGuild;
	}
	else
	{
		pNode = m_pInvalid;
	}

	if( pNode )
	{
		USHORT sNum = pNode->GetChildCount();
		for( USHORT n = 0; n < sNum; n++ )
		{
			CTreeNodeObj* pChild = pNode->GetChildNode( n );
			if( pChild == NULL )
			{
				g_pGameApp->MsgBox( g_oLangRec.GetString(733), wMisID, n );
				break;
			}
			DWORD dwData = pChild->GetTag();
			if( dwData == wMisID )
			{
				m_pMisTree->DelNode( pChild );
				m_pMisTree->Refresh();
				break;
			}
		}
		if( pNode->GetChildCount() <= 0 )
		{
			m_pMisTree->DelNode( pNode );
			m_pMisTree->Refresh();
			if( pNode == m_pInvalid )
			{
				m_pInvalid = NULL;
			}
			else if( pNode == m_pNormal )
			{
				m_pNormal = NULL;
			}
			else if( pNode == m_pHistory )
			{
				m_pHistory = NULL;
			}
			else if( pNode == m_pGuild )
			{
				m_pGuild = NULL;
			}
		}
	}

	NET_MISLOG Log[ROLE_MAXNUM_MISSION];
	memset( Log, 0, sizeof(NET_MISLOG)*ROLE_MAXNUM_MISSION );
	memcpy( Log, m_LogList.MisLog, sizeof(NET_MISLOG)*m_LogList.byNumLog );
	memset( m_LogList.MisLog + nIndex, 0, sizeof(NET_MISLOG)*(m_LogList.byNumLog - nIndex) );
	memcpy( m_LogList.MisLog + nIndex, Log + nIndex + 1, sizeof(NET_MISLOG)*(m_LogList.byNumLog - nIndex - 1) );
	m_LogList.byNumLog--;

	if( m_wMisID == wMisID )
	{
		m_wMisID = 0xFFFF;
		m_pMisInfo->Init();
		if( m_LogList.byNumLog > 0 )
		{
			CS_MisLogInfo( m_LogList.MisLog[0].wMisID );
		}
	}
}

void CMisLogForm::MisAddLog( WORD wMisID, BYTE byState )
{
	m_LogList.MisLog[m_LogList.byNumLog].byState = byState;
	m_LogList.MisLog[m_LogList.byNumLog].wMisID  = wMisID;
	
	AddNode( wMisID, byState, m_LogList.MisLog[m_LogList.byNumLog].byType );

	m_LogList.byNumLog++;

	if( m_wMisID == 0xFFFF )
	{
		CS_MisLogInfo( m_LogList.MisLog[0].wMisID );
	}
}

void CMisLogForm::MisLogState( WORD wMisID, BYTE byState )
{
	int i = 0;
	for( ; i < m_LogList.byNumLog; i++ )
	{
		if( m_LogList.MisLog[i].wMisID == wMisID )
		{
			if( m_LogList.MisLog[i].byState == byState )
				return;
			
			// ��������״̬
			m_LogList.MisLog[i].byState = byState;
			break;
		}
	}

	CTreeNodeObj* pNode = NULL;
	if( m_LogList.MisLog[i].byType == mission::MIS_TREENODE_NORMAL )
	{
		pNode = m_pNormal;
	}
	else if( m_LogList.MisLog[i].byType == mission::MIS_TREENODE_HISTORY )
	{
		pNode = m_pHistory;
	}
	else if( m_LogList.MisLog[i].byType == mission::MIS_TREENODE_GUILD )
	{
		pNode = m_pGuild;
	}
	else
	{
		pNode = m_pInvalid;
	}

	CTreeNode* pFind = NULL;
	if( pNode )
	{
		USHORT sNum = pNode->GetChildCount();
		for( USHORT n = 0; n < sNum; n++ )
		{
			CTreeNodeObj* pChild = pNode->GetChildNode( n );
			if( pChild == NULL )
			{
				g_pGameApp->MsgBox( g_oLangRec.GetString(734), wMisID, n );
				break;
			}
			DWORD dwData = pChild->GetTag();
			if( dwData == wMisID )
			{
				pFind = dynamic_cast<CTreeNode*>(pChild);
				break;
			}
		}
	}
	
	if( pFind == NULL ) 
	{
		g_pGameApp->MsgBox( g_oLangRec.GetString(735), wMisID );
		return;
	}

	CColorItem* pItem = dynamic_cast<CColorItem*>(pFind->GetItem());

	// ��ȡ������Ϣ
	char szData[128];
	BYTE byType = 0;
	sprintf( szData,g_oLangRec.GetString(722), wMisID );
	GetMisData( wMisID, byType, szData, 32 );

	string strData = "<p";
	strData += szData;
	strData += ">";
	strData += "<r";
	int nLen = 24 - (int)strlen(szData) - 6;
	for( int i = 0; i < nLen; i++ )
	{
		strData += ".";
	}
	strData += ">";

	if( byState == ROLE_MIS_PENDING_FLAG )
	{
		strData += g_oLangRec.GetString(724);
	}
	else if( byState == ROLE_MIS_COMPLETE_FLAG )
	{
		strData += g_oLangRec.GetString(725);
	}
	else if( byState == ROLE_MIS_FAILURE_FALG )
	{
		strData += g_oLangRec.GetString(726);
	}
	else
	{
		strData += g_oLangRec.GetString(727);
	}

	if( byState == ROLE_MIS_PENDING_FLAG )
	{
		strData += g_oLangRec.GetString(736);
	}
	else if( byState == ROLE_MIS_COMPLETE_FLAG )
	{
		strData += g_oLangRec.GetString(737);
	}
	else if( byState == ROLE_MIS_FAILURE_FALG )
	{
		strData += g_oLangRec.GetString(738);
	}
	else
	{
		strData += g_oLangRec.GetString(739);
	}

	pItem->SetString( strData.c_str() );
}
