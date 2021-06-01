//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// This program is modified recently by Zhang.Dabo in 2003.7
//=================================================================
#ifndef ROBJECT_H
#define ROBJECT_H

#include "DBCCommon.h"
#include "excp.h"

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)

//=================================================================
//common ancestry class define
template<bool sync =false>
class robject								//�������ü������з�������ԭʼ����
{
protected:
	robject():_m_RefCount(0)
	{
		if(sync&&!_m_SyncMutex.Create(false))THROW_EXCP(excpSync,"ͬ������������.");
		InitRCount(1);		//��Ӧ�ü���Ϊ1
	};
	virtual ~robject(){};

	virtual void Free(){delete this;};	//ȱʡ���ͷź���,ȱʡ�Ĳ�������ɾ���Լ�.
public:
	uLong adopt()								//[����]�������,���ü���������1
	{
		_lock();
		try{
			_m_RefCount++;
			uLong l_count =_m_RefCount;
			_unlock();
			return l_count;
		}catch(...){}
		_unlock();
		return 0;
	}
	uLong discard()							//[����]����������ʹ��,���ü�����1,��0�͵���Free�ͷ�
	{
		_lock();
		try{
			if(!_m_RefCount)throw 0;
			_m_RefCount--;
			uLong l_count =_m_RefCount;
			_unlock();
			if(!l_count)Free();
			return l_count;
		}catch(...){}
		_unlock();
		return 0;
	};
	uLong GetRCount(){return _m_RefCount;};
protected:
	void InitRCount(uLong iniref){_m_RefCount =iniref;}
private:
	void	_lock()	{if(sync){_m_SyncMutex.lock();}}
	void	_unlock(){if(sync){_m_SyncMutex.unlock();}}
	uLong	volatile	_m_RefCount;
	Mutex				_m_SyncMutex;
};
//------------------------------------------------------------------------------------------------------------------
template<class T,bool sync =false>
class rptr					//���robjectʹ�õ�����ָ����
{
public:
	rptr()					:_m_objptr(0)				{if(sync && !(_m_SyncMutex.Create(false)))THROW_EXCP(excpSync,"�����彨������");};
	rptr(const rptr &ptr)	:_m_objptr(ptr._m_objptr)	{if(sync && !(_m_SyncMutex.Create(false)))THROW_EXCP(excpSync,"�����彨������");if(_m_objptr)_m_objptr->adopt();};
	rptr(T *objptr)			:_m_objptr(objptr)			{if(sync && !(_m_SyncMutex.Create(false)))THROW_EXCP(excpSync,"�����彨������");if(_m_objptr)_m_objptr->adopt();};
	~rptr()
	{
		lock();
		try{
			if(_m_objptr)_m_objptr->discard();
		}catch(...){}
		unlock();
	};

	inline void	lock()	{if(sync){_m_SyncMutex.lock();}}
	inline void	unlock(){if(sync){_m_SyncMutex.unlock();}}

	rptr &operator=(const rptr &ptr){
		lock();
		try{
			if(_m_objptr) _m_objptr->discard();
			_m_objptr	=ptr._m_objptr;
			if(_m_objptr) _m_objptr->adopt();
		}catch(...){}
		unlock();
		return *this;
	}
	rptr &operator=(T *objptr){
		lock();
		try{
			if(_m_objptr) _m_objptr->discard();
			_m_objptr	=objptr;
			if(_m_objptr) _m_objptr->adopt();
		}catch(...){}
		unlock();
		return *this;
	}
	operator bool()const{return _m_objptr!=0;}
	T& operator* ()const{return *_m_objptr;}
	T* operator->()const{return _m_objptr;}
	operator T*  ()const{return	_m_objptr;}
/*	operator T* &()
	{
		if(_m_objptr) _m_objptr->discard();
		_m_objptr =0;
		return _m_objptr;
	}
*/
private:
	T		*volatile	_m_objptr;
	Mutex				_m_SyncMutex;
};

#pragma pack(pop)
_DBC_END

#endif
