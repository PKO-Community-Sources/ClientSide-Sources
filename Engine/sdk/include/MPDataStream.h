#ifndef MPDataStream_H
#define MPDataStream_H

#include "MPEffPrerequisites.h"
#include "MPSharedPtr.h"

/** ͨ�������ڰ�װ���������.
@remarks
This class performs basically the same tasks as std::basic_istream, 
except that it does not have any formatting capabilities, and is
designed to be subclassed to receive data from multiple sources,
including libraries which have no compatiblity with the STL's
stream interfaces. As such, this is an abstraction of a set of 
wrapper classes which pretend to be standard stream classes but 
can actually be implemented quite differently. 
@par
	ͨ����������Ӧ�ó����ṩArchiveFactory����ôҲӦ�ṩ���Է�������
Archive��������MPDataStream�������࣬����Ĭ���ṩ�Ŀ�������Ҫ��
@note
	�������ܵ�Ҫ�����ﲻ��֤�����̰߳�ȫ�ġ������ϣ���첽����������
������Լ���֯�����������⾺��
*/
class MINDPOWER_API MPDataStream
{
public:
	/// ���캯������������������
	MPDataStream() : m_Size(0) {}
	/// ���캯������������������
	MPDataStream(const String& name) : m_Name(name), m_Size(0) {}
	virtual ~MPDataStream() {}

	// ������
	template<typename T> MPDataStream& operator>>(T& val);
	/** �����ж�ȡ�����ֽڵ�����, ������eof��ֹͣ.
	@param buf �����ָ��
	@param count ��Ҫ��ȡ���ֽ���
	@returns ��ȡ���ֽ���
	*/
	virtual size_t read(void* buf, size_t count) = 0;
	/** �����еõ�һ������.
	@remarks
		�ָ����������ڷ��ص�������,���һᱻ�����������´ζ�ȡ��ʱ��Ҳ��
	�����.�����н�������ֹ��.
	@param buf �����ָ��
	@param maxCount ��ȡһ�����ݵ���󳤶ȣ���������ֹ��
	@param delim �ָ���
	@returns ��ȡ���ֽ�������������ֹ��
	*/
	virtual size_t readLine(char* buf, size_t maxCount, const String& delim = "\n") = 0;

	/** Returns a String containing the next line of data, optionally 
	trimmed for whitespace. 
	@remarks
	This is a convenience method for text streams only, allowing you to 
	retrieve a String object containing the next line of data. The data
	is read up to the next newline character and the result trimmed if
	required.
	@param 
	trimAfter If true, the line is trimmed for whitespace (as in 
	String.trim(true,true))
	*/
	virtual String getLine( bool trimAfter = true );

	/** ���ַ�������ʽ����������������. */
	virtual String getAsString(void);

	/** ����������һ��.
	@param delim �зָ���
	@returns �����������ֽ���
	*/
	virtual size_t skipLine(const String& delim = "\n") = 0;

	/** �����ƶ����ֽ���,Ҳ������һ����ֵ����ʱ����ǰ�����ƶ��ֽڵ�����. */
	virtual void skip(long count) = 0;

	/** ������ȡָ���Ƶ�ָ�����ֽڴ�.
	*/
	virtual void seek( size_t pos ) = 0;

	/** ����Ŀǰָ�뵽ͷ��ƫ�� */
	virtual size_t tell(void) const = 0;

	/** �������eof������true.*/
	virtual bool eof(void) const = 0;

	/** �ر���. */
	virtual void close(void) = 0;

	/** �����������ݵĴ�С�������ȷ������Ϊ0. */
	size_t size(void) const { return m_Size; }
	/// �����������֣�����еĻ�.
	const String& getName(void) const { return m_Name; }

protected:
	/// ���ڱ�ʶ��Դ�����֣�������Դ����(��ѡ)
	String m_Name;		
	/// �������ݵĴ�С(������ܾ������ݵĴ�С����Ϊ0)
	size_t m_Size;
#define MP_STREAM_TEMP_SIZE 128
	/// ��ʽ��ȡ����ʱ������
	char m_TmpArea[MP_STREAM_TEMP_SIZE];
};

typedef MPSharedPtr<MPDataStream> MPDataStreamPtr;
typedef std::list<MPDataStreamPtr> MPDataStreamList;
typedef MPSharedPtr<MPDataStreamList> MPDataStreamListPtr;

/** DataStream�Ĺ��������࣬���ڴ����ڴ��е�������.*/
class MINDPOWER_API MPMemoryDataStream : public MPDataStream
{
public:

	/** �����е��ڴ���װ����.
	@param pMem �ִ��ڴ��ָ��
	@param size �ڴ��Ĵ�С���ֽڣ�
	@param freeOnClose ���Ϊtrue�������ͷ�ʱ���ڴ潫�ᱻ�ͷ�.
	*/
	MPMemoryDataStream(void* pMem, size_t size, bool freeOnClose = false);
	/** �����е��ڴ���װ����������.
	@param name ��������
	@param pMem �ִ��ڴ��ָ��
	@param size �ڴ��Ĵ�С���ֽڣ�
	@param freeOnClose ���Ϊtrue�������ͷ�ʱ���ڴ潫�ᱻ�ͷ�.
	*/
	MPMemoryDataStream(const String& name, void* pMem, size_t size, 
		bool freeOnClose = false);
	/** ������һ�����Ĵ���һ����.
	@param sourceStream ��һ��������
	@param freeOnClose ���Ϊtrue�������ͷ�ʱ���ڴ潫�ᱻ�ͷ�.
	*/
	MPMemoryDataStream(MPDataStream& sourceStream, bool freeOnClose = true);
	/** ������һ�����Ĵ���һ����.
	@param sourceStream ��һ��������(������)
	@param freeOnClose ���Ϊtrue�������ͷ�ʱ���ڴ潫�ᱻ�ͷ�.
	*/
	MPMemoryDataStream(MPDataStreamPtr& sourceStream, bool freeOnClose = true);
	/** ������һ�����Ĵ���һ����������.
	@param name ��������
	@param sourceStream ��һ��������
	@param freeOnClose ���Ϊtrue�������ͷ�ʱ���ڴ潫�ᱻ�ͷ�.
	*/
	MPMemoryDataStream(const String& name, MPDataStream& sourceStream, 
		bool freeOnClose = true);
	/** ������һ�����Ĵ���һ����������.
	@param name ��������
	@param sourceStream ��һ��������(������)
	@param freeOnClose ���Ϊtrue�������ͷ�ʱ���ڴ潫�ᱻ�ͷ�.
	*/
	MPMemoryDataStream(const String& name, const MPDataStreamPtr& sourceStream, 
		bool freeOnClose = true);
	/** ����һ���յ���.
	@param size ���Ĵ�С
	@param freeOnClose ���Ϊtrue�������ͷ�ʱ���ڴ潫�ᱻ�ͷ�.
	*/
	MPMemoryDataStream(size_t size, bool freeOnClose = true);
	/** ����һ���յ���������.
	@param name ��������
	@param size ���Ĵ�С
	@param freeOnClose ���Ϊtrue�������ͷ�ʱ���ڴ潫�ᱻ�ͷ�.
	*/
	MPMemoryDataStream(const String& name, size_t size, bool freeOnClose = true);

	~MPMemoryDataStream();

	/** �õ�������ڴ�����ʼָ��. */
	uchar* getPtr(void) { return m_Data; }

	/** �õ������Ŀǰָ���ָ��. */
	uchar* getCurrentPtr(void) { return m_Pos; }

	/** �����Ƿ������ر�ʱ�ͷ��ڴ�. */
	void setFreeOnClose(bool free) { m_FreeOnClose = free; }

	/** @copydoc DataStream::read		*/
	size_t read(void* buf, size_t count);
	/** @copydoc DataStream::readLine	*/
	size_t readLine(char* buf, size_t maxCount, const String& delim = "\n");
	/** @copydoc DataStream::skipLine	*/
	size_t skipLine(const String& delim = "\n");
	/** @copydoc DataStream::skip		*/
	void skip(long count);
	/** @copydoc DataStream::seek		*/
	void seek( size_t pos );
	/** @copydoc DataStream::tell		*/
	size_t tell(void) const;
	/** @copydoc DataStream::eof		*/
	bool eof(void) const;
	/** @copydoc DataStream::close		*/
	void close(void);

protected:
	/// ָ��������ͷ��ָ��
	uchar* m_Data;
	/// ָ��Ŀǰ����λ�õ�ָ��
	uchar* m_Pos;
	/// ָ���������β��ָ��
	uchar* m_End;
	/// ���ر�ʱ�Ƿ�ɾ������
	bool m_FreeOnClose;			
};

typedef MPSharedPtr<MPMemoryDataStream> MPMemoryDataStreamPtr;

/** DataStream�Ĺ���������,���ڴ���std::basic_istream������.*/
class MINDPOWER_API MPFileStreamDataStream : public MPDataStream
{
public:
	/** �ӱ�׼��stl��������
	@param s stl����ָ��
	@param freeOnClose ���Ϊtrue�������ͷ�ʱ���ڴ潫�ᱻ�ͷ�.
	*/
	MPFileStreamDataStream(std::ifstream* s, bool freeOnClose = true);
	/** �ӱ�׼��stl������������
	@param name ��������
	@param s stl����ָ��
	@param freeOnClose ���Ϊtrue�������ͷ�ʱ���ڴ潫�ᱻ�ͷ�.
	*/
	MPFileStreamDataStream(const String& name, std::ifstream* s, bool freeOnClose = true);
	/** �ӱ�׼��stl������������, ����֪���С
	@remarks
	This variant tells the class the size of the stream too, which 
	means this class does not need to seek to the end of the stream 
	to determine the size up-front. This can be beneficial if you have
	metadata about the contents of the stream already.
	@param name ��������
	@param s stl����ָ��
	@param size ���Ĵ�С���ֽڣ�
	@param freeOnClose ���Ϊtrue�������ͷ�ʱ���ڴ潫�ᱻ�ͷ�.
	*/
	MPFileStreamDataStream(const String& name, std::ifstream* s, size_t size, bool freeOnClose = true);

	~MPFileStreamDataStream();

	/** @copydoc DataStream::read		*/
	size_t read(void* buf, size_t count);
	/** @copydoc DataStream::readLine	*/
	size_t readLine(char* buf, size_t maxCount, const String& delim = "\n");
	/** @copydoc DataStream::skipLine	*/
	size_t skipLine(const String& delim = "\n");
	/** @copydoc DataStream::skip		*/
	void skip(long count);
	/** @copydoc DataStream::seek		*/
	void seek( size_t pos );
	/** @copydoc DataStream::tell		*/
	size_t tell(void) const;
	/** @copydoc DataStream::eof		*/
	bool eof(void) const;
	/** @copydoc DataStream::close		*/
	void close(void);

protected:
	std::ifstream* m_pStream;
	bool m_FreeOnClose;			
};

/** DataStream�Ĺ���������,���ڴ���C�����ļ����������.*/
class MINDPOWER_API MPFileHandleDataStream : public MPDataStream
{
public:
	/// ��c����ļ���ʽ������
	MPFileHandleDataStream(FILE* handle);
	/// ��c����ļ���ʽ����������
	MPFileHandleDataStream(const String& name, FILE* handle);
	~MPFileHandleDataStream();

	/** @copydoc DataStream::read		*/
	size_t read(void* buf, size_t count);
	/** @copydoc DataStream::readLine	*/
	size_t readLine(char* buf, size_t maxCount, const String& delim = "\n");
	/** @copydoc DataStream::skipLine	*/
	size_t skipLine(const String& delim = "\n");
	/** @copydoc DataStream::skip		*/
	void skip(long count);
	/** @copydoc DataStream::seek		*/
	void seek( size_t pos );
	/** @copydoc DataStream::tell		*/
	size_t tell(void) const;
	/** @copydoc DataStream::eof		*/
	bool eof(void) const;
	/** @copydoc DataStream::close		*/
	void close(void);

protected:
	FILE* m_FileHandle;
};


#endif