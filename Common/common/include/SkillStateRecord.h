//=============================================================================
// FileName: SkillStateRecord.h
// Creater: ZhangXuedong
// Date: 2005.02.04
// Comment: Skill State Record
//=============================================================================

#ifndef SKILLSTATERECORD_H
#define SKILLSTATERECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"

const char cchSkillStateRecordKeyValue = (char)0xff;

#define defSKILLSTATE_NAME_LEN		17
#define defSKILLSTATE_SCRIPT_NAME	32
#define defSKILLSTATE_ACT_NUM		3

class CSkillStateRecord : public CRawDataInfo
{
public:
	// CSkillStateRecord();

	char	chID;									// ���
	char	szName[defSKILLSTATE_NAME_LEN];			// ����
	short	sFrequency;								// ������ʱ��������λ���룩
	char	szOnTransfer[defSKILLSTATE_SCRIPT_NAME];// ״̬������Ŀ���ϵ�����ʱ��ű�
	char	szAddState[defSKILLSTATE_SCRIPT_NAME];	// ʹ��Ч���ű�
	char	szSubState[defSKILLSTATE_SCRIPT_NAME];	// �Ƴ�Ч���ű�
	char	chAddType;								// �滻����
	bool	bCanCancel;								// �Ƿ�����ն�ȡ��
	bool	bCanMove;								// �Ƿ��ܹ��ƶ�
	bool	bCanMSkill;								// �Ƿ��ܹ�ʹ��ħ������
	bool	bCanGSkill;								// �Ƿ��ܹ�ʹ����ͨ����
	bool	bCanTrade;								// �Ƿ��ܹ�����
	bool	bCanItem;								// �Ƿ��ܹ�ʹ����Ʒ
	bool	bCanUnbeatable;							// �Ƿ��޵�
	bool	bCanItemmed;							// �Ƿ��ܹ���ʹ����Ʒ
	bool	bCanSkilled;							// �Ƿ��ܹ���ʹ�ü���
	bool	bNoHide;								// �Ƿ�����
	bool	bNoShow;								// �Ƿ�����
	bool	bOptItem;								// �Ƿ�ɲ�������
	bool	bTalkToNPC;								// �Ƿ�ɺ�NPC�Ի�
	char	bFreeStateID;							// �����Ч�����

	// �ͻ��˱���
	char	chScreen;								// ��ĻЧ��
	char    nActBehave[defSKILLSTATE_ACT_NUM];		// ��������
	short	sChargeLink;							// �ͻ��˱��ֱ�ʶ,���ڱ��ֳ�籴��
    short   sAreaEffect;                            // ���������Ч
	bool	IsShowCenter;							// ����״̬��Ч,�Ƿ����ʾ���ĵ�
	bool	IsDizzy;								// �Ƿ���ʾѣ��Ч��
	short	sEffect;								// ��Ч������Ч����
	short	sDummy1;								// ����Ч��ҵĲ���dummy��
	short	sBitEffect;								// ��״̬���ܻ���Ч����
	short	sDummy2;								// ��Ч����dummy��
	short	sIcon;									// �и�Ч���������ϵ�ICON����
	char	szIcon[defSKILLSTATE_NAME_LEN];
	char	szDesc[255];
	int		lColour;
public:
	void	RefreshPrivateData();

	int		GetActNum()			{ return _nActNum;		}

public:
	int		_nActNum;								// �������ָ���

};

class CSkillStateRecordSet : public CRawDataSet
{
public:

	static CSkillStateRecordSet* I() { return _Instance; }

	CSkillStateRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CSkillStateRecordSet* _Instance; // �൱�ڵ���, ���Լ���ס

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CSkillStateRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CSkillStateRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CSkillStateRecord);
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

inline CSkillStateRecord* GetCSkillStateRecordInfo( int nTypeID )
{
	return (CSkillStateRecord*)CSkillStateRecordSet::I()->GetRawDataInfo(nTypeID);
}

#endif // SKILLSTATERECORD_H