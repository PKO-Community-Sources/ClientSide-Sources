//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// This program is modified recently by Zhang.Dabo in 2003.7
//=================================================================
/*
	写中可嵌套写和读
	读中只可嵌套读、不可嵌套写
*/
#ifndef READWRITESYNC_H
#define READWRITESYNC_H

#ifndef USING_TAO		//使用Win32基本Platform SDK
#include <winsock2.h>		//确保调用新的WinSock2.2版本
#include <windows.h>
#else
#include "TAOSpecial.h"
#endif

#include "DBCCommon.h"
#include "PreAlloc.h"

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)

//==============================================================================
class RWMutex{
public:
	RWMutex():m_RDCount(0),m_WRCount(0)
	{
		m_semWrite.Create(1,1,0);
		m_semRead.Create(1,1,0);
		m_mtxRWCount.Create(false);

		if(!m_semWrite||!m_semRead||!m_mtxRWCount)
		{
			THROW_EXCP(excpSync,"读写同步对象建立操作系统同步对象失败");
		}
	};
	void BeginRead();
	void EndRead();
	void BeginWrite();
	void EndWrite();
private:
	volatile int m_RDCount;
	volatile int m_WRCount;

	Sema		m_semRead,m_semWrite;
	Mutex		m_mtxRWCount;

};

#pragma pack(pop)
_DBC_END

#endif		//READWRITEMUTEX_H
