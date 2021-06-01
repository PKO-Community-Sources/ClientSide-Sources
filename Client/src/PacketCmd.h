#ifndef PACKETCMD_H
#define PACKETCMD_H

#include "NetIF.h"
#include "NetProtocol.h"

extern char autoLoginChar[32] ;
extern bool canAutoLoginChar;

class CActionState;

extern char overrideIP[16];
//-----------------------------------------------------------------------------------
//  协议函数列表 类别包括: [登录协议] [角色选择协议] [移动协议] [战斗协议] [聊天协议]
//-----------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
//                          Client To Server Protocol
//---------------------------------------------------------------------------------
// 协议C->S : 连接游戏服务器,返回值true，提交连接请求成功，false，提交连接请求失败，提交成功后使用GetConnStat检测连接过程的状态。
extern bool	CS_Connect(dbc::cChar *hostname,dbc::uShort port,dbc::uLong timeout);
// 协议C->S : 断开连接,程序退出应该直接调用ShutDown,不用单独调用Disconnect;
extern void	CS_Disconnect(int reason);


// 协议C->S : 发送登录命令
extern void	CS_Login(const char *accounts,const char *password, const char* passport);
// 协议C->S : 发送登出命令
extern void	CS_Logout();

// 要求取消计时退出
extern void CS_CancelExit();

// 创建二次密码
extern void CS_CreatePassword2( const char szPassword[] );
// 更新二次密码
extern void CS_UpdatePassword2( const char szOld[], const char szPassword[] );

//背包锁定
extern void CS_LockKitbag();
extern void CS_UnlockKitbag( const char szPassword[] );
extern void CS_KitbagCheck();
extern void CS_AutoKitbagLock(bool bAutoLock);

// 协议C->S : 发送开始玩角色命令
extern void	CS_BeginPlay(const char *cha);
// 协议C->S : 发送结束玩命令
extern void	CS_EndPlay();

// 协议C->S : 发送新建角色命令
extern void	CS_NewCha(const char *chaname,const char *birth,LOOK &look);
extern void	CS_NewCha2(const char *chaname,const char *birth,int type, int hair, int face);
// 协议C->S : 发送删除角色命令
extern void	CS_DelCha(const char *cha, const char szPassword2[]);

// 协议C->S : 发送行动信息
extern void	CS_BeginAction( CCharacter* pCha, DWORD type, void* param, CActionState* pState=NULL );
// 协议C->S : 发送停止行动信息
extern void	CS_EndAction( CActionState* pState=NULL );
// 协议C->S : 发送死亡回城信息
extern void	CS_DieReturn(char chReliveType);
// 协议C->S	: Ping服务器，返回毫秒数。
extern void	CS_SendPing();
// 大地图
extern void	CS_MapMask(char *szMapName = "");

// 协议C->S : 发送聊天消息
extern void CP_GuildAttr(int attr);
extern void CS_GuildBankOper(stNetBank * pNetBank);
extern void CS_BagOfHoldingOper(stNetBank * pNetBank);
extern void CS_GuildBankTakeGold(int gold);
extern void CS_GuildBankTakeGold(int gold);

extern void CS_Say(const char *pszContent);
extern void CS_InvSort(int type,int dir = 0);
extern void CS_RequestBagOfHolding(int type,int dir);
extern void CS_Register(const char *user,const char *pass,const char *email);
extern void CS_GuildBankGiveGold(int value);
extern void CS_GuildBankTakeGold(int value);
extern void CS_StallSearch(int itemID);
extern void CS_ChangePass(const char *pass,const char *pin);
// Added by knight-gongjian 2004.11.29
// NPC对话
extern void	CS_RequestTalk( DWORD dwNpcID, BYTE byCmd );
extern void	CS_SelFunction( DWORD dwNpcID, BYTE byPageID, BYTE byIndex );

// NPC交易
extern void	CS_Sale( DWORD dwNpcID, BYTE byIndex, BYTE byCount );
extern void	CS_Buy(  DWORD dwNpcID, BYTE byItemType, BYTE byIndex1, BYTE byIndex2, BYTE byCount );

// 船舱货物交易
extern void CS_SelectBoatList( DWORD dwNpcID, BYTE byType, BYTE byIndex );
extern void CS_SaleGoods( DWORD dwNpcID, DWORD dwBoatID, BYTE byIndex, BYTE byCount );
extern void CS_BuyGoods( DWORD dwNpcID, DWORD dwBoatID, BYTE byItemType, BYTE byIndex1, BYTE byIndex2, BYTE byCount );

// 角色交易,交易中的所有ID都为当前的WorldID,通过类型byType知道是船的货舱交易还是人的道具交易
extern void	CS_RequestTrade( BYTE byType, DWORD dwCharID );
extern void	CS_AcceptTrade( BYTE byType, DWORD dwCharID );
extern void	CS_AddItem( BYTE byType, DWORD dwCharID, BYTE byOpType, BYTE byIndex, BYTE byItemIndex, BYTE byCount );
extern void	CS_AddMoney( BYTE byType, DWORD dwCharID, BYTE byOpType, DWORD dwMoney );
extern void	CS_AddIMP( BYTE byType, DWORD dwCharID, BYTE byOpType, DWORD dwMoney );
extern void	CS_ValidateTradeData( BYTE byType, DWORD dwCharID );
extern void	CS_ValidateTrade( BYTE byType, DWORD dwCharID );
extern void	CS_CancelTrade( BYTE byType, DWORD dwCharID );

// 任务操作
extern void	CS_MissionPage( DWORD dwNpcID, BYTE byCmd, BYTE bySelItem = 0, BYTE byParam = 0 );
extern void	CS_SelMission( DWORD dwNpcID, BYTE byIndex );
extern void	CS_MissionTalk( DWORD dwNpcID, BYTE byCmd );
extern void	CS_SelMissionFunc( DWORD dwNpcID, BYTE byPageID, BYTE byIndex );
extern void	CS_MisLog();
extern void	CS_MisLogInfo( WORD wID );
extern void	CS_MisClear( WORD wID );

// 精练
extern void	CS_ForgeItem( BYTE byIndex );

// 造船
extern void	CS_UpdateBoat( char szHeader, char szEngine, char szCannon, char szEquipment );
extern void	CS_CancelBoat();
extern void	CS_CreateBoat( const char szBoat[], char szHeader, char szEngine, char szCannon, char szEquipment );
extern void	CS_SelectBoat( DWORD dwNpcID, BYTE byIndex );
extern void CS_GetBoatInfo();

// 船只物品打包
extern void	CS_SelectBoatBag( DWORD dwNpcID, BYTE byIndex );

// 事件实体交互
extern void CS_EntityEvent( DWORD dwEntityID );

// 摆摊货物交易
extern void CS_StallInfo( const char szName[], mission::NET_STALL_ALLDATA& Data );
extern void CS_StallOpen( DWORD dwCharID );
extern void CS_StallBuy( DWORD dwCharID, BYTE byIndex, BYTE byCount,int gridID );
extern void CS_StallClose();
extern BOOL	SC_SynStallName(LPRPACKET pk);

//读书   add by jilinlee 2007/4/20
extern void CS_ReadBookStart();
extern void CS_ReadBookClose();

// 计时退出
extern BOOL SC_StartExit( LPRPACKET packet );
extern BOOL SC_CancelExit( LPRPACKET packet );

// 请求更新发型
extern void CS_UpdateHair( stNetUpdateHair& stData );
// End

// 队伍单挑
extern void CS_TeamFightAsk( unsigned long ulWorldID, long lHandle, char chType );
extern void CS_TeamFightAnswer(bool bAccess);

// 道具修理
extern void CS_ItemRepairAsk(long lRepairmanID, long lRepairmanHandle, char chPosType, char chPosID);
extern void	CS_ItemRepairAnswer(bool bAccess);

// 道具精炼
extern void	CS_ItemForgeAsk(bool bSure, stNetItemForgeAsk *pSForge = NULL);
extern void CS_ItemForgeAsk(bool bSure, int nType, int arPacketPos[], int nPosCount);
extern void CS_ItemForgeAnswer(bool bAccess);

// Add by lark.li 20080514 begin
extern void CS_ItemLotteryAnswer(bool bAccess);
extern void CS_ItemLotteryAsk(bool bSure, stNetItemLotteryAsk *pSLottery = NULL);
// End

extern void CS_ItemAmphitheaterAsk(bool bSure,int IDindex);//Add by sunny.sun 20080726

// 商城
extern void CS_StoreOpenAsk(const char szPassword[]);
extern void CS_StoreListAsk(long lClsID, short sPage, short sNum);
extern void CS_StoreBuyAsk(long lComID);
extern void CS_StoreChangeAsk(long lNum);
extern void CS_StoreQuery(long lNum);
//extern void CS_StoreVIP(short sVipID, short sMonth);
extern void CS_StoreClose();

// 黑市商人
extern void CS_BlackMarketExchangeReq( DWORD dwNpcID, short sSrcID, short sSrcNum, short sTarID, short sTarNum, short sTimeNum, BYTE byIndex );

//精灵老虎机
extern void CS_TigerStart(DWORD dwNpcID, short sSel1, short sSel2, short sSel3);
extern void CS_TigerStop(DWORD dwNpcID, short sNum);

//老手带新手
extern void CS_VolunteerList(short sPage, short sNum);
extern void CS_VolunteerAdd();
extern void CS_VolunteerDel();
extern void CS_VolunteerSel(const char *szName);
extern void CS_VolunteerOpen(short sNum);
extern void CS_VolunteerAsr(BOOL bRet, const char *szName);

//同步临时背包
extern void CS_SyncKitbagTemp();

// 报告外挂
extern void CS_ReportWG( const char szInfo[] );

// Add by lark.li 20080707 begun
extern void CS_CaptainConfirmAsr(short sRet, DWORD dwTeamID);
// End

//拜师
extern void CS_MasterInvite(const char *szName, DWORD dwCharID);
extern void CS_MasterAsr(short sRet, const char *szName, DWORD dwCharID);
extern void CS_MasterDel(const char *szName, uLong ulChaID);
extern void CS_PrenticeDel(const char *szName, uLong ulChaID);
extern void CP_Master_Refresh_Info(unsigned long chaid);
extern void CP_Prentice_Refresh_Info(unsigned long chaid);
extern void CS_PrenticeInvite(const char *szName, DWORD dwCharID);
extern void CS_PrenticeAsr(short sRet, const char *szName, DWORD dwCharID);

//extern void CS_PKSilverSort(DWORD dwNPCID, short sItemPos);

extern void CS_LifeSkill(long type, DWORD dwNPCID);
extern void CS_Compose(DWORD dwNPCID, int* iPos, int iCount, bool asr = false);
extern void CS_Break(DWORD dwNPCID, int* iPos, int iCount, bool asr = false);
extern void CS_Found(DWORD dwNPCID, int* iPos, int iCount, short big, bool asr = false);
extern void CS_Cooking(DWORD dwNPCID, int* iPos, int iCount, short percent, bool asr = false);
extern void CS_UnlockCharacter();

extern void CS_Say2Camp(const char *szContent);

extern void CS_GMSend(DWORD dwNPCID, const char *szTitle, const char *szContent);
extern void CS_GMRecv(DWORD dwNPCID);

//extern void CS_PKCtrl(bool bCanPK);

extern void CS_CheatCheck(cChar *answer);
//add by ALLEN 2007-10-19
extern void CS_AutionBidup(DWORD dwNPCID, short sItemID, uLong price);

extern void CS_AntiIndulgence_Close();

//	道具加锁。
extern	void	CS_DropLock(int slot);
extern	void	CS_UnlockItem( const char szPassword[],int slot);

					
extern	void	CS_SetGuildPerms( DWORD ID, uLong Perms );

extern	void	CS_SendGameRequest( const char szPassword[]);
//---------------------------------------------------------------------------------
//                          Server To Client Protocol
//---------------------------------------------------------------------------------
// 协议S->C : 接收登陆命令
extern BOOL SC_ShowRanking(LPRPACKET pk);
extern BOOL SC_GuildQuest(LPRPACKET pk);
extern BOOL SC_UpdateGuildGold(LPRPACKET pk);
extern BOOL SC_ShowStallSearch(LPRPACKET pk);
extern BOOL SC_CHARSTR(LPRPACKET pk);
extern BOOL	SC_Login(LPRPACKET pk);
extern BOOL	SC_EnterMap(LPRPACKET pk);
extern BOOL SC_BeginPlay(LPRPACKET pk);
extern BOOL	SC_EndPlay(LPRPACKET pk);
extern BOOL	SC_NewCha(LPRPACKET pk);
extern BOOL	SC_DelCha(LPRPACKET pk);
extern BOOL SC_CreatePassword2(LPRPACKET pk);
extern BOOL SC_UpdatePassword2(LPRPACKET pk);

// 协议S->C : 其它角色(包含怪物等)出现
extern BOOL    SC_ChaBeginSee(LPRPACKET pk);

// 协议S->C : 其它角色(包含怪物等)消失
extern BOOL	SC_ChaEndSee(LPRPACKET pk);

// 协议S->C : 添加道具角色
extern BOOL	SC_AddItemCha(LPRPACKET pk);

// 协议S->C : 删除道具角色
extern BOOL	SC_DelItemCha(LPRPACKET pk);

// 协议S->C : 角色行动通知
// 消息定义 : CMD_MC_NOTIACTION
// 适用对象 : 主角, 他人
extern BOOL	SC_CharacterAction(LPRPACKET pk);

// 协议S->C : 角色请求的行动失败
extern BOOL	SC_FailedAction(LPRPACKET pk);

// 协议S->C : 其它角色(包含怪物等)出现
extern BOOL    SC_ItemCreate(LPRPACKET pk);

// 协议S->C : 其它角色(包含怪物等)消失
extern BOOL	SC_ItemDestroy(LPRPACKET pk);

// 协议S->C : 同步角色属性
extern BOOL	SC_SynAttribute(LPRPACKET pk);

// 协议S->C : 同步角色技能包
extern BOOL	SC_SynSkillBag(LPRPACKET pk);

// 协议S->C : 同步角色技能包
extern BOOL	SC_SynDefaultSkill(LPRPACKET pk);

// 协议S->C : 同步角色技能状态
extern BOOL	SC_SynSkillState(LPRPACKET pk);

// 协议S->C : 同步队伍信息
extern BOOL	SC_SynTeam(LPRPACKET pk);

// 协议S->C : 同步角色的队长信息
extern BOOL	SC_SynTLeaderID(LPRPACKET pk);

// 协议S->C : 信息显示
extern BOOL	SC_Message(LPRPACKET pk);

// 协议S->C : 其它角色(包含怪物等)出现
extern BOOL    SC_AStateBeginSee(LPRPACKET pk);

// 协议S->C : 其它角色(包含怪物等)消失
extern BOOL	SC_AStateEndSee(LPRPACKET pk);


// 怪物表情
extern BOOL	SC_Cha_Emotion(LPRPACKET pk);

//
extern BOOL	PC_Ping(LPRPACKET pk);
extern BOOL PC_REGISTER(LPRPACKET pk);
extern BOOL	SC_Ping(LPRPACKET pk);
extern BOOL	SC_CheckPing(LPRPACKET pk);

//系统信息提示
extern BOOL	SC_SysInfo(LPRPACKET pk);
extern BOOL	SC_BickerNotice( LPRPACKET pk );
extern BOOL SC_ColourNotice( LPRPACKET pk );
extern BOOL	SC_Say(LPRPACKET pk);

extern BOOL	SC_GuildInfo(LPRPACKET pk);
extern BOOL GuildSysInfo;

// Added by knight-gongjian 2004.11.29
// NPC对话
extern BOOL	SC_TalkInfo( LPRPACKET packet );
extern BOOL	SC_FuncInfo( LPRPACKET packet );
extern BOOL	SC_CloseTalk( LPRPACKET packet );
extern BOOL	SC_HelpInfo( LPRPACKET packet );

extern BOOL SC_TradeData( LPRPACKET packet );
extern BOOL SC_TradeAllData( LPRPACKET packet );
extern BOOL	SC_TradeInfo( LPRPACKET packet );
extern BOOL	SC_TradeUpdate( LPRPACKET packet );
extern BOOL	SC_TradeResult( LPRPACKET packet );

// npc 任务状态切换
extern BOOL	SC_NpcStateChange( LPRPACKET packet );

// 事件实体状态切换
extern BOOL SC_EntityStateChange( LPRPACKET packet );

// 角色交易
extern BOOL	SC_CharTradeInfo( LPRPACKET packet );

// 任务操作
extern BOOL	SC_MissionInfo( LPRPACKET packet );
extern BOOL	SC_MisPage( LPRPACKET packet );
extern BOOL	SC_MisLog( LPRPACKET packet );
extern BOOL	SC_MisLogInfo( LPRPACKET packet );
extern BOOL	SC_MisLogClear( LPRPACKET packet );
extern BOOL	SC_MisLogAdd( LPRPACKET packet );
extern BOOL	SC_MisLogState( LPRPACKET packet );

// 任务触发器动作通知
extern BOOL	SC_TriggerAction( LPRPACKET packet );

// 物品精练
extern BOOL	SC_Forge( LPRPACKET packet );

extern BOOL SC_Unite( LPRPACKET packet );
extern BOOL SC_Milling( LPRPACKET packet );
extern BOOL SC_Fusion( LPRPACKET packet );
extern BOOL SC_Upgrade( LPRPACKET packet );
extern BOOL SC_EidolonMetempsychosis( LPRPACKET packet );
extern BOOL SC_Eidolon_Fusion(LPRPACKET packet);
extern BOOL SC_Purify(LPRPACKET packet);
extern BOOL SC_Fix(LPRPACKET packet);
extern BOOL SC_GetStone(LPRPACKET packet);
extern BOOL SC_Energy(LPRPACKET packet);
extern BOOL SC_Tiger(LPRPACKET packet);
extern BOOL SC_GMSend(LPRPACKET packet);
extern BOOL SC_GMRecv(LPRPACKET packet);

// 船只建造
extern BOOL	SC_CreateBoat( LPRPACKET packet );
extern BOOL	SC_UpdateBoat( LPRPACKET packet );
extern BOOL	SC_UpdateBoatPart( LPRPACKET packet );
extern BOOL	SC_BoatList( LPRPACKET packet );
extern BOOL SC_BoatInfo( LPRPACKET packet );

// 船只打包
//extern BOOL	SC_BoatBagList( LPRPACKET packet );

// 角色货物摆摊交易
extern BOOL SC_StallInfo( LPRPACKET packet );
extern BOOL SC_StallUpdateInfo( LPRPACKET packet );
extern BOOL SC_StallDelGoods( LPRPACKET packet );
extern BOOL SC_StallClose( LPRPACKET packet );
extern BOOL SC_StallSuccess( LPRPACKET packet );

// End by knight.gong

// 理发
extern BOOL SC_OpenHairCut( LPRPACKET packet );		// 打开理发界面
extern BOOL SC_UpdateHairRes( LPRPACKET packet );	// 理发结果返回

// 队伍挑战
extern BOOL	SC_TeamFightAsk(LPRPACKET packet);

// 道具修理
extern BOOL	SC_BeginItemRepair(LPRPACKET packet);
extern BOOL	SC_ItemRepairAsk(LPRPACKET packet);

// 道具精炼/合成
extern BOOL	SC_ItemForgeAsk(LPRPACKET packet);
extern BOOL	SC_ItemForgeAnswer(LPRPACKET packet);

// 使用道具成功
extern BOOL	SC_ItemUseSuc(LPRPACKET packet);

// 背包容量
extern BOOL	SC_EspeItem(LPRPACKET packet);

// 特殊道具
extern BOOL	SC_KitbagCapacity(LPRPACKET packet);

// 附加外观
extern BOOL	SC_SynAppendLook(LPRPACKET packet);

//地图服务器故障
extern BOOL    SC_MapCrash(LPRPACKET packet);

extern BOOL    SC_QueryCha(LPRPACKET packet);
extern BOOL    SC_QueryChaItem(LPRPACKET packet);
extern BOOL    SC_QueryChaPing(LPRPACKET packet);

extern BOOL    SC_QueryRelive(LPRPACKET packet);
extern BOOL    SC_PreMoveTime(LPRPACKET packet);
extern BOOL    SC_MapMask(LPRPACKET packet);
extern BOOL    SC_SynEventInfo(LPRPACKET packet);
extern BOOL    SC_SynSideInfo(LPRPACKET packet);

//背包锁定
extern BOOL    SC_KitbagCheckAnswer(LPRPACKET packet);

//商城
extern BOOL	SC_StoreOpenAnswer(LPRPACKET packet);
extern BOOL	SC_StoreListAnswer(LPRPACKET packet);
extern BOOL	SC_StoreBuyAnswer(LPRPACKET packet);
extern BOOL	SC_StoreChangeAnswer(LPRPACKET packet);
extern BOOL	SC_StoreHistory(LPRPACKET packet);
extern BOOL	SC_ActInfo(LPRPACKET packet);
extern BOOL SC_StoreVIP(LPRPACKET packet);

//黑市商人
extern BOOL SC_BlackMarketExchangeData(LPRPACKET packet);
extern BOOL SC_BlackMarketExchangeAsr(LPRPACKET packet);
extern BOOL SC_BlackMarketExchangeUpdate(LPRPACKET packet);
extern BOOL SC_ExchangeData(LPRPACKET packet);

extern BOOL SC_TigerItemID(LPRPACKET packet);

//老手带新手
extern BOOL SC_VolunteerList(LPRPACKET packet);
extern BOOL SC_VolunteerState(LPRPACKET packet);
extern BOOL SC_VolunteerOpen(LPRPACKET packet);
extern BOOL SC_VolunteerAsk(LPRPACKET packet);

//同步临时背包
extern BOOL SC_SyncKitbagTemp(LPRPACKET packet);

extern BOOL SC_SyncTigerString(LPRPACKET packet);

extern BOOL SC_MasterAsk(LPRPACKET packet);
extern BOOL PC_MasterRefresh(LPRPACKET packet);
extern BOOL PC_MasterCancel(LPRPACKET packet);
extern BOOL PC_MasterRefreshInfo(LPRPACKET packet);
extern BOOL PC_PrenticeRefreshInfo(LPRPACKET packet);
extern BOOL SC_PrenticeAsk(LPRPACKET packet);

extern BOOL SC_Say2Camp(LPRPACKET packet);

extern BOOL SC_GMMail(LPRPACKET packet);

extern BOOL SC_PopupNotice(LPRPACKET pk);

extern BOOL SC_CheatCheck(LPRPACKET pk);

extern BOOL SC_ListAuction(LPRPACKET pk);

extern void	ReadChaBasePacket(LPRPACKET pk, stNetActorCreate &SCreateInfo);
extern BOOL	ReadChaSkillBagPacket(LPRPACKET pk, stNetSkillBag &SCurSkill, char *szLogName);
extern void	ReadChaSkillStatePacket(LPRPACKET pk, stNetSkillState &SCurSState, char* szLogName);
extern void	ReadChaAttrPacket(LPRPACKET pk, stNetChaAttr &SChaAttr, char* szLogName);
extern void	ReadChaLookPacket(LPRPACKET pk, stNetLookInfo &SLookInfo, char *szLogName);
extern void	ReadChaKitbagPacket(LPRPACKET pk, stNetKitbag &SKitbag, char *szLogName);
extern void	ReadChaShortcutPacket(LPRPACKET pk, stNetShortCut &SShortcut, char* szLogName);
extern void	ReadChaLookEnergyPacket(LPRPACKET pk, stLookEnergy &SLookEnergy, char *szLogName);
extern void	ReadChaPKPacket(LPRPACKET pk, stNetPKCtrl &SNetPKCtrl, char *szLogName);
extern void	ReadEntEventPacket(LPRPACKET pk, stNetEvent &SNetEvent, char *szLogName = 0);
extern void	ReadChaSidePacket(LPRPACKET pk, stNetChaSideInfo &SNetSideInfo, char *szLogName);
extern void	ReadChaAppendLookPacket(LPRPACKET pk, stNetAppendLook &SNetAppendLook, char *szLogName = 0);

//反斗白银城
extern BOOL PC_PKSilver(LPRPACKET packet);

extern BOOL SC_LifeSkillShow(LPRPACKET packet);
extern BOOL SC_LifeSkill(LPRPACKET packet);
extern BOOL SC_LifeSkillAsr(LPRPACKET packet);

extern BOOL SC_ChaPlayEffect(LPRPACKET packet);

//	道具锁定。
extern	BOOL	SC_DropLockAsr(LPRPACKET pk );
extern  BOOL    SC_UnlockItemAsr(LPRPACKET pk );

//==================================================================================


extern  BOOL    SC_InitGuildHouseObjects(LPRPACKET pk );
extern  BOOL    SC_AddGuildHouseObject(LPRPACKET pk );
extern  BOOL    SC_RemoveGuildHouseObject(int index);
extern  BOOL    SC_RemoveAllGuildHouseObjects();



#endif		//PACKETCMD_H
