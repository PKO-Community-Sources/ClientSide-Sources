//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// This program is modified recently by Zhang.Dabo in 2003.7
//=================================================================
#ifndef THREADPOOL_H
#define THREADPOOL_H

#ifndef USING_TAO				//ʹ��Win32����Platform SDK
#include <winsock2.h>			//ȷ�������µ�WinSock2.2�汾
#include <windows.h>
#else
#include "TAOSpecial.h"
#endif

#include "DBCCommon.h"
#include "excp.h"
#include "PreAlloc.h"

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)

#ifdef USING_IOCP
/*
	ʹ��IOCPʵ�ֵ��̳߳�
*/
class Task;
class Thread;
class ThrdQue;

#define	NOTWAITSTATE	(0x3FFFFFFF)
//========ThreadPool==============================================================================
class ThreadPool										//�̳߳ض���
{
	friend class Thread;
public:
	static ThreadPool* CreatePool(long running=0,long max=0x10,int nPriority=THREAD_PRIORITY_NORMAL);
	void DestroyPool();
	void AddTask(Task *task);

	long getCurr()			{return m_curr;}			//���ܼ��Ҫ��
	long getCurrFree()		{return m_currfree;}		//���ܼ��Ҫ��

	InterLockedLong			m_taskexitflag;
private:
	ThreadPool(long running,long max,int nPriority);
	virtual ~ThreadPool();
	void ThreadExcute(Thread *);

private:
	InterLockedLong			m_exitflag;
	const int				m_nPriority;
	const long				m_max,m_running;
	volatile long			m_curr,m_currfree;

	HANDLE		volatile	m_iocpHandle;
	ThrdQue	*	volatile	m_thrdQue;
};
//========Task==================================================================================
class Task{												//���������
	friend class ThreadPool;
	friend class TaskQue;
public:
	Task():__canwait(false),__ThisThread(0)
	{
		__mtxtaskwait.Create(false);
		if(!__mtxtaskwait)
		{
			THROW_EXCP(excpSync,"�������������ϵͳͬ���������");
		}
	};
	bool				GetExitFlag();
protected:
	virtual				~Task(){}
	virtual	Task	*	Lastly(){Free();return 0;}
	HANDLE				GetHandle();					//��ȡ�����̵߳ľ��
	DWORD				GetThreadID();					//��ȡ�����̵߳ı�־ID
private:
	virtual	void		Free(){delete this;}
	virtual	long		Process()=0;
	inline	Task	*	TaskExec(Thread *ThisThread);

	Thread				*	volatile	__ThisThread;
protected:
	struct
	{
		OVERLAPPED						__overlapped;
		uLong							__NumberOfBytes,
		int								__CompletionKey,
	};
};
//========PreAllocTask======================================================================
class PreAllocTask:public PreAllocStru,public Task{
public:
	PreAllocTask(uLong size):PreAllocStru(size){};
	virtual void Free(){PreAllocStru::Free();}
};





























































#else	//NOT_USE_IOCP************************************************************************
/*
	�����ǲ���ʹ��IOCPƽ̨���̳߳�ʵ��
*/
class Task;
class TaskQue;
class Thread;
class ThrdQue;
class TaskWait;

#define	NOTWAITSTATE	(0x3FFFFFFF)
//========ThreadPool==============================================================================
class ThreadPool										//�̳߳ض���
{
	friend class Thread;
public:
	static ThreadPool* CreatePool(long idle=0,long max=0x10,long taskmaxnum=0x100
		,int nPriority=THREAD_PRIORITY_NORMAL);
	void DestroyPool();
	void AddTask(Task *task);
	long WaitForTask(Task *task);
	long GetTaskCount();

	long getCurr()			{return m_curr;}			//���ܼ��Ҫ��
	long getCurrFree()		{return m_currfree;}		//���ܼ��Ҫ��

	InterLockedLong			m_taskexitflag;
private:
	ThreadPool(long idle,long max,long taskmaxnum,int nPriority);
	virtual ~ThreadPool();
	void ThreadExcute(Thread *);
	inline Task	* PoolProcess(Thread *l_myself);

private:
	InterLockedLong			m_exitflag;
	const int				m_nPriority;
	const long				m_max,m_idle;
	volatile long			m_curr,m_currfree;

	TaskQue	*	volatile	m_taskQue;
	ThrdQue	*	volatile	m_thrdQue;
};
//========Task==================================================================================
class Task{												//���������
	friend class ThreadPool;
	friend class TaskQue;
public:
	Task():__canwait(false),__ThisThread(0),__TaskQue(0),__nexttask(0),__taskwait(0)
	{
		__mtxtaskwait.Create(false);
		if(!__mtxtaskwait)
		{
			THROW_EXCP(excpSync,"�������������ϵͳͬ���������");
		}
	};
	bool				GetExitFlag();
protected:
	virtual				~Task(){}
	virtual	Task	*	Lastly(){Free();return 0;}
	HANDLE				GetHandle();					//��ȡ�����̵߳ľ��
	DWORD				GetThreadID();					//��ȡ�����̵߳ı�־ID
private:
	virtual	void		Free(){delete this;}
	virtual	long		Process()=0;
	inline	Task	*	TaskExec(Thread *ThisThread);
	long				WaitMe();

	Thread				*	volatile	__ThisThread;
	TaskQue				*	volatile	__TaskQue;

	bool					volatile	__canwait;
	Mutex								__mtxtaskwait;
	TaskWait			*	volatile	__taskwait;
	static PreAllocHeapPtr<TaskWait>	__freetaskwait;

	Task				*	volatile	__nexttask;
};
//========PreAllocTask======================================================================
class PreAllocTask:public PreAllocStru,public Task{
public:
	PreAllocTask(uLong size):PreAllocStru(size){};
	virtual void Free(){PreAllocStru::Free();}
};
#endif	//USING_IOCP
#pragma pack(pop)
_DBC_END
#endif