#include "StdAfx.h"
#include "chastate.h"
#include "SkillStateRecord.h"
#include "netprotocol.h"
#include "character.h"
#include "EffectObj.h"

//---------------------------------------------------------------------------
// class CChaStateMgr
//---------------------------------------------------------------------------
CSkillStateRecord* CChaStateMgr::_pLastActInfo = NULL;
int CChaStateMgr::_nShopLevel = 0;

CChaStateMgr::stChaState CChaStateMgr::_sInitState[SKILL_STATE_MAXID];
bool CChaStateMgr::_IsInit = false;

CChaStateMgr::CChaStateMgr(CCharacter* pCha)
: _pCha(pCha)
{
	if( !_IsInit )
	{
		_IsInit=true;

		memset( &_sInitState, 0, sizeof(_sInitState) );
		for( int i=1; i<SKILL_STATE_MAXID; i++ )
		{
			_sInitState[i].pInfo = GetCSkillStateRecordInfo( i );
		}
	}
	memcpy( _sChaState, _sInitState, sizeof(_sChaState) );
}

void CChaStateMgr::ChaDestroy()
{
	for( states::iterator it=_states.begin(); it!=_states.end(); it++ )
	{
		if( (*it)->pEffect ) 
		{
			LG( _pCha->getLogName(), g_oLangRec.GetString(29), (*it)->pEffect->getIdxID(), (*it)->pEffect );

			(*it)->pEffect->SetValid( FALSE );
			(*it)->pEffect = NULL;
		}
	}

	memcpy( _sChaState, _sInitState, sizeof(_sChaState) );
	_states.clear();
}

CBoolSet& CChaStateMgr::Synchro( stSkillState* pState, int nCount )
{
	static CBoolSet _ChaState;
	static stChaState* stTmp[SKILL_STATE_MAXID] = { NULL };
	static unsigned int nTmpCount = 0;

	// 用于确认是否已经修改
	static bool IsExist[SKILL_STATE_MAXID] = { NULL };
	memset( IsExist, 0, sizeof(IsExist) );

	static int nID = 0;
	nTmpCount = 0;
	for( states::iterator it=_states.begin(); it!=_states.end(); it++ )
	{
		(*it)->IsDel = true;

		nID = (*it)->pInfo->nID;
		if( !IsExist[nID] )
		{
			IsExist[nID] = true;

			stTmp[nTmpCount++] = *it;
		}
	}

	stChaState* pChaState = NULL;
    for( int i=0; i<nCount; i++ )
	{
		nID = pState[i].chID;
		if( nID<0 || nID>=SKILL_STATE_MAXID ) 
		{
			continue;
		}

		pChaState = &_sChaState[nID];
		if( pChaState->pInfo && pState[i].chLv>0 )
		{
			pChaState->IsDel = false;
			pChaState->chStateLv = pState[i].chLv;
			pChaState->lTimeRemaining = pState[i].lTimeRemaining;

			if( !IsExist[nID] )
			{
				IsExist[nID] = true;

				stTmp[nTmpCount++] = pChaState;
			}
		}
	}

	_states.clear();

	CCharacter::hits& _hits = _pCha->_hits;
	_hits.clear();

	_ChaState.AllTrue();
	CSkillStateRecord* pInfo = NULL;
	for ( unsigned int i=0; i<nTmpCount; i++ )
	{
		pChaState = stTmp[i];
		if( pChaState->IsDel )
		{
			LG( _pCha->getLogName(), g_oLangRec.GetString(30), pChaState->pInfo->nID, pChaState->pInfo->szName, pChaState->pInfo->sEffect );

			// 已有的删除
			if( pChaState->pEffect )
			{
				LG( _pCha->getLogName(), g_oLangRec.GetString(31), pChaState->pEffect->getIdxID(), pChaState->pEffect );

				pChaState->pEffect->SetValid( FALSE );
				pChaState->pEffect = NULL;
			}
			pChaState->chStateLv = 0;
		}
		else
		{
			// 增加
			_states.push_back( pChaState );
			LG( _pCha->getLogName(), g_oLangRec.GetString(32), pChaState->pInfo->nID, pChaState->pInfo->szName, pChaState->pInfo->sEffect );

			pInfo = pChaState->pInfo;
			if( pInfo->sBitEffect>0 )
			{
				_hits.push_back( CCharacter::stHit(pInfo->sBitEffect,pInfo->sDummy2) );
			}

			if( !pInfo->bCanMove ) _ChaState.SetFalse( enumChaStateMove );
			if( !pInfo->bCanGSkill ) _ChaState.SetFalse( enumChaStateAttack );
			if( !pInfo->bCanMSkill ) _ChaState.SetFalse( enumChaStateUseSkill );
			if( !pInfo->bCanTrade ) _ChaState.SetFalse( enumChaStateTrade );
			if( !pInfo->bCanItem ) _ChaState.SetFalse( enumChaStateUseItem );
			if( !pInfo->bNoHide ) _ChaState.SetFalse( enumChaStateNoHide );
			if( pInfo->IsDizzy ) _ChaState.SetFalse( enumChaStateNoDizzy );
			if( pInfo->GetActNum()>0 ) 
			{
				_pLastActInfo = pInfo;
				_ChaState.SetFalse( enumChaStateNoAni );
			}
			if( pInfo->nID==99 )
			{
				_nShopLevel = pChaState->chStateLv;
				_ChaState.SetFalse( enumChaStateNoShop );
			}

			if( pInfo->sEffect>0 && !pChaState->pEffect )
			{
				pChaState->pEffect = _pCha->SelfEffect( pInfo->sEffect, pInfo->sDummy1, true );
				LG( _pCha->getLogName(), g_oLangRec.GetString(33), pInfo->sEffect, pInfo->sDummy1, pChaState->pEffect );
			}
		}
	}
	return _ChaState;
}

