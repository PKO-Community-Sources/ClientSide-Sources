//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// This program is modified recently by Zhang.Dabo in 2003.7
//=================================================================
#ifndef PREALLOC_H
#define PREALLOC_H

#ifndef USING_TAO			//ʹ��Win32����Platform SDK
#include <winsock2.h>		//ȷ�������µ�WinSock2.2�汾
#include <windows.h>
#else
#include "TAOSpecial.h"
#endif

#include "DBCCommon.h"
#include "excp.h"

//===Ԥ����ṹ========

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)
//=======PreAllocStru=======================================================================
class PreAllocStru
{
protected:
	PreAllocStru(uLong):__preAllocHeap(0),__preAllocHeapPtr(0),__nextHeapItem(0){}
	virtual ~PreAllocStru(){}
public:
	bool			IsFree(){return !__preAllocHeap;}
	virtual	void	Free();
	virtual	uLong	Size();//������Override���ص�ǰ�ṹ����Ļ���ߴ�

	template<class T>	friend class PreAllocHeap;
	template<class T>	friend class PreAllocHeapPtr;
private:
	virtual void Initially(){}
	virtual void Finally(){}
	PreAllocHeap<PreAllocStru>		*	volatile __preAllocHeap;
	PreAllocHeapPtr<PreAllocStru>	*	volatile __preAllocHeapPtr;
	PreAllocStru					*	volatile __nextHeapItem;
};
//==========PreAllocHeap====================================================================
struct __FIFOQu_
{
	PreAllocStru	*	volatile head;
	PreAllocStru	*	volatile tail;
	uLong				volatile free;		//������;
};
template <class T>
class PreAllocHeap
{
	friend class PreAllocStru;
	template <class T> friend class PreAllocHeapPtr;
public:
	PreAllocHeap(uLong unitsize)
		:m_unitsize(unitsize?unitsize:1),m_initnum(0)
		,m_unitflag(unitsize?unitsize:1)
		,m_numptr(0),m_freestru(0),m_usednum(0)
	{}
	PreAllocHeap(uLong unitsize,uLong initnum)
		:m_unitsize(unitsize?unitsize:1),m_initnum(initnum?initnum:1)
		,m_unitflag(unitsize?unitsize:1)
		,m_numptr(0),m_freestru(0),m_usednum(0)
	{}
	virtual ~PreAllocHeap(){Finally();*const_cast<uLong*>(&m_unitflag)=0;};

	void Init(uLong bufnumlist[])		//bufnumlistΪһ0��β����
	{
		MutexArmor l_lock(m_mtxfreeacc);
		PreAllocStru *t;
		for(uLong k=0;bufnumlist[k];k++)
		{
			for(uLong i=0;i<bufnumlist[k];i++)
			{
				t	=newT(m_unitsize*(k+1));
				t->__preAllocHeap	=0;
				InsertT(t,m_unitsize*(k+1));
			}
		}
		l_lock.unlock();
	};
	void Init()
	{
		MutexArmor l_lock(m_mtxfreeacc);
		if(!m_numptr)
		{
			m_numptr =1;
			m_freestru =new __FIFOQu_[m_numptr];
			if(!m_freestru)
			{
				THROW_EXCP(excpMem,"Ԥ������ڴ����ʧ��");
			}
			m_freestru[0].head	=m_freestru[0].tail =0;
			m_freestru[0].free	=0;
		}
		PreAllocStru *t;
		for(uLong i=0;i<m_initnum;i++)
		{
			t	=newT(m_unitsize);
			t->__preAllocHeap	=0;
			InsertT(t,m_unitsize);
		}
		l_lock.unlock();
	};
	T *Get(uLong size =0)
	{
		if(!m_unitsize)
		{
			return 0;
		}
		PreAllocStru	*l_t =0;
		if(!size ||m_initnum)size =m_unitsize;

		uLong l_reqpos =(size+m_unitsize-1)/m_unitsize;

		MutexArmor l_lock(m_mtxfreeacc);
		if((l_reqpos <=m_numptr)&&m_freestru[l_reqpos-1].head)
		{
			l_t =m_freestru[l_reqpos-1].head;
			m_freestru[l_reqpos -1].head =l_t ->__nextHeapItem;
			if(!m_freestru[l_reqpos -1].head)
			{
				m_freestru[l_reqpos -1].tail	=0;
			}
			m_freestru[l_reqpos -1].free --;
		}
		l_lock.unlock();

		if(!l_t)	l_t =newT(m_unitsize *l_reqpos);
		l_t->__preAllocHeap	=reinterpret_cast<PreAllocHeap<PreAllocStru>*>(this);
		l_t->__nextHeapItem	=0;

		++m_usednum;
		try{l_t->Initially();}catch(...){throw;}

		return	static_cast<T*>(l_t);
	}
	uLong GetUsedNum()const{return m_usednum;}
	uLong GetUnitSize()const{return m_unitsize;}
private:
	PreAllocHeap<T> & operator<<(PreAllocStru *t)
	{
		uLong	l_size	=t->Size();

		MutexArmor l_lock(m_mtxfreeacc);
		if(t->__preAllocHeap==reinterpret_cast<PreAllocHeap<PreAllocStru>*>(this))
		{
			t->__preAllocHeap	=0;
			l_lock.unlock();

			try{
				t->Finally();
			}catch(...)
			{
				--m_usednum;

				InsertT(t,l_size);
				throw;
			}
			--m_usednum;

			InsertT(t,l_size);
		}
		return *this;
	}
	void InsertT(PreAllocStru *t,uLong size)
	{
		MutexArmor l_lock(m_mtxfreeacc);
		if(!m_unitflag)
		{
			l_lock.unlock();
			delete t;
			return;
		}
		uLong l_pos =(size+m_unitsize-1)/m_unitsize;
		if(l_pos >m_numptr)
		{
			uLong l_num =(l_pos+15)/16;
			__FIFOQu_* l_ptr =new __FIFOQu_[l_num*=16];
			if(!l_ptr)
			{
				THROW_EXCP(excpMem,"Ԥ������ڴ����ʧ��");
			}
			MemCpy((char*)l_ptr,(char*)m_freestru,m_numptr*sizeof(__FIFOQu_));
			MemSet((char*)(l_ptr+m_numptr),0,(l_num-m_numptr)*sizeof(__FIFOQu_));//�¿��ٵĿռ���Ϊ0;
			delete []m_freestru;
			m_freestru =l_ptr;
			m_numptr =l_num;
		}
		if(m_freestru[l_pos -1].tail)
		{
			m_freestru[l_pos -1].tail->__nextHeapItem =t;
		}else
		{
			m_freestru[l_pos -1].head	=t;
		}
		m_freestru[l_pos -1].tail =t;
		m_freestru[l_pos -1].free++;
		t->__nextHeapItem	=0;
		l_lock.unlock();
	};
	PreAllocStru* newT(uLong size)
	{
		PreAllocStru	*l_t =new T(size);
		if(l_t)
		{
			l_t->__preAllocHeap	=reinterpret_cast<PreAllocHeap<PreAllocStru>*>(this);
			if(l_t->Size()<size)
			{
				delete l_t;
				THROW_EXCP(excpMem,"Ԥ������ڴ����ʧ��");
			}
		}else
		{
			THROW_EXCP(excpMem,"Ԥ������ڴ����ʧ��");
		}
		return l_t;
	}
	void Finally()
	{
		MutexArmor l_lock(m_mtxfreeacc);
		if(m_freestru)
		{
			PreAllocStru	*l_struct;
			for(uLong i=0;i<m_numptr;i++)
			{
				while(m_freestru[i].head)
				{
					l_struct	=m_freestru[i].head;
					m_freestru[i].head	=l_struct->__nextHeapItem;

					l_struct->__nextHeapItem	=0;
					try{
						delete l_struct;
					}catch(...)
					{
						throw;//��ǰû��
					}
				}
				m_freestru[i].tail	=0;
			}
			delete []m_freestru;
			m_freestru =0;
			m_numptr=0;
		}
		l_lock.unlock();
	}
private:
	cuLong				m_initnum;
	cuLong				m_unitsize,m_unitflag;
	struct
	{
		Mutex					m_mtxfreeacc;
		uLong		 volatile	m_numptr;
		__FIFOQu_	*volatile	m_freestru;
	};
	InterLockedLong				m_usednum;
};
//=======PreAllocHeapPtr=======================================================================
template <class T>
class PreAllocHeapPtr
{
	friend class PreAllocStru;
public:
	PreAllocHeapPtr(uLong unitsize):m_unitsize(unitsize?unitsize:1),m_initnum(0),m_ptr(0)
	{
		if(!m_mtxptr.Create(false))THROW_EXCP(excpSync,"Ԥ�����ָ������ͬ��������ʧ��");
	};
	PreAllocHeapPtr(uLong unitsize,uLong initnum):m_unitsize(unitsize?unitsize:1),m_initnum(initnum?initnum:1),m_ptr(0)
	{
		if(!m_mtxptr.Create(false))THROW_EXCP(excpSync,"Ԥ�����ָ������ͬ��������ʧ��");
	};
	~PreAllocHeapPtr()
	{
		MutexArmor l_lockPtr(m_mtxptr);
		if(m_ptr)
		{
			delete m_ptr;
			m_ptr	=0;
		}
		l_lockPtr.unlock();
	};
	T *Get(uLong size =0)
	{
		if(!m_ptr)
		{
			if(m_initnum)
			{
				Init();
			}else
			{
				uLong l_bufnumlist[] ={1,0};
				Init(l_bufnumlist);
			}
		}
		T	*l_t	=m_ptr->Get(size);
		l_t->__preAllocHeapPtr	=reinterpret_cast<PreAllocHeapPtr<PreAllocStru>*>(this);
		return l_t;
	}
	bool Init(uLong bufnumlist[])
	{
		if(m_ptr)return false;
		MutexArmor l_lockPtr(m_mtxptr);
		try
		{
			if(!m_ptr)
			{
				PreAllocHeap<T> *l_ptr =new PreAllocHeap<T>(m_unitsize);
				if(!l_ptr)THROW_EXCP(excpMem,"Ԥ������ڴ����ʧ��");
				l_ptr->Init(bufnumlist);
				m_ptr	=l_ptr;
			}
		}catch(...)
		{
			delete m_ptr;
			m_ptr	=0;
			l_lockPtr.unlock();
			throw;
		}
		l_lockPtr.unlock();
		return true;
	}
	uLong	GetUnitSize()const{return m_ptr?m_ptr->GetUnitSize():m_unitsize;}
	uLong	GetUsedNum()const{return m_ptr?m_tr->GetUsedNum():0;}

	uLong	GetBufNum()const{return m_ptr?m_ptr->m_numptr:0;}
	const __FIFOQu_ *GetPerformance()const{return m_ptr?m_ptr->m_freestru:0;}
private:
	PreAllocHeapPtr<T> & operator<<(PreAllocStru *t)
	{
		if(t->__preAllocHeapPtr==reinterpret_cast<PreAllocHeapPtr<PreAllocStru>*>(this))
		{
			MutexArmor l_lock(m_mtxptr);
			t->__preAllocHeapPtr	=0;
			if(m_ptr)
			{
				*m_ptr<<t;
			}else
			{
				l_lock.unlock();
				try{
					t->__preAllocHeap	=0;
					t->Finally();
				}catch(...)
				{
					delete t;
					throw;
				}
				delete t;
			}
		}
		return *this;
	}
	void Init()
	{
		if(m_ptr)return;
		MutexArmor l_lockPtr(m_mtxptr);
		try
		{
			if(!m_ptr)
			{
				PreAllocHeap<T> *l_ptr =new PreAllocHeap<T>(m_unitsize,m_initnum);
				if(!l_ptr)THROW_EXCP(excpMem,"Ԥ������ڴ����ʧ��");
				l_ptr->Init();
				m_ptr	=l_ptr;
			}
		}catch(...)
		{
			delete m_ptr;
			m_ptr	=0;
			l_lockPtr.unlock();
			throw;
		}
		l_lockPtr.unlock();
	}
	uLong				volatile	m_initnum;
	uLong				volatile	m_unitsize;
	Mutex							m_mtxptr;
	PreAllocHeap<T>		*volatile	m_ptr;
};

#pragma pack(pop)
_DBC_END
#include "dstring.h"

#endif
