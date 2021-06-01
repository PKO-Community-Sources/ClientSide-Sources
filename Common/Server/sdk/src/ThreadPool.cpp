//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// This program is modified recently by Zhang.Dabo in 2003.7
//=================================================================

#include <iostream>
#include <ios>
#include <LogStream.h>

#include "ThreadPool.h"
#include "Thread.h"

_DBC_USING
#ifdef USING_IOCP
//============================ThreadPoolʵ��=====================================
ThreadPool::ThreadPool(long running,long max,int nPriority)
	:m_max((max<0)?0:max)
	,m_running((running>m_max)?m_max:running)
	,m_nPriority(nPriority)
	,m_curr(0),m_currfree(0),m_exitflag(0),m_taskexitflag(0)
	,m_iocpHandle(0)
	,m_thrdQue(new ThrdQue())
{
	//��������
	if(m_nPriority !=THREAD_PRIORITY_NORMAL &&
		m_nPriority !=THREAD_PRIORITY_ABOVE_NORMAL &&
		m_nPriority !=THREAD_PRIORITY_BELOW_NORMAL &&
		m_nPriority !=THREAD_PRIORITY_HIGHEST &&
		m_nPriority !=THREAD_PRIORITY_IDLE &&
		m_nPriority !=THREAD_PRIORITY_LOWEST &&
		m_nPriority !=THREAD_PRIORITY_TIME_CRITICAL)
	{
		*const_cast<int*>(&m_nPriority)	=THREAD_PRIORITY_NORMAL;
	}
	if(m_max ==0)
	{
		SYSTEM_INFO systemInfo;
		GetSystemInfo( &systemInfo );
		m_max = (!m_max)?(2 * systemInfo.dwNumberOfProcessors + 2):m_max;
	}
	m_iocpHandle	=CreateIoCompletionPort(INVALID_HANDLE_VALUE , NULL , 0 , m_running);
	if(!m_iocpHandle)
	{
		THROW_EXCP(excp,"CompletionPort����ʧ�ܣ�");
	}
//�̶߳��г�ʼ��
	if(!m_thrdQue)
	{
		CloseHandle(m_iocpHandle);
		THROW_EXCP(excpMem,"�̳߳ط�������ڴ�ʧ��");
	}
	m_thrdQue->m_mtxUpdate.lock();
	try{
		Thread *l_newthrd;long l_err1count =0,l_err2count =0;
		for(int i=0;i<m_max;i++)
		{
			try
			{
				l_newthrd =new Thread(this);
			}catch(excpThrd &)
			{
				l_err1count++;
				if(l_err1count >=20)//�������20�η������,��ʱ1000����(1��)
					throw;
				Sleep(50);			//��Ϣ50�����ٷ���һ��"
				i--;
				continue;
			}
			if(!l_newthrd)
			{
				l_err2count++;
				if(l_err2count >=20)//�������20�η������,��ʱ1000����(1��)
					THROW_EXCP(excpMem,"�̳߳س�ʼ���̶߳��������ڴ�ʧ��");
				Sleep(50);//��Ϣ50�����ٷ���һ��"
				i--;
				continue;
			}
			m_thrdQue->InsThrd(l_newthrd);
		}
	}catch(...)
	{
		m_thrdQue->m_mtxUpdate.unlock();
		throw;
	}
	m_thrdQue->m_mtxUpdate.unlock();
};
ThreadPool::~ThreadPool()
{
	ThrdQue	*l_thq	=m_thrdQue;
	if(l_thq)
	{
		m_thrdQue	=0;
		delete l_thq;
	}
};
ThreadPool* ThreadPool::CreatePool(long running,long max,int nPriority)
{
	return new ThreadPool(running,max,nPriority);
}
void ThreadPool::DestroyPool()
{
	m_taskexitflag	=1;
	m_exitflag		=1;
	for(long WaitCount =0;m_thrdQue && m_thrdQue->m_thread&&(WaitCount<40);WaitCount++)		//�ȴ��߳̽���
	{//�ȴ�����10����ǿ���ж������߳�
		Sleep(100);					//��Ϣ100������
	}
	if(m_thrdQue)
	{
		m_thrdQue->m_mtxUpdate.lock();
		try{
			Thread	*l_th;
			while(m_thrdQue &&m_thrdQue->m_thread)
			{
				l_th =m_thrdQue->m_thread;
				if(l_th&&l_th->m_handle)
				{
					TerminateThread(l_th->m_handle,-1);
					m_thrdQue->DelThrd(l_th);
					m_curr--;							//m_curr--
					m_currfree -=l_th->m_freeflag?1:0;
					delete l_th;
				}
			}
		}catch(...){}
		m_thrdQue->m_mtxUpdate.unlock();
	}
	delete this;
}
//==============================================================================
inline Task	*	Task::TaskExec(Thread *ThisThread)
{
	TaskWait	*l_taskwait;
	long l_retval	=-10000;

	__ThisThread =ThisThread;

	try{l_retval =Process();}catch(...){}

	__mtxtaskwait.lock();
	try{
		__canwait	=false;
		while(__taskwait){
			__taskwait->m_retval	=l_retval;
			l_taskwait	=__taskwait->next;
			__taskwait->m_semWait.unlock();
			__taskwait =l_taskwait;
		}				//ֱ��	m_taskwait	=0;
	}catch(...){}
	__mtxtaskwait.unlock();

	__ThisThread =0;
	Task	* l_nextTask	=0;
	try{l_nextTask	=Lastly();}catch(...){}
	return l_nextTask;
}
//==============================================================================
inline void ThrdQue::InsThrd(Thread *th)
{
	th->m_last	=0;
	th->m_next	=m_thread;
	if(m_thread)
	{
		m_thread->m_last =th;
	}
	m_thread =th;
}
inline void ThrdQue::DelThrd(Thread *th)
{
	if(th->m_next){th->m_next->m_last =th->m_last;}
	if(th->m_last){th->m_last->m_next =th->m_next;}
	if(m_thread ==th){m_thread =th->m_next;}
}
//==============================================================================
DWORD WINAPI Thread::ThreadProc(LPVOID lpParameter)//Ӳ�̹߳���(����ThreadExcute)
{
	Thread		*l_myself =reinterpret_cast<Thread*>(lpParameter);
	ThreadPool	*l_pool =l_myself->GetPool();
	SetThreadPriority(GetCurrentThread(),l_pool->m_nPriority );
	l_pool->ThreadExcute(l_myself);
	SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_NORMAL);
	return 0;
};
//==============================================================================
void ThreadPool::ThreadExcute(Thread *l_myself)
{
	uLong			NumberOfBytes;
	int				CompletionKey;
	LPOVERLAPPED	lpOverlapped;
	BOOL			bStatus;

	Task	*	l_task	=0;
	while(true){
		NumberOfBytes	=INFINITE;
		lpOverlapped	=0;
		bStatus	=GetQueuedCompletionStatus(m_iocpHandle,&NumberOfBytes,&CompletionKey,&lpOverlapped,2000);
		if(!bStatus && !lpOverlapped && GetLastError() ==WAIT_TIMEOUT)
		{
			continue;
		}
		if(lpOverlapped)
		{
			if(l_task	=CONTAINING_RECORD(lpOverlapped,Task,__overlapped))
			{
				try
				{
					l_task->__NumberOfBytes	=NumberOfBytes;
					l_task->__CompletionKey	=CompletionKey;
					while(l_task =l_task->TaskExec(l_myself)){};
				}catch(...){}
			}
		}
///
		if(l_task)
		{
		}else if(m_exitflag)
		{
			m_thrdQue->m_mtxUpdate.lock();
			try{
				if(l_myself->m_freeflag)
				{
					l_myself->m_freeflag	=false;
					m_currfree--;
					m_curr--;
					m_thrdQue->DelThrd(l_myself);
					CloseHandle(l_myself->m_handle);
					try{
					delete l_myself;
					}catch(...){}
				}
			}catch(...){}
			m_thrdQue->m_mtxUpdate.unlock();
			break;
		}else
		{
			CloseHandle(l_myself->m_handle);
			delete l_myself;
			break;
		}


		m_thrdQue->m_mtxUpdate.lock();
		try{
			l_myself->m_freeflag =true;
			m_currfree ++;
		}catch(...){}
		m_thrdQue->m_mtxUpdate.unlock();
	}
}
//==============================================================================
void ThreadPool::AddTask(Task *task)
{
	m_taskQue->AddTask(task);
}
//============================Threadʵ��=====================================
Thread::Thread(ThreadPool *pool):m_pool(pool),m_freeflag(true)
{
	m_handle =CreateThread(0,0,ThreadProc,this,0,&m_threadid);
	if(!m_handle) THROW_EXCP(excpThrd,"�̶߳���������ϵͳ�߳�ʧ��");

	m_pool->m_curr++;
	m_pool->m_currfree++;
};
//==============================================================================
Thread::~Thread()
{
	//�̶߳����Ǳ��Լ����߳�ɾ�������õȴ��߳̽�����
	//CloseHandle(this->m_handle);
};























































































#else
//============================ThreadPoolʵ��=====================================
ThreadPool::ThreadPool(long idle,long max,long taskmaxnum,int nPriority)
	:m_max((max<1)?1:max)
	,m_idle((idle<1)?1:((idle>m_max)?m_max:idle))
	,m_nPriority(nPriority)
	,m_curr(0),m_currfree(0),m_exitflag(0),m_taskexitflag(0)
	,m_taskQue(new TaskQue(taskmaxnum))
	,m_thrdQue(new ThrdQue())
{
	if(m_nPriority !=THREAD_PRIORITY_NORMAL &&
		m_nPriority !=THREAD_PRIORITY_ABOVE_NORMAL &&
		m_nPriority !=THREAD_PRIORITY_BELOW_NORMAL &&
		m_nPriority !=THREAD_PRIORITY_HIGHEST &&
		m_nPriority !=THREAD_PRIORITY_IDLE &&
		m_nPriority !=THREAD_PRIORITY_LOWEST &&
		m_nPriority !=THREAD_PRIORITY_TIME_CRITICAL)
	{
		*const_cast<int*>(&m_nPriority)	=THREAD_PRIORITY_NORMAL;
	}
//�̶߳��г�ʼ��
	if(!m_taskQue||!m_thrdQue)
	{
		delete m_thrdQue;
		delete m_taskQue;
		THROW_EXCP(excpMem,"�̳߳ط�������ڴ�ʧ��");
	}
	m_thrdQue->m_mtxPool.lock();
	try{
		m_thrdQue->m_mtxUpdate.lock();
		try{
			Thread *l_newthrd;long l_err1count =0,l_err2count =0;
			for(int i=0;i<m_idle;i++)
			{
				try
				{
					l_newthrd =new Thread(this);
				}catch(excpThrd &)
				{
					l_err1count++;
					if(l_err1count >=20)//�������20�η������,��ʱ1000����(1��)
						throw;
					Sleep(50);				//��Ϣ50�����ٷ���һ��"
					i--;
					continue;
				}
				if(!l_newthrd)
				{
					l_err2count++;
					if(l_err2count >=20)//�������20�η������,��ʱ1000����(1��)
						THROW_EXCP(excpMem,"�̳߳س�ʼ���̶߳��������ڴ�ʧ��");
					Sleep(50);//��Ϣ50�����ٷ���һ��"
					i--;
					continue;
				}
				m_thrdQue->InsThrd(l_newthrd);
			}
		}catch(...)
		{
			m_thrdQue->m_mtxUpdate.unlock();
			throw;
		}
		m_thrdQue->m_mtxUpdate.unlock();
	}catch(...)
	{
		m_thrdQue->m_mtxPool.unlock();
		throw;
	}
	m_thrdQue->m_mtxPool.unlock();
};
ThreadPool::~ThreadPool()
{
	ThrdQue	*l_thq	=m_thrdQue;
	if(l_thq)
	{
		m_thrdQue	=0;
		delete l_thq;
	}
	TaskQue	*l_tkq	=m_taskQue;
	if(l_tkq)
	{
		m_taskQue	=0;
		delete l_tkq;
	}
};
ThreadPool* ThreadPool::CreatePool(long idle,long max,long taskmaxnum,int nPriority)
{
	return new ThreadPool(idle,max,taskmaxnum,nPriority);
}
void ThreadPool::DestroyPool()
{
	m_taskexitflag	=1;
	m_exitflag		=1;
	for(long WaitCount =0;m_thrdQue && m_thrdQue->m_thread&&(WaitCount<40);WaitCount++)		//�ȴ��߳̽���
	{//�ȴ�����10����ǿ���ж������߳�
		Sleep(100);					//��Ϣ100������
	}
	if(m_thrdQue)
	{
		m_thrdQue->m_mtxUpdate.lock();
		try{
			Thread	*l_th;
			while(m_thrdQue &&m_thrdQue->m_thread)
			{
				l_th =m_thrdQue->m_thread;
				if(l_th&&l_th->m_handle)
				{
					TerminateThread(l_th->m_handle,-1);
					m_thrdQue->DelThrd(l_th);
					m_curr--;							//m_curr--
					m_currfree -=l_th->m_freeflag?1:0;
					delete l_th;
				}
			}
		}catch(...){}
		m_thrdQue->m_mtxUpdate.unlock();
	}
	delete this;
}
//==============================================================================
inline Task	*	Task::TaskExec(Thread *ThisThread)
{
	TaskWait	*l_taskwait;
	long l_retval	=-10000;

	__ThisThread =ThisThread;

	try{l_retval =Process();}catch(...){}

	__mtxtaskwait.lock();
	try{
		__canwait	=false;
		while(__taskwait){
			__taskwait->m_retval	=l_retval;
			l_taskwait	=__taskwait->next;
			__taskwait->m_semWait.unlock();
			__taskwait =l_taskwait;
		}				//ֱ��	m_taskwait	=0;
	}catch(...){}
	__mtxtaskwait.unlock();

	__ThisThread =0;
	Task	* l_nextTask	=0;
	try{l_nextTask	=Lastly();}catch(...){}
	return l_nextTask;
}
//==============================================================================
inline void ThrdQue::InsThrd(Thread *th)
{
	th->m_last	=0;
	th->m_next	=m_thread;
	if(m_thread)
	{
		m_thread->m_last =th;
	}
	m_thread =th;
}
inline void ThrdQue::DelThrd(Thread *th)
{
	if(th->m_next){th->m_next->m_last =th->m_last;}
	if(th->m_last){th->m_last->m_next =th->m_next;}
	if(m_thread ==th){m_thread =th->m_next;}
}
//==============================================================================
DWORD WINAPI Thread::ThreadProc(LPVOID lpParameter)//Ӳ�̹߳���(����ThreadExcute)
{
	Thread		*l_myself =reinterpret_cast<Thread*>(lpParameter);
	LogLine l_line(g_dbclog);
	l_line<<newln<<"�߳�:["<<l_myself->m_threadid<<"]����!";
	l_line<<endln;
	ThreadPool	*l_pool =l_myself->GetPool();
	SetThreadPriority(GetCurrentThread(),l_pool->m_nPriority );
	l_pool->ThreadExcute(l_myself);
	SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_NORMAL);
	l_line<<newln<<"�߳�:["<<::GetCurrentThreadId()<<"]�˳�!";
	l_line<<endln;
	return 0;
};
//==============================================================================
void ThreadPool::ThreadExcute(Thread *l_myself)
{
	Task	*	l_task	=0;
	while(true){
		m_thrdQue->m_mtxPool.lock();
		try{
			l_task	=PoolProcess(l_myself);
		}catch(...){}
		m_thrdQue->m_mtxPool.unlock();
		if(l_task)
		{
			try
			{
				while(l_task =l_task->TaskExec(l_myself)){};
			}catch(...){}
		}else if(m_exitflag)
		{
			m_thrdQue->m_mtxUpdate.lock();
			try{
				if(l_myself->m_freeflag)
				{
					l_myself->m_freeflag	=false;
					m_currfree--;
					m_curr--;
					m_thrdQue->DelThrd(l_myself);
					CloseHandle(l_myself->m_handle);
					try{
					delete l_myself;
					}catch(...){}
				}
			}catch(...){}
			m_thrdQue->m_mtxUpdate.unlock();
			break;
		}else
		{
			CloseHandle(l_myself->m_handle);
			delete l_myself;
			break;
		}


		m_thrdQue->m_mtxUpdate.lock();
		try{
			l_myself->m_freeflag =true;
			m_currfree ++;
		}catch(...){}
		m_thrdQue->m_mtxUpdate.unlock();
	}
}
//==============================================================================
inline Task	* ThreadPool::PoolProcess(Thread *l_myself)
{
	uLong			l_howidle	=0;
	Task		*	l_rettask	=0;
	Thread		*	l_newthrd;

	while(true)
	{
		if(!l_rettask)
		{
			m_thrdQue->m_mtxUpdate.lock();
			try
			{
				l_howidle =((m_currfree<m_idle)&&(m_curr<m_max))
					?((m_currfree<long((0.618*m_idle) +0.5))?0:50)
					:((m_currfree==m_idle)?1000:50);
			}catch(...)
			{
				l_howidle =10;
			}
			m_thrdQue->m_mtxUpdate.unlock();

			l_rettask =m_taskQue->GetTask(l_howidle);

		}
		if(m_taskexitflag)
		{
			if(l_rettask)
			{
				while(l_rettask	=l_rettask->Lastly()){};
			}else if(m_exitflag)
			{
				break;
			}else
			{
				Sleep(3);
			}
		}else if(l_rettask)
		{
			m_thrdQue->m_mtxUpdate.lock();
			try
			{
				if ((m_currfree<=1)&&(m_curr<m_max))
				{
					l_newthrd =new Thread(this);
					if(l_newthrd)
					{
						m_thrdQue->InsThrd(l_newthrd);
					}
				}
				m_currfree --;
				l_myself->m_freeflag	=false;
			}catch(excp &e)
			{
				std::cout<<e.what()<<std::endl;		//����־�м�¼����
			}catch(...)
			{
			}
			m_thrdQue->m_mtxUpdate.unlock();
			break;
		}else
		{
			m_thrdQue->m_mtxUpdate.lock();
			try
			{
				if((m_currfree>m_idle)&&(l_howidle>=50))
				{
					l_myself->m_freeflag	=false;
					m_currfree--;
					m_curr--;
					m_thrdQue->DelThrd(l_myself);
					m_thrdQue->m_mtxUpdate.unlock();
					break;
				}
				if ((m_currfree<m_idle)&&(m_curr<m_max))
				{
					l_newthrd =new Thread(this);
					if(l_newthrd)
					{
						m_thrdQue->InsThrd(l_newthrd);
					}
				}
			}catch(excp &e)					//����־�м�¼����
			{
				std::cout<<e.what()<<std::endl;
			}catch(...)
			{
			}
			m_thrdQue->m_mtxUpdate.unlock();
		}
	}
	return l_rettask;
};
//==============================================================================
long ThreadPool::WaitForTask(Task *task)
{
	return task->WaitMe();
};
void ThreadPool::AddTask(Task *task)
{
	m_taskQue->AddTask(task);
}
long ThreadPool::GetTaskCount()
{
	return m_taskQue->GetTaskCount();
}
//============================Threadʵ��=====================================
Thread::Thread(ThreadPool *pool):m_pool(pool),m_freeflag(true)
{
	m_handle =CreateThread(0,0,ThreadProc,this,0,&m_threadid);
	if(!m_handle) THROW_EXCP(excpThrd,"�̶߳���������ϵͳ�߳�ʧ��");

	m_pool->m_curr++;
	m_pool->m_currfree++;
};
//==============================================================================
Thread::~Thread()
{
	//�̶߳����Ǳ��Լ����߳�ɾ�������õȴ��߳̽�����
	//CloseHandle(this->m_handle);
};
#endif
