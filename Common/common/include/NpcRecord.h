// NpcRecord.h Created by knight-gongjian 2004.11.24.
//---------------------------------------------------------
#pragma once

#ifndef _NPCRECORD_H_
#define _NPCRECORD_H_

#include <tchar.h>
#include "util.h"
#include "TableData.h"

//---------------------------------------------------------

#define NPC_MAXSIZE_NAME			128 // npc���Ƴ���
#define NPC_MAXSIZE_MSGPROC			16	// npc��Ϣ���������Ƴ���

class CNpcRecord : public CRawDataInfo
{
public:	
	char szName[NPC_MAXSIZE_NAME];		// ��ͼ����ʾnpc����
	USHORT sNpcType;					// npc����
	USHORT sCharID;						// ��ɫ�б�������
	BYTE byShowType;					// �ͻ�����ʾ��ɫ����
	DWORD dwxPos0, dwyPos0;				// npcλ����Ϣ
	DWORD dwxPos1, dwyPos1;
	USHORT sDir;
	USHORT sParam1, sParam2;
	char szNpc[NPC_MAXSIZE_NAME];		// npc�ļ���
	char szMsgProc[NPC_MAXSIZE_MSGPROC];// npc��Ϣ������
	char szMisProc[NPC_MAXSIZE_MSGPROC];// npc����״̬������
};

class CNpcRecordSet : public CRawDataSet
{
public:
	CNpcRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
	: CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Init();
	}

protected:

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CNpcRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CNpcRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CNpcRecord);
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
//---------------------------------------------------------

#endif // _NPCRECORD_H_