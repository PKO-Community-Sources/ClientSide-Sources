#ifndef THREAD_H
#define THREAD_H

#include "DBCCommon.h"
#include "PreAlloc.h"

_DBC_BEGIN
#ifdef USING_IOCP
/*
	使用IOCP实现的线程池
*/
//=======Thread==============================================================================
class Thread											//线程对象
{
	friend class ThreadPool;
	friend class ThrdQue;
	friend class Task;
protected:
	Thread(ThreadPool *threadpool);
	~Thread();

	ThreadPool *GetPool()	{return m_pool;};//获取线程所在的池对象
private:
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);
	HANDLE GetHandle()		{return m_handle;};			//获取线程的HANDLE句柄
	DWORD  GetThreadID()	{return m_threadid;};		//获取线程的ID标志

	bool			volatile	m_freeflag;				//线程是否空闲标志
	HANDLE						m_handle;				//线程的HANDLE句柄
	DWORD						m_threadid;				//线程的ID标志
	ThreadPool		*volatile	m_pool;					//池指针
	Thread	*volatile	m_last,*volatile	m_next;		//队列指针，m_last队列上一个，m_next队列下一个
};
//========PoolHelper==============================================================================
class ThrdQue
{
	friend class ThreadPool;
	ThrdQue():m_thread(0)
	{
		m_mtxPool.Create(false);
		m_mtxUpdate.Create(false);
		if(!m_mtxPool||!m_mtxUpdate)
		{
			THROW_EXCP(excpSync,"线程池建立操作系统同步对象失败");
		}
	};
	inline void InsThrd(Thread *);
	inline void DelThrd(Thread *);

	Thread 	*volatile	m_thread;				//池所拥有的线程队列
	Mutex				m_mtxUpdate;
};






























































#else
//========TaskQue======================================================================
class TaskQue
{
	friend class ThreadPool;
	TaskQue(long max):m_max(max),m_taskcount(0),m_head(0),m_tail(0)
	{																			//池任务队列初始化
		m_mtxQue.Create(true);
		try
		{
			m_semQueAdd.Create(0,m_max,0);
			m_semQueGet.Create(m_max,m_max,0);
			if(!m_mtxQue||!m_semQueAdd||!m_semQueGet)
				THROW_EXCP(excpSync,"线程池建立任务队列相关操作系统同步对象失败");
		}catch(...)
		{
			m_mtxQue.unlock();
			throw;
		}
		m_mtxQue.unlock();
	};
	~TaskQue();
	void		AddTask(Task *task);
	Task	*	GetTask(uLong l_howidle);
	long		GetTaskCount(){return m_taskcount;}

	Mutex					m_mtxQue;
	long		const		m_max;
	long		volatile	m_taskcount;
	Task	*	volatile	m_head;
	Task	*	volatile	m_tail;
	Sema					m_semQueAdd;
	Sema					m_semQueGet;
};
//=========TaskWait==================================================================================
class TaskWait:public PreAllocStru						//任务等待的纪录结构
{
	friend class Task;
public:
	TaskWait(uLong size =0):PreAllocStru(size)
	{
		if(!m_semWait.Create(0,1,0))THROW_EXCP(excpSync,"任务等待结构建立操作系统同步器失败");
	};
private:
	Sema					m_semWait;
	long		volatile	m_retval;
	TaskWait	*volatile	next;		//不需要维护的链表指针
};
//=======Thread==============================================================================
class Thread											//线程对象
{
	friend class ThreadPool;
	friend class ThrdQue;
	friend class Task;
protected:
	Thread(ThreadPool *threadpool);
	~Thread();

	ThreadPool *GetPool()	{return m_pool;};//获取线程所在的池对象
private:
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);
	HANDLE GetHandle()		{return m_handle;};			//获取线程的HANDLE句柄
	DWORD  GetThreadID()	{return m_threadid;};	//获取线程的ID标志

	bool			volatile	m_freeflag;						//线程是否空闲标志
	HANDLE						m_handle;						//线程的HANDLE句柄
	DWORD						m_threadid;						//线程的ID标志
	ThreadPool		*volatile	m_pool;							//池指针
	Thread	*volatile	m_last,*volatile	m_next;		//队列指针，m_last队列上一个，m_next队列下一个
};
//========PoolHelper==============================================================================
class ThrdQue
{
	friend class ThreadPool;
	ThrdQue():m_thread(0)
	{
		m_mtxPool.Create(false);
		m_mtxUpdate.Create(false);
		if(!m_mtxPool||!m_mtxUpdate)
		{
			THROW_EXCP(excpSync,"线程池建立操作系统同步对象失败");
		}
	};
	inline void InsThrd(Thread *);
	inline void DelThrd(Thread *);

	Thread 	*volatile	m_thread;				//池所拥有的线程队列
	Mutex				m_mtxPool,m_mtxUpdate;
};

#endif
_DBC_END

#endif
