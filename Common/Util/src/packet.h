
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
    DWORD dwSize; // ��ͷ����
    DWORD dwFullSize; // ����������
    DWORD dwItemCount; // ��������Item�ĸ���
    DWORD dwReserved; // ����
    } LOGPACKETHDR, * PLOGPACKETHDR;

typedef struct tagLogItem
    {
    DWORD dwSize; // �������ܳ��ȣ� dwSize = sizeof DWORD + sizeof data
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
    DWORD m_Size; // ����������洢����С
    PVOID m_pPacket;
    PLOGPACKETHDR m_pPacketHdr;
    PVOID m_pPacketData;
    DWORD m_DataSize; // ������������С

    PBYTE m_pDataCurr; // ��ǰ��������ȡд��λ��ָ��
    };


#endif