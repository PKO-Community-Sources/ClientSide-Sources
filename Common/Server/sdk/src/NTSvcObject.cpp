//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// This program is modified recently by Zhang.Dabo in 2003.7
//=================================================================

#include "NTSvcObject.h"
#include "shlwapi.h"

_DBC_USING

NTServiceSet dbc::g_svcset;

//================NTSvcObject=================================================================
NTSvcObject::NTSvcObject():m_dwCheckPoint(0),m_SvcArgc(0),m_SvcArgv(0),m_nextservice(0),
					m_hAppEvtSrc(0),m_hSecEvtSrc(0),m_hSysEvtSrc(0),m_delay(0)
{
	m_semSignal1.Create(0,1,0);
	m_semSignal2.Create(0,1,0);
	if(!m_semSignal1||!m_semSignal2){
		THROW_EXCP(excpSync,"�����ʼ���ڼ佨������ϵͳͬ��������");
	}
};
//==============================================================================
BOOL NTSvcObject::NotifyState(DWORD CurrentState,DWORD WaitHint,DWORD Win32ExitCode,DWORD ServiceSpecificExitCode)   // address of status structure
{
	SERVICE_STATUS	l_svcstat;

	m_dwCurrentState =CurrentState;
	l_svcstat.dwServiceType =SERVICE_WIN32_SHARE_PROCESS |
					SERVICE_INTERACTIVE_PROCESS ;
	l_svcstat.dwCurrentState =m_dwCurrentState;
	l_svcstat.dwWin32ExitCode =Win32ExitCode;
	l_svcstat.dwServiceSpecificExitCode =ServiceSpecificExitCode;
	l_svcstat.dwWaitHint =WaitHint;
	switch(m_dwCurrentState){
		case SERVICE_START_PENDING:
			l_svcstat.dwCurrentState =RelyMainThread()?SERVICE_RUNNING:m_dwCurrentState;//����������Ϊ�˷���ʼ��������ƭSC
			l_svcstat.dwControlsAccepted =0;
			l_svcstat.dwCheckPoint =++m_dwCheckPoint;
			break;
		case SERVICE_RUNNING:
			l_svcstat.dwControlsAccepted =SERVICE_ACCEPT_STOP |
								(CanPaused()?SERVICE_ACCEPT_PAUSE_CONTINUE:0) |
								SERVICE_ACCEPT_SHUTDOWN;
			l_svcstat.dwCheckPoint =0;
			l_svcstat.dwWaitHint =0;
			break;
		case SERVICE_PAUSE_PENDING:
			l_svcstat.dwControlsAccepted =0;
			l_svcstat.dwCheckPoint =++m_dwCheckPoint;
			break;
		case SERVICE_PAUSED:
			l_svcstat.dwControlsAccepted =SERVICE_ACCEPT_STOP |
								(CanPaused()?SERVICE_ACCEPT_PAUSE_CONTINUE:0) |
								SERVICE_ACCEPT_SHUTDOWN;
			l_svcstat.dwCheckPoint =0;
			l_svcstat.dwWaitHint =0;
			break;
		case SERVICE_CONTINUE_PENDING:
			l_svcstat.dwControlsAccepted =0;
			l_svcstat.dwCheckPoint =++m_dwCheckPoint;
			break;
		case SERVICE_STOP_PENDING:
			l_svcstat.dwCurrentState =m_ServiceExitFlag&&RelyMainThread()?SERVICE_RUNNING:m_dwCurrentState;//����������Ϊ�˷���ʼ��������ƭSC
			l_svcstat.dwControlsAccepted =0;
			l_svcstat.dwCheckPoint =++m_dwCheckPoint;
			break;
		case SERVICE_STOPPED:
			l_svcstat.dwControlsAccepted =0;
			l_svcstat.dwCheckPoint =0;
			l_svcstat.dwWaitHint =100;
			break;
	}
	return SetServiceStatus(m_hServiceStatus,&l_svcstat);
};
//============NTServiceSet=====================================================
void NTSvcObject::Trace_Event(uChar Severity,cChar * msg)const
{
	if(HasAppEvent()||HasSysEvent()){
		ReportEvent(HasSysEvent()?m_hSysEvtSrc:m_hAppEvtSrc,
			Severity==TE_SUCC?EVENTLOG_SUCCESS:
			Severity==TE_INFO?EVENTLOG_INFORMATION_TYPE:
			Severity==TE_WARN?EVENTLOG_WARNING_TYPE:
						EVENTLOG_ERROR_TYPE,
			m_dwCurrentState==SERVICE_START_PENDING?CAT_STARTING:
			m_dwCurrentState==SERVICE_STOP_PENDING?CAT_STOPPING:
						CAT_RUNNING,
			Severity==TE_SUCC?EVT_SUCCESS:
			Severity==TE_INFO?EVT_INFO:
			Severity==TE_WARN?EVT_WARNING:
									 EVT_ERROR,
			0,1,0,&msg,0);
	}
}
//============NTServiceSet=====================================================
void NTServiceSet::UnRegEventLog(NTSvcObject* svc)const
{
	dstring l_dstr;
	if(svc->HasAppEvent()){
		l_dstr	="SYSTEM\\CurrentControlSet\\Services\\Eventlog\\Application\\";
		l_dstr	<<svc->SetDispName();
		SHDeleteKey(HKEY_LOCAL_MACHINE,l_dstr);
	}
	if(svc->HasSecEvent()){
		l_dstr	="SYSTEM\\CurrentControlSet\\Services\\Eventlog\\Security\\";
		l_dstr	<<svc->SetDispName()<<".SEC";
		SHDeleteKey(HKEY_LOCAL_MACHINE,l_dstr);
	}
	if(svc->HasSysEvent()){
		l_dstr	="SYSTEM\\CurrentControlSet\\Services\\Eventlog\\System\\";
		l_dstr	<<svc->SetDispName()<<".SYS";
		SHDeleteKey(HKEY_LOCAL_MACHINE,l_dstr);
	}
};
int NTServiceSet::RegEventLog(NTSvcObject* svc)const
{
	HKEY	l_hkey;
	LONG	l_retval	=ERROR_SUCCESS;
	DWORD l_data;
	dstring l_dstr;
	char *l_str;
	if(svc->HasAppEvent()){
		l_dstr	="SYSTEM\\CurrentControlSet\\Services\\Eventlog\\Application\\";
		l_dstr	<<svc->SetDispName();
		SHDeleteKey(HKEY_LOCAL_MACHINE,l_dstr);
		l_retval =l_retval?l_retval:RegCreateKeyEx(HKEY_LOCAL_MACHINE,l_dstr,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,&l_hkey,0);
		l_dstr	=svc->SetRCFile()?svc->SetRCFile():(m_argv[0]+1);
		if(l_str =strchr(l_dstr,'\"'))*l_str =0;
		l_retval =l_retval?l_retval:RegSetValueEx(l_hkey,"EventMessageFile",0,REG_EXPAND_SZ,l_dstr.c_ustr(),DWORD(l_dstr.length()+1));
		l_retval =l_retval?l_retval:RegSetValueEx(l_hkey,"CategoryMessageFile",0,REG_EXPAND_SZ,l_dstr.c_ustr(),DWORD(l_dstr.length()+1));
		l_data	=svc->SetNumOfEvtCat();
		l_retval =l_retval?l_retval:RegSetValueEx(l_hkey,"CategoryCount",0,REG_DWORD,(cuChar *)&l_data,sizeof(l_data));
		l_data	=0x7;
		l_retval =l_retval?l_retval:RegSetValueEx(l_hkey,"TypesSupported",0,REG_DWORD,(cuChar *)&l_data,sizeof(l_data));
		RegCloseKey(l_hkey);
	}
	if(svc->HasSecEvent()){
		l_dstr	="SYSTEM\\CurrentControlSet\\Services\\Eventlog\\Security\\";
		l_dstr	<<svc->SetDispName()<<".SEC";
		SHDeleteKey(HKEY_LOCAL_MACHINE,l_dstr);
		l_retval =l_retval?l_retval:RegCreateKeyEx(HKEY_LOCAL_MACHINE,l_dstr,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,&l_hkey,0);
		l_dstr	=svc->SetRCFile()?svc->SetRCFile():(m_argv[0]+1);
		if(l_str =strchr(l_dstr,'\"'))*l_str =0;
		l_retval =l_retval?l_retval:RegSetValueEx(l_hkey,"EventMessageFile",0,REG_EXPAND_SZ,l_dstr.c_ustr(),DWORD(l_dstr.length()+1));
		l_retval =l_retval?l_retval:RegSetValueEx(l_hkey,"CategoryMessageFile",0,REG_EXPAND_SZ,l_dstr.c_ustr(),DWORD(l_dstr.length()+1));
		l_data	=svc->SetNumOfEvtCat();
		l_retval =l_retval?l_retval:RegSetValueEx(l_hkey,"CategoryCount",0,REG_DWORD,(cuChar *)&l_data,sizeof(l_data));
		l_data	=0x1F;
		l_retval =l_retval?l_retval:RegSetValueEx(l_hkey,"TypesSupported",0,REG_DWORD,(cuChar *)&l_data,sizeof(l_data));
		RegCloseKey(l_hkey);
	}
	if(svc->HasSysEvent()){
		l_dstr	="SYSTEM\\CurrentControlSet\\Services\\Eventlog\\System\\";
		l_dstr	<<svc->SetDispName()<<".SYS";
		SHDeleteKey(HKEY_LOCAL_MACHINE,l_dstr);
		l_retval =l_retval?l_retval:RegCreateKeyEx(HKEY_LOCAL_MACHINE,l_dstr,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,&l_hkey,0);
		l_dstr	=svc->SetRCFile()?svc->SetRCFile():(m_argv[0]+1);
		if(l_str =strchr(l_dstr,'\"'))*l_str =0;
		l_retval =l_retval?l_retval:RegSetValueEx(l_hkey,"EventMessageFile",0,REG_EXPAND_SZ,l_dstr.c_ustr(),DWORD(l_dstr.length()+1));
		l_retval =l_retval?l_retval:RegSetValueEx(l_hkey,"CategoryMessageFile",0,REG_EXPAND_SZ,l_dstr.c_ustr(),DWORD(l_dstr.length()+1));
		l_data	=svc->SetNumOfEvtCat();
		l_retval =l_retval?l_retval:RegSetValueEx(l_hkey,"CategoryCount",0,REG_DWORD,(cuChar *)&l_data,sizeof(l_data));
		l_data	=0x7;
		l_retval =l_retval?l_retval:RegSetValueEx(l_hkey,"TypesSupported",0,REG_DWORD,(cuChar *)&l_data,sizeof(l_data));
		RegCloseKey(l_hkey);
	}
	return l_retval;
};
int NTServiceSet::RegSvc()const
{
	int l_has =0,l_errcount =0;

	SC_HANDLE sc =OpenSCManager(0,0,GENERIC_WRITE);
	if(!sc){
		l_errcount =m_svclen+1;
	}else{
		l_errcount=0;
		for(PNTSVC l_svc=m_svchead;l_svc;l_svc =l_svc->m_nextservice){
			SC_HANDLE sv =OpenService(sc,l_svc->SetSvcName(),
																STANDARD_RIGHTS_REQUIRED);
			if(sv){
				l_has ++;
				DeleteService(sv);
				CloseServiceHandle(sv);
				Sleep(500);
			}
			sv =CreateService(sc,l_svc->SetSvcName(),
								l_svc->SetDispName(),SERVICE_ALL_ACCESS,
								SERVICE_WIN32_SHARE_PROCESS|SERVICE_INTERACTIVE_PROCESS,
								SERVICE_AUTO_START,
								SERVICE_ERROR_NORMAL,m_argv[0],0,0,
								l_svc->SetDependencies(),0,0);
			if(sv)
				CloseServiceHandle(sv);
			else
				l_errcount++;
			RegEventLog(l_svc);
		}
		CloseServiceHandle(sc);
	}
	dstring	l_caption,l_msg;
	int		l_retval =0;

	if(l_errcount >m_svclen){
		l_caption	="Sorry!";
		l_msg			="û��ִ��ע�����,��������û�Ȩ�޲���.";
		l_retval =-3;
	}else if(l_has ==m_svclen){
		l_msg			="���з��񶼴���,��ȫ����ɾ������ִ����ע�����,";
		if(l_errcount ==m_svclen){
			l_caption	="Sorry!";
			l_msg			+="����ע��ʧ����!��������������ע��.";
			l_retval =-2;
		}else if(l_errcount){
			l_caption	="Sorry!";
			l_msg			+="ֻ�в���ע��ɹ���!��������������ע��.";
			l_retval =-1;
		}else{
			l_caption	="��ϲ!��ϲ!";
			l_msg			+="�������з���ע��ɹ���!";
			l_retval =0;
		}
	}else if(l_has){
		l_msg			="���ַ������,������ɾ������ִ����ע�����,";
		if(l_errcount ==m_svclen){
			l_caption	="Sorry!";
			l_msg			+="���ҵ��ǰ���������������ע��ʧ����!��������������ע��.";
			l_retval =-2;
		}else if(l_errcount){
			l_caption	="Sorry!";
			l_msg			+="���ҵ��ǰ���������������ֻ�в���ע��ɹ���!��������������ע��.";
			l_retval =-1;
		}else{
			l_caption	="��ϲ!��ϲ!";
			l_msg			+="�������з���ע��ɹ���!";
			l_retval =0;
		}

	}else if(l_errcount ==m_svclen){
		l_caption	="Sorry!";
		l_msg			="û���κη��񱻳ɹ�ע��!������������,�ٽ���ע��.";
		l_retval =-2;
	}else if(l_errcount){
		l_caption	="Sorry!";
		l_msg			="���ַ���ע��ʧ��!������������,�ٽ���ע��.";
		l_retval =-1;
	}else{
		l_caption	="��ϲ!��ϲ!";
		l_msg			="���з���ע��ɹ���!";
		l_retval =0;
	}
	#ifdef _DEBUG
		MessageBox(0,l_msg,l_caption,MB_OK);
	#else
		if(l_retval)THROW_EXCP(excp,strcat(l_caption.GetBuffer(),l_msg));
	#endif
	return l_retval;
}
//==============================================================================
int NTServiceSet::UnRegSvc()const
{
	int l_errcount =0;
	
	SC_HANDLE sc =OpenSCManager(0,0,GENERIC_WRITE);
	if(!sc)
		l_errcount =m_svclen+1;
	else{
		l_errcount=0;
		for(PNTSVC l_svc=m_svchead;l_svc;l_svc =l_svc->m_nextservice){
			UnRegEventLog(l_svc);
			SC_HANDLE sv =OpenService(sc,l_svc->SetSvcName(),STANDARD_RIGHTS_REQUIRED);
			if(!sv)	l_errcount++;
			else{
				if(!DeleteService(sv))l_errcount++;
				CloseServiceHandle(sv);
			}
		}
		CloseServiceHandle(sc);
	}
	dstring l_caption,l_msg;
	int		l_retval =0;

	if(l_errcount >m_svclen){
		l_caption	="Sorry!";
		l_msg			="û��ɾ���κη���!��������û�Ȩ�޲���.";
	}else if(l_errcount ==m_svclen){
		l_caption	="Sorry!";
		l_msg			="û��ɾ���κη���!����������Щ���񶼲�����,������ϵͳ������������ɾ��.";
	}else if(l_errcount){
		l_caption	="Sorry!";
		l_msg			="���ַ���ɾ��ʧ��!������Щ���񲻴���,������ϵͳ������������ɾ��.";
	}else{
		l_caption	="Sorry!";
		l_msg			="���з��񶼱��ɹ�ɾ��!";
	}
	l_retval =l_errcount>m_svclen?-3:(l_errcount==m_svclen?-2:(l_errcount?-1:0));
	#ifdef _DEBUG
		MessageBox(0,l_msg,l_caption,MB_OK);
	#else
		if(l_retval) THROW_EXCP(excp,l_msg);
	#endif
	return l_retval;
}
//==============================================================================
int NTServiceSet::Run()
{
//�����������
	uLong	l_pos;
	char		*argv[256];
	dstring	l_dstr;
	char		l_cmdline[8*1024];

	l_pos	 =0;
	m_argv =argv;
	LPWSTR *l_argv	=CommandLineToArgvW(GetCommandLineW(),&m_argc);
	for(int i=0;i<m_argc;i++){
		m_argv[i] =l_cmdline+l_pos;
		if(!i){
			m_argv[i][0]='"';
			wctmbcpy(argv[i]+1,l_argv[i]);
         GetFullPathName(m_argv[i]+1,1024,m_argv[i]+1,0);
			l_dstr	=m_argv[i]+strlen(m_argv[i])-4;
			strlwr(l_dstr.GetBuffer());
			if(l_dstr !=".exe"){				//�ж�ĩβ�Ƿ���.exe,û�о�׷����
				strcat(m_argv[i],".exe");
			}
			strcat(m_argv[i],"\"");
		}else
		{
			wctmbcpy(m_argv[i],l_argv[i]);
			if(m_argv[i][0]=='/')m_argv[i][0]='-';	//�������Ϊ/abc��ʽ��ת��Ϊ��׼��ʽ-abc
		}
		l_pos +=uLong(strlen(m_argv[i]))+1;
	}
//��������
	cChar *l_s0=0,*l_s1=0,*l_sd=0;
	for(long i=1;i<m_argc;i++){
		l_dstr	=m_argv[i];
		strlwr(l_dstr.GetBuffer());			//ת����Сд
		l_s0	=(!strncmp(l_dstr,"-reg",4)||!strncmp(l_dstr,"-inst",5))?l_dstr.c_str():0;
		l_s1	=(!strncmp(l_dstr,"-dereg",6)||!strncmp(l_dstr,"-uninst",7))?l_dstr.c_str():0;
		l_sd	=!strncmp(l_dstr,"-dbg",4)?l_dstr.c_str():0;//�Ƿ��е��Բ���
		if(l_s0||l_s1||l_sd){break;}
	}
	l_dstr.SetSize(0);						//�ռ������
//��������
	if(l_s0){								//��������
		return RegSvc();
	}else	if(l_s1){						//ɾ������
		return UnRegSvc();
	}else	if(l_sd){						//���Է���
		l_sd +=4;
		l_s0 =strchr(l_sd,' ');
		if(l_s0)*const_cast<char*>(l_s0)=0;	//��atoi����ʹ��
		long i;
		NTSvcObject* l_svc=m_svchead;
		for(i=atoi(l_sd)-1;(i>0)&&l_svc;i--,l_svc =l_svc->m_nextservice);
		if(l_s0)*const_cast<char*>(l_s0)=' ';//atoiʹ�ú�ָ��ո�.

		if(l_svc&&!i){
			try{
				l_svc->ServiceStart();
			}catch(excp &e){
				MessageBox(0,dstring("�������̳����쳣,�쳣����������Ϣ:")<<e.what(),dstring("����:")<<l_svc->SetDispName()<<" ����ģʽ",MB_OK);
			}catch(...){
				MessageBox(0,"�������̳���δ֪�쳣",dstring("����:")<<l_svc->SetDispName()<<" ����ģʽ",MB_OK);
			}
			while(IDYES==MessageBox(0,"�������Է�����?(��ʾ:ѡ���ǽ�����[������]����)",dstring("����:")<<l_svc->SetDispName()<<" ����ģʽ",MB_YESNO))
			{
				try{
					l_svc->ServiceProcess();
				}catch(excp &e){
					MessageBox(0,dstring("������̳����쳣,�쳣����������Ϣ:")<<e.what(),dstring("����:")<<l_svc->SetDispName()<<" ����ģʽ",MB_OK);
				}catch(...){
					MessageBox(0,"������̳���δ֪�쳣",dstring("����:")<<l_svc->SetDispName()<<" ����ģʽ",MB_OK);
				}
			}
			try{
				l_svc->ServiceStop();
			}catch(excp &e){
				MessageBox(0,dstring("ֹͣ���̳����쳣,�쳣����������Ϣ:")<<e.what(),dstring("����:")<<l_svc->SetDispName()<<" ����ģʽ",MB_OK);
			}catch(...){
				MessageBox(0,"ֹͣ���̳���δ֪�쳣",dstring("����:")<<l_svc->SetDispName()<<" ����ģʽ",MB_OK);
			}
		}
		return 0;
	}

	LPSERVICE_TABLE_ENTRY DispatchTable=new SERVICE_TABLE_ENTRY[m_svclen+1];
	long k=0;PNTSVC l_svc;
	while(m_svchead){
		l_svc =m_svchead;
		DispatchTable[k].lpServiceName=const_cast<char*>(l_svc->SetSvcName());
		DispatchTable[k].lpServiceProc=l_svc->m_ServiceMain;
		k++;
		DelService(l_svc);
	}
	DispatchTable[k].lpServiceName=0;
	DispatchTable[k].lpServiceProc=0;
	if( StartServiceCtrlDispatcher(DispatchTable) )
	{
//		_flushall();
	}
	delete []DispatchTable;
	return 0;
}
