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

#ifndef USING_TAO				//使用Win32基本Platform SDK
#include <winsock2.h>			//确保调用新的WinSock2.2版本
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
	使用IOCP实现的线程池
*/
class Task;
class Thread;
class ThrdQue;

#define	NOTWAITSTATE	(0x3FFFFFFF)
//========ThreadPool==============================================================================
class ThreadPool										//线程池对象
{
	friend class Thread;
public:
	static ThreadPool* CreatePool(long running=0,long max=0x10,int nPriority=THREAD_PRIORITY_NORMAL);
	void DestroyPool();
	void AddTask(Task *task);

	long getCurr()			{return m_curr;}			//性能监测要求。
	long getCurrFree()		{return m_currfree;}		//性能监测要求。

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
class Task{												//任务抽象类
	friend class ThreadPool;
	friend class TaskQue;
public:
	Task():__canwait(false),__ThisThread(0)
	{
		__mtxtaskwait.Create(false);
		if(!__mtxtaskwait)
		{
			THROW_EXCP(excpSync,"任务对象建立操作系统同步对象错误");
		}
	};
	bool				GetExitFlag();
protected:
	virtual				~Task(){}
	virtual	Task	*	Lastly(){Free();return 0;}
	HANDLE				GetHandle();					//获取处理线程的句柄
	DWORD				GetThreadID();					//获取处理线程的标志ID
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
	下面是不能使用IOCP平台的线程池实现
*/
class Task;
class TaskQue;
class Thread;
class ThrdQue;
class TaskWait;

#define	NOTWAITSTATE	(0x3FFFFFFF)
//========ThreadPool==============================================================================
class ThreadPool										//线程池对象
{
	friend class Thread;
public:
	static ThreadPool* CreatePool(long idle=0,long max=0x10,long taskmaxnum=0x100
		,int nPriority=THREAD_PRIORITY_NORMAL);
	void DestroyPool();
	void AddTask(Task *task);
	long WaitForTask(Task *task);
	long GetTaskCount();

	long getCurr()			{return m_curr;}			//性能监测要求。
	long getCurrFree()		{return m_currfree;}		//性能监测要求。

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
class Task{												//任务抽象类
	friend class ThreadPool;
	friend class TaskQue;
public:
	Task():__canwait(false),__ThisThread(0),__TaskQue(0),__nexttask(0),__taskwait(0)
	{
		__mtxtaskwait.Create(false);
		if(!__mtxtaskwait)
		{
			THROW_EXCP(excpSync,"任务对象建立操作系统同步对象错误");
		}
	};
	bool				GetExitFlag();
protected:
	virtual				~Task(){}
	virtual	Task	*	Lastly(){Free();return 0;}
	HANDLE				GetHandle();					//获取处理线程的句柄
	DWORD				GetThreadID();					//获取处理线程的标志ID
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