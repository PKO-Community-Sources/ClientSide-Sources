//
// packet.cpp
//
//  created by claude fan at 2004-8-9
//

#include "pch.h"
#include "packet.h"
#include <memory>

/////////////////////////////////////////////////////
CLogString::CLogString()
    {
    m_pBuf = NULL;
    }

CLogString::CLogString(char* String)
    {
    m_dwBufLen = (DWORD) strlen(String) + 1;
    m_pBuf = new CHAR[m_dwBufLen];
    strcpy(m_pBuf, String);
    m_pBuf[m_dwBufLen - 1] = 0;
    }

CLogString::CLogString(PBYTE pbSrc, DWORD dwLen)
    {
    m_pBuf = new CHAR[dwLen];
    memcpy(m_pBuf, pbSrc, dwLen);
    m_dwBufLen = dwLen;
    }

CLogString::CLogString(const CLogString& LogString)
    {
    if (m_pBuf)
        {
        delete m_pBuf;
        }

    m_dwBufLen = LogString.m_dwBufLen;
    m_pBuf = new CHAR[m_dwBufLen];
    memcpy(m_pBuf, LogString.m_pBuf, m_dwBufLen);
    }

CLogString& CLogString::operator =(const char* String)
    {
    if (m_pBuf)
        {
        delete m_pBuf;
        }

    m_dwBufLen = (DWORD) strlen(String) + 1;
    m_pBuf = new CHAR[m_dwBufLen];
    strcpy(m_pBuf, String);
    m_pBuf[m_dwBufLen - 1] = 0;
    return *this;
    }

CLogString& CLogString::operator =(const CLogString& LogString)
    {
    if (this == &LogString)
        {
        return *this;
        }

    if (m_pBuf)
        {
        delete m_pBuf;
        }

    m_dwBufLen = LogString.m_dwBufLen;
    m_pBuf = new CHAR[m_dwBufLen];
    memcpy(m_pBuf, LogString.m_pBuf, m_dwBufLen);
    return *this;
    }

CLogString::~CLogString()
    {
    if (m_pBuf)
        {
        delete m_pBuf;
        }
    }





/////////////////////////////////////////////////////
CLogPacket::CLogPacket(DWORD Size /* = 800 */)
    {
    m_Size = max(800, Size);
	try
		{
		m_pPacket = new UCHAR[m_Size];
		}
	catch (bad_alloc& e)
		{
		throw int(-1);
		}
    m_pPacketHdr = NULL;
    m_pPacketData = NULL;
    m_DataSize = 0;
    m_pDataCurr = NULL;
    }

CLogPacket::~CLogPacket(void)
    {
    delete m_pPacket;
    m_pPacket = NULL;
    m_Size = 0;
    }

void CLogPacket::operator <<(LOGPACKETHDR& hdr)
    {
    try
        {
        memcpy(m_pPacket, &hdr, sizeof hdr);
        m_pPacketHdr = (PLOGPACKETHDR) m_pPacket;
        m_pPacketData = (PUCHAR) m_pPacket + sizeof hdr;
        m_DataSize = max(0, hdr.dwFullSize - hdr.dwSize);
        m_pDataCurr = (PBYTE) m_pPacketData;
        }
    catch (...)
        {
        }
    }

void CLogPacket::operator >>(LOGPACKETHDR& hdr)
    {
    try
        {
        memcpy(&hdr, m_pPacketHdr, sizeof hdr);
        }
    catch (...)
        {
        }
    }

void CLogPacket::operator <<(CLogString& lstr)
    {
    DWORD slen;
    DWORD dlen;
    PLOGITEM pitem;

    try
        {
        slen = lstr.m_dwBufLen;
        dlen = m_Size - ((PUCHAR) m_pDataCurr - (PUCHAR) m_pPacket);
        DWORD len = min(slen, dlen);

        pitem = (PLOGITEM) m_pDataCurr;
        pitem->dwSize = sizeof DWORD + len;
        memcpy(pitem->data, lstr.m_pBuf, len);
        m_pDataCurr += pitem->dwSize;
        m_DataSize += pitem->dwSize;
        m_pPacketHdr->dwFullSize += pitem->dwSize;

        ++ (m_pPacketHdr->dwItemCount);
        }
    catch (...)
        {
        }
    }

void CLogPacket::operator >>(CLogString& lstr)
    {
    PLOGITEM pitem;

    try
        {
        pitem = (PLOGITEM) m_pDataCurr;
        CLogString l_lstr(pitem->data, pitem->dwSize - sizeof DWORD);
        lstr = l_lstr;
        m_pDataCurr += pitem->dwSize;
        }
    catch (...)
        {
        }
    }

void CLogPacket::operator <<(string& lstr)
    {
    DWORD slen;
    DWORD dlen;
    PLOGITEM pitem;

    try
        {
		slen = DWORD(lstr.size());
        dlen = m_Size - ((PUCHAR) m_pDataCurr - (PUCHAR) m_pPacket);
        DWORD len = min(slen, dlen);

        pitem = (PLOGITEM) m_pDataCurr;
        pitem->dwSize = sizeof DWORD + len;
		memcpy(pitem->data, lstr.c_str(), len);
        m_pDataCurr += pitem->dwSize;
        m_DataSize += pitem->dwSize;
        m_pPacketHdr->dwFullSize += pitem->dwSize;

        ++ (m_pPacketHdr->dwItemCount);
        }
    catch (...)
        {
        }
    }

void CLogPacket::operator >>(string& lstr)
    {
    PLOGITEM pitem;

    try
        {
        pitem = (PLOGITEM) m_pDataCurr;
        lstr = (char*)(pitem->data);
        m_pDataCurr += pitem->dwSize;
        }
    catch (...)
        {
        }
    }

void CLogPacket::operator <<(COPYDATASTRUCT& cds)
    {
    try
        {
        memcpy(m_pPacket, cds.lpData, min(m_Size, cds.cbData));
        m_pPacketHdr = (PLOGPACKETHDR) m_pPacket;
        m_pPacketData = (PUCHAR) m_pPacket + sizeof LOGPACKETHDR;
        m_DataSize = max(0, m_pPacketHdr->dwFullSize - m_pPacketHdr->dwSize);
        m_pDataCurr = (PBYTE) m_pPacketData;
        }
    catch (...)
        {
        }
    }

void CLogPacket::operator >>(COPYDATASTRUCT& cds)
    {
    try
        {
        cds.cbData = m_pPacketHdr->dwFullSize;
        cds.lpData = m_pPacket;
        }
    catch (...)
        {
        }
    }
