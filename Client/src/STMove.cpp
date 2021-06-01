#include "stdafx.h"
#include "stmove.h"
#include "actor.h"
#include "GameApp.h"
#include "NetProtocol.h"
#include "Character.h"
#include "DrawPointList.h"
#include "CharacterAction.h"
#include "PacketCmd.h"
#include "SceneItem.h"
#include "FindPath.h"
#include "uiminimapform.h"
#include "CameraCtrl.h"
#include "STAttack.h"

//---------------------------------------------------------------------------
// class CWaitMoveState
//---------------------------------------------------------------------------
const float	_fNormalRate = 0.95f;

unsigned long CWaitMoveState::_ulPreMoveTime = 500;
float	CWaitMoveState::_fMoveRate = 1.0f;
DWORD	CWaitMoveState::_dwPreMoveDis = 240;

int CWaitMoveState::MaxPing = 0;
int CWaitMoveState::MinPing = 5000;
int CWaitMoveState::LastPing[LAST_NUM] = { 0 };
int CWaitMoveState::LastPingCnt = 0;
int CWaitMoveState::nTotalPing = 0;
int CWaitMoveState::nTotalPingCnt = 0;
float CWaitMoveState::fAveragePing = 0.0f;
int CWaitMoveState::LastPingShow[LAST_NUM] = { 0 };

void CWaitMoveState::SetPreMoveTime( long time )		
{ 
	_ulPreMoveTime = time;

	if( MaxPing < time )
		MaxPing = time;

	if( MinPing > time )
		MinPing = time;

	LastPing[ LastPingCnt ] = time;

	int nCnt = LastPingCnt;
	for( int i=0; i<LAST_NUM; i++ )
	{
		LastPingShow[i] = LastPing[ nCnt ];
		nCnt--;
		if( nCnt<0 ) nCnt = LAST_NUM-1;
	}

	LastPingCnt++;
	if( LastPingCnt >= LAST_NUM ) LastPingCnt = 0;

	nTotalPing += time;
	nTotalPingCnt++;
	fAveragePing = (float)nTotalPing / (float)nTotalPingCnt;

	static DWORD nTotalFPS = 0;
	static DWORD nTotalFPSCnt = 0;
	static float fFPSAverage = 0.0f;
	static DWORD nMaxFPS = 0;
	static DWORD nMinFPS = 500;
	if( nMaxFPS < g_Render.GetFPS() )
		nMaxFPS = g_Render.GetFPS();
	if( nMinFPS > g_Render.GetFPS() )
		nMinFPS = g_Render.GetFPS();
	nTotalFPS += g_Render.GetFPS();
	nTotalFPSCnt++;
	fFPSAverage = (float)nTotalFPS / (float)nTotalFPSCnt;
	LG( "ping", g_oLangRec.GetString(404), LastPingShow[0], LastPingShow[1], LastPingShow[2], MaxPing, fAveragePing, MinPing, nMaxFPS, fFPSAverage, nMinFPS );
}

void CWaitMoveState::RenderPing()
{
	// ���� CTRL + ALT + SHIFT + K ��ʾ������

	//static char szBuf[128] = { 0 };
	//sprintf( szBuf, g_oLangRec.GetString(405), LastPingShow[0], LastPingShow[1], LastPingShow[2], MaxPing, fAveragePing, MinPing );
	//g_pGameApp->GetFont()->DrawText( szBuf, 5, 55, D3DCOLOR_ARGB(255,255,255,255));
}

void CWaitMoveState::RenderCameraInfo()
{
#if 1
	CCameraCtrl* pCam = g_pGameApp->GetMainCam();

	static char szBuf[128] = { 0 };
	int y(65), step(16), line(5);

	GetRender().FillFrame(0, y - 5, 160, y + 5 + step * line);	// ��ɫ����

	sprintf( szBuf, " CameraRangeXY  = %.2f ", pCam->m_fxy);
	g_pGameApp->GetFont()->DrawText( szBuf, 5, y, D3DCOLOR_ARGB(255,255,255,255));
	y += step;

	sprintf( szBuf, " CameraRangeZ   = %.2f ", pCam->m_fz);
	g_pGameApp->GetFont()->DrawText( szBuf, 5, y, D3DCOLOR_ARGB(255,255,255,255));
	y += step;

	sprintf( szBuf, " CameraRangeFOV = %.2f ", pCam->m_ffov);
	g_pGameApp->GetFont()->DrawText( szBuf, 5, y, D3DCOLOR_ARGB(255,255,255,255));
	y += step;

	sprintf( szBuf, " CameraRotate   = %.2f ", pCam->m_fAngle);
	g_pGameApp->GetFont()->DrawText( szBuf, 5, y, D3DCOLOR_ARGB(255,255,255,255));
	y += step;

	sprintf( szBuf, " CameraShowSize = %d ", pCam->m_iShowWidth);
	g_pGameApp->GetFont()->DrawText( szBuf, 5, y, D3DCOLOR_ARGB(255,255,255,255));
	y += step;
#endif
}


CWaitMoveState::CWaitMoveState(CActor* p)
:CActionState(p)
{
    _pCha = GetActor()->GetCha();
}

CWaitMoveState::~CWaitMoveState()
{
}

void CWaitMoveState::ChaRun()
{
    _pCha->CheckIsFightArea();

	_pCha->PlayPose( _pCha->GetPose(POSE_RUN), PLAY_LOOP_SMOOTH );
	_pCha->SetPoseVelocity( (float)_pCha->getMoveSpeed() / 480.0f );
    _pCha->RefreshItem();
}

bool CWaitMoveState::_Start()
{
    ChaRun();
	return true;
}

void CWaitMoveState::FrameMove()
{	
	if( _pCha->GetIsArrive() )
	{
		if( !GetMoveList()->IsEmpty() )
		{
			GetMoveList()->GotoFront(_pCha);
		}
		else if( _IsOver )
		{
			PopState();
			return;
		} 
	}
}
 
void CWaitMoveState::PushPoint( int x, int y )	
{	
	GetMoveList()->PushPoint( x, y );
}

bool CWaitMoveState::IsAllowMove()
{
	if( g_stUIMap.IsShowBigmap() )
	{
		// g_pGameApp->SysInfo( "��ʾ���ͼʱ�������ƶ�" );
		return false;
	}

	if( _pCha->GetChaState()->IsFalse(enumChaStateMove) )
	{
		g_pGameApp->SysInfo( g_oLangRec.GetString(406) );
		return false;
	}

 //   if( _pCha->getMoveSpeed()<=0 )
	//{
	//	g_pGameApp->SysInfo( "�ٶ�Ϊ��,�����ƶ�" );
 //       return false;
	//}

	return true;
}

//---------------------------------------------------------------------------
// class CMoveState
//---------------------------------------------------------------------------
CMoveState::CMoveState(CActor* p) 
: CWaitMoveState(p), _IsWalkLine(false), _nTargetX(0), _nTargetY(0)
{
	_IsSend = true;
}

bool CMoveState::_Start()
{	
    if( !IsAllowMove() )
        return false;

    int serverdis = 0;
    bool isServer = false;
    static stNetMoveInfo serverPath;
	if( g_cFindPath.Find( _pCha->GetScene(), _pCha, _pCha->GetServerX(), _pCha->GetServerY(), _nTargetX, _nTargetY, _IsWalkLine ) )
    {            
        WriteInfo( g_cFindPath.GetResultPath(), serverPath );
        serverdis = g_cFindPath.GetLength();
    }

    if( !g_cFindPath.GetPathFindingState() )
    {
        return false;
    }

    WriteInfo( g_cFindPath.GetResultPath(), _stPathInfo );

    // �Ƚ�����Ѱ·,�Ƿ��յ���ͬ,�յ㲻ͬ,��������Ѱ·
    if( isServer )
    {
        if( memcmp( &serverPath.pos_buf[serverPath.pos_num-1], &_stPathInfo.pos_buf[_stPathInfo.pos_num-1], sizeof(POINT) )!=0 )
        {
            memcpy( _stPathInfo.pos_buf, serverPath.pos_buf, sizeof(_stPathInfo.pos_buf) );
            _stPathInfo.pos_num = serverPath.pos_num;

            isServer = false;
        } 
    }

    float fRate = 1.0f;
    if( isServer )
    {
        fRate = (float)g_cFindPath.GetLength() / (float)serverdis;

        if( fRate>1.10f ) fRate = 1.10f;
        if( fRate<0.90f ) fRate = 0.90f;
    }

    // ��һ�������Լ�������
    int n = _stPathInfo.pos_num;
    for( int i=2; i<n; i++ )
    {
        GetMoveList()->PushPoint( _stPathInfo.pos_buf[i].x, _stPathInfo.pos_buf[i].y );
    }

    ChaRun();

    _pCha->MoveTo( _stPathInfo.pos_buf[1].x, _stPathInfo.pos_buf[1].y );

#ifdef _STATE_DEBUG
    LG( "movestate", "start SelfPos:%d, %d, Target: %d, %d Tick - %d\n", GetActor()->GetCha()->GetCurX(), GetActor()->GetCha()->GetCurY(), _nTargetX, _nTargetY, GetTickCount() );
#endif

    if( _IsSend )    
    {
        if( isServer )
        {
            serverPath.dwAveragePing = _ulPreMoveTime;
            CS_BeginAction( _pCha, enumACTION_MOVE, (void*)&serverPath, this );
        }
        else
        {
            _stPathInfo.dwAveragePing = _ulPreMoveTime;
            CS_BeginAction( _pCha, enumACTION_MOVE, (void*)&_stPathInfo, this );
        }
    }
	return true;
}

void CMoveState::WriteInfo( S_BVECTOR<D3DXVECTOR3>& path, stNetMoveInfo& info )
{	
	char buf[80] = { 0 };

    int n = path.size();
    if( n > defMAX_POS_NUM )  n = defMAX_POS_NUM;		
    for( int i=0; i<n; i++ )
    {
        info.pos_buf[i].x = (long)(path[i]->x * 100.0f) / 50 * 50 + 25;
        info.pos_buf[i].y = (long)(path[i]->y * 100.0f) / 50 * 50 + 25;

		LG( "move_path", "%d, %d\n", info.pos_buf[i].x, info.pos_buf[i].y );
    }

    info.pos_num = n;

	LG( "move_path", "\n\n" );
}

void CMoveState::PushPoint( int x, int y )	
{
    int dis = GetDistance( x, y, _pCha->GetCurX(), _pCha->GetCurY() );
    if( dis > 500 )
    {
        g_pGameApp->AddTipText( g_oLangRec.GetString(407), dis );
    }
}

bool CMoveState::_IsAllowCancel()
{	
	if( !g_NetIF->IsConnected() ) return true;
	else return _AllowCancel();
}

void CMoveState::MoveEnd(int x, int y, int nState)
{
	_IsOver = true;
    if( !GetParent() )
    {
        if( enumMSTATE_BLOCK & nState )
        {
            PopState();

            _SynchroServer();
            return;
        }

        if( GetDistance( _pCha->GetServerX(), _pCha->GetServerY(), _pCha->GetCurX(), _pCha->GetCurY() )>2000 )
        {
            _SynchroServer();
        }
    }
}

void CMoveState::_End()
{
    if ( !GetIsCancel() )
    {
		if( _pCha->GetServerX()!=_pCha->GetCurX() || _pCha->GetServerY()!=_pCha->GetCurY() )
        {
            _SynchroServer();
        }
    }
}

void CMoveState::FrameMove()
{
    if( _IsCancel && _IsOver )
    {
        _pCha->StopMove();
        PopState();
        return;
    }

    if( _pCha->GetIsArrive() )
    {
        if( !GetMoveList()->IsEmpty() )
        {
            GetMoveList()->GotoFront(_pCha);
        }
        else if( _IsOver )
        {
			if( _pCha->GetServerX()!=_pCha->GetCurX() || _pCha->GetServerY()!=_pCha->GetCurY() )
            {
                _SynchroServer();
            }

            PopState();
            return;
        }
        else
        {
            if( !GetParent() && GetActor()->IsEmpty() )
            {
                GetActor()->IdleState();
            }
        }
    }
}

void CMoveState::_SynchroServer()
{
	_pCha->setPos( _pCha->GetServerX(), _pCha->GetServerY() );
	_pCha->MoveTo( _pCha->GetServerX(), _pCha->GetServerY() );

	g_pGameApp->SetCameraPos( _pCha->GetPos() );
	g_pGameApp->GetMainCam()->BeginMove();
}

void CMoveState::Cancel()
{
    _pCha->StopMove();
    GetMoveList()->Clear();
    _IsOver = true;
}

//---------------------------------------------------------------------------
// class CServerMoveState
//---------------------------------------------------------------------------
DWORD CServerMoveState::_dwLastEndTime = 0;
DWORD CServerMoveState::_dwLastMoveTime = 0;

bool CServerMoveState::_Start()
{	
	_fMoveRate = _fNormalRate;

	if( _nTargetX<0 || _nTargetY<0 )
		return false;

	if( !IsAllowMove() )
		return false;

    bool isServer = false;
	if( _pCha->GetServerX()!=_pCha->GetCurX() || _pCha->GetServerY()!=_pCha->GetCurY() )
    {
		if( g_cFindPath.Find( _pCha->GetScene(), _pCha, _pCha->GetServerX(), _pCha->GetServerY(), _nTargetX, _nTargetY, _IsWalkLine ) )
        {            
            WriteInfo( g_cFindPath.GetResultPath(), _stPathInfo );
            isServer = true;
        }
        else
        {
            return false;
        }
    }

    if( !isServer )
    {
		//LG( "test", "msgtest" );
        if( g_cFindPath.Find( _pCha->GetScene(), _pCha, _pCha->GetCurX(), _pCha->GetCurY(), _nTargetX, _nTargetY, _IsWalkLine ) )
        {
            WriteInfo( g_cFindPath.GetResultPath(), _stPathInfo );
        }
        else
        {
            return false;
        }
    }

	_stPathInfo.dwAveragePing = _ulPreMoveTime;

    if( !GetParent() )
    {
		PreMove();
    }

    if( _IsSend )    
    {
        CS_BeginAction( _pCha, enumACTION_MOVE, (void*)&_stPathInfo, this );
		_dwLastMoveTime = CGameApp::GetCurTick();
    }
    return true;
}

void CServerMoveState::PreMove()
{
	ChaRun();

	POINT	SPos[defMOVE_LIST_POINT_NUM];
	int		nPointNum = defMOVE_LIST_POINT_NUM;
    int dis = int(_ulPreMoveTime * (float)_pCha->getMoveSpeed() / 1000.0f);
	int num = GetMovePoint( _stPathInfo.pos_buf, _stPathInfo.pos_num, dis, SPos, nPointNum );
	if( num > 1 )
	{
	    _pCha->MoveTo( SPos[1].x, SPos[1].y );
		for( int i=2; i<num; i++ )
		{
			GetMoveList()->PushPoint( SPos[i].x, SPos[i].y );
		}

#ifdef _STATE_DEBUG
		g_pGameApp->GetDrawPoints()->Add( SPos[num-1].x, SPos[num-1].y, 0xff00ff00, 0.1f );
#endif
	}
}

void CServerMoveState::Cancel()
{
    CActionState::Cancel();

	if( !g_NetIF->IsConnected() )
	{
		GetMoveList()->Clear();
	}
}

void CServerMoveState::FrameMove()
{
    CWaitMoveState::FrameMove();

	// ���ݾ�������ٶ�
	//DWORD dis = 0;
	//if( !_IsClientMove && _pCha->getMoveSpeed()>0 )
	//{
	//	dis = GetMoveList()->GetListDistance( _pCha->GetCurX(), _pCha->GetCurY() );
	//	//if( dis > _dwPreMoveDis )
	//	//{
	//		//_fMoveRate = 1.0f + float( dis - _dwPreMoveDis ) / (float)dis / 4.0f;
	//	//}

	//	float fRate = 1.0f + float( dis ) / (float)_pCha->getMoveSpeed(); 
	//	if( _fMoveRate > fRate )
	//		_fMoveRate -= 0.02f;
	//	else
	//		_fMoveRate += 0.02f;
	//}
	//LG( "moverate", "%d, %f\n", dis, _fMoveRate );
}

void CServerMoveState::_End()
{
	_dwLastEndTime = CGameApp::GetCurTick();

	DWORD dwSpace = _dwLastEndTime - _dwLastMoveTime;
	LG( this->GetActor()->GetCha()->getLogName(), g_oLangRec.GetString(408), dwSpace, _dwLastMoveTime, _dwLastEndTime, (float)dwSpace * (float)GetActor()->GetCha()->getMoveSpeed() / 1000.0f );
}

void CServerMoveState::MoveEnd(int x, int y, int nState)
{
    _IsOver = true;
}

void CServerMoveState::PushPoint( int x, int y )	
{
	if( _IsClientMove )
	{
		if( GetMoveList()->IsEmpty() )
		{
			_pCha->MoveTo( x, y );
		}
		else
		{
			POINT& pt = GetMoveList()->GetBack();
			pt.x = x;
			pt.y = y;
		}

		_IsClientMove = false;
		return;
	}

	GetMoveList()->PushPoint( x, y );
	return;
}

//---------------------------------------------------------------------------
// class CBackMoveState
//---------------------------------------------------------------------------
bool CBackMoveState::_Start()
{
	ChaRun();
	_pCha->ForceMove( _nTargetX, _nTargetY );
	return true;
}

void CBackMoveState::FrameMove()
{
	if( _pCha->GetIsArrive() )
	{
		PopState();
	}
}

//---------------------------------------------------------------------------
// class COneMoveState
//---------------------------------------------------------------------------
CMoveList COneMoveState::_List;

COneMoveState::COneMoveState(CActor* p)
: CWaitMoveState(p), _IsWalkLine(false), _nTargetX(0), _nTargetY(0)
{
	_nStartX = _nStartY = 0;
	_IsStartWalkLine = false;

	_nMoveCount = _nStartCount = _nEndCount = _nSendCount = 0;
	_dwSendTime = _dwEndTime = 0;
	_IsSendCancel = false;
	_fRate = _fNormalRate;
}

// �����µ��ƶ�·��
bool COneMoveState::ContinueMove( int nTargetX, int nTargetY, bool isWalkLine, bool IsCheckTime )
{	
	// ��������Ƿ�Ϸ������Ϸ���ֱ������
	if( GetDistance( _pCha->GetCurX(), _pCha->GetCurY(), _pCha->GetServerX(), _pCha->GetServerY() ) > 2000 )
	{
		SynchroPos( _pCha->GetServerX(), _pCha->GetServerY() );
		if( !g_cFindPath.Find( _pCha->GetScene(), _pCha, _pCha->GetCurX(), _pCha->GetCurY(), nTargetX, nTargetY, isWalkLine ) )
		{
			return false;
		}
	}
	else if( !g_cFindPath.Find( _pCha->GetScene(), _pCha, _pCha->GetCurX(), _pCha->GetCurY(), nTargetX, nTargetY, isWalkLine ) )
	{
		if( GetDistance( _pCha->GetCurX(), _pCha->GetCurY(), _pCha->GetServerX(), _pCha->GetServerY() ) < GetSyschroDistance() )
			return false;

		SynchroPos( _pCha->GetServerX(), _pCha->GetServerY() );
		if ( !g_cFindPath.Find(_pCha->GetScene(), _pCha, _pCha->GetCurX(), _pCha->GetCurY(), nTargetX, nTargetY, isWalkLine ) )
			return false;
			
		/*if( !g_cFindPath.GetPathFindingState() )
		{
			return false;
		}*/
	}

	LG( "main_move", "ContinueMove( %d, %d, %d )\n", nTargetX, nTargetY, isWalkLine );

	bool rv = true;

	// ���ݲ��������µ��ƶ�·���ֶ�Ϊ����ƶ�·��
	int len = -1;		// ���ֶ�
	extern int MOVE_LENGTH;
	if( MOVE_LENGTH > 0 ) 
	{
		len = _pCha->getMoveSpeed() / 2;
		if ( len < (int)_dwPreMoveDis ) len = _dwPreMoveDis;
		if ( len < 350 ) len = 350;
		if( len < MOVE_LENGTH ) len = MOVE_LENGTH;
	}

	CompartMoveList( _List, g_cFindPath.GetResultPath(), len );

	const DWORD MOUSE_TIME = 300;
	static DWORD dwTime = 0;
	if( !IsCheckTime || ( IsCheckTime && CGameApp::GetCurTick() > dwTime ) )
	{
		if( IsCheckTime )
		{
			dwTime = CGameApp::GetCurTick() + MOUSE_TIME;
		}

		POINT cur_pt = _List.GetFront();
		_List.PopFront();
		if( !StartMove( cur_pt.x, cur_pt.y, isWalkLine ) ) 
			rv = false;
	}

	// �����û�������ƶ�·��
	_cNeedList.clear();
	while( !_List.IsEmpty() )
	{
		POINT pt = _List.GetFront();
		_List.PopFront();
		AddPath( pt.x, pt.y, isWalkLine );
	}
	return rv;
}

bool COneMoveState::AddPath( int x, int y, bool IsWalkLine )
{
	LG( "main_move", "AddPath( %d, %d, %d )\n", x, y, IsWalkLine );

	_cNeedList.push_back( NeedPath( x, y, IsWalkLine ) );
	return true;
}

bool COneMoveState::SendInfo()
{
	// �����ƶ�Э��
	if( !g_cFindPath.GetPathFindingState() )
	{
		return true;
	}

	ChaRun();

	LG( "main_move", "SendInfo: ServerPos[%d, %d], Target[%d, %d]\n", _pCha->GetServerX(), _pCha->GetServerY(), _nTargetX, _nTargetY );
	_nSendCount++;

	stNetMoveInfo   stPathInfo;
	WriteInfo( g_cFindPath.GetResultPath(), stPathInfo );
	CS_BeginAction( _pCha, enumACTION_MOVE, (void*)&stPathInfo, this );
	_IsSendCancel = true;
	_nServerDis = g_cFindPath.GetLength();
	_nServerStepDis = 0;

	_dwSendTime = CGameApp::GetCurTick();
	DWORD len = (DWORD)( (float)(_nServerDis) / (float)_pCha->getMoveSpeed() * 1000.0f );
	_dwEndTime = CGameApp::GetCurTick() + len;

	int nLocal = 0;
	if( _cLocalList.IsEmpty() )
	{
		nLocal = GetDistance( _pCha->GetCurX(), _pCha->GetCurY(), _nTargetX, _nTargetY );
	}
	else
	{
		nLocal = _cLocalList.GetListDistance( _pCha->GetCurX(), _pCha->GetCurY() ) + _pCha->GetTargetDistance();
	}

	// �����������ʣ������������̫Զ��ֱ����������
	if( abs( nLocal - _nServerDis ) > GetSyschroDistance() )
	{
		// �����������
		_fMoveRate = _fNormalRate;
		_fRate = _fNormalRate;
		SynchroPos( _pCha->GetServerX(), _pCha->GetServerY() );

		_nLocalDis = g_cFindPath.GetLength();

		_cLocalList.Clear();

		S_BVECTOR<D3DXVECTOR3>& path = g_cFindPath.GetResultPath();
		int n = path.size();
		for( int i=1; i<n; i++ )
		{
			_cLocalList.PushPoint( (long)(path[i]->x * 100.0f), (long)(path[i]->y * 100.0f) );
		}
		return true;
	}

	_fRate = RefreshRate( nLocal, _nServerDis );
	LG( "moverate", "SendInfo:%f\n", _fRate );
	return true;
}

int	COneMoveState::GetSyschroDistance()
{
	// ��Ҫͬ���ľ��룬���������پ���ʱ��Ҫֱ����������
	int len = _pCha->getMoveSpeed() + _pCha->getMoveSpeed() + (int)_dwPreMoveDis;
	if( len < 1000 ) len = 1000;
	return len;
}

void COneMoveState::FrameMove()
{
	// ��̬���������ƶ�����
	const float fAddStep = 0.005f;
	const float fDownStep = 0.05f;
	if( _fMoveRate > _fRate )
	{
		_fMoveRate -= fDownStep;
	}
	else if( _fMoveRate < _fRate - fDownStep )
	{
		_fMoveRate += fAddStep;
	}

	if( _nSendCount==_nEndCount && _nStartCount>_nSendCount )
	{
		// �������Ѿ������ƶ��������������ǻ����ƶ��������µ��ƶ�Э��
		if( !SendInfo() )
		{
			PopState();
			return;
		}
	}
	
	if( _pCha->GetIsArrive() )
	{
		if( _cLocalList.IsEmpty() && _cNeedList.empty() && _nSendCount==_nEndCount )
		{
			// �������Ѿ������ƶ�����������Ҳû�л����ƶ���״̬����
			PopState();
			return;
		}

		if( !_cLocalList.IsEmpty() )
		{
			// ִ�л����ƶ�
			if( !(_pCha->GetCurPoseType()==POSE_RUN || _pCha->GetCurPoseType()==POSE_RUN2) )
			{
				ChaRun();
			}
			_cLocalList.GotoFront(_pCha);
		}
		else if( GetActor()->IsEmpty() && _cNeedList.empty() )
		{			
			// �����Ѿ��ȷ��������ƶ���Ŀ�ĵأ����ڵȴ��������Ľ�����־
			// Ϊ�˷�ֹ����ԭ��̤����ֱ�ӵ���Idle
			GetActor()->IdleState();
		}
	}

	if( _nSendCount==_nEndCount && CGameApp::GetCurTick()>_dwEndTime )
	{
		if( dynamic_cast<CAttackState*>( GetActor()->GetNextState() ) )
		{
			// ��������һ��״̬Ϊ����״̬���ҷ������Ѿ����ؽ���
			// �����Ƿ���ڻ����ƶ����������ƶ�״̬������׼������
			PopState();
			return;
		}

		if( !_cNeedList.empty() )
		{
			// �������Ѿ����ؽ�����ִ���µĻ����ƶ�
			NeedPath& pt = _cNeedList.front();
			StartMove( pt.x, pt.y, pt.IsLine );
			_cNeedList.pop_front();
		}
	}
}

void COneMoveState::PushPoint( int x, int y )
{
	// ����������������ľ����������ǵ��ٶ�
	int nLocal = 0;
	if( _cLocalList.IsEmpty() )
	{
		nLocal = GetDistance( _pCha->GetCurX(), _pCha->GetCurY(), _nTargetX, _nTargetY );
	}
	else
	{
		nLocal = _cLocalList.GetListDistance( _pCha->GetCurX(), _pCha->GetCurY() ) + _pCha->GetTargetDistance();
	}

	_fRate = _fNormalRate;
	if( GetDistance( _pCha->GetServerX(), _pCha->GetServerY(), _nTargetX, _nTargetY )>80 )
	{
		if( g_cFindPath.GetPathFindingState() )
		{
			_fRate = RefreshRate( nLocal, g_cFindPath.GetLength() );
		}
	}
	LG( "moverate", "PushPoint:%f\n", _fRate );
}

bool COneMoveState::StartMove( int nTargetX, int nTargetY, bool isWalkLine )
{
	// ���µ��ƶ�·����������ֹ��ǰ�ƶ���Э�飬����������ǰ�����µ�·������
	if( !IsAllowMove() )
		return false;

	if( !g_cFindPath.GetPathFindingState() )
		return false;

	LG( "main_move", "StartMove Target[%d, %d], StartCount:%d, SendCount:%d, EndCount:%d\n", _nTargetX, _nTargetY, _nStartCount, _nSendCount, _nEndCount );
	_nMoveCount++;
	if( _nStartCount==_nSendCount )
	{
		_nStartCount++;
		if( _nSendCount==_nEndCount+1 )
		{
			CS_EndAction( this );

			_dwSendTime = CGameApp::GetCurTick();
			_dwEndTime = CGameApp::GetCurTick();
			_IsSendCancel = false;
		}
	}
	
	_cLocalList.Clear();
	S_BVECTOR<D3DXVECTOR3>& path = g_cFindPath.GetResultPath();
    int n = path.size();
    for( int i=1; i<n; i++ )
    {
		_cLocalList.PushPoint( (long)(path[i]->x * 100.0f), (long)(path[i]->y * 100.0f) );
    }
	if( _cLocalList.GetCount() > 0 )
	{
		_pCha->StopMove();
	}

	_nLocalDis = g_cFindPath.GetLength();
	_nTargetX = _cLocalList.GetBack().x;
	_nTargetY = _cLocalList.GetBack().y;
	_IsWalkLine = isWalkLine;

	// ���ݱ����������Ҫ�ƶ��ľ����,���������ƶ�����
	if( g_cFindPath.GetPathFindingState() )
	{
		_fRate = RefreshRate( _nLocalDis, g_cFindPath.GetLength() );
	}
	LG( "moverate", "StartMove:%f\n", _fRate );
	return true;
}

float COneMoveState::RefreshRate( int nLocalLen, int nServerLen )
{
	float fRate = (float)nLocalLen / (float)(nServerLen) / 0.95f;
	if( fRate > 10.30f ) fRate = 10.30f;
	if( fRate < 0.85f ) fRate = 0.85f;
	LG( "moverate", "RefreshRate(%d, %d) Rate:%f\n", nLocalLen, nServerLen, _fRate );	
	return fRate;
}

bool COneMoveState::_Start()
{
	// ��ʼ�ƶ�
	if( !IsAllowMove() )
		return false;

	if( !ContinueMove( _nStartX, _nStartY, _IsStartWalkLine ) )
	{
		return false;
	}

    _dwPreMoveDis = int(_ulPreMoveTime * (float)_pCha->getMoveSpeed() / 1000.0f);

	//_fMoveRate = _fNormalRate;
	//_fRate = _fNormalRate;
	ChaRun();
	return true;
}

void COneMoveState::_End()
{
	LG( "main_move", "COneMoveState::_End\n" );
}

void COneMoveState::SynchroPos( int x, int y )
{
	// ֱ�������ǵ�������
	_pCha->setPos( x, y );
	_pCha->MoveTo( x, y );

	g_pGameApp->SetCameraPos( _pCha->GetPos(), false );
	g_pGameApp->GetMainCam()->BeginMove();
}

void COneMoveState::MoveEnd(int x, int y, int nState)
{
	// �������ƶ�����
	LG( "main_move", "MoveEnd( %d, %d, %d )\n", x, y, nState );
	_nEndCount++;

	_IsSendCancel = false;

	// ����������ϰ�ͣס,��ֱ��ͣס
	if( (nState & enumMSTATE_BLOCK) || (nState & enumMSTATE_CANTMOVE) )
	{
		PopState();
		SynchroPos( x, y );
		return;
	}
		
	if( (_nStartCount==_nSendCount) && (nState & enumMSTATE_CANCEL) )
	{
		_cLocalList.Clear();
		_cLocalList.PushPoint( x, y );
		_pCha->StopMove();
		return;
	}
}

void COneMoveState::Cancel()
{
	// _IsCancel = true;

	// ���ƶ�ʱ��cancel
	if( _IsSendCancel )
	{
		LG( "main_move", "Canlce\n" );
		CS_EndAction( this );
		_cNeedList.clear();

		_dwSendTime = CGameApp::GetCurTick();
		_dwEndTime = CGameApp::GetCurTick();
		_IsSendCancel = false;
	}
}

void COneMoveState::WriteInfo( S_BVECTOR<D3DXVECTOR3>& path, stNetMoveInfo& info )
{	
	// ��Ѱ·�ĵ�ת��Ϊ���͵���������Э������
    int n = path.size();
    if( n > defMAX_POS_NUM )  n = defMAX_POS_NUM;
    for( int i=0; i<n; i++ )
    {
        info.pos_buf[i].x = (long)(path[i]->x * 100.0f) / 50 * 50 + 25;
        info.pos_buf[i].y = (long)(path[i]->y * 100.0f) / 50 * 50 + 25;
		LG( "m_path", "Path:[%d, %d]\n", info.pos_buf[i].x, info.pos_buf[i].y );
		if( info.pos_buf[i].x!=(long)(path[i]->x * 100.0f) || info.pos_buf[i].y!=(long)(path[i]->y * 100.0f) )
			LG( "m_path", "###########FindPath:[%d, %d]\n", (long)(path[i]->x * 100.0f), (long)(path[i]->y * 100.0f) );
    }
    info.pos_num = n;
	LG( "m_path", "Count:%d\n", info.pos_num );
}

bool COneMoveState::IsSameServerPos( int x, int y )
{
	return _nMoveCount<=1;
}

void  COneMoveState::CompartMoveList( CMoveList& outlist, S_BVECTOR<D3DXVECTOR3>& path, int length )
{
	// ��һ��Ѱ·�㣬��length��Ϊ��Σ�С��0���ֶ�
	CMoveList& cLocalList = outlist;
	cLocalList.Clear();
	if( length<=0 ) 
	{
		int n = path.size();
		cLocalList.PushPoint( (long)(path[n-1]->x * 100.0f), (long)(path[n-1]->y * 100.0f) );
		return;

		//for( int i=1; i<n; i++ ) 
		//{
		//	cLocalList.PushPoint( (long)(path[i]->x * 100.0f), (long)(path[i]->y * 100.0f) );
		//}
		//return;
	}

	int n = path.size();
	int x, y;
	int oldx = (long)(path[0]->x * 100.0f);
	int oldy = (long)(path[0]->y * 100.0f);
	int dis = 0;
	int segment = 0;
	int dx, dy;
	for( int i=1; i<n; i++ ) 
	{
		x = (long)(path[i]->x * 100.0f);
		y = (long)(path[i]->y * 100.0f);

		do
		{
			segment = ::GetDistance( oldx, oldy, x, y );
			if( segment + dis > (int)length )
			{
				GetDistancePos( x, y, oldx, oldy, dis + segment - length, dx, dy );
				cLocalList.PushPoint( dx, dy );
				if( cLocalList.GetCount() > 10 ) 
					break;

				dis = 0;
				oldx = dx;
				oldy = dy;
			}
			else
			{
				dis += segment;

				oldx = x;
				oldy = y;
				break;
			}
		}
		while(true);

		if( i==n-1 ) 
		{
			POINT& pt = cLocalList.GetBack();
			if( GetDistance( pt.x, pt.y, x, y ) > (int)length / 2 )
			{
				cLocalList.PushPoint( x, y );
			}
			else
			{
				pt.x = x;
				pt.y = y;
			}
		}
	}
}

