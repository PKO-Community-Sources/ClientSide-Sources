//----------------------------------------------------------------------
// 名称:管理玩家技能状态
// 作者:lh 2005-05-31
//----------------------------------------------------------------------
#pragma once

#include "boolset.h"
#include "skillstatetype.h"

struct stSkillState;
class CCharacter;
class CEffectObj;
class CSkillStateRecord;

class CChaStateMgr
{
public:
	CChaStateMgr(CCharacter* pCha);

	void		ChaDestroy();									// 角色无效时调用
	void		ChaDied()	{ ChaDestroy();	}					// 角色死亡时调用

	CBoolSet&	Synchro( stSkillState* pState, int nCount );	// 同步状态时调用

	int					GetSkillStateNum()					{ return (int)_states.size();	}
	CSkillStateRecord*	GetSkillState( unsigned int nID )	{ return _states[nID]->pInfo;	}

	bool		HasSkillState( unsigned int nID );
	int	GetStateLv( unsigned int nID );

	static CSkillStateRecord* GetLastActInfo()				{ return _pLastActInfo;			}
	static int				  GetLastShopLevel()			{ return _nShopLevel;			}

	struct stChaState
	{
		bool				IsDel;

		unsigned char		chStateLv;
		CSkillStateRecord*	pInfo;
		CEffectObj*			pEffect;
		unsigned long lTimeRemaining;
	};
	stChaState GetStateData( unsigned int nID );
private:
	CCharacter*		_pCha;

	

	stChaState		_sChaState[SKILL_STATE_MAXID];
	typedef vector<stChaState*>	states;
	states			_states;

	static	bool		_IsInit;
	static stChaState	_sInitState[SKILL_STATE_MAXID];

	static CSkillStateRecord*	_pLastActInfo;		// 用于同步时返回最后一个有动作表现的数据
	static int					_nShopLevel;		// 用于时返回摆摊状态等级

		

};


inline bool CChaStateMgr::HasSkillState( unsigned int nID )		
{
	return nID<SKILL_STATE_MAXID && _sChaState[nID].chStateLv;
}

inline int CChaStateMgr::GetStateLv( unsigned int nID )		
{
	return HasSkillState( nID )	?_sChaState[nID].chStateLv:0;
}

inline CChaStateMgr::stChaState CChaStateMgr::GetStateData( unsigned int nID )		
{
	return _sChaState[nID];
}
