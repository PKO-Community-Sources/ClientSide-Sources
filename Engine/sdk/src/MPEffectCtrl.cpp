#include "StdAfx.h"
//#include <mindpower.h>
#include "GlobalInc.h"
#include "MPModelEff.h"

#include "mpeffectctrl.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
#ifndef SCRIPT_TABLE

CEffectScript	CEffectScript::m_cEffScript;
CEffectScript::CEffectScript()
{
}
CEffectScript::~CEffectScript()
{
}

bool	CEffectScript::InitScript()
{
	if(!m_cScript.OpenFileRead("effect\\script.txt"))
		return false;
	return true;
}

void	CEffectScript::ReleaseScript()
{
	m_cScript.CloseFile();
}

bool CEffectScript::LoadParamFromName( EFF_Param* pParam, const s_string& strName )
{
	if(!m_cScript.ReadSection( (char*)(LPCSTR)strName.c_str() ))
		return false;
	char pszModel[64];
	char pszN[64];

	if( !m_cScript.ReadLine( "ģ������",&pParam->m_iModelNum) )
		return false;
	pParam->m_vecModelName.clear();
	for (int n = 0; n < pParam->m_iModelNum; n++)
	{
		sprintf(pszN,"ģ��%d",n + 1);
		if( !m_cScript.ReadLine( pszN,pszModel,64) )
			return false;
		pParam->m_vecModelName.push_back(pszModel);
	}

	if( !m_cScript.ReadLine( "�ٶ�",&pParam->m_fvel) )
		return false;

	if( !m_cScript.ReadLine( "num",&pParam->m_iPartNum) )
		return false;


	pParam->m_vecParName.clear();
	pParam->m_vecDummyIdx.clear();
	int idummy;
	for ( n = 0; n < pParam->m_iPartNum; n++)
	{
		sprintf(pszN,"par%d",n + 1);
		if( !m_cScript.ReadLine( pszN,pszModel,64) )
			return false;
		//char* s  = pszModel;
		//s += lstrlen(pszModel) - 4;
		//*s = '\0';
		pParam->m_vecParName.push_back(pszModel);
		sprintf(pszN,"dummy%d",n + 1);
		if( !m_cScript.ReadLine( pszN,&idummy) )
			return false;
		pParam->m_vecDummyIdx.push_back(idummy);
	}
	if( !m_cScript.ReadLine( "�˶���ʽ",&pParam->m_iRenderIdx) )
		return false;
	if( !m_cScript.ReadLine( "Light",&pParam->m_iLightID) )
		return false;
	if( !m_cScript.ReadLine( "Result",pszModel,64) )
		return false;
	pParam->m_strResult = pszModel;

	return true;
}

bool CEffectScript::LoadParamFromName( Group_Param* pParam, const s_string& strName )
{
	if(!m_cScript.ReadSection( (char*)(LPCSTR)strName.c_str() ))
		return false;
	char pszN[64];

	if( !m_cScript.ReadLine( "Ԫ������",&pParam->nClass) )
		return false;
	pParam->vecParam.resize(pParam->nClass);
	pParam->vecNum.resize(pParam->nClass);

	int parma;
	for (int n = 0; n < pParam->nClass; n++)
	{
		sprintf(pszN,"Ԫ��%d",n + 1);
		if( !m_cScript.ReadLine( pszN,&parma) )
			return false;
		pParam->vecParam[n] = parma;
		if( !m_cScript.ReadLine( "����",&parma) )
			return false;
		pParam->vecNum[n] = parma;
	}

	if( !m_cScript.ReadLine( "�˶���ʽ",&pParam->nRenderIdx) )
		return false;

	return true;
}
#else
CEff_ParamSet* CEff_ParamSet::_Instance = NULL;
CGroup_ParamSet* CGroup_ParamSet::_Instance = NULL;
//CEff_ParamSet* CEff_ParamSet::I() { return _Instance; }
#endif
/************************************************************************/
/* class CMPEffectCtrl*/
/************************************************************************/
CMPEffectCtrl::CMPEffectCtrl(void)
{
}
CMPEffectCtrl::~CMPEffectCtrl(void)
{
}


void CMPEffectCtrl::Emission(WORD wID, D3DXVECTOR3* vBegin, D3DXVECTOR3* vEnd)
{
}

void CMPEffectCtrl::SetTarget(D3DXVECTOR3* vTarget)
{
}

void	CMPEffectCtrl::FrameMove(DWORD dwDailTime)
{
	_CPartCtrl.FrameMove(dwDailTime);
}

void	CMPEffectCtrl::Render()
{
	_CPartCtrl.Render();
}

void	CMPEffectCtrl::MoveTo(D3DXVECTOR3* vPos,MPMap* pmap)
{
	_CPartCtrl.MoveTo(vPos,pmap);
}

void	CMPEffectCtrl::BindingBone(D3DXMATRIX* pMatBone)
{
	_CPartCtrl.BindingBone(pMatBone);
}

void	CMPEffectCtrl::SetFontEffect(char* pszText,CMPFont* pFont)
{
	if( pszText )
	{
		_CPartCtrl.setFontEffect(pszText,pFont);
	}
	else
	{
		LG( "error", "msg CMPEffectCtrl::SetFontEffect��ʾ������Ч���������ָ�룡" );
	}
}
/************************************************************************/
/*����*/
/************************************************************************/

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CMagicCtrl::CMagicCtrl(void)
{
	_CpModel.clear();
	_iPartNum = 0;
	_iModelNum = 0;
	_bPlay = false;
	_bStop = true;
	_iLightID = 0;
	m_iIdx = 0;
	_iTargetID = -1;
	_iCurSNum = 1;
	_fCurTime = 0;
	MagicUpdate = NULL;
	_fHalfHei = 5;
	_fDailTime = 0;
	_fCurEmiTime = 0;
	_pPartResult = NULL;
}
CMagicCtrl::~CMagicCtrl(void)
{
	SAFE_DELETE(_pPartResult);

	for (int n = 0; n < _iModelNum; n++)
	{
		SAFE_DELETE(_CpModel[n]);
	}
	_CpModel.clear();
	_vecPartCtrl.clear();
	_vecDummy.clear();
	_iPartNum = 0;
	_iModelNum = 0;
}
#ifndef SCRIPT_TABLE

bool	CMagicCtrl::Create(int iID,CMPResManger	*pCResMagr)
{
	_fDailTime = 0;
	_fCurEmiTime = 0;
	m_pCResMagr = pCResMagr;

	if(m_iIdx == iID)
	{
		Reset();
		_iCurSNum = 1;
		_fCurTime = 0;

		return true;
	}
	EFF_Param pParam;
	char pszID[10];
	sprintf(pszID,"%d",iID);
	if(!CEffectScript::m_cEffScript.LoadParamFromName(&pParam,pszID))
		return false;

	for (int n = 0; n < _iModelNum; n++)
	{
		SAFE_DELETE(_CpModel[n]);
	}
	_CpModel.clear();

	_iModelNum = pParam.m_iModelNum;
	if(_iModelNum <= 0)
		return false;
	_CpModel.resize(_iModelNum);

	int tid;
	for ( n = 0; n < _iModelNum; n++)
	{
		_CpModel[n] = NULL;
		tid = pCResMagr->GetEffectID(pParam.m_vecModelName[n]);
		if(tid == -1)
			return false;

		_CpModel[n] = new CMPModelEff;
		_CpModel[n]->BindingEffect(pCResMagr->GetEffectByID(tid));
		_CpModel[n]->BindingRes(pCResMagr);

	}
	_iRnederIdx = pParam.m_iRenderIdx;

	_vecPartCtrl.clear();

	//_pDailTime = pCResMagr->GetDailTime();

	_iLightID = 0;

	_iLightID = pParam.m_iLightID;
	_strResult = pParam.m_strResult;
	SAFE_DELETE(_pPartResult);
	if(_strResult != "0")
	{
		int tid = pCResMagr->GetPartCtrlID(_strResult + ".par");
		if (tid < 0)
			return false;
//#ifndef USE_GAME
		CMPPartCtrl* tctrl = pCResMagr->GetPartCtrlByID(tid);
//#else
//		CMemoryBuf* tctrl = pCResMagr->GetPartCtrlByID(tid);
//#endif
		if(tctrl)
		{
			_pPartResult = new CMPPartCtrl;
//#ifndef USE_GAME
			_pPartResult->CopyPartCtrl(tctrl);
//#else
//			_pPartResult->LoadFromMemory(tctrl);
//#endif
			_pPartResult->BindingRes(pCResMagr);
		}
	}

	m_iIdx = iID;
	_fStartDist = 0;

	_fVel = pParam.m_fvel;
	_iPartNum = pParam.m_iPartNum;
	if( pParam.m_iPartNum <= 0)
		return true;

	_vecPartCtrl.resize(pParam.m_iPartNum);
	for ( n = 0; n < pParam.m_iPartNum; n++)
	{
		tid = pCResMagr->GetPartCtrlID(pParam.m_vecParName[n] + ".par");
		if(tid == -1)
			return false;
		_vecPartCtrl[n].Clear();
//#ifndef USE_GAME
		CMPPartCtrl* tctrl = pCResMagr->GetPartCtrlByID(tid);
		_vecPartCtrl[n].CopyPartCtrl(tctrl);
//#else
//		CMemoryBuf* tctrl = pCResMagr->GetPartCtrlByID(tid);
//		_vecPartCtrl[n].LoadFromMemory(tctrl);
//#endif
		_vecPartCtrl[n].BindingRes(pCResMagr);
		if(_iModelNum > 0)
			if(_CpModel[0]->m_vecEffect[0]->IsItem())
			{
				_vecPartCtrl[0].SetStripItem((MPSceneItem*)_CpModel[0]->m_vecEffect[0]->m_pCModel,true);
			}
		_vecDummy.push_back(pParam.m_vecDummyIdx[n]);
	}


	return true;
}
#else
bool	CMagicCtrl::Create(int iID,CMPResManger	*pCResMagr)
{
	_fDailTime = 0;
	_fCurEmiTime = 0;
	m_pCResMagr = pCResMagr;

	if(m_iIdx == iID)
	{
		Reset();
		_iCurSNum = 1;
		_fCurTime = 0;

		return true;
	}
	EFF_Param* pParam;
	pParam = GetEFFParam(iID);
	if(!pParam)
		return false;

	int n;
	for ( n = 0; n < _iModelNum; n++)
	{
		SAFE_DELETE(_CpModel[n]);
	}
	_CpModel.clear();

	_iModelNum = pParam->nModelNum;
	if(_iModelNum <= 0)
		return false;
	_CpModel.resize(_iModelNum);

	int tid;
	s_string strName;
	for ( n = 0; n < _iModelNum; n++)
	{
		_CpModel[n] = NULL;
		strName = pParam->strModel[n];
		tid = pCResMagr->GetEffectID(strName);
		if(tid == -1)
		{
			return false;
		}

		_CpModel[n] = new CMPModelEff;
		_CpModel[n]->BindingEffect(pCResMagr->GetEffectByID(tid));
		_CpModel[n]->BindingRes(pCResMagr);

	}
	_iRnederIdx = pParam->nRenderIdx;

	_vecPartCtrl.clear();

	_iLightID = 0;

	_iLightID = pParam->nLightID;
	_strResult =  pParam->strResult;
	SAFE_DELETE(_pPartResult);
	if(_strResult != "0")
	{
		int tid = pCResMagr->GetPartCtrlID(_strResult + ".par");
		if (tid < 0)
			return false;
		CMPPartCtrl* tctrl = pCResMagr->GetPartCtrlByID(tid);
		if(tctrl)
		{
			_pPartResult = new CMPPartCtrl;
			_pPartResult->CopyPartCtrl(tctrl);
			_pPartResult->BindingRes(pCResMagr);
		}
	}

	m_iIdx = iID;
	_fStartDist = 0;

	_fVel = (float)pParam->nVel;
	_iPartNum = pParam->nParNum;
	if( _iPartNum<= 0)
		return true;

	s_string str;
	_vecPartCtrl.resize(_iPartNum);
	for ( n = 0; n < _iPartNum; n++)
	{
		str = pParam->strPart[n];
		str +=  ".par";
		tid = pCResMagr->GetPartCtrlID(str);
		if(tid == -1)
		{
			return false;
		}
		_vecPartCtrl[n].Clear();
		CMPPartCtrl* tctrl = pCResMagr->GetPartCtrlByID(tid);
		_vecPartCtrl[n].CopyPartCtrl(tctrl);
		_vecPartCtrl[n].BindingRes(pCResMagr);
		if(_iModelNum > 0)
			if(_CpModel[0]->m_vecEffect[0]->IsItem())
			{
				_vecPartCtrl[0].SetStripItem((MPSceneItem*)_CpModel[0]->m_vecEffect[0]->m_pCModel,true);
			}
			_vecDummy.push_back(pParam->nDummy[n]);
	}


	return true;
}
#endif

int		CMagicCtrl::GetLightID()
{
	return _iLightID;
}

void	CMagicCtrl::Reset()
{
	for (int n = 0; n < _iModelNum; n++)
	{
		_CpModel[n]->Reset();
	}
	for (int n = 0; n < _iPartNum; n++)
	{
		_vecPartCtrl[n].Reset();
	}
	_iCurSNum = 1;
	_fCurTime = 0;
	_fCurDist = 0;
	_fStartDist = 0;

	_fDailTime = 0;
	_fCurEmiTime = 0;
	if(_pPartResult)
		_pPartResult->Reset();
}

void	CMagicCtrl::MoveTo(D3DXVECTOR3* vPos)
{
	_vPos = *vPos;
	for (int n = 0; n < _iModelNum; n++)
	{
		_CpModel[n]->MoveTo(_vPos.x,_vPos.y,_vPos.z);
	}
}

void	CMagicCtrl::CalculateEmission(D3DXVECTOR3* vStart,D3DXVECTOR3* vTarget)
{
	if(!vTarget||!vStart)
		return;
	_vOldPos = *vStart;
	_vOldTarget = *vTarget;
	//_vOldTarget.z = _vOldPos.z;
	_vPos = _vOldPos;

	_vDir = _vOldTarget - _vOldPos;
	_fDist = D3DXVec3LengthSq(&_vDir);

	D3DXVec3Normalize(&_vDir,&_vDir);
	//�͵�ͼ����ײ��
	//_vMapTarget = _vOldPos;
	//_vMapTarget.z = GetScene()->GetGridHeight(_vMapTarget.x,_vMapTarget.y);
	//���㻡���㷨������Ŀ���֮������ܵ��ĸ߶�
	_fHalfHei =  D3DXVec3Length(&(_vOldTarget - _vOldPos));
	_vArcOrg = _vOldPos+ _vDir * (_fHalfHei/2);
	_vArcOrg.z -= _fHalfHei/2;

	//������㵽Բ�ĵľ��루�뾶��
	_fHalfHei = D3DXVec3Length(&(_vOldPos - _vArcOrg));
	D3DXVec3Normalize(&_vArcAxis,&(_vOldPos - _vArcOrg));
	D3DXVec3Cross(&_vArcAxis,&_vArcAxis,&D3DXVECTOR3(0,0,-1));
	_fCurArc = 0;
}

void	CMagicCtrl::Emission(D3DXVECTOR3* vStart,D3DXVECTOR3* vTarget)
{
	if(!vTarget||!vStart)
		return;
	CalculateEmission(vStart,vTarget);
	MoveTo(&_vPos);
	_fCurDist = 0;

	_vTarget = *vTarget;
	if(_iRnederIdx == 5)
	{
		_vTarget.z = _vPos.z;
	}
		_vDir = _vTarget - _vPos;
		if(_fStartDist <= 0)
			_fStartDist = D3DXVec3Length(&_vDir);

		D3DXVec3Normalize(&_vDir,&_vDir);

		float fDist = D3DXVec3Length(&_vDir);
		if(_vDir.z == 0)
			_fDirXZ[0] = 0;
		else
		{
			_fDirXZ[0] = asinf(D3DXVec3Dot(&_vDir,
				&D3DXVECTOR3(0.0f, 0.0f, 1.0f))/ fDist);
		}

		//����X�᷽����ת�ĽǶ�
		if(_vDir.x == 0 && _vDir.y == 0)
			_fDirXZ[1] = 0;
		else
		{
			fDist = D3DXVec3Length(&D3DXVECTOR3(_vDir.x, _vDir.y, 0.0f));
			_fDirXZ[1] = acosf(D3DXVec3Dot(&D3DXVECTOR3(_vDir.x, _vDir.y, 0.0f),
				&D3DXVECTOR3(0.0f, 1.0f, 0.0f))/fDist);
			if( _vDir.x >= 0.0f )
				_fDirXZ[1]	= -_fDirXZ[1];
		}
		//����Z�᷽����ת�ĽǶ�
		for (int n = 0; n < _iModelNum; n++) 
		{
			_CpModel[n]->RotatingXZ(_fDirXZ[0], _fDirXZ[1]);
			if(_iRnederIdx == 5)
				_CpModel[n]->Play(true);
			else
				_CpModel[n]->Play(false);
		}
	for (int n = 0; n < _iPartNum; n++)
	{
		_vecPartCtrl[n].Play(0);
	}
	_bPlay = true;
	_bStop = false;
}

void	CMagicCtrl::ResetDir(D3DXVECTOR3* vTarget)
{
	_vTarget = *vTarget;
	_vDir = _vTarget - _vPos;
	D3DXVec3Normalize(&_vDir,&_vDir);

	float fDist = D3DXVec3Length(&_vDir);
	if(_vDir.z == 0)
		_fDirXZ[0] = 0;
	else
	{
		_fDirXZ[0] = asinf(D3DXVec3Dot(&_vDir,
			&D3DXVECTOR3(0.0f, 0.0f, 1.0f))/ fDist);
	}

	//����X�᷽����ת�ĽǶ�
	if(_vDir.x == 0 && _vDir.y == 0)
		_fDirXZ[1] = 0;
	else
	{
		fDist = D3DXVec3Length(&D3DXVECTOR3(_vDir.x, _vDir.y, 0.0f));
		_fDirXZ[1] = acosf(D3DXVec3Dot(&D3DXVECTOR3(_vDir.x, _vDir.y, 0.0f),
			&D3DXVECTOR3(0.0f, 1.0f, 0.0f))/fDist);
		if( _vDir.x >= 0.0f )
			_fDirXZ[1]	= -_fDirXZ[1];
	}
	//����Z�᷽����ת�ĽǶ�
	for (int n = 0; n < _iModelNum; n++) 
	{
		_CpModel[n]->RotatingXZ(_fDirXZ[0], _fDirXZ[1]);
		//_CpModel[n]->Play(0);
	}
}

void	CMagicCtrl::FrameMove(DWORD	dwDailTime)
{
	if(!_bPlay)
		return;

	if(	IsDail())
	{
		_fCurEmiTime += *m_pCResMagr->GetDailTime();
		return;
	}
	if(_bStop)
		return;
	else
		for (int n = 0; n < _iModelNum; n++)
		{
			_CpModel[n]->FrameMove(dwDailTime);
		}
}

void	CMagicCtrl::Render()
{
	if(!_bPlay)
		return;
	if(_bStop && _pPartResult)
	{
		_pPartResult->FrameMove(0);
		_pPartResult->Render();
		if(!_pPartResult->IsPlaying())
			SetInValid();
	}
	else
	{
		MoveTo(&_vPos);

		for (int n = 0; n < _iModelNum; n++)
		{
			_CpModel[n]->Render();
		}

		D3DXMATRIX mat;
		for ( n = 0; n < _iPartNum; n++)
		{
			if(_vecDummy[n] != -1)
			{
				_CpModel[0]->GetRunningDummyMatrix(&mat,_vecDummy[n]);
				_vecPartCtrl[n].MoveTo((D3DXVECTOR3*)&mat._41);

			}
			else
			{
				_vecPartCtrl[n].MoveTo(&_vPos);
			}
			_vecPartCtrl[n].FrameMove(0);
			_vecPartCtrl[n].Render();
		}
	}
}
