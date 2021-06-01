//=============================================================================
// FileName: ItemContent.h
// Creater: ZhangXuedong
// Date: 2005.10.19
// Comment: Item Content class
//=============================================================================

#ifndef ITEMCONTENT_H
#define ITEMCONTENT_H

#include "ItemAttr.h"

enum EItemDBParam
{
	enumITEMDBP_FORGE,			// ���߾���
	enumITEMDBP_INST_ID,		// ����ʵ����������(1.��ֻ���ݿ�ID, 2.....)

	enumITEMDBP_MAXNUM,			// ���������
};

#define defITEM_INSTANCE_ATTR_NUM			5

#pragma pack(push)
#pragma pack(2)
struct SItemGrid // ���߸�����
{
    SItemGrid(short sId = 0, short sINum = 0);

// ��Ա����
	void	SetInstAttrInvalid(void) {sInstAttr[0][0] = 0;}
	bool	IsInstAttrValid(void) {return sInstAttr[0][0] > 0 ? true : false;}

	// װ���ȼ���Ϣ�ӿ�
	char	GetItemLevel() 
	{ 
		char chLevel = char(lDBParam[1]&0xFFFF);
		return chLevel; 
	}

	void    SetItemLevel( char chLevel ) 
	{
		lDBParam[1] &= 0xFFFF0000;
		lDBParam[1] |= chLevel; 
	}

	void	AddItemLevel( char chLevel = 1 ) 
	{
		char chData = char(lDBParam[1]&0xFFFF);
		chData += chLevel;
		SetItemLevel( chData );
	}

	USHORT  GetFusionItemID() 
	{
		USHORT sID = USHORT(lDBParam[1]>>16);
		return sID;
	}

	void	SetFusionItemID( USHORT sID ) 
	{
		lDBParam[1] &= 0x0000FFFF;
		lDBParam[1] |= DWORD(sID)<<16;
	}

	char	GetForgeLv(void) {return chForgeLv;}
	void	SetForgeLv(char chFLv) {chForgeLv = chFLv; SetChange();}
	void	AddForgeLv(char chAddLv) {chForgeLv += chAddLv; if (chForgeLv < 0) chForgeLv = 0; SetChange();}
	long	GetDBParam(short sParamID) {return lDBParam[sParamID];}
	void	SetDBParam(short sParamID, long lParamVal);
	long	GetForgeParam(void) {return GetDBParam(enumITEMDBP_FORGE);}
	void	SetForgeParam(long lVal) {SetDBParam(enumITEMDBP_FORGE, lVal);}

	bool	operator == (SItemGrid& SItem);
	bool	HasInstAttr(long lAttrID);
	short	GetInstAttr(long lAttrID);
	bool	SetInstAttr(long lAttrID, short sAttr);
	bool	AddInstAttr(long lAttrID, short sAttr);

	bool	InitAttr(void);
	bool	CheckAttr(void);
	short	GetAttr(long lAttrID);
	short	SetAttr(long lAttrID, short sAttr);
	short	AddAttr(long lAttrID, short sAttr);

	void	CheckValid(void);
	bool	IsValid(void) {return bValid;}
	void	SetValid(bool bVld = true) {bValid = bVld; SetChange();}
	bool	IsChange(void) {return bChange;}
	void	SetChange(bool bChg = true) {bChange = bChg;}
	
	// ���Ƶ���ʵ������
	void    CopyInstAttr( SItemGrid& item );
	bool	FusionCheck( SItemGrid& item ); 

	bool	bIsLock;
	short	sNeedLv;
	DWORD	dwDBID;
	short	sID;		// ���߱��е�ID��0��ʾû�е��ߣ�
	short	sNum;		// ���߸���
	short	sEndure[2];	// �;öȣ���ǰֵ/���ֵ��
	short	sEnergy[2];	// ��������ǰֵ/���ֵ��
	char	chForgeLv;	// �����ȼ�
	long	lDBParam[enumITEMDBP_MAXNUM];	// ���ݿ����
	short	sInstAttr[defITEM_INSTANCE_ATTR_NUM][2];	// ʵ������
	CItemAttr	CAttr;	// ����

	bool	bValid;
	bool	bChange; // �����Ƿ�䶯

};
#pragma pack(pop)

inline SItemGrid::SItemGrid(short sId, short sINum)
{
	bIsLock = 0;
	sNeedLv = 0;
	dwDBID = 0;
	sNum = sINum;
	sID = sId;
	memset(sEndure, 0, sizeof(short) * 2);
	memset(sEnergy, 0, sizeof(short) * 2);
	chForgeLv = 0;
	memset(lDBParam, 0, sizeof(long) * enumITEMDBP_MAXNUM);
	memset(sInstAttr, 0, sizeof(short) * defITEM_INSTANCE_ATTR_NUM * 2);
	SetInstAttrInvalid();
	SetValid();
	SetChange(false);
}

inline void SItemGrid::SetDBParam(short sParamID, long lParamVal)
{
	if (sParamID >= 0 && sParamID < enumITEMDBP_MAXNUM) lDBParam[sParamID] = lParamVal;
	else memset(lDBParam, 0, sizeof(long) * enumITEMDBP_MAXNUM);
	SetChange();
}

inline bool SItemGrid::operator == (SItemGrid& SItem)
{
	if (sID == SItem.sID
		&& sNum == SItem.sNum
		&& sEndure[0] == SItem.sEndure[0]
		&& sEndure[1] == SItem.sEndure[1]
		&& sEnergy[0] == SItem.sEnergy[0]
		&& sEnergy[1] == SItem.sEnergy[1]
		&& chForgeLv == SItem.chForgeLv
		)
	{
		for (int i = 0; i < enumITEMDBP_MAXNUM; i++)
			if (lDBParam[i] != SItem.lDBParam[i])
				return false;
		for (int i = 0; i < defITEM_INSTANCE_ATTR_NUM; i++)
			if (sInstAttr[i][0] != SItem.sInstAttr[i][0] || sInstAttr[i][1] != SItem.sInstAttr[i][1])
				return false;
		return true;
	}
	else
		return false;
}

inline bool SItemGrid::HasInstAttr(long lAttrID)
{
	if (lAttrID == ITEMATTR_URE)
		return true;
	if (lAttrID == ITEMATTR_MAXURE)
		return true;
	if (lAttrID == ITEMATTR_ENERGY)
		return true;
	if (lAttrID == ITEMATTR_MAXENERGY)
		return true;
	if (lAttrID == ITEMATTR_FORGE)
		return true;

	for (int i = 0; i < defITEM_INSTANCE_ATTR_NUM; i++)
	{
		if (lAttrID == sInstAttr[i][0])
			return true;
	}

	return false;
}

inline short SItemGrid::GetInstAttr(long lAttrID)
{
	if (lAttrID == ITEMATTR_URE)
		return sEndure[0];
	if (lAttrID == ITEMATTR_MAXURE)
		return sEndure[1];
	if (lAttrID == ITEMATTR_ENERGY)
		return sEnergy[0];
	if (lAttrID == ITEMATTR_MAXENERGY)
		return sEnergy[1];
	if (lAttrID == ITEMATTR_FORGE)
		return chForgeLv;

	for (int i = 0; i < defITEM_INSTANCE_ATTR_NUM; i++)
	{
		if (lAttrID == sInstAttr[i][0])
			return sInstAttr[i][1];
	}

	return 0;
}

inline bool SItemGrid::SetInstAttr(long lAttrID, short sAttr)
{
	if (lAttrID == ITEMATTR_MAXURE)
	{
		sEndure[1] = sAttr;
		if (sEndure[0] > sEndure[1])
			sEndure[0] = sEndure[1];
		goto ItemAttrSetSuc;
	}
	if (lAttrID == ITEMATTR_URE)
	{
		sEndure[0] = sAttr;
		if (sEndure[0] > sEndure[1])
			sEndure[0] = sEndure[1];
		goto ItemAttrSetSuc;
	}
	if (lAttrID == ITEMATTR_MAXENERGY)
	{
		sEnergy[1] = sAttr;
		if (sEnergy[0] > sEnergy[1])
			sEnergy[0] = sEnergy[1];
		goto ItemAttrSetSuc;
	}
	if (lAttrID == ITEMATTR_ENERGY)
	{
		sEnergy[0] = sAttr;
		if (sEnergy[0] > sEnergy[1])
			sEnergy[0] = sEnergy[1];
		goto ItemAttrSetSuc;
	}
	if (lAttrID == ITEMATTR_FORGE)
	{
		chForgeLv = (char)sAttr;
		goto ItemAttrSetSuc;
	}

	for (int i = 0; i < defITEM_INSTANCE_ATTR_NUM; i++)
	{
		if (lAttrID == sInstAttr[i][0])
		{
			sInstAttr[i][1] = sAttr;
			goto ItemAttrSetSuc;
		}
	}

	return false;

ItemAttrSetSuc:
	SetChange();
	return true;
}

inline bool SItemGrid::AddInstAttr(long lAttrID, short sAttr)
{
	if (lAttrID == ITEMATTR_MAXURE)
	{
		sEndure[1] += sAttr;
		if (sEndure[0] > sEndure[1])
			sEndure[0] = sEndure[1];
		goto ItemAttrAddSuc;
	}
	if (lAttrID == ITEMATTR_URE)
	{
		sEndure[0] += sAttr;
		if (sEndure[0] > sEndure[1])
			sEndure[0] = sEndure[1];
		goto ItemAttrAddSuc;
	}
	if (lAttrID == ITEMATTR_MAXENERGY)
	{
		sEnergy[1] += sAttr;
		if (sEnergy[0] > sEnergy[1])
			sEnergy[0] = sEnergy[1];
		goto ItemAttrAddSuc;
	}
	if (lAttrID == ITEMATTR_ENERGY)
	{
		sEnergy[0] += sAttr;
		if (sEnergy[0] > sEnergy[1])
			sEnergy[0] = sEnergy[1];
		goto ItemAttrAddSuc;
	}
	if (lAttrID == ITEMATTR_FORGE)
	{
		chForgeLv += (char)sAttr;
		goto ItemAttrAddSuc;
	}

	for (int i = 0; i < defITEM_INSTANCE_ATTR_NUM; i++)
	{
		if (lAttrID == sInstAttr[i][0])
		{
			sInstAttr[i][1] += sAttr;
			goto ItemAttrAddSuc;
		}
	}

	return false;

ItemAttrAddSuc:
	SetChange();
	return true;
}

//inline bool SItemGrid::InitAttr(void)
//{
//	if( sID >= enumItemFusionStart && sID < enumItemFusionEnd && lDBParam[1] )
//	{
//		sID = (USHORT)lDBParam[1];
//	}
//
//	if (!CAttr.Init(sID))
//		return false;
//	SetAttr(ITEMATTR_URE, sEndure[0]);
//	SetAttr(ITEMATTR_MAXURE, sEndure[1]);
//	SetAttr(ITEMATTR_ENERGY, sEnergy[0]);
//	SetAttr(ITEMATTR_MAXENERGY, sEnergy[1]);
//	SetAttr(ITEMATTR_FORGE, chForgeLv);
//	for (char i = 0; i < defITEM_INSTANCE_ATTR_NUM; i++)
//	{
//		if (sInstAttr[i][0] == 0)
//			break;
//		SetAttr(sInstAttr[i][0], sInstAttr[i][1]);
//	}
//
//	return true;
//}

inline bool SItemGrid::CheckAttr(void)
{
	if (!CAttr.HasInit())
		return InitAttr();
	return true;
}

inline short SItemGrid::GetAttr(long lAttrID)
{
	if (!CheckAttr())
		return 0;
	return CAttr.GetAttr(short(lAttrID));
}

inline short SItemGrid::SetAttr(long lAttrID, short sAttr)
{
	if (!CheckAttr())
		return 0;
	return CAttr.SetAttr(short(lAttrID), sAttr);
}

inline short SItemGrid::AddAttr(long lAttrID, short sAttr)
{
	if (!CheckAttr())
		return 0;
	return CAttr.AddAttr(short(lAttrID), sAttr);
}

inline void SItemGrid::CopyInstAttr( SItemGrid& item )
{
	memcpy( sEndure, item.sEndure, 2*sizeof(short) );
	memcpy( sEnergy, item.sEnergy, 2*sizeof(short) );
	chForgeLv = item.chForgeLv;
	memcpy( sInstAttr, item.sInstAttr, sizeof(short)*2*defITEM_INSTANCE_ATTR_NUM );

	InitAttr();
}

#endif // ITEMCONTENT_H