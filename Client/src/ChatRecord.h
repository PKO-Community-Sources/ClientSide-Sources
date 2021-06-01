#pragma once

class CChatRecord
{
public:
	CChatRecord(void);
	~CChatRecord(void);
	static bool Save(const string name, DWORD number, const string chatData);
	static string GetLastSavePath();

private:
	static string m_strPath;
};
