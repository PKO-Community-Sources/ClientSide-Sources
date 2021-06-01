#ifndef UI_BOOTH_FORM_H
#define UI_BOOTH_FORM_H

#include "uiform.h"
#include "uiGlobalVar.h"

#include <vector>

struct NET_CHARTRADE_BOATDATA;

struct SItemGrid;


namespace GUI
{
	struct stNumBox;
	struct stTradeBox;
	struct stSelectBox;

	class CBoothMgr : public CUIInterface // 界面用户摆摊类
	{
	public:
		CBoothMgr();
		~CBoothMgr();

		// 显示设置摆摊界面
		void ShowSetupBoothForm(int iLevel);
		void SearchAllStalls();
		// 显示摆摊交易界面
		void ShowTradeBoothForm(DWORD dwOwnerId, const char *szBoothName, int nItemNum);

		// 拖动到摊位
		bool PushToBooth(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem);
		// 拖动出摊位
		bool PopFromBooth(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem);
		// 摊位拖动到摊位
		bool BoothToBooth(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem);

		// 服务器传回的增减摊位内货物
		void AddTradeBoothItem(int iGrid, DWORD dwItemID, int iCount, DWORD dwMoney);
		void AddTradeBoothBoat(int iGrid, DWORD dwItemID, int iCount, DWORD dwMoney, NET_CHARTRADE_BOATDATA& Data);
		void AddTradeBoothGood(int iGrid, DWORD dwItemID, int iCount, DWORD dwMoney, SItemGrid& rSItemGrid);
		void RemoveTradeBoothItem( DWORD dwCharID, int iGrid, int iCount); // 删减物品

		void SetupBoothSuccess(); 
		void PullBoothSuccess();   

		// Getters And Setters
		CGoodsGrid* GetBoothItemsGrid() { return grdBoothItem; }
		DWORD GetOwnerId() { return m_dwOwnerId; }
		void  SetOwnerId(DWORD dwOwnerId) { m_dwOwnerId = dwOwnerId; }
		//判断表单是否打开
		bool IsOpen() { return frmBooth->GetIsShow(); }
		//判断是否在摆摊
		bool IsSetupedBooth() { return m_bSetupedBooth; }
		void SetSetupedBooth(bool bSetupedBooth) { m_bSetupedBooth = bSetupedBooth; }

		//因其他人收摊而关闭表单
		void CloseBoothByOther(DWORD dwOtherId)	{ if (dwOtherId == m_dwOwnerId)	CloseBoothUI();	}

	protected:
		virtual bool Init();
		virtual void End();
		virtual void CloseForm();

	private:

		struct SBoothItem;

		// 设置摊位时，拖动到摊位
		bool PushToBoothSetup(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CItemCommand& rkItemCmd);
		// 设置摊位时，拖动出摊位
		bool PopFromBoothSetup(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CItemCommand& rkItemCmd);
		// 交易摊位时，拖动到摊位
		bool PushToBoothTrade(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CItemCommand& rkItemCmd);
		// 交易摊位时，拖动出摊位
		bool PopFromBoothTrade(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CItemCommand& rkItemCmd);
		//根据等级返回摆摊数量
		int GetItemNumByLevel(int iLevel);
		//清除摊位内的物品
		void ClearBoothItems();
		//将当前的拖动的物品放入摊位
		void AddBoothItem(SBoothItem* pBoothItem);
		//将当前拖动的若干个物品放入装备栏
		void RemoveBoothItemByNum(SBoothItem* pBoothItem, int iNum);
		//打开摆摊界面
		void OpenBoothUI();
		//关闭摆摊界面
		void CloseBoothUI();

	private:
		static void _MainMouseBoothEvent(CCompent *pSender, int nMsgType, 
										 int x, int y, DWORD dwKey);
		static void _MainBoothOnCloseEvent(CForm* pForm, bool& IsClose);

		static void _InquireSetupPushItemNumEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _PushItemCurrencyType(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _PushItemTradeQuantity(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _PushItemTradeID(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _SearchStallID(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _PushItemTradeNumEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _InquireSetupPushItemPriceEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _InquireSetupPopItemNumEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _InquireTradeItemNumEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

		static void _BuyGoodsEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _BuyAGoodEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

	private:
		//交易摊位表单
		CForm			* frmBooth;
		CLabel			* lblOwnerName;
		CEdit			* edtBoothName;
		CGoodsGrid		* grdBoothItem;
		CTextButton		* btnSetupBooth;
		CTextButton		* btnPullStakes;


		typedef std::vector<SBoothItem*> BoothItemContainer;
		typedef BoothItemContainer::iterator BoothItemConIter;

		SBoothItem		* m_pkCurrSetupBooth;

		BoothItemContainer m_kBoothItems;
		DWORD			m_dwOwnerId;
		int				m_iBoothItemMaxNum;
		bool			m_isOldEquipFormShow;
		bool			m_bSetupedBooth;

		stNumBox*		m_NumBox;
		stTradeBox*		m_TradeBox;
		stSelectBox*	m_SelectBox;

	};	// end of class CBoothMgr

	//add by ALLEN 2007-10-16
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class CReadBookMgr
	{
	public:
		static bool IsCanReadBook(CCharacter* pCha);
		static bool ShowReadBookForm();

	private:
		static void _evtSelectBox(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _evtMsgBox(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static CCharacter* _pCha;
	};
}	// end of namespace GUI

#endif	// UI_BOOTH_FORM_H
