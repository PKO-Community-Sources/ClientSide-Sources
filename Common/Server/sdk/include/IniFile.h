#ifndef INIFILE_H
#define INIFILE_H

#include "excp.h"

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)

struct IniItem{
	dstring	name;
	dstring	value;
};
class IniSection{
friend class IniFile;
public:
	int	ItemCount(){return m_itemcount;}
	IniItem & operator[](int);
	dstring & operator[](const char *name)const;
private:
	IniSection():m_itemcount(0),m_itemsize(0),m_item(0){}
	~IniSection();

	IniItem * AddItem(const char *name,const char *value);

	struct{
		int	m_itemcount;
		int	m_itemsize;
		IniItem ** m_item;
		dstring	m_sectname;
	};
};

class IniFile{
public:
	IniFile(const char *filename);
	~IniFile();

	int	SectCount(){return m_sectcount;}
	IniSection & operator[](int);
	IniSection & operator[](const char *sectname)const;
private:
	void Flush();
	void ReadFile(const char *filename);
	IniSection * AddSection(const char *sectname);

	struct{
		int	m_sectcount;
		int	m_sectsize;
		IniSection ** m_sect;
	};
	bool	m_update;
	bool	m_rw;					//false:Ö»¶Á;true:¶ÁºÍÐ´¡£
	char	m_filename[512];
};

#pragma pack(pop)
_DBC_END

#endif

