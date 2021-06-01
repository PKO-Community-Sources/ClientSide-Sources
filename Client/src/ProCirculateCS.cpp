#include "StdAfx.h"
#include "procirculate.h"
#include "GameApp.h"
#include "PacketCmd.h"
#include "Character.h"
#include "ChaAttr.h"
#include "GameConfig.h"
#include "util2.h"
#include "LoginScene.h"
#include "netprotocol.h"

#ifdef _TEST_CLIENT
#include "..\..\TestClient\testclient.h"
#endif

_DBC_USING

// 协议C->S : 发送行动信息
void CProCirculateCS::BeginAction( CCharacter* pCha, DWORD type, void* param, CActionState* pState )
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_BEGINACTION);
#ifdef _TEST_CLIENT
	CTestClient* pClient = reinterpret_cast<CTestClient*>( g_NetIF->m_connect.GetDatasock()->GetPointer() );
	DWORD dwCharID=(pCha)?pCha->getAttachID():pClient->GetCharID();
	pk.WriteLong(dwCharID);
#else
	pk.WriteLong(pCha->getAttachID());
#endif

    char	szLogName[1024] = { "BeginAction" };

    if( pCha )
    {
        strcpy(szLogName, pCha->getLogName() );            
    }

	try
	{
		LG(szLogName, "$$$PacketID:\t%d\n", pCNetIf->m_ulPacketCount);

#ifdef defPROTOCOL_HAVE_PACKETID
		pk.WriteLong(pCNetIf->m_ulPacketCount++);
#endif
		pk.WriteChar((dbc::uChar)type);
		switch (type)
		{
		case	enumACTION_MOVE:
			{
				stNetMoveInfo *pMove = (stNetMoveInfo *)param;
				pk.WriteSequence((cChar *)pMove->pos_buf ,uShort(sizeof(Point) * pMove->pos_num));
				pCNetIf->SendPacketMessage(pk);

				LG(szLogName, "###Send(Move):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, "Ping:\t%3d\n", pMove->dwAveragePing);
				LG(szLogName, "Point:\t%3d\n", pMove->pos_num);
				for (DWORD i = 0; i < pMove->pos_num; i++)
				{
					LG(szLogName, "\t%d, \t%d\n", pMove->pos_buf[i].x, pMove->pos_buf[i].y);
				}
				LG(szLogName, "\n");
				break;
			}
		case	enumACTION_SKILL:
			{
				stNetSkillInfo *pSkill = (stNetSkillInfo *)param;
				pk.WriteChar(pSkill->chMove);
				pk.WriteChar(pSkill->byFightID);
				if (pSkill->chMove == 2)
				{
					pk.WriteSequence((cChar *)pSkill->SMove.pos_buf, uShort(sizeof(POINT) * pSkill->SMove.pos_num));
				}
				pk.WriteLong(pSkill->lSkillID);
				pk.WriteLong(pSkill->lTarInfo1);
				pk.WriteLong(pSkill->lTarInfo2);

				//for (int n = 0; n < 200; ++n)
				//{
					pCNetIf->SendPacketMessage(pk);
				//}

				LG(szLogName, "###Send(Skill):\tTick:[%d]\n", GetTickCount());
				if (pSkill->chMove == 2)
				{
					LG(szLogName, "Ping:\t%3d\n", pSkill->SMove.dwAveragePing);
					LG(szLogName, "Point:\t%3d\n", pSkill->SMove.pos_num);
					for (DWORD i = 0; i < pSkill->SMove.pos_num; i++)
					{
						LG(szLogName, "\t%d, \t%d\n", pSkill->SMove.pos_buf[i].x, pSkill->SMove.pos_buf[i].y);
					}
				}
				LG(szLogName, "Skill:\t%3d, FightID:%d\n", pSkill->lSkillID, pSkill->byFightID);
				LG(szLogName, "Target:\t%u, \t%u\n", pSkill->lTarInfo1, pSkill->lTarInfo2);
				LG(szLogName, "\n");
				break;
			}
		case	enumACTION_STOP_STATE:
			{
				pk.WriteShort(*((short *)param));
				pCNetIf->SendPacketMessage(pk);

				// log
				LG(szLogName, "###Send(Stop Skill State %d):\tTick:[%d]\n", *((short *)param), GetTickCount());
				LG(szLogName, "\n");
				//
				break;
			}
		case	enumACTION_LEAN: // 倚靠
			{
				stNetLeanInfo *pSLean = (stNetLeanInfo *)param;
				pk.WriteLong(pSLean->lPose);
				pk.WriteLong(pSLean->lAngle);
				pk.WriteLong(pSLean->lPosX);
				pk.WriteLong(pSLean->lPosY);
				pk.WriteLong(pSLean->lHeight);
				pCNetIf->SendPacketMessage(pk);

				// log
				LG(szLogName, "###Send(Lean):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, "\n");
				//
				break;
			}
		case	enumACTION_ITEM_PICK: // 捡道具
			{
				stNetItemPick *pPick = (stNetItemPick *)param;
				pk.WriteLong(pPick->lWorldID);
				pk.WriteLong(pPick->lHandle);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Pick):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_ITEM_THROW: // 丢道具
			{
				stNetItemThrow *pThrow = (stNetItemThrow *)param;
				pk.WriteShort(pThrow->sGridID);
				pk.WriteShort((short)pThrow->lNum);
				pk.WriteLong(pThrow->lPosX);
				pk.WriteLong(pThrow->lPosY);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Throw):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_ITEM_USE:
			{
				stNetUseItem *pUseItem = (stNetUseItem *)param;
				pk.WriteShort(pUseItem->sGridID);
				pk.WriteShort(pUseItem->sTarGridID);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Use Item):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, g_oLangRec.GetString(322), pUseItem->sGridID, pUseItem->sTarGridID);
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_ITEM_UNFIX: // 卸装道具
			{
				stNetItemUnfix *pUnfix = (stNetItemUnfix *)param;
				pk.WriteChar(pUnfix->chLinkID);
				pk.WriteShort(pUnfix->sGridID);
				if (pUnfix->sGridID < 0) // 丢到地上
				{
					pk.WriteLong(pUnfix->lPosX);
					pk.WriteLong(pUnfix->lPosY);
				}
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Unfix):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_ITEM_POS:
			{
				stNetItemPos *pChangePos = (stNetItemPos *)param;
				pk.WriteShort(pChangePos->sSrcGridID);
				pk.WriteShort(pChangePos->sSrcNum);
				pk.WriteShort(pChangePos->sTarGridID);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Item pos):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_ITEM_DELETE:
			{
				stNetDelItem *pDelItem = (stNetDelItem *)param;
				pk.WriteShort(pDelItem->sGridID);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Del Item):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, g_oLangRec.GetString(323), pDelItem->sGridID);
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_ITEM_INFO:
			{
				stNetItemInfo *pItemInfo = (stNetItemInfo *)param;
				pk.WriteChar(pItemInfo->chType);
				pk.WriteShort(pItemInfo->sGridID);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Item Info):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, g_oLangRec.GetString(324), pItemInfo->chType, pItemInfo->sGridID);
                LG(szLogName, "\n");
				break;
				break;
			}
		case	enumACTION_SHORTCUT: // 更新快捷栏
			{
				stNetShortCutChange *pShortcutChange = (stNetShortCutChange *)param;
				pk.WriteChar(pShortcutChange->chIndex);
				pk.WriteChar(pShortcutChange->chType);
				pk.WriteShort(pShortcutChange->shyGrid);
				//pk.WriteByte(pShortcutChange->shyGrid2==-1?0:1);
				//pk.WriteShort(pShortcutChange->shyGrid2);
				pCNetIf->SendPacketMessage(pk);
				break;
			}
		case	enumACTION_LOOK: // 更新外观（如船的换装）
			{
				stNetChangeChaPart *pSChaPart = (stNetChangeChaPart *)param;
				pk.WriteShort(pSChaPart->sTypeID);
				for (int i = 0; i < enumEQUIP_NUM; i++)
				{
					pk.WriteShort(pSChaPart->SLink[i].sID);
				}
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Look):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_TEMP: //临时换装协议
			{
				stTempChangeChaPart *pSTempChaPart = (stTempChangeChaPart *)param;
				pk.WriteLong(pSTempChaPart->dwItemID);
				pk.WriteLong(pSTempChaPart->dwPartID);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Temp):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_EVENT: // 触发事件
			{
				stNetActivateEvent *pEvent = (stNetActivateEvent *)param;
				pk.WriteLong(pEvent->lTargetID);
				pk.WriteLong(pEvent->lHandle);
				pk.WriteShort(pEvent->sEventID);
                pCNetIf->SendPacketMessage(pk);

				// log
				LG(szLogName, "###Send(Event):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, "\n");
				//
				break;
			}
		case	enumACTION_FACE:
			{
				stNetFace	*pNetFace = (stNetFace *)param;
				pk.WriteShort(pNetFace->sAngle);
				pk.WriteShort(pNetFace->sPose);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Face):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_SKILL_POSE:
			{
				stNetFace	*pNetFace = (stNetFace *)param;
				pk.WriteShort(pNetFace->sAngle);
				pk.WriteShort(pNetFace->sPose);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Skill Pos):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, "Angle:%d, Pose:%d\n", pNetFace->sAngle, pNetFace->sPose );
				break;
			}
		case enumACTION_BAGOFHOLDINGNAME:{
			const char* name =static_cast<const char*>(param) ; // (const char*)param;
			pk.WriteString(name);
			pCNetIf->SendPacketMessage(pk);
			break;
		}

		case enumACTION_BAGOFHOLDING:
		case enumACTION_GUILDBANK:
		case	enumACTION_BANK:
			{
				stNetBank	*pNetBank = (stNetBank *)param;
				pk.WriteChar(pNetBank->chSrcType);
				pk.WriteShort(pNetBank->sSrcID);
				pk.WriteShort(pNetBank->sSrcNum);
				pk.WriteChar(pNetBank->chTarType);
				pk.WriteShort(pNetBank->sTarID);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Bank Req):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_CLOSE_BANK:
			{
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Bank Close):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_REQUESTGUILDQUEST:
		case	enumACTION_REQUESTGUILDATTR:
		case	enumACTION_REQUESTGUILDBANK:
		case enumACTION_ENTERGUILDHOUSE:
			{
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(request guild):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_KITBAGTMP_DRAG:	// 拖动临时背包
			{
				stNetTempKitbag* pNetTempKitbag = (stNetTempKitbag*)param;

				pk.WriteShort(pNetTempKitbag->sSrcGridID);
				pk.WriteShort(pNetTempKitbag->sSrcNum);
				pk.WriteShort(pNetTempKitbag->sTarGridID);

				pCNetIf->SendPacketMessage(pk);
				break;
			}
		case enumACTION_GOLDSTORE:{
				int gold=  (int)param;
				pk.WriteLong(gold);
				pCNetIf->SendPacketMessage(pk);
			break;
		}
		default:
			break;
		}
	}
	catch (...)
	{
		MessageBox(0, "!!!!!!!!!!!!!!!!!!!!exception: Begin Action", "error", 0);
	}
}

// 协议C->S : 发送停止行动信息
void CProCirculateCS::EndAction( CActionState* pState )
{
	WPacket pk	=pCNetIf->GetWPacket();

	pk.WriteCmd(CMD_CM_ENDACTION);	//结束行动
	pCNetIf->SendPacketMessage(pk);

	// log
    CCharacter *pMainCha = CGameApp::GetCurScene()->GetMainCha();
	if (pMainCha)
	{
		LG(pMainCha->getLogName(), "###Send(EndAction):\tTick:[%d]\n", GetTickCount());
	}
	//
}

bool CProCirculate::Connect(const char *hostname,unsigned short port,unsigned long timeout)
{
	return pCNetIf->m_connect.Connect(hostname,port,timeout);
}
void CProCirculate::Disconnect(int reason)
{
	pCNetIf->m_connect.Disconnect(reason);
}

void CProCirculate::Login(const char *accounts,const char *password, const char* passport)
{
	strcpy(g_NetIF->m_accounts,accounts);
	strcpy(g_NetIF->m_passwd,password);
	KCHAPc l_chap(accounts,password);

	//if(strlen(g_Config.m_szMD5Pass) == 32) // 配置文件里直接输入MD5值密码登陆, 方便诊断玩家报告的错误
	//{
	//	l_chap.m_strPwdDig = g_Config.m_szMD5Pass; // 中途修改输入密码的MD5值
	//}

	l_chap.SetChapString(g_NetIF->m_chapstr);
	char l_buf[1024];int l_len;
	l_chap.GetStep1Data(l_buf,sizeof(l_buf),l_len);

	extern short g_sClientVer;
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_LOGIN);
	pk.WriteString(passport);
	pk.WriteString(accounts);

	if (g_TomServer.bEnable)
	{
		pk.WriteSequence(password,(uShort)strlen(password));
	}
	else
	{
		//char szBuf[33]={ 0 };
		//md5string(password, szBuf);
		//pk.WriteString(szBuf);

		pk.WriteSequence(l_buf,l_len);
	}

	string strMac=GetMacString();
	if (strMac.empty()) strMac="Unknown";
	pk.WriteString(strMac.c_str());
	pk.WriteShort(911);
	pk.WriteShort(g_sClientVer);

	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::Logout()
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_LOGOUT);
	pCNetIf->SyncSendPacketMessage(pk,3000);
}

void CProCirculate::BeginPlay(const char *cha)
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_BGNPLAY);
	pk.WriteString(cha);

	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::EndPlay()
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_ENDPLAY);
	pCNetIf->SendPacketMessage(pk);
}
void CProCirculate::NewCha(const char *chaname,const char *birth,LOOK &look)
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_NEWCHA);
	pk.WriteString(chaname);
	pk.WriteString(birth);
	pk.WriteSequence(reinterpret_cast<char *>(&look),sizeof(LOOK));

	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::NewCha2(const char *chaname,const char *birth,int type, int hair, int face)
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_NEWCHA);
	pk.WriteString(chaname);
	pk.WriteString(birth);
	pk.WriteLong(type);
	pk.WriteLong(hair);
	pk.WriteLong(face);
	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::DelCha(const char *cha, const char szPassword2[] )
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_DELCHA);
	pk.WriteString(cha);
	pk.WriteString(szPassword2);

	pCNetIf->SendPacketMessage(pk);
}

//------------------------
// 协议C->S : 发送聊天消息
//------------------------
void CProCirculate::OpenRankings()
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_RANK);
	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::Say(const char *content)
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_SAY);			//命令
	pk.WriteSequence(content, uShort(strlen(content))+1);
	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::GuildColour(int colour,int icon){
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CP_GUILDCIRCLECOLOUR);
	pk.WriteLong(colour);
	pk.WriteChar(icon);
	pCNetIf->SendPacketMessage(pk);
}

//----------------------------
// 协议C->S : 发送分配属性的请求
//----------------------------
void CProCirculate::SynBaseAttribute(CChaAttr *pCAttr)
{
	char	chAttrNum = 0;
	for (int i = ATTR_STR; i <= ATTR_LUK; i++)
		if (pCAttr->GetChangeBitFlag(i))
			chAttrNum++;

	if (chAttrNum == 0)
		return;

	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_SYNATTR);	//同步基本属性
	pk.WriteChar(chAttrNum);
	for (int i = ATTR_STR; i <= ATTR_LUK; i++)
	{
		if (pCAttr->GetChangeBitFlag(i))
		{
			pk.WriteShort(i);
			pk.WriteLong((uLong)pCAttr->GetAttr(i));
		}
	}

	// log
	char szReqChangeAttr[256] = {0};
	strcpy(szReqChangeAttr, g_oLangRec.GetString(325));

	for (int i = ATTR_STR; i <= ATTR_LUK; i++)
	{
		if (pCAttr->GetChangeBitFlag(i))
			LG(szReqChangeAttr, g_oLangRec.GetString(326), i, pCAttr->GetAttr(i));
	}
	LG(szReqChangeAttr, "\n");

	pCNetIf->SendPacketMessage(pk);
}

//----------------------------
// 协议C->S : 发送刷新角色属性的请求
//----------------------------
void CProCirculate::RefreshChaData(long lWorldID, long lHandle)
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_REFRESH_DATA);	//同步基本属性
	pk.WriteLong(lWorldID);
	pk.WriteLong(lHandle);

	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::SkillUpgrade(short sSkillID, char chAddLv)
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_SKILLUPGRADE);	//同步基本属性
	pk.WriteShort(sSkillID);
	pk.WriteChar(chAddLv);

	// log
	char szReqChangeAttr[256] = {0};
	strcpy(szReqChangeAttr, g_oLangRec.GetString(327));

	LG(szReqChangeAttr, g_oLangRec.GetString(328), sSkillID, chAddLv);

	pCNetIf->SendPacketMessage(pk);
}

