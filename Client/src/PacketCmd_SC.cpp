
#include "StdAfx.h"
#include "Character.h"
#include "Scene.h"
#include "GameApp.h"
#include "actor.h"
#include "NetProtocol.h"
#include "PacketCmd.h"
#include "GameAppMsg.h"
#include "CharacterRecord.h"
#include "DrawPointList.h"
#include "Algo.h"
#include "NetChat.h"
#include "NetGuild.h"
#include "CommFunc.h"
#include "ShipSet.h"
#include "GameConfig.h"
#include "GameWG.h"
#include "UIEquipForm.h"
#include "UIDoublePwdForm.h"
#include "uisystemform.h"
#include "uiStoreForm.h"
#include "uiBourseForm.h"
#include "uipksilverform.h"
#include "uicomposeform.h"
#include "UIBreakForm.h"
#include "UIFoundForm.h"
#include "UICookingForm.h"
#include "UISpiritForm.h"
#include "UIFindTeamForm.h"
#include "UINpcTradeForm.h"
#include "UIChat.h"
#include "UIMailForm.h"
#include "UIMiniMapForm.h"
#include "UINumAnswer.h"
#include "UIChurchChallenge.h"

#include "uistartform.h"
#include "UIGuildMgr.h"

#include "UIGuildBankForm.h"
#include "UIGlobalVar.h"

#ifdef _TEST_CLIENT
#include "..\..\TestClient\testclient.h"
#endif


#include "SceneObj.h"
#include "Scene.h"



_DBC_USING


//--------------------------------------------------
// ����Э�鴦��淶
// ���������� 
//      PacketCmd_DoSomething(LPPacket pk)
//
// ������������
//      ��ɫ CCharacter *pCha 
//      ���� CCharacter* pMainCha
//      ��� CSceneItem* pItem
//      ���� CGameScene* pScene
//      APP  CGameApp*   g_pGameApp
//--------------------------------------------------
extern short g_sKeyData;
extern char g_szSendKey[4];
extern char g_szRecvKey[4];
static unsigned long g_ulWorldID = 0;

BOOL SC_GuildQuest(LPRPACKET pk){
	g_stUIGuildMgr.UpdateGuildQuests(pk);
	return true;
}

BOOL SC_UpdateGuildGold(LPRPACKET pk){
	g_stUIGuildBank.UpdateGuildGold(pk.ReadString());
	return true;
}

BOOL SC_ShowStallSearch(LPRPACKET pk){
	uChar l_num		=pk.ReverseReadShort();
	NetMC_LISTGUILD_BEGIN(0);
	for(uChar i =0;i<l_num;++i)
	{
		//char buf[8];
		//sprintf(buf,"%03d>",i+1);
		uLong	 l_id			=i+1;
		cChar	*l_name			=pk.ReadString();//player name
		cChar	*l_motto		=pk.ReadString();//stall name
		cChar	*l_leadername	=pk.ReadString();//location
		uShort	 l_memtotal		=pk.ReadLong(); //count remaining
		__int64	 l_exp	=pk.ReadLong(); //cost each
		NetMC_LISTGUILD(l_id,l_name,l_motto,l_leadername,l_memtotal,l_exp);
	}
	NetMC_LISTGUILD_END();
	return TRUE;
}

BOOL SC_ShowRanking(LPRPACKET pk){
	uChar l_num		=pk.ReverseReadShort();
	NetMC_LISTGUILD_BEGIN(0);
	for(uChar i =0;i<l_num;++i)
	{
		char buf[8];
		sprintf(buf,"%03d>",i+1);
		uLong	 l_id			=i+1;
		cChar	*l_name			=buf;//rank
		cChar	*l_motto		=pk.ReadString();//name
		cChar	*l_leadername	=pk.ReadString();//job
		uShort	 l_memtotal		=pk.ReadShort(); //level
		uLong	 l_explow		=0;
		uLong	 l_exphigh		=0;
		__int64	 l_exp	=l_exphigh *0x100000000 +l_explow;
		NetMC_LISTGUILD(l_id,l_name,l_motto,l_leadername,l_memtotal,l_exp);
	}
	NetMC_LISTGUILD_END();
	return TRUE;
}

BOOL	SC_CHARSTR(LPRPACKET pk)
{T_B
	strcpy(g_NetIF->m_chapstr,pk.ReadString());

	// ����Э���Ƿ����
	#if NET_PROTOCOL_ENCRYPT
	{
		int nNoise = g_sKeyData * int(*(int*)(g_NetIF->m_chapstr + strlen(g_NetIF->m_chapstr) - 4));
		init_Noise( nNoise, g_szSendKey );
		init_Noise( nNoise, g_szRecvKey );

		//lua_getglobal(g_NetIF->g_sLvm, "initNoise");
		//lua_pushnumber(g_NetIF->g_sLvm, nNoise);
		//lua_pushlightuserdata(g_NetIF->g_sLvm, g_szSendKey);
		//lua_call(g_NetIF->g_sLvm, 2, 0);

		//lua_getglobal(g_NetIF->g_sLvm, "initNoise");
		//lua_pushnumber(g_NetIF->g_sLvm, nNoise);
		//lua_pushlightuserdata(g_NetIF->g_sLvm, g_szRecvKey);
		//lua_call(g_NetIF->g_sLvm, 2, 0);
	}
	#endif

	g_NetIF->m_connect.CHAPSTR();
	return TRUE;
T_E}

BOOL	SC_Login(LPRPACKET pk)
{T_B
	uShort l_errno	=pk.ReadShort();

	if(l_errno)	//ʧ��
	{
		cChar *l_errtext=pk.ReadString();
		NetLoginFailure(l_errno);

#ifdef _TEST_CLIENT
        CTestClient* pClient = reinterpret_cast<CTestClient*>( g_NetIF->m_connect.GetDatasock()->GetPointer() );
		pClient->Failure(l_errno);
#endif
	}else			//�ɹ�
	{
		uShort	l_keylen;
		const char *l_sKey	=pk.ReadSequence(l_keylen);

		char l_buf[100];
		int	 l_len;
		
		KCHAPc l_chap(g_NetIF->m_accounts,g_NetIF->m_passwd);

		//if(strlen(g_Config.m_szMD5Pass) == 32) // �����ļ���ֱ������MD5ֵ�����½, ���������ұ���Ĵ���
		//{
		//	l_chap.m_strPwdDig = g_Config.m_szMD5Pass; // ��;�޸����������MD5ֵ
		//}

		l_chap.GetSessionClrKey((void*)l_sKey,l_keylen,l_buf,sizeof(l_buf),l_len);
		memset(g_NetIF->m_passwd,0,sizeof(g_NetIF->m_passwd));
		memset(g_NetIF->m_chapstr,0,sizeof(g_NetIF->m_chapstr));
		
		char	l_chanum	=pk.ReadChar();
		NetChaBehave	l_netcha[10];
		uShort	l_looklen;
		for(int i=0;i<l_chanum;i++)
		{
			if(!pk.ReadChar())
			{
				i--;
				l_chanum--;
				continue;
			}
			l_netcha[i].sCharName	=pk.ReadString();
			l_netcha[i].sJob		=pk.ReadString();
			l_netcha[i].iDegree		=pk.ReadShort();
			l_netcha[i].sLook		=reinterpret_cast<const LOOK *>(pk.ReadSequence(l_looklen));
			if(l_looklen !=sizeof(LOOK))
			{
				i--;l_chanum--;
			}
		}

		char    chPassword  =pk.ReadChar();

		NetLoginSuccess(chPassword, l_chanum,l_netcha);

        // ���������Ϣ
        g_NetIF->_comm_enc = pk.ReadLong();
		DWORD dwFlag = pk.ReadLong(); //0x3214

        g_NetIF->_key_len = l_len;
        memcpy(g_NetIF->_key, l_buf, g_NetIF->_key_len);
        g_NetIF->_enc = true;

        for (int i = 0; i < g_NetIF->_key_len; ++ i)
            {
            lua_getglobal(g_NetIF->g_rLvm, "apple_allot");
            lua_pushnumber(g_NetIF->g_rLvm, g_NetIF->_key[i]);
            lua_pushnumber(g_NetIF->g_rLvm, i);
            lua_call(g_NetIF->g_rLvm, 2, 0);

            lua_getglobal(g_NetIF->g_sLvm, "apple_allot");
            lua_pushnumber(g_NetIF->g_sLvm, g_NetIF->_key[i]);
            lua_pushnumber(g_NetIF->g_sLvm, i);
            lua_call(g_NetIF->g_sLvm, 2, 0);}

#ifdef _TEST_CLIENT
        CTestClient* pClient = reinterpret_cast<CTestClient*>( g_NetIF->m_connect.GetDatasock()->GetPointer() );
		pClient->OnLogin( g_NetIF->_enc, g_NetIF->_comm_enc, g_NetIF->_key, g_NetIF->_key_len );
		pClient->ServerCha( l_chanum, l_netcha );
#endif

		// ������Ҽ���߳�  add by Philip.Wu  2006-07-10
		extern CGameWG g_oGameWG;
		g_oGameWG.SafeTerminateThread();
		g_oGameWG.BeginThread();
		
		/*
		if(canAutoLoginChar){
			CS_BeginPlay( autoLoginChar ); 
			canAutoLoginChar = false;
		}
		*/
		
	}

	return TRUE;
T_E}

//--------------------
// Э��S->C : ѡ���ɫ
//--------------------
BOOL SC_EnterMap(LPRPACKET pk)
{T_B
	g_LogName.Init();

	g_listguild_begin	=false;

    stNetSwitchMap SMapInfo;
    memset(&SMapInfo, 0, sizeof(SMapInfo));
	SMapInfo.sEnterRet = pk.ReadShort();
	if (SMapInfo.sEnterRet != ERR_SUCCESS)
	{
		NetSwitchMap(SMapInfo);
		return FALSE;
	}

	bool bAutoLock   = pk.ReadChar() ? true : false;	// �Զ���״̬
	bool bKitbagLock = pk.ReadChar() ? true : false;	// �Ƿ���������
	g_stUISystem.m_sysProp.m_gameOption.bLockMode = bAutoLock;
	g_stUIEquip.SetIsLock(bKitbagLock);

	SMapInfo.chEnterType = pk.ReadChar();	// �����ͼ���ͣ��μ�CompCommand.h EEnterMapType
	SMapInfo.bIsNewCha = pk.ReadChar() == 1 ? true : false;
	SMapInfo.szMapName = (char *)pk.ReadString();
	SMapInfo.bCanTeam = pk.ReadChar() != 0 ? true : false;
	NetSwitchMap(SMapInfo);
	LG(g_oLangRec.GetString(295), "%s\n", SMapInfo.szMapName);

	
	int IMPs = pk.ReadLong();
	
	g_stUIEquip.UpdateIMP(IMPs);
	
	stNetActorCreate SCreateInfo;
	ReadChaBasePacket(pk, SCreateInfo);
	SCreateInfo.chSeeType = enumENTITY_SEEN_NEW;
	SCreateInfo.chMainCha = 1;
	SCreateInfo.CreateCha();
	g_ulWorldID = SCreateInfo.ulWorldID;

	char* szLogName = g_LogName.GetLogName( SCreateInfo.ulWorldID );

	stNetSkillBag SCurSkill;
	ReadChaSkillBagPacket(pk, SCurSkill, szLogName);
	NetSynSkillBag(SCreateInfo.ulWorldID, &SCurSkill);

#ifdef _TEST_CLIENT
	CTestClient* pClient = reinterpret_cast<CTestClient*>( g_NetIF->m_connect.GetDatasock()->GetPointer() );
	pClient->SetCharID(SCreateInfo.ulWorldID);
	pClient->StartGame(SCreateInfo.SArea.centre.x, SCreateInfo.SArea.centre.y);
	return TRUE;
#endif

	stNetSkillState SCurSState;
	ReadChaSkillStatePacket(pk, SCurSState, szLogName);
	NetSynSkillState(SCreateInfo.ulWorldID, &SCurSState);

	stNetChaAttr SChaAttr;
	ReadChaAttrPacket(pk, SChaAttr, szLogName);
	NetSynAttr(SCreateInfo.ulWorldID, SChaAttr.chType, SChaAttr.sNum, SChaAttr.SEff );

	// ����
	stNetKitbag SKitbag;
	ReadChaKitbagPacket(pk, SKitbag, szLogName);
	SKitbag.chBagType = 0;
	NetChangeKitbag(SCreateInfo.ulWorldID, SKitbag);

#if 0
	stNetKitbag SKitbagTemp;
	ReadChaKitbagPacket(pk, SKitbagTemp, szLogName);
	SKitbagTemp.chBagType = 2;
	NetChangeKitbag(SCreateInfo.ulWorldID, SKitbagTemp);
#endif

	stNetShortCut SShortcut;
	ReadChaShortcutPacket(pk, SShortcut, szLogName);
	NetShortCut(SCreateInfo.ulWorldID, SShortcut);

	// ��ֻ��Ϣ
	char	chBoatNum = pk.ReadChar();
	for (char i = 0; i < chBoatNum; i++)
	{
		ReadChaBasePacket(pk, SCreateInfo);
		SCreateInfo.chSeeType = enumENTITY_SEEN_NEW;
		SCreateInfo.chMainCha = 2;
		SCreateInfo.CreateCha();

		szLogName = g_LogName.GetLogName( SCreateInfo.ulWorldID );

		ReadChaAttrPacket(pk, SChaAttr, szLogName);
		NetSynAttr(SCreateInfo.ulWorldID, SChaAttr.chType, SChaAttr.sNum, SChaAttr.SEff );

		ReadChaKitbagPacket(pk, SKitbag, szLogName);
		NetChangeKitbag(SCreateInfo.ulWorldID, SKitbag);

		ReadChaSkillStatePacket(pk, SCurSState, szLogName);
		NetSynSkillState(SCreateInfo.ulWorldID, &SCurSState);
	}

	stNetChangeCha SChangeCha;
	SChangeCha.ulMainChaID = pk.ReadLong();
	NetActorChangeCha(SCreateInfo.ulWorldID, SChangeCha);

	// ��ʱ��������ʱע�ͣ�!!!��Ҫ����entermap��
#if 0
	stNetKitbag SKitbagTemp;
	ReadChaKitbagPacket(pk, SKitbagTemp, szLogName);
	SKitbagTemp.chBagType = 2;
	NetChangeKitbag(SCreateInfo.ulWorldID, SKitbagTemp);
#endif

//#ifdef _TEST_CLIENT
//    CTestClient* pClient = reinterpret_cast<CTestClient*>( g_NetIF->m_connect.GetDatasock()->GetPointer() );
//    pClient->StartGame(SCreateInfo.SArea.centre.x, SCreateInfo.SArea.centre.y);
//#endif


	// �رշ�����
	//CS_AntiIndulgence_Close();

	return TRUE;
T_E}

BOOL    SC_BeginPlay(LPRPACKET pk)
{T_B
	uShort	l_errno	=pk.ReadShort();
	if(l_errno == ERR_PT_GMISLOG){
		NetShowMapCrash("GM is currently logged on your account.");
		CGameApp::Waiting(false);
	}else{
		NetBeginPlay(l_errno);
	}
	
	return TRUE;
T_E}

BOOL	SC_EndPlay(LPRPACKET pk)
{T_B
	uShort	l_errno	=pk.ReadShort();

	//char    chPassword  =pk.ReadChar();
	//g_Config.m_IsDoublePwd = chPassword ? true : false;

	char	l_chanum	=pk.ReadChar();
	NetChaBehave	l_netcha[10];
	uShort	l_looklen;
	for(int i=0;i<l_chanum;i++)
	{
		if(!pk.ReadChar())
		{
			i--;
			l_chanum--;
			continue;
		}
		l_netcha[i].sCharName	=pk.ReadString();
		l_netcha[i].sJob		=pk.ReadString();
		l_netcha[i].iDegree		=pk.ReadShort();
		l_netcha[i].sLook		=reinterpret_cast<const LOOK *>(pk.ReadSequence(l_looklen));
		if(l_looklen !=sizeof(LOOK))
		{
			i--;l_chanum--;
		}
	}
	NetEndPlay(l_chanum,l_netcha);

#ifdef _TEST_CLIENT
	CTestClient* pClient = reinterpret_cast<CTestClient*>( g_NetIF->m_connect.GetDatasock()->GetPointer() );
	pClient->ServerCha( l_chanum,l_netcha );
#endif

	return TRUE;
T_E}


BOOL	SC_NewCha(LPRPACKET pk)
{T_B
	uShort	l_errno	=pk.ReadShort();
	NetNewCha(l_errno);

#ifdef _TEST_CLIENT
	CTestClient* pClient = reinterpret_cast<CTestClient*>( g_NetIF->m_connect.GetDatasock()->GetPointer() );
	pClient->Failure( l_errno );
#endif
	return TRUE;
T_E}

BOOL	SC_DelCha(LPRPACKET pk)
{T_B
	uShort	l_errno	=pk.ReadShort();
	NetDelCha(l_errno);
	return TRUE;
T_E}

BOOL SC_CreatePassword2(LPRPACKET pk)
{T_B
	uShort	l_errno	=pk.ReadShort();
	NetCreatePassword2(l_errno);
	return TRUE;
T_E}

BOOL SC_UpdatePassword2(LPRPACKET pk)
{T_B
	uShort	l_errno	=pk.ReadShort();
	NetUpdatePassword2(l_errno);
	return TRUE;
T_E}

BOOL PC_REGISTER(LPRPACKET pk){
	CGameApp::Waiting(false);
	char sucess = pk.ReadChar();
	if( g_NetIF->IsConnected() ){
		CS_Disconnect(DS_DISCONN);			
	}
	if(sucess == 1){
		
		//CLoginScene* pkScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());			
		//registerLogin = false;
		//pkScene->LoginFlow();
		g_pGameApp->MsgBox("Account Created.");
	}else{
		g_pGameApp->MsgBox( pk.ReadString() );
	}
	return TRUE;
}

BOOL	PC_Ping(LPRPACKET pk)
{T_B
	WPacket l_wpk = g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_PING);
	g_NetIF->SendPacketMessage(l_wpk);
	return TRUE;
T_E}

BOOL	SC_Ping(LPRPACKET pk)
{T_B
	//dbc::uLong lID = pk.ReadLong();

	//g_NetIF->m_curdelay = GetTickCount() - g_NetIF->dwLatencyTime[lID];

	// xuedong 2004.09.01
	MutexArmor l(g_NetIF->m_mutmov);

	WPacket wpk = g_NetIF->GetWPacket();
	wpk.WriteCmd(CMD_CM_PING);
	wpk.WriteLong(pk.ReadLong());
	wpk.WriteLong(pk.ReadLong());
	wpk.WriteLong(pk.ReadLong());
	wpk.WriteLong(pk.ReadLong());
	wpk.WriteLong(pk.ReadLong());
	g_NetIF->SendPacketMessage(wpk);
	//g_NetIF->m_ulDelayTime[g_NetIF->m_ulCurStatistic] = g_NetIF->m_curdelay;
	//g_NetIF->m_ulCurStatistic ++;
	//if (g_NetIF->m_ulCurStatistic >= 4)
	//	g_NetIF->m_ulCurStatistic = 0;

	l.unlock();
	// end

	if(g_NetIF->m_curdelay > g_NetIF->m_maxdelay) g_NetIF->m_maxdelay = g_NetIF->m_curdelay;

	if(g_NetIF->m_curdelay < g_NetIF->m_mindelay) g_NetIF->m_mindelay = g_NetIF->m_curdelay;

	return TRUE;
T_E}

BOOL	SC_CheckPing(LPRPACKET pk)
{T_B
	WPacket wpk = g_NetIF->GetWPacket();
	wpk.WriteCmd(CMD_CM_CHECK_PING);
	g_NetIF->SendPacketMessage(wpk);

	return TRUE;
T_E}

BOOL	SC_Say(LPRPACKET pk)
{T_B
	uShort	 l_len;
	stNetSay l_netsay;
	l_netsay.m_srcid	=pk.ReadLong();
	l_netsay.m_content	=pk.ReadSequence(l_len);
	DWORD dwColour = pk.ReadLong();
	NetSay(l_netsay,dwColour);

	return TRUE;
T_E}

//--------------------
// Э��S->C : ϵͳ��Ϣ��ʾ
//--------------------
BOOL	SC_SysInfo(LPRPACKET pk)
{T_B
#ifdef _TEST_CLIENT
	return TRUE;
#endif
	uShort	l_retlen;
	stNetSysInfo l_sysinfo;
	l_sysinfo.m_sysinfo	=pk.ReadSequence(l_retlen);
	NetSysInfo(l_sysinfo);
	return TRUE;
T_E}

BOOL GuildSysInfo = false;

BOOL	SC_GuildInfo(LPRPACKET pk)
{T_B
	uShort	l_retlen;
	stNetSysInfo l_sysinfo;
	l_sysinfo.m_sysinfo	=pk.ReadSequence(l_retlen);
	GuildSysInfo = true;
	NetSysInfo(l_sysinfo);
	return TRUE;
T_E}

BOOL SC_PopupNotice(LPRPACKET pk)
{T_B
	uShort	l_retlen;
	cChar *szNotice	= pk.ReadSequence(l_retlen);
	g_pGameApp->MsgBox(szNotice);
	return TRUE;
T_E}

BOOL	SC_BickerNotice( LPRPACKET pk )
{T_B
	char szData[1024];
	strncpy( szData, pk.ReadString(), 1024 - 1 );
	NetBickerInfo( szData );
	return TRUE;
T_E}

BOOL	SC_ColourNotice( LPRPACKET pk )
{T_B
	char szData[1024];
	unsigned int rgb = pk.ReadLong();
	strncpy( szData, pk.ReadString(), 1024 - 1 );
	NetColourInfo( rgb, szData );
	return TRUE;
T_E}

//------------------------------------
// Э��S->C : ������ɫ(���������)����
//------------------------------------
BOOL SC_ChaBeginSee(LPRPACKET pk)
{T_B
	stNetActorCreate SCreateInfo;
	char	chSeeType = pk.ReadChar();

	ReadChaBasePacket(pk, SCreateInfo);
	SCreateInfo.chSeeType = chSeeType;
	SCreateInfo.chMainCha = 0;
	CCharacter *pCha = SCreateInfo.CreateCha();
	if (!pCha)	return FALSE;

	char* szLogName = g_LogName.GetLogName( SCreateInfo.ulWorldID );

	stNetNPCShow	SNpcShow;
	// NPC����״̬��Ϣ
	SNpcShow.byNpcType	   = pk.ReadChar();
    SNpcShow.byNpcState     = pk.ReadChar();
	SNpcShow.SetNpcShow( pCha );
		

	// �ж�����
	switch (pk.ReadShort())
	{
	case	enumPoseLean:
		{
			stNetLeanInfo SLean;
			SLean.chState = pk.ReadChar();
			SLean.lPose = pk.ReadLong();
			SLean.lAngle = pk.ReadLong();
			SLean.lPosX = pk.ReadLong();
			SLean.lPosY = pk.ReadLong();
			SLean.lHeight = pk.ReadLong();
			NetActorLean(SCreateInfo.ulWorldID, SLean);
			break;
		}
	case	enumPoseSeat:
		{
			stNetFace SNetFace;
			SNetFace.sAngle = pk.ReadShort();
			SNetFace.sPose = pk.ReadShort();
			NetFace( SCreateInfo.ulWorldID, SNetFace, enumACTION_SKILL_POSE );
			break;
		}
	default:
		{
			break;
		}
	}

	stNetChaAttr SChaAttr;
	ReadChaAttrPacket(pk, SChaAttr, szLogName);
	NetSynAttr(SCreateInfo.ulWorldID, SChaAttr.chType, SChaAttr.sNum, SChaAttr.SEff );

	stNetSkillState SCurSState;
	ReadChaSkillStatePacket(pk, SCurSState, szLogName);
	NetSynSkillState(SCreateInfo.ulWorldID, &SCurSState);

	return TRUE;
T_E}

//------------------------------------
// Э��S->C : ������ɫ(���������)��ʧ
//------------------------------------
BOOL	SC_ChaEndSee(LPRPACKET pk)
{T_B
#ifdef _TEST_CLIENT
	return TRUE;
#endif
	char	chSeeType = pk.ReadChar(); // �μ� CompCommand.h��EEntitySeenType
	uLong l_id	= pk.ReadLong();
	NetActorDestroy( l_id, chSeeType );
	if (g_stUIStart.targetInfoID == l_id){
		g_stUIStart.RemoveTarget();
	}
	// log
	LG(g_LogName.GetLogName( l_id ), "+++++++++++++Destroy\n");
	//
	return TRUE;
T_E}

BOOL	SC_ItemCreate(LPRPACKET pk)
{T_B
	stNetItemCreate SCreateInfo;
	memset(&SCreateInfo, 0, sizeof(SCreateInfo));
	SCreateInfo.lWorldID = pk.ReadLong();	// world ID
	SCreateInfo.lHandle = pk.ReadLong();
	SCreateInfo.lID = pk.ReadLong();		// ID
	SCreateInfo.SPos.x = pk.ReadLong();		// ��ǰxλ��
	SCreateInfo.SPos.y = pk.ReadLong();		// ��ǰyλ��
	SCreateInfo.sAngle = pk.ReadShort();	// ����
	SCreateInfo.sNum = pk.ReadShort();	// ����
	SCreateInfo.chAppeType = pk.ReadChar();	// ����ԭ�򣨲μ�CompCommand.h EItemAppearType��
	SCreateInfo.lFromID = pk.ReadLong();	// �׳�Դ��ID

	ReadEntEventPacket(pk, SCreateInfo.SEvent);

	CSceneItem	*CItem = NetCreateItem(SCreateInfo);
	if (!CItem)
		return FALSE;

	// log
	LG("SC_Item", "CreateType = %d, WorldID:%u, ItemID = %d, Pos = [%d,%d], SrcID = %u, \n", SCreateInfo.chAppeType, SCreateInfo.lWorldID, SCreateInfo.lID, SCreateInfo.SPos.x, SCreateInfo.SPos.y, SCreateInfo.lFromID);
	//
	return TRUE;
T_E}

BOOL	SC_ItemDestroy(LPRPACKET pk)
{T_B
	unsigned long lID = pk.ReadLong();				//ID

	NetItemDisappear(lID);
	LG("SC_Item", "Item Destroy[%u]\n", lID);
	return TRUE;
T_E}

BOOL SC_AStateBeginSee(LPRPACKET pk)
{T_B
	stNetAreaState	SynAState;

	char	chValidNum = 0;
	SynAState.sAreaX = pk.ReadShort();
	SynAState.sAreaY = pk.ReadShort();
	SynAState.chStateNum = pk.ReadChar();
	for (char j = 0; j < SynAState.chStateNum; j++)
	{
		SynAState.State[chValidNum].chID = pk.ReadChar();
		if (SynAState.State[chValidNum].chID == 0)
			continue;
		SynAState.State[chValidNum].chLv = pk.ReadChar();
		SynAState.State[chValidNum].lWorldID = pk.ReadLong();
		SynAState.State[chValidNum].uchFightID = pk.ReadChar();
		chValidNum++;
	}
	SynAState.chStateNum = chValidNum;

	NetAreaStateBeginSee(&SynAState);

	// log
	char* szLogName = g_LogName.GetMainLogName();

	LG(szLogName, g_oLangRec.GetString(296), SynAState.sAreaX, SynAState.sAreaY, SynAState.chStateNum);
	for (char j = 0; j < SynAState.chStateNum; j++)
		LG(szLogName, "\t%d\t%d\n", SynAState.State[j].chID, SynAState.State[j].chLv);
	LG(szLogName, "\n");
	//

	return TRUE;
T_E}

BOOL SC_AStateEndSee(LPRPACKET pk)
{T_B
	stNetAreaState	SynAState;

	SynAState.sAreaX = pk.ReadShort();
	SynAState.sAreaY = pk.ReadShort();

	NetAreaStateEndSee(&SynAState);

	// log
	LG(g_LogName.GetMainLogName(), g_oLangRec.GetString(296), SynAState.sAreaX, SynAState.sAreaY, 0);
	//

	return TRUE;
T_E}

// Э��S->C : ��ӵ��߽�ɫ
BOOL SC_AddItemCha(LPRPACKET pk)
{T_B
	long	lMainChaID = pk.ReadLong();
	stNetActorCreate SCreateInfo;
	ReadChaBasePacket(pk, SCreateInfo);
	SCreateInfo.chSeeType = enumENTITY_SEEN_NEW;
	SCreateInfo.chMainCha = 2;
	SCreateInfo.CreateCha();

	char *szLogName = g_LogName.GetLogName( SCreateInfo.ulWorldID );

	stNetChaAttr SChaAttr;
	ReadChaAttrPacket(pk, SChaAttr, szLogName);
	NetSynAttr(SCreateInfo.ulWorldID, SChaAttr.chType, SChaAttr.sNum, SChaAttr.SEff );

	stNetKitbag SKitbag;
	ReadChaKitbagPacket(pk, SKitbag, szLogName);
	SKitbag.chBagType = 0;
	NetChangeKitbag(SCreateInfo.ulWorldID, SKitbag);

	stNetSkillState SCurSState;
	ReadChaSkillStatePacket(pk, SCurSState, szLogName);
	NetSynSkillState(SCreateInfo.ulWorldID, &SCurSState);

	return TRUE;
T_E}

// Э��S->C : ɾ�����߽�ɫ
BOOL SC_DelItemCha(LPRPACKET pk)
{T_B
	long	lMainChaID = pk.ReadLong();

	char	chSeeType = enumENTITY_SEEN_NEW;
	uLong l_id	= pk.ReadLong();
	NetActorDestroy( l_id, chSeeType );

	return TRUE;
T_E}

// �������
BOOL SC_Cha_Emotion(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadLong();
	uShort sEmotion = pk.ReadShort();

	NetChaEmotion( l_id, sEmotion );
	LG( g_LogName.GetLogName( l_id ), g_oLangRec.GetString(297), sEmotion );
	return TRUE;
T_E}

// Э��S->C : ��ɫ�ж�ͨ��
BOOL SC_CharacterAction(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadLong();

	char* szLogName = g_LogName.GetLogName( l_id );

	// try
	{
		long lPacketId = 0;
#ifdef defPROTOCOL_HAVE_PACKETID
		lPacketId = pk.ReadLong();
#endif
		LG(szLogName, "$$$PacketID:\t%u\n", lPacketId);

		switch(pk.ReadChar())
		{
		case	enumACTION_MOVE:
			{
				stNetNotiMove SMoveInfo;
				SMoveInfo.sState = pk.ReadShort();
				if (SMoveInfo.sState != enumMSTATE_ON)
					SMoveInfo.sStopState = pk.ReadShort();
				Point *STurnPos;
				uShort ulTurnNum;
				STurnPos = (Point *)pk.ReadSequence(ulTurnNum);
				SMoveInfo.nPointNum = ulTurnNum / sizeof(Point);
				memcpy(SMoveInfo.SPos, STurnPos, ulTurnNum);

				// log
				long lDistX, lDistY, lDist = 0;
				LG(szLogName, "===Recieve(Move):\tTick:[%u]\n", GetTickCount());
				LG(szLogName, "Point:\t%3d\n", SMoveInfo.nPointNum);
				bool isMainCha = g_LogName.IsMainCha( l_id );
				for (int i = 0; i < SMoveInfo.nPointNum; i++)
				{
#ifdef _STATE_DEBUG
					if( isMainCha )	
					{
						g_pGameApp->GetDrawPoints()->Add( SMoveInfo.SPos[i].x, SMoveInfo.SPos[i].y, 0xffff0000, 0.5f );
						if (SMoveInfo.sState && i==SMoveInfo.nPointNum-1)
						{
							g_pGameApp->GetDrawPoints()->Add( SMoveInfo.SPos[i].x, SMoveInfo.SPos[i].y, 0xff000000, 0.3f );
						}
					}
#endif

					if (i > 0)
					{
						lDistX = SMoveInfo.SPos[i].x - SMoveInfo.SPos[i - 1].x;
						lDistY = SMoveInfo.SPos[i].y - SMoveInfo.SPos[i - 1].y;
						lDist = (long)sqrt((double)lDistX * lDistX + lDistY * lDistY);
					}
					LG(szLogName, "\t%d, %d\t%d\n", SMoveInfo.SPos[i].x, SMoveInfo.SPos[i].y, lDist);
				}
				if (SMoveInfo.sState)
					LG(szLogName, "@@@End Move\tState:0x%x\n", SMoveInfo.sState);


#ifdef _TEST_CLIENT
				CTestClient* pClient = reinterpret_cast<CTestClient*>( g_NetIF->m_connect.GetDatasock()->GetPointer() );
				pClient->MoveTo( l_id, SMoveInfo.SPos[SMoveInfo.nPointNum - 1].x, SMoveInfo.SPos[SMoveInfo.nPointNum - 1].y );
				break;
#endif

				if (SMoveInfo.sState & enumMSTATE_CANCEL)
				{
					long	lDist;
					CCharacter	*pCMainCha = CGameScene::GetMainCha();
					if( pCMainCha )
					{
						lDist = (pCMainCha->GetCurX() - SMoveInfo.SPos[SMoveInfo.nPointNum - 1].x) * (pCMainCha->GetCurX() - SMoveInfo.SPos[SMoveInfo.nPointNum - 1].x)
							+ (pCMainCha->GetCurY() - SMoveInfo.SPos[SMoveInfo.nPointNum - 1].y) * (pCMainCha->GetCurY() - SMoveInfo.SPos[SMoveInfo.nPointNum - 1].y);
						LG(szLogName, "++++++++++++++Distance: %d\n", (long)sqrt(double(lDist)));
					}
				}
				LG(szLogName, "\n");
				//

				NetActorMove( l_id, SMoveInfo );

			}
			break;
		case	enumACTION_SKILL_SRC:
			{
				stNetNotiSkillRepresent SSkillInfo;
				SSkillInfo.chCrt = 0;
				SSkillInfo.sStopState = enumEXISTS_WAITING;

				SSkillInfo.byFightID = pk.ReadChar();
				SSkillInfo.sAngle = pk.ReadShort();
				SSkillInfo.sState = pk.ReadShort();
				if (SSkillInfo.sState != enumFSTATE_ON)
					SSkillInfo.sStopState = pk.ReadShort();
				SSkillInfo.lSkillID = pk.ReadLong();
				SSkillInfo.lSkillSpeed = pk.ReadLong();
				char chTarType = pk.ReadChar();
				if (chTarType == 1)
				{
					SSkillInfo.lTargetID = pk.ReadLong();
					SSkillInfo.STargetPoint.x = pk.ReadLong();
					SSkillInfo.STargetPoint.y = pk.ReadLong();
				}
				else if (chTarType == 2)
				{
					SSkillInfo.lTargetID = 0;
					SSkillInfo.STargetPoint.x = pk.ReadLong();
					SSkillInfo.STargetPoint.y = pk.ReadLong();
				}
				SSkillInfo.sExecTime = pk.ReadShort();

				// ͬ������
				short lResNum =  pk.ReadShort();
				if (lResNum > 0)
				{
					SSkillInfo.SEffect.Resize( lResNum );
					for (long i = 0; i < lResNum; i++)
					{
						SSkillInfo.SEffect[i].lAttrID = pk.ReadChar();
						SSkillInfo.SEffect[i].lVal = pk.ReadLong();
						/*if(SSkillInfo.SEffect[i].lAttrID==ATTR_CEXP||SSkillInfo.SEffect[i].lAttrID==ATTR_NLEXP ||SSkillInfo.SEffect[i].lAttrID==ATTR_CLEXP)
							SSkillInfo.SEffect[i].lVal= pk.ReadLongLong();
						else
							SSkillInfo.SEffect[i].lVal = (long)pk.ReadLong();*/
					}
				}

				// ͬ������״̬
				short chSStateNum = pk.ReadChar();
				if ( chSStateNum > 0 )
				{
					SSkillInfo.SState.Resize( chSStateNum );
					for (short chNum = 0; chNum < chSStateNum; chNum++)
					{
						SSkillInfo.SState[chNum].chID = pk.ReadChar();
						SSkillInfo.SState[chNum].chLv = pk.ReadChar();
					}
				}

				// log
				LG(szLogName, "===Recieve(Skill Represent):\tTick:[%u]\n", GetTickCount());
				LG(szLogName, "Angle:\t%d\tFightID:%d\n", SSkillInfo.sAngle, SSkillInfo.byFightID );
				LG(szLogName, "SkillID:\t%u\tSkillSpeed:%d\n", SSkillInfo.lSkillID, SSkillInfo.lSkillSpeed);
					LG(szLogName, "TargetInfo(ID, PosX, PosY):\t%d\n", SSkillInfo.lTargetID, SSkillInfo.STargetPoint.x, SSkillInfo.STargetPoint.y);
				LG(szLogName, "Effect:[ID, Value]\n");
				for (DWORD i = 0; i < SSkillInfo.SEffect.GetCount(); i++)
					LG(szLogName, "\t%d,\t%d\n", SSkillInfo.SEffect[i].lAttrID, SSkillInfo.SEffect[i].lVal);
				if (SSkillInfo.SState.GetCount() > 0)
					LG(szLogName, "Skill State:[ID, LV]\n");
				for (DWORD chNum = 0; chNum < SSkillInfo.SState.GetCount(); chNum++)
					LG(szLogName, "\t%d, %d\n", SSkillInfo.SState[chNum].chID, SSkillInfo.SState[chNum].chLv);
				if (SSkillInfo.sState)
					LG(szLogName, "@@@End Skill\tState:0x%x\n", SSkillInfo.sState);
				LG(szLogName, "\n");
				//

				NetActorSkillRep(l_id, SSkillInfo);
			}
			break;
		case	enumACTION_SKILL_TAR:
			{
				stNetNotiSkillEffect SSkillInfo;

				SSkillInfo.byFightID = pk.ReadChar();
				SSkillInfo.sState = pk.ReadShort();
				SSkillInfo.bDoubleAttack = pk.ReadChar() ? true : false;
				SSkillInfo.bMiss = pk.ReadChar() ? true : false;
				if (SSkillInfo.bBeatBack = pk.ReadChar() ? true : false)
				{
					SSkillInfo.SPos.x = pk.ReadLong();
					SSkillInfo.SPos.y = pk.ReadLong();
				}
				SSkillInfo.lSrcID = pk.ReadLong();
				SSkillInfo.SSrcPos.x = pk.ReadLong();
				SSkillInfo.SSrcPos.y = pk.ReadLong();
				SSkillInfo.lSkillID = pk.ReadLong();
				SSkillInfo.SSkillTPos.x = pk.ReadLong();
				SSkillInfo.SSkillTPos.y = pk.ReadLong();
				SSkillInfo.sExecTime = pk.ReadShort();

				// ͬ������
				pk.ReadChar();
				short lResNum = pk.ReadShort();
				if (lResNum > 0)
				{
					SSkillInfo.SEffect.Resize(lResNum);
					for (long i = 0; i < lResNum; i++)
					{
						SSkillInfo.SEffect[i].lAttrID = pk.ReadChar();
						SSkillInfo.SEffect[i].lVal = pk.ReadLong();
						/*if(SSkillInfo.SEffect[i].lAttrID==ATTR_CEXP||SSkillInfo.SEffect[i].lAttrID==ATTR_NLEXP ||SSkillInfo.SEffect[i].lAttrID==ATTR_CLEXP)
							SSkillInfo.SEffect[i].lVal= pk.ReadLongLong();
						else
							SSkillInfo.SEffect[i].lVal = (long)pk.ReadLong();*/

						char val[32];
						char buff[234];
						sprintf(buff, "ID = %d val = %s\r\n", SSkillInfo.SEffect[i].lAttrID, _i64toa(SSkillInfo.SEffect[i].lVal, val, 10));
						::OutputDebugStr(buff);
					}
				}

				short chSStateNum = 0;
				// ͬ������״̬
				if (pk.ReadChar() == 1)
				{
					pk.ReadLong();
					chSStateNum = pk.ReadChar();
					if (chSStateNum > 0)
					{
						SSkillInfo.SState.Resize( chSStateNum );
						for (short chNum = 0; chNum < chSStateNum; chNum++)
						{
							SSkillInfo.SState[chNum].chID = pk.ReadChar();
							SSkillInfo.SState[chNum].chLv = pk.ReadChar();
							
							pk.ReadLong();
							pk.ReadLong();
						}
					}
				}

				short lSrcResNum = 0;
				short chSrcSStateNum = 0;
				if (pk.ReadChar())
				{
					// ����Դ״̬
					SSkillInfo.sSrcState = pk.ReadShort();
					// ͬ������Դ����
					pk.ReadChar();
					lSrcResNum = pk.ReadShort();
					if ( lSrcResNum > 0)
					{
						SSkillInfo.SSrcEffect.Resize( lSrcResNum );
						for (long i = 0; i < lSrcResNum; i++)
						{
							SSkillInfo.SSrcEffect[i].lAttrID = pk.ReadChar();
							SSkillInfo.SSrcEffect[i].lVal = pk.ReadLong();
							/*if(SSkillInfo.SSrcEffect[i].lAttrID==ATTR_CEXP||SSkillInfo.SSrcEffect[i].lAttrID==ATTR_NLEXP ||SSkillInfo.SSrcEffect[i].lAttrID==ATTR_CLEXP)
								SSkillInfo.SSrcEffect[i].lVal= pk.ReadLongLong();
							else
								SSkillInfo.SSrcEffect[i].lVal = (long)pk.ReadLong();*/
						}
					}
					NetActorSkillEff(l_id, SSkillInfo);
					break; 
					
					if (pk.ReadChar() == 1)
					{
						// ͬ������Դ����״̬
						pk.ReadLong();
						chSrcSStateNum = pk.ReadChar();
						if (chSrcSStateNum > 0)
						{
							SSkillInfo.SSrcState.Resize( chSrcSStateNum );
							for (short chNum = 0; chNum < chSrcSStateNum; chNum++)
							{
								SSkillInfo.SSrcState[chNum].chID = pk.ReadChar();
								SSkillInfo.SSrcState[chNum].chLv = pk.ReadChar();
								pk.ReadLong();
								pk.ReadLong();
							}
						}
					}
				}

				NetActorSkillEff(l_id, SSkillInfo);
			}
			break;
		case	enumACTION_LEAN: // �п�
			{
				stNetLeanInfo SLean;
				SLean.chState = pk.ReadChar();
				if (!SLean.chState)
				{
					SLean.lPose = pk.ReadLong();
					SLean.lAngle = pk.ReadLong();
					SLean.lPosX = pk.ReadLong();
					SLean.lPosY = pk.ReadLong();
					SLean.lHeight = pk.ReadLong();
				}

				// log
				LG(szLogName, "===Recieve(Lean):\tTick:[%u]\n", GetTickCount());
				if (SLean.chState)
					LG(szLogName, "@@@End Lean\tState:%d\n", SLean.chState);
				LG(szLogName, "\n");
				//

				NetActorLean(l_id, SLean);
			}
			break;
		case	enumACTION_ITEM_FAILED:
			{
				stNetSysInfo l_sysinfo;
				short sFailedID = pk.ReadShort();
				l_sysinfo.m_sysinfo = g_GetUseItemFailedInfo(sFailedID);
				NetSysInfo(l_sysinfo);
			}
			break;
		case	enumACTION_LOOK: // ���½�ɫ���
			{
				stNetLookInfo SLookInfo;
				ReadChaLookPacket(pk, SLookInfo, szLogName);
				CCharacter* pCha = CGameApp::GetCurScene()->SearchByID( l_id );

				if(! g_stUIMap.IsPKSilver() || pCha->GetMainType()==enumMainPlayer)
				{
					NetChangeChaPart(l_id, SLookInfo);
				}
			}
			break;
		case	enumACTION_LOOK_ENERGY: // ���½�ɫ��۵�����
			{
				stLookEnergy SLookEnergy;
				ReadChaLookEnergyPacket(pk, SLookEnergy, szLogName);
				NetChangeChaLookEnergy(l_id, SLookEnergy);
			}
			break;
		case	enumACTION_KITBAG: // ���½�ɫ������
			{
				stNetKitbag SKitbag;
				ReadChaKitbagPacket(pk, SKitbag, szLogName);
				SKitbag.chBagType = 0;
				NetChangeKitbag(l_id, SKitbag);
			}
			break;
		case	enumACTION_ITEM_INFO:
			{
			}
			break;
		case	enumACTION_SHORTCUT: // ���¿����
			{
				stNetShortCut SShortcut;
				ReadChaShortcutPacket(pk, SShortcut, szLogName);
				NetShortCut(l_id, SShortcut);
			}
			break;
		case	enumACTION_TEMP:
			{
				stTempChangeChaPart STempChaPart;
				STempChaPart.dwItemID = pk.ReadLong();
				STempChaPart.dwPartID = pk.ReadLong();
				NetTempChangeChaPart(l_id, STempChaPart);
			}
			break;
		case	enumACTION_CHANGE_CHA:
			{
				stNetChangeCha SChangeCha;
				SChangeCha.ulMainChaID = pk.ReadLong();

				NetActorChangeCha(l_id, SChangeCha);
			}
			break;
		case	enumACTION_FACE:
			{
				stNetFace SNetFace;
				SNetFace.sAngle = pk.ReadShort();
				SNetFace.sPose = pk.ReadShort();
                NetFace( l_id, SNetFace, enumACTION_FACE );

				// log
				LG(szLogName, "===Recieve(Face):\tTick:[%u]\n", GetTickCount());
				LG(szLogName, "Angle[%d]\tPose[%d]\n", SNetFace.sAngle, SNetFace.sPose);
				LG(szLogName, "\n");
				//

			}
			break;
		case	enumACTION_SKILL_POSE:
			{
				stNetFace SNetFace;
				SNetFace.sAngle = pk.ReadShort();
				SNetFace.sPose = pk.ReadShort();
                NetFace( l_id, SNetFace, enumACTION_SKILL_POSE );

				// log
				LG(szLogName, "===Recieve(Skill Pos):\tTick:[%u]\n", GetTickCount());
				LG(szLogName, "Angle[%d]\tPose[%d]\n", SNetFace.sAngle, SNetFace.sPose);
				LG(szLogName, "\n");
				//

			}
			break;
		case	enumACTION_PK_CTRL:
			{
				stNetPKCtrl	SNetPKCtrl;
				ReadChaPKPacket(pk, SNetPKCtrl, szLogName);

				SNetPKCtrl.Exec( l_id );
			}
			break;
		case	enumACTION_BANK:	// ������
			{
				stNetKitbag SKitbag;
				ReadChaKitbagPacket(pk, SKitbag, szLogName);
				SKitbag.chBagType = 1;
				NetChangeKitbag(l_id, SKitbag);
			}
			break;
		case	enumACTION_BAGOFHOLDING:	// ������
			{
				stNetKitbag SKitbag;
				ReadChaKitbagPacket(pk, SKitbag, szLogName);
				SKitbag.chBagType = 4;
				NetChangeKitbag(l_id, SKitbag);
			}
			break;
		case	enumACTION_GUILDBANK:	// ������
			{
				stNetKitbag SKitbag;
				ReadChaKitbagPacket(pk, SKitbag, szLogName);
				SKitbag.chBagType = 3;
				NetChangeKitbag(l_id, SKitbag);
			}
			break;
		case	enumACTION_KITBAGTMP:	// ����ʱ����
			{
				stNetKitbag STempKitbag;
				ReadChaKitbagPacket(pk, STempKitbag, szLogName);
				STempKitbag.chBagType = 2;
				NetChangeKitbag(l_id, STempKitbag);
				break;
			}
		case enumACTION_REQUESTGUILDATTR:{
			g_stUIGuildMgr.UpdateGuildAttr(pk);
			break;
		}
		default:
			break;
		}
	}
	//catch (...)
	//{
	//	MessageBox(0, "!!!!!!!!!!!!!!!!!!!!exception: Notice Action", "error", 0);
	//}

	return TRUE;
T_E}

// Э��S->C : ��ɫ������ж�ʧ��
BOOL SC_FailedAction(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadLong();

	char	chType, chReason;
	chType = pk.ReadChar();
	chReason = pk.ReadChar();
    NetFailedAction( chReason );    
	return TRUE;
T_E}

// ͬ����ɫ����
BOOL SC_SynAttribute(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadLong();
	char* szLogName = g_LogName.GetLogName( l_id );

	stNetChaAttr SChaAttr;
	ReadChaAttrPacket(pk, SChaAttr, szLogName);

	//char val[32];
	//char buff[245];
	//sprintf(buff, "NetSynAttr %s\r\n", _i64toa(SChaAttr.SEff[0].lVal , val, 10));
	//OutputDebugStr(buff);

	NetSynAttr(l_id, SChaAttr.chType, SChaAttr.sNum, SChaAttr.SEff );

	return TRUE;
T_E}

// ͬ��������
BOOL SC_SynSkillBag(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadLong();
	char* szLogName = g_LogName.GetLogName( l_id );

	stNetSkillBag SCurSkill;
	if (ReadChaSkillBagPacket(pk, SCurSkill, szLogName))
		NetSynSkillBag(l_id, &SCurSkill);

	return TRUE;
T_E}

// ͬ��������
BOOL SC_SynDefaultSkill(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadLong();
	stNetDefaultSkill	SDefaultSkill;
	SDefaultSkill.sSkillID = pk.ReadShort();
	SDefaultSkill.Exec();
	return TRUE;
T_E}

BOOL SC_SynSkillState(LPRPACKET pk)
{T_B
#ifdef _TEST_CLIENT
	return TRUE;
#endif
	uLong l_id = pk.ReadLong();
	char* szLogName = g_LogName.GetLogName( l_id );

	stNetSkillState SCurSState;
	ReadChaSkillStatePacket(pk, SCurSState, szLogName);

	NetSynSkillState(l_id, &SCurSState);

	return TRUE;
T_E}

BOOL SC_SynTeam(LPRPACKET pk)
{T_B
	stNetTeamState	STeamState;

	STeamState.ulID = pk.ReadLong();
	STeamState.lHP = pk.ReadLong();
    STeamState.lMaxHP = pk.ReadLong();
	STeamState.lSP = pk.ReadLong();
    STeamState.lMaxSP = pk.ReadLong();
	STeamState.lLV = pk.ReadLong();

    LG( "Team", "Refresh, ID[%u], HP[%d], MaxHP[%d], SP[%d], MaxSP[%d], LV[%d]\n", STeamState.ulID, STeamState.lHP, STeamState.lMaxHP, STeamState.lSP, STeamState.lMaxSP, STeamState.lLV );

	stNetLookInfo SLookInfo;
	ReadChaLookPacket(pk, SLookInfo, const_cast<char*>(g_oLangRec.GetString(299)));
	stNetChangeChaPart	&SFace = SLookInfo.SLook;
	STeamState.SFace.sTypeID = SFace.sTypeID;
	STeamState.SFace.sHairID = SFace.sHairID;
	for (int i = 0; i < enumEQUIP_NUM; i++)
	{
		STeamState.SFace.SLink[i].sID = SFace.SLink[i].sID;
		STeamState.SFace.SLink[i].sNum = SFace.SLink[i].sNum;
		STeamState.SFace.SLink[i].chForgeLv = SFace.SLink[i].chForgeLv;
		STeamState.SFace.SLink[i].lFuseID = SFace.SLink[i].lDBParam[1]>>16;
	}

	NetSynTeam(&STeamState);

	return true;
T_E}

BOOL SC_SynTLeaderID(LPRPACKET pk)
{T_B
	long	lID = pk.ReadLong();
	long	lLeaderID = pk.ReadLong();

	NetChaTLeaderID(lID, lLeaderID);

	// log
	LG(g_LogName.GetLogName( lID ), g_oLangRec.GetString(300), lLeaderID, lID);
	//

	return TRUE;
T_E}

BOOL SC_HelpInfo( LPRPACKET packet )
{T_B
	NET_HELPINFO Info;
	memset( &Info, 0, sizeof(NET_HELPINFO) );

	Info.byType = packet.ReadChar();
	if( Info.byType == mission::MIS_HELP_DESP || Info.byType == mission::MIS_HELP_IMAGE || 
		Info.byType == mission::MIS_HELP_BICKER )
	{
		const char* pszDesp = packet.ReadString();
		strncpy( Info.szDesp, pszDesp, ROLE_MAXNUM_DESPSIZE - 1 );
	}
	else if( Info.byType == mission::MIS_HELP_SOUND )
	{
		Info.sID = packet.ReadShort();
	}
	else
	{
		return FALSE;
	}

	NetShowHelp( Info );
	return TRUE;
T_E}

// NPC �Ի���Ϣ����
BOOL SC_TalkInfo( LPRPACKET packet )
{T_B
	DWORD dwNpcID = packet.ReadLong();
	BYTE byCmd = packet.ReadChar();
	USHORT sLen = 0;
	const char* pszDesp = packet.ReadString();
	if( pszDesp == NULL ) return FALSE;
	NetShowTalk( pszDesp, byCmd, dwNpcID  );
	return TRUE;
T_E}

BOOL SC_FuncInfo( LPRPACKET packet )
{T_B
	NET_FUNCPAGE FuncPage;
	memset( &FuncPage, 0, sizeof(NET_FUNCPAGE) );

	DWORD dwNpcID = packet.ReadLong();
	BYTE byPage  = packet.ReadChar();
	strncpy( FuncPage.szTalk, packet.ReadString(), ROLE_MAXNUM_DESPSIZE - 1 );
	BYTE byCount = packet.ReadChar();
	
	if( byCount > ROLE_MAXNUM_FUNCITEM ) byCount = ROLE_MAXNUM_FUNCITEM;
	for( int i = 0; i < byCount; i++ )
	{
		const char* pszFunc = packet.ReadString();
		strncpy( FuncPage.FuncItem[i].szFunc, pszFunc, ROLE_MAXNUM_FUNCITEMSIZE - 1 );
	}

	BYTE byMisCount = packet.ReadChar();
	if( byMisCount > ROLE_MAXNUM_CAPACITY ) {	byMisCount = ROLE_MAXNUM_CAPACITY; }

	for( int i = 0; i < byMisCount; i++ )
	{
		const char* pszMis = packet.ReadString();
		strncpy( FuncPage.MisItem[i].szMis, pszMis, ROLE_MAXNUM_FUNCITEMSIZE - 1 );
		FuncPage.MisItem[i].byState = packet.ReadChar();
	}

	NetShowFunction( byPage, byCount, byMisCount, FuncPage, dwNpcID );
	return TRUE;
T_E}

BOOL SC_CloseTalk( LPRPACKET packet )
{T_B
	DWORD dwNpcID = packet.ReadLong();
	NetCloseTalk( dwNpcID );
	return TRUE;
T_E}

BOOL SC_TradeData( LPRPACKET packet )
{T_B
	DWORD dwNpcID = packet.ReadLong();
	BYTE byPage = packet.ReadChar();
	BYTE byIndex = packet.ReadChar();
	USHORT sItemID = packet.ReadShort();
	USHORT sCount = packet.ReadShort();
	DWORD dwPrice = packet.ReadLong();

	NetUpdateTradeData( dwNpcID, byPage, byIndex, sItemID, sCount, dwPrice );
	return TRUE;
T_E}

BOOL SC_TradeAllData( LPRPACKET packet )
{T_B
	DWORD dwNpcID = packet.ReadLong();
	BYTE  byType = packet.ReadChar();
	DWORD dwParam = packet.ReadLong();
	BYTE  byCount = packet.ReadChar();

	NET_TRADEINFO Info;
	for( BYTE i = 0; i < byCount; i++ )
	{
		BYTE byItemType = packet.ReadChar();
		switch( byItemType )
		{
		case mission::TI_WEAPON:
		case mission::TI_DEFENCE:
		case mission::TI_OTHER:		
		case mission::TI_SYNTHESIS:		
			{			
				Info.TradePage[byItemType].byCount = packet.ReadChar();
				if( Info.TradePage[byItemType].byCount > ROLE_MAXNUM_TRADEITEM )
					Info.TradePage[byItemType].byCount = ROLE_MAXNUM_TRADEITEM;

				for( BYTE n = 0; n < Info.TradePage[byItemType].byCount; n++ )
				{
					Info.TradePage[byItemType].sItemID[n] = packet.ReadShort();
					if( byType == mission::TRADE_GOODS )
					{
						Info.TradePage[byItemType].sCount[n] = packet.ReadShort();
						Info.TradePage[byItemType].dwPrice[n] = packet.ReadLong();
						Info.TradePage[byItemType].byLevel[n] = packet.ReadChar();
					}
				}
			}
			break;
		default:
			break;
		}
	}
	NetUpdateTradeAllData( Info, byType, dwNpcID, dwParam );
	return TRUE;
T_E}

BOOL SC_TradeInfo( LPRPACKET packet )
{T_B
	DWORD dwNpcID = packet.ReadLong();
	BYTE  byType = packet.ReadChar();
	DWORD dwParam = packet.ReadLong();
	BYTE  byCount = packet.ReadChar();

	NET_TRADEINFO Info;
	for( BYTE i = 0; i < byCount; i++ )
	{
		BYTE byItemType = packet.ReadChar();
		switch( byItemType )
		{
		case mission::TI_WEAPON:
		case mission::TI_DEFENCE:
		case mission::TI_OTHER:		
		case mission::TI_SYNTHESIS:		
			{			
				Info.TradePage[byItemType].byCount = packet.ReadChar();
				if( Info.TradePage[byItemType].byCount > ROLE_MAXNUM_TRADEITEM )
					Info.TradePage[byItemType].byCount = ROLE_MAXNUM_TRADEITEM;

				for( BYTE n = 0; n < Info.TradePage[byItemType].byCount; n++ )
				{
					Info.TradePage[byItemType].sItemID[n] = packet.ReadShort();
					if( byType == mission::TRADE_GOODS )
					{
						Info.TradePage[byItemType].sCount[n] = packet.ReadShort();
						Info.TradePage[byItemType].dwPrice[n] = packet.ReadLong();
						Info.TradePage[byItemType].byLevel[n] = packet.ReadChar();
					}
				}
			}
			break;
		default:
			break;
		}
	}
	NetShowTrade( Info, byType, dwNpcID, dwParam );
	return TRUE;
T_E}

BOOL SC_TradeUpdate( LPRPACKET packet )
{T_B
	//���н��׸���,ֻ���ڴ򿪺��н��׽�������²Ÿ���

	DWORD dwNpcID = packet.ReadLong();
	BYTE  byType = packet.ReadChar();
	DWORD dwParam = packet.ReadLong();
	BYTE  byCount = packet.ReadChar();

	NET_TRADEINFO Info;
	for( BYTE i = 0; i < byCount; i++ )
	{
		BYTE byItemType = packet.ReadChar();
		switch( byItemType )
		{
		case mission::TI_WEAPON:
		case mission::TI_DEFENCE:
		case mission::TI_OTHER:		
		case mission::TI_SYNTHESIS:		
			{			
				Info.TradePage[byItemType].byCount = packet.ReadChar();
				if( Info.TradePage[byItemType].byCount > ROLE_MAXNUM_TRADEITEM )
					Info.TradePage[byItemType].byCount = ROLE_MAXNUM_TRADEITEM;

				for( BYTE n = 0; n < Info.TradePage[byItemType].byCount; n++ )
				{
					Info.TradePage[byItemType].sItemID[n] = packet.ReadShort();
					if( byType == mission::TRADE_GOODS )
					{
						Info.TradePage[byItemType].sCount[n] = packet.ReadShort();
						Info.TradePage[byItemType].dwPrice[n] = packet.ReadLong();
						Info.TradePage[byItemType].byLevel[n] = packet.ReadChar();
					}
				}
			}
			break;
		default:
			break;
		}
	}

	if(g_stUINpcTrade.GetIsShow())
	{
		NetShowTrade( Info, byType, dwNpcID, dwParam );
	}

	return TRUE;
T_E}

BOOL SC_TradeResult( LPRPACKET packet )
{T_B
	BYTE byType = packet.ReadChar();
	BYTE byIndex = packet.ReadChar();
	BYTE byCount = packet.ReadChar();
	USHORT sItemID = packet.ReadShort();
	DWORD  dwMoney  = packet.ReadLong();
	LG("trade", g_oLangRec.GetString(301), byType, byIndex, byCount, sItemID, dwMoney);
	  NetTradeResult( byType, byIndex, byCount, sItemID, dwMoney );
	LG("trade", g_oLangRec.GetString(302));
	return TRUE;
T_E}

BOOL SC_CharTradeInfo( LPRPACKET packet )
{T_B
	USHORT usCmd = packet.ReadShort();
	switch( usCmd )
	{
	case CMD_MC_CHARTRADE_REQUEST:
		{
			BYTE byType = packet.ReadChar();
			DWORD dwRequestID = packet.ReadLong();
			NetShowCharTradeRequest( byType, dwRequestID );
		}
		break;
	case CMD_MC_CHARTRADE_CANCEL:
		{
			DWORD dwCharID = packet.ReadLong();
			NetCancelCharTrade( dwCharID );
		}
		break;
	case CMD_MC_CHARTRADE_MONEY:
		{
			DWORD dwCharID = packet.ReadLong();
			DWORD dwMoney  = packet.ReadLong();
			int currency = packet.ReadChar();
	
			if(currency==0){
				NetTradeShowMoney( dwCharID, dwMoney );
			}else if(currency==1){
				NetTradeShowIMP( dwCharID, dwMoney );
			}
		}
		break;
	case CMD_MC_CHARTRADE_ITEM:
		{
			DWORD dwRequestID = packet.ReadLong();
			BYTE byOpType = packet.ReadChar();
			if( byOpType == mission::TRADE_DRAGTO_ITEM )
			{
				BYTE byItemIndex = packet.ReadChar();
				BYTE byIndex = packet.ReadChar();
				BYTE byCount = packet.ReadChar();
				
				// ��Ʒʵ������
				NET_CHARTRADE_ITEMDATA Data;
				memset( &Data, 0, sizeof(NET_CHARTRADE_ITEMDATA) );

				NetTradeAddItem( dwRequestID, byOpType, 0, byIndex, byCount, byItemIndex, Data );
			}
			else if( byOpType == mission::TRADE_DRAGTO_TRADE )
			{
				USHORT sItemID = packet.ReadShort();
				BYTE byItemIndex = packet.ReadChar();
				BYTE byIndex = packet.ReadChar();				
				BYTE byCount = packet.ReadChar();				
				USHORT sType = packet.ReadShort();
				
				if( sType == enumItemTypeBoat )
				{
					NET_CHARTRADE_BOATDATA Data;
					memset( &Data, 0, sizeof(NET_CHARTRADE_BOATDATA) );

					if( packet.ReadChar() == 0 )
					{
						// ��Ϣ����
					}
					else
					{
						strncpy( Data.szName, packet.ReadString(), BOAT_MAXSIZE_BOATNAME - 1 );
						Data.sBoatID = packet.ReadShort();
						Data.sLevel = packet.ReadShort();
						Data.dwExp = packet.ReadLong();
						Data.dwHp = packet.ReadLong();
						Data.dwMaxHp = packet.ReadLong();
						Data.dwSp = packet.ReadLong();
						Data.dwMaxSp = packet.ReadLong();
						Data.dwMinAttack = packet.ReadLong();
						Data.dwMaxAttack = packet.ReadLong();
						Data.dwDef = packet.ReadLong();
						Data.dwSpeed = packet.ReadLong();
						Data.dwShootSpeed = packet.ReadLong();
						Data.byHasItem = packet.ReadChar();
						Data.byCapacity = packet.ReadChar();
						Data.dwPrice = packet.ReadLong();
						NetTradeAddBoat( dwRequestID, byOpType, sItemID, byIndex, byCount, byItemIndex, Data );
					}
				}
				else
				{
					// ��Ʒʵ������
					NET_CHARTRADE_ITEMDATA Data;
					memset( &Data, 0, sizeof(NET_CHARTRADE_ITEMDATA) );

					Data.sEndure[0] = packet.ReadShort();
					Data.sEndure[1] = packet.ReadShort();
					Data.sEnergy[0] = packet.ReadShort();
					Data.sEnergy[1] = packet.ReadShort();
					Data.byForgeLv = packet.ReadChar();
					Data.bValid = packet.ReadChar() != 0 ? true : false;
					Data.lDBParam[enumITEMDBP_FORGE] = packet.ReadLong();
					Data.lDBParam[enumITEMDBP_INST_ID] = packet.ReadLong();

					Data.byHasAttr = packet.ReadChar();
					if( Data.byHasAttr )
					{
						for( int i = 0; i < defITEM_INSTANCE_ATTR_NUM; i++ )
						{
							Data.sInstAttr[i][0] = packet.ReadShort();
							Data.sInstAttr[i][1] = packet.ReadShort();
						}
					}

					NetTradeAddItem( dwRequestID, byOpType, sItemID, byIndex, byCount, byItemIndex, Data );
				}
			}
			else
			{
				MessageBox( NULL, g_oLangRec.GetString(303), g_oLangRec.GetString(25), MB_OK );
				return FALSE;
			}
			
		}
		break;
	case CMD_MC_CHARTRADE_PAGE:
		{
			BYTE byType = packet.ReadChar();
			DWORD dwAcceptID = packet.ReadLong();
			DWORD dwRequestID = packet.ReadLong();
			NetShowCharTradeInfo( byType, dwAcceptID, dwRequestID );
		}
		break;
	case CMD_MC_CHARTRADE_VALIDATEDATA:
		{
			DWORD dwCharID = packet.ReadLong();
			NetValidateTradeData( dwCharID );
		}
		break;
	case CMD_MC_CHARTRADE_VALIDATE:
		{
			DWORD dwCharID = packet.ReadLong();
			NetValidateTrade( dwCharID );
		}
		break;
	case CMD_MC_CHARTRADE_RESULT:
		{
			BYTE byResult = packet.ReadChar();
			if( byResult == mission::TRADE_SUCCESS )
			{
				NetTradeSuccess();
			}
			else
			{
				NetTradeFailed();
			}
			
		}
		break;
	default:
		return FALSE;
		break;
	}
	return TRUE;
T_E}

BOOL SC_MissionInfo( LPRPACKET packet )
{T_B
	DWORD dwNpcID = packet.ReadLong();
	NET_MISSIONLIST list;
	memset( &list, 0, sizeof(NET_MISSIONLIST) );

	list.byListType = packet.ReadChar();
	list.byPrev = packet.ReadChar();
	list.byNext = packet.ReadChar();
	list.byPrevCmd = packet.ReadChar();
	list.byNextCmd = packet.ReadChar();
	list.byItemCount = packet.ReadChar();
	
	if( list.byItemCount > ROLE_MAXNUM_FUNCITEM ) list.byItemCount = ROLE_MAXNUM_FUNCITEM;
	for( int i = 0; i < list.byItemCount; i++ )
	{
		USHORT sLen = 0;
		const char* pszFunc = packet.ReadString();
		strncpy( list.FuncPage.FuncItem[i].szFunc, pszFunc, 32 );
	}

	NetShowMissionList( dwNpcID, list );
	return TRUE;
T_E}

BOOL SC_MisPage( LPRPACKET packet )
{T_B
	BYTE byCmd = packet.ReadChar();
	DWORD dwNpcID = packet.ReadLong();
	NET_MISPAGE page;
	memset( &page, 0,sizeof(NET_MISPAGE) );
	
	// ��������
	const char* pszName = packet.ReadString();
	strncpy( page.szName, pszName, ROLE_MAXSIZE_MISNAME - 1 );

	switch( byCmd )
	{
	case ROLE_MIS_BTNACCEPT:
	case ROLE_MIS_BTNDELIVERY:
	case ROLE_MIS_BTNPENDING:
		{
			// ����������Ϣ
			page.byNeedNum = packet.ReadChar();
			for( int i = 0; i < page.byNeedNum; i++ )
			{
				page.MisNeed[i].byType = packet.ReadChar();
				if( page.MisNeed[i].byType == mission::MIS_NEED_ITEM || page.MisNeed[i].byType == mission::MIS_NEED_KILL )
				{
					page.MisNeed[i].wParam1 = packet.ReadShort();
					page.MisNeed[i].wParam2 = packet.ReadShort();
					page.MisNeed[i].wParam3 = packet.ReadChar();
				}
				else if( page.MisNeed[i].byType == mission::MIS_NEED_DESP )
				{
					const char* pszTemp = packet.ReadString();
					strncpy( page.MisNeed[i].szNeed, pszTemp, ROLE_MAXNUM_NEEDDESPSIZE - 1 );
				}
				else
				{
					// δ֪����������
					LG( "mission_error", g_oLangRec.GetString(304));
					return FALSE;
				}
			}
			
			// ��������Ϣ
			page.byPrizeSelType = packet.ReadChar();
			page.byPrizeNum = packet.ReadChar();
			for( int i = 0; i < page.byPrizeNum; i++ )
			{
				page.MisPrize[i].byType = packet.ReadChar();
				page.MisPrize[i].wParam1 = packet.ReadShort();
				page.MisPrize[i].wParam2 = packet.ReadShort();
			}

			// ����������Ϣ
			const char* pszDesp = packet.ReadString();
			strncpy( page.szDesp, pszDesp, ROLE_MAXNUM_DESPSIZE - 1 );
		}
		break;
	default:
		return FALSE;
	}

	NetShowMisPage( dwNpcID, byCmd, page );
	return TRUE;
T_E}

BOOL SC_MisLog( LPRPACKET packet )
{T_B
#ifdef _TEST_CLIENT
	return TRUE;
#endif
	NET_MISLOG_LIST LogList;
	memset( &LogList, 0, sizeof(NET_MISLOG_LIST) );

	LogList.byNumLog = packet.ReadChar();
	for( int i = 0; i < LogList.byNumLog; i++ )
	{		
		LogList.MisLog[i].wMisID  = packet.ReadShort();
		LogList.MisLog[i].byState = packet.ReadChar();
	}

	NetMisLogList( LogList );
	return TRUE;
T_E}

BOOL SC_MisLogInfo( LPRPACKET packet )
{T_B
	NET_MISPAGE page;
	memset( &page, 0,sizeof(NET_MISPAGE) );

	// ���������Ϣ
	WORD wMisID = packet.ReadShort();	
	const char* pszName = packet.ReadString();
	strncpy( page.szName, pszName, ROLE_MAXSIZE_MISNAME - 1 );

	// ����������Ϣ
	page.byNeedNum = packet.ReadChar();
	for( int i = 0; i < page.byNeedNum; i++ )
	{
		page.MisNeed[i].byType = packet.ReadChar();
		if( page.MisNeed[i].byType == mission::MIS_NEED_ITEM || page.MisNeed[i].byType == mission::MIS_NEED_KILL )
		{
			page.MisNeed[i].wParam1 = packet.ReadShort();
			page.MisNeed[i].wParam2 = packet.ReadShort();
			page.MisNeed[i].wParam3 = packet.ReadChar();
		}
		else if( page.MisNeed[i].byType == mission::MIS_NEED_DESP )
		{
			const char* pszTemp = packet.ReadString();
			strncpy( page.MisNeed[i].szNeed, pszTemp, ROLE_MAXNUM_NEEDDESPSIZE - 1 );
		}
		else
		{
			// δ֪����������
			LG( "mission_error", g_oLangRec.GetString(304));
			return FALSE;
		}
	}

	// ��������Ϣ
	page.byPrizeSelType = packet.ReadChar();
	page.byPrizeNum = packet.ReadChar();
	for( int i = 0; i < page.byPrizeNum; i++ )
	{
		page.MisPrize[i].byType = packet.ReadChar();
		page.MisPrize[i].wParam1 = packet.ReadShort();
		page.MisPrize[i].wParam2 = packet.ReadShort();
	}

	// ����������Ϣ
	const char* pszDesp = packet.ReadString();
	strncpy( page.szDesp, pszDesp, ROLE_MAXNUM_DESPSIZE - 1 );

	NetShowMisLog( wMisID, page );
	return TRUE;
T_E}

BOOL SC_MisLogClear( LPRPACKET packet )
{T_B
	WORD wMisID  = packet.ReadShort();

	NetMisLogClear( wMisID );
	return TRUE;
T_E}

BOOL SC_MisLogAdd( LPRPACKET packet )
{T_B
	WORD wMisID  = packet.ReadShort();
	BYTE byState = packet.ReadChar();

	NetMisLogAdd( wMisID, byState );
	return TRUE;
T_E}

BOOL SC_MisLogState( LPRPACKET packet )
{T_B
	WORD wID = packet.ReadShort();
	BYTE byState = packet.ReadChar();

	NetMisLogState( wID, byState );
	return TRUE;
T_E}

BOOL SC_TriggerAction( LPRPACKET packet )
{T_B
    stNetNpcMission info;
    info.byType = packet.ReadChar();	
	info.sID = packet.ReadShort();		// ���ݻ��������ID
	info.sNum = packet.ReadShort();		// ��Ҫ�ݻ����������
	info.sCount = packet.ReadShort();	// ����ɼ���
    NetTriggerAction( info );
	return TRUE;
T_E}

BOOL SC_NpcStateChange( LPRPACKET packet )
{T_B
	DWORD dwNpcID = packet.ReadLong();
	BYTE  byState = packet.ReadChar();
	NetNpcStateChange( dwNpcID, byState );
	return TRUE;
T_E}

BOOL SC_EntityStateChange( LPRPACKET packet )
{T_B
	DWORD dwEntityID = packet.ReadLong();
	BYTE byState = packet.ReadChar();
	NetEntityStateChange( dwEntityID, byState );
	return TRUE;
T_E}

BOOL SC_Forge( LPRPACKET packet )
{T_B
	NetShowForge();
	return TRUE;
T_E}


BOOL SC_Unite( LPRPACKET packet )
{T_B
	NetShowUnite();
	return TRUE;
T_E}

BOOL SC_Milling( LPRPACKET packet )
{T_B
	NetShowMilling();
	return TRUE;
T_E}

BOOL SC_Fusion( LPRPACKET packet )
{T_B
	NetShowFusion();
	return TRUE;
T_E}

BOOL SC_Upgrade( LPRPACKET packet )
{T_B
	NetShowUpgrade();
	return TRUE;
T_E}

BOOL SC_EidolonMetempsychosis( LPRPACKET packet )
{T_B
	//NetShowEidolonMetempsychosis();
	NetShowEidolonFusion();
	return TRUE;
T_E}

BOOL SC_Eidolon_Fusion(LPRPACKET packet)
{T_B
	NetShowEidolonFusion();
	return TRUE;
T_E}

BOOL SC_Purify(LPRPACKET packet)
{T_B
	NetShowPurify();
	return TRUE;
T_E}

BOOL SC_Fix(LPRPACKET packet)
{T_B
	NetShowRepairOven();
	return TRUE;
T_E}

BOOL SC_GMSend(LPRPACKET packet)
{T_B
	g_stUIMail.ShowQuestionForm();
	return TRUE;
T_E}

BOOL SC_GMRecv(LPRPACKET packet)
{T_B
	DWORD dwNpcID = packet.ReadLong();
	CS_GMRecv(dwNpcID);
	return TRUE;
T_E}

BOOL SC_GetStone(LPRPACKET packet)
{T_B
	NetShowGetStone();
	return TRUE;
T_E}

BOOL SC_Energy(LPRPACKET packet)
{T_B
	NetShowEnergy();
	return TRUE;
T_E}

BOOL SC_Tiger(LPRPACKET packet)
{T_B
	NetShowTiger();
	return TRUE;
T_E}

BOOL SC_CreateBoat( LPRPACKET packet )
{T_B
	DWORD dwBoatID = packet.ReadLong();
	xShipBuildInfo Info;
	memset( &Info, 0, sizeof(xShipBuildInfo) );

	char szBoat[BOAT_MAXSIZE_NAME] = {0};	// �����Զ���
	strncpy( szBoat, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	strncpy( Info.szName, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	strncpy( Info.szDesp, packet.ReadString(), BOAT_MAXSIZE_DESP - 1 );
	strncpy( Info.szBerth, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	Info.byIsUpdate = packet.ReadChar();
	Info.sPosID = packet.ReadShort();
	Info.dwBody = packet.ReadLong();
	strncpy( Info.szBody, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	
	if( Info.byIsUpdate )
	{
		Info.byHeader = packet.ReadChar();
		Info.dwHeader = packet.ReadLong();
		strncpy( Info.szHeader, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );

		Info.byEngine = packet.ReadChar();
		Info.dwEngine = packet.ReadLong();
		strncpy( Info.szEngine, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
		for( int i = 0; i < BOAT_MAXNUM_MOTOR; i++ )
		{
			Info.dwMotor[i] = packet.ReadLong();
		}
	}
	Info.byCannon = packet.ReadChar();
	strncpy( Info.szCannon, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );

	Info.byEquipment = packet.ReadChar();
	strncpy( Info.szEquipment, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );

	Info.dwMoney = packet.ReadLong();
	Info.dwMinAttack = packet.ReadLong();
	Info.dwMaxAttack = packet.ReadLong();
	Info.dwCurEndure = packet.ReadLong();
	Info.dwMaxEndure = packet.ReadLong();
	Info.dwSpeed  = packet.ReadLong();
	Info.dwDistance = packet.ReadLong();
	Info.dwDefence = packet.ReadLong();
	Info.dwCurSupply = packet.ReadLong();
	Info.dwMaxSupply = packet.ReadLong();
	Info.dwConsume = packet.ReadLong();
	Info.dwAttackTime = packet.ReadLong();
	Info.sCapacity = packet.ReadShort();

	NetCreateBoat( Info );
	return TRUE;
T_E}

BOOL SC_UpdateBoat( LPRPACKET packet )
{T_B
	DWORD dwBoatID = packet.ReadLong();
	xShipBuildInfo Info;
	memset( &Info, 0, sizeof(xShipBuildInfo) );
	
	char szBoat[BOAT_MAXSIZE_NAME] = {0};	// �����Զ���
	strncpy( szBoat, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	strncpy( Info.szName, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	strncpy( Info.szDesp, packet.ReadString(), BOAT_MAXSIZE_DESP - 1 );
	strncpy( Info.szBerth, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	Info.byIsUpdate = packet.ReadChar();
	Info.sPosID = packet.ReadShort();
	Info.dwBody = packet.ReadLong();
	strncpy( Info.szBody, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	
	if( Info.byIsUpdate )
	{
		Info.byHeader = packet.ReadChar();
		Info.dwHeader = packet.ReadLong();
		strncpy( Info.szHeader, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );

		Info.byEngine = packet.ReadChar();
		Info.dwEngine = packet.ReadLong();
		strncpy( Info.szEngine, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
		for( int i = 0; i < BOAT_MAXNUM_MOTOR; i++ )
		{
			Info.dwMotor[i] = packet.ReadLong();
		}
	}
	Info.byCannon = packet.ReadChar();
	strncpy( Info.szCannon, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );

	Info.byEquipment = packet.ReadChar();
	strncpy( Info.szEquipment, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );

	Info.dwMoney = packet.ReadLong();
	Info.dwMinAttack = packet.ReadLong();
	Info.dwMaxAttack = packet.ReadLong();
	Info.dwCurEndure = packet.ReadLong();
	Info.dwMaxEndure = packet.ReadLong();
	Info.dwSpeed  = packet.ReadLong();
	Info.dwDistance = packet.ReadLong();
	Info.dwDefence = packet.ReadLong();
	Info.dwCurSupply = packet.ReadLong();
	Info.dwMaxSupply = packet.ReadLong();
	Info.dwConsume = packet.ReadLong();
	Info.dwAttackTime = packet.ReadLong();
	Info.sCapacity = packet.ReadShort();

	NetUpdateBoat( Info );
	return TRUE;
T_E}

BOOL SC_BoatInfo( LPRPACKET packet )
{T_B
	DWORD dwBoatID = packet.ReadLong();
	xShipBuildInfo Info;
	memset( &Info, 0, sizeof(xShipBuildInfo) );

	char szBoat[BOAT_MAXSIZE_NAME] = {0};	// �����Զ���
	strncpy( szBoat, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	strncpy( Info.szName, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	strncpy( Info.szDesp, packet.ReadString(), BOAT_MAXSIZE_DESP - 1 );
	strncpy( Info.szBerth, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	Info.byIsUpdate = packet.ReadChar();
	Info.sPosID = packet.ReadShort();
	Info.dwBody = packet.ReadLong();
	strncpy( Info.szBody, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	
	if( Info.byIsUpdate )
	{
		Info.byHeader = packet.ReadChar();
		Info.dwHeader = packet.ReadLong();
		strncpy( Info.szHeader, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );

		Info.byEngine = packet.ReadChar();
		Info.dwEngine = packet.ReadLong();
		strncpy( Info.szEngine, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
		for( int i = 0; i < BOAT_MAXNUM_MOTOR; i++ )
		{
			Info.dwMotor[i] = packet.ReadLong();
		}
	}
	Info.byCannon = packet.ReadChar();
	strncpy( Info.szCannon, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );

	Info.byEquipment = packet.ReadChar();
	strncpy( Info.szEquipment, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );

	Info.dwMoney = packet.ReadLong();
	Info.dwMinAttack = packet.ReadLong();
	Info.dwMaxAttack = packet.ReadLong();
	Info.dwCurEndure = packet.ReadLong();
	Info.dwMaxEndure = packet.ReadLong();
	Info.dwSpeed  = packet.ReadLong();
	Info.dwDistance = packet.ReadLong();
	Info.dwDefence = packet.ReadLong();
	Info.dwCurSupply = packet.ReadLong();
	Info.dwMaxSupply = packet.ReadLong();
	Info.dwConsume = packet.ReadLong();
	Info.dwAttackTime = packet.ReadLong();
	Info.sCapacity = packet.ReadShort();

	NetBoatInfo( dwBoatID, szBoat, Info );
	return TRUE;
T_E}

BOOL SC_UpdateBoatPart( LPRPACKET packet )
{T_B
	return TRUE;
T_E}

BOOL SC_BoatList( LPRPACKET packet )
{T_B
	DWORD dwNpcID = packet.ReadLong();
	BYTE byType = packet.ReadChar();
	BYTE byNumBoat = packet.ReadChar();
	BOAT_BERTH_DATA Data;
	memset( &Data, 0, sizeof(BOAT_BERTH_DATA) );
	for( BYTE i = 0; i < byNumBoat; i++ )
	{
		strncpy( Data.szName[i], packet.ReadString(), BOAT_MAXSIZE_BOATNAME + BOAT_MAXSIZE_DESP - 1 );
	}

	NetShowBoatList( dwNpcID, byNumBoat, Data, byType );
	return TRUE;
T_E}

//BOOL	SC_BoatBagList( LPRPACKET packet )
//{
//	BYTE byNumBoat = packet.ReadChar();
//	BOAT_BERTH_DATA Data;
//	memset( &Data, 0, sizeof(BOAT_BERTH_DATA) );
//	for( BYTE i = 0; i < byNumBoat; i++ )
//	{
//		strncpy( Data.szName[i], packet.ReadString(), BOAT_MAXSIZE_BOATNAME - 1 );
//	}
//
//	NetShowBoatBagList( byNumBoat, Data );
//	return TRUE;
//}

BOOL SC_StallInfo( LPRPACKET packet )
{T_B
	DWORD dwCharID = packet.ReadLong();
	BYTE byNum = packet.ReadChar();
	const char* pszName = packet.ReadString();
	if( !pszName ) return FALSE;

	NetStallInfo( dwCharID, byNum, pszName );

	for( BYTE i = 0; i < byNum; ++i )
	{
		BYTE byGrid = packet.ReadChar();
		USHORT sItemID = packet.ReadShort();
		BYTE byCount = packet.ReadChar();
		DWORD dwMoney = packet.ReadLong();
		USHORT sType = packet.ReadShort();

		if( sType == enumItemTypeBoat )
		{
			NET_CHARTRADE_BOATDATA Data;
			memset( &Data, 0, sizeof(NET_CHARTRADE_BOATDATA) );

			if( packet.ReadChar() == 0 )
			{
				// ��Ϣ����
			}
			else
			{
				strncpy( Data.szName, packet.ReadString(), BOAT_MAXSIZE_BOATNAME - 1 );
				Data.sBoatID = packet.ReadShort();
				Data.sLevel = packet.ReadShort();
				Data.dwExp = packet.ReadLong();
				Data.dwHp = packet.ReadLong();
				Data.dwMaxHp = packet.ReadLong();
				Data.dwSp = packet.ReadLong();
				Data.dwMaxSp = packet.ReadLong();
				Data.dwMinAttack = packet.ReadLong();
				Data.dwMaxAttack = packet.ReadLong();
				Data.dwDef = packet.ReadLong();
				Data.dwSpeed = packet.ReadLong();
				Data.dwShootSpeed = packet.ReadLong();
				Data.byHasItem = packet.ReadChar();
				Data.byCapacity = packet.ReadChar();
				Data.dwPrice = packet.ReadLong();
				NetStallAddBoat( byGrid, sItemID, byCount, dwMoney, Data );
			}
		}
		else
		{
			// ��Ʒʵ������
			NET_CHARTRADE_ITEMDATA Data;
			memset( &Data, 0, sizeof(NET_CHARTRADE_ITEMDATA) );

			Data.sEndure[0] = packet.ReadShort();
			Data.sEndure[1] = packet.ReadShort();
			Data.sEnergy[0] = packet.ReadShort();
			Data.sEnergy[1] = packet.ReadShort();
			Data.byForgeLv = packet.ReadChar();
			Data.bValid = packet.ReadChar() != 0 ? true : false;
			Data.lDBParam[enumITEMDBP_FORGE] = packet.ReadLong();
			Data.lDBParam[enumITEMDBP_INST_ID] = packet.ReadLong();

			Data.byHasAttr = packet.ReadChar();
			if( Data.byHasAttr )
			{
				for( int i = 0; i < defITEM_INSTANCE_ATTR_NUM; i++ )
				{
					Data.sInstAttr[i][0] = packet.ReadShort();
					Data.sInstAttr[i][1] = packet.ReadShort();
				}
			}
			NetStallAddItem( byGrid, sItemID, byCount, dwMoney, Data );
		}
	}
	return TRUE;
T_E}

BOOL SC_StallUpdateInfo( LPRPACKET packet )
{T_B
	return TRUE;
T_E}

BOOL SC_StallDelGoods( LPRPACKET packet )
{T_B
	DWORD dwCharID = packet.ReadLong();
	BYTE byGrid = packet.ReadChar();
	BYTE byCount = packet.ReadChar();
	NetStallDelGoods( dwCharID, byGrid, byCount );
	return TRUE;
T_E}

BOOL SC_StallClose( LPRPACKET packet )
{T_B
	DWORD dwCharID = packet.ReadLong();
	NetStallClose( dwCharID );
	return TRUE;
T_E}

BOOL SC_StallSuccess( LPRPACKET packet )
{T_B
	DWORD dwCharID = packet.ReadLong();
	NetStallSuccess( dwCharID );
	return TRUE;
T_E}

BOOL SC_SynStallName(LPRPACKET packet)
{T_B
	DWORD dwCharID = packet.ReadLong();
	const char	*szStallName = packet.ReadString();
	NetStallName( dwCharID, szStallName );
	return TRUE;
T_E}

BOOL SC_StartExit( LPRPACKET packet )
{T_B
	DWORD dwExitTime = packet.ReadLong();
	NetStartExit( dwExitTime );
	return TRUE;
T_E}

BOOL SC_CancelExit( LPRPACKET packet )
{T_B
	NetCancelExit();
	return TRUE;
T_E}

BOOL SC_UpdateHairRes( LPRPACKET packet )
{T_B
	stNetUpdateHairRes rv;
	rv.ulWorldID = packet.ReadLong();
	rv.nScriptID = packet.ReadShort();
	rv.szReason = packet.ReadString();
	rv.Exec();
	return TRUE;
T_E}

BOOL SC_OpenHairCut( LPRPACKET packet )
{T_B
	stNetOpenHair hair;
	hair.Exec();
	return TRUE;
T_E}

BOOL SC_TeamFightAsk(LPRPACKET packet)
{T_B
	char szLogName[128] = {0};
	strcpy(szLogName, g_oLangRec.GetString(305));

	stNetTeamFightAsk SFightAsk;
	SFightAsk.chSideNum2 = packet.ReverseReadChar();
	SFightAsk.chSideNum1 = packet.ReverseReadChar();
	LG(szLogName, g_oLangRec.GetString(306), SFightAsk.chSideNum1, SFightAsk.chSideNum2);
	for (char i = 0; i < SFightAsk.chSideNum1 + SFightAsk.chSideNum2; i++)
	{
		SFightAsk.Info[i].szName = packet.ReadString();
		SFightAsk.Info[i].chLv = packet.ReadChar();
		SFightAsk.Info[i].szJob = packet.ReadString();
		SFightAsk.Info[i].usFightNum = packet.ReadShort();
		SFightAsk.Info[i].usVictoryNum = packet.ReadShort();
		LG(szLogName, g_oLangRec.GetString(307), SFightAsk.Info[i].szName, SFightAsk.Info[i].chLv, SFightAsk.Info[i].szJob);
	}
	LG(szLogName, "\n");
	SFightAsk.Exec();
	return TRUE;
T_E}

BOOL SC_BeginItemRepair(LPRPACKET packet)
{T_B
	NetBeginRepairItem();
	return TRUE;
T_E}

BOOL SC_ItemRepairAsk(LPRPACKET packet)
{T_B
	stNetItemRepairAsk	SRepairAsk;
	SRepairAsk.cszItemName = packet.ReadString();
	SRepairAsk.lRepairMoney = packet.ReadLong();
	SRepairAsk.Exec();

	return TRUE;
T_E}

BOOL SC_ItemForgeAsk(LPRPACKET packet)
{T_B
	stSCNetItemForgeAsk	SForgeAsk;
	SForgeAsk.chType = packet.ReadChar();
	SForgeAsk.lMoney = packet.ReadLong();
	SForgeAsk.Exec();

	return TRUE;
T_E}

BOOL SC_ItemForgeAnswer(LPRPACKET packet)
{T_B
	stNetItemForgeAnswer	SForgeAnswer;
	SForgeAnswer.lChaID = packet.ReadLong();
	SForgeAnswer.chType = packet.ReadChar();
	SForgeAnswer.chResult = packet.ReadChar();
	SForgeAnswer.Exec();

	return TRUE;
T_E}

BOOL SC_ItemUseSuc(LPRPACKET packet)
{T_B
	unsigned int nChaID = packet.ReadLong();
	short sItemID = packet.ReadShort();
	NetItemUseSuccess(nChaID, sItemID);

	return TRUE;
T_E}

BOOL SC_KitbagCapacity(LPRPACKET packet)
{T_B
	unsigned int nChaID = packet.ReadLong();
	short sKbCap = packet.ReadShort();
	NetKitbagCapacity(nChaID, sKbCap);

	return TRUE;
T_E}

BOOL SC_EspeItem(LPRPACKET packet)
{T_B
	stNetEspeItem	SEspItem;
	unsigned int nChaID = packet.ReadLong();
	SEspItem.chNum = packet.ReadChar();
	for(char i = 0; i < 4; i++)
	{
		SEspItem.SContent[i].sPos = packet.ReadShort();
		SEspItem.SContent[i].sEndure = packet.ReadShort();
		SEspItem.SContent[i].sEnergy = packet.ReadShort();
	}

	NetEspeItem( nChaID, SEspItem );
	return TRUE;
T_E}

BOOL   SC_MapCrash(LPRPACKET packet)
{T_B
	const char* pszDesp = packet.ReadString();
	if( pszDesp == NULL ) 
		return FALSE;

	NetShowMapCrash(pszDesp);
	return TRUE;
T_E}

BOOL	SC_Message(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadLong();

    NetShowMessage( pk.ReadLong() );
	return TRUE;
T_E}

BOOL SC_QueryCha(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadLong();

	stNetSysInfo	SShowInfo;
	char	szInfo[512] = "";
	const char	*pChaName = pk.ReadString();
	const char	*pMapName = pk.ReadString();
	long	lPosX = pk.ReadLong();
	long	lPosY = pk.ReadLong();
	long	lChaID = pk.ReadLong();
	sprintf(szInfo, g_oLangRec.GetString(308), pChaName, lChaID, pMapName, lPosX, lPosY);
	SShowInfo.m_sysinfo = szInfo;
	NetSysInfo(SShowInfo);

	return TRUE;
T_E}

BOOL SC_QueryChaItem(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadLong();

	return TRUE;
T_E}

BOOL SC_QueryChaPing(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadLong();

	stNetSysInfo	SShowInfo;
	char	szInfo[512] = "";
	const char	*pChaName = pk.ReadString();
	const char	*pMapName = pk.ReadString();
	long	lPing = pk.ReadLong();
	sprintf(szInfo, g_oLangRec.GetString(309), pMapName, lPing);
	SShowInfo.m_sysinfo = szInfo;
	NetSysInfo(SShowInfo);

	return TRUE;
T_E}

BOOL SC_QueryRelive(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadLong();

	stNetQueryRelive SQueryRelive;
	SQueryRelive.szSrcChaName = pk.ReadString();
	SQueryRelive.chType = pk.ReadChar();
	NetQueryRelive(l_id, SQueryRelive);

	return TRUE;
T_E}

BOOL SC_PreMoveTime(LPRPACKET pk)
{T_B
	uLong ulPreMoveTime = pk.ReadLong();
	NetPreMoveTime(ulPreMoveTime);

	return TRUE;
T_E}

BOOL SC_MapMask(LPRPACKET pk)
{T_B
	uLong	l_id = pk.ReadLong();
	uShort	usLen = 0;
	BYTE	*pMapMask = 0;
	if (pk.ReadChar())
		pMapMask = (BYTE*)pk.ReadSequence(usLen);

	//char	*szMask = new char[usLen + 1];
	//memcpy(szMask, pMapMask, usLen);
	//szMask[usLen] = '\0';
	//LG("���ͼ", "%s\n", szMask);

	NetMapMask(l_id, pMapMask, usLen);

	return TRUE;
T_E}

BOOL SC_SynEventInfo(LPRPACKET pk)
{T_B
	stNetEvent	SNetEvent;
	ReadEntEventPacket(pk, SNetEvent);
	
	SNetEvent.ChangeEvent();
	return TRUE;
T_E}

BOOL SC_SynSideInfo(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadLong();
	char* szLogName = g_LogName.GetLogName( l_id );
	stNetChaSideInfo	SNetSideInfo;
	ReadChaSidePacket(pk, SNetSideInfo, szLogName);
	NetChaSideInfo(l_id, SNetSideInfo);

	return TRUE;
T_E}

BOOL SC_SynAppendLook(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadLong();
	char* szLogName = g_LogName.GetLogName( l_id );
	stNetAppendLook	SNetAppendLook;
	ReadChaAppendLookPacket(pk, SNetAppendLook, szLogName);
	SNetAppendLook.Exec(l_id);

	return TRUE;
T_E}

BOOL SC_KitbagCheckAnswer(LPRPACKET packet)
{T_B
	bool bLock = packet.ReadChar() ? true : false;
	NetKitbagCheckAnswer(bLock);

    return TRUE;
T_E}

BOOL SC_StoreOpenAnswer(LPRPACKET packet)
{T_B
	bool bValid = packet.ReadChar() ? true : false; // �̳��Ƿ���
	if(bValid)
	{
		g_stUIStore.ClearStoreTreeNode();
		g_stUIStore.ClearStoreItemList();
		//g_stUIStore.SetStoreBuyButtonEnable(true);

		long lVip = packet.ReadLong(); // VIP
		long lMoBean   = packet.ReadLong();	// Ħ��
		long lRplMoney = packet.ReadLong();	// ���ң�ˮ����
		g_stUIStore.SetStoreMoney(lMoBean, lRplMoney);
		g_stUIStore.SetStoreVip(lVip);

		long lAfficheNum = packet.ReadLong(); // ��������
		int i;
		for(i = 0; i < lAfficheNum; i++)
		{
			long lAfficheID = packet.ReadLong(); // ����ID
			uShort len;
			cChar *szTitle = packet.ReadSequence(len); // �������
			cChar *szComID = packet.ReadSequence(len); // ��Ӧ��ƷID,�ö��Ÿ���

			// ��ӹ���
		}
		long lFirstClass = 0;
		long lClsNum = packet.ReadLong(); // ��������
		for(i = 0; i < lClsNum; i++)
		{
			short lClsID = packet.ReadShort(); // ����ID
			uShort len;
			cChar *szClsName = packet.ReadSequence(len); // ������
			short lParentID = packet.ReadShort(); // ����ID

			// ����̳������
			g_stUIStore.AddStoreTreeNode(lParentID, lClsID, szClsName);

			// �ύ��һ������
			if(i == 0)
			{
				lFirstClass = lClsID;
			}
		}

		g_stUIStore.AddStoreUserTreeNode();// ��Ӹ��˹���
		g_stUIStore.ShowStoreForm(true);

		if(lFirstClass > 0)
		{
			::CS_StoreListAsk(lFirstClass, 1, (short)CStoreMgr::GetPageSize());
		}
	}
	else
	{
		// �̳�δ����,����ҳ
		g_stUIStore.ShowStoreWebPage();
	}

	g_stUIStore.DarkScene(false);
	g_stUIStore.ShowStoreLoad(false);

	return TRUE;
T_E}

BOOL SC_StoreListAnswer(LPRPACKET packet)
{T_B
	g_stUIStore.ClearStoreItemList();

	short sPageNum = packet.ReadShort(); // ҳ��
	short sPageCur = packet.ReadShort(); // ��ǰҳ
	short sComNum = packet.ReadShort(); // ��Ʒ��

	long i;
	for(i = 0; i < sComNum; i++)
	{
		long lComID = packet.ReadLong(); // ��ƷID
		uShort len;
		cChar *szComName = packet.ReadSequence(len); // ��Ʒ����
		long lPrice = packet.ReadLong(); // ��Ʒ�۸�
		cChar *szRemark = packet.ReadSequence(len); // ��Ʒ��ע
		bool isHot = packet.ReadChar() ? true : false;	// �Ƿ���������Ʒ
		long nTime = packet.ReadLong();
		long lComNumber = packet.ReadLong();	// ��Ʒʣ�������-1������
		long lComExpire = packet.ReadLong();	// ��Ʒ����ʱ��

		// ��ӵ��̳�
		g_stUIStore.AddStoreItemInfo(i, lComID, szComName, lPrice, szRemark, isHot, nTime, lComNumber, lComExpire);

		short sItemClsNum = packet.ReadShort(); // ������������
		int j;
		for(j = 0; j < sItemClsNum; j++)
		{
			short sItemID = packet.ReadShort(); // ����ID
			short sItemNum = packet.ReadShort(); // ��������
			short sFlute = packet.ReadShort(); // ��������
			short pItemAttrID[5];
			short pItemAttrVal[5];
			int k;
			for(k = 0; k < 5; k++)
			{
				pItemAttrID[k] = packet.ReadShort(); // ����ID
				pItemAttrVal[k] = packet.ReadShort(); // ����ֵ
			}

			// ��Ʒϸ������
			g_stUIStore.AddStoreItemDetail(i, j, sItemID, sItemNum, sFlute, pItemAttrID, pItemAttrVal);
		}
	}

	// ����ҳ��
	g_stUIStore.SetStoreItemPage(sPageCur, sPageNum);

	return TRUE;
T_E}

BOOL SC_StoreBuyAnswer(LPRPACKET packet)
{T_B
	bool bSucc = packet.ReadChar() ? true : false; // �����Ƿ�ɹ�
	long lMoBean = 0;
	long lRplMoney = 0;

	// ֪ͨ��ҹ���ɹ�
	// ...
	if(bSucc)
	{
		//lMoBean = packet.ReadLong();
		lRplMoney = packet.ReadLong();
		g_stUIEquip.UpdateIMP(lRplMoney);
		g_stUIStore.SetStoreMoney(-1, lRplMoney);
	}
	else
	{
		g_pGameApp->MsgBox(g_oLangRec.GetString(907)); // �������ʧ��!
	}

	g_stUIStore.SetStoreBuyButtonEnable(true);
	return TRUE;
T_E}

BOOL SC_StoreChangeAnswer(LPRPACKET packet)
{T_B
	bool bSucc = packet.ReadChar() ? true : false; // �һ������Ƿ�ɹ�
	if(bSucc)
	{
		long lMoBean = packet.ReadLong(); // ��ǰĦ������
		long lRplMoney = packet.ReadLong(); // ��ǰ��������

		g_stUIStore.SetStoreMoney(lMoBean, lRplMoney);
	}
	else
	{
		g_pGameApp->MsgBox(g_oLangRec.GetString(908));	// ���Ҷһ�ʧ��!
	}
	return TRUE;
T_E}

BOOL SC_StoreHistory(LPRPACKET packet)
{T_B
	long lNum = packet.ReadLong(); // ��ȡ�Ľ��׼�¼����
	int i;
	for(i = 0; i < lNum; i++)
	{
		uShort len;
		cChar *szTime = packet.ReadSequence(len); // ����ʱ��
		long lComID = packet.ReadLong(); // ��ƷID
		cChar *szName = packet.ReadSequence(len); // ��Ʒ����
		long lMoney = packet.ReadLong(); // ���׽��
	}
	return TRUE;
T_E}

BOOL SC_ActInfo(LPRPACKET packet)
{T_B
	bool bSucc = packet.ReadChar() ? true : false; // �ʺ���Ϣ��ѯ�Ƿ�ɹ�
	if(bSucc)
	{
		long lMoBean = packet.ReadLong(); // Ħ������
		long lRplMoney = packet.ReadLong(); // ��������
	}
	return TRUE;
T_E}

BOOL SC_StoreVIP(LPRPACKET packet)
{T_B
	bool bSucc = packet.ReadChar() ? true : false;
	if(bSucc)
	{
		short sVipID = packet.ReadShort();
		short sMonth = packet.ReadShort();
		long lShuijing = packet.ReadLong();
		long lModou = packet.ReadLong();

		g_stUIStore.SetStoreVip(sVipID);
		g_stUIStore.SetStoreMoney(lModou, lShuijing);
	}
	return TRUE;
T_E}

BOOL SC_BlackMarketExchangeData(LPRPACKET packet)
{T_B
	DWORD dwNpcID = packet.ReadLong();
	g_stUIBlackTrade.SetNpcID(dwNpcID);

	stBlackTrade SBlackTrade;
	short sCount = packet.ReadShort();
	for(short sIndex = 0; sIndex < sCount; ++sIndex)
	{
		memset(&SBlackTrade, 0, sizeof(stBlackTrade));

		SBlackTrade.sIndex   = sIndex;
		SBlackTrade.sSrcID   = packet.ReadShort();		// ������ƷID
		SBlackTrade.sSrcNum  = packet.ReadShort();		// ������Ʒ����
		SBlackTrade.sTarID   = packet.ReadShort();		// Ŀ����ƷID
		SBlackTrade.sTarNum  = packet.ReadShort();		// Ŀ����Ʒ����
		SBlackTrade.sTimeNum = packet.ReadShort();		// timeֵ

		g_stUIBlackTrade.SetItem(& SBlackTrade);
	}

	g_stUIBlackTrade.ShowBlackTradeForm(true);

	return TRUE;
T_E}

BOOL SC_ExchangeData(LPRPACKET packet)
{T_B
	DWORD dwNpcID = packet.ReadLong();
	g_stUIBlackTrade.SetNpcID(dwNpcID);

	stBlackTrade SBlackTrade;

	short sCount = packet.ReadShort();
	for(short sIndex = 0; sIndex < sCount; ++sIndex)
	{
		memset(&SBlackTrade, 0, sizeof(stBlackTrade));

		SBlackTrade.sIndex   = sIndex;
		SBlackTrade.sSrcID   = packet.ReadShort();		// ������ƷID
		SBlackTrade.sSrcNum  = packet.ReadShort();		// ������Ʒ����
		SBlackTrade.sTarID   = packet.ReadShort();		// Ŀ����ƷID
		SBlackTrade.sTarNum  = packet.ReadShort();		// Ŀ����Ʒ����
		SBlackTrade.sTimeNum = 0;

		g_stUIBlackTrade.SetItem(& SBlackTrade);
	}

	g_stUIBlackTrade.ShowBlackTradeForm(true);

	return TRUE;
T_E}

BOOL SC_BlackMarketExchangeUpdate(LPRPACKET packet)
{T_B
	//���жһ�����,ֻ���ڴ򿪺��жһ���������²Ÿ���!!!�м�

	DWORD dwNpcID = packet.ReadLong();
	stBlackTrade SBlackTrade;

	// ��ԭ�ȵĺ��жһ��������
	g_stUIBlackTrade.ClearItemData();

	short sCount = packet.ReadShort();
	for(short sIndex = 0; sIndex < sCount; ++sIndex)
	{
		memset(&SBlackTrade, 0, sizeof(stBlackTrade));

		SBlackTrade.sIndex   = sIndex;
		SBlackTrade.sSrcID   = packet.ReadShort();		// ������ƷID
		SBlackTrade.sSrcNum  = packet.ReadShort();		// ������Ʒ����
		SBlackTrade.sTarID   = packet.ReadShort();		// Ŀ����ƷID
		SBlackTrade.sTarNum  = packet.ReadShort();		// Ŀ����Ʒ����
		SBlackTrade.sTimeNum = packet.ReadShort();		// timeֵ

		if(g_stUIBlackTrade.GetIsShow() && g_stUIBlackTrade.GetNpcID() == dwNpcID)
		{
			g_stUIBlackTrade.SetItem(& SBlackTrade);
		}
	}

	if(g_stUIBlackTrade.GetIsShow() && g_stUIBlackTrade.GetNpcID() == dwNpcID)
	{
		g_stUIBlackTrade.ShowBlackTradeForm(true);
	}

	return TRUE;
T_E}

BOOL SC_BlackMarketExchangeAsr(LPRPACKET packet)
{T_B
	bool bSucc = (packet.ReadChar() == 1) ? true : false;
	if(bSucc)
	{
		stBlackTrade SBlackTrade;
		memset(&SBlackTrade, 0, sizeof(stBlackTrade));

		SBlackTrade.sSrcID  = packet.ReadShort();
		SBlackTrade.sSrcNum = packet.ReadShort();
		SBlackTrade.sTarID  = packet.ReadShort();
		SBlackTrade.sTarNum = packet.ReadShort();

		g_stUIBlackTrade.ExchangeAnswerProc(bSucc, &SBlackTrade);
	}

	return TRUE;
T_E}

BOOL SC_TigerItemID(LPRPACKET packet)
{T_B
	short sNum = packet.ReadShort();	// ����
	short sItemID[3] = {0};

	for(int i = 0; i < 3; i++)
	{
		sItemID[i] = packet.ReadShort();
	}

	g_stUISpirit.UpdateErnieNumber(sNum, sItemID[0], sItemID[1], sItemID[2]);

	if(sNum == 3)
	{
		// ���һ��
		g_stUISpirit.ShowErnieHighLight();
	}

	return TRUE;
T_E}

BOOL SC_VolunteerList(LPRPACKET packet)
{T_B
	short sPageNum = packet.ReadShort();	//��ҳ��
	short sPage = packet.ReadShort();		//��ǰҳ��
	short sRetNum = packet.ReadShort();		//־Ը������

	g_stUIFindTeam.SetFindTeamPage(sPage, sPageNum);
	g_stUIFindTeam.RemoveTeamInfo();

	for(int i = 0; i < sRetNum; i++)
	{
		const char *szName = packet.ReadString();
		long level = packet.ReadLong();
		long job = packet.ReadLong();
		const char *szMapName = packet.ReadString();

		g_stUIFindTeam.AddFindTeamInfo(i, szName, level, job, szMapName);
	}

	return TRUE;
T_E}

BOOL SC_VolunteerState(LPRPACKET packet)
{T_B
	bool bState = (packet.ReadChar() == 0) ? false : true;
	g_stUIFindTeam.SetOwnFindTeamState(bState);

	return TRUE;
T_E}

BOOL SC_VolunteerOpen(LPRPACKET packet)
{T_B
	bool bState = (packet.ReadChar() == 0) ? false : true;
	short sPageNum = packet.ReadShort();	//��ҳ��
	short sRetNum = packet.ReadShort();		//־Ը������

	g_stUIFindTeam.SetOwnFindTeamState(bState);
	g_stUIFindTeam.SetFindTeamPage(1, sPageNum <= 0 ? 1 : sPageNum);
	g_stUIFindTeam.RemoveTeamInfo();

	for(int i = 0; i < sRetNum; i++)
	{
		const char *szName = packet.ReadString();
		long level = packet.ReadLong();
		long job = packet.ReadLong();
		const char *szMapName = packet.ReadString();

		g_stUIFindTeam.AddFindTeamInfo(i, szName, level, job, szMapName);
	}

	g_stUIFindTeam.ShowFindTeamForm();

	return TRUE;
T_E}

BOOL SC_VolunteerAsk(LPRPACKET packet)
{T_B
	const char *szName = packet.ReadString();
	g_stUIFindTeam.FindTeamAsk(szName);
	
	return TRUE;
T_E}

BOOL SC_SyncKitbagTemp(LPRPACKET packet)
{
	/*stNetActorCreate SCreateInfo;
	ReadChaBasePacket(packet, SCreateInfo);
	SCreateInfo.chSeeType = enumENTITY_SEEN_NEW;
	SCreateInfo.chMainCha = 1;
	SCreateInfo.CreateCha();*/
	//g_ulWorldID = SCreateInfo.ulWorldID;

	stNetKitbag SKitbagTemp;
	ReadChaKitbagPacket(packet, SKitbagTemp, "KitbagTemp");
	SKitbagTemp.chBagType = 2;
	NetChangeKitbag(g_ulWorldID, SKitbagTemp);

	//CWorldScene* pWorldScene = dynamic_cast<CWorldScene*>(g_pGameApp->GetCurScene());
	//if(pWorldScene)

	return TRUE;
}

BOOL SC_SyncTigerString(LPRPACKET packet)
{
	const char *szString = packet.ReadString();
	g_stUISpirit.UpdateErnieString(szString);

	return TRUE;
}

BOOL SC_MasterAsk(LPRPACKET packet)
{
	const char *szName = packet.ReadString(); // ����
	DWORD dwCharID = packet.ReadLong();
	g_stUIChat.MasterAsk(szName, dwCharID);
	return TRUE;
}

BOOL SC_PrenticeAsk(LPRPACKET packet)
{
	const char *szName = packet.ReadString(); // ����
	DWORD dwCharID = packet.ReadLong();
	g_stUIChat.PrenticeAsk(szName, dwCharID);
	return TRUE;
}

BOOL PC_MasterRefresh(LPRPACKET packet)
{
	unsigned char l_type =packet.ReadChar();
	switch (l_type)
	{
	case MSG_MASTER_REFRESH_ONLINE:
		{
			uLong ulChaID = packet.ReadLong();
			NetMasterOnline(ulChaID);
		}
		break;
	case MSG_MASTER_REFRESH_OFFLINE:
		{
			uLong ulChaID = packet.ReadLong();
			NetMasterOffline(ulChaID);
		}
		break;
	case MSG_MASTER_REFRESH_DEL:
		{
			uLong ulChaID = packet.ReadLong();
			NetMasterDel(ulChaID);
		}
		break;
	case MSG_MASTER_REFRESH_ADD:
		{
			cChar	*l_grp		=packet.ReadString();
			uLong	l_chaid		=packet.ReadLong();
			cChar	*l_chaname	=packet.ReadString();
			cChar	*l_motto	=packet.ReadString();
			uShort	l_icon		=packet.ReadShort();
			NetMasterAdd(l_chaid,l_chaname,l_motto,l_icon,l_grp);
		}
		break;
	case MSG_MASTER_REFRESH_START:
		{
			stNetFrndStart l_self;
			l_self.lChaid	=packet.ReadLong();
			l_self.szChaname=packet.ReadString();
			l_self.szMotto	=packet.ReadString();
			l_self.sIconID	=packet.ReadShort();
			stNetFrndStart l_nfs[100];
			uShort	l_nfnum=0,l_grpnum	=packet.ReadShort();
			for(uShort l_grpi =0;l_grpi<l_grpnum;l_grpi++)
			{
				cChar*	l_grp		=packet.ReadString();
				uShort	l_grpmnum	=packet.ReadShort();
				for(uShort l_grpmi =0;l_grpmi<l_grpmnum;l_grpmi++)
				{
					l_nfs[l_nfnum].szGroup	=l_grp;
					l_nfs[l_nfnum].lChaid	=packet.ReadLong();
					l_nfs[l_nfnum].szChaname=packet.ReadString();
					l_nfs[l_nfnum].szMotto	=packet.ReadString();
					l_nfs[l_nfnum].sIconID	=packet.ReadShort();
					l_nfs[l_nfnum].cStatus	=packet.ReadChar();
					l_nfnum	++;
				}
			}
			NetMasterStart(l_self,l_nfs,l_nfnum);
		}
		break;

	case MSG_PRENTICE_REFRESH_ONLINE:
		{
			uLong ulChaID = packet.ReadLong();
			NetPrenticeOnline(ulChaID);
		}
		break;
	case MSG_PRENTICE_REFRESH_OFFLINE:
		{
			uLong ulChaID = packet.ReadLong();
			NetPrenticeOffline(ulChaID);
		}
		break;
	case MSG_PRENTICE_REFRESH_DEL:
		{
			uLong ulChaID = packet.ReadLong();
			NetPrenticeDel(ulChaID);
		}
		break;
	case MSG_PRENTICE_REFRESH_ADD:
		{
			cChar	*l_grp		=packet.ReadString();
			uLong	l_chaid		=packet.ReadLong();
			cChar	*l_chaname	=packet.ReadString();
			cChar	*l_motto	=packet.ReadString();
			uShort	l_icon		=packet.ReadShort();
			NetPrenticeAdd(l_chaid,l_chaname,l_motto,l_icon,l_grp);
		}
		break;
	case MSG_PRENTICE_REFRESH_START:
		{
			stNetFrndStart l_self;
			l_self.lChaid	=packet.ReadLong();
			l_self.szChaname=packet.ReadString();
			l_self.szMotto	=packet.ReadString();
			l_self.sIconID	=packet.ReadShort();
			stNetFrndStart l_nfs[100];
			uShort	l_nfnum=0,l_grpnum	=packet.ReadShort();
			for(uShort l_grpi =0;l_grpi<l_grpnum;l_grpi++)
			{
				cChar*	l_grp		=packet.ReadString();
				uShort	l_grpmnum	=packet.ReadShort();
				for(uShort l_grpmi =0;l_grpmi<l_grpmnum;l_grpmi++)
				{
					uShort index = l_grpmi % (sizeof(l_nfs) / sizeof(l_nfs[0]));
					l_nfs[index].szGroup	= l_grp;
					l_nfs[index].lChaid		= packet.ReadLong();
					l_nfs[index].szChaname	= packet.ReadString();
					l_nfs[index].szMotto	= packet.ReadString();
					l_nfs[index].sIconID	= packet.ReadShort();
					l_nfs[index].cStatus	= packet.ReadChar();
					l_nfnum	++;
				}
			}
			NetPrenticeStart(l_self,l_nfs,min(l_nfnum, (sizeof(l_nfs) / sizeof(l_nfs[0]))));
		}
		break;
	}
	return TRUE;
}

BOOL PC_MasterCancel(LPRPACKET packet)
{
	unsigned char reason =packet.ReadChar();
	uLong ulChaID = packet.ReadLong();
	NetMasterCancel(packet.ReadLong(),reason);
	return TRUE;
}

BOOL PC_MasterRefreshInfo(LPRPACKET packet)
{
	unsigned long l_chaid	=packet.ReadLong();
	const char	* l_motto	=packet.ReadString();
	unsigned short l_icon	=packet.ReadShort();
	unsigned short l_degr	=packet.ReadShort();
	const char	* l_job		=packet.ReadString();
	const char	* l_guild	=packet.ReadString();
	NetMasterRefreshInfo(l_chaid,l_motto,l_icon,l_degr,l_job,l_guild);
	return TRUE;
}

BOOL PC_PrenticeRefreshInfo(LPRPACKET packet)
{
	unsigned long l_chaid	=packet.ReadLong();
	const char	* l_motto	=packet.ReadString();
	unsigned short l_icon	=packet.ReadShort();
	unsigned short l_degr	=packet.ReadShort();
	const char	* l_job		=packet.ReadString();
	const char	* l_guild	=packet.ReadString();
	NetPrenticeRefreshInfo(l_chaid,l_motto,l_icon,l_degr,l_job,l_guild);
	return TRUE;
}

BOOL SC_ChaPlayEffect(LPRPACKET packet)
{
	unsigned int uiWorldID = packet.ReadLong();
	int nEffectID = packet.ReadLong();

	NetChaPlayEffect(uiWorldID, nEffectID);

	return TRUE;
}

BOOL SC_Say2Camp(LPRPACKET packet)
{
	cChar *szName = packet.ReadString();
	cChar *szContent = packet.ReadString();

	//g_pGameApp->SysInfo("[��Ӫ]%s:%s", szName, szContent);
	NetSideInfo(szName, szContent);

	return TRUE;
}

BOOL SC_GMMail(LPRPACKET packet)
{
	cChar *szTitle = packet.ReadString();
	cChar *szContent = packet.ReadString();
	long lTime = packet.ReadLong();

	g_stUIMail.ShowAnswerForm(szTitle, szContent);

	return TRUE;
}

BOOL SC_CheatCheck(LPRPACKET packet)
{
	short count = packet.ReadShort(); // ͼƬ����
	for(int i = 0; i < count; i++)
	{
		char *picture = NULL;
		short size = packet.ReadShort(); // ͼƬ��С
		picture = new char[size];
		for(int j = 0; j < size; j++)
		{
			picture[j] = packet.ReadChar();
		}

		g_stUINumAnswer.SetBmp(i, (BYTE*)picture, size);

		delete [] picture;
	}

	g_stUINumAnswer.Refresh();
	g_stUINumAnswer.ShowForm();

	return TRUE;
}

BOOL SC_ListAuction(LPRPACKET pk)
{
	short sNum = pk.ReverseReadShort(); // ��������
	stChurchChallenge stInfo;

	for(int i = 0; i < sNum; i++)
	{
		short  sItemID   = pk.ReadShort();	// id
		cChar* szName    = pk.ReadString();	// name
		cChar* szChaName = pk.ReadString();	// ��ս��
		short  sCount    = pk.ReadShort();	// ����
		long   baseprice = pk.ReadLong();	// �׼�
		long   minbid    = pk.ReadLong();	// ��ͳ���
		long   curprice  = pk.ReadLong();	// ��ǰ���ļ�

		stInfo.sChurchID  = sItemID;
		stInfo.sCount     = sCount;
		stInfo.nBasePrice = baseprice;
		stInfo.nMinbid    = minbid;
		stInfo.nCurPrice  = curprice;
		strncpy(stInfo.szChaName, szChaName, sizeof(stInfo.szChaName) - 1);
		strncpy(stInfo.szName, szName, sizeof(stInfo.szName) - 1);
	}

	NetChurchChallenge(&stInfo);

	return TRUE;
}

// �������ݱ�=======================================================================
void ReadChaBasePacket(LPRPACKET pk, stNetActorCreate &SCreateInfo)
{T_B
	memset(&SCreateInfo, 0, sizeof(SCreateInfo));
	// ��������
	SCreateInfo.ulChaID		= pk.ReadLong();
	SCreateInfo.ulWorldID	= pk.ReadLong();
	SCreateInfo.ulCommID	= pk.ReadLong();
	SCreateInfo.szCommName	= pk.ReadString();
	SCreateInfo.chGMLv		= pk.ReadChar();
	SCreateInfo.lHandle		= pk.ReadLong();
	SCreateInfo.chCtrlType	= pk.ReadChar();	// ��ɫ���ͣ��ο�CompCommand.h EChaCtrlType��
	SCreateInfo.szName = (char *)pk.ReadString();
	SCreateInfo.strMottoName = pk.ReadString(); // ����������
	SCreateInfo.sIcon       = pk.ReadShort();   // ��ɫ����ͷ��
	SCreateInfo.lGuildID = pk.ReadLong();
	SCreateInfo.strGuildName = pk.ReadString(); // ������
	SCreateInfo.strGuildMotto = pk.ReadString(); // ����������
	SCreateInfo.chGuildPermission = pk.ReadLong(); // ����������
	SCreateInfo.lGuildCircleColour = pk.ReadLong(); // ����������
	SCreateInfo.chGuildIcon = pk.ReadChar(); // ����������
	SCreateInfo.strStallName = pk.ReadString(); // ����������
	SCreateInfo.sState = pk.ReadShort();
	SCreateInfo.SArea.centre.x = pk.ReadLong();
	SCreateInfo.SArea.centre.y = pk.ReadLong();
	SCreateInfo.SArea.radius = pk.ReadLong();
	SCreateInfo.sAngle = pk.ReadShort();
	SCreateInfo.ulTLeaderID = pk.ReadLong(); // �ӳ�ID
	
	SCreateInfo.chIsPlayer = pk.ReadChar();

	char* szLogName = g_LogName.SetLogName( SCreateInfo.ulWorldID, SCreateInfo.szName );
	ReadChaSidePacket(pk, SCreateInfo.SSideInfo, szLogName);
	ReadEntEventPacket(pk, SCreateInfo.SEvent, szLogName);
	ReadChaLookPacket(pk, SCreateInfo.SLookInfo, szLogName);
	ReadChaPKPacket(pk, SCreateInfo.SPKCtrl, szLogName);
	ReadChaAppendLookPacket(pk, SCreateInfo.SAppendLook, szLogName);

	LG(szLogName, "+++++++++++++Create(State: %u\tPos: [%d, %d]\n", SCreateInfo.sState, SCreateInfo.SArea.centre.x, SCreateInfo.SArea.centre.y);
	LG(szLogName, "CtrlType:%d, TeamdID:%u\n", SCreateInfo.chCtrlType, SCreateInfo.ulTLeaderID );

T_E}

BOOL ReadChaSkillBagPacket(LPRPACKET pk, stNetSkillBag &SCurSkill, char *szLogName)
{T_B
	memset(&SCurSkill, 0, sizeof(SCurSkill));
	stNetDefaultSkill	SDefaultSkill;
	SDefaultSkill.sSkillID = pk.ReadShort();
	SDefaultSkill.Exec();

	SCurSkill.chType = pk.ReadChar();
	short sSkillNum = pk.ReadShort();
	if (sSkillNum <= 0)
		return TRUE;

	SCurSkill.SBag.Resize( sSkillNum );
	SSkillGridEx* pSBag = SCurSkill.SBag.GetValue();
	short i = 0;
	for (; i < sSkillNum; i++)
	{
		pSBag[i].sID = pk.ReadShort();
		pSBag[i].chState = pk.ReadChar();
		pSBag[i].chLv = pk.ReadChar();
		pSBag[i].sUseSP = pk.ReadShort();
		pSBag[i].sUseEndure = pk.ReadShort();
		pSBag[i].sUseEnergy = pk.ReadShort();
		pSBag[i].lResumeTime = pk.ReadLong();
		pSBag[i].sRange[0] = pk.ReadShort();
		if (pSBag[i].sRange[0] != enumRANGE_TYPE_NONE)
			for (short j = 1; j < defSKILL_RANGE_PARAM_NUM; j++)
				pSBag[i].sRange[j] = pk.ReadShort();
	}

	// log
	LG(szLogName, "Syn Skill Bag, Type:%d,\tTick:[%u]\n", SCurSkill.chType, GetTickCount());
	LG(szLogName, g_oLangRec.GetString(310));
	char	szRange[256];
	for (i = 0; i < sSkillNum; i++)
	{
		sprintf(szRange, "%d", pSBag[i].sRange[0]);
		if (pSBag[i].sRange[0] != enumRANGE_TYPE_NONE)
			for (short j = 1; j < defSKILL_RANGE_PARAM_NUM; j++)
				sprintf(szRange + strlen(szRange), ",%d", pSBag[i].sRange[j]);
		LG(szLogName, "\t%4d\t%4d\t%4d\t%6d\t%6d\t%6d\t%18d\t%s\n", pSBag[i].sID, pSBag[i].chState, pSBag[i].chLv, pSBag[i].sUseSP, pSBag[i].sUseEndure, pSBag[i].sUseEnergy, pSBag[i].lResumeTime, szRange);
	}
	LG(szLogName, "\n");
	//

	return TRUE;
T_E}

void ReadChaSkillStatePacket(LPRPACKET pk, stNetSkillState &SCurSState, char* szLogName)
{T_B
	unsigned long currentClient = GetTickCount();	
	unsigned long currentServer = pk.ReadLong()/1000;//current server time
	memset(&SCurSState, 0, sizeof(SCurSState));
	SCurSState.chType = 0;
	short sNum = pk.ReadChar();
	if ( sNum>0 )
	{
		SCurSState.SState.Resize( sNum );
		for (int nNum = 0; nNum < sNum; nNum++)
		{
			SCurSState.SState[nNum].chID = pk.ReadChar();
			SCurSState.SState[nNum].chLv = pk.ReadChar();
			
			
			unsigned long duration = pk.ReadLong();//duration
			unsigned long start = pk.ReadLong()/1000;//start time
			
			
			unsigned long dif = currentServer - currentClient;
			unsigned long end = start - dif + duration;
			
			SCurSState.SState[nNum].lTimeRemaining = duration==0?0:end-currentClient; //end time, corrected for client
		}
	}

	// log
	LG(szLogName, "Syn Skill State: Num[%d]\tTick[%u]\n", sNum, GetTickCount());
	LG(szLogName, g_oLangRec.GetString(311));
	for (char i = 0; i < sNum; i++)
		LG(szLogName, "\t%8d\t%4d\n", SCurSState.SState[i].chID, SCurSState.SState[i].chLv);
	LG(szLogName, "\n");
	//
T_E}

void ReadChaAttrPacket(LPRPACKET pk, stNetChaAttr &SChaAttr, char* szLogName)
{T_B
	memset(&SChaAttr, 0, sizeof(SChaAttr));
	SChaAttr.chType = pk.ReadChar();
	SChaAttr.sNum = pk.ReadShort();
	for (short i = 0; i < SChaAttr.sNum; i++)
	{
		SChaAttr.SEff[i].lAttrID = pk.ReadChar();
		SChaAttr.SEff[i].lVal = (long)pk.ReadLong();
	}

	// log
	LG(szLogName, "Syn Character Attr: Count=%d\t, Type:%d\tTick:[%u]\n", SChaAttr.sNum, SChaAttr.chType, GetTickCount());
	LG(szLogName, g_oLangRec.GetString(312));
	for (short i = 0; i < SChaAttr.sNum; i++)
	{
		LG(szLogName, "\t%d\t%d\n", SChaAttr.SEff[i].lAttrID, SChaAttr.SEff[i].lVal);
	}
	LG(szLogName, "\n");
	//
T_E}

void ReadChaLookPacket(LPRPACKET pk, stNetLookInfo &SLookInfo, char *szLogName)
{T_B
	memset(&SLookInfo, 0, sizeof(SLookInfo));
	SLookInfo.chSynType = pk.ReadChar();
	stNetChangeChaPart	&SChaPart = SLookInfo.SLook;
	SChaPart.sTypeID = pk.ReadShort();
	if( pk.ReadChar() == 1 ) // �������
	{
		SChaPart.sPosID = pk.ReadShort();
		SChaPart.sBoatID = pk.ReadShort();
		SChaPart.sHeader = pk.ReadShort();
		SChaPart.sBody = pk.ReadShort();
		SChaPart.sEngine = pk.ReadShort();
		SChaPart.sCannon = pk.ReadShort();
		SChaPart.sEquipment = pk.ReadShort();

		// log
		LG(szLogName, "===Recieve(Look):\tTick:[%u]\n", GetTickCount());
		LG(szLogName, "TypeID:%d, PoseID:%d\n", SChaPart.sTypeID, SChaPart.sPosID);
		LG(szLogName, "\tPart: Boat:%u, Header:%u, Body:%u, Engine:%u, Cannon:%u, Equipment:%u\n", SChaPart.sBoatID, SChaPart.sHeader, SChaPart.sBody, SChaPart.sEngine, SChaPart.sCannon, SChaPart.sEquipment );
		LG(szLogName, "\n");
		//
	}
	else
	{
		SChaPart.sHairID = pk.ReadShort();
		SItemGrid *pItem;
		for (int i = 0; i < enumEQUIP_NUM; i++)
		{
			pItem = SChaPart.SLink + i;
			pItem->sID = pk.ReadShort();
			pItem->dwDBID = pk.ReadLong();
			pItem->sNeedLv = pk.ReadShort();
			if (pItem->sID == 0)
			{
				memset(pItem, 0, sizeof(SItemGrid));
				continue;
			}
			if (SLookInfo.chSynType == enumSYN_LOOK_CHANGE)
			{
				pItem->sEndure[0] = pk.ReadShort();
				pItem->sEnergy[0] = pk.ReadShort();
				pItem->SetValid(pk.ReadChar() != 0 ? true : false);
				continue;
			}
			else
			{
				pItem->sNum = pk.ReadShort();
				pItem->sEndure[0] = pk.ReadShort();
				pItem->sEndure[1] = pk.ReadShort();
				pItem->sEnergy[0] = pk.ReadShort();
				pItem->sEnergy[1] = pk.ReadShort();
				pItem->chForgeLv = pk.ReadChar();
				pItem->SetValid(pk.ReadChar() != 0 ? true : false);
			}

			if (pk.ReadChar() == 0)
				continue;

			pItem->SetDBParam(enumITEMDBP_FORGE, pk.ReadLong());
			pItem->SetDBParam(enumITEMDBP_INST_ID, pk.ReadLong());
			if (pk.ReadChar()) // ����ʵ������
			{
				for (int j = 0; j < defITEM_INSTANCE_ATTR_NUM; j++)
				{
					pItem->sInstAttr[j][0] = pk.ReadShort();
					pItem->sInstAttr[j][1] = pk.ReadShort();
				}
			}
		}

		// log
		LG(szLogName, "===Recieve(Look)\tTick:[%u]\n", GetTickCount());
		LG(szLogName, "TypeID:%d, HairID:%d\n", SChaPart.sTypeID, SChaPart.sHairID);
		for (int i = 0; i < enumEQUIP_NUM; i++)
			LG(szLogName, "\tLink: %d\n", SChaPart.SLink[i].sID);
		LG(szLogName, "\n");
		//
	}
T_E}

void ReadChaLookEnergyPacket(LPRPACKET pk, stLookEnergy &SLookEnergy, char *szLogName)
{T_B
	memset(&SLookEnergy, 0, sizeof(SLookEnergy));
	for (int i = 0; i < enumEQUIP_NUM; i++)
	{
		SLookEnergy.sEnergy[i] = pk.ReadShort();
	}
T_E}

void ReadChaPKPacket(LPRPACKET pk, stNetPKCtrl &SNetPKCtrl, char *szLogName)
{T_B
	char	chPKCtrl = pk.ReadChar();
	SNetPKCtrl.bInGymkhana = chPKCtrl & 0x02 ? true : false;
	SNetPKCtrl.bInPK = chPKCtrl & 0x01 ? true : false;

	// log
	LG(szLogName, "===Recieve(PKCtrl)\tTick:[%u]\n", GetTickCount());
	LG(szLogName, "\tInGymkhana: %d, InPK: %d\n", SNetPKCtrl.bInGymkhana, SNetPKCtrl.bInPK);
	LG(szLogName, "\n");
	//
T_E}

void ReadChaSidePacket(LPRPACKET pk, stNetChaSideInfo &SNetSideInfo, char *szLogName)
{T_B
	SNetSideInfo.chSideID = pk.ReadChar();

	// log
	LG(szLogName, "===Recieve(SideInfo)\tTick:[%u]\n", GetTickCount());
	LG(szLogName, "\tSideID: %d\n", SNetSideInfo.chSideID);
	LG(szLogName, "\n");
	//
T_E}

void ReadChaAppendLookPacket(LPRPACKET pk, stNetAppendLook &SNetAppendLook, char *szLogName)
{T_B
	for (char i = 0; i < defESPE_KBGRID_NUM; i++)
	{
		SNetAppendLook.sLookID[i] = pk.ReadShort();
		if (SNetAppendLook.sLookID[i] != 0)
			SNetAppendLook.bValid[i] = pk.ReadChar() != 0 ? true : false;
	}

	// log
	LG(szLogName, "===Recieve(Append Look)\tTick:[%u]\n", GetTickCount());
	LG(szLogName, "\tAppend Look:%d(%d), %d(%d), %d(%d), %d(%d)\n",
		SNetAppendLook.sLookID[0], SNetAppendLook.bValid[0],
		SNetAppendLook.sLookID[1], SNetAppendLook.bValid[1],
		SNetAppendLook.sLookID[2], SNetAppendLook.bValid[2],
		SNetAppendLook.sLookID[3], SNetAppendLook.bValid[3]);
	LG(szLogName, "\n");
	//
T_E}

void ReadEntEventPacket(LPRPACKET pk, stNetEvent &SNetEvent, char *szLogName)
{T_B
	SNetEvent.lEntityID = pk.ReadLong();
	SNetEvent.chEntityType = pk.ReadChar();
	SNetEvent.usEventID = pk.ReadShort();
	SNetEvent.cszEventName = pk.ReadString();

	// log
	if (szLogName)
	{
		LG(szLogName, "===Recieve(Event)\tTick:[%u]\n", GetTickCount());
		LG(szLogName, "\tEntityID: %u, EventID: %u, EventName: %s\n", SNetEvent.lEntityID, SNetEvent.usEventID, SNetEvent.cszEventName);
		LG(szLogName, "\n");
	}
	//
T_E}

void ReadChaKitbagPacket(LPRPACKET pk, stNetKitbag &SKitbag, char *szLogName)
{T_B
	memset(&SKitbag, 0, sizeof(SKitbag));
	SKitbag.chType = pk.ReadChar(); // ���ο�CompCommand.h��ESynKitbagType��
	int nGridNum = 0;
	if (SKitbag.chType == enumSYN_KITBAG_INIT) // ����������
	{
		SKitbag.nKeybagNum = pk.ReadShort();
	}
	LG(szLogName, "===Recieve(Update Kitbag):\tGridNum:%d\tType:%d\tTick:[%u]\n", SKitbag.nKeybagNum, SKitbag.chType, GetTickCount());
	stNetKitbag::stGrid* Grid = SKitbag.Grid;
	SItemGrid *pItem;
	CItemRecord* pItemRec;
	while (1)
	{
		short sGridID = pk.ReadShort();
		if(sGridID < 0) break;

		Grid[nGridNum].sGridID = sGridID;

		pItem = &Grid[nGridNum].SGridContent;
		pItem->sID = pk.ReadShort();
		LG(szLogName, g_oLangRec.GetString(313), Grid[nGridNum].sGridID, pItem->sID);
		if (pItem->sID > 0) // ���ڵ���
		{
			pItem->dwDBID = pk.ReadLong();
			pItem->sNeedLv		=	pk.ReadShort();
			pItem->sNum			=	pk.ReadShort();
			pItem->sEndure[0]	=	pk.ReadShort();
			pItem->sEndure[1]	=	pk.ReadShort();
			pItem->sEnergy[0]	=	pk.ReadShort();
			pItem->sEnergy[1]	=	pk.ReadShort();
			LG(szLogName, g_oLangRec.GetString(314), pItem->sNum, pItem->sEndure[0], pItem->sEndure[1], pItem->sEnergy[0], pItem->sEnergy[1]);
			pItem->chForgeLv = pk.ReadChar();
			pItem->SetValid(pk.ReadChar() != 0 ? true : false);

			pItemRec = GetItemRecordInfo( pItem->sID );
			if(pItemRec==NULL)
			{
				char szBuf[256] = { 0 };
				sprintf( szBuf, g_oLangRec.GetString(315), pItem->sID );
				MessageBox( 0, szBuf, "Error", 0 );
#ifdef USE_DSOUND
				if( g_dwCurMusicID )
				{
					AudioSDL::get_instance()->stop( g_dwCurMusicID );
					g_dwCurMusicID = 0;
					Sleep( 60 );
				}
#endif
				exit(0);
			}

			if( pItemRec->sType == enumItemTypeBoat ) // �����ߣ�д�봬��WorldID�����ڿͻ��˽������봬��ɫ�ҹ�
			{
				pItem->SetDBParam(enumITEMDBP_INST_ID, pk.ReadLong());
			}

			pItem->SetDBParam(enumITEMDBP_FORGE, pk.ReadLong());
			if( pItemRec->sType == enumItemTypeBoat ) 
			{
				pk.ReadLong();
			}
			else
			{
				pItem->SetDBParam(enumITEMDBP_INST_ID, pk.ReadLong());
			}

			LG(szLogName, g_oLangRec.GetString(316), pItem->GetDBParam(enumITEMDBP_FORGE));
			if (pk.ReadChar()) // ����ʵ������
			{
				for (int j = 0; j < defITEM_INSTANCE_ATTR_NUM; j++)
				{
					pItem->sInstAttr[j][0] = pk.ReadShort();
					pItem->sInstAttr[j][1] = pk.ReadShort();
					LG(szLogName, g_oLangRec.GetString(317), pItem->sInstAttr[j][0], pItem->sInstAttr[j][1]);
				}
			}
		}
		nGridNum++;
		if (nGridNum > defMAX_KBITEM_NUM_PER_TYPE) // ���ó��ֵ����
		{
			LG(g_oLangRec.GetString(318), g_oLangRec.GetString(319), nGridNum, defMAX_KBITEM_NUM_PER_TYPE);
			break;
		}
	}
	SKitbag.nGridNum = nGridNum;
	LG(szLogName, g_oLangRec.GetString(320), SKitbag.nGridNum);
T_E}

void ReadChaShortcutPacket(LPRPACKET pk, stNetShortCut &SShortcut, char* szLogName)
{T_B
	memset(&SShortcut, 0, sizeof(SShortcut));
	LG(szLogName, "===Recieve(Update Shortcut):\tTick:[%u]\n", GetTickCount());
	for (int i = 0; i < SHORT_CUT_NUM; i++)
	{
		SShortcut.chType[i] = pk.ReadChar();
		SShortcut.byGridID[i] = pk.ReadShort();
		LG(szLogName, g_oLangRec.GetString(321), SShortcut.chType[i], SShortcut.byGridID[i]);
	}
T_E}


BOOL PC_PKSilver(LPRPACKET packet)
{T_B
    std::string szName;
    long sLevel;
    std::string szProfession;
    long lPkval;
    for(int i = 0; i < MAX_PKSILVER_PLAYER; i++)
    {
        szName = packet.ReadString();
        sLevel = packet.ReadLong();
        szProfession = packet.ReadString();
        lPkval = packet.ReadLong();
        g_stUIPKSilver.AddFormAttribute(i, szName, sLevel, szProfession, lPkval);
    }

    g_stUIPKSilver.ShowPKSilverForm();
    return TRUE;
T_E}


BOOL SC_LifeSkillShow(LPRPACKET packet)
{T_B
    long lType = packet.ReadLong();
    switch(lType)
    {
    case 0:     //  �ϳ�
        {
            g_stUICompose.ShowComposeForm();
        }  break;
    case 1:     //  �ֽ�
        {
            g_stUIBreak.ShowBreakForm();
        }  break;
    case 2:     //  ����
        {
            g_stUIFound.ShowFoundForm();
        }  break;
    case 3:     //  ���
        {
            g_stUICooking.ShowCookingForm();
        }  break;
    }
    return TRUE;
T_E}


BOOL SC_LifeSkill(LPRPACKET packet)
{T_B
    long lType = packet.ReadLong();
    short ret = packet.ReadShort();
    std::string txt = packet.ReadString();

    switch(lType)
    {
    case 0:     //  �ϳ�
        {
            g_stUICompose.CheckResult(ret, txt.c_str());
        }  break;
    case 1:     //  �ֽ�
        {
            g_stUIBreak.CheckResult(ret, txt.c_str());
        }  break;
    case 2:     //  ����
        {
            std::string strVer[3];
            Util_ResolveTextLine(txt.c_str(), strVer, 3, ',');
            g_stUIFound.CheckResult(ret, strVer[0].c_str(), strVer[1].c_str(), strVer[2].c_str());
        }  break;
    case 3:     //  ���
        {
            g_stUICooking.CheckResult(ret);
        }  break;
    }

    return TRUE;
T_E}


BOOL SC_LifeSkillAsr(LPRPACKET packet)
{T_B
    long lType = packet.ReadLong();
    short tim = packet.ReadShort();
    std::string txt = packet.ReadString();

    switch(lType)
    {
    case 0:     //  �ϳ�
        {
            g_stUICompose.StartTime(tim);
        }  break;
    case 1:     //  �ֽ�
        {
            g_stUIBreak.StartTime(tim, txt.c_str());
        }  break;
    case 2:     //  ����
        {
            g_stUIFound.StartTime(tim);
        }  break;
    case 3:     //  ���
        {
            g_stUICooking.StartTime(tim);
        }  break;
    }
    return TRUE;
T_E}

//	2008-7-30	yangyinyu	add
bool	g_yyy_add_lock_item_wait_return_state	=	false;

//	���������Ļ�Ӧ��Ϣ��
BOOL	SC_DropLockAsr(	LPRPACKET	pk	)
{T_B
	//
	dbc::Char	success	=	pk.ReadChar();

	//
	if(	g_yyy_add_lock_item_wait_return_state	)
	{
		//	�رյ��߶Ի���
		if(	success	)
		{
			//g_stUIEquip.CloseAllForm();
			//g_pGameApp->MsgBox("�������߳ɹ���");
			g_pGameApp->SysInfo("Locking successful!");
		}
		else
		{
			//g_stUIEquip.CloseAllForm();
			//g_pGameApp->MsgBox("������߲��ɼ�����");
			g_pGameApp->SysInfo("Locking failed!");
		}
	}
	//g_stUIEquip.CloseAllForm();

	//
	g_yyy_add_lock_item_wait_return_state	=	false;

	//	���õȴ���ꡣ
	CCursor::I()->SetCursor(	CCursor::stNormal	);

	//
	return	TRUE;
T_E};


// add by ning.yan ���߽����Ļ�Ӧ��Ϣ 2008-11-12 begin
BOOL SC_UnlockItemAsr( LPRPACKET pk )
{T_B
	dbc::Char	success	=	pk.ReadChar();
	if(	g_yyy_add_lock_item_wait_return_state	)
	{
		if( success==0	)
		{
			g_pGameApp->SysInfo( "Unlocking failed!" );
		}
		else if( success==1 )
		{
			g_pGameApp->SysInfo( "Item unlocked!" );
		}
		else if( success==2 )
		{
		}
	}
	g_yyy_add_lock_item_wait_return_state	=	false;
	CCursor::I()->SetCursor( CCursor::stNormal );
	return	TRUE;
T_E}

//==================================================================================


//guild house system by billy WIP

# define M_PI           3.14159265358979323846 
//std::vector<CSceneObj *> guildHouseObjects;
std::vector<vector<CSceneObj *> > guildHouseObjects;

std::vector<CSceneObj *> guildHouseActions;

BOOL SC_InitGuildHouseObjects(LPRPACKET pk){
	guildHouseObjects.clear();
	int num = pk.ReadLong();
	for(int i = 0;i<num;i++){
		SC_AddGuildHouseObject(pk);
	}
	return true;
}

BOOL SC_AddGuildHouseObject(LPRPACKET pk){
	
	
	
	int ID = pk.ReadLong();
	int posX = pk.ReadLong();
	int posY = pk.ReadLong();
	int yaw = pk.ReadLong();
		
	CGameScene* pScene = CGameApp::GetCurScene();
	CSceneObj *pObj = pScene->AddSceneObj(ID);
	if(pObj){
		pScene->AddRBO( pObj );
		pObj->recordRBO = false;
		pObj->setHeightOff( 0 );
		pObj->setPos( posX, posY );
		pObj->setYaw( yaw );
		pObj->setRBOHeight(0);
		pObj->GetObject()->CullPrimitive();
		
		std::vector<CSceneObj *> guildHouseLinkedObjects;
		guildHouseLinkedObjects.push_back(pObj);
		
		int extraCount = pk.ReadChar();
		if(extraCount > 0){
			for(int index = 0;index<extraCount;index++){
				int actionID = pk.ReadLong();
				CSceneObj *pObj2 = pScene->AddSceneObj(actionID);
				pScene->AddRBO( pObj2 );
				pObj2->recordRBO = false;
				
				signed float offX = pk.ReadLong();
				signed float offY = pk.ReadLong();
				
				signed float actionYaw = pk.ReadLong();
				actionYaw += yaw;
				if (actionYaw > 360){
					actionYaw -=360;
				}else if(actionYaw < 0 ){
					actionYaw +=360;
				}
				
				float rotZ = ( M_PI * actionYaw)/180;
				
				signed int actionHeight = pk.ReadLong();
				
				float deltaX = offX* cos ( rotZ ) - offY* sin(rotZ);
				float deltaY = offY* cos ( rotZ ) + offX* sin(rotZ);
				int actionPosX = posX + deltaX;
				int actionPosY = posY + deltaY;
				
				pObj2->setHeightOff( actionHeight );
				pObj2->setPos( actionPosX,actionPosY );
				pObj2->setYaw( actionYaw );
				pObj2->setRBOHeight(actionHeight);
				pObj2->GetObject()->CullPrimitive();
				guildHouseLinkedObjects.push_back(pObj2);
			}
		}
		
		guildHouseObjects.push_back(guildHouseLinkedObjects);
		
		
	}
	return true;
}

BOOL SC_RemoveGuildHouseObject(int index){
	
	
	if(index < guildHouseObjects.size()){
		
		int size = guildHouseObjects[index].size();
		for (int i = 0;i<size;i++){ 
			if(guildHouseObjects[index][i]){
				guildHouseObjects[index][i]->SetIsValid(false);
			}
		}
		
		guildHouseObjects[index].clear();
		
		guildHouseObjects.erase(guildHouseObjects.begin()+index);
		
	}
	
	return true;
}

BOOL SC_RemoveAllGuildHouseObjects(){
	int size = guildHouseObjects.size();
	for (int i = 0;i<size;i++){ 
		SC_RemoveGuildHouseObject(0);
	}
	guildHouseObjects.clear();
	return true;
}

