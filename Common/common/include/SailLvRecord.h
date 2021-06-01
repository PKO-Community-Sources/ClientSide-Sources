//=============================================================================
// FileName: SailLvRecord.h
// Creater: ZhangXuedong
// Date: 2005.05.28
// Comment: CSailLvRecord class
//=============================================================================

#ifndef SAILLVRECORD_H
#define SAILLVRECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"

const char cchSailLvRecordKeyValue = (char)0xff;

class CSailLvRecord : public CRawDataInfo
{
public:
	//CSailLvRecord();

	long	lID;			// ���
	short	sLevel;			// �ȼ�
	unsigned long	ulExp;	// ����ֵ
};

class CSailLvRecordSet : public CRawDataSet
{
public:

	static CSailLvRecordSet* I() { return _Instance; }

	CSailLvRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CSailLvRecordSet* _Instance; // �൱�ڵ���, ���Լ���ס

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CSailLvRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CSailLvRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CSailLvRecord);
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
};

inline CSailLvRecord* GetSailLvRecordInfo( int nTypeID )
{
	return (CSailLvRecord*)CSailLvRecordSet::I()->GetRawDataInfo(nTypeID);
}

#endif // SAILLVRECORD_H