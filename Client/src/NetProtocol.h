#pragma once
#include "GameCommon.h"
#include "Point.h"
#include "Kitbag.h"
#include "SkillStateType.h"
#include "Tools.h"
#include "Shipset.h"
#include "uigoodsgrid.h"

class CActor;
class CCharacter;
class CSceneItem;
struct xShipBuildInfo;
struct BOAT_BERTH_DATA;

#define defMOVE_LIST_POINT_NUM	32
struct SMoveList
{
	POINT	SPos[defMOVE_LIST_POINT_NUM];
	int		nPointNum;
	DWORD   dwState;
};

#define defMAX_POS_NUM	32

struct stNetMoveInfo // enumACTION_MOVE
{
	DWORD dwAveragePing; // 单位：毫秒，通过 NetIF::GetAveragePing() 获得，此值乘以速度就是预移动的距离
	POINT pos_buf[defMAX_POS_NUM]; // 移动拐点序列（包含当前位置）
	DWORD pos_num; // 有效点的个数
};

struct stNetSkillInfo // enumACTION_SKILL
{
	BYTE	byFightID;
	char	chMove;		// 1，直接使用技能。2，移动到位置再使用技能

	long	lSkillID;		// 0，物理攻击。>0，魔法攻击

	// 若目标是实体，则分别表示WorldID,Handle。若目标是坐标，则分别表示坐标的x,y
	struct
	{
		long	lTarInfo1;
		long	lTarInfo2;
	};

	stNetMoveInfo	SMove;
};

struct stNetNotiMove // enumACTION_MOVE
{
	short	sState;					// 状态（参见CompCommand.h EMoveState）。
	short	sStopState;				// 停止状态enumEXISTS_WAITING或enumEXISTS_SLEEPING
	POINT	SPos[defMAX_POS_NUM];	// 移动拐点序列（包含当前位置）
	long	nPointNum;				// 有效点的个数
};

struct stEffect
{
	long	lAttrID;	// 影响的属性ID
	LONG64	lVal;		// 影响的数值
};

struct stSkillState
{
	BYTE	chID;
	BYTE	chLv;       // 为零删除这个状态,大于零填加这个状态
	unsigned long		lTimeRemaining;
};

struct stAreaSkillState
{
	BYTE	chID;
	BYTE	chLv;
	long	lWorldID;	// 施加者的唯一标识
	unsigned char	uchFightID;
};

struct stNetNotiSkillRepresent //技能表现 enumACTION_SKILL_SRC
{
	BYTE	byFightID;
	short	sAngle;         // 方向，server通告
	short	sState;			// 状态（参见CompCommand.h EFightState）
	short	sStopState;		// 停止状态enumEXISTS_WAITING或enumEXISTS_SLEEPING
	char	chCrt;          // 0，没有爆击。1，存在爆击

	long	lSkillID;		// 技能编号
	long	lSkillSpeed;	// 技能使用频率（毫秒）
	char	chObjType;		// 目标类型：0，实体。1，坐标
	long	lTargetID;		// 目标ID
	POINT	STargetPoint;	// 目标点
	short	sExecTime;	    // 作用次数,大于等于零

	CSizeArray<stEffect>		SEffect;	// 影响的结果序列
	CSizeArray<stSkillState>	SState;		// 技能状态
};

struct stNetNotiSkillEffect // 技能的作用结果 enumACTION_SKILL_TAR
{
	BYTE	byFightID;
	short	sState;			// 状态（参见CompCommand.h EFightState）
	bool	bDoubleAttack;	// 双倍攻击
	bool	bMiss;			// Miss;
	bool	bBeatBack;		// 是否击退
	Point	SPos;			// 击退后的位置
	long	lSkillID;		// 使用的技能ID
	char	chObjType;		// 目标类型：0，实体。1，坐标
	long	lSrcID;			// 使用方的角色ID
	Point	SSrcPos;		// 使用方的位置
	Point	SSkillTPos;		// 技能落点
	short	sExecTime;		// 作用次数,为一

	// 自身
	CSizeArray<stEffect>		SEffect;	// 影响的结果序列
	CSizeArray<stSkillState>	SState;		// 技能状态

	// 技能源
	short						sSrcState;	// 状态（参见CompCommand.h EFightState）
	CSizeArray<stEffect>		SSrcEffect;	// 影响的结果序列
	CSizeArray<stSkillState>	SSrcState;	// 技能状态
};

struct stNetPKCtrl
{
	bool	bInGymkhana;	// 在竞技场，此时不允许设置bInPK
	bool	bInPK;			// 在PK

	void	Exec( CCharacter* pCha );
	void	Exec( unsigned long	ulWorldID );
};

struct stNetChaSideInfo
{
	char	chSideID;
};

struct stNetAppendLook
{
	short	sLookID[defESPE_KBGRID_NUM];
	bool	bValid[defESPE_KBGRID_NUM];

	void	Exec(unsigned long	ulWorldID);
	void	Exec(CCharacter* pCha);
};

class CSceneNode;
class CEvent;
struct stNetEvent
{
	long			lEntityID;
	char			chEntityType;					// 1-角色,2-道具
	unsigned short	usEventID;
	const char*		cszEventName;

	CEvent*			ChangeEvent();					// 改变事件属性

	CEvent*			Exec( CSceneNode* pNode );		// 创建事件

};

struct stNetLookInfo
{
	char	chSynType;	// 参见 ESynLookType，当值为enumSYN_LOOK_CHANGE时，只有SLook.sID的值大于0，才更新该位置
	stNetChangeChaPart	SLook;
};

struct stNetActorCreate				// 创建角色的信息
{
	stNetActorCreate() : ulWorldID(0), lHandle(0), ulChaID(0), chCtrlType(0), sAngle(0), ulTLeaderID(0), sState(0)
		, sIcon(0), chSeeType(enumENTITY_SEEN_NEW){}

	unsigned long	ulWorldID;
	unsigned long	ulCommID;		// 通信ID
	const char		*szCommName;	// 通行角色名
	long			lHandle;		// 服务器的信息，原值返回
	unsigned long	ulChaID;
	char			chCtrlType;		// 控制类型（玩家，NPC，怪物等。参考CompCommand.h EChaCtrlType）
	int			chGuildPermission;		// 控制类型（玩家，NPC，怪物等。参考CompCommand.h EChaCtrlType）
	int			lGuildCircleColour;
	int			chGuildIcon;
	Circle			SArea;
	short			sAngle;			// 方向，server通告
	unsigned long	ulTLeaderID;	// 队长ID，没有为0
	short			sState;			// 状态 0x00，生活中。0x01，死亡中。0x02，复活中
	string 			strMottoName;
	short			sIcon;
	long			lGuildID;
	string 			strGuildName;
	string 			strGuildMotto;
	string			strStallName;
	char			*szName;
	char			chSeeType;		// 出现的类型,EEntitySeenType
	char			chGMLv;			// GM等级
	int 			chIsPlayer;
	
	
	stNetChaSideInfo	SSideInfo;	// 分边信息
	stNetLookInfo		SLookInfo;
	stNetPKCtrl		SPKCtrl;
	stNetEvent		SEvent;
	stNetAppendLook	SAppendLook;

	char			chMainCha;		// 0-其它角色,1-为主角,2-主角的道具

	CCharacter*		CreateCha();
	void			SetValue( CCharacter* pCha );
};

struct stNetNPCShow
{
	BYTE			byNpcType;	 // 视野出现时 
    BYTE            byNpcState;  // 如果是NPC,则附带NPC状态

	void			SetNpcShow( CCharacter* pCha );
};

struct stNetLeanInfo // 倚靠
{
	char	chState;

	long	lPose;
	long	lAngle;
	long	lPosX, lPosY;
	long	lHeight;
};

struct stNetSwitchMap // 角色复活的信息
{
	short	sEnterRet;
	char	*szMapName;
	char	chEnterType;
	bool	bIsNewCha;
	bool	bCanTeam;	// 地图是否可操作队伍
};

// 说话
struct stNetSysInfo		// 系统信息：提示某人不在线上等
{
	const char *m_sysinfo;		// 系统信息内容
};

struct stNetSay			// 视野内说话
{
	unsigned long	m_srcid;	// 说话人的ID
	const char	*	m_content;	// 内容
};

// 拾取道具流程
// 1.客户端拾取到道具,发送拾取道具到服务器
// 2.服务器通知主客户端,道具栏发生变化
// 3.服务器通知所有客户端,地面上道具消失
// 
// 
// 装备道具
// 1.在道具栏选择道具拖动到装备栏,发送装备协议
//    成功:1.通知所有客户端外观发生变化,同时主客户端更新相应装备栏
//         2.通知主客户端道具栏发生变化

struct stNetItemCreate	// 物品出现，消失
{
	long	lWorldID;
	long	lHandle;		// 服务器信息，原值返回
	long	lEntityAddr;	// 服务端的实体地址
	long	lID;
	Point	SPos;
	short	sAngle;
	short	sNum;			// 物品个数
	char	chAppeType;		// 出生类型（参见CompCommand.h EItemAppearType）
	long	lFromID;		// 抛出源的ID，如果为0，则不存在抛出源

	stNetEvent	SEvent;
};

struct stNetItemUnfix	// 道具卸装
{
	char    chLinkID;	// Link点
	short	sGridID;	// 大于等于零,丢弃到物品栏的位置,-1,丢弃到物品栏,不指定位置,-2,丢弃在地面
    long	lPosX;		// 丢弃在地面的位置
    long	lPosY;
};

struct stNetItemPick	// 道具的拾取
{
	long	lWorldID;
	long	lHandle;
};

struct stNetItemThrow	// 道具的丢弃
{
	short	sGridID;	// 物品栏位置
	long	lNum;		// 丢弃物品的个数
	long	lPosX;		// 丢弃在地面的位置
	long	lPosY;
};

struct stNetItemPos		// 道具改变在栏中的位置
{
	short	sSrcGridID;	// 物品栏位置
	short	sSrcNum;	// 源物品的数量,如果为零，则是全部物品交换
	short	sTarGridID;	// 物品栏位置
};

struct stNetBank		// 银行交互
{
	char	chSrcType;	// 源格类型 0，道具栏 1，银行
	short	sSrcID;		// 物品栏位置
	short	sSrcNum;	// 物品操作数
	char	chTarType;	// 目标格类型
	short	sTarID;		// 物品栏位置
};

struct stNetTempKitbag	// 临时背包拖至背包
{
	short   sSrcGridID;	// 临时背包位置
	short	sSrcNum;	// 拖动个数
	short	sTarGridID;	// 背包位置
};

struct stNetUseItem		// 使用道具
{
	stNetUseItem() {sTarGridID = -1;}
	short	sGridID;	// 物品栏位置
	short	sTarGridID;	// 目标位置（用于给精灵喂食等情况）
};

struct stNetDelItem		// 删除道具
{
	short	sGridID;	// 物品栏位置
};

struct stNetItemInfo	// 道具信息
{
	char	chType;
	short	sGridID;	// 物品栏位置
};

struct stTempChangeChaPart
{
	DWORD			dwPartID;
	DWORD           dwItemID;
};

struct stNetKitbag				    // 整个道具栏更新
{
	char	chBagType;				// 0，道具栏 1，银行
	char	chType;					// 更新类型（参考CompCommand.h的ESynKitbagType）
    int     nKeybagNum;             // 道具栏格子数
    struct stGrid
    {
	    short	    sGridID;		// 表格ID
	    SItemGrid	SGridContent;
    };
    int     nGridNum;               // 有效的格子数
    stGrid  Grid[defMAX_KBITEM_NUM_PER_TYPE];     
};

struct stNetSkillBag			    // 更新技能栏
{
	char			chType;			// 更新类型（参考CompCommand.h的ESynSkillBagType）
	CSizeArray<SSkillGridEx>		SBag;	// 技能内容
};

struct stNetDefaultSkill
{
	short	sSkillID;

	void	Exec(void);
};

struct stNetSkillState				// 更新技能状态
{
	char	chType;					// 更新类型（未定义）
	CSizeArray<stSkillState> SState;
};

struct stNetChangeCha				// 更换角色协议
{
	unsigned long	ulMainChaID;	// 主角ID
};

struct stNetActivateEvent
{
	long	lTargetID;
	long	lHandle;
	short	sEventID;
};

struct stNetFace                    // 客户端换表情,对应enumACTION_FACE
{
    short   sPose;              
    short	sAngle;
};

struct stLookEnergy
{
	short	sEnergy[enumEQUIP_NUM];
};

// 对话窗口信息
typedef struct _NET_FUNCITEM
{
	char szFunc[ROLE_MAXNUM_FUNCITEMSIZE];
} NET_FUNCITEM, *PNET_FUNCITEM;

typedef struct _NET_MISITEM
{
	char szMis[ROLE_MAXNUM_FUNCITEMSIZE];
	BYTE byState;

} NET_MISITEM, *PNET_MISITEM;

typedef struct _NET_FUNCPAGE
{
	char szTalk[ROLE_MAXNUM_DESPSIZE];
	NET_FUNCITEM FuncItem[ROLE_MAXNUM_FUNCITEM];
	NET_MISITEM  MisItem[ROLE_MAXNUM_CAPACITY];

} NET_FUNCPAGE, *PNET_FUNCPAGE;

// 任务列表信息
typedef struct _NET_MISSIONLIST
{
	BYTE byListType;
	BYTE byPrev;
	BYTE byNext;
	BYTE byPrevCmd;
	BYTE byNextCmd;
	BYTE byItemCount;
	NET_FUNCPAGE FuncPage;

} NET_MISSIONLIST, *PNET_MISSIONLIST;


#define HELPINFO_DESPSIZE 4096

// 显示图形，文字，播放声音
typedef struct _NET_HELPINFO
{
	BYTE byType;
	union
	{
		char szDesp[HELPINFO_DESPSIZE];//[ROLE_MAXNUM_DESPSIZE];
		USHORT sID;
	};

} NET_HELPINFO, *PNET_HELPINFO;

// 任务页信息
typedef struct _NET_MISNEED
{
	BYTE byType;
	union
	{
		struct
		{
			WORD wParam1;
			WORD wParam2;
			WORD wParam3;
		};
		char szNeed[ROLE_MAXNUM_NEEDDESPSIZE];
	};

} NET_MISNEED, *PNET_MISNEED;

typedef struct _NET_MISPRIZE
{
	BYTE byType;
	WORD wParam1;
	WORD wParam2;

} NET_MISPRIZE, *PNET_MISPRIZE;

typedef struct _NET_MISPAGE
{
	BYTE byNeedNum;
	NET_MISNEED MisNeed[ROLE_MAXNUM_MISNEED];

	BYTE byPrizeSelType;
	BYTE byPrizeNum;
	NET_MISPRIZE MisPrize[ROLE_MAXNUM_MISPRIZE];

	// 
	char szName[ROLE_MAXSIZE_MISNAME];
	char szDesp[ROLE_MAXNUM_DESPSIZE];

} NET_MISPAGE, *PNET_MISPAGE;

typedef struct _NET_MISLOG
{
	BYTE byType;
	BYTE byState;
	WORD wMisID;

} NET_MISLOG, *PNET_MISLOG;

typedef struct _NET_MISLOG_LIST
{
	BYTE	   byNumLog;
	NET_MISLOG MisLog[ROLE_MAXNUM_MISSION];

} NET_MISLOG_LIST, *PNET_MISLOG_LIST;

// 交易窗口信息
typedef struct _NET_TRADEPAGE
{
	BYTE   byCount;
	USHORT sItemID[ROLE_MAXNUM_TRADEITEM];
	USHORT sCount[ROLE_MAXNUM_TRADEITEM];
	DWORD  dwPrice[ROLE_MAXNUM_TRADEITEM];
	BYTE   byLevel[ROLE_MAXNUM_TRADEITEM];

} NET_TRADEPAGE, *PNET_TRADEPAGE;

typedef struct _NET_TRADEINFO
{
	NET_TRADEPAGE TradePage[mission::MAXTRADE_ITEMTYPE];

	_NET_TRADEINFO()
	{
		Clear();
	}

	void Clear()
	{
		memset( TradePage, 0, sizeof(NET_TRADEPAGE)*mission::MAXTRADE_ITEMTYPE );
	}

} NET_TRADEINFO, *PNETTRADEINFO;

struct NET_CHARTRADE_BOATDATA
{
	char szName[BOAT_MAXSIZE_BOATNAME];
	USHORT sBoatID;
	USHORT sLevel;
	DWORD dwExp;
	DWORD dwHp;
	DWORD dwMaxHp;
	DWORD dwSp;
	DWORD dwMaxSp;
	DWORD dwMinAttack;
	DWORD dwMaxAttack;
	DWORD dwDef;
	DWORD dwSpeed;
	DWORD dwShootSpeed;
	BYTE  byHasItem;
	BYTE  byCapacity;
	DWORD dwPrice;

};

// 角色交易物品实例信息结构
typedef struct _NET_CHARTRADE_ITEMDATA
{
	BYTE byForgeLv;
	BYTE byHasAttr;

	short sInstAttr[defITEM_INSTANCE_ATTR_NUM][2];	// 实例属性
	short sEndure[2];	// 耐久度（当前值/最大值）
	short sEnergy[2];	// 能量（当前值/最大值）

	long	lDBParam[enumITEMDBP_MAXNUM];	// 数据库参数
	bool	bValid;

} NET_CHARTRADE_ITEMDATA, *PNET_CHARTRADE_ITEMDATA;

// 公会挑战信息结构
#define MAX_GUILD_CHALLLEVEL				3	// 公会排名，前三
typedef struct _NET_GUILD_CHALLINFO
{
	BYTE byIsLeader;
	BYTE byLevel[MAX_GUILD_CHALLLEVEL];
	BYTE byStart[MAX_GUILD_CHALLLEVEL];
	char szGuild[MAX_GUILD_CHALLLEVEL][64];
	char szChall[MAX_GUILD_CHALLLEVEL][64];
	DWORD dwMoney[MAX_GUILD_CHALLLEVEL];

} NET_GUILD_CHALLINFO, *PNET_GUILD_CHALLINFO;
struct NetChaBehave
{
	const char	*	sCharName;			//角色名
	const char	*	sJob;				//职业
	short			iDegree;			//角色等级
	const LOOK	*	sLook;				//外观数据
};

struct stNetShortCutChange
{
	//stNetShortCutChange() : shyGrid2(-1){};
	char     chIndex;
	char     chType;
	short    shyGrid;
	//short    shyGrid2;
};

struct stNetNpcMission
{
    BYTE    byType;     // 类型
    USHORT  sNum;		// 需要摧毁物件总数量
    USHORT  sID;	    // 被摧毁物件类型ID
    USHORT  sCount;     // 已完成计数    
};

struct stNetAreaState
{
	short				sAreaX;			// 米
	short				sAreaY;
	char				chStateNum;
	stAreaSkillState	State[AREA_STATE_NUM];
};

struct stNetChaAttr
{
	char		chType;
	short		sNum;
	stEffect	SEff[MAX_ATTR_CLIENT];
};

struct stNetQueryRelive
{
	char		chType;	// 同CompCommand的EPlayerReliveType
	const char	*szSrcChaName;
};

// 服务器要求打开理发界面
struct stNetOpenHair
{
	void Exec();
};

// 请求更换发型
struct stNetUpdateHair
{
	short	sScriptID;			// 对应的Hair脚本ID,如果为0则代表关闭理发界面
	short	sGridLoc[4];		// 物品所在的格子
};

// 更换发型的返回值
struct stNetUpdateHairRes
{
	unsigned long	ulWorldID;			// 对应角色
	int				nScriptID;			// 更换后的脚本ID
	const char*		szReason;			// 更换时的原因,更换成功为:ok,更换为差的发型为：fail,更换失败为:其它原因

	void	Exec();
};

// 对方收到的队伍挑战请求
struct stNetTeamFightAsk
{
	struct
	{
		const char*	szName;
		const char*	szJob;
		char		chLv;
		unsigned short usVictoryNum;
		unsigned short usFightNum;
	}Info[10];
	char	chSideNum1;
	char	chSideNum2;

	void	Exec();
};

struct stNetItemRepairAsk
{
	const char	*cszItemName;
	long	lRepairMoney;

	void	Exec();
};

struct stSCNetItemForgeAsk
{
	char	chType;
	long	lMoney;

	void	Exec();
};

struct stNetItemForgeAnswer
{
	long	lChaID;
	char	chType;
	char	chResult;
	void	Exec();
};

// Add by lark.li 20080516 begin
struct stSCNetItemLotteryAsk
{
	long	lMoney;

	void	Exec();
};

struct stNetItemLotteryAnswer
{
	long	lChaID;
	char	chResult;
	void	Exec();
};
// End

#define	defMAX_FORGE_GROUP_NUM	defMAX_ITEM_FORGE_GROUP

struct SForgeCell
{
	SForgeCell() : sCellNum(0), pCell(0) {}
	~SForgeCell() 
	{
		delete[] pCell;
	}

	short	sCellNum;
	struct  SCell
	{
		short	sPosID;
		short	sNum;
	} *pCell;
};

struct stNetItemForgeAsk
{
	char	chType;	// 1，精炼。2，合成
	SForgeCell	SGroup[defMAX_FORGE_GROUP_NUM];
};

struct SLotteryCell
{
	SLotteryCell() : sCellNum(0), pCell(0) {}
	~SLotteryCell() 
	{
		delete[] pCell;
	}

	short	sCellNum;
	struct  SCell
	{
		short	sPosID;
		short	sNum;
	} *pCell;
};

#define	defMAX_LOTTERY_GROUP_NUM	7

struct stNetItemLotteryAsk
{
	SLotteryCell	SGroup[defMAX_LOTTERY_GROUP_NUM];
};

struct stNetEspeItem
{
	char	chNum;
	struct
	{
		short	sPos;
		short	sEndure;
		short	sEnergy;
	} SContent[4];
};

// 黑市商人结构体
struct stBlackTrade
{
	short sIndex;		// 
	short sSrcID;		// 需求物品ID
	short sSrcNum;		// 需求物品数量
	short sTarID;		// 目标物品ID
	short sTarNum;		// 目标物品数量
	short sTimeNum;		// time值
};


struct stChurchChallenge
{
	short sChurchID;	// id
	char  szName[32];	// name
	char  szChaName[32];// 当前竟拍者
	short sCount;		// 数量
	long  nBasePrice;	// 底价
	long  nMinbid;		// 最低出价
	long  nCurPrice;	// 当前竞拍价

	stChurchChallenge()	{ memset(this, 0, sizeof(stChurchChallenge)); }
};

extern void	NetLoginSuccess(char byPassword, char iCharNum,NetChaBehave chabehave[]);//加密Key//角色数目//角色信息
extern void NetLoginFailure(unsigned short Errno);	                //登陆错误信息
extern void	NetBeginPlay(unsigned short Errno);
extern void	NetEndPlay(char iCharNum,NetChaBehave chabehave[]);
extern void NetNewCha(unsigned short Errno);
extern void NetDelCha(unsigned short Errno);
extern void NetCreatePassword2(unsigned short Errno);
extern void NetUpdatePassword2(unsigned short Errno);

extern void NetActorDestroy(unsigned int nID, char chSeeType);
extern void NetActorMove(unsigned int nID, stNetNotiMove &list);
extern void NetActorSkillRep(unsigned int nID, stNetNotiSkillRepresent &skill);
extern void NetActorSkillEff(unsigned int nID, stNetNotiSkillEffect &skill);
extern void NetActorLean(unsigned int nID, stNetLeanInfo &lean);
extern void NetSwitchMap(stNetSwitchMap	&switchmap);
extern void NetSysInfo(stNetSysInfo &sysinfo);
extern void NetSideInfo(const char szName[], const char szInfo[]);
extern void NetBickerInfo( const char szData[] );
extern void NetColourInfo( unsigned int rgb, const char szData[] );
extern void NetSay(stNetSay &netsay,DWORD dwColour = 0xffffffff);
extern CSceneItem* NetCreateItem(stNetItemCreate &pSCreateInfo);
extern void NetItemDisappear(unsigned int nID);
extern void NetChangeChaPart( CCharacter* pCha, stNetLookInfo &SLookInfo );
extern void NetChangeChaPart( unsigned int nID, stNetLookInfo &SLookInfo );
extern void NetTempChangeChaPart( unsigned int nID, stTempChangeChaPart &SPart );
extern void NetActorChangeCha(unsigned int nID, stNetChangeCha &SChangeCha);
extern void NetShowTalk( const char szTalk[], BYTE byCmd, DWORD dwNpcID );
extern void NetShowHelp( const NET_HELPINFO& Info );
extern void NetShowMapCrash(const char szTalk[]);
extern void NetShowFunction( BYTE byFuncPage, BYTE byItemNum, BYTE byMisNum, const NET_FUNCPAGE& FuncArray, DWORD dwNpcID );
extern void NetShowMissionList( DWORD dwNpcID, const NET_MISSIONLIST& MisList );
extern void NetShowMisPage( DWORD dwNpcID, BYTE byCmd, const NET_MISPAGE& page );
extern void	NetMisLogList( NET_MISLOG_LIST& List );
extern void NetShowMisLog( WORD wMisID, const NET_MISPAGE& page );
extern void NetMisLogClear( WORD wMisID );
extern void NetMisLogAdd( WORD wMisID, BYTE byState );
extern void NetMisLogState( WORD wID, BYTE byState );
extern void NetCloseTalk( DWORD dwNpcID );
extern void NetCreateBoat( const xShipBuildInfo& Info );
extern void NetUpdateBoat( const xShipBuildInfo& Info );
extern void NetBoatInfo( DWORD dwBoatID, const char szName[], const xShipBuildInfo& Info );
extern void NetShowBoatList( DWORD dwNpcID, BYTE byNumBoat,  const BOAT_BERTH_DATA& Data, BYTE byType = mission::BERTH_LUANCH_LIST );
extern void NetChangeChaLookEnergy( unsigned int nID, stLookEnergy &SLookEnergy );
extern void NetQueryRelive(unsigned int nID, stNetQueryRelive &SQueryRelive);
extern void NetPreMoveTime(unsigned long ulTime);
extern void NetMapMask(unsigned int nID, BYTE *pMask, long lLen);

// 显示npc交易物品界面
extern void NetShowTrade( const NET_TRADEINFO& TradeInfo, BYTE byCmd, DWORD dwNpcID, DWORD dwParam );
extern void NetUpdateTradeAllData( const NET_TRADEINFO& TradeInfo, BYTE byCmd, DWORD dwNpcID, DWORD dwParam );
extern void NetUpdateTradeData( DWORD dwNpcID, BYTE byPage, BYTE byIndex, USHORT sItemID, USHORT sCount, DWORD dwPrice );

// 与npc交易结果
extern void NetTradeResult( BYTE byCmd, BYTE byIndex, BYTE byCount, USHORT sItemID, DWORD dwMoney );

// 显示角色交易邀请
extern void NetShowCharTradeRequest( BYTE byType, DWORD dwRequestID );

// 开始进行角色交易
extern void NetShowCharTradeInfo( BYTE byType, DWORD dwAcceptID, DWORD dwRequestID );

// 取消角色交易
extern void NetCancelCharTrade( DWORD dwCharID );

// 角色交易确认交易数据信息
extern void NetValidateTradeData( DWORD dwCharID );

// 角色交易确认交易
extern void NetValidateTrade( DWORD dwCharID );

// 角色交易拖动船长证明物品
extern void NetTradeAddBoat( DWORD dwCharID, BYTE byOpType, USHORT sItemID, BYTE byIndex, 
							BYTE byCount, BYTE byItemIndex, const NET_CHARTRADE_BOATDATA& Data );

// 角色交易拖动物品
extern void NetTradeAddItem( DWORD dwCharID, BYTE byOpType, USHORT sItemID, BYTE byIndex, 
							BYTE byCount, BYTE byItemIndex, const NET_CHARTRADE_ITEMDATA& Data );

// 角色交易界面显示金钱数量
extern void NetTradeShowMoney( DWORD dwCharID, DWORD dwMoney );
extern void NetTradeShowIMP( DWORD dwCharID, DWORD dwMoney );

// 角色交易结果
extern void NetTradeSuccess();
extern void NetTradeFailed();

// 摆摊
extern void NetStallInfo( DWORD dwCharID, BYTE byNum, const char szName[] );
extern void NetStallAddBoat( BYTE byGrid, USHORT sItemID, BYTE byCount, DWORD dwMoney, NET_CHARTRADE_BOATDATA& Data );
extern void NetStallAddItem( BYTE byGrid, USHORT sItemID, BYTE byCount, DWORD dwMoney, NET_CHARTRADE_ITEMDATA& Data );
extern void NetStallDelGoods( DWORD dwCharID, BYTE byGrid, BYTE byCount );
extern void NetStallClose( DWORD dwCharID );
extern void NetStallSuccess( DWORD dwCharID );
extern void NetStallName(DWORD dwCharID, const char *szStallName);
extern void NetStartExit( DWORD dwExitTime );
extern void NetCancelExit();
extern void NetSynAttr( DWORD dwWorldID, char chType, short sNum, stEffect *pEffect );
extern void NetFace(DWORD dwCharID, stNetFace& netface, char chType);
extern void NetChangeKitbag(DWORD dwChaID, stNetKitbag& SKitbag);
extern void NetNpcStateChange( DWORD dwChaID, BYTE byState );
extern void NetEntityStateChange( DWORD dwEntityID, BYTE byState );
extern void NetShortCut( DWORD dwChaID, stNetShortCut& stShortCut );  
extern void NetTriggerAction( stNetNpcMission& info );
extern void NetShowForge();

extern void NetShowUnite();
extern void NetShowMilling();
extern void NetShowFusion();
extern void NetShowUpgrade();
extern void NetShowEidolonMetempsychosis();
extern void NetShowEidolonFusion();
extern void NetShowPurify();
extern void NetShowGetStone();
extern void NetShowRepairOven();
extern void NetShowEnergy();
extern void NetShowTiger();

extern void NetSynSkillBag(DWORD dwCharID, stNetSkillBag *pSSkillBag);
extern void NetSynSkillState(DWORD dwCharID, stNetSkillState *pSSkillState);
extern void NetAreaStateBeginSee(stNetAreaState *pState);
extern void NetAreaStateEndSee(stNetAreaState *pState);
extern void NetFailedAction( char chState );
extern void NetShowMessage( long lMes );
extern void NetChaTLeaderID(long lID, long lLeaderID);
extern void NetChaEmotion( long lID, short sEmotion );

extern void NetChaSideInfo( long lID, stNetChaSideInfo &SNetSideInfo );
extern void NetBeginRepairItem(void);

extern void NetItemUseSuccess(unsigned int nID, short sItemID);
extern void NetKitbagCapacity(unsigned int nID, short sKbCap);
extern void NetKitbagLockedSpaces(short slots,CGoodsGrid* grd);
extern void NetEspeItem(unsigned int nID, stNetEspeItem& SEspeItem);

extern void NetKitbagCheckAnswer(bool bLock);
extern void NetChaPlayEffect(unsigned int uiWorldID, int nEffectID);

extern void NetChurchChallenge(const stChurchChallenge* pInfo);

