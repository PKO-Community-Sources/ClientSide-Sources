#ifndef CONNECTION_H
#define CONNECTION_H

#include "ThreadPool.h"
#include "DataSocket.h"

class NetIF;
#pragma pack(push)
#pragma pack(4)
class Connection : public dbc::Task
{
public:
	enum{CNST_INVALID =0,CNST_CONNECTING=1,CNST_FAILURE=2,CNST_CONNECTED=3,CNST_TIMEOUT=4};
	Connection(NetIF *netif):m_netif(netif),m_status(CNST_INVALID),m_datasock(0){}
    void Clear() { m_status = CNST_INVALID; m_datasock = 0;}
	bool Connect(dbc::cChar *hostname,dbc::uShort port,dbc::uLong timeout =0);//∫¡√Î
	void Disconnect(int reason);
	void OnDisconnect();
	bool IsConnected()	{return m_status ==CNST_CONNECTED;}
	int  GetConnStat();
	void CHAPSTR();

	dbc::DataSocket	*GetDatasock()              { return m_datasock;    }

	long Process();
	Task * Lastly(){return 0;}
    
#ifdef _TEST_CLIENT
    void SwitchSocket( dbc::DataSocket* pSock, int status )  { m_datasock=pSock;  m_status=status; }
#endif

private:
	dbc::Mutex						m_mtx;
	NetIF			*const		m_netif;
	dbc::InterLockedLong		m_status;
	dbc::DataSocket	*volatile	m_datasock;

	dbc::uLong					m_timeout;
	dbc::uLong					m_tick;
	SOCKET						m_sock;
	char m_hostname[129];
	dbc::uShort	m_port;
};
#pragma pack(pop)

#endif		//CONNECTION_H
