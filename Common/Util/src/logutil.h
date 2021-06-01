
//
// logutil.h
//
//  created by claude fan at 2004-8-9
//

#ifndef LOG_UTIL_H_
#define LOG_UTIL_H_

#include "packet.h"
#include "util2.h"

class CLogMechanism
    {
public:
    CLogMechanism();
    virtual ~CLogMechanism();

    virtual int Process(CLogPacket& lp) = 0;

protected:
private:
    };


#ifdef WIN32
//
// 使用Windows平台上的WM_COPYDATA消息将数据转发给其他进程
//
class CW32_WMCOPYDATA : public CLogMechanism
    {
public:
	CW32_WMCOPYDATA(char const* pszWindowTitle)
		{
		m_WindowTitle = pszWindowTitle;
		m_hWnd = NULL;
		}
    ~CW32_WMCOPYDATA()
		{
		}

    int Process(CLogPacket& lp);

protected:
private:
    HWND m_hWnd; // 本地目的窗口句柄
	string m_WindowTitle;
    };
#endif // WIN32



enum {P_RECV, P_SEND, P_OTHER};

// 使用的LOG方法的标识，可放在包头的dwReserved域
enum LOG_CALL_TYPE {LG_CALL = 0, GPL_CALL = 1};

class CLogMgr2 : public GAME::Singleton<CLogMgr2>
    {
public:
    CLogMgr2();
    ~CLogMgr2();

	CLogMechanism* Add(char const* pszFunName, CLogMechanism* pLogMech);
	CLogMechanism* Get(char const* pszFunName);
	bool Remove(char const* pszFunName);

protected:
private:
    map<string, CLogMechanism*> m_LogIdx;
    };


#endif // LOG_UTIL_H_
