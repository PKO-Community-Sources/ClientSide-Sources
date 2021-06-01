//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// This program is modified recently by Zhang.Dabo in 2003.7
//=================================================================
#ifndef NTSERVICE_H
#define NTSERVICE_H

#ifndef USING_TAO		//使用Win32基本Platform SDK
#include <winsock2.h>		//确保调用新的WinSock2.2版本
#include <windows.h>
#else
#include "TAOSpecial.h"
#endif

#include "NTSvcObject.h"

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)

#define DECLARE_SERVICE(Service)			dbc::PNTSVC dbc::GenNTService<Service>::m_svc;
#define INIT_SERVICE(Service)				dbc::GenNTService<Service> DBC##Service;
#define SERVICE_RUN							dbc::g_svcset.Run();
//=======NTService=======================================================================
class NTService:NTSvcObject{
private:
	virtual void ServiceStart()=0;
	virtual void ServiceStop()=0;
	virtual void ServicePause(){};
	virtual void ServiceContinue(){};
	virtual long ServiceProcess(){return 1000;};				//返回服务需要的调用间隔时间,缺省为1000毫秒(1秒),后裔类有信号等待应该返回0,函数中等待不能太久(应小于3秒),以保持对用户动作响应的及时性

	virtual cChar *SetSvcName()const =0;
	virtual cChar *SetDispName()const	 {return SetSvcName();};//显示名缺省等于服务名
	virtual cChar *SetDependencies()const{return 0;}		//缺省服务没有依赖,返回格式："MSSQL\0Server\0"
	virtual cChar *SetRCFile()const		 {return 0;};	//缺省使用可执行文件做资源文件
	virtual DWORD SetNumOfEvtCat()const	 {return 0;}			//CategoryMessageFile文件中的目录总数,后裔可以丢弃以提供目录.

	virtual bool CanPaused()const		{return true;};	//缺省支持暂停和继续操作
	virtual bool RelyMainThread()const	{return false;};	//尽量不依赖主线程,除非特殊需要就override返回true;
	virtual bool HasAppEvent()const		{return true;};
	virtual bool HasSecEvent()const		{return false;};	//一般来说没有安全事件
	virtual bool HasSysEvent()const		{return false;};	//一般来说没有系统事件
public:
	void GetSvcArg(DWORD &argc,LPTSTR *&argv)const{argc =m_SvcArgc;argv =m_SvcArgv;};
	void GetExeArg(int   &argc,char	**&argv)const{argc =g_svcset.m_argc;argv =g_svcset.m_argv;};
	char *GetExeFullPath()						  const{return g_svcset.m_argv[0];};

	void Trace_Event(uChar Severity,cChar * msg)const	{NTSvcObject::Trace_Event(Severity,msg);};//Severity见类首的SVRT宏定义
	void ServiceExit(DWORD Win32ExitCode=NO_ERROR,DWORD ServiceSpecExitCode=0)//参数参考Win32 API的SetServiceStatus()函数,一般使用缺省值即可
	{
		NTSvcObject::ServiceExit(Win32ExitCode,ServiceSpecExitCode);
	};
	BOOL NotifyState(DWORD WaitHint=5000)
	{
		return NTSvcObject::NotifyState(m_dwCurrentState,WaitHint);
	}
};


#define CATCH_COM_ERROR 	catch (_com_error &e)\
	{\
		char l_buf[128];\
		l_buf[0] =0;\
		sprintf(l_buf,"Error:");\
		sprintf(l_buf,"Code = %08lx", e.Error());\
		sprintf(l_buf,"Code meaning = %s", (TCHAR*) e.ErrorMessage());\
		sprintf(l_buf,"Source = %s", (TCHAR*) e.Source());\
		sprintf(l_buf,"Error Description = %s\n", (TCHAR*) e.Description());\
		RollbackTrans();\
		THROW_EXCP(excpCOM,l_buf);\
	}

#pragma pack(pop)
_DBC_END

#endif
