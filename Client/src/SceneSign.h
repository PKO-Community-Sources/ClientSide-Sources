#pragma once

class CGameScene;
class CSceneItem;
class CCharacter;
class CShadeEff;

// �������û���־
class CSceneSign
{
public:
	CSceneSign();
	~CSceneSign();

	bool	Init( CGameScene* pScene );
	void	Show( int x, int y, int h=0 );
	void	Show( CCharacter* pCha );
	void	Hide();
	void	FrameMove( DWORD dwTime );
	void	ChaDisappear( CCharacter* pCha );

	bool	IsShowMove()				{ return _eStyle==enumMove;	}

private:
	void	Reset( CSceneItem* pItem );

private:
	enum eStyle
	{
		enumNone,
		enumAttack,
		enumMove,
	};
	eStyle			_eStyle;

	CGameScene*		_pScene;

	CSceneItem*		_pAttack;		// ����ʱ��ʾ�Ĺ⻷
	CShadeEff*		_pAttackShade;
	CSceneItem*		_pMove;         // �ƶ���ʰȡʱ��ʾ�Ĺ⻷
	CCharacter*		_pCha;			// �������Ľ�ɫ

	int	_nX, nY, _nH;	

};

inline void	CSceneSign::ChaDisappear( CCharacter* pCha )	
{ 
	if( _eStyle==enumAttack && pCha==_pCha ) 
	{
		Hide();
	}
}
