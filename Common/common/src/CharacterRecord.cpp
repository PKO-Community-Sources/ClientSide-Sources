//======================================================================================================================
// FileName: CharacterRecord.cpp
// Creater: ZhangXuedong
// Date: 2004.09.01
// Comment: CChaRecordSet class
//======================================================================================================================

#include "CharacterRecord.h"
#include <memory.h>

//---------------------------------------------------------------------------
// class CChaRecord
//---------------------------------------------------------------------------
void CChaRecord::RefreshPrivateData()
{
	_HaveEffectFog = false;
	for( int i=0; i<defCHA_HP_EFFECT_NUM; i++ )
	{
		if( _nHPEffect[i] != 0 ) 
		{
			_HaveEffectFog = true;
			break;
		}
	}
}

//---------------------------------------------------------------------------
// class CChaRecordSet
//---------------------------------------------------------------------------
CChaRecordSet * CChaRecordSet::_Instance = NULL;

BOOL CChaRecordSet::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
{T_B
	if(ParamList.size()==0) return FALSE;

	CChaRecord *pInfo = (CChaRecord*)pRawDataInfo;

	pInfo->lID = pInfo->nID;
	_tcsncpy(pInfo->szName, pInfo->szDataName, defCHA_NAME_LEN);
	pInfo->szName[defCHA_NAME_LEN - 1] = _TEXT('\0');

	int m = 0, n = 0;
    string strList[80];
	string strLine;

	// 头像图标名称
	_tcsncpy(pInfo->szIconName, ParamList[m++].c_str(), defCHA_ICON_NAME_LEN);
	pInfo->szIconName[defCHA_ICON_NAME_LEN - 1] = _TEXT('\0');
	// 模型类型
	pInfo->chModalType = Str2Int(ParamList[m++]);
	// 控制类型
	pInfo->chCtrlType = Str2Int(ParamList[m++]);
	// 骨架号
	pInfo->sModel = Str2Int(ParamList[m++]);
	// 套号
	pInfo->sSuitID = Str2Int(ParamList[m++]);
	// 套装数量
	pInfo->sSuitNum = Str2Int(ParamList[m++]);
	// 皮肤
	memset(pInfo->sSkinInfo, cchChaRecordKeyValue, sizeof(pInfo->sSkinInfo));
	for (int i = 0; i < defCHA_SKIN_NUM; i++)
	{
		pInfo->sSkinInfo[i] = Str2Int(ParamList[m++]);
	}
	//// FeffID
	//pInfo->sFeffID = Str2Int(ParamList[m++]);
	// //EeffID
	//pInfo->sEeffID = Str2Int(ParamList[m++]);
	string lstr[4]; 
	strLine = ParamList[m++];

	n = Util_ResolveTextLine(strLine.c_str(), lstr, 4, ',');
	memset(pInfo->sFeffID,0 ,sizeof(pInfo->sFeffID));
	for (int e = 0; e < n; e++)
	{
		pInfo->sFeffID[e] = Str2Int(lstr[e]);
	}
	pInfo->sEeffID = n;
	m++;

	// 特效动作编号
	memset(pInfo->sEffectActionID, 0, sizeof(pInfo->sEffectActionID));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine( strLine.c_str(), strList, 80, ',' );
	int nCount = 
	n = n > 3 ? 3 : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->sEffectActionID[i] = Str2Int(strList[i]);
	}
	// 影子
	pInfo->sShadow = Str2Int(ParamList[m++]);
    // 动作编号
    pInfo->sActionID = Str2Int(ParamList[m++]);    
    // 透明度
    pInfo->chDiaphaneity = Str2Int(ParamList[m++]);
	// 行走音效
	pInfo->sFootfall = Str2Int(ParamList[m++]);
	// 喘息音效
	pInfo->sWhoop = Str2Int(ParamList[m++]);
	// 死亡音效
	pInfo->sDirge = Str2Int(ParamList[m++]);
	// 是否可控
	pInfo->chControlAble = Str2Int(ParamList[m++]);
	//// 可否移动
	//pInfo->chMoveAble = Str2Int(ParamList[m++]);
	// 区域限制
	pInfo->chTerritory = Str2Int(ParamList[m++]);
	// 高度偏移
	pInfo->sSeaHeight = Str2Int(ParamList[m++]);	
	// 可装备道具
	memset(pInfo->sItemType, cchChaRecordKeyValue, sizeof(pInfo->sItemType));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_ITEM_KIND_NUM ? defCHA_ITEM_KIND_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->sItemType[i] = Str2Int(strList[i]);
	}
	// 长（米）
	pInfo->fLengh = Str2Float(ParamList[m++]);
	// 宽（米）
	pInfo->fWidth = Str2Float(ParamList[m++]);
	// 高（米）
	pInfo->fHeight = Str2Float(ParamList[m++]);
	// 高（米）
	pInfo->sRadii = Str2Int(ParamList[m++]);

	// 出生表现
	memset( pInfo->nBirthBehave, 0, sizeof(pInfo->nBirthBehave) );
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_BIRTH_EFFECT_NUM ? defCHA_BIRTH_EFFECT_NUM : n;	
	for (int i = 0; i < n; i++)
	{		
		pInfo->nBirthBehave[i] = Str2Int(strList[i]);
	}

	// 死亡表现
	memset( pInfo->nDiedBehave, 0, sizeof(pInfo->nDiedBehave) );
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_DIE_EFFECT_NUM ? defCHA_DIE_EFFECT_NUM : n;	
	for (int i = 0; i < n; i++)
	{		
		pInfo->nDiedBehave[i] = Str2Int(strList[i]);
	}

    // 出生特效
    pInfo->sBornEff = Str2Int(ParamList[m++]);

    // 死亡特效
    pInfo->sDieEff = Str2Int(ParamList[m++]);

	// 休眠动作
    pInfo->sDormancy = Str2Int(ParamList[m++]);

    // 死亡时的动作
    pInfo->chDieAction = Str2Int(ParamList[m++]);

	// 剩余hp特效表现
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_HP_EFFECT_NUM ? defCHA_HP_EFFECT_NUM : n;
	memset( pInfo->_nHPEffect, 0, sizeof(pInfo->_nHPEffect) );
	for (int i = 0; i < n; i++)
	{
		pInfo->_nHPEffect[i] = Str2Int(strList[i]);
	}

	// 是否可旋转
	pInfo->_IsFace = Str2Int(ParamList[m++]) != 0 ? true : false;

	// 是否可被飓风吹起
	pInfo->_IsCyclone = Str2Int(ParamList[m++]) != 0 ? true : false;

	// 脚本文件ID
	pInfo->lScript = Str2Int(ParamList[m++]);
	// 道具表中的武器ID
	pInfo->lWeapon = Str2Int(ParamList[m++]);

	// 技能及使用频率
	memset(pInfo->lSkill, cchChaRecordKeyValue, sizeof(pInfo->lSkill));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_INIT_SKILL_NUM ? defCHA_INIT_SKILL_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lSkill[i][0] = Str2Int(strList[i]);
	}

	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_INIT_SKILL_NUM ? defCHA_INIT_SKILL_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lSkill[i][1] = Str2Int(strList[i]);
	}

	// 物品及掉料几率
	for (int i = 0; i < defCHA_INIT_ITEM_NUM; i++)
	{
		pInfo->lItem[i][0] = cchChaRecordKeyValue;
		pInfo->lItem[i][1] = cchChaRecordKeyValue;
	}
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_INIT_ITEM_NUM ? defCHA_INIT_ITEM_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lItem[i][0] = Str2Int(strList[i]);
	}

	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_INIT_ITEM_NUM ? defCHA_INIT_ITEM_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lItem[i][1] = Str2Int(strList[i]);
	}

	// 每次最多爆出物品数量
	pInfo->lMaxShowItem = Str2Int(ParamList[m++]);
	// 打爆几率
	pInfo->fAllShow = Str2Float(ParamList[m++]);
	// 前缀等级
	pInfo->lPrefix = Str2Int(ParamList[m++]);

	// 任务物品及掉料几率
	for (int i = 0; i < defCHA_INIT_ITEM_NUM; i++)
	{
		pInfo->lTaskItem[i][0] = cchChaRecordKeyValue;
		pInfo->lTaskItem[i][1] = cchChaRecordKeyValue;
	}
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_INIT_ITEM_NUM ? defCHA_INIT_ITEM_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lTaskItem[i][0] = Str2Int(strList[i]);
	}

	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_INIT_ITEM_NUM ? defCHA_INIT_ITEM_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lTaskItem[i][1] = Str2Int(strList[i]);
	}

	// AI编号
	pInfo->lAiNo = Str2Int(ParamList[m++]);
	// 攻击时是否转向
	pInfo->chCanTurn = Str2Int(ParamList[m++]);
	// 视野范围（厘米）
	pInfo->lVision = Str2Int(ParamList[m++]);
	// 喊叫范围（厘米）
	pInfo->lNoise = Str2Int(ParamList[m++]);
	// 直接获得的EXP
	pInfo->lGetEXP = Str2Int(ParamList[m++]);
	// 是否受光照影响
	pInfo->bLight = Str2Int(ParamList[m++]) ? true : false;

	// 普通exp
	pInfo->lMobexp = Str2Int(ParamList[m++]);
	// 角色等级
	pInfo->lLv = Str2Int(ParamList[m++]);
	// 最大HP
	pInfo->lMxHp = Str2Int(ParamList[m++]);
	// 当前HP
	pInfo->lHp = Str2Int(ParamList[m++]);
	// 最大SP
	pInfo->lMxSp = Str2Int(ParamList[m++]);
	// 当前SP
	pInfo->lSp = Str2Int(ParamList[m++]);
	// 最小攻击力
	pInfo->lMnAtk = Str2Int(ParamList[m++]);
	// 最大攻击力
	pInfo->lMxAtk = Str2Int(ParamList[m++]);
	// 物理抵抗
	pInfo->lPDef = Str2Int(ParamList[m++]);
	// 防御力
	pInfo->lDef = Str2Int(ParamList[m++]);
	// 命中率
	pInfo->lHit = Str2Int(ParamList[m++]);
	// 闪避率
	pInfo->lFlee = Str2Int(ParamList[m++]);
	// 爆击率
	pInfo->lCrt = Str2Int(ParamList[m++]);
	// 寻宝率
	pInfo->lMf = Str2Int(ParamList[m++]);
	// hp恢复速度
	pInfo->lHRec = Str2Int(ParamList[m++]);
	// sp恢复速度
	pInfo->lSRec = Str2Int(ParamList[m++]);
	// 攻击间隔
	pInfo->lASpd = Str2Int(ParamList[m++]);
	// 攻击距离
	pInfo->lADis = Str2Int(ParamList[m++]);
	// 追击范围
	pInfo->lCDis = Str2Int(ParamList[m++]);
	// 移动速度
	pInfo->lMSpd = Str2Int(ParamList[m++]);
	// 资源采集速度
	pInfo->lCol = Str2Int(ParamList[m++]);
	// 力量
	pInfo->lStr = Str2Int(ParamList[m++]);
	// 敏捷
	pInfo->lAgi = Str2Int(ParamList[m++]);
	// 专注
	pInfo->lDex = Str2Int(ParamList[m++]);
	// 体质
	pInfo->lCon = Str2Int(ParamList[m++]);
	// 精力
	pInfo->lSta = Str2Int(ParamList[m++]);
	// 幸运
	pInfo->lLuk = Str2Int(ParamList[m++]);
	// 左手武器发挥
	pInfo->lLHandVal = Str2Int(ParamList[m++]);

	// 行会名称
	_tcsncpy(pInfo->szGuild, ParamList[m++].c_str(), defCHA_GUILD_NAME_LEN);
	pInfo->szGuild[defCHA_GUILD_NAME_LEN - 1] = _TEXT('\0');
	// 角色称号
	_tcsncpy(pInfo->szTitle, ParamList[m++].c_str(), defCHA_TITLE_NAME_LEN);
	pInfo->szGuild[defCHA_GUILD_NAME_LEN - 1] = _TEXT('\0');
	// 角色职业
	_tcsncpy(pInfo->szJob, ParamList[m++].c_str(), defCHA_JOB_NAME_LEN);
	pInfo->szGuild[defCHA_GUILD_NAME_LEN - 1] = _TEXT('\0');

	// 当前经验
	pInfo->lCExp = Str2Int(ParamList[m++].c_str());
	// 下一级所需经验
	pInfo->lNExp = Str2Int(ParamList[m++].c_str());
	// 名声
	pInfo->lFame = Str2Int(ParamList[m++]);
	// 属性点
	pInfo->lAp = Str2Int(ParamList[m++]);
	// 技能点
	pInfo->lTp = Str2Int(ParamList[m++]);
	// 金钱
	pInfo->lGd = Str2Int(ParamList[m++]);
	// 炮弹飞行速度
	pInfo->lSpri = Str2Int(ParamList[m++]);
	// 货舱(背包)容量
	pInfo->lStor = Str2Int(ParamList[m++]);
	// 船员上限
	pInfo->lMxSail = Str2Int(ParamList[m++]);
	// 现有船员的数量
	pInfo->lSail = Str2Int(ParamList[m++]);
	// 标准人员配置数量
	pInfo->lStasa = Str2Int(ParamList[m++]);
	// 补给消耗
	pInfo->lScsm = Str2Int(ParamList[m++]);

	// 临时力量
	pInfo->lTStr = Str2Int(ParamList[m++]);
	// 临时敏捷
	pInfo->lTAgi = Str2Int(ParamList[m++]);
	// 临时专注
	pInfo->lTDex = Str2Int(ParamList[m++]);
	// 临时体质
	pInfo->lTCon = Str2Int(ParamList[m++]);
	// 临时耐力
	pInfo->lTSta = Str2Int(ParamList[m++]);
	// 临时幸运
	pInfo->lTLuk = Str2Int(ParamList[m++]);
	// 临时最大血量
	pInfo->lTMxHp = Str2Int(ParamList[m++]);
	// 临时最大精力值
	pInfo->lTMxSp = Str2Int(ParamList[m++]);
	// 临时攻击力
	pInfo->lTAtk = Str2Int(ParamList[m++]);
	// 临时防御力
	pInfo->lTDef = Str2Int(ParamList[m++]);
	// 临时命中率
	pInfo->lTHit = Str2Int(ParamList[m++]);
	// 临时闪避率
	pInfo->lTFlee = Str2Int(ParamList[m++]);
	// 临时寻宝率
	pInfo->lTMf = Str2Int(ParamList[m++]);
	// 临时爆击率
	pInfo->lTCrt = Str2Int(ParamList[m++]);
	// 临时hp恢复速度
	pInfo->lTHRec = Str2Int(ParamList[m++]);
	// 临时sp恢复速度
	pInfo->lTSRec = Str2Int(ParamList[m++]);
	// 临时攻击间隔
	pInfo->lTASpd = Str2Int(ParamList[m++]);
	// 临时攻击距离
	pInfo->lTADis = Str2Int(ParamList[m++]);
	// 临时移动速度
	pInfo->lTSpd = Str2Int(ParamList[m++]);
	// 临时飞行速度
	pInfo->lTSpri = Str2Int(ParamList[m++]);
	// 临时补给消耗
	pInfo->lTScsm = Str2Int(ParamList[m++]);

	// 缩放信息
	memset(pInfo->scaling, 0, sizeof(pInfo->scaling));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 3 ? 3 : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->scaling[i] = Str2Float(strList[i]);
	}

	return TRUE;
T_E}

void CChaRecordSet::_ProcessRawDataInfo(CRawDataInfo *pInfo)
{
	CChaRecord *pChaInfo = (CChaRecord*)pInfo;

    // 更新动态属性
	pChaInfo->RefreshPrivateData();	
}