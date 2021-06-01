//======================================================================================================================
// FileName: HairRecord.h
// Creater: Jerry li
// Date: 2005.08.29
// Comment: CHairRecordSet class
//======================================================================================================================

#ifndef	HAIRRECORD_H
#define	HAIRRECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"

#define defHAIR_MAX_ITEM		4
#define defHAIR_MAX_FAIL_ITEM	3

class CHairRecord : public CRawDataInfo
{
public:
	CHairRecord();
	char	szColor[10];

	DWORD	dwNeedItem[defHAIR_MAX_ITEM][2];		// ��Ҫ�ĵ���ID,�Լ�����Ҫ������
	DWORD	dwMoney;								// ��Ҫ�Ľ�Ǯ
	DWORD	dwItemID;								// ��Ӧ��(���Ͳ�λ)����ID
	DWORD	dwFailItemID[defHAIR_MAX_FAIL_ITEM];	// ʧ��ʱ�ķ���
	bool	IsChaUse[4];							// 4�������Ƿ����
	
	int		GetFailItemNum()		{ return _nFailNum;		}
	void	RefreshPrivateData();

private:
	int		_nFailNum;
	
};

class CHairRecordSet : public CRawDataSet
{
public:
	static CHairRecordSet* I() { return _Instance; }

	CHairRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:
	static CHairRecordSet* _Instance; // �൱�ڵ���, ���Լ���ס

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CHairRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CHairRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CHairRecord);
	}

	virtual void*	_CreateNewRawData(CRawDataInfo *pInfo)
	{
		return NULL;
	}

	virtual void  _DeleteRawData(CRawDataInfo *pInfo)
	{
		SAFE_DELETE(pInfo->pData);
	}
    virtual void  _AfterLoad();

	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList);
	virtual void _ProcessRawDataInfo(CRawDataInfo *pInfo);

};

inline CHairRecord* GetHairRecordInfo( int nTypeID )
{
	return (CHairRecord*)CHairRecordSet::I()->GetRawDataInfo(nTypeID);
}

#endif //HAIRRECORD_H