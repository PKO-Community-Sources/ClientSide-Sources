#pragma once
#include "STStateObj.h"
#include "Actor.h"

class CCharacter;
class CArcTrack;
class CMonsterItem;
class CReadyDieState : public CActionState		// �����
{
public:
	CReadyDieState(CActor* p);
	~CReadyDieState();

    virtual const char* GetExplain()        { return "CReadyDieState";      }

	virtual void	ActionEnd( DWORD pose_id );
	virtual void	FrameMove();
    virtual void    ActionFrame( DWORD pose_id, int key_frame );

	void			SetState( eActorState s )		{ _state = s;			}
    void            SetAttack( CCharacter* pCha )   { _pAttack = pCha;      }

protected:
	virtual bool	_Start();
    virtual bool    _IsAllowCancel()		        { return false;			}							
    virtual void	_End();

private:
    void    _Died();

private:
    enum eDieState
    {
        enumInit,
        enumDie,
        enumFallDown,
    };

    eDieState       _eDieState;
	eActorState		_state;	
	bool			_IsActionEnd;			// ��ǰ�����Ѿ�������ϣ�����һ���Զ���
    CCharacter*     _pCha;
    CCharacter*     _pAttack;               // ������
    bool            _isFlyOff;

    int             _nFallDownTime;    
    bool            _IsAlreadyEffect;       // �Ѿ�������Ч
    int             _nDelayTime;

    CArcTrack*		_pArcTrack;

};

