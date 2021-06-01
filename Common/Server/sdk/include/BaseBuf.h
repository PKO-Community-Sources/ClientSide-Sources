//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// This program is modified recently by Zhang.Dabo in 2003.7
//=================================================================
#ifndef BASEBUF
#define BASEBUF

#include "PreAlloc.h"
#include "robject.h"

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)

//基础的数据缓存类
class BaseBuf:public PreAllocStru{
public:
	BaseBuf(uLong size):PreAllocStru(size),m_size(size),m_buf((char *)malloc(size)){}
	virtual uLong Size(){return m_buf?m_size:0;}
	char *getbuf(){return m_buf;}
protected:
	~BaseBuf(){if(m_buf)free(m_buf);}
private:
	char	*const volatile	m_buf;
	cuLong					m_size;
};
//基础的基于引用计数内存管理得数据缓存类
class rbuf:public BaseBuf,public robject<true>{
public:
	rbuf(uLong size):BaseBuf(size){}
protected:
	void Initially(){InitRCount(0);}
	void Finally(){InitRCount(1);}
	void Free(){BaseBuf::Free();}
private:
};

#pragma pack(pop)
_DBC_END

#endif
