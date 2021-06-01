
#include "DataSocket.h"
#include "Sender.h"
#include "Packet.h"
#include "Comm.h"
#include "LogStream.h"

_DBC_USING

extern PreAllocHeap<rbuf>	__bufheap;
PreAllocHeapPtr<SNDPacket> Sender::m_SNDPackets(0,30);
//==============================================================================
class SendAll:public PreAllocTask{
public:
	SendAll(uLong size):PreAllocTask(size){}
	long		Process();
	Task	*	Lastly(){return PreAllocTask::Lastly();}
	inline void Init(DataSocket * datasock);
private:
	DataSocket	*	m_datasock;
};
//--------------------------------------------------
inline void SendAll::Init(DataSocket * datasock)
{
	m_datasock	=datasock;
}
long SendAll::Process()
{
	try{
		m_datasock->GetTcpApp()->OnSendAll(m_datasock);
	}catch(...){}
	return 0;
}
static PreAllocHeapPtr<SendAll> s_SendAlls(0,30);
//================================================================================
Sender::Sender(DataSocket * datasock)
	:m_datasock(datasock),__tca(0),m_p(0)
	,m_send(0),m_head(0),m_tail(0)
{
	m_mtxarra.Create(false);
	m_mtxcomb.Create(false);
	m_mtxsend.Create(false);
}
void Sender::Initially()
{
	m_p	=0;
}
void Sender::Finally()
{
	MutexArmor l_lockSend(m_mtxsend);
	MutexArmor l_lockArra(m_mtxarra);
	if(m_send)
	{
		m_send->Free();
	}
	while(m_head)
	{
		m_send	=m_head;
		m_head	=m_head->m_next;
		m_send->Free();
	}
	m_send	=0;
	m_tail	=0;
	l_lockArra.unlock();
	l_lockSend.unlock();
	m_semSndC.Destroy();
}
void Sender::Init()
{
	const_cast<TcpCommApp*>(__tca)	=m_datasock->GetTcpApp();
	m_semSndC.Create(__tca->__maxsndque,__tca->__maxsndque,0);
}
//================================================================================
void SNDPacket::Init(DataSocket *datasock,uLong size)
{
	rptr<rbuf>::operator =(__bufheap.Get(size));
	m_haspace	=true;
	m_datasock	=datasock;
}
void SNDPacket::Init(DataSocket *datasock,rbuf	*buf)
{
	rptr<rbuf>::operator =(buf);
	m_haspace	=true;
	m_datasock	=datasock;
}
//--------------------------------------------------
inline SNDPacket	&SNDPacket::operator<<(WPacket &wpk)
{
	//uLong l_len	=HasSpace();
	//l_len		=min(l_len ,wpk.GetPktLen() -wpk.m_cpos);
	//if(l_len)
	//{
	//	MemCpy(GetBufAddr() +m_cpos ,wpk.GetPktAddr() +wpk.m_cpos ,l_len);
	//}
	//m_cpos		+=l_len;
	//wpk.m_cpos	+=l_len;
	//if(wpk.m_cpos ==wpk.GetPktLen())
	//{
	//	wpk	=0;
	//	m_pkts ++;
	//}
	//return *this;
//====
	uLong l_len		=wpk.GetPktLen() -wpk.m_cpos;
	if(wpk.m_wpos <SIGN32 -em_cmdsize)
	{
		l_len			+=(l_len+9)/10;
	}
	if(HasSpace() <l_len)
	{
		m_haspace	=false;
	}else if(l_len)
	{
		l_len		=wpk.GetDataLen();
		if(wpk.m_wpos <SIGN32 -em_cmdsize)
		{
			try{
				m_datasock->GetTcpApp()->OnEncrypt(m_datasock,GetBufAddr() +m_cpos +wpk.Head(),wpk.GetDataAddr(),l_len);
			}catch(...){}
			wpk.m_wpos		=l_len	-em_cmdsize;
			wpk.WritePktLen();
			MemCpy(GetBufAddr() +m_cpos ,wpk.GetPktAddr(),wpk.Head());
		}else
		{
			wpk.WritePktLen();
			MemCpy(GetBufAddr() +m_cpos ,wpk.GetPktAddr() ,wpk.GetPktLen());
		}
		m_cpos		+=wpk.GetPktLen();
		wpk.m_cpos	+=wpk.GetPktLen();
		if(wpk.m_cpos ==wpk.GetPktLen())
		{
			wpk	=0;
			m_pkts ++;
		}
	}
	return *this;
}
//--------------------------------------------------
Sender	&Sender::operator<<(WPacket &wpk)
{
	if(!bool(wpk))return *this;
	//wpk.WritePktLen();

	MutexArmor l_lockSend(m_mtxsend);
	MutexArmor l_lockComb(m_mtxcomb);
	while(bool(wpk) && m_datasock->m_delflag ==0)
	{
		if(m_tail && m_tail->HasSpace())
		{
			*m_tail<<wpk;
			if(m_head->m_cpos >uLong(0.618 *m_head->SendSize()) && !m_send && !m_datasock->m_sendflag&&!m_datasock->m_delflag)
			{
				if(m_datasock->m_sendflag++	==0)
				{
					m_datasock->GetTcpApp()->GetCommunicator()->AddTask(this);
				}else
				{
					m_datasock->m_sendflag--;
				}
			}
		}else
		{
			l_lockComb.unlock();
			try{
			uLong	l_timelock =WAIT_TIMEOUT;
			bool l_flag =true;
			if((l_timelock=m_semSndC.trylock())!=WAIT_OBJECT_0)
			{
				do{
					try{l_flag=m_datasock->GetTcpApp()->OnSendBlock(m_datasock);}catch(...){}
				}while(!m_datasock->m_delflag && !GetExitFlag() && l_flag && (l_timelock=m_semSndC.timelock(100))!=WAIT_OBJECT_0);
			}

			if(!l_flag)
			{
				m_datasock->GetTcpApp()->Disconnect(m_datasock,0,-7);
			}

			if(l_timelock !=WAIT_OBJECT_0)
			{
				wpk	=0;
			}else
			{
				SNDPacket *	l_snd	=m_SNDPackets.Get();
				uLong		l_len	=wpk.GetPktLen() +(wpk.GetPktLen()+9)/10;
				l_snd->Init(m_datasock,max(m_datasock->GetSendBuf(),l_len));
				*l_snd<<wpk;

				MutexArmor l_lockArra(m_mtxarra);
				if(m_tail)
				{
					m_tail->m_next	=l_snd;
					m_tail			=l_snd;
				}else
				{
					m_head	=m_tail	=l_snd;
					if(m_head->m_cpos >uLong(0.618 *m_head->SendSize()) &&!m_send && !m_datasock->m_sendflag&&!m_datasock->m_delflag)
					{
						if(m_datasock->m_sendflag++	==0)
						{
							m_datasock->GetTcpApp()->GetCommunicator()->AddTask(this);
						}else
						{
							m_datasock->m_sendflag--;
						}
					}
				}
				l_lockArra.unlock();
			}}catch(...)
			{
				LogLine l_line(g_dbclog);
				l_line <<newln<<"Sender::operator<<(WPacket &wpk) exception,PeerIP:"<<m_datasock->GetPeerIP();
			}
			l_lockComb.lock();
		}//if(m_tail && m_tail->HasSpace())
	}//while(bool(wpk) && m_datasock->m_delflag ==0)
	l_lockComb.unlock();
	l_lockSend.unlock();
	return *this;
}
//--------------------------------------------------
bool Sender::PopSNDPacket(int instancy)
{
	bool l_bret =false;
	MutexArmor l_lockArra(m_mtxarra);
	if(!m_send)
	{
		if(!m_head)
		{
			m_send =0;
		}else if(m_head !=m_tail)
		{
			m_send	=m_head;
			m_head	=m_head->m_next;
		}else// if(m_head ==m_tail)
		{
			MutexArmor l_lockComb(m_mtxcomb);
			if(!instancy || m_head->m_cpos >uLong(0.618 *m_head->SendSize()))
			{
				m_send	=m_head;
				m_head	=m_tail	=0;
			}
			l_lockComb.unlock();
		}
		l_bret	=m_send?true:false;
	}else
	{
		l_bret	=false;
	}
	l_lockArra.unlock();
	return l_bret;
}
//--------------------------------------------------
long Sender::Process()
{
	try{
	long	l_retval;
	int		l_sendlen	=0;
	while(!m_datasock->m_delflag && !GetExitFlag() && HasData())
	{
		if(!m_send)
		{
			if(!PopSNDPacket(0))
			{
				break;
			}
		}
		l_sendlen	=send(m_datasock->m_socket,m_send->GetBufAddr() +m_p , m_send->m_cpos -m_p,0);
		if(l_sendlen >=0)
		{
			m_datasock->m_sendtime	=__tca->GetCurrentTick();
			m_p			+=l_sendlen;
			l_retval	=0;
		}else if(l_sendlen ==SOCKET_ERROR)
		{
			l_retval	=WSAGetLastError();
		}else
		{
			l_retval	=-2;
		}
		if(l_retval	&& (l_retval !=WSAEWOULDBLOCK))
		{
			m_datasock->GetTcpApp()->Disconnect(m_datasock,0,l_retval);
			break;
		}else if(m_p == m_send->m_cpos)
		{
			m_datasock->m_sbts		+=m_p;
			m_datasock->m_spks		+=m_send->m_pkts;

			m_send	->Free();
			m_send	=0;
			m_p		=0;
			m_semSndC.unlock();

			if(!PopSNDPacket(1))
			{
				SendAll	*l_task	=s_SendAlls.Get();
				l_task->Init(m_datasock);
				if(m_datasock->GetTcpApp()->GetProcessor())
				{
					m_datasock->GetTcpApp()->GetProcessor()->AddTask(l_task);
				}else
				{
					try{l_task->Process();}catch(...){}
					try{l_task->Lastly();}catch(...){}
				}
				break;
			}
		}else
		{
			break;
		}

	}//while(!m_datasock->m_delflag && !GetExitFlag() && HasData())
	}catch(...)
	{
		LogLine l_line(g_dbclog);
		l_line <<newln<<"Sender::Process() exception,PeerIP:"<<m_datasock->GetPeerIP()
			<<"this=0x"<<this<<"m_datasock=0x"<<m_datasock
			<<"m_send=0x"<<m_send<<"m_head=0x"<<m_head
			<<"m_tail=0x"<<m_tail
			<<"m_p="<<m_p<<"m_datasock->m_sendflag="<<m_datasock->m_sendflag;
	}

	m_datasock->m_sendflag--;
	return 0;
}
