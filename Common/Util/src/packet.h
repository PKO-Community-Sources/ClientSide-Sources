
//
// packet.h
//
//  created by claude fan at 2004-8-9
//

#ifndef PACKET_H_
#define PACKET_H_


#include "pch.h"

typedef struct tagLogPacketHdr
    {
    DWORD dwSize; // 包头长度
    DWORD dwFullSize; // 整个包长度
    DWORD dwItemCount; // 数据区中Item的个数
    DWORD dwReserved; // 保留
    } LOGPACKETHDR, * PLOGPACKETHDR;

typedef struct tagLogItem
    {
    DWORD dwSize; // 此子项总长度： dwSize = sizeof DWORD + sizeof data
    BYTE data[1];
    } LOGITEM, * PLOGITEM;

class CLogString
    {
    friend class CLogPacket;

public:
    CLogString();
    CLogString(char* String);
    CLogString(PBYTE pbSrc, DWORD dwLen);
    CLogString(const CLogString& LogString);
    ~CLogString();

    CLogString& operator =(const char* String);
    CLogString& operator =(const CLogString& LogString);
    operator LPSTR() {return m_pBuf;}

protected:
private:
    CHAR* m_pBuf;
    DWORD m_dwBufLen;
    };

class CLogPacket
    {
public:
    CLogPacket(DWORD Size = 800);
    ~CLogPacket(void);

    void operator <<(LOGPACKETHDR& hdr);
    void operator >>(LOGPACKETHDR& hdr);
    void operator <<(CLogString& lstr);
    void operator >>(CLogString& lstr);
	void operator <<(string& lstr);
	void operator >>(string& lstr);
    void operator <<(COPYDATASTRUCT& cds);
    void operator >>(COPYDATASTRUCT& cds);

protected:
private:
    DWORD m_Size; // 整个包分配存储区大小
    PVOID m_pPacket;
    PLOGPACKETHDR m_pPacketHdr;
    PVOID m_pPacketData;
    DWORD m_DataSize; // 包中数据区大小

    PBYTE m_pDataCurr; // 当前数据区读取写入位置指针
    };


#endif