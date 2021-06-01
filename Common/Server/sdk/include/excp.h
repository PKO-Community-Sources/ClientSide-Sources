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
class excp:public exception			//���쳣��
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
class excpMem:public excp				//�ڴ������ͷ��쳣
{
public:
	excpMem(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpArr:public excp				//����Խ�������������ش����쳣
{
public:
	excpArr(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpSync:public excp				//����ϵͳͬ����������쳣
{
public:
	excpSync(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpThrd:public excp				//����ϵͳ�̲߳����쳣
{
public:
	excpThrd(cChar * desc):excp(desc){};
};
class excpSock:public excp				//����ϵͳ�̲߳����쳣
{
public:
	excpSock(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpCOM:public excp				//COM�����쳣
{
public:
	excpCOM(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpDB:public excp				//���ݿ�����쳣
{
public:
	excpDB(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpIniF:public excp				//�ļ������쳣
{
public:
	excpIniF(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpFile:public excp				//�ļ������쳣
{
public:
	excpFile(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpXML:public excp				//�ļ������쳣
{
public:
	excpXML(cChar * desc):excp(desc){};
};

#pragma pack(pop)
_DBC_END

//===================================================================================================================================

#define THROW_EXCP(EXCP,DESC) throw EXCP(dbc::dstring("File:")<<__FILE__<<" Line:"<<__LINE__<<" desc:"<<DESC);//���������쳣���õĺ궨��
#ifndef PREALLOC_H
#include "dstring.h"
#endif

#endif
