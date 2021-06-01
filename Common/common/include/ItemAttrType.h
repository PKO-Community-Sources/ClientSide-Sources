//=============================================================================
// FileName: ItemAttrType.h
// Creater: ZhangXuedong
// Date: 2004.12.28
// Comment: Item Attribute type
//=============================================================================

#ifndef ITEMATTRTYPE_H
#define ITEMATTRTYPE_H

// ！！！注意，道具属性编号必须大于0（与外观信息有约定）
// ！！！注意，道具属性编号的顺序要和角色属性编号的顺序一致

const long	ITEMATTR_COUNT_BASE0    = 0;
const long	ITEMATTR_COE_STR        = ITEMATTR_COUNT_BASE0 + 1; // 力量系数加成（strength coefficient）
const long	ITEMATTR_COE_AGI        = ITEMATTR_COUNT_BASE0 + 2; // 敏捷系数加成
const long	ITEMATTR_COE_DEX        = ITEMATTR_COUNT_BASE0 + 3; // 专注系数加成
const long	ITEMATTR_COE_CON        = ITEMATTR_COUNT_BASE0 + 4; // 体质系数加成
const long	ITEMATTR_COE_STA        = ITEMATTR_COUNT_BASE0 + 5; // 精力系数加成
const long	ITEMATTR_COE_LUK        = ITEMATTR_COUNT_BASE0 + 6; // 幸运系数加成
const long	ITEMATTR_COE_ASPD       = ITEMATTR_COUNT_BASE0 + 7; // 攻击频率系数加成
const long	ITEMATTR_COE_ADIS       = ITEMATTR_COUNT_BASE0 + 8; // 攻击距离系数加成
const long	ITEMATTR_COE_MNATK      = ITEMATTR_COUNT_BASE0 + 9; // 最小攻击力系数加成
const long	ITEMATTR_COE_MXATK      = ITEMATTR_COUNT_BASE0 + 10; // 最大攻击力系数加成
const long	ITEMATTR_COE_DEF        = ITEMATTR_COUNT_BASE0 + 11; // 防御系数加成
const long	ITEMATTR_COE_MXHP       = ITEMATTR_COUNT_BASE0 + 12; // 最大Hp系数加成
const long	ITEMATTR_COE_MXSP       = ITEMATTR_COUNT_BASE0 + 13; // 最大Sp系数加成
const long	ITEMATTR_COE_FLEE       = ITEMATTR_COUNT_BASE0 + 14; // 闪避率系数加成
const long	ITEMATTR_COE_HIT        = ITEMATTR_COUNT_BASE0 + 15; // 命中率系数加成
const long	ITEMATTR_COE_CRT        = ITEMATTR_COUNT_BASE0 + 16; // 爆击率系数加成
const long	ITEMATTR_COE_MF         = ITEMATTR_COUNT_BASE0 + 17; // 寻宝率系数加成
const long	ITEMATTR_COE_HREC       = ITEMATTR_COUNT_BASE0 + 18; // hp恢复速度系数加成
const long	ITEMATTR_COE_SREC       = ITEMATTR_COUNT_BASE0 + 19; // sp恢复速度系数加成
const long	ITEMATTR_COE_MSPD       = ITEMATTR_COUNT_BASE0 + 20; // 移动速度系数加成
const long	ITEMATTR_COE_COL        = ITEMATTR_COUNT_BASE0 + 21; // 资源采集速度系数加成
const long	ITEMATTR_COE_PDEF       = ITEMATTR_COUNT_BASE0 + 22; // 物理抵抗系数加成

const long	ITEMATTR_COUNT_BASE1    = 25;
const long	ITEMATTR_VAL_STR        = ITEMATTR_COUNT_BASE1 + 1; // 力量常数加成（strength value）
const long	ITEMATTR_VAL_AGI        = ITEMATTR_COUNT_BASE1 + 2; // 敏捷常数加成
const long	ITEMATTR_VAL_DEX        = ITEMATTR_COUNT_BASE1 + 3; // 专注常数加成
const long	ITEMATTR_VAL_CON        = ITEMATTR_COUNT_BASE1 + 4; // 体质常数加成
const long	ITEMATTR_VAL_STA        = ITEMATTR_COUNT_BASE1 + 5; // 精力常数加成
const long	ITEMATTR_VAL_LUK        = ITEMATTR_COUNT_BASE1 + 6; // 幸运常数加成
const long	ITEMATTR_VAL_ASPD       = ITEMATTR_COUNT_BASE1 + 7; // 攻击频率常数加成
const long	ITEMATTR_VAL_ADIS       = ITEMATTR_COUNT_BASE1 + 8; // 攻击距离常数加成
const long	ITEMATTR_VAL_MNATK      = ITEMATTR_COUNT_BASE1 + 9; // 最小攻击力常数加成
const long	ITEMATTR_VAL_MXATK      = ITEMATTR_COUNT_BASE1 + 10; // 最大攻击力常数加成
const long	ITEMATTR_VAL_DEF        = ITEMATTR_COUNT_BASE1 + 11; // 防御常数加成
const long	ITEMATTR_VAL_MXHP       = ITEMATTR_COUNT_BASE1 + 12; // 最大Hp常数加成
const long	ITEMATTR_VAL_MXSP       = ITEMATTR_COUNT_BASE1 + 13; // 最大Sp常数加成
const long	ITEMATTR_VAL_FLEE       = ITEMATTR_COUNT_BASE1 + 14; // 闪避率常数加成
const long	ITEMATTR_VAL_HIT        = ITEMATTR_COUNT_BASE1 + 15; // 命中率常数加成
const long	ITEMATTR_VAL_CRT        = ITEMATTR_COUNT_BASE1 + 16; // 爆击率常数加成
const long	ITEMATTR_VAL_MF         = ITEMATTR_COUNT_BASE1 + 17; // 寻宝率常数加成
const long	ITEMATTR_VAL_HREC       = ITEMATTR_COUNT_BASE1 + 18; // hp恢复速度常数加成
const long	ITEMATTR_VAL_SREC       = ITEMATTR_COUNT_BASE1 + 19; // sp恢复速度常数加成
const long	ITEMATTR_VAL_MSPD       = ITEMATTR_COUNT_BASE1 + 20; // 移动速度常数加成
const long	ITEMATTR_VAL_COL        = ITEMATTR_COUNT_BASE1 + 21; // 资源采集速度常数加成
const long	ITEMATTR_VAL_PDEF       = ITEMATTR_COUNT_BASE1 + 22; // 物理抵抗常数加成

const long	ITEMATTR_COUNT_BASE2    = 49;
const long	ITEMATTR_LHAND_VAL      = ITEMATTR_COUNT_BASE2 + 1; // 道具左手加成
const long	ITEMATTR_MAXURE	        = ITEMATTR_COUNT_BASE2 + 2; // 最大耐久度
const long	ITEMATTR_MAXFORGE       = ITEMATTR_COUNT_BASE2 + 3; // 最大精炼等级
const long	ITEMATTR_MAXENERGY      = ITEMATTR_COUNT_BASE2 + 4; // 最大能量
const long	ITEMATTR_URE            = ITEMATTR_COUNT_BASE2 + 5; // 当前耐久度
const long	ITEMATTR_FORGE          = ITEMATTR_COUNT_BASE2 + 6; // 当前精炼等级
const long	ITEMATTR_ENERGY         = ITEMATTR_COUNT_BASE2 + 7; // 当前能量

const long	ITEMATTR_MAX_NUM        = 58;
const long  ITEMATTR_CLIENT_MAX     = ITEMATTR_VAL_PDEF + 1; // 客户端用于读取属性用，因为最后几个属性不需要显示

const long  ITEMATTR_COUNT_BASE3    = 180;
const long  ITEMATTR_VAL_PARAM1		= ITEMATTR_COUNT_BASE3 + 1;	// 道具附加信息一
const long  ITEMATTR_VAL_PARAM2		= ITEMATTR_COUNT_BASE3 + 2;	// 道具附加信息二
const long  ITEMATTR_VAL_LEVEL		= ITEMATTR_COUNT_BASE3 + 3; // 道具装备等级信息
const long  ITEMATTR_VAL_FUSIONID   = ITEMATTR_COUNT_BASE3 + 4; // 道具装备熔合信息ID

#endif // ITEMATTRTYPE_H
