#pragma once

class CGameScene;
class CCharacter;

class CFindPath
{
public:
    CFindPath(long step,int range);

    void	RenderPath(MPTerrain* pTerrain);
    S_BVECTOR<D3DXVECTOR3>&		GetResultPath() {return _vecPathPoint;}

    int     GetLength()         { return _nLength;      }
    bool    Find( CGameScene* pScene, CCharacter* pCha, int nSelfX, int nSelfY, int nTargetX, int nTargetY, bool &IsWalkLine );
	bool    GetPathFindingState(){return m_bFindPath;}
	bool    LongPathFinding(){return m_bLongPathFinding;}
	void    SetLongPathFinding(long step, int range);
	void    SetShortPathFinding(long step, int range);

protected:	
    void		SetTargetPos(int nCurX,int nCurY,int& nTargetX, int& nTargetY, bool bback = true);
	BOOL		IsCross(int nCurX,int nCurY, int nTargetX, int nTargetY);
	BYTE*		GetTempTerrain(CGameScene* pScene, CCharacter* pCha,int iCurX, int iCurY);

    void	    Lock()		{ _bblock = true;	}
    void	    UnLock()	{ _bblock = false;	}
    BOOL	    FindPath(CGameScene* pScene, CCharacter* pCha, int nSelfX, int nSelfY, int nTargetX, int nTargetY, bool &IsWalkLine);

protected:
	D3DXVECTOR3	_vStart;

	int			_nCurX,_nCurY;

	int			_nTargetX;
	int			_nTargetY;

	S_BVECTOR<BYTE>			_vecDir;
	S_BVECTOR<BYTE>			_vecDirSave;
	S_BVECTOR<D3DXVECTOR3>	_vecPathPoint;

	int			_nWidth;

	bool		_bblock;

    int         _nLength;

private:
	long		MAX_PATH_STEP;	
	long		STEP_LIMIT;		

	BYTE*		_byTempBlock; // jze
	bool        m_bFindPath; //jze
	int			m_iRange; //jze
	bool        m_bLongPathFinding; //jze
};

extern CFindPath g_cFindPath;
