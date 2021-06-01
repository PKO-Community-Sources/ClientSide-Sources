//=============================================================================
// FileName: EventRecord.h
// Creater: ZhangXuedong
// Date: 2004.11.24
// Comment:
//=============================================================================

#ifndef EVENTRECORD_H
#define EVENTRECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"
#include "point.h"

// �¼���������
enum EEventTouchType
{
	enumEVENTT_RANGE	= 1,		// ���뾶����
};

enum EEventExecType
{
	enumEVENTE_SMAP_ENTRY	= 1,	// ��ͼ��̬�л���
	enumEVENTE_DMAP_ENTRY	= 2,	// ��ͼ��̬�л���
};

enum EEventType
{
	enumEVENT_TYPE_ACTION = 1,	// 
	enumEVENT_TYPE_ENTITY,
};

// ��������,�����ͻ�����������
enum EEventArouseType
{
	enumEVENT_AROUSE_DISTANCE,		// ���뾶����
	enumEVENT_AROUSE_CLICK,			// �������
};

const char cchEventRecordKeyValue = (char)0xff;

#define defEVENT_NAME_LEN	18

class CEventRecord : public CRawDataInfo
{
public:
	//CEventRecord();

	long	lID;						// ���
	char	szName[defEVENT_NAME_LEN];	// ����
    short   sEventType;                 // �¼�����,�������ַ���Э�鷽ʽ,�ο�EEventType
	short	sArouseType;				// ��������
	short	sArouseRadius;				// �����뾶
	short	sEffect;					// ������Ч
	short	sMusic;						// ������Ч
	short   sBornEffect;				// ����ʱ����Ч
	short	sCursor;					// �¼���ʾ�Ĺ��
	char	chMainChaType;				// �¼����ʱʲô�����Ч(0-����Ч,1-�ˣ�2-��)

	bool	IsValid( int MainChaType )	{ return chMainChaType==0 || MainChaType==chMainChaType;	}
};

class CEventRecordSet : public CRawDataSet
{
public:

	static CEventRecordSet* I() { return _Instance; }

	CEventRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CEventRecordSet* _Instance; // �൱�ڵ���, ���Լ���ס

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CEventRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CEventRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CEventRecord);
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

inline CEventRecord* GetEventRecordInfo( int nTypeID )
{
	return (CEventRecord*)CEventRecordSet::I()->GetRawDataInfo(nTypeID);
}

#endif // EVENTRECORD_H