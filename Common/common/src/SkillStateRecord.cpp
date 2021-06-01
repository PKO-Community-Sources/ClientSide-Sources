//=============================================================================
// FileName: SkillStateRecord.cpp
// Creater: ZhangXuedong
// Date: 2005.02.04
// Comment: Skill State Record
//=============================================================================

#include "SkillStateRecord.h"

//---------------------------------------------------------------------------
// class CSkillStateRecord
//---------------------------------------------------------------------------
void CSkillStateRecord::RefreshPrivateData()
{
	_nActNum = 0;
	for (int i = 0; i < defSKILLSTATE_ACT_NUM; i++)
	{		
		if( nActBehave[i]!=0 )
		{
			_nActNum++;
		}
		else
		{
			break;
		}
	}
}

//---------------------------------------------------------------------------
// class CSkillStateRecordSet
//---------------------------------------------------------------------------
CSkillStateRecordSet * CSkillStateRecordSet::_Instance = NULL;

BOOL CSkillStateRecordSet::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
{   
	if(ParamList.size()==0) return FALSE;

	CSkillStateRecord *pInfo = (CSkillStateRecord*)pRawDataInfo;

	int m = 0, n = 0;
    string strList[80];
	string strLine;

	// ���
	pInfo->chID = (char)pInfo->nID;
	// ����
	strncpy(pInfo->szName, pInfo->szDataName, defSKILLSTATE_NAME_LEN);
	pInfo->szName[defSKILLSTATE_NAME_LEN - 1] = '\0';
	// ������ʱ��������λ���룩
	pInfo->sFrequency = Str2Int(ParamList[m++]);
	// ״̬������Ŀ���ϵ�����ʱ��ű�
	strncpy(pInfo->szOnTransfer, ParamList[m++].c_str(), defSKILLSTATE_SCRIPT_NAME);
	pInfo->szOnTransfer[defSKILLSTATE_SCRIPT_NAME - 1] = '\0';
	// ʹ��Ч���ű�
	strncpy(pInfo->szAddState, ParamList[m++].c_str(), defSKILLSTATE_SCRIPT_NAME);
	pInfo->szAddState[defSKILLSTATE_SCRIPT_NAME - 1] = '\0';
	// �Ƴ�Ч���ű�
	strncpy(pInfo->szSubState, ParamList[m++].c_str(), defSKILLSTATE_SCRIPT_NAME);
	pInfo->szSubState[defSKILLSTATE_SCRIPT_NAME - 1] = '\0';
	// �滻����
	pInfo->chAddType = Str2Int(ParamList[m++]);
	// �Ƿ�����ն�ȡ��
	pInfo->bCanCancel = Str2Int(ParamList[m++]) != 0 ? true : false;
	// �Ƿ��ܹ��ƶ�
	pInfo->bCanMove = Str2Int(ParamList[m++]) ? true : false;
	// �Ƿ��ܹ�ʹ��ħ������
	pInfo->bCanMSkill = Str2Int(ParamList[m++]) ? true : false;
	// �Ƿ��ܹ�ʹ����ͨ����
	pInfo->bCanGSkill = Str2Int(ParamList[m++]) ? true : false;
	// �Ƿ��ܹ�����
	pInfo->bCanTrade = Str2Int(ParamList[m++]) ? true : false;
	// �Ƿ��ܹ�ʹ����Ʒ
	pInfo->bCanItem = Str2Int(ParamList[m++]) ? true : false;
	// �Ƿ��޵�
	pInfo->bCanUnbeatable = Str2Int(ParamList[m++]) ? true : false;
	// �Ƿ��ܹ���ʹ����Ʒ
	pInfo->bCanItemmed = Str2Int(ParamList[m++]) ? true : false;
	// �Ƿ��ܹ���ʹ�ü���
	pInfo->bCanSkilled = Str2Int(ParamList[m++]) ? true : false;
	// �Ƿ�����
	pInfo->bNoHide = Str2Int(ParamList[m++]) ? true : false;
	// �Ƿ�ǿ������
	pInfo->bNoShow = Str2Int(ParamList[m++]) ? true : false;
	// �Ƿ�ɲ�������
	pInfo->bOptItem = Str2Int(ParamList[m++]) ? true : false;
	// �Ƿ�ɲ�������
	pInfo->bTalkToNPC = Str2Int(ParamList[m++]) ? true : false;
	// �����Ч�����
	pInfo->bFreeStateID = Str2Int(ParamList[m++]);

	// ��ĻЧ��
	pInfo->chScreen = Str2Int(ParamList[m++]);
	// ��������
	memset( pInfo->nActBehave, 0, sizeof(pInfo->nActBehave) );
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defSKILLSTATE_ACT_NUM ? defSKILLSTATE_ACT_NUM : n;	
	for (int i = 0; i < n; i++)
	{		
		pInfo->nActBehave[i] = Str2Int(strList[i]);
	}

	//�ͻ��˱��ֱ�ʶ
    pInfo->sChargeLink = Str2Int(ParamList[m++]);

    // ���������Ч
    pInfo->sAreaEffect = Str2Int(ParamList[m++]);
	// ����״̬��Ч,�Ƿ����ʾ���ĵ�
    pInfo->IsShowCenter = Str2Int(ParamList[m++]) ? true : false;
	// ��Ч����POSE
	pInfo->IsDizzy = Str2Int(ParamList[m++]) ? true : false;
	// ��Ч������Ч����
	pInfo->sEffect = Str2Int(ParamList[m++]);
	// ����Ч��ҵĲ���dummy��
	pInfo->sDummy1 = Str2Int(ParamList[m++]);
	// ��״̬���ܻ���Ч����
	pInfo->sBitEffect = Str2Int(ParamList[m++]);
	// ��Ч����dummy��
	pInfo->sDummy2 = Str2Int(ParamList[m++]);
	// �и�Ч���������ϵ�ICON����
	pInfo->sIcon = Str2Int(ParamList[m++]);
	
	strncpy(pInfo->szIcon, ParamList[m++].c_str(), defSKILLSTATE_NAME_LEN);
	pInfo->szIcon[defSKILLSTATE_NAME_LEN - 1] = '\0';

	strncpy(pInfo->szDesc, ParamList[m++].c_str(), 255 - 1);
	pInfo->szIcon[255 - 1] = '\0';
	
	pInfo->lColour = Str2Int(ParamList[m++]);


	
	
	return TRUE;
}

void CSkillStateRecordSet::_ProcessRawDataInfo(CRawDataInfo *pInfo)
{
	CSkillStateRecord *pState = (CSkillStateRecord*)pInfo;

    // ���¶�̬����
	pState->RefreshPrivateData();	
}