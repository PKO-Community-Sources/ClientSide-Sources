#ifndef DBCEXCP_H
#define DBCEXCP_H

#include <exception>
#include <string.h>
#include "DBCCommon.h"

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)

//------------------------------------------------------------------------------------------------------------------
//common exception define
class excp:public exception			//基异常类
{
public:
	excp(cChar * desc):exception((_mPtr=new char[strlen(desc)+1])?strcpy(_mPtr,desc):""){}
	excp(excp &e):exception(_mPtr=e._mPtr){e.setZero();}
	virtual ~excp(){delete[]_mPtr;}
	virtual cChar *what() const{return _mPtr;}
private:
	excp &operator=(excp &e){
		delete[]_mPtr;
		exception::operator =(e);
		_mPtr=e._mPtr;
		e.setZero();
		return *this;
	};
	void setZero(){exception::operator=(exception());_mPtr =0;}

	char *_mPtr;
};
//------------------------------------------------------------------------------------------------------------------
class excpMem:public excp				//内存分配或释放异常
{
public:
	excpMem(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpArr:public excp				//数组越界或其他数组相关错误异常
{
public:
	excpArr(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpSync:public excp				//操作系统同步对象操作异常
{
public:
	excpSync(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpThrd:public excp				//操作系统线程操作异常
{
public:
	excpThrd(cChar * desc):excp(desc){};
};
class excpSock:public excp				//操作系统线程操作异常
{
public:
	excpSock(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpCOM:public excp				//COM操作异常
{
public:
	excpCOM(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpDB:public excp				//数据库操作异常
{
public:
	excpDB(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpIniF:public excp				//文件操作异常
{
public:
	excpIniF(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpFile:public excp				//文件操作异常
{
public:
	excpFile(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpXML:public excp				//文件操作异常
{
public:
	excpXML(cChar * desc):excp(desc){};
};

#pragma pack(pop)
_DBC_END

//===================================================================================================================================

#define THROW_EXCP(EXCP,DESC) throw EXCP(dbc::dstring("File:")<<__FILE__<<" Line:"<<__LINE__<<" desc:"<<DESC);//程序中抛异常所用的宏定义
#ifndef PREALLOC_H
#include "dstring.h"
#endif

#endif
