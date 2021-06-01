#pragma once

#include "HMSynchroObj.h"

class CStateSynchro;
class CSkillRecord;
class CActor;
class CCharater;
class CAttackEffect;
class CCharacter;
class CAttackRepSynchro;

const int ERROR_FIGHT_ID = INT_MAX;

class CServerHarm
{
    typedef list<CAttackEffect*> synchros;
    friend CActor;
public:
    CServerHarm( CActor* pActor );
    ~CServerHarm();

	void	InitMemory();

    void    Exec();             
    void    Gouge( int nGouge );                // ��һ�����ָ���Ϊ����ִ��

    bool    AddHarm( CAttackEffect* s, CSkillRecord *pSkill );        // �������紫�����˺���,���˺�ʱִ��
    bool    AddRep( CAttackRepSynchro* s );         // ���ְ�,�ڿ�ʼִ��ʱִ��
    bool    IsAllowStateOver();                 // �Ƿ�����״̬����

    void    ReadyExec()             { _nReadyExec++;            }
	void    SetIsOuter( bool v );

	void	ExecAll(int nCount);
	void	ExecAll();

	void	SetFightID( int v )		{ _nFightID = v;			}
	void	SetSkill( CSkillRecord* p )	{ _pSkill = p;			}
	
	int		GetCount()				{ return (int)_harm.size();	}

	void	DelHarm( CAttackEffect* p )	{ _harm.remove( p );	}

	void 	HitRepresent( CSkillRecord* pSkill, int nAngle );

public:
    CSkillRecord*   GetSkill()      { return _pSkill;           }

    int     GetFightID()            { return _nFightID;         }
	bool	GetIsExecEnd()		    { return _nFightID==ERROR_FIGHT_ID; }

private:

private:
    int         _nFightID;
    CActor*     _pActor;            // ����Actor
	CCharacter* _pCha;

    synchros    _harm;              // �˺���,�ڹؼ�֡����
    synchros    _rep;               // ���ְ�,�ڿ�ʼʱ����

    CSkillRecord*   _pSkill;

    int         _nReadyExec;        // �ͻ�������׼��ִ�еĴ���

	bool		_IsOuter;			// �Ƿ����ⲿ����

};

// ��������
