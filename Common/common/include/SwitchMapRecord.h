//=============================================================================
// FileName: SwitchMapRecord.h
// Creater: ZhangXuedong
// Date: 2004.11.23
// Comment:
//=============================================================================

#ifndef SWITCHMAPRECORD_H
#define SWITCHMAPRECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"
#include "point.h"

const char cchSwitchMapRecordKeyValue = (char)0xff;

#define defMAP_NAME_LEN		16

class CSwitchMapRecord : public CRawDataInfo
{
public:
	//CSwitchMapRecord();

	long	lID;							// ���
	long	lEntityID;						// ���󶨵�ʵ��ID
	long	lEventID;						// �¼����
	Point	SEntityPos;						// ʵ���λ�ã����ף�
	short	sAngle;							// ʵ��ķ���
	_TCHAR	szTarMapName[defMAP_NAME_LEN];	// Ŀ���ͼ��
	Point	STarPos;						// Ŀ���ͼ��λ�ã����ף�
};

class CSwitchMapRecordSet : public CRawDataSet
{
public:

	static CSwitchMapRecordSet* I() { return _Instance; }

	CSwitchMapRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CSwitchMapRecordSet* _Instance; // �൱�ڵ���, ���Լ���ס

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CSwitchMapRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CSwitchMapRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CSwitchMapRecord);
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

inline CSwitchMapRecord* GetSwitchMapRecordInfo( int nTypeID )
{
	return (CSwitchMapRecord*)CSwitchMapRecordSet::I()->GetRawDataInfo(nTypeID);
}

#endif