//=============================================================================
// FileName: EventRecord.cpp
// Creater: ZhangXuedong
// Date: 2004.11.24
// Comment:
//=============================================================================

#include "EventRecord.h"

CEventRecordSet * CEventRecordSet::_Instance = NULL;

BOOL CEventRecordSet::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
{   
	if(ParamList.size()==0) return FALSE;

	CEventRecord *pInfo = (CEventRecord*)pRawDataInfo;

	int m = 0, n = 0;
    string strList[80];
	string strLine;

	// ���
	pInfo->lID = pInfo->nID;
	// ����
	_tcsncpy(pInfo->szName, pInfo->szDataName, defEVENT_NAME_LEN);
	pInfo->szName[defEVENT_NAME_LEN - 1] = _TEXT('0');

    // �¼�����
    pInfo->sEventType = Str2Int(ParamList[m++]);
	// ��������
	pInfo->sArouseType = Str2Int(ParamList[m++]);
	// �����뾶
	pInfo->sArouseRadius = Str2Int(ParamList[m++]);
	// ������Ч
	pInfo->sEffect = Str2Int(ParamList[m++]);
	// ������Ч
	pInfo->sMusic = Str2Int(ParamList[m++]);
	// ����ʱ����Ч
	pInfo->sBornEffect = Str2Int(ParamList[m++]);	
	// �¼���ʾ�Ĺ��
	pInfo->sCursor = Str2Int(ParamList[m++]);		
	// ���ָ��ʲô�����Ч(0-����Ч,1-�ˣ�2-��)
	pInfo->chMainChaType = Str2Int(ParamList[m++]);		
	return TRUE;
}
