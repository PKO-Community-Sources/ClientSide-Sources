#pragma once

// 用于稳定帧数,并且不超帧
// 实现方法:降低渲染次数
class CSteadyFrame
{
public:
	CSteadyFrame() { 
		SetFPS( 30 );
	}

	bool	Init();

	static DWORD	GetFPS()	{ return _dwFPS;		}
	void	SetFPS( DWORD v )	{ 
		_dwFPS = v;	
		_dwTimeSpace = (int)(1000.0f/(float)_dwFPS);
	}

	bool	Run(){
		if( _lRun>0 && _lRun>0 )
		{
			_lRun=0;
			_dwCurTime = GetTickCount();

			_dwRunCount++;
			return true;
		}
		return false;
	}
	
	// Add by lark.li 20080923 begin
	void	Exit();
	// End

	DWORD	GetTick()		{ return _dwCurTime;		}
	void	End()			{ 
		_dwTotalTime += GetTickCount() - _dwCurTime;
	}

	void	RefreshFPS()	{ if(_dwFPS!=_dwRefreshFPS) SetFPS(_dwRefreshFPS);	}

private:
	static DWORD WINAPI _SleepThreadProc( LPVOID lpParameter ){
		((CSteadyFrame*)lpParameter)->_Sleep();
		return 0;
	}

	void	_Sleep();

private:
	static DWORD	_dwFPS;			// 设定的FPS,一秒要渲染多少帧

	long	_lRun;

	DWORD	_dwCurTime;	
	DWORD	_dwTimeSpace;

	DWORD	_dwTotalTime;
	DWORD	_dwRunCount;

	DWORD	_dwRefreshFPS;

	// Add by lark.li 20080923 begin
	HANDLE hThread;
	// End
};
