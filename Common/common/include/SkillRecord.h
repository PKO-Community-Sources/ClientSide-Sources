//======================================================================================================================
// FileName: SkillRecord.h
// Creater: ZhangXuedong
// Date: 2004.09.01
// Comment: CSkillRecordSet class
//======================================================================================================================

#ifndef	SKILLRECORD_H
#define	SKILLRECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"
#include "SkillBag.h"

const char cchSkillRecordKeyValue = (char)0xfe;				// Ϊ-1ʱ�Ƕ�������,Ϊ0ʱ�ǿ���,���Ը�Ϊ-2

#define	defSKILL_NAME_LEN	17
#define defSKILL_JOB_SELECT_NUM	9
#define defSKILL_ITEM_NEED_NUM	8
#define defSKILL_PRE_SKILL_NUM	3
#define defSKILL_ICON_NAME_LEN	17
#define defEFFECT_SELF_ATTR_NUM	2
#define defEFFECT_TAR_ATTR_NUM	2
#define defSELF_EFFECT_NUM		2
#define defEXPEND_ITEM_NUM	2
#define defSKILL_OPERATE_NUM	3
#define defSKILL_POSE_NUM       10
#define defSKILL_EFFECT_SCRIPT_LEN	33
#define defSKILL_RANGE_SET_SCRIPT	33
#define defSKILL_ACTION_EFFECT  3
#define defSKILL_ITEM_EFFECT    2

enum ESkillItemNeed
{
	enumSKILL_ITEM_NEED_TYPE	= 1,
	enumSKILL_ITEM_NEED_ID		= 2,
};

enum ESkillObjType
{
	// �ü��ܵ�ʩ��Ŀ��ѡ��1���Լ���2�����ѣ�3��������4�����ˣ�5��ȫ��

	enumSKILL_TYPE_SELF		= 1,	// ����
	enumSKILL_TYPE_TEAM		= 2,	// ����
	enumSKILL_TYPE_SCENE	= 3,	// ������Ч
	enumSKILL_TYPE_ENEMY	= 4,	// ����
	enumSKILL_TYPE_ALL		= 5,	// ȫ��
	enumSKILL_TYPE_PLAYER_DIE	= 6,	// ���ʬ��
	enumSKILL_TYPE_EXCEPT_SELF  = 7,    // �����Լ���������н�ɫ�͹���

	// ���⼼������
	enumSKILL_TYPE_REPAIR	= 17,	// �޲�
	enumSKILL_TYPE_TREE		= 18,	// ����
	enumSKILL_TYPE_MINE		= 19,	// ����
	enumSKILL_TYPE_TRADE	= 22,	// ��̯
	enumSKILL_TYPE_FISH		= 28,	// ����
	enumSKILL_TYPE_SALVAGE	= 29,	// ���̳���

};

enum eSelectCha		// ���ͻ�����
{
	enumSC_NONE = 0,		// ���ܶ�������ʹ��
	enumSC_ALL,				// ��������ʹ��
	enumSC_PLAYER,			// �������,�������Լ�,���ڲ˵�

	enumSC_ENEMY,			// ����:��PK�����+��ͬ�����,�����Ϊ����
	enumSC_PLAYER_ASHES,	// ���ʬ��(��������)

	enumSC_MONS,			// ��ͨ����
	enumSC_MONS_REPAIRABLE, // ���޲�����
	enumSC_MONS_TREE,       // ������
	enumSC_MONS_MINE,		// ��ʯ����
	enumSC_MONS_FISH,		// �����
	enumSC_MONS_DBOAT,		// ��������

	enumSC_SELF,			// ����Լ�
	enumSC_TEAM,			// ��Ա
};

enum ESkillEffType
{
	enumSKILL_EFF_BANEFUL	= 0,	// �к���
	enumSKILL_EFF_HELPFUL	= 1,	// �����
};

enum ESkillUpgrade
{
	enumSKILL_UPGRADE_NONE,			// ��������
	enumSKILL_UPGRADE_CAN,			// ������
	enumSKILL_UPGRADE_MAX,			// �Ѿ��������ֵ
};

enum ESkillFightType
{
	enumSKILL_LAND_LIVE		= 0, // ½�������
	enumSKILL_FIGHT			= 1, // ս������
	enumSKILL_SAIL			= 2, // ��������
	enumSKILL_SEE_LIVE		= 3, // ���������
};

enum ESkillSrcType
{
	enumSKILL_SRC_HUMAN	= 1,	// ��
	enumSKILL_SRC_BOAT	= 2,	// ��
};

enum ESkillTarHabitatType
{
	enumSKILL_TAR_LAND	= 1,	// ½��
	enumSKILL_TAR_SEA	= 2,	// ����
	enumSKILL_TAR_LORS	= 3,	// ����
};

enum ESkillType
{
	enumSKILL_INBORN	= 0,	// �����ļ���
	enumSKILL_ACTIVE	= 1,	// �����ļ���
	enumSKILL_PASSIVE	= 2,	// �����ļ���
};

enum ESkillPhaseType
{
	enumSKILL_NOT_MANUAL_ADD   = 6,	// ��Ҳ������ֶ�����
};


class CSkillRecord : public CRawDataInfo
{
public:
	CSkillRecord();

	short	sID;												// ���
	_TCHAR	szName[defSKILL_NAME_LEN];							// ����
	char    chFightType;										// �������ͣ�����ܣ�ս������
	char	chJobSelect[defSKILL_JOB_SELECT_NUM][2];			// ����ѧϰ�ü��ܵ�ְҵ������Ӧ��ְҵ����ѧ������ߵȼ���
																// 0������ 1����ʿ 2���ѻ��� 3������ 4������ʿ 5������ 6��ҽ�� 7����ʦ 8������
	short	sItemNeed[3][defSKILL_ITEM_NEED_NUM][2];			// ��(0)��(1)�֣�����(2)����װ����Ӧ�ĵ��߲ſ�ʹ�ü��ܡ�
	short	sConchNeed[defSKILL_ITEM_NEED_NUM][3];				// ��������װ����λ�����ͣ���ţ�װ����λ�����ͣ���ţ���������
	char	chPhase;											// ���ܽ׶Σ�1�����м��ܣ�2���������ܣ�3�����׼��ܣ�4���ռ����ܣ�5�����Ѽ��ܣ�6�����ܵȼ��ܷ��ֶ����
	char	chType;												// �������ͣ�1���������ܣ��ֶ��ͷţ���2���������ܣ��Զ��ͷţ�
	short	sLevelDemand;										// ѧϰ�ü�����Ҫ�ĵȼ�
	short	sPremissSkill[defSKILL_PRE_SKILL_NUM][2];			// ��Ҫ��3��ǰ�Ἴ�ܣ����ܱ�ţ��ȼ���
	char	chPointExpend;										// ���ĵļ��ܵ�
	char	chSrcType;											// ʩ������̬���ˣ�����
	char	chTarType;											// Ŀ�����ͣ�½������������
	short	sApplyDistance;										// ʹ�þ��롣��λ����
	char	chApplyTarget;										// ʹ�õ�Ŀ�ꡣ
	char	chApplyType;										// �������ͣ�1�����壬2����Χ��3������
	char	chHelpful;											// ��������,�����к�
	short	sAngle;												// �Ƕȣ�0-360�������ڡ���Χ���������͡�
	short	sRadii;												// �뾶�����ڡ���Χ���������͡�
	char	chRange;											// ����������״���
	char	szPrepare[defSKILL_RANGE_SET_SCRIPT];				// ׼������
	char	szUseSP[defSKILL_EFFECT_SCRIPT_LEN];				// ����SP��ʽ
	char	szUseEndure[defSKILL_EFFECT_SCRIPT_LEN];			// ���ġ��;öȡ���ʽ
	char	szUseEnergy[defSKILL_EFFECT_SCRIPT_LEN];			// ���ġ���������ʽ
	char	szSetRange[defSKILL_EFFECT_SCRIPT_LEN];				// ���������趨
	char	szRangeState[defSKILL_EFFECT_SCRIPT_LEN];			// �ر�״̬�趨
	char	szUse[defSKILL_EFFECT_SCRIPT_LEN];					// ʩ�Ž׶ι�ʽ
	char	szEffect[defSKILL_EFFECT_SCRIPT_LEN];				// Ч���׶ι�ʽ
	char	szActive[defSKILL_EFFECT_SCRIPT_LEN];				// ��Ч����ʽ
	char	szInactive[defSKILL_EFFECT_SCRIPT_LEN];				// ��Ч����ʽ
	int		nStateID;											// �󶨵Ŀ��ֶ��Ƴ�״̬���
	short	sSelfAttr[defEFFECT_SELF_ATTR_NUM];					// ����������ֵ�ı仯����ʽ��š�
	short	sSelfEffect[defSELF_EFFECT_NUM];					// ����Ч��
	short	sItemExpend[defEXPEND_ITEM_NUM][2];					// ���ĵ���Ʒ��ID��������
	short	sBeingTime;											// ����ʱ��
	short	sTargetAttr[defEFFECT_TAR_ATTR_NUM];				// Ŀ��������ֵ�ı仯����ʽ��š�
	short	sSplashPara;										// �������
	short	sTargetEffect;										// Ŀ�����Ч��
	short	sSplashEffect;										// �������Ч��
	short	sVariation;											// ������
	short	sSummon;											// �ٻ����
	short	sPreTime;											// �ͷ�ʱ��
	char	szFireSpeed[defSKILL_EFFECT_SCRIPT_LEN];			// ��ʩ���ٶȹ�ʽ
	char	chOperate[defSKILL_OPERATE_NUM];					// ���������0���ޡ�1���ϴ���2���´�

public:		// �ͻ��˱�����
	short	sActionHarm;										// ������Ч�����˺����ַ�ʽ
	char	chActionPlayType;									// �������ŷ�ʽ:0-˳�򲥷�,1-�������
	short	sActionPose[defSKILL_POSE_NUM];						// �ͷŸü������ǵĶ���
	short	sActionKeyFrme;										// ���ͷż��ܶ����ؼ���
	short	sWhop;												// ������Ч
	short	sActionDummyLink[defSKILL_ACTION_EFFECT];			// �ͷ�������link��
	short	sActionEffect[defSKILL_ACTION_EFFECT];				// �ͷŸü������ǵ���Ч
	short	sActionEffectType[defSKILL_ACTION_EFFECT];			// �ͷŸü��ܵķ�ʽ:0-�ޱ仯,1-���Զ�����
	short	sItemDummyLink;										// �ͷ��ߵ���link��
	short	sItemEffect1[defSKILL_ITEM_EFFECT];					// �ͷ��ߵ�����Ч,0λ-������Ч,1λ-ִ��ʱ���
	short	sItemEffect2[defSKILL_ITEM_EFFECT];					// 
	short	sSkyEffectActionKeyFrame;							// �ɳ���Ч�����ؼ���
	short   sSkyEffectActionDummyLink;							// ���г���������dummy
	short   sSkyEffectItemDummyLink;							// ���г��������dummy;
	short   sSkyEffect;											// �ͷŸü��ܷ��е���Ч
	short	sSkySpd;											// �����ٶ�
	short	sWhoped;											// �ܻ���Ч
	short   sTargetDummyLink;									// �ܻ���link��
	short	sTargetEffectID;									// �ܻ���Ч
	char	chTargetEffectTime;									// �ܻ���Чʱ��� ,0-������ʼ��1���ؼ�֡��2����������(������Ч�˺���Ч)
    short   sAgroundEffectID;                                   // ���ڷ�Χ����,���ϵĳ��ֵ���Ч������ˮ��
	short	sWaterEffectID;										// ���ڷ�Χ����,ˮ���ϵĳ��ֵ���Ч 
	char	szICON[defSKILL_ICON_NAME_LEN];						// �ü�������Ϸ������ֵ�ͼ��
	char	chPlayTime;											// ���ܲ��Ŵ���
	char	szDescribeHint[128];								// ����,����hint
	char	szEffectHint[128];									// Ч��,����hint
	char	szExpendHint[128];									// ����,����hint

public:
    bool    IsPlayCyc()         { return chPlayTime==1;     }   // �Ƿ�ѭ������
    bool    IsAttackArea()		{ return chApplyType==2;    }   // �Ƿ񹥻�����
	bool	IsHarmRange()		{ return chApplyType!=1;	}	// �Ƿ�Χ�˺�
    bool    IsActKeyHarm()      { return sActionHarm==1;    }   // �Ƿ����ؼ�֡�˺�
    bool    IsEffectHarm()      { return sActionHarm==2;    }   // �Ƿ���Ч�˺�
    bool    IsNoHarm()          { return sActionHarm==0;    }   // ���˺�����
	bool	IsShow()			{ return szICON[1]!='\0';	}	// �Ƿ���ʾ����
	bool	IsPlayRand()		{ return chActionPlayType==1;	}
	int		GetPoseNum()		{ return _nPoseNum;			}

public:
    void    SetSkillGrid( SSkillGridEx& v )   { _Skill = v;   }
    SSkillGridEx&     GetSkillGrid()  { return _Skill;        }

	int     GetSPExpend()       { return _Skill.sUseSP;     }   // SP����
    short   GetRange()          { return _Skill.sRange[1];  }   // ������Χ
    short*  GetParam()          { return &_Skill.sRange[1]; }
    int     GetShape()          { return _Skill.sRange[0];  }   // ������״
    int     GetDistance()       { return sApplyDistance;    }   // ʹ�þ���
    int     GetLevel()          { return _Skill.chLv;       }
    int     GetFireSpeed()      { return _Skill.lResumeTime;}   // �����ٶ�
    bool    GetIsValid()        { return _Skill.chState!=0; }   // �Ƿ����

	int     GetUpgrade()		{ return _nUpgrade;			}
	bool    GetIsUpgrade()		{ return _nUpgrade==enumSKILL_UPGRADE_CAN;	}

	bool	GetIsUse()			{ return chType!=2;			}
	bool	GetIsHelpful()		{ return chHelpful==1;		}	// ����������ģ������ڰ�ȫ��

	void	Refresh( int nJob );

	void	SetIsActive( bool v )	{ _IsActive = v;		}
	bool	GetIsActive()			{ return _IsActive;		}

	void	SetAttackTime( DWORD v ){ _dwAttackTime=v;			}
	bool	IsAttackTime( DWORD v )	{ return v>=_dwAttackTime;	}

	int		GetSelectCha()		{ return _eSelectCha;		}

	bool	IsAutoAttack()		{ return !IsPlayCyc() && GetDistance()>0;	}

	void	RefreshPrivateData();				// ˢ���ڲ�����
	
	bool	IsJobAllow( int nJob );
	int		GetJobMax( int nJob );

private:
    SSkillGridEx		_Skill;
	int					_nUpgrade;				// 0-��������,1-������,2-�Ѿ����������ֵ

	bool				_IsActive;				// ���ڶ�������,���ڼ���Ļ�һ����̬�߿�
	DWORD				_dwAttackTime;			// �´οɹ�����ʱ��

	eSelectCha			_eSelectCha;			// ���ڳ����жϿ���ѡ����Щ��ҹ���

	int					_nPoseNum;				// �ܹ���pose����

	// added by clp
public:
	bool IsReadingSkill()
	{
		return sID == 512;
	}
};

class CSkillRecordSet : public CRawDataSet
{
public:

	static CSkillRecordSet* I() { return _Instance; }

	CSkillRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CSkillRecordSet* _Instance; // �൱�ڵ���, ���Լ���ס

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CSkillRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CSkillRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CSkillRecord);
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

inline CSkillRecord* GetSkillRecordInfo( int nTypeID )
{
	return (CSkillRecord*)CSkillRecordSet::I()->GetRawDataInfo(nTypeID);
}

inline CSkillRecord* GetSkillRecordInfo( const char* szName )
{
	return (CSkillRecord*)CSkillRecordSet::I()->GetRawDataInfo(szName);
}

inline bool CSkillRecord::IsJobAllow( int nJob )
{
	if( chJobSelect[0][0]==-1 ) return true;

	for (char i = 0; i<defSKILL_JOB_SELECT_NUM; i++ )
	{
		if (chJobSelect[i][0] == cchSkillRecordKeyValue)
			return false;
		else if( chJobSelect[i][0]==nJob )
			return true;
	}
	return false;
}

inline int CSkillRecord::GetJobMax( int nJob )
{
	if( chJobSelect[0][0]==-1 ) return chJobSelect[0][1];

	for (int i = 0; i<defSKILL_JOB_SELECT_NUM; i++ )
	{
		if (chJobSelect[i][0] == cchSkillRecordKeyValue)
			return -1;
		else if( chJobSelect[i][0]==nJob )
			return chJobSelect[i][1];
	}

	return -1;
}
#endif //SKILLRECORD_H