//=============================================================================
// FileName: CompCommand.h
// Creater: ZhangXuedong
// Date: 2004.11.22
// Comment: compositive command
//=============================================================================

#ifndef COMPOSITIVECOMMAND_H
#define COMPOSITIVECOMMAND_H

#include "util.h"
#include "ItemAttrType.h"
#include "ChaAttrType.h"
#include "ItemContent.h"

#define defPROTOCOL_HAVE_PACKETID // 控制协议中是否包含包的ID

enum EActionType // server,client 之间的行动类型
{
	enumACTION_NONE = 0,
	enumACTION_MOVE,		// 移动
	enumACTION_SKILL,		// 技能
	enumACTION_SKILL_SRC,	// 使用技能
	enumACTION_SKILL_TAR,	// 被使用技能
	enumACTION_LOOK,		// 更新角色外观
	enumACTION_KITBAG,		// 更新角色的道具栏
	enumACTION_SKILLBAG,	// 更新技能栏
	enumACTION_ITEM_PICK,	// 捡道具
	enumACTION_ITEM_THROW,	// 丢道具
	enumACTION_ITEM_UNFIX,	// 道具卸装
	enumACTION_ITEM_USE,	// 道具使用
	enumACTION_ITEM_POS,	// 道具改变位置
	enumACTION_ITEM_DELETE,	// 道具删除
	enumACTION_ITEM_INFO,	// 道具信息
	enumACTION_ITEM_FAILED,	// 道具操作失败
	enumACTION_LEAN,		// 倚靠
	enumACTION_CHANGE_CHA,	// 更换角色
	enumACTION_EVENT,		// 触发事件
    enumACTION_FACE,        // 客户端做表情动作,目前服务器仅需要转发给其它客户端
	enumACTION_STOP_STATE,	// 停止技能状态
	enumACTION_SKILL_POSE,	// 技能Pose
	enumACTION_PK_CTRL,		// PK控制
	enumACTION_LOOK_ENERGY,	// 更新角色外观能量

	enumACTION_TEMP,		// 临时协议

    enumACTION_SHORTCUT,    // 客户端发送快捷栏给服务器存盘，或服务器通知客户端快捷栏内容,注:必须要道具栏,技能栏初始完成后才能发送
	enumACTION_BANK,		// 更新银行信息
	enumACTION_CLOSE_BANK,	// 关闭银行

	enumACTION_KITBAGTMP,		//更新临时背包
	enumACTION_KITBAGTMP_DRAG,	//拖放临时背包中的道具
	
	enumACTION_GUILDBANK,
	enumACTION_REQUESTGUILDBANK,
	enumACTION_REQUESTGUILDATTR,
	enumACTION_REQUESTGUILDQUEST,
	enumACTION_ENTERGUILDHOUSE,
	enumACTION_GOLDSTORE,
	enumACTION_BAGOFHOLDING,
	enumACTION_BAGOFHOLDINGNAME,
	enumMAX_ACTION_NUM		// 最大行动个数
};

enum EAttrSynType // server->client 属性同步类型
{
	enumATTRSYN_INIT,			// 初始化
	enumATTRSYN_ITEM_EQUIP,		// 因装备或卸装道具,属性发生改变
	enumATTRSYN_ITEM_MEDICINE,	// 因使用物品（药物）,属性发生改变
    enumATTRSYN_ATTACK,			// 因为攻击或受击,属性发生改变,如升级
	enumATTRSYN_TASK,			// 因任务，属性改变
	enumATTRSYN_TRADE,			// 因交易，属性改变
	enumATTRSYN_REASSIGN,		// 分配属性点，导致其他属性改变
	enumATTRSYN_SKILL_STATE,	// 因技能状态改变而导致属性改变
	enumATTRSYN_AUTO_RESUME,	// 自动恢复（HP,SP）
	enumATTRSYN_CHANGE_JOB,		// 转职
	enumATTRSYN_RECTIFY,		// 纠正属性
};

enum EItemAppearType // server->client 道具出现类型
{
	enumITEM_APPE_MONS,		// 怪物掉落
	enumITEM_APPE_THROW,	// 被角色扔出
	enumITEM_APPE_NATURAL,	// 自然出现
};

enum ESynKitbagType // server->client 道具栏同步类型
{
	enumSYN_KITBAG_INIT,	// 道具栏初始化
	enumSYN_KITBAG_EQUIP,	// 装备道具引发同步道具栏
	enumSYN_KITBAG_UNFIX,	// 卸装道具引发同步道具栏
	enumSYN_KITBAG_PICK,	// 捡道具
	enumSYN_KITBAG_THROW,	// 扔道具
	enumSYN_KITBAG_SWITCH,	// 交换道具位置
	enumSYN_KITBAG_TRADE,	// 交易
	enumSYN_KITBAG_FROM_NPC,// NPC给与
	enumSYN_KITBAG_TO_NPC,	// 被NPC取走
	enumSYN_KITBAG_SYSTEM,	// 系统放入
	enumSYN_KITBAG_FORGES,	// 精炼成功
	enumSYN_KITBAG_FORGEF,	// 精炼失败
	enumSYN_KITBAG_BANK,	// 银行
	enumSYN_KITBAG_ATTR,	// 属性改变（耐久损耗）
};

enum ESynSkillBagType // server->client 技能栏同步类型
{
	enumSYN_SKILLBAG_INIT,	// 技能栏初始化（发送全部的技能信息）
	enumSYN_SKILLBAG_ADD,	// 增加技能（只发送新增加的技能的信息）
	enumSYN_SKILLBAG_MODI,	// 修改技能（只发送被修改的技能的信息）
};

enum ESynLookType
{
	enumSYN_LOOK_SWITCH,	// 交换。
	enumSYN_LOOK_CHANGE,	// 数值改变。
};

enum EEquipLinkPos // 道具的装备部位
{
	enumEQUIP_HEAD		= 0,	// 五个身体部位,头,脸,身体,手,脚
	enumEQUIP_FACE		= 1,
	enumEQUIP_BODY		= 2,
	enumEQUIP_GLOVE		= 3,	// 手套
	enumEQUIP_SHOES		= 4,	// 鞋子

	enumEQUIP_NECK		= 5,	// 脖子:带项链,徽章
	enumEQUIP_LHAND		= 6,	// 左手		-- 左手右手的道具值为客户端Link点
	enumEQUIP_HAND1		= 7,	// 两个手饰
	enumEQUIP_HAND2		= 8,
	enumEQUIP_RHAND		= 9,	// 右手
	enumEQUIP_Jewelry1	= 10,
	enumEQUIP_Jewelry2	= 11,
	enumEQUIP_Jewelry3	= 12,
	enumEQUIP_Jewelry4	= 13,
	enumEQUIP_WING		= 14,
	
	enumEQUIP_CLOAK		= 15,
	enumEQUIP_FAIRY		= 16,
	enumEQUIP_REAR		= 17,
	enumEQUIP_MOUNT		= 18,

	enumEQUIP_HEADAPP = 19,
	enumEQUIP_FACEAPP = 20,
	enumEQUIP_BODYAPP = 21,
	enumEQUIP_GLOVEAPP = 22,
	enumEQUIP_SHOESAPP = 23,
	
	enumEQUIP_FAIRYAPP = 24,
	enumEQUIP_GLOWAPP = 25,
	
	enumEQUIP_DAGGERAPP = 26,
	enumEQUIP_GUNAPP = 27,
	enumEQUIP_SWORD1APP = 28,
	enumEQUIP_GREATSWORDAPP = 29,
	enumEQUIP_STAFFAPP = 30,
	enumEQUIP_BOWAPP = 31,
	enumEQUIP_SWORD2APP = 32,
	enumEQUIP_SHIELDAPP = 33,
	
	
	enumEQUIP_PART_NUM  = 5,	// 总共五个部位
	enumEQUIP_NUM		= 34,	// 装备部位的个数
    enumEQUIP_BOTH_HAND = 9999,  // 如果是双手武器,另一边就为这个值
    enumEQUIP_TOTEM		= 9998,  // 如果是图腾装备,其他部位就为这个值
};

enum EBoatLinkPos	// 船的装备位置
{
	enumBOAT_BODY		= 0,	// 船身
	enumBOAT_HEADER		= 1,	// 船头	
	enumBOAT_ENGINE		= 2,	// 船只动力
	enumBOAT_FLAG		= 3,	// 船标志
	enumBOAT_MOTOR0		= 4,	// 最多4个马达
	enumBOAT_MOTOR1		= 5,
	enumBOAT_MOTOR2		= 6,
	enumBOAT_MOTOR3		= 7,
};

enum EMoveState
{
	enumMSTATE_ON		= 0x00,	// 移动中
	enumMSTATE_ARRIVE	= 0x01,	// 到达目标点停止
	enumMSTATE_BLOCK	= 0x02,	// 遇到障碍停止
	enumMSTATE_CANCEL	= 0x04,	// 被要求停止
	enumMSTATE_INRANGE	= 0x08, // 已经到达要求的范围而停止
	enumMSTATE_NOTARGET	= 0x10, // 目标不存在而停止
	enumMSTATE_CANTMOVE	= 0x20,	// 不能移动
};

enum EFightState
{// enumFSTATE_TARGET_NO之前是引发表现的状态，后面是不引发表现的状态
	enumFSTATE_ON			= 0x0000,	// 战斗中
	enumFSTATE_STOP			= 0x0001,	// 正常停止

	enumFSTATE_TARGET_NO	= 0x0010,	// 对象不存在而停止（没有找到技能施用对象），不引发表现
	enumFSTATE_TARGET_OUT	= 0x0020,	// 对象离开使用范围而停止，不引发表现
	enumFSTATE_TARGET_IMMUNE= 0x0040,	// 对象不能被攻击，不引发表现
	enumFSTATE_CANCEL		= 0x0080,	// 被要求停止，不引发表现
	enumFSTATE_DIE			= 0x0100,	// 自身死亡而停止，不引发表现
    enumFSTATE_TARGET_DIE   = 0x0200,	// 目标被正常攻击死亡而停止，不引发表现
	enumFSTATE_OFF			= 0x0400,	// 不能攻击型实体，不引发表现
	enumFSTATE_NO_EXPEND	= 0x0800,	// 消耗品（MP，能量，道具等）不足，不引发表现
};

enum EExistState
{
	// 生活状态
	enumEXISTS_NATALITY,	// 出生
	enumEXISTS_WAITING,		// 待机
	enumEXISTS_SLEEPING,	// 休眠
	enumEXISTS_MOVING,		// 移动
	enumEXISTS_FIGHTING,	// 技能
	//
	// 死亡状态
	enumEXISTS_WITHERING,	// 消亡中
	enumEXISTS_RESUMEING,	// 复活中
	enumEXISTS_DIE,			// 死亡
	//
};

enum ESkillStateAdd
{
	enumSSTATE_ADD_UNDEFINED		= 0,	// 未定义的替换策略
	enumSSTATE_ADD_EQUALORLARGER	= 1,	// 大于等于替换
	enumSSTATE_ADD_LARGER			= 2,	// 大于替换
	enumSSTATE_NOTADD				= 3,	// 不替换
	enumSSTATE_ADD					= 4,	// 替换
};

enum ERangeType
{
	enumRANGE_TYPE_NONE				= 0,	// 空
	enumRANGE_TYPE_STICK			= 1,	// 柱形（长度，宽度）
	enumRANGE_TYPE_FAN				= 2,	// 扇形（半径，角度）
	enumRANGE_TYPE_SQUARE			= 3,	// 正方（半径）
	enumRANGE_TYPE_CIRCLE			= 4,	// 圆形（半径）
};

// 行动请求失败的原因
enum EFailedActionReason
{
	enumFACTION_ACTFORBID,		// 存在禁止行动的状态
	enumFACTION_EXISTACT,		// 之前的行动没有结束
	enumFACTION_MOVEPATH,		// 移动路径错误
	enumFACTION_CANTMOVE,		// 不能移动
	enumFACTION_NOSKILL,		// 技能不存在
	enumFACTION_NOOBJECT,		// 目标不存在
	enumFACTION_ITEM_INEXIST,	// 道具不存在
	enumFACTION_SKILL_NOTMATCH,	// 技能施放者不匹配
};

enum ESkillUseState
{
	enumSUSTATE_INACTIVE,		// 技能未激活的
	enumSUSTATE_ACTIVE,			// 技能激活的
};

enum EItemOperateResult
{
	enumITEMOPT_SUCCESS,		// 道具操作成功
	enumITEMOPT_ERROR_NONE,		// 道具不存在
	enumITEMOPT_ERROR_KBFULL,	// 道具栏已满
	enumITEMOPT_ERROR_UNUSE,	// 不能使用的道具
	enumITEMOPT_ERROR_UNPICKUP,	// 道具不能拾取
	enumITEMOPT_ERROR_UNTHROW,	// 道具不能丢弃
	enumITEMOPT_ERROR_UNDEL,	// 道具不能销毁
	enumITEMOPT_ERROR_KBLOCK,	// 道具栏处于锁定状态
	enumITEMOPT_ERROR_DISTANCE,	// 距离太远
	enumITEMOPT_ERROR_EQUIPLV,	// 装备等级不满足
	enumITEMOPT_ERROR_EQUIPJOB,	// 装备职业不满足
	enumITEMOPT_ERROR_STATE,	// 存在不能使用道具的状态
	enumITEMOPT_ERROR_PROTECT,	// 道具被保护
	enumITEMOPT_ERROR_AREA,		// 不同的区域类型
	enumITEMOPT_ERROR_BODY,		// 体型不匹配
	enumITEMOPT_ERROR_TYPE,		// 此道具无法存放
	enumITEMOPT_ERROR_INVALID,	// 无效的道具
	enumITEMOPT_ERROR_KBRANGE,	// 超出道具栏范围
};

enum EEntitySeenType // 用于客户端判断该实体是否已经存在（对于主角的船只，则已经存在）
{
	enumENTITY_SEEN_NEW,		// 新建实体
	enumENTITY_SEEN_SWITCH,		// 切换实体
};

enum EPlayerReliveType
{
	enumEPLAYER_RELIVE_NONE,	// 空选择
	enumEPLAYER_RELIVE_CITY,	// 回城复活
	enumEPLAYER_RELIVE_ORIGIN,	// 原点复活
	enumEPLAYER_RELIVE_NORIGIN,	// 拒绝原点复活
	enumEPLAYER_RELIVE_MAP,		// 原地图复活
	enumEPLAYER_RELIVE_NOMAP,	// 拒绝原地图复活
};

enum EUseSkill
{
	enumESKILL_SUCCESS,				// 技能使用成功
	enumESKILL_FAILD_NPC,			// 目标是NPC
	enumESKILL_FAILD_NOT_SKILLED,	// 目标不能被使用技能
	enumESKILL_FAILD_SAFETY_BELT,	// 目标在安全区
	enumESKILL_FAILD_NOT_LAND,		// 技能不能用于陆地
	enumESKILL_FAILD_NOT_SEA,		// 技能不能用于海洋
	enumESKILL_FAILD_ONLY_SELF,		// 技能只能用于自身
	enumESKILL_FAILD_ONLY_DIEPLY,	// 技能只能用于玩家尸体
	enumESKILL_FAILD_FRIEND,		// 技能不能用于友方
	enumESKILL_FAILD_NOT_FRIEND,	// 技能只能用于友方
	enumESKILL_FAILD_NOT_PALYER,	// 技能只能用于玩家
	enumESKILL_FAILD_NOT_MONS,		// 技能只能用于怪物
	enumESKILL_FAILD_ESP_MONS,		// 技能只能用于特殊怪物
	enumESKILL_FAILD_SELF,			// 技能不能用于自身
};

enum EEnterMapType // 用于客户端判断是否显示Loading画面
{
	enumENTER_MAP_EDGE,				// 边界进入
	enumENTER_MAP_CARRY,			// 传送进入
};

enum EFightType // 挑战类型，此类型也用于脚本中，请勿改动
{
	enumFIGHT_NONE		= 0, // 空
	enumFIGHT_TEAM		= 1, // 队伍挑战
	enumFIGHT_MONOMER	= 2, // 单体挑战
	enumFIGHT_GUILD		= 3, // 公会挑战
};

/*	2008-7-28	yangyinyu	close!		//	为了增加唯一ID，特升级版本，新升级的版本在每个道具项前面增加唯一ID号，如果是0，则表示是没有唯一ID的道具。
#define defKITBAG_CUR_VER					113	// 标示是否有属性数据
*/
#define defKITBAG_CUR_VER111				111	// 较之前的版本，最前面加入背包容量单元（后接@符号）
#define defLOOK_CUR_VER						111	// 标示是否有属性数据
#define defLOOK_CUR_VER110					110
#define defITEM_INSTANCE_ATTR_NUM_VER110	10

#define defMAP_GARNER_WIDTH					4096
#define defMAP_GARNER_HEIGHT				4096
#define defMAP_DARKBLUE_WIDTH				4096
#define defMAP_DARKBLUE_HEIGHT				4096
#define defMAP_MAGICSEA_WIDTH				4096
#define defMAP_MAGICSEA_HEIGHT				4096
#define defMAP_EASTGOAF_WIDTH				1024
#define defMAP_EASTGOAF_HEIGHT				1024
#define defMAP_LONETOWER_WIDTH				1024
#define defMAP_LONETOWER_HEIGHT				1024

#define defMAX_KBITEM_NUM_PER_TYPE		48 // 每种物品空间的物品数量
#define defDEF_KBITEM_NUM_PER_TYPE		24 // 每种物品空间的物品数量
#define defESPE_KBGRID_NUM				4  // 特殊道具格数

#pragma pack(push)
#pragma pack(1)
typedef
struct stNetChangeChaPart		// 改变角色外观,注:客户端-服务器端默认皮肤的约定:客户端接收到0的皮肤时,自动载入CharacterRecord中的皮肤
{
	stNetChangeChaPart() {sVer = 0; sTypeID = 0;}

	short	sVer;
	short	sTypeID;
	union
	{
		struct
		{
			SItemGrid	SLink[enumEQUIP_NUM];
			short       sHairID;        // 默认的头发和脸型
		};

		struct
		{
			USHORT sPosID;		// 船动作ID
			USHORT sBoatID;				// 船信息表ID
			USHORT sHeader;				// 船头
			USHORT sBody;				// 船身
			USHORT sEngine;				// 船只马达
			USHORT sCannon;				// 船只火炮
			USHORT sEquipment;			// 船只装备
		};
	};
} LOOK;
#pragma pack(pop)

#define defMAX_ITEM_FORGE_GROUP	5

struct SForgeItem
{
	struct
	{
		short	sGridNum;
		struct
		{
			short	sGridID;
			short	sItemNum;
		} SGrid[defMAX_KBITEM_NUM_PER_TYPE];
	} SGroup[defMAX_ITEM_FORGE_GROUP];
};

#define defMAX_ITEM_LOTTERY_GROUP	7

struct SLotteryItem
{
	struct
	{
		short	sGridNum;
		struct
		{
			short	sGridID;
			short	sItemNum;
		} SGrid[defMAX_KBITEM_NUM_PER_TYPE];
	} SGroup[defMAX_ITEM_LOTTERY_GROUP];
};

#define defMAX_ITEM_LIFESKILL_GROUP	6
struct SLifeSkillItem
{
		short sGridID[defMAX_ITEM_LIFESKILL_GROUP];
		short sbagCount;
		short sReturn;
};


#define defMAX_CITY_NUM		8
extern const char	*g_szCityName[defMAX_CITY_NUM];

//检查人物部分ID号用到的声明
const int PLAY_NUM = 4;
extern const long g_PartIdRange[PLAY_NUM][enumEQUIP_NUM + 1][2];
//end

#define defMOTTO_LEN		40
#define defGUILD_NAME_LEN	17	// 公会名称
#define defGUILD_MOTTO_LEN	51	// 公会座佑名
#define defPICKUP_DISTANCE	350 // 拾取范围（厘米）
#define defTHROW_DISTANCE	350 // 丢弃范围（厘米）
#define defBANK_DISTANCE	350 // 银行交互范围（厘米）
#define defRANGE_TOUCH_DIS	350 // 范围触发距离（厘米）

// 快捷栏
const DWORD MAX_FAST_ROW = 3;
const DWORD MAX_FAST_COL = 12;
const DWORD SHORT_CUT_NUM = MAX_FAST_ROW * MAX_FAST_COL;
#define defItemShortCutType       1
#define defSkillFightShortCutType 2
#define defSkillLifeShortCutType  3
#define defSkillSailShortCutType  4

struct stNetShortCut
{
    char     chType[SHORT_CUT_NUM];     // 1-道具,2-战斗技能,3-生活技能,0-空
    short    byGridID[SHORT_CUT_NUM];   // 在道具栏处于第几格,或技能栏中的技能编号
};
//

enum EPoseState
{
	enumPoseStand   = 0,
	enumPoseLean    = 1,
	enumPoseSeat	= 2,
};

enum EGuildState		//16位状态控制位屏蔽码
{
	emGldTypeNavy			=0,			//海军
	emGldTypePirate			=1,			//海盗

	emGldMembStatNormal		=0,			//普通状态
	emGldMembStatTry		=1,			//申请状态

	//emGldPermMgr			=0x1,		//管理
	//emGldPermBank			=0x2,		//银行
	//emGldPermBuild			=0x4,		//建设
	
	emGldPermSpeak		= 1,		
	emGldPermMgr			= 2,	
	emGldPermViewBank			= 4,	
	emGldPermDepoBank			= 8,	
	emGldPermTakeBank			= 16,
	emGldPermRecruit			= 32,	
	emGldPermKick			= 64,		
	emGldPermMotto		= 128,		
	emGldPermAttr		= 256,	
	emGldPermEnter = 512,
	emGldPermPlace = 1024,
	emGldPermRem = 2048,
	emGldPermDisband = 4096,
	emGldPermLeader = 8192,

	emGldPermMax = 0xFFFFFFFF,//0x7FFFFFF,
	emGldPermDefault = 1,
	emGldPermNum = 14,

	emMaxMemberNum			=80,
	emMaxTryMemberNum		=40,

	emGuildInitVal			=0x0000,	//初始值
	emGuildGetName			=0x0001,	//是否在获取新建公会名字状态
	emGuildReplaceOldTry	=0x0002,	//询问是否代替老的申请状态
};

#define defKITBAG_ITEM_NUM	100

#define defLOOK_DATA_STRING_LEN	2048

enum EAreaMask
{
	enumAREA_TYPE_LAND	    = 0x0001,	// 第0位 1，陆地。0，是海洋
    enumAREA_TYPE_NOT_FIGHT = 0x0002,	// 第1位 1，非战斗区。0，战斗区
    enumAREA_TYPE_PK		= 0x0004,	// 第2位 1，PK区域
	enumAREA_TYPE_BRIDGE	= 0x0008,	// 第3位 1，桥。
	enumAREA_TYPE_NOMONSTER	= 0x0010,	// 第4位 1，禁怪区
    enumAREA_TYPE_MINE		= 0x0020,	// 第5位 1
    enumAREA_TYPE_FIGHT_ASK	= 0x0040,	// 第6位 1，邀请PK
};

extern const char* g_GetAreaName( int nValue );

#define defCHA_TERRITORY_DISCRETIONAL	2	// 任意空间
#define defCHA_TERRITORY_LAND			0	// 陆地
#define defCHA_TERRITORY_SEA			1	// 海洋

#endif // COMPOSITIVECOMMAND_H