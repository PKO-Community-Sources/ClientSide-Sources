//----------------------------------------------------------------------
// ����:��Ϸ�¼�
// ����:lh 2004-05-26
// ��;:������Ӧ�¼�
// ʵ��:�¼����������ڽ�ɫ����ߴ���
//----------------------------------------------------------------------
#pragma once

class CSkillRecord;
class CCharacter;

// С����,�����жϹ�������ļ����Ƿ��ʹ��
class CIsSkillUse
{
public:
	CIsSkillUse();

	// ���жϼ����Ƿ����
	bool		IsValid( CSkillRecord* pSkill, CCharacter* pSelf );

	// �����ۺ��ж��Ƿ����
	bool		IsUse( CSkillRecord* pSkill, CCharacter* pSelf, CCharacter* pTarget );

	// �жϼ����Ƿ�ɶ�ĳ���ʹ��
	bool		IsAttack( CSkillRecord* pSkill, CCharacter* pSelf, CCharacter* pTarget );		

	// �������ʹ��,�õ�����ԭ��
	const char*	GetError();	

private:
	enum eError
	{
		enumNone,

		enumSelf,
		enumFish,
		enumDieBoat,
		enumOnlyTeam,
		enumTree,
		enumMine,
		enumDie,
		enumRepair,

		enumAttackMain,
		enumAttackTeam,
		enumHelpMons,
		enumAttackDie,
		enumTargetError,
		enumAttackPlayer,

		enumInValid,
		enumNotEnergy,
		enumNotAttack,
		enumNotUse,
		enumNotMP,
		enumNotTarget,

	};
	eError			_eError;
	CSkillRecord*	_pSkill;

};

extern CIsSkillUse	g_SkillUse;
