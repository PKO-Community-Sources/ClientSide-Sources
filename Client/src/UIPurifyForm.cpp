
#include "stdafx.h"
#include "UIPurifyForm.h"
#include "GameApp.h"
#include "UIEquipForm.h"
#include "uigoodsgrid.h"
#include "packetcmd.h"
#include "character.h"
#include "UIBoxform.h"
#include "packetCmd.h"
#include "uiitemcommand.h"


namespace GUI
{

	CPurifyMgr::CPurifyMgr(void)
	{
	}

	CPurifyMgr::~CPurifyMgr(void)
	{
	}


	bool CPurifyMgr::Init()
	{
		frmEquipPurify = CFormMgr::s_Mgr.Find("frmEquipPurify");
		if(! frmEquipPurify)
		{
			LG("gui", "main.clu   frmEquipPurify not found.\n");
			return false;
		}

		labMoneyShow = dynamic_cast<CLabelEx*>(frmEquipPurify->Find("labMoneyShow"));
		if(! labMoneyShow)
		{
			LG("gui", "main.clu   frmEquipPurify:labMoneyShow not found.\n");
			return false;
		}

		btnForgeYes = dynamic_cast<CTextButton*>(frmEquipPurify->Find("btnForgeYes"));
		if(! btnForgeYes)
		{
			LG("gui", "main.clu   frmEquipPurify:btnForgeYes not found.\n");
			return false;
		}

		// Command������ʾ
		char szName[32] = {0};
		for(int i = 0; i < TYPE_COUNT; ++i)
		{
			sprintf(szName, "labHintleft%d", i + 1);
			labHintleft[i] = dynamic_cast<CLabelEx*>(frmEquipPurify->Find(szName));
			if(! labHintleft[i])
			{
				LG("gui", "main.clu   frmEquipPurify:%s not found.\n", szName);
				return false;
			}

			sprintf(szName, "labHintright%d", i + 1);
			labHintright[i] = dynamic_cast<CLabelEx*>(frmEquipPurify->Find(szName));
			if(! labHintright[i])
			{
				LG("gui", "main.clu   frmEquipPurify:%s not found.\n", szName);
				return false;
			}

			sprintf(szName, "labTitle%d", i + 1);
			labTitle[i] = dynamic_cast<CLabelEx*>(frmEquipPurify->Find(szName));
			if(! labTitle[i])
			{
				LG("gui", "main.clu   frmEquipPurify:%s not found.\n", szName);
				return false;
			}
		}

		cmdEquipPurify[PURIFY_ONE] = dynamic_cast<COneCommand*>(frmEquipPurify->Find("cmdEquipOne"));
		if(! cmdEquipPurify[PURIFY_ONE])
		{
			LG("gui", "main.clu   frmEquipPurify:cmdEquipOne not found.\n");
			return false;
		}

		cmdEquipPurify[PURIFY_TWO] = dynamic_cast<COneCommand*>(frmEquipPurify->Find("cmdEquipTwo"));
		if(! cmdEquipPurify[PURIFY_TWO])
		{
			LG("gui", "main.clu   frmEquipPurify:cmdEquipTwo not found.\n");
			return false;
		}

		frmEquipPurify->evtClose             = _evtClosePurifyForm;
		frmEquipPurify->evtEntrustMouseEvent = _evtMainMouseButton;

		cmdEquipPurify[PURIFY_ONE]->evtBeforeAccept = _evtDragPurifyOne;
		cmdEquipPurify[PURIFY_TWO]->evtBeforeAccept = _evtDragPurifyTwo;

		return true;
	}


	void CPurifyMgr::CloseForm()
	{
		// �����κδ���
	}


	void CPurifyMgr::ShowForm(int nType)
	{
		if(frmEquipPurify)
		{
			ClearAllCommand();

			if(PURIFY_TYPE <= nType && nType < PURIFY_TYPE + TYPE_COUNT)
			{
				m_nType = nType;  // ��������

				for(int i = 0; i < TYPE_COUNT; ++i)
				{
					labHintleft[i]->SetIsShow (i + PURIFY_TYPE == m_nType);
					labHintright[i]->SetIsShow(i + PURIFY_TYPE == m_nType);
					labTitle[i]->SetIsShow    (i + PURIFY_TYPE == m_nType);
				}

				SetPurifyUI();

				frmEquipPurify->SetPos(100, 100);
				frmEquipPurify->Refresh();
				frmEquipPurify->SetIsShow(true);

				int nLeft, nTop;
				nLeft = frmEquipPurify->GetX2();
				nTop  = frmEquipPurify->GetY();

				g_stUIEquip.GetItemForm()->SetPos(nLeft, nTop);
				g_stUIEquip.GetItemForm()->Refresh();
				g_stUIEquip.GetItemForm()->Show();
			}
			else
			{
				frmEquipPurify->SetIsShow(false);
			}
		}
	}


	void CPurifyMgr::CloseAllForm()
	{
		if(frmEquipPurify)
		{
			frmEquipPurify->SetIsShow(false);
		}
	}


	void CPurifyMgr::ClearAllCommand()
	{
		PopItem(PURIFY_ONE);
		PopItem(PURIFY_TWO);
	}


	int CPurifyMgr::GetItemComIndex(COneCommand* pCom)
	{
		for(int i = 0; i < PURIFY_CELL_COUNT; ++i)
		{
			if(cmdEquipPurify[i] == pCom)
				return i;
		}

		return NO_USE;
	}


	void CPurifyMgr::DragItemToEquipGrid(int nIndex)
	{
		if(PURIFY_ONE == nIndex)
		{
			ClearAllCommand();
		}
		else if(PURIFY_TWO == nIndex)
		{
			PopItem(PURIFY_TWO);
		}

		SetPurifyUI();
	}


	// �Ƿ��ǿ���װ������Ʒ
	bool CPurifyMgr::IsEquipItem(CItemCommand& rItem)
	{
		CItemRecord* pItemRecord = rItem.GetItemInfo();
		if(pItemRecord)
		{
			short sType = pItemRecord->sType;

			switch(sType)
			{
			case 1:		//��
			case 2:		//�޽�
			case 3:		//��
			case 4:		//��ǹ
			case 7:		//ذ��
			case 9:		//����
			case 11:	//����
			case 20:	//ñ��
			case 22:	//�·�
			case 23:	//����
			case 24:	//Ь��
			case 27:	//����
				return true;

			default:
				return false;
			}
		}

		return false;
	}


	bool CPurifyMgr::IsMainLifeItem(CItemCommand& rItem)
	{
		CItemRecord* pItemRecord = rItem.GetItemInfo();
		if(pItemRecord)
		{
			long nType = pItemRecord->lID;

			switch(nType)
			{
			case 1067://	ˮ����	
			case 1068://	�ڶ�ˮ��	
			case 1069://	������ˮ��	
			case 1070://	������ˮ��
				return true;

			default:
				return false;
			}
		}

		return false;
	}


	bool CPurifyMgr::IsRepairLifeItem(CItemCommand& rItem)
	{
		CItemRecord* pItemRecord = rItem.GetItemInfo();
		if(pItemRecord)
		{
			long nType = pItemRecord->lID;

			switch(nType)
			{
			case 2236://	�޲�����
				return true;

			default:
				return false;
			}
		}

		return false;
	}


	void CPurifyMgr::PushItem(int iIndex, CItemCommand& rItem)
	{
		// �жϵ����Ƿ������
		if(! rItem.GetIsValid())
		{
			return;
		}

		// �鿴ԭ����Cmd���Ƿ��Ѿ���Item�ˣ���������Ƴ�
		CItemCommand* pItemCommand =  
			dynamic_cast<CItemCommand*>(cmdEquipPurify[iIndex]->GetCommand());
		if (pItemCommand)
		{
			PopItem(iIndex);
		}

		// ��¼Item����Ʒ���е�λ��
		m_iPurifyItemPos[iIndex] = g_stUIEquip.GetGoodsGrid()->GetDragIndex();
		// ��Item��Ӧ����Ʒ���ҵ�
		rItem.SetIsValid(false);

		// ��������Item����Cmd�У�������new������PopItem()��ɾ��
		CItemCommand* pItemCmd = new CItemCommand(rItem);
		pItemCmd->SetIsValid(true);
		cmdEquipPurify[iIndex]->AddCommand(pItemCmd);

		SetPurifyUI();
	}


	void CPurifyMgr::PopItem(int iIndex)
	{
		// ɾ��Cmd�е�Item����Item����PushItem()����new����
		CItemCommand* pItemCommand =  
			dynamic_cast<CItemCommand*>(cmdEquipPurify[iIndex]->GetCommand());
		if (! pItemCommand) return;

		cmdEquipPurify[iIndex]->DelCommand();	// �ú�����ɾ��delete Item

		// ��Item��Ӧ����Ʒ���ָ��ɿ���
		CCommandObj* pItem = 
					g_stUIEquip.GetGoodsGrid()->GetItem(m_iPurifyItemPos[iIndex]);
		if (pItem)
		{
			pItem->SetIsValid(true);
		}

		// ��¼Item����Ʒ���е�λ��
		m_iPurifyItemPos[iIndex] = NO_USE;
	}


	// ���ý��棬������۸�
	void CPurifyMgr::SetPurifyUI()
	{
		CItemCommand* pItem1 = dynamic_cast<CItemCommand*>(cmdEquipPurify[PURIFY_ONE]->GetCommand());
		CItemCommand* pItem2 = dynamic_cast<CItemCommand*>(cmdEquipPurify[PURIFY_TWO]->GetCommand());

		if (NULL == pItem1 || NULL == pItem2)
		{
			btnForgeYes->SetIsEnabled(false);
			labMoneyShow->SetCaption("");
			return;
		}

		pItem1 = dynamic_cast<CItemCommand*>(g_stUIEquip.GetGoodsGrid()->GetItem(m_iPurifyItemPos[PURIFY_ONE]));
		pItem2 = dynamic_cast<CItemCommand*>(g_stUIEquip.GetGoodsGrid()->GetItem(m_iPurifyItemPos[PURIFY_TWO]));

		if (NULL == pItem1 || NULL == pItem2)
		{
			btnForgeYes->SetIsEnabled(false);
			labMoneyShow->SetCaption("");
			return;
		}

		// ������λ���Ѿ����룬��������ȷ������ť
		btnForgeYes->SetIsEnabled(true);

		int nMoney = 0;

		// �����ᴿ
		if(GetType() == CPurifyMgr::PURIFY_TYPE)
		{
			//int nMainLevel = pItem1->GetItemInfo()->sNeedLv;
			//nMoney = nMainLevel * nMainLevel * 1000;

			nMoney = 1000000;
		}
		// ����
		else if(GetType() == CPurifyMgr::ENERGY_TYPE)
		{
			int nBatteryID = pItem2->GetItemInfo()->lID;
			if(nBatteryID == 1022)
			{
				nMoney = 300;
			}
			else if(nBatteryID == 1024)
			{
				nMoney = 1000;
			}
		}
		// ��ʯ
		else if(GetType() == CPurifyMgr::GETSTONE_TYPE)
		{
			int nStoneLevel = pItem1->GetForgeInfo().nStoneLevel[0] + 
							  pItem1->GetForgeInfo().nStoneLevel[1] + 
							  pItem1->GetForgeInfo().nStoneLevel[2];
			nMoney = nStoneLevel * 10000;
		}
		// �����ƹ�
		else if(g_stUIPurify.GetType() == CPurifyMgr::REPAIR_OVEN_TYPE)
		{
			//int nOvenLevel = 0;
			//for(int i = 0; i < 5; ++i)
			//{
			//	if(pItem1->GetData().sInstAttr[i][0] == ITEMATTR_VAL_STR)
			//	{
			//		nOvenLevel = pItem1->GetData().sInstAttr[i][1];
			//		break;
			//	}
			//}

			//nMoney = nOvenLevel * nOvenLevel * 100;

			nMoney = 200;
		}

		char szBuffer[256] = {0};
		sprintf(szBuffer, "%d", nMoney);
		labMoneyShow->SetCaption(szBuffer);
	}


	// �����ᴿЭ��
	void CPurifyMgr::SendNetProtocol()
	{
		CS_ItemForgeAsk(true, GetType(), m_iPurifyItemPos, PURIFY_CELL_COUNT);
	}


///////////////////////////////////////////////////////////////////////////
//
//	������Ϣ����
//

	// ������Ҫ��Ʒ
	void CPurifyMgr::_evtDragPurifyOne(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
	{
		CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand || !pItemCommand->GetIsValid()) return;

		CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
		if( pGood != g_stUIEquip.GetGoodsGrid() ) return;

		// �����ᴿ
		if(g_stUIPurify.GetType() == CPurifyMgr::PURIFY_TYPE)
		{
			//if(! g_stUIPurify.IsEquipItem(*pItemCommand))
			//{
			//	g_pGameApp->MsgBox(g_oLangRec.GetString(831)); // ��Ҫװ�����Ϸ���������ѡ��
			//	return;
			//}
		}
		// ����
		else if(g_stUIPurify.GetType() == CPurifyMgr::ENERGY_TYPE)
		{
			if(pItemCommand->GetItemInfo()->sType != 29)
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(852)); // ������Ĳ��Ǳ��ǣ�������ѡ��
				return;
			}
		}
		// ��ʯ
		else if(g_stUIPurify.GetType() == CPurifyMgr::GETSTONE_TYPE)
		{
			if(! g_stUIPurify.IsEquipItem(*pItemCommand))
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(831)); // ��Ҫװ�����Ϸ���������ѡ��
				return;
			}
		}
		// �����ƹ�
		else if(g_stUIPurify.GetType() == CPurifyMgr::REPAIR_OVEN_TYPE)
		{
			if(! g_stUIPurify.IsMainLifeItem(*pItemCommand))
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(891)); // ������Ĳ�������ܹ��ߣ�������ѡ��
				return;
			}
		}
		else	// ���Ͳ��Ϸ�
		{
			return;
		}

		g_stUIPurify.PushItem(PURIFY_ONE, *pItemCommand);
	}


	// �����ᴿ��Ʒ
	void CPurifyMgr::_evtDragPurifyTwo(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
	{
		CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand || !pItemCommand->GetIsValid()) return;

		CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
		if( pGood != g_stUIEquip.GetGoodsGrid() ) return;

		CItemCommand* pMainItem = dynamic_cast<CItemCommand*>(g_stUIPurify.cmdEquipPurify[PURIFY_ONE]->GetCommand());

		// �����ᴿ
		if(g_stUIPurify.GetType() == CPurifyMgr::PURIFY_TYPE)
		{
			if(NULL == pMainItem)
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(828)); // ���ȷ�����Ҫװ��
				return;
			}

			//if(pMainItem->GetItemInfo()->sType != pItemCommand->GetItemInfo()->sType)
			//{
			//	g_pGameApp->MsgBox(g_oLangRec.GetString(829)); // �ᴿװ������Ҫװ������ͬһ���ͣ�������ѡ��
			//	return;
			//}

			//if(pMainItem->GetData().GetItemLevel() > pItemCommand->GetData().GetItemLevel() + 5)
			//{
			//	g_pGameApp->MsgBox(g_oLangRec.GetString(830)); // �ᴿװ���ȼ�����С����Ҫװ�� 5 ����������ѡ��
			//	return;
			//}
		}
		// ����
		else if(g_stUIPurify.GetType() == CPurifyMgr::ENERGY_TYPE)
		{
			if(NULL == pMainItem)
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(853)); // ���ȷ��뱴��
				return;
			}
		}
		// ��ʯ
		else if(g_stUIPurify.GetType() == CPurifyMgr::GETSTONE_TYPE)
		{
			if(NULL == pMainItem)
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(828)); // ���ȷ�����Ҫװ��
				return;
			}
		}
		// �����ƹ�
		else if(g_stUIPurify.GetType() == CPurifyMgr::REPAIR_OVEN_TYPE)
		{
			if(NULL == pMainItem)
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(892)); // �������������ܹ���
				return;
			}

			if(! g_stUIPurify.IsRepairLifeItem(*pItemCommand))
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(893)); // ������Ĳ����޲����ߣ�������ѡ��
				return;
			}
		}
		// ���Ͳ��Ϸ�
		else
		{
			return;
		}

		g_stUIPurify.PushItem(PURIFY_TWO, *pItemCommand);
	}


	// �������ť�¼�
	void CPurifyMgr::_evtMainMouseButton(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		string strName = pSender->GetName();

		if(strName == "btnForgeYes")
		{
			g_stUIPurify.SendNetProtocol();
			g_stUIPurify.CloseAllForm();
		}
	}


	// �رմ����¼�
	void CPurifyMgr::_evtClosePurifyForm(CForm* pForm, bool& IsClose)
	{
		g_stUIPurify.ClearAllCommand();
		CS_ItemForgeAsk(false);
	}

}

