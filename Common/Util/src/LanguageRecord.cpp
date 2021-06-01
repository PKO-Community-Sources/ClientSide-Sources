
#include "LanguageRecord.h"
#include <io.h>

__byte ENC_KEY[] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};


CLanguageRecord::CLanguageRecord(void)
{
}

CLanguageRecord::CLanguageRecord(const char* szBinFile, const char* szTxtFile)
{
	if(! LoadFromBinFile(szBinFile))
	{
		LoadFromTxtFile(szTxtFile);
	}
}


CLanguageRecord::~CLanguageRecord(void)
{
}


// ����ַ���
const char* CLanguageRecord::GetString(int nID)
{
	std::map<int, std::string>::iterator it = m_mapString.find(nID);

	if(it != m_mapString.end())
	{
		// �ҵ���Ӧ�ַ���
		return it->second.c_str();
	}

	// δ�ҵ���Ӧ�ַ���
	return "";
}


// �� BIN �ļ��ж�ȡ��BIN -> TXT��
bool CLanguageRecord::LoadFromBinFile(const char* szBinFile)
{
	m_mapString.clear();

	std::ifstream fs;
	fs.open(szBinFile);

	if(! fs.is_open())
		return false;

	char szBuffer[1024];
	char szNewBuf[1024];

	SetEncKey(ENC_KEY);

	while(fs.getline(szBuffer, sizeof(szBuffer) / sizeof(szBuffer[0])))
	{
		// �Ƚ���
		Decrypt((__byte*)szNewBuf, 1024, (__byte*)szBuffer, strlen(szBuffer));

		Add(szNewBuf);
	}

	return true;
}


// �� TXT �ļ��ж�ȡ
//
//	[0] <TAB> "�ַ���"
//
bool CLanguageRecord::LoadFromTxtFile(const char* szTxtFile)
{
	m_mapString.clear();

	std::ifstream fs;
	fs.open(szTxtFile);

	if(! fs.is_open())
		return false;

	char szBuffer[1024];
	while(fs.getline(szBuffer, sizeof(szBuffer) / sizeof(szBuffer[0])))
	{
		Add(szBuffer);
	}

	return true;
}


// ���ܲ����ɶ����ƣ�TXT -> BIN��
bool CLanguageRecord::MadeBinFile(const char* szBinFile, const char* szTxtFile)
{
	if(-1 != access(szBinFile, 0))	// BIN �ļ��Ѵ���
		return false;

	std::ifstream ifs;
	std::ofstream ofs;
	ifs.open(szTxtFile);
	ofs.open(szBinFile);

	if(! ifs.is_open() || ! ofs.is_open())
		return false;

	SetEncKey(ENC_KEY);
	char szBuffer[1024];
	char szBinBuf[1024];
	while(ifs.getline(szBuffer, sizeof(szBuffer) / sizeof(szBuffer[0])))
	{
		Encrypt((__byte*)szBinBuf, 1024, (__byte*)szBuffer, strlen(szBuffer) + 1);

		ofs.write(szBinBuf, strlen(szBinBuf));
		ofs.write("\n", 1);
	}

	ofs.flush();

	return false;
}


// ����ַ�������
int CLanguageRecord::GetRecordCount(void)
{
	return (int)(m_mapString.size());
}


// ���һ��
void CLanguageRecord::Add(char* szLine)
{
	// �滻��Դ�ļ����ת���
	ReplaceString(szLine, "\\n", "\n");
	ReplaceString(szLine, "\\t", "\t");

	int nPos, nID;
	std::string strLine;
	std::string strID;

	strLine = szLine;
	if(7 >= strLine.size() || (nPos = (int)strLine.find("\t")) <= -1)
	{
		return;
	}

	strID   = strLine.substr(0, nPos);
	strLine = strLine.substr(nPos + 1, strLine.size() - nPos - 1);

	if(2 < strID.size() && strID[0] == '[' && strID[(int)strID.size() - 1] == ']' &&
		2 < strLine.size() && strLine[0] == '\"' && strLine[(int)strLine.size() - 1] == '\"')
	{
		nID = atoi(strID.substr(1, (int)strID.size() - 2).c_str());
		m_mapString[nID] = strLine.substr(1, (int)strLine.size() - 2);
	}
}


// �滻
int CLanguageRecord::ReplaceString(char* _str, const char* _old, const char* _new)
{
    int __pos = 0;
	__pos = Find(_str, _old);
    if(!__pos)
    {
        return 0;
    }

    char* __ptr = 0;
    int __oldlen = (int)strlen(_old);
    int __newlen = (int)strlen(_new);
    int __other = 0;

    __ptr = _str + (__pos - 1);
    __other = (int)strlen(__ptr + __oldlen);

    char* __tmp = new char[__other + 1];

    strcpy(__tmp, __ptr + __oldlen);
    strcpy(__ptr, _new);

    __ptr += __newlen;

    strcpy(__ptr, __tmp);

	if(__tmp) delete[] (__tmp);

    return ReplaceString(__ptr, _old, _new) + 1;
}


// �滻
int CLanguageRecord::Find(const char* _str, const char* _find)
{
    char* __ptr = 0;

    __ptr = strstr(_str, _find);
    if(__ptr)
    {
        return (int)(__ptr - _str + 1);
    }
    return 0;
}

