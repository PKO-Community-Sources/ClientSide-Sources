//======================================================================================================================
// FileName: CharacterRecord.h
// Creater: ZhangXuedong
// Date: 2004.09.01
// Comment: CChaRecordSet class
//======================================================================================================================

#ifndef CHARACTERRECORD_H
#define CHARACTERRECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"

// 外观信息-模型类型
enum EChaModalType
{
	enumMODAL_MAIN_CHA		= 1,
	enumMODAL_BOAT,				
	enumMODAL_EMPL,
	enumMODAL_OTHER,
};

// 控制类型
enum EChaCtrlType
{
	enumCHACTRL_NONE		= 0, // 未定义的

	enumCHACTRL_PLAYER		= 1, // 玩家

	enumCHACTRL_NPC			= 2, // 普通NPC
	enumCHACTRL_NPC_EVENT	= 3, // 事件NPC

	enumCHACTRL_MONS		    = 5, // 普通怪物
	enumCHACTRL_MONS_TREE	    = 6, // 树怪物
	enumCHACTRL_MONS_MINE	    = 7, // 矿石怪物
	enumCHACTRL_MONS_FISH	    = 8, // 鱼怪物
	enumCHACTRL_MONS_DBOAT	    = 9, // 沉船怪物

	enumCHACTRL_PLAYER_PET      = 10, // 玩家宠物

	enumCHACTRL_MONS_REPAIRABLE = 17, // 可修补怪物
};

#define defCHA_AI_NONE				0 // 待机
#define defCHA_AI_ATTACK_PASSIVE	1 // 被动攻击
#define defCHA_AI_ATTACK_ACTIVE		2 // 主动攻击

const char cchChaRecordKeyValue = (char)0xff;

#define defCHA_NAME_LEN			32
#define defCHA_ICON_NAME_LEN	17

#define defCHA_SKIN_NUM			8
#define defCHA_INIT_SKILL_NUM	11		// 0空手,1-10为武器类型
#define defCHA_INIT_ITEM_NUM	10
#define defCHA_GUILD_NAME_LEN	33
#define defCHA_TITLE_NAME_LEN	33
#define defCHA_JOB_NAME_LEN		17
#define defCHA_ITEM_KIND_NUM	20
#define defCHA_DIE_EFFECT_NUM	3		// 客户端最多死亡表现
#define defCHA_BIRTH_EFFECT_NUM	3		// 客户端最多出生表现
#define defCHA_HP_EFFECT_NUM    3

class CChaRecord : public CRawDataInfo
{
public:
	//CChaRecord();

	long	lID;								// 编号
	_TCHAR	szName[defCHA_NAME_LEN];			// 名称
	_TCHAR	szIconName[defCHA_ICON_NAME_LEN];	// 头像图标名称
	char	chModalType;						// 模型类型
	char	chCtrlType;							// 控制类型
	short	sModel;								// 骨架号
	short	sSuitID;							// 套号
	short	sSuitNum;							// 套装数量
	short	sSkinInfo[defCHA_SKIN_NUM];			// 皮肤
	short	sFeffID[4];							// FeffID
	short	sEeffID;							// EeffID
	short   sEffectActionID[3];					// 特效动作编号,0-动作编号,1-特效,2-dummy
	short	sShadow;							// 影子
    short   sActionID;                          // 动作编号
    char    chDiaphaneity;                      // 透明度
	short	sFootfall;							// 行走音效
	short	sWhoop;								// 喘息音效
	short	sDirge;								// 死亡音效
	char	chControlAble;						// 是否可控
	//char	chMoveAble;							// 可否移动
	char	chTerritory;						// 区域限制
	short   sSeaHeight;							// 高度偏移
	short	sItemType[defCHA_ITEM_KIND_NUM];	// 可装备道具
	float	fLengh;								// 长（米）
	float	fWidth;								// 宽（米）
	float	fHeight;							// 高（米）
	short	sRadii;								// 半径
	char    nBirthBehave[defCHA_BIRTH_EFFECT_NUM];// 出生表现
	char    nDiedBehave[defCHA_DIE_EFFECT_NUM];	// 死亡表现
    short   sBornEff;                           // 出生特效
    short   sDieEff;                            // 死亡特效
	short	sDormancy;							// 休眠动作
    char    chDieAction;                        // 死亡时的动作
	int		_nHPEffect[defCHA_HP_EFFECT_NUM];	// 剩余hp特效表现
	bool	_IsFace;							// 是否可旋转
	bool	_IsCyclone;							// 是否可被飓风吹起
	long	lScript;							// 脚本文件ID
	long	lWeapon;							// 道具表中的武器ID
	long	lSkill[defCHA_INIT_SKILL_NUM][2];	// 技能及使用频率
	long	lItem[defCHA_INIT_ITEM_NUM][2];		// 物品及掉料几率
	long	lTaskItem[defCHA_INIT_ITEM_NUM][2];	// 任务物品及掉料几率
	long	lMaxShowItem;						// 每次最多爆出物品数量
	float	fAllShow;							// 打爆几率
	long	lPrefix;							// 前缀等级
	long	lAiNo;								// AI编号
	char	chCanTurn;							// 攻击时是否转向
	long	lVision;							// 视野范围（厘米）
	long	lNoise;								// 喊叫范围（厘米）
	long	lGetEXP;							// 直接获得的EXP
	bool	bLight;								// 是否受光照影响

	long	lMobexp;// 普通exp
	long	lLv;	// 角色等级
	long	lMxHp;	// 最大HP
	long	lHp;	// 当前HP
	long	lMxSp;	// 最大SP
	long	lSp;	// 当前SP
	long	lMnAtk;	// 最小攻击力
	long	lMxAtk;	// 最大攻击力
	long	lPDef;	// 物理抵抗
	long	lDef;	// 防御力
	long	lHit;	// 命中率
	long	lFlee;	// 闪避率
	long	lCrt;	// 爆击率
	long	lMf;	// 寻宝率
	long	lHRec;	// hp恢复速度
	long	lSRec;	// sp恢复速度
	long	lASpd;	// 攻击间隔
	long	lADis;	// 攻击距离
	long	lCDis;	// 追击范围
	long	lMSpd;	// 移动速度
	long	lCol;	// 资源采集速度
	long	lStr;	// 力量（strength）		影响攻击力、添弹速度
	long	lAgi;	// 敏捷（agility）		影响闪避率
	long	lDex;	// 专注（dexterity）	影响命中率
	long	lCon;	// 体质（constitution）	影响防御力和hp
	long	lSta;	// 精力（stamina）		影响sp，影响技能攻击力
	long	lLuk;	// 幸运（luck）			影响会心一击几率、物品掉落几率
	long	lLHandVal;	// 左手武器发挥
	_TCHAR	szGuild[defCHA_GUILD_NAME_LEN];	// 行会名称
	_TCHAR	szTitle[defCHA_TITLE_NAME_LEN];	// 角色称号
	_TCHAR	szJob[defCHA_JOB_NAME_LEN];		// 角色职业
	LONG32	lCExp;	// 当前经验
	LONG32	lNExp;	// 下一级所需经验
	long	lFame;	// 名声
	long	lAp;	// 属性点
	long	lTp;	// 技能点，每次升级获得（技能点，技巧点technique point）
	long	lGd;	// 金钱
	long	lSpri;	// 炮弹飞行速度
	long	lStor;	// 货舱(背包)容量
	long	lMxSail;	// 船员上限
	long	lSail;	// 现有船员的数量
	long	lStasa;	// 标准人员配置数量
	long	lScsm;	// 补给消耗

	long	lTStr;	// 临时力量				由道具或者技能加成的，拿掉道具或技能时间到则自动减除
	long	lTAgi;	// 临时敏捷				由道具或者技能加成的，拿掉道具或技能时间到则自动减除
	long	lTDex;	// 临时专注				由道具或者技能加成的，拿掉道具或技能时间到则自动减除
	long	lTCon;	// 临时体质				由道具或者技能加成的，拿掉道具或技能时间到则自动减除
	long	lTSta;	// 临时耐力				由道具或者技能加成的，拿掉道具或技能时间到则自动减除
	long	lTLuk;	// 临时幸运				由道具或者技能加成的，拿掉道具或技能时间到则自动减除
	long	lTMxHp;	// 临时最大血量			由道具或者技能加成的，拿掉道具或技能时间到则自动减除
	long	lTMxSp;	// 临时最大精力值		由道具或者技能加成的，拿掉道具或技能时间到则自动减除
	long	lTAtk;	// 临时攻击力			临时增加的攻击力
	long	lTDef;	// 临时防御力			临时增加的防御力
	long	lTHit;	// 临时命中率			临时增加的命中率
	long	lTFlee;	// 临时闪避率			临时增加的闪避率
	long	lTMf;	// 临时寻宝率			临时增加的寻宝率
	long	lTCrt;	// 临时爆击率			临时增加的爆击率
	long	lTHRec;	// 临时hp恢复速度		临时增加的每秒钟恢复hp的数值
	long	lTSRec;	// 临时sp恢复速度		临时增加的每秒钟恢复sp的数值
	long	lTASpd;	// 临时攻击间隔			临时增加玩家攻击间隔
	long	lTADis;	// 临时攻击距离			临时增加玩家的攻击距离
	long	lTSpd;	// 临时移动速度			临时增加的移动速度
	long	lTSpri;	// 临时飞行速度			临时跑弹每秒钟飞行的距离
	long	lTScsm;	// 临时补给消耗			临时改变的单位时间内的消耗

	// added by clp 缩放信息
	float	scaling[3];

public:
	// 以下两函数用于与血相关时,要显示的特效
	bool	HaveEffectFog()				{ return _HaveEffectFog;	}
	int		GetEffectFog( int i )		{ return _nHPEffect[i];		}

	bool	IsFace()					{ return _IsFace;			}			// 是否可旋转
	bool	IsCyclone()					{ return _IsCyclone;		}			// 是否可被飓风等吹起来

	void	RefreshPrivateData();				// 刷新内部数据

private:
	bool	_HaveEffectFog;

};

class CChaRecordSet : public CRawDataSet
{
public:
	static CChaRecordSet* I() { return _Instance; }

	CChaRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CChaRecordSet* _Instance; // 相当于单键, 把自己记住

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CChaRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CChaRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CChaRecord);
	}

	virtual void*	_CreateNewRawData(CRawDataInfo *pInfo)
	{
		return NULL;
	}

	virtual void  _DeleteRawData(CRawDataInfo *pInfo)
	{
		SAFE_DELETE(pInfo->pData);
	}

	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList);
	virtual void _ProcessRawDataInfo(CRawDataInfo *pInfo);

};

inline CChaRecord* GetChaRecordInfo( int nTypeID )
{
	return (CChaRecord*)CChaRecordSet::I()->GetRawDataInfo(nTypeID);
}

#endif // CHARACTERRECORD_H