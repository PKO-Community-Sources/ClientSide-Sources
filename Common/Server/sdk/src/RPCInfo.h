//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// This program is modified recently by Zhang.Dabo in 2003.7
//=================================================================
#ifndef RPCINFO_H
#define RPCINFO_H
#include "ThreadPool.h"
#include "Comm.h"
#define SESSFLAG (0x80000000)

_DBC_BEGIN
//==============================================================================
class TGenSessID
{
	friend class RPCMGR;
	friend class RPCInfo;

	TGenSessID()
		:m_nextsessid(1)
	{
		m_mtxSessid.Create(false);
	}
	uLong	GetSessid(){
		uLong l_retval;
		m_mtxSessid.lock();
		try{
			l_retval	=m_nextsessid ++;
			m_nextsessid=(m_nextsessid >(SESSFLAG-2))?1:m_nextsessid;//最高位作为标志位,0-调用,1-返回
		}catch(...){}
		m_mtxSessid.unlock();
		return l_retval;
	};

	Mutex				m_mtxSessid;
	uLong	volatile	m_nextsessid;
};
//==============================================================================
class TFNCall:public PreAllocStru
{
	friend class RPCMGR;
	friend class RPCInfo;
public:
	TFNCall(uLong size):PreAllocStru(size),m_retbuf(0),m_pointer(0),m_nextfnc(0),m_sessid(0)
	{
		m_semBlock.Create(0,1,0);
		m_mtx.Create(false);
	};
	~TFNCall()
	{
		delete m_nextfnc;
	}
private:
	void Initially(){
		TimeBlock(0);	//刷掉信号
	}
	void Finally(){
		m_retbuf	=0;
		m_pointer	=0;
		m_nextfnc	=0; 

		m_sessid	=0;
	}
	DWORD TimeBlock(DWORD time)
	{
		return (m_semBlock.timelock(time)==WAIT_OBJECT_0)?0:-1;
	}
	int SendSignal(){return m_semBlock.unlock();}

	uLong		volatile	m_sessid;			//调用的sessid
	uLong		volatile	m_oldsessid;		//备份的sessid
	bool		volatile	m_isSync;

	RPacket					m_retbuf;			//同步需要
	void		*volatile	m_pointer;			//异步需要

	Sema					m_semBlock;
	Mutex					m_mtx;

	TFNCall		*volatile	m_nextfnc;
};
//==============================================================================
class RPCInfo
{
	friend class DataSocket;
	friend class RPCMGR;
	RPCInfo():m_fncall(0)
	{
		m_mtxfncall.Create(false);
	};
	~RPCInfo()
	{
		TFNCall	*l_fncall;
		while(l_fncall =m_fncall)
		{
			l_fncall->m_mtx.lock();
			try{
				if(l_fncall->m_sessid && DelFnCall(l_fncall))
				{
					if(l_fncall->m_isSync)
					{
						l_fncall->m_sessid	=0;
						l_fncall->SendSignal();
					}
					else
					{
						l_fncall->Free();
					}
				}
			}catch(...){}
			l_fncall->m_mtx.unlock();
		}
	}
/*调用记录*/

	uLong			AddFnCall(TFNCall	*fncall);
	bool			DelFnCall(TFNCall	*fncall);

	TGenSessID					m_gensessid;

	Mutex						m_mtxfncall;		//控制函数调用结构链表操作的读写互斥体
	TFNCall			*volatile	m_fncall;			//函数调用结构
};
//================================================================================
class OnServeCall:public PreAllocTask
{
public:
	OnServeCall(uLong size):PreAllocTask(size),m_rpk(0),m_sessid(0){};
	long			Process();
	Task		*	Lastly(){return	PreAllocTask::Lastly();}
	inline void		Init(DataSocket * datasock,uLong sessid,RPacket &rpk);
private:
	TcpCommApp	*volatile	__tca;
	RPCMGR		*volatile	__rpc;
	DataSocket	*volatile	m_datasock;
	uLong					m_sessid;
	RPacket					m_rpk;
};
//================================================================================
class OnAsynReturn:public PreAllocTask
{
public:
	OnAsynReturn(uLong size):PreAllocTask(size),m_rpk(0),m_pointer(0){};
	long			Process();
	Task		*	Lastly(){return	PreAllocTask::Lastly();}
	inline void		Init(DataSocket * datasock,RPacket &rpk,void *pointer);
private:
	TcpCommApp	*volatile	__tca;
	RPCMGR		*volatile	__rpc;
	DataSocket	*volatile	m_datasock;
	RPacket					m_rpk;
	void		*volatile	m_pointer;
};
_DBC_END	//RPCINFO_H

#endif
