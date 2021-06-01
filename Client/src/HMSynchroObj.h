#pragma once

class CSynchroManage;
class CServerHarm;
class CCharacter;

// ���ڿͻ���ͬ�������������ʾ
class CStateSynchro
{
    friend class CSynchroManage;
public:
    CStateSynchro();
    virtual ~CStateSynchro();

    // ����ǰ�������������ָ���(����)��һ����
    virtual CStateSynchro*   Gouge( float fRate )    { return NULL;  }

    void    Exec();
    void    Exec( DWORD time );

    bool    GetIsExec()							{ return _isExec;       }

	void	SetServerHarm( CServerHarm* pHarm )	{ _pServerHarm=pHarm; 	}
	CServerHarm*	GetServerHarm()				{ return _pServerHarm;	}

	void	Reset()								{ _isExec=false;		}

	virtual CCharacter*	GetHarmCha() = 0;
	virtual const char* GetClassName()			{ return "CStateSynchro";}

protected:
    virtual void _Exec(){}

    bool			_isExec;

	CServerHarm*	_pServerHarm;

private:
    int				_nID;				// CSynchroManage������
	DWORD			_dwExecTime;		// ִ��ʱ��
	DWORD			_dwCreateTime;

};

// ���ڹ���ͬ������ʹ�䰴һ��˳��ִ��
class CSynchroManage
{
    friend class CStateSynchro;
public:
    CSynchroManage();
    ~CSynchroManage();

    void	FrameMove( DWORD dwTime );
	void	Reset();

public: 
    static CSynchroManage* I() { return _pInstance;     }

private:
    void    _CollectEmpty();        // ����ͷβ�Ѿ���ӣ���ʵ�����пռ䣬�ռ�ʣ��ռ�

private:
    int     _AddState( CStateSynchro* pState );
    bool    _DelState( CStateSynchro* pState );

private:
    enum 
    {
        MAX_SIZE = 4096,
        ERROR_ID = -1,
    };

    CStateSynchro*  _All[MAX_SIZE];
    DWORD           _dwHead, _dwTail;       // ͷβ��Ч��
    DWORD           _nSynchroNum;           // ��Ч״̬����

private:
    static CSynchroManage* _pInstance;

};

// ��������
inline void CStateSynchro::Exec()
{ 
    _isExec = true; 
}

inline void CStateSynchro::Exec( DWORD time )
{
	if( time<_dwExecTime )
	{
		_dwExecTime = time;
	}
}

