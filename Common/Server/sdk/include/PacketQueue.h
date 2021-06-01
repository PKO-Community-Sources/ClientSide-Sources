#ifndef PACKETQUEUE_H
#define PACKETQUEUE_H

#include "PreAlloc.h"
#include "Packet.h"
class dbc::DataSocket;

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)

struct PKItem:public PreAllocStru {
	PKItem(uLong size)
		:PreAllocStru(size),m_inpk(0),m_retpk(0),m_next(0),m_iscall(0)
	{
		m_semWait.Create(0,1,0);
	}
	void Initially(){}
	void Finally(){
		m_inpk		=0;
		m_retpk		=0;
		m_next		=0;
		m_iscall	=0;
	}
	Sema					m_semWait;
	volatile char			m_iscall;
	DataSocket	*volatile	m_datasock;
	RPacket					m_inpk;
	WPacket					m_retpk;
	PKItem		*volatile	m_next;
	DWORD					m_dwLastTime;
}; 
struct	PKQueue{
public:
	PKQueue(bool mode =true);
	~PKQueue()
	{
		CloseQueue();
	}
	void	PeekPacket(uLong sleep =0);
	uLong	GetPkTotal(){MutexArmor l(m_mut);return m_pktotal;}

	void	CloseQueue();
	WPacket	SyncPK(DataSocket *datasock,RPacket &in_para,uLong ulMilliseconds=5*1000);
	void	AddPK(DataSocket * datasock,RPacket &pk);
private:
	PKItem	*GetPKItem(uLong end);

	virtual void ProcessData(DataSocket *datasock,RPacket &recvbuf)=0;
	virtual	WPacket	ServeCall(DataSocket *datasock,RPacket &in_para){return 0;};
	const bool		m_mode;
	bool			volatile m_isclose;
	uLong			volatile m_pktotal;
	Mutex			m_mut;
	Sema			m_sem;
	PKItem		*	volatile m_head;
	PKItem		*	volatile m_tail;
	static PreAllocHeapPtr<PKItem>	m_heap;
};

#pragma pack(pop)
_DBC_END

#endif	//PACKETQUEUE_H