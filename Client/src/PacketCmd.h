#ifndef PACKETCMD_H
#define PACKETCMD_H

#include "NetIF.h"
#include "NetProtocol.h"

extern char autoLoginChar[32] ;
extern bool canAutoLoginChar;

class CActionState;

extern char overrideIP[16];
//-----------------------------------------------------------------------------------
//  Э�麯���б� ������: [��¼Э��] [��ɫѡ��Э��] [�ƶ�Э��] [ս��Э��] [����Э��]
//-----------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
//                          Client To Server Protocol
//---------------------------------------------------------------------------------
// Э��C->S : ������Ϸ������,����ֵtrue���ύ��������ɹ���false���ύ��������ʧ�ܣ��ύ�ɹ���ʹ��GetConnStat������ӹ��̵�״̬��
extern bool	CS_Connect(dbc::cChar *hostname,dbc::uShort port,dbc::uLong timeout);
// Э��C->S : �Ͽ�����,�����˳�Ӧ��ֱ�ӵ���ShutDown,���õ�������Disconnect;
extern void	CS_Disconnect(int reason);


// Э��C->S : ���͵�¼����
extern void	CS_Login(const char *accounts,const char *password, const char* passport);
// Э��C->S : ���͵ǳ�����
extern void	CS_Logout();

// Ҫ��ȡ����ʱ�˳�
extern void CS_CancelExit();

// ������������
extern void CS_CreatePassword2( const char szPassword[] );
// ���¶�������
extern void CS_UpdatePassword2( const char szOld[], const char szPassword[] );

//��������
extern void CS_LockKitbag();
extern void CS_UnlockKitbag( const char szPassword[] );
extern void CS_KitbagCheck();
extern void CS_AutoKitbagLock(bool bAutoLock);

// Э��C->S : ���Ϳ�ʼ���ɫ����
extern void	CS_BeginPlay(const char *cha);
// Э��C->S : ���ͽ���������
extern void	CS_EndPlay();

// Э��C->S : �����½���ɫ����
extern void	CS_NewCha(const char *chaname,const char *birth,LOOK &look);
extern void	CS_NewCha2(const char *chaname,const char *birth,int type, int hair, int face);
// Э��C->S : ����ɾ����ɫ����
extern void	CS_DelCha(const char *cha, const char szPassword2[]);

// Э��C->S : �����ж���Ϣ
extern void	CS_BeginAction( CCharacter* pCha, DWORD type, void* param, CActionState* pState=NULL );
// Э��C->S : ����ֹͣ�ж���Ϣ
extern void	CS_EndAction( CActionState* pState=NULL );
// Э��C->S : ���������س���Ϣ
extern void	CS_DieReturn(char chReliveType);
// Э��C->S	: Ping�����������غ�������
extern void	CS_SendPing();
// ���ͼ
extern void	CS_MapMask(char *szMapName = "");

// Э��C->S : ����������Ϣ
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
// NPC�Ի�
extern void	CS_RequestTalk( DWORD dwNpcID, BYTE byCmd );
extern void	CS_SelFunction( DWORD dwNpcID, BYTE byPageID, BYTE byIndex );

// NPC����
extern void	CS_Sale( DWORD dwNpcID, BYTE byIndex, BYTE byCount );
extern void	CS_Buy(  DWORD dwNpcID, BYTE byItemType, BYTE byIndex1, BYTE byIndex2, BYTE byCount );

// ���ջ��ｻ��
extern void CS_SelectBoatList( DWORD dwNpcID, BYTE byType, BYTE byIndex );
extern void CS_SaleGoods( DWORD dwNpcID, DWORD dwBoatID, BYTE byIndex, BYTE byCount );
extern void CS_BuyGoods( DWORD dwNpcID, DWORD dwBoatID, BYTE byItemType, BYTE byIndex1, BYTE byIndex2, BYTE byCount );

// ��ɫ����,�����е�����ID��Ϊ��ǰ��WorldID,ͨ������byType֪���Ǵ��Ļ��ս��׻����˵ĵ��߽���
extern void	CS_RequestTrade( BYTE byType, DWORD dwCharID );
extern void	CS_AcceptTrade( BYTE byType, DWORD dwCharID );
extern void	CS_AddItem( BYTE byType, DWORD dwCharID, BYTE byOpType, BYTE byIndex, BYTE byItemIndex, BYTE byCount );
extern void	CS_AddMoney( BYTE byType, DWORD dwCharID, BYTE byOpType, DWORD dwMoney );
extern void	CS_AddIMP( BYTE byType, DWORD dwCharID, BYTE byOpType, DWORD dwMoney );
extern void	CS_ValidateTradeData( BYTE byType, DWORD dwCharID );
extern void	CS_ValidateTrade( BYTE byType, DWORD dwCharID );
extern void	CS_CancelTrade( BYTE byType, DWORD dwCharID );

// �������
extern void	CS_MissionPage( DWORD dwNpcID, BYTE byCmd, BYTE bySelItem = 0, BYTE byParam = 0 );
extern void	CS_SelMission( DWORD dwNpcID, BYTE byIndex );
extern void	CS_MissionTalk( DWORD dwNpcID, BYTE byCmd );
extern void	CS_SelMissionFunc( DWORD dwNpcID, BYTE byPageID, BYTE byIndex );
extern void	CS_MisLog();
extern void	CS_MisLogInfo( WORD wID );
extern void	CS_MisClear( WORD wID );

// ����
extern void	CS_ForgeItem( BYTE byIndex );

// �촬
extern void	CS_UpdateBoat( char szHeader, char szEngine, char szCannon, char szEquipment );
extern void	CS_CancelBoat();
extern void	CS_CreateBoat( const char szBoat[], char szHeader, char szEngine, char szCannon, char szEquipment );
extern void	CS_SelectBoat( DWORD dwNpcID, BYTE byIndex );
extern void CS_GetBoatInfo();

// ��ֻ��Ʒ���
extern void	CS_SelectBoatBag( DWORD dwNpcID, BYTE byIndex );

// �¼�ʵ�彻��
extern void CS_EntityEvent( DWORD dwEntityID );

// ��̯���ｻ��
extern void CS_StallInfo( const char szName[], mission::NET_STALL_ALLDATA& Data );
extern void CS_StallOpen( DWORD dwCharID );
extern void CS_StallBuy( DWORD dwCharID, BYTE byIndex, BYTE byCount,int gridID );
extern void CS_StallClose();
extern BOOL	SC_SynStallName(LPRPACKET pk);

//����   add by jilinlee 2007/4/20
extern void CS_ReadBookStart();
extern void CS_ReadBookClose();

// ��ʱ�˳�
extern BOOL SC_StartExit( LPRPACKET packet );
extern BOOL SC_CancelExit( LPRPACKET packet );

// ������·���
extern void CS_UpdateHair( stNetUpdateHair& stData );
// End

// ���鵥��
extern void CS_TeamFightAsk( unsigned long ulWorldID, long lHandle, char chType );
extern void CS_TeamFightAnswer(bool bAccess);

// ��������
extern void CS_ItemRepairAsk(long lRepairmanID, long lRepairmanHandle, char chPosType, char chPosID);
extern void	CS_ItemRepairAnswer(bool bAccess);

// ���߾���
extern void	CS_ItemForgeAsk(bool bSure, stNetItemForgeAsk *pSForge = NULL);
extern void CS_ItemForgeAsk(bool bSure, int nType, int arPacketPos[], int nPosCount);
extern void CS_ItemForgeAnswer(bool bAccess);

// Add by lark.li 20080514 begin
extern void CS_ItemLotteryAnswer(bool bAccess);
extern void CS_ItemLotteryAsk(bool bSure, stNetItemLotteryAsk *pSLottery = NULL);
// End

extern void CS_ItemAmphitheaterAsk(bool bSure,int IDindex);//Add by sunny.sun 20080726

// �̳�
extern void CS_StoreOpenAsk(const char szPassword[]);
extern void CS_StoreListAsk(long lClsID, short sPage, short sNum);
extern void CS_StoreBuyAsk(long lComID);
extern void CS_StoreChangeAsk(long lNum);
extern void CS_StoreQuery(long lNum);
//extern void CS_StoreVIP(short sVipID, short sMonth);
extern void CS_StoreClose();

// ��������
extern void CS_BlackMarketExchangeReq( DWORD dwNpcID, short sSrcID, short sSrcNum, short sTarID, short sTarNum, short sTimeNum, BYTE byIndex );

//�����ϻ���
extern void CS_TigerStart(DWORD dwNpcID, short sSel1, short sSel2, short sSel3);
extern void CS_TigerStop(DWORD dwNpcID, short sNum);

//���ִ�����
extern void CS_VolunteerList(short sPage, short sNum);
extern void CS_VolunteerAdd();
extern void CS_VolunteerDel();
extern void CS_VolunteerSel(const char *szName);
extern void CS_VolunteerOpen(short sNum);
extern void CS_VolunteerAsr(BOOL bRet, const char *szName);

//ͬ����ʱ����
extern void CS_SyncKitbagTemp();

// �������
extern void CS_ReportWG( const char szInfo[] );

// Add by lark.li 20080707 begun
extern void CS_CaptainConfirmAsr(short sRet, DWORD dwTeamID);
// End

//��ʦ
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

//	���߼�����
extern	void	CS_DropLock(int slot);
extern	void	CS_UnlockItem( const char szPassword[],int slot);

					
extern	void	CS_SetGuildPerms( DWORD ID, uLong Perms );

extern	void	CS_SendGameRequest( const char szPassword[]);
//---------------------------------------------------------------------------------
//                          Server To Client Protocol
//---------------------------------------------------------------------------------
// Э��S->C : ���յ�½����
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

// Э��S->C : ������ɫ(���������)����
extern BOOL    SC_ChaBeginSee(LPRPACKET pk);

// Э��S->C : ������ɫ(���������)��ʧ
extern BOOL	SC_ChaEndSee(LPRPACKET pk);

// Э��S->C : ��ӵ��߽�ɫ
extern BOOL	SC_AddItemCha(LPRPACKET pk);

// Э��S->C : ɾ�����߽�ɫ
extern BOOL	SC_DelItemCha(LPRPACKET pk);

// Э��S->C : ��ɫ�ж�֪ͨ
// ��Ϣ���� : CMD_MC_NOTIACTION
// ���ö��� : ����, ����
extern BOOL	SC_CharacterAction(LPRPACKET pk);

// Э��S->C : ��ɫ������ж�ʧ��
extern BOOL	SC_FailedAction(LPRPACKET pk);

// Э��S->C : ������ɫ(���������)����
extern BOOL    SC_ItemCreate(LPRPACKET pk);

// Э��S->C : ������ɫ(���������)��ʧ
extern BOOL	SC_ItemDestroy(LPRPACKET pk);

// Э��S->C : ͬ����ɫ����
extern BOOL	SC_SynAttribute(LPRPACKET pk);

// Э��S->C : ͬ����ɫ���ܰ�
extern BOOL	SC_SynSkillBag(LPRPACKET pk);

// Э��S->C : ͬ����ɫ���ܰ�
extern BOOL	SC_SynDefaultSkill(LPRPACKET pk);

// Э��S->C : ͬ����ɫ����״̬
extern BOOL	SC_SynSkillState(LPRPACKET pk);

// Э��S->C : ͬ��������Ϣ
extern BOOL	SC_SynTeam(LPRPACKET pk);

// Э��S->C : ͬ����ɫ�Ķӳ���Ϣ
extern BOOL	SC_SynTLeaderID(LPRPACKET pk);

// Э��S->C : ��Ϣ��ʾ
extern BOOL	SC_Message(LPRPACKET pk);

// Э��S->C : ������ɫ(���������)����
extern BOOL    SC_AStateBeginSee(LPRPACKET pk);

// Э��S->C : ������ɫ(���������)��ʧ
extern BOOL	SC_AStateEndSee(LPRPACKET pk);


// �������
extern BOOL	SC_Cha_Emotion(LPRPACKET pk);

//
extern BOOL	PC_Ping(LPRPACKET pk);
extern BOOL PC_REGISTER(LPRPACKET pk);
extern BOOL	SC_Ping(LPRPACKET pk);
extern BOOL	SC_CheckPing(LPRPACKET pk);

//ϵͳ��Ϣ��ʾ
extern BOOL	SC_SysInfo(LPRPACKET pk);
extern BOOL	SC_BickerNotice( LPRPACKET pk );
extern BOOL SC_ColourNotice( LPRPACKET pk );
extern BOOL	SC_Say(LPRPACKET pk);

extern BOOL	SC_GuildInfo(LPRPACKET pk);
extern BOOL GuildSysInfo;

// Added by knight-gongjian 2004.11.29
// NPC�Ի�
extern BOOL	SC_TalkInfo( LPRPACKET packet );
extern BOOL	SC_FuncInfo( LPRPACKET packet );
extern BOOL	SC_CloseTalk( LPRPACKET packet );
extern BOOL	SC_HelpInfo( LPRPACKET packet );

extern BOOL SC_TradeData( LPRPACKET packet );
extern BOOL SC_TradeAllData( LPRPACKET packet );
extern BOOL	SC_TradeInfo( LPRPACKET packet );
extern BOOL	SC_TradeUpdate( LPRPACKET packet );
extern BOOL	SC_TradeResult( LPRPACKET packet );

// npc ����״̬�л�
extern BOOL	SC_NpcStateChange( LPRPACKET packet );

// �¼�ʵ��״̬�л�
extern BOOL SC_EntityStateChange( LPRPACKET packet );

// ��ɫ����
extern BOOL	SC_CharTradeInfo( LPRPACKET packet );

// �������
extern BOOL	SC_MissionInfo( LPRPACKET packet );
extern BOOL	SC_MisPage( LPRPACKET packet );
extern BOOL	SC_MisLog( LPRPACKET packet );
extern BOOL	SC_MisLogInfo( LPRPACKET packet );
extern BOOL	SC_MisLogClear( LPRPACKET packet );
extern BOOL	SC_MisLogAdd( LPRPACKET packet );
extern BOOL	SC_MisLogState( LPRPACKET packet );

// ���񴥷�������֪ͨ
extern BOOL	SC_TriggerAction( LPRPACKET packet );

// ��Ʒ����
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

// ��ֻ����
extern BOOL	SC_CreateBoat( LPRPACKET packet );
extern BOOL	SC_UpdateBoat( LPRPACKET packet );
extern BOOL	SC_UpdateBoatPart( LPRPACKET packet );
extern BOOL	SC_BoatList( LPRPACKET packet );
extern BOOL SC_BoatInfo( LPRPACKET packet );

// ��ֻ���
//extern BOOL	SC_BoatBagList( LPRPACKET packet );

// ��ɫ�����̯����
extern BOOL SC_StallInfo( LPRPACKET packet );
extern BOOL SC_StallUpdateInfo( LPRPACKET packet );
extern BOOL SC_StallDelGoods( LPRPACKET packet );
extern BOOL SC_StallClose( LPRPACKET packet );
extern BOOL SC_StallSuccess( LPRPACKET packet );

// End by knight.gong

// ��
extern BOOL SC_OpenHairCut( LPRPACKET packet );		// ��������
extern BOOL SC_UpdateHairRes( LPRPACKET packet );	// ���������

// ������ս
extern BOOL	SC_TeamFightAsk(LPRPACKET packet);

// ��������
extern BOOL	SC_BeginItemRepair(LPRPACKET packet);
extern BOOL	SC_ItemRepairAsk(LPRPACKET packet);

// ���߾���/�ϳ�
extern BOOL	SC_ItemForgeAsk(LPRPACKET packet);
extern BOOL	SC_ItemForgeAnswer(LPRPACKET packet);

// ʹ�õ��߳ɹ�
extern BOOL	SC_ItemUseSuc(LPRPACKET packet);

// ��������
extern BOOL	SC_EspeItem(LPRPACKET packet);

// �������
extern BOOL	SC_KitbagCapacity(LPRPACKET packet);

// �������
extern BOOL	SC_SynAppendLook(LPRPACKET packet);

//��ͼ����������
extern BOOL    SC_MapCrash(LPRPACKET packet);

extern BOOL    SC_QueryCha(LPRPACKET packet);
extern BOOL    SC_QueryChaItem(LPRPACKET packet);
extern BOOL    SC_QueryChaPing(LPRPACKET packet);

extern BOOL    SC_QueryRelive(LPRPACKET packet);
extern BOOL    SC_PreMoveTime(LPRPACKET packet);
extern BOOL    SC_MapMask(LPRPACKET packet);
extern BOOL    SC_SynEventInfo(LPRPACKET packet);
extern BOOL    SC_SynSideInfo(LPRPACKET packet);

//��������
extern BOOL    SC_KitbagCheckAnswer(LPRPACKET packet);

//�̳�
extern BOOL	SC_StoreOpenAnswer(LPRPACKET packet);
extern BOOL	SC_StoreListAnswer(LPRPACKET packet);
extern BOOL	SC_StoreBuyAnswer(LPRPACKET packet);
extern BOOL	SC_StoreChangeAnswer(LPRPACKET packet);
extern BOOL	SC_StoreHistory(LPRPACKET packet);
extern BOOL	SC_ActInfo(LPRPACKET packet);
extern BOOL SC_StoreVIP(LPRPACKET packet);

//��������
extern BOOL SC_BlackMarketExchangeData(LPRPACKET packet);
extern BOOL SC_BlackMarketExchangeAsr(LPRPACKET packet);
extern BOOL SC_BlackMarketExchangeUpdate(LPRPACKET packet);
extern BOOL SC_ExchangeData(LPRPACKET packet);

extern BOOL SC_TigerItemID(LPRPACKET packet);

//���ִ�����
extern BOOL SC_VolunteerList(LPRPACKET packet);
extern BOOL SC_VolunteerState(LPRPACKET packet);
extern BOOL SC_VolunteerOpen(LPRPACKET packet);
extern BOOL SC_VolunteerAsk(LPRPACKET packet);

//ͬ����ʱ����
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

//����������
extern BOOL PC_PKSilver(LPRPACKET packet);

extern BOOL SC_LifeSkillShow(LPRPACKET packet);
extern BOOL SC_LifeSkill(LPRPACKET packet);
extern BOOL SC_LifeSkillAsr(LPRPACKET packet);

extern BOOL SC_ChaPlayEffect(LPRPACKET packet);

//	����������
extern	BOOL	SC_DropLockAsr(LPRPACKET pk );
extern  BOOL    SC_UnlockItemAsr(LPRPACKET pk );

//==================================================================================


extern  BOOL    SC_InitGuildHouseObjects(LPRPACKET pk );
extern  BOOL    SC_AddGuildHouseObject(LPRPACKET pk );
extern  BOOL    SC_RemoveGuildHouseObject(int index);
extern  BOOL    SC_RemoveAllGuildHouseObjects();



#endif		//PACKETCMD_H
