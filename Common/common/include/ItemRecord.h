//=============================================================================
// FileName: ItemRecord.h
// Creater: ZhangXuedong
// Date: 2004.09.01
// Comment: CItemRecordSet class
//=============================================================================

#ifndef	ITEMRECORD_H
#define ITEMRECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"
#include "CompCommand.h"
#include "JobType.h"

const char cchItemRecordKeyValue = (char)0xfe;	// ���߶�ְҵ��������ռ����-1

#define defITEM_NAME_LEN	80
#define defITEM_MODULE_NUM	5
#define defITEM_MODULE_LEN	19
#define defITEM_ICON_NAME_LEN	17
#define defITEM_ATTREFFECT_NAME_LEN	33

#define defITEM_DESCRIPTOR_NAME_LEN      257
#define defITEM_DESCRIPTOR_SET_LEN      33

#define defITEM_BIND_EFFECT_NUM			8

#define defITEM_BODY 4

enum EItemType
{
	enumItemTypeSword		= 1,	// ��
	enumItemTypeGlave		= 2,	// �޽�
	enumItemTypeBow			= 3,	// ��
	enumItemTypeHarquebus	= 4,	// ����ǹ
	enumItemTypeFalchion	= 5,	// ��
	enumItemTypeMitten		= 6,	// ȭ��
	enumItemTypeStylet		= 7,	// ذ��
	enumItemTypeMoneybag	= 8,	// Ǯ��
	enumItemTypeCosh		= 9,	// �̰�
	enumItemTypeSinker		= 10,	// ����
	enumItemTypeShield		= 11,	// ��
	enumItemTypeArrow		= 12,	// ��
	enumItemTypeAmmo		= 13,	// ��ҩ
	enumItemTypeHeadpiece	= 19,	// ͷ��
	enumItemTypeHair		= 20,	// ͷ��
	enumItemTypeFace		= 21,	// ����
	enumItemTypeClothing	= 22,	// �·�
	enumItemTypeGlove		= 23,	// ����
	enumItemTypeBoot		= 24,	// ѥ��
	enumItemTypeConch		= 29,	// ����
	enumItemTypeMedicine	= 31,	// ҩƷ
	enumItemTypeOvum		= 32,	// ����
	enumItemTypeScroll		= 36,	// �سǾ���
	enumItemTypeGeneral		= 41,	// һ����Ʒ
    enumItemTypeMission     = 42,   // �������
	enumItemTypeBoat		= 43,	// ����֤��
	enumItemTypeWing		= 44,	// ��򣬿ͻ���������ʾ��Ч��
	enumItemTypeTrade		= 45,	// ó��֤��
	enumItemTypeBravery		= 46,	// ����֤֮
	enumItemTypeHull		= 51,	// ����
	enumItemTypeEmbolon		= 52,	// ײ��
	enumItemTypeEngine		= 53,	// ������
	enumItemTypeArtillery	= 54,	// ����
	enumItemTypeAirscrew	= 55,	// ������
	enumItemTypeBoatSign	= 56,	// ����־
	enumItemTypePetFodder	= 57,	// ��������
	enumItemTypePetSock		= 58,	// ������ʳ
	enumItemTypePet			= 59,	// �ػ�����

	enumItemCloak			= 88,
	enumItemMount			= 90,

	// Add by lark.li 20080514 begin
	enumItemTypeNo			= 99,	// ��ƱС��
	// End
};

enum EItemPickTo
{
	enumITEM_PICKTO_KITBAG,
	enumITEM_PICKTO_CABIN,
};

class CItemRecord : public CRawDataInfo
{
public:
	enum EItemExtendInfo
	{
		enumItemNormalStart		= 0,		// ��ͨ����
		enumItemNormalEnd		= 4999,
		enumItemFusionEndure   = 25000,      // �ںϵ�����Ҫ�������;ö�//Modify by ning.yan 20080821 ������Ҫ���ںϵ��ߵ������ɵ�����ʼ����������Ϊ����;öȣ�
		enumItemFusionStart		= 5000,		// �ۺϵ�����ʼ��λ
		enumItemFusionEnd		= 7000,		// �ۺϵ��߽�����λ
		// Modify by lark.li 20080703
		enumItemMax				= 32768,		// ���߱�����¼����
		// End

	};

	CItemRecord();

	long	lID;							// ���
	_TCHAR	szName[defITEM_NAME_LEN];		// ����
	char    szICON[defITEM_ICON_NAME_LEN];	// ICON
	_TCHAR	chModule[defITEM_MODULE_NUM][defITEM_MODULE_LEN];	// ģ��
	short	sShipFlag;						// ����־
	short	sShipType;						// �����ͺ�
	short	sType;							// ����

	char	chForgeLv;						// �����ȼ�
	char	chForgeSteady;					// ����ֵ��������Ʒ���ȶ�ֵ���������ֵ��ÿ����һ�ξ͵ݼ�һ���ɹ��ʣ�
	char	chExclusiveID;					// �Ƿ�ӵ��ΨһID
    char    chIsTrade;                      // �ɽ���
    char    chIsPick;                       // ��ʰȡ
    char    chIsThrow;                      // �ɶ���
	char	chIsDel;						// ������
    long    lPrice;                         // ���׼۸�
	char	chBody[defITEM_BODY];			// ��������
	short	sNeedLv;						// Ҫ��ĵȼ�
    char    szWork[MAX_JOB_TYPE];           // ְҵ

	int		nPileMax;						// �ڵ�����һ���п��Է��õ�������
	char	chInstance;						// �Ƿ�ʵ����
	char	szAbleLink[enumEQUIP_NUM];		// ��װ����λ��
	char	szNeedLink[enumEQUIP_NUM];		// ��Ҫ��λ��
	char	chPickTo;						// 0 ʰȡʱ���뱳����1 ���봬��

	short	sStrCoef;						// ����ϵ���ӳ�
	short	sAgiCoef;						// ����ϵ���ӳ�
	short	sDexCoef;						// רעϵ���ӳ�
	short	sConCoef;						// ����ϵ���ӳ�
	short	sStaCoef;						// ����ϵ���ӳ�
	short	sLukCoef;						// ����ϵ���ӳ�	
	short	sASpdCoef;						// ����Ƶ��ϵ���ӳ�
	short	sADisCoef;						// ��������ϵ���ӳ�
	short	sMnAtkCoef;						// ��С������ϵ���ӳ�
	short	sMxAtkCoef;						// ��󹥻���ϵ���ӳ�
	short	sDefCoef;						// ����ϵ���ӳ�
	short	sMxHpCoef;						// ���Hpϵ���ӳ�
	short	sMxSpCoef;						// ���Spϵ���ӳ�
	short	sFleeCoef;						// ������ϵ���ӳ�
	short	sHitCoef;						// ������ϵ���ӳ�
	short	sCrtCoef;						// ������ϵ���ӳ�
	short	sMfCoef;						// Ѱ����ϵ���ӳ�
	short	sHRecCoef;						// hp�ָ��ٶ�ϵ���ӳ�
	short	sSRecCoef;						// sp�ָ��ٶ�ϵ���ӳ�
	short	sMSpdCoef;						// �ƶ��ٶ�ϵ���ӳ�
	short	sColCoef;						// ��Դ�ɼ��ٶ�ϵ���ӳ�

	short	sStrValu[2];					// ���������ӳɣ���Сֵ�����ֵ��
	short	sAgiValu[2];					// ���ݳ����ӳ�
	short	sDexValu[2];					// רע�����ӳ�
	short	sConValu[2];					// ���ʳ����ӳ�
	short	sStaValu[2];					// ���������ӳ�
	short	sLukValu[2];					// ���˳����ӳ�
	short	sASpdValu[2];					// ����Ƶ�ʳ����ӳ�
	short	sADisValu[2];					// �������볣���ӳ�
	short	sMnAtkValu[2];					// ��С�����������ӳ�
	short	sMxAtkValu[2];					// ��󹥻��������ӳ�
	short	sDefValu[2];					// ���������ӳ�
	short	sMxHpValu[2];					// ���Hp�����ӳ�
	short	sMxSpValu[2];					// ���Sp�����ӳ�
	short	sFleeValu[2];					// �����ʳ����ӳ�
	short	sHitValu[2];					// �����ʳ����ӳ�
	short	sCrtValu[2];					// �����ʳ����ӳ�
	short	sMfValu[2];						// Ѱ���ʳ����ӳ�
	short	sHRecValu[2];					// hp�ָ��ٶȳ����ӳ�
	short	sSRecValu[2];					// sp�ָ��ٶȳ����ӳ�
	short	sMSpdValu[2];					// �ƶ��ٶȳ����ӳ�
	short	sColValu[2];					// ��Դ�ɼ��ٶȳ����ӳ�

	short	sPDef[2];						// ����ֿ�
	short	sLHandValu;						// �������ַ���
	short	sEndure[2];						// �;ö�
	short	sEnergy[2];						// ����
	short	sHole;							// ����
	char	szAttrEffect[defITEM_ATTREFFECT_NAME_LEN];	// ʹ��Ч��
	short	sDrap;							// drap
	short	sEffect[defITEM_BIND_EFFECT_NUM][2];		// ����������Ч�Լ���Ӧdummy
	short	sItemEffect[2];					// ���������Ч
	short	sAreaEffect[2];					// ���ڵ���ʱ����Ч
	short	sUseItemEffect[2];				// ʹ��ʱ����Ч
    _TCHAR  szDescriptor[defITEM_DESCRIPTOR_NAME_LEN];   // ����

	int		nCooldown;
	//short	sSeteffect[enumEQUIP_NUM][2];
	//_TCHAR  szDescriptor[5][defITEM_DESCRIPTOR_SET_LEN];

public:
    const char* GetIconFile();              // �õ�������icon�ļ�����
	static bool	IsVaildFusionID(CItemRecord* pItem); // add by ning.yan  2008-08-21
	
	bool		GetIsPile()							{ return nPileMax>1;	}

	// ����ɫ�����Ƿ����װ��
	bool		IsAllowEquip( unsigned int nChaID );

	// ����Ƿ�û����������
	bool		IsAllEquip()						{ return chBody[0]==-1;	}

	void		RefreshData();

	int			GetTypeValue( int nType );

	bool		IsFaceItem()						{ return sItemEffect[0]!=0;	}

	bool		IsSendUseItem()						{ return sUseItemEffect[0]!=0;	}

public:
	short		sEffNum;					// ����������Ч����

private:
	bool		_IsBody[5];

};

class CItemRecordSet : public CRawDataSet
{
public:

	static CItemRecordSet* I() { return _Instance; }

	CItemRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CItemRecordSet* _Instance; // �൱�ڵ���, ���Լ���ס

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CItemRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CItemRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CItemRecord);
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

inline CItemRecord* GetItemRecordInfo( int nTypeID )
{
	return (CItemRecord*)CItemRecordSet::I()->GetRawDataInfo(nTypeID);
}

inline CItemRecord* GetItemRecordInfo(const char *pszItemName)
{
    return (CItemRecord*)CItemRecordSet::I()->GetRawDataInfo(pszItemName);
}

inline bool CItemRecord::IsAllowEquip( unsigned int nChaID )	
{ 
	if( nChaID<5 ) return _IsBody[nChaID];

	return false;
}

#endif //ITEMRECORD_H