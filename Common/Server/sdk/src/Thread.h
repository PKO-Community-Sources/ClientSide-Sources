#ifndef THREAD_H
#define THREAD_H

#include "DBCCommon.h"
#include "PreAlloc.h"

_DBC_BEGIN
#ifdef USING_IOCP
/*
	ʹ��IOCPʵ�ֵ��̳߳�
*/
//=======Thread==============================================================================
class Thread											//�̶߳���
{
	friend class ThreadPool;
	friend class ThrdQue;
	friend class Task;
protected:
	Thread(ThreadPool *threadpool);
	~Thread();

	ThreadPool *GetPool()	{return m_pool;};//��ȡ�߳����ڵĳض���
private:
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);
	HANDLE GetHandle()		{return m_handle;};			//��ȡ�̵߳�HANDLE���
	DWORD  GetThreadID()	{return m_threadid;};		//��ȡ�̵߳�ID��־

	bool			volatile	m_freeflag;				//�߳��Ƿ���б�־
	HANDLE						m_handle;				//�̵߳�HANDLE���
	DWORD						m_threadid;				//�̵߳�ID��־
	ThreadPool		*volatile	m_pool;					//��ָ��
	Thread	*volatile	m_last,*volatile	m_next;		//����ָ�룬m_last������һ����m_next������һ��
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
			THROW_EXCP(excpSync,"�̳߳ؽ�������ϵͳͬ������ʧ��");
		}
	};
	inline void InsThrd(Thread *);
	inline void DelThrd(Thread *);

	Thread 	*volatile	m_thread;				//����ӵ�е��̶߳���
	Mutex				m_mtxUpdate;
};






























































#else
//========TaskQue======================================================================
class TaskQue
{
	friend class ThreadPool;
	TaskQue(long max):m_max(max),m_taskcount(0),m_head(0),m_tail(0)
	{																			//��������г�ʼ��
		m_mtxQue.Create(true);
		try
		{
			m_semQueAdd.Create(0,m_max,0);
			m_semQueGet.Create(m_max,m_max,0);
			if(!m_mtxQue||!m_semQueAdd||!m_semQueGet)
				THROW_EXCP(excpSync,"�̳߳ؽ������������ز���ϵͳͬ������ʧ��");
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
class TaskWait:public PreAllocStru						//����ȴ��ļ�¼�ṹ
{
	friend class Task;
public:
	TaskWait(uLong size =0):PreAllocStru(size)
	{
		if(!m_semWait.Create(0,1,0))THROW_EXCP(excpSync,"����ȴ��ṹ��������ϵͳͬ����ʧ��");
	};
private:
	Sema					m_semWait;
	long		volatile	m_retval;
	TaskWait	*volatile	next;		//����Ҫά��������ָ��
};
//=======Thread==============================================================================
class Thread											//�̶߳���
{
	friend class ThreadPool;
	friend class ThrdQue;
	friend class Task;
protected:
	Thread(ThreadPool *threadpool);
	~Thread();

	ThreadPool *GetPool()	{return m_pool;};//��ȡ�߳����ڵĳض���
private:
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);
	HANDLE GetHandle()		{return m_handle;};			//��ȡ�̵߳�HANDLE���
	DWORD  GetThreadID()	{return m_threadid;};	//��ȡ�̵߳�ID��־

	bool			volatile	m_freeflag;						//�߳��Ƿ���б�־
	HANDLE						m_handle;						//�̵߳�HANDLE���
	DWORD						m_threadid;						//�̵߳�ID��־
	ThreadPool		*volatile	m_pool;							//��ָ��
	Thread	*volatile	m_last,*volatile	m_next;		//����ָ�룬m_last������һ����m_next������һ��
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
			THROW_EXCP(excpSync,"�̳߳ؽ�������ϵͳͬ������ʧ��");
		}
	};
	inline void InsThrd(Thread *);
	inline void DelThrd(Thread *);

	Thread 	*volatile	m_thread;				//����ӵ�е��̶߳���
	Mutex				m_mtxPool,m_mtxUpdate;
};

#endif
_DBC_END

#endif
