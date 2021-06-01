
#ifndef LOGSTREAM_H
#define LOGSTREAM_H

#include <iostream>
#include <fstream>
#include <string>

#include "DBCCommon.h"
#include "dstring.h"
#include "RunBiDirectChain.h"

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)
class LogLine;
//======================================================================
class LogStream: public RunBiDirectItem<LogStream>
{
	friend std::ofstream & newln(LogLine & line);
	friend std::ofstream & endln(LogLine & line);
	friend std::ostream  & endln(std::ostream & of);
public:
	LogStream(const char *LogFileName,const char *DirName ="log");
	~LogStream();
	static void	Backup();
	bool is_open() const{return m_ofs.is_open();}
private:
	void beginln();
	void endln();

	void Free(){}
	void Open(bool bakmode =true);

	dstring				m_LogFileName;
	dstring				m_DirName;
	dstring				m_DirRoot;
	Mutex				m_mtxline;
	bool	volatile	m_line;
	LogLine			*	m_logline;
	std::ofstream		m_ofs;
};
//======================================================================
class LogLine
{
	friend std::ofstream & newln(LogLine & line);
	friend std::ofstream & endln(LogLine & line);
public:
	LogLine(LogStream & logstream):m_line(false),m_logstream(logstream){}
	~LogLine();
private:
	bool			m_line;
	LogStream	&	m_logstream;
};
//======================================================================
typedef std::ofstream & (* FOper)(LogLine & line);
inline std::ofstream & operator<<(LogLine & line,FOper f)
{
	return (*f)(line);
}
std::ofstream & newln(LogLine & line);
std::ofstream & endln(LogLine & line);
std::ostream  & endln(std::ostream & of);





extern LogStream g_dbclog;

#pragma pack(pop)
_DBC_END

#endif
