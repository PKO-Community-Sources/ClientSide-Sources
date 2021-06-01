#ifndef MPStringUtil_H
#define MPStringUtil_H

#include "MPEffPrerequisites.h"
#include "MPStringVector.h"

class MINDPOWER_API MPStringUtil
{
public:
	typedef std::stringstream StrStreamType;

public:
	/** ��ȥ�κοո��ַ�, ��׼�ո�����ո��Tabsbe�ȵ�.
	@remarks
	����ָ���Ƿ���ȥǰ�˻����Ŀո�Ĭ�϶���ȥ.
	*/
	static void trim( String& str, bool left = true, bool right = true );

	/** ����MPStringVector�������ڷָ���֮��������Ӵ�.
	@param 
	delims �ָ����б�
	@param 
	maxSplits ִ�зָ��������������Ϊ0�����޴ηָ�������ָ���������0
	��ָ������Ӵ������ﵽ�����Ϊֹ.
	*/
	static MPStringVector split( const std::string& str, const std::string& delims = "\t\n ", unsigned int maxSplits = 0);

	/** ���ַ����е��ַ�ȫ��ת����Сд.
	*/
	static void toLowerCase( String& str );

	/** ���ַ����е��ַ�ȫ��ת���ɴ�дд.
	*/
	static void toUpperCase( String& str );

};
#endif