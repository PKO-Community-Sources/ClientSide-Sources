#include "MPDataStream.h"

#include "MPStringUtil.h"
#include "log.h"

//=============================================================================
// MPDataStream
//=============================================================================
template<typename T> MPDataStream& MPDataStream::operator>>(T& val)
{
	read(static_cast<void*>(&val), sizeof(T));
}
//-----------------------------------------------------------------------------
String MPDataStream::getLine(bool trimAfter)
{
	MPStringUtil::StrStreamType str;
	size_t c = MP_STREAM_TEMP_SIZE-1;
	// һֱѭ��ֱ�������ָ���
	while (c == MP_STREAM_TEMP_SIZE-1)
	{
		c = readLine(m_TmpArea, MP_STREAM_TEMP_SIZE-1);
		str << m_TmpArea;
	}

	String retString(str.str());
	if (trimAfter)
	{
		MPStringUtil::trim(retString);
	}

	return retString;
}
//-----------------------------------------------------------------------------
String MPDataStream::getAsString(void)
{
	// ��ȡ��������
	char* pBuf = new char[m_Size+1];
	read(pBuf, m_Size);
	pBuf[m_Size] = '\0';
	String str;
	str.insert(0, pBuf, m_Size);
	delete [] pBuf;
	return str;
}
//=============================================================================
// MPMemoryDataStream
//=============================================================================
MPMemoryDataStream::MPMemoryDataStream(void* pMem, size_t size, bool freeOnClose)
	: MPDataStream()
{
	m_Data = m_Pos = static_cast<uchar*>(pMem);
	m_Size = size;
	m_End  = m_Data + m_Size;
	m_FreeOnClose = freeOnClose;
}
//-----------------------------------------------------------------------------
MPMemoryDataStream::MPMemoryDataStream(const String& name, void* pMem, size_t size, 
				   bool freeOnClose)
	: MPDataStream(name)
{
	m_Data = m_Pos = static_cast<uchar*>(pMem);
	m_Size = size;
	m_End  = m_Data + m_Size;
	m_FreeOnClose = freeOnClose;
}
//-----------------------------------------------------------------------------
MPMemoryDataStream::MPMemoryDataStream(MPDataStream& sourceStream, bool freeOnClose)
	: MPDataStream()
{
	// ����һ������������
	m_Size = sourceStream.size();
	m_Data = new uchar[m_Size];
	sourceStream.read(m_Data, m_Size);
	m_Pos = m_Data;
	m_End = m_Data + m_Size;
	m_FreeOnClose = freeOnClose;
}
//-----------------------------------------------------------------------------
MPMemoryDataStream::MPMemoryDataStream(MPDataStreamPtr& sourceStream, bool freeOnClose)
	: MPDataStream()
{
	// ����һ������������
	m_Size = sourceStream->size();
	m_Data = new uchar[m_Size];
	sourceStream->read(m_Data, m_Size);
	m_Pos = m_Data;
	m_End = m_Data + m_Size;
	m_FreeOnClose = freeOnClose;
}
//-----------------------------------------------------------------------------
MPMemoryDataStream::MPMemoryDataStream(const String& name, MPDataStream& sourceStream, bool freeOnClose)
	: MPDataStream(name)
{
	// ����һ������������
	m_Size = sourceStream.size();
	m_Data = new uchar[m_Size];
	sourceStream.read(m_Data, m_Size);
	m_Pos = m_Data;
	m_End = m_Data + m_Size;
	m_FreeOnClose = freeOnClose;
}
//-----------------------------------------------------------------------------
MPMemoryDataStream::MPMemoryDataStream(const String& name, const MPDataStreamPtr& sourceStream, bool freeOnClose)
	: MPDataStream(name)
{
	// ����һ������������
	m_Size = sourceStream->size();
	m_Data = new uchar[m_Size];
	sourceStream->read(m_Data, m_Size);
	m_Pos = m_Data;
	m_End = m_Data + m_Size;
	m_FreeOnClose = freeOnClose;
}
//-----------------------------------------------------------------------------
MPMemoryDataStream::MPMemoryDataStream(size_t size, bool freeOnClose)
	: MPDataStream()
{
	m_Size = size;
	m_Data = new uchar[m_Size];
	m_Pos = m_Data;
	m_End = m_Data + m_Size;
	m_FreeOnClose = freeOnClose;
}
//-----------------------------------------------------------------------------
MPMemoryDataStream::MPMemoryDataStream(const String& name, size_t size, bool freeOnClose)
	: MPDataStream(name)
{
	m_Size = size;
	m_Data = new uchar[m_Size];
	m_Pos = m_Data;
	m_End = m_Data + m_Size;
	m_FreeOnClose = freeOnClose;
}
//-----------------------------------------------------------------------------
MPMemoryDataStream::~MPMemoryDataStream()
{
	close();
}
//-----------------------------------------------------------------------------
size_t MPMemoryDataStream::read(void* buf, size_t count)
{
	size_t cnt = count;

	if (m_Pos + cnt > m_End)
		cnt = m_End - m_Pos;
	if (cnt == 0)
		return 0;

	memcpy(buf, m_Pos, cnt);
	m_Pos += cnt;
	return cnt;
}
//-----------------------------------------------------------------------------
size_t MPMemoryDataStream::readLine(char* buf, size_t maxCount, const String& delim)
{
	// ���ﴦ��Unix��Windows���ļ������� DOS/Windows ���ı��ļ���ÿһ��ĩ
	// β��һ�� CR���س����� LF�����У����� UNIX �ı�ֻ��һ�����У�
	bool trimCR = false;
	if (delim.find_first_of('\n') != String::npos)
	{
		trimCR = true;
	}

	// ������һ���ָ���("\n")��λ��
	size_t pos = strcspn((const char*)m_Pos, delim.c_str());
	if (pos > maxCount)
		pos = maxCount;

	// ȷ��pos���ᳬ��eof
	if(m_Pos + pos > m_End) pos = m_End - m_Pos; 

	if (pos > 0)
	{
		memcpy(buf, (const void*)m_Pos, pos);
	}

	// ��������Ŀǰ��ָ��λ��
	m_Pos += pos + 1;

	// �޳�CR���س����� LF�����У�
	if (trimCR && buf[pos-1] == '\r')
	{
		// ɾ��'\r'
		--pos;
	}
	// ��ֹ��
	buf[pos] = '\0';

	return pos;
}
//-----------------------------------------------------------------------------
size_t MPMemoryDataStream::skipLine(const String& delim)
{
	// ������һ���ָ���("\n")��λ��
	size_t pos = strcspn( (const char*)m_Pos, delim.c_str() );

	// ȷ��pos���ᳬ��eof
	if(m_Pos + pos > m_End) pos = m_End - m_Pos; 

	m_Pos += pos + 1;

	return pos;
}
//-----------------------------------------------------------------------------
void MPMemoryDataStream::skip(long count)
{
	size_t newpos = (size_t)( ( m_Pos - m_Data ) + count );
	assert( m_Data + newpos <= m_End );        

	m_Pos = m_Data + newpos;
}
//-----------------------------------------------------------------------------
void MPMemoryDataStream::seek( size_t pos )
{
	assert( m_Data + pos <= m_End );
	m_Pos = m_Data + pos;
}
//-----------------------------------------------------------------------------
size_t MPMemoryDataStream::tell(void) const
{
	return m_Pos - m_Data;
}
//-----------------------------------------------------------------------------
bool MPMemoryDataStream::eof(void) const
{
	return m_Pos >= m_End;
}
//-----------------------------------------------------------------------------
void MPMemoryDataStream::close(void)
{
	if (m_FreeOnClose && m_Data)
	{
		delete [] m_Data;
		m_Data = 0;
	}
}
//=============================================================================
// MPFileStreamDataStream
//=============================================================================
MPFileStreamDataStream::MPFileStreamDataStream(std::ifstream* s, bool freeOnClose)
	: MPDataStream()
{
	// �����С
	m_pStream->seekg(0, std::ios_base::end);
	m_Size = m_pStream->tellg();
	m_pStream->seekg(0, std::ios_base::beg);
}
//-----------------------------------------------------------------------------
MPFileStreamDataStream::MPFileStreamDataStream(const String& name, std::ifstream* s, bool freeOnClose)
	: MPDataStream(name)
{
	// �����С
	m_pStream->seekg(0, std::ios_base::end);
	m_Size = m_pStream->tellg();
	m_pStream->seekg(0, std::ios_base::beg);
}
//-----------------------------------------------------------------------------
MPFileStreamDataStream::MPFileStreamDataStream(const String& name, std::ifstream* s, size_t size, bool freeOnClose)
	: MPDataStream(name)
{
	m_Size = size;
}
//-----------------------------------------------------------------------------
MPFileStreamDataStream::~MPFileStreamDataStream()
{
	close();
}
//-----------------------------------------------------------------------------
size_t MPFileStreamDataStream::read(void* buf, size_t count)
{
	m_pStream->read(static_cast<char*>(buf), count);
	return m_pStream->gcount();
}
//-----------------------------------------------------------------------------
size_t MPFileStreamDataStream::readLine(char* buf, size_t maxCount, const String& delim)
{
	if (delim.empty())
	{
		LG("No delimiter provided","FileStreamDataStream::readLine");
		assert(0);
	}
	if (delim.size() > 1)
	{
		LG("WARNING: FileStreamDataStream::readLine","using only first delimeter");
	}
	// ���ﴦ��Unix��Windows���ļ�
	bool trimCR = false;
	if (delim.at(0) == '\n') 
	{
		trimCR = true;
	}
	// ��Ϊ��������ֹ��������ҪmaxCount + 1
	m_pStream->getline(buf, maxCount+1, delim.at(0));
	size_t ret = m_pStream->gcount();
	// 3�����
	// 1) ������eof
	// 2) ��������Ŀռ�
	// 3) ��ȡ��һ���� - ��������·ָ��������У�������д�뵽���棬����
	//    ��ȡ���ݵĳ���Ϊret-1,��β��ret-2��
	// ����������涼��null��β

	if (m_pStream->eof()) 
	{
		// û����
	}
	else if (m_pStream->fail())
	{
		// Did we fail because of maxCount hit? No - no terminating character
		// in included in the count in this case
		if (ret == maxCount)
		{
			// clear failbit for next time 
			m_pStream->clear();
		}
		else
		{
			LG("Streaming error occurred", "FileStreamDataStream::readLine");
			assert(0);
		}
	}
	else 
	{
		--ret;
	}

	// �޳�CR���س����� LF�����У�
	if (trimCR && buf[ret-1] == '\r')
	{
		--ret;
		buf[ret] = '\0';
	}
	return ret;
}
//-----------------------------------------------------------------------------
size_t MPFileStreamDataStream::skipLine(const String& delim)
{
	size_t c = MP_STREAM_TEMP_SIZE-1;
	size_t total = 0;
	while (c == MP_STREAM_TEMP_SIZE-1)
	{
		c = readLine(m_TmpArea, MP_STREAM_TEMP_SIZE-1);
		total += c;
	}
	return total;
}
//-----------------------------------------------------------------------------
void MPFileStreamDataStream::skip(long count)
{
	m_pStream->clear(); //��һeof�Ѿ����ã������
	m_pStream->seekg(static_cast<std::ifstream::pos_type>(count), std::ios::cur);
}
//-----------------------------------------------------------------------------
void MPFileStreamDataStream::seek( size_t pos )
{
	m_pStream->clear(); //��һeof�Ѿ����ã������
	m_pStream->seekg(static_cast<std::ifstream::pos_type>(pos), std::ios::beg);
}
//-----------------------------------------------------------------------------
size_t MPFileStreamDataStream::tell(void) const
{
	m_pStream->clear(); //��һeof�Ѿ����ã������
	return m_pStream->tellg();
}
//-----------------------------------------------------------------------------
bool MPFileStreamDataStream::eof(void) const
{
	return m_pStream->eof();
}
//-----------------------------------------------------------------------------
void MPFileStreamDataStream::close(void)
{
	if (m_pStream)
	{
		m_pStream->close();
		if (m_FreeOnClose)
		{
			delete m_pStream;
			m_pStream = 0;
		}
	}
}
//=============================================================================
// MPFileHandleDataStream
//=============================================================================
MPFileHandleDataStream::MPFileHandleDataStream(FILE* handle)
	: MPDataStream(), m_FileHandle(handle)
{
	// �����С
	fseek(m_FileHandle, 0, SEEK_END);
	m_Size = ftell(m_FileHandle);
	fseek(m_FileHandle, 0, SEEK_SET);
}
//-----------------------------------------------------------------------------
MPFileHandleDataStream::MPFileHandleDataStream(const String& name, FILE* handle)
	: MPDataStream(name), m_FileHandle(handle)
{
	// �����С
	fseek(m_FileHandle, 0, SEEK_END);
	m_Size = ftell(m_FileHandle);
	fseek(m_FileHandle, 0, SEEK_SET);
}
//-----------------------------------------------------------------------------
MPFileHandleDataStream::~MPFileHandleDataStream()
{
	close();
}
//-----------------------------------------------------------------------------
size_t MPFileHandleDataStream::read(void* buf, size_t count)
{
	return fread(buf, count, 1, m_FileHandle);
}
//-----------------------------------------------------------------------------
size_t MPFileHandleDataStream::readLine(char* buf, size_t maxCount, const String& delim)
{
	// ���ﴦ��Unix��Windows���ļ�
	bool trimCR = false;
	if (delim.find_first_of('\n') != String::npos)
	{
		trimCR = true;
	}

	size_t chunkSize = min(maxCount, (size_t)MP_STREAM_TEMP_SIZE-1);
	size_t totalCount = 0;
	size_t readCount; 
	while (chunkSize && (readCount = fread(m_TmpArea, chunkSize, 1, m_FileHandle)))
	{
		// ��ֹ��
		m_TmpArea[readCount] = '\0';
		// ���ҵ�һ���ָ�����λ��
		size_t pos = strcspn(m_TmpArea, delim.c_str());

		if (pos < readCount)
		{
			// �ҵ��ָ�����������¶�λλ��
			fseek(m_FileHandle, pos - readCount + 1, SEEK_CUR);
		}

		if (pos > 0)
		{
			// ����ҵ�CR����ǰ��ֹ
			if (trimCR && m_TmpArea[pos-1] == '\r')
			{
				--pos;
			}

			if (buf)
			{
				memcpy(buf, (const void*)m_TmpArea, pos);
				buf[pos] = '\0';
			}
			totalCount += pos;
		}

		if (pos < readCount)
		{
			break;
		}
		// Ϊ�´β������µ������С
		chunkSize = min(maxCount-totalCount, (size_t)MP_STREAM_TEMP_SIZE-1);

	}
	return totalCount;
}
//-----------------------------------------------------------------------------
size_t MPFileHandleDataStream::skipLine(const String& delim)
{
	// ����readLine(), ������������
	char* nullBuf = 0;
	return readLine(nullBuf, 1024, delim);
}
//-----------------------------------------------------------------------------
void MPFileHandleDataStream::skip(long count)
{
	fseek(m_FileHandle, count, SEEK_CUR);
}
//-----------------------------------------------------------------------------
void MPFileHandleDataStream::seek( size_t pos )
{
	fseek(m_FileHandle, pos, SEEK_SET);
}
//-----------------------------------------------------------------------------
size_t MPFileHandleDataStream::tell(void) const
{
	return ftell( m_FileHandle );
}
//-----------------------------------------------------------------------------
bool MPFileHandleDataStream::eof(void) const
{
	return feof(m_FileHandle) != 0;
}
//-----------------------------------------------------------------------------
void MPFileHandleDataStream::close(void)
{
	fclose(m_FileHandle);
	m_FileHandle = 0;
}
