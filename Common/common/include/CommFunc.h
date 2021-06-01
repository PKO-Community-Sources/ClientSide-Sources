//=============================================================================
// FileName: CommFunc.h
// Creater: ZhangXuedong
// Date: 2005.01.06
// Comment: 
//	2005.4.28	Arcol	add the text filter manager class: CTextFilter
//=============================================================================

#ifndef COMMFUNC_H
#define COMMFUNC_H

#include "CompCommand.h"
#include "SkillRecord.h"
#include "CharacterRecord.h"
#include "ItemRecord.h"
#include "ItemAttrType.h"
#include "JobType.h"
#include "NetRetCode.h"

extern bool KitbagStringConv(short sKbCapacity, std::string &strData);

//=============================================================================
/*---------------------------------------------------------------
* 用途:用于检测创建的角色外观数据是否合法
* nPart - 对应外观ID,nValue - 外观的值
* 返回值：外观数据是否合法。
*/
extern bool g_IsValidLook( int nType, int nPart, long nValue );

/*---------------------------------------------------------------
* ulAreaMask 区域类型
* 返回值：true 海洋。false 陆地
*/
inline bool g_IsSea(unsigned short usAreaMask)
{
	return !(usAreaMask & enumAREA_TYPE_LAND);
}

inline bool g_IsLand(unsigned short usAreaMask)
{
	return (usAreaMask & enumAREA_TYPE_LAND) || (usAreaMask & enumAREA_TYPE_BRIDGE);
}

// 根据传入的左右手道具ID
// 返回可以使用的默认技能,返回-1,没有技能
extern int g_GetItemSkill( int nLeftItemID, int nRightItemID );

extern BOOL IsDist( int x1, int y1, int x2, int y2, DWORD dwDist );

// 是否正确的技能目标
extern int	g_IsRightSkillTar(int nTChaCtrlType, bool bTIsDie, bool bTChaBeSkilled, int nTChaArea,
					  int nSChaCtrlType, int nSSkillObjType, int nSSkillObjHabitat, int nSSkillEffType,
					  bool bIsTeammate, bool bIsFriend, bool bIsSelf);

/*---------------------------------------------------------------
* 参数:左手，右手，身体的道具ID。技能编号。
* 返回值:1-可使用,0-不可使用,-1-技能不存在
*/
extern int g_IsUseSkill( stNetChangeChaPart *pSEquip, int nSkillID );
extern bool g_IsRealItemID(int nItemID);

inline int g_IsUseSkill( stNetChangeChaPart *pSEquip, CSkillRecord *p )
{
	if( !p )  return -1;

	return g_IsUseSkill( pSEquip, p->nID );
}

inline int g_IsUseSeaLiveSkill(long lFitNo, CSkillRecord *p)
{
	if( !p )  return -1;

	for( int i=0; i<defSKILL_ITEM_NEED_NUM; i++ )
	{	
		if( p->sItemNeed[0][i][0] == cchSkillRecordKeyValue )
			break;

		if (p->sItemNeed[0][i][0] == enumSKILL_ITEM_NEED_ID)
		{
			if (p->sItemNeed[0][i][1] == lFitNo)
				return 1;
		}
	}

	return 0;
}

inline bool g_IsPlyCtrlCha(int nChaCtrlType)
{
	if (nChaCtrlType == enumCHACTRL_PLAYER || nChaCtrlType == enumCHACTRL_PLAYER_PET)
		return true;
	return false;
}

inline bool g_IsMonsCtrlCha(int nChaCtrlType)
{
	if (nChaCtrlType == enumCHACTRL_MONS
		|| nChaCtrlType == enumCHACTRL_MONS_TREE
		|| nChaCtrlType == enumCHACTRL_MONS_MINE
		|| nChaCtrlType == enumCHACTRL_MONS_FISH
		|| nChaCtrlType == enumCHACTRL_MONS_DBOAT
		|| nChaCtrlType == enumCHACTRL_MONS_REPAIRABLE
		)
		return true;
	return false;
}

inline bool g_IsNPCCtrlCha(int nChaCtrlType)
{
	if (nChaCtrlType == enumCHACTRL_NPC || nChaCtrlType == enumCHACTRL_NPC_EVENT)
		return true;
	return false;
}

inline bool g_IsChaEnemyCtrlSide(int nSCtrlType, int nTCtrlType)
{
	if (g_IsPlyCtrlCha(nSCtrlType) & g_IsPlyCtrlCha(nTCtrlType))
		return false;
	if (g_IsMonsCtrlCha(nSCtrlType) & g_IsMonsCtrlCha(nTCtrlType))
		return false;
	return true;
}

inline bool g_IsFileExist(const char *szFileName)
{
    FILE	*fp = NULL;
    if (NULL == (fp = fopen(szFileName,"rb")))
        return false;
    if (fp) fclose(fp);
    return true;
}

extern char* LookData2String(const stNetChangeChaPart *pLook, char *szLookBuf, int nLen, bool bNewLook = true);
extern bool Strin2LookData(stNetChangeChaPart *pLook, std::string &strData);
extern char* ShortcutData2String(const stNetShortCut *pShortcut, char *szShortcutBuf, int nLen);
extern bool String2ShortcutData(stNetShortCut *pShortcut, std::string &strData);


inline long g_ConvItemAttrTypeToCha(long lItemAttrType)
{
	if (lItemAttrType >= ITEMATTR_COE_STR && lItemAttrType <= ITEMATTR_COE_PDEF)
		return lItemAttrType + (ATTR_ITEMC_STR - ITEMATTR_COE_STR);
	else if (lItemAttrType >= ITEMATTR_VAL_STR && lItemAttrType <= ITEMATTR_VAL_PDEF)
		return lItemAttrType + (ATTR_ITEMV_STR - ITEMATTR_VAL_STR);
	else
		return 0;
}

// 对应区域类型的参数个数
inline short g_GetRangeParamNum(char RangeType)
{
	short	sParamNum = 0;
	switch (RangeType)
	{
	case	enumRANGE_TYPE_STICK:
		sParamNum = 2;
		break;
	case	enumRANGE_TYPE_FAN:
		sParamNum = 2;
		break;
	case	enumRANGE_TYPE_SQUARE:
		sParamNum = 1;
		break;
	case	enumRANGE_TYPE_CIRCLE:
		sParamNum = 1;
		break;
	}

	return sParamNum + 1;
}

//=============================================================================
// chChaType 角色类型
// chChaTerrType 角色活动空间的类型
// bIsBlock 是否障碍
// ulAreaMask 区域类型
// 返回值：true 可在该单元上移动。false 不可移动
//=============================================================================
inline bool	g_IsMoveAble(char chChaCtrlType, char chChaTerrType, unsigned short usAreaMask)
{
	bool	bRet1 = false;
	if (chChaTerrType == defCHA_TERRITORY_DISCRETIONAL)
		bRet1 = true;
	else if (chChaTerrType == defCHA_TERRITORY_LAND)
	{
		if (usAreaMask & enumAREA_TYPE_LAND || usAreaMask & enumAREA_TYPE_BRIDGE)
			bRet1 = true;
	}
	else if (chChaTerrType == defCHA_TERRITORY_SEA)
	{
		if (!(usAreaMask & enumAREA_TYPE_LAND))
			bRet1 = true;
	}

	bool	bRet2 = true;
	if (usAreaMask & enumAREA_TYPE_NOT_FIGHT) // 非战斗区域
	{
		if (g_IsMonsCtrlCha(chChaCtrlType))
			bRet2 = false;
	}

	return bRet1 && bRet2;
}

inline const char* g_GetJobName(short sJobID)
{
	if (sJobID < 0 || sJobID >= MAX_JOB_TYPE)
		return g_szJobName[0];

	return g_szJobName[sJobID];
}

inline short g_GetJobID(const char *szJobName)
{
	for (short i = 0; i < MAX_JOB_TYPE; i++)
	{
		if (!strcmp(g_szJobName[i], szJobName))
			return i;
	}

	return 0;
}

inline const char* g_GetCityName(short sCityID)
{
	if (sCityID < 0 || sCityID >= defMAX_CITY_NUM)
		return "";

	return g_szCityName[sCityID];
}

inline short g_GetCityID(const char *szCityName)
{
	for (short i = 0; i < defMAX_CITY_NUM; i++)
	{
		if (!strcmp(g_szCityName[i], szCityName))
			return i;
	}

	return -1;
}

inline bool g_IsSeatPose( int pose )
{
	return 16==pose;
}

// 引发表现返回真
inline bool g_IsValidFightState( int nState )
{
	return nState < enumFSTATE_TARGET_NO;
}

inline bool g_ExistStateIsDie(char chState)
{
	if (chState >= enumEXISTS_WITHERING)
		return true;

	return false;
}

inline const char* g_GetItemAttrExplain( int v )
{
    switch( v )
    {
    case ITEMATTR_COE_STR: return "Strength Bonus";// "力量加成"; 
    case ITEMATTR_COE_AGI: return "Agility Bonus";// "敏捷加成"; 
    case ITEMATTR_COE_DEX: return "Accuracy Bonus";// "专注加成"; 
    case ITEMATTR_COE_CON: return "Constitution Bonus";// "体质加成"; 
    case ITEMATTR_COE_STA: return "Spirit Bonus";// "精神加成"; 
    case ITEMATTR_COE_LUK: return "Luck Bonus";// "幸运加成"; 
    case ITEMATTR_COE_ASPD: return "Attack Rate Bonus";// "攻击频率加成"; 
    case ITEMATTR_COE_ADIS: return "Attack range Bonus";// "攻击距离加成"; 
    case ITEMATTR_COE_MNATK: return "Minimum attack Bonus";// "最小攻击力加成"; 
    case ITEMATTR_COE_MXATK: return "Maximum attack Bonus";// "最大攻击力加成"; 
    case ITEMATTR_COE_DEF: return "Defense Bonus";// "防御加成"; 
    case ITEMATTR_COE_MXHP: return "Maximum HP Bonus";// "最大HP加成"; 
    case ITEMATTR_COE_MXSP: return "Maximum SP Bonus";// "最大SP加成"; 
    case ITEMATTR_COE_FLEE: return "Dodge rate Bonus";// "闪避率加成"; 
    case ITEMATTR_COE_HIT: return "Hit rate Bonus";// "命中率加成"; 
    case ITEMATTR_COE_CRT: return "Critical hitrate Bonus";// "爆击率加成"; 
    case ITEMATTR_COE_MF: return "Drop rate Bonus";// "寻宝率加成"; 
    case ITEMATTR_COE_HREC: return "HP recovery speed Bonus";// "HP恢复速度加成"; 
    case ITEMATTR_COE_SREC: return "SP Recovery rate Bonus";// "SP恢复速度加成"; 
    case ITEMATTR_COE_MSPD: return "Movement speed Bonus";// "移动速度加成"; 
    case ITEMATTR_COE_COL: return "Material Mining Speed Bonus";// "资源采集速度加成"; 

    case ITEMATTR_VAL_STR: return "Strength Bonus";// "力量加成"; 
    case ITEMATTR_VAL_AGI: return "Agility Bonus";// "敏捷加成"; 
    case ITEMATTR_VAL_DEX: return "Accuracy Bonus";// "专注加成"; 
    case ITEMATTR_VAL_CON: return "Constitution Bonus";// "体质加成"; 
    case ITEMATTR_VAL_STA: return "Spirit Bonus";// "精神加成"; 
    case ITEMATTR_VAL_LUK: return "Luck Bonus";// "幸运加成"; 
    case ITEMATTR_VAL_ASPD: return "Attack Rate Bonus";// "攻击频率加成"; 
    case ITEMATTR_VAL_ADIS: return "Attack range Bonus";// "攻击距离加成"; 
    case ITEMATTR_VAL_MNATK: return "Minimum attack Bonus";// "最小攻击力加成"; 
    case ITEMATTR_VAL_MXATK: return "Maximum attack Bonus";// "最大攻击力加成"; 
    case ITEMATTR_VAL_DEF: return "Defense Bonus";// "防御加成"; 
    case ITEMATTR_VAL_MXHP: return "Maximum HP Bonus";// "最大HP加成"; 
    case ITEMATTR_VAL_MXSP: return "Maximum SP Bonus";// "最大SP加成"; 
    case ITEMATTR_VAL_FLEE: return "Dodge rate Bonus";// "闪避率加成"; 
    case ITEMATTR_VAL_HIT: return "Hit rate Bonus";// "命中率加成"; 
    case ITEMATTR_VAL_CRT: return "Critical hitrate Bonus";// "爆击率加成"; 
    case ITEMATTR_VAL_MF: return "Drop rate Bonus";// "寻宝率加成"; 
    case ITEMATTR_VAL_HREC: return "HP recovery speed Bonus";// "HP恢复速度加成"; 
    case ITEMATTR_VAL_SREC: return "SP Recovery rate Bonus";// "SP恢复速度加成"; 
    case ITEMATTR_VAL_MSPD: return "Movement speed Bonus";// "移动速度加成"; 
    case ITEMATTR_VAL_COL: return "Material Mining Speed Bonus";// "资源采集速度加成"; 

	case ITEMATTR_VAL_PDEF: return "Physical resist Bonus";// "物理抵抗加成";
    case ITEMATTR_MAXURE: return "Max durability";// "最大耐久度"; 
    case ITEMATTR_MAXENERGY: return "Max energy";// "最大能量"; 
    default: return "Unknown tools characteristics";// "未知道具属性";
    }
}

inline const char* g_GetServerError( int error_code )  // 解析错误码
{
	switch( error_code )
	{
	case ERR_AP_INVALIDUSER:return "Invalid Account";// "无效账号";
	case ERR_AP_INVALIDPWD:	return "Password incorrect";// "密码错误";
	case ERR_AP_ACTIVEUSER: return "Account activation failed";// "激活用户失败";
    case ERR_AP_DISABLELOGIN: return "Your cha is currently in logout save mode, please try logging in again later.";// "目前您的角色正处于下线存盘过程中，请稍后再尝试登录。";
    case ERR_AP_LOGGED: return "This account is already online";// "此帐号已处于登录状态";
    case ERR_AP_BANUSER: return "Account has been banned";// "帐号已封停";
	case ERR_AP_GPSLOGGED: return "This GroupServer has login";// "此GroupServer已登录";
	case ERR_AP_GPSAUTHFAIL: return "This GroupServer Verification failed";// "此GroupServer认证失败";
	case ERR_AP_SAVING:	return "Saving your character, please try again in 15 seconds...";// "正在保存你的角色，请15秒后重试...";
    case ERR_AP_LOGINTWICE: return "Your account is logged on far away";// "你的账号在远处再次登录";
    case ERR_AP_ONLINE: return "Your account is already online";// "你的账号已在线";
	case ERR_AP_DISCONN: return "GroupServer disconnected";// "GroupServer已断开";
	case ERR_AP_UNKNOWNCMD: return "unknown agreement, don\'t deal with";// "未知协议，不处理";
	case ERR_AP_TLSWRONG: return "local saving error";// "本地存储错误";
	case ERR_AP_NOBILL:	return "This account has expired, please topup!";// "此账号已过期，请充值！";

	case ERR_PT_LOGFAIL:	return "GateServer to GroupServer login failed";// "GateServer向GroupServer的登录失败";
	case ERR_PT_SAMEGATENAME:return "GateServer and login GateServer have similar name";// "GateServer与已登录GateServer重名";

	case ERR_PT_INVALIDDAT:return "Ineffective data model";// "无效的数据格式";
	case ERR_PT_INERR: return "server link operation integrality error ";// "服务器之间的操作完整性错误";
	case ERR_PT_NETEXCP: return "Account server has encountered a malfunction";// "帐号服务器故障";	// GroupServer发现的到AccuntServer的网络故障
	case ERR_PT_DBEXCP: return "database server malfunction";// "数据库服务器故障";	// GroupServer发现的到Database的故障
	case ERR_PT_INVALIDCHA: return "Current account does not have a request (Select/Delete) to character";// "当前帐号不拥有请求(选择/删除)的角色";
	case ERR_PT_TOMAXCHA: return "reached the maximum number of characters you can create";// "已经达到最多能创建的角色数了";
	case ERR_PT_SAMECHANAME: return "Character name already exist";// "重复的角色名";
	case ERR_PT_INVALIDBIRTH: return "illegal birth place";// "非法的出生地";
	case ERR_PT_TOOBIGCHANM:return "Character name is too long";// "角色名太长";
	case ERR_PT_ISGLDLEADER:return "Guild must have a leader, please disband your guild first then delete your character";// "公会不可一日无长，请先解散公会再删除角色";
	case ERR_PT_ERRCHANAME:return "Illegal character name";// "非法的角色名称";
	case ERR_PT_SERVERBUSY: return "System is busy, please try again later";// "系统忙，请稍后再试";
	case ERR_PT_TOOBIGPW2: return "second code length illegal";// "二次密码长度非法";
	case ERR_PT_INVALID_PW2: return "Cha second password not created";// "未创建角色保护二次密码";
	case ERR_PT_BADBOY:	return "My child, you are very bold. You have been reported to the authority. Please do not commit the offense again!";// "孩子，你很BT，已经自动对你作出了通报批评，要引以为戒，不可再犯！";

	case ERR_MC_NETEXCP: return "Discovered exceptional line error on GateServer";// "GateServer发现的网络异常";
	case ERR_MC_NOTSELCHA: return "current not yet handled character state";// "当前未处于选择角色状态";
	case ERR_MC_NOTPLAY: return "Currently not in gameplay, unable to send ENDPLAY command";// "当前不处于玩游戏状态，不能发送ENDPLAY命令";
	case ERR_MC_NOTARRIVE: return "target map cannot be reached";// "目标地图不可到达";
	case ERR_MC_TOOMANYPLY: return "This server is currently full, please select another server!";// "本服务器组人数已满, 请选择其它组进行游戏!";
	case ERR_MC_NOTLOGIN: return "Youa re not login";// "你尚未登陆";
	case ERR_MC_VER_ERROR: return "Client version error, server refused connection!";// "客户端的版本号错误,服务器拒绝登录！";
	case ERR_MC_ENTER_ERROR: return "failed to enter map!";// "进入地图失败！";
	case ERR_MC_ENTER_POS: return "Map position illegal, you\'ll be sent back to your birth city, please relog!";// "地图位置非法，您将被送回出生城市，请重新登陆！";

	case ERR_TM_OVERNAME: return "GameServer name repeated";// "GameServer名重复";
	case ERR_TM_OVERMAP: return "GameServerMapNameRepeated";// "GameServer上地图名重复";
	case ERR_TM_MAPERR: return "GameServer map assign language error";// "GameServer地图配置语法错误";

	case ERR_SUCCESS:return "Jack is too BT, correct also will ask me if anything is wrong!";// "Jack太BT了，正确也来问我什么错误！";
	default:
		{
			int l_error_code	=error_code;
			l_error_code	/=500;
			l_error_code	*=500;
			static char l_buffer[500];
			char		l_convt[20];
			switch ( l_error_code )
			{
			case ERR_MC_BASE:return strcat(strcpy(l_buffer,itoa(error_code,l_convt,10)),"(GameServer/GateServer->Client return error code space 1-500)"); //"(GameServer/GateServer->Client返回的错误码空间1－500)");
			case ERR_PT_BASE: return strcat(strcpy(l_buffer,itoa(error_code,l_convt,10)),"(GroupServer->GateServer return error code range 501-1000)"); //"(GroupServer->GateServer返回的错误码空间501－1000)"); 
			case ERR_AP_BASE: return strcat(strcpy(l_buffer,itoa(error_code,l_convt,10)),"(AccountServer->GroupServe return error code from 1001-1500)"); //"(AccountServer->GroupServer返回的错误码空间1001－1500)");
			case ERR_MT_BASE: return strcat(strcpy(l_buffer,itoa(error_code,l_convt,10)),"(GameServer->GateServer return error code range 1501-2000)"); //"(GameServer->GateServer返回的错误码空间1501－2000)");
			default:return strcat(strcpy(l_buffer,itoa(error_code,l_convt,10)),"(Jack is too insane, he made a mistake that I don\'t even know.)"); //"(Jack太BT了，弄个错误连我都不认识。)");
			}
		}
	}
}


inline bool isNumeric(const char *name, unsigned short len){
	const unsigned char *l_name = reinterpret_cast<const unsigned char *>(name);
	if (len == 0){
		return false;
	}
	for (unsigned short i = 0; i < len; i++){
		if (!l_name[i] || !isdigit(l_name[i])){
			return false;
		}
	}
	return true;
}

inline bool isAlphaNumeric(const char *name, unsigned short len){
	const unsigned char *l_name = reinterpret_cast<const unsigned char *>(name);
	if (len == 0){
		return false;
	}
	for (unsigned short i = 0; i < len; i++){
		if (!l_name[i] || !isalnum(l_name[i])){
			return false;
		}
	}
	return true;
}

//本函数功能包括检查字符串中GBK双字节汉字字符的完整性、网络包中字符串的完整性等。
//name为只允许有大小写字母数字和汉字（去除全角空格）才返回true;
//len参数为字符串name的长度=strlen(name),不包括结尾NULL字符。
inline bool IsValidName(const char *name,unsigned short len)
{
	const unsigned char *l_name =reinterpret_cast<const unsigned char *>(name);
	bool l_ishan	=false;
	//if (len == 0)
	//	return 0;
	for(unsigned short i=0;i<len;i++)
	{
		if(!l_name[i])
		{
			return false;
		}else if(l_ishan)
		{
			if(l_name[i-1] ==0xA1 && l_name[i] ==0xA1)	//过滤全角空格
			{
				return false;
			}
			if(l_name[i] >0x3F && l_name[i] <0xFF && l_name[i] !=0x7F)
			{
				l_ishan =false;
			}else
			{
				return false;
			}
		}else if(l_name[i]>0x80 && l_name[i] <0xFF)
		{
			l_ishan	=true;
		}else if((l_name[i] >='A' && l_name[i] <='Z') ||(l_name[i] >='a' && l_name[i] <='z') ||(l_name[i] >='0' && l_name[i] <='9'))
		{
			
		}else
		{
			return false;
		}
	}
	return !l_ishan;
}

inline const char* g_GetUseItemFailedInfo(short sErrorID)
{
	switch (sErrorID)
	{
	case	enumITEMOPT_SUCCESS:
		return "Item operation succesful";// "道具操作成功";
		break;
	case	enumITEMOPT_ERROR_NONE:
		return "Equipment does not exist";// "道具不存在";
		break;
	case	enumITEMOPT_ERROR_KBFULL:
		return "Inventory is full";// "道具栏已满";
		break;
	case	enumITEMOPT_ERROR_UNUSE:
		return "Failed to use item";// "道具使用失败";
		break;
	case	enumITEMOPT_ERROR_UNPICKUP:
		return "礡l??荒蹺癈?";// "道具不能拾取";
		break;
	case	enumITEMOPT_ERROR_UNTHROW:
		return "Item cannot be thrown";// "道具不能丢弃";
		break;
	case	enumITEMOPT_ERROR_UNDEL:
		return "Item cannot be destroyed";// "道具不能销毁";
		break;
	case	enumITEMOPT_ERROR_KBLOCK:
		return "inventory is currently locked";// "道具栏处于锁定状态";
		break;
	case	enumITEMOPT_ERROR_DISTANCE:
		return "Distance too far";// "距离太远";
		break;
	case	enumITEMOPT_ERROR_EQUIPLV:
		return "Equipment level mismatch";// "装备等级不满足";
		break;
	case	enumITEMOPT_ERROR_EQUIPJOB:
		return "Does not meet the class requirement for the equipment";// "装备职业不满足";
		break;
	case	enumITEMOPT_ERROR_STATE:
		return "Unable to operate items under the current condition";// "该状态下不能操作道具";
		break;
	case	enumITEMOPT_ERROR_PROTECT:
		return "Item is being protected";// "道具被保护";
		break;
	case	enumITEMOPT_ERROR_AREA:
		return "different region type";// "不同的区域类型";
		break;
	case	enumITEMOPT_ERROR_BODY:
		return "type of build does not match";// "体型不匹配";
		break;
	case	enumITEMOPT_ERROR_TYPE:
		return "Unable to store this item";// "此道具无法存放";
		break;
	case	enumITEMOPT_ERROR_INVALID:
		return "Item not in used";// "无效的道具";
		break;
	case	enumITEMOPT_ERROR_KBRANGE:
		return "out of inventory range";// "超出道具栏范围";
		break;
	default:
		return "Unknown item usage failure code";// "未知的道具操作失败码";
		break;
	}
}

class CTextFilter
{
public:
#define eTableMax 5
	enum eFilterTable{NAME_TABLE=0, DIALOG_TABLE=1, MAX_TABLE=eTableMax};
	/*
	* Warning : Do not use MAX_TABLE enum value, it just use for the maximum limit definition,
	*			If you want to expand this enum table value more than the default number eTableMax(5),
	*			please increase the eTableMax definition
	*/

	CTextFilter();
	~CTextFilter();
	static bool Add(const eFilterTable eTable, const char *szFilterText);
	static bool IsLegalText(const eFilterTable eTable, const string strText);
	static bool Filter(const eFilterTable eTable, string &strText);
	static bool LoadFile(const char *szFileName, const eFilterTable eTable=NAME_TABLE);
	static BYTE* GetNowSign(const eFilterTable eTable);

private:
	static bool ReplaceText(string &strText, const string *pstrFilterText);
	static bool bCheckLegalText(const string &strText, const string *pstrIllegalText);

	static vector<string> m_FilterTable[eTableMax];
	static BYTE m_NowSign[eTableMax][8];
};

#endif // COMMFUNC_H