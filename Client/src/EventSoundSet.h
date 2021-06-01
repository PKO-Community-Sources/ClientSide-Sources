#pragma once
#include "TableData.h"

class CEventSoundInfo : public CRawDataInfo
{
public:
	CEventSoundInfo() : nSoundID(-1)
	{
	}

	int nSoundID;
};

class CEventSoundSet : public CRawDataSet
{
public:
	static CEventSoundSet* I() { return _Instance; }

	CEventSoundSet(int nIDStart, int nIDCnt, int nCol = 8)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:
	static CEventSoundSet* _Instance; // �൱�ڵ���, ���Լ���ס

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CEventSoundInfo[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CEventSoundInfo*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CEventSoundInfo);
	}

	virtual void*	_CreateNewRawData(CRawDataInfo *pInfo)
	{
		return NULL;
	}

	virtual void  _DeleteRawData(CRawDataInfo *pInfo)
	{
		SAFE_DELETE(pInfo->pData);
	}

	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
	{   
		CEventSoundInfo *pInfo = (CEventSoundInfo*)pRawDataInfo;
		pInfo->nSoundID = Str2Int(ParamList[0]);
		return TRUE;
	}
};

inline CEventSoundInfo* GetEventSoundInfo(int nTypeID)
{
	return (CEventSoundInfo*)CEventSoundSet::I()->GetRawDataInfo(nTypeID);
}
