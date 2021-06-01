
#ifndef	SCENEOBJFILE_H
#define	SCENEOBJFILE_H

#define	OBJ_FILE_VER100	    100
#define	OBJ_FILE_VER200		200
#define	OBJ_FILE_VER300		300
#define	OBJ_FILE_VER400		400
#define	OBJ_FILE_VER500		500
#define	OBJ_FILE_VER600		600
#define MAX_MAP_SECTION_OBJ 25

struct ReallyBigObjectInfo;

struct SSceneObjInfo
{
    short	sTypeID;	// ��2λ��type(0: �������, 1: ��Ч���), ������ID
	int     nX;			// �������
	int  	nY;
	short	sHeightOff;
	short	sYawAngle;
    short	sScale;		// ����δʹ��

	short GetType()
	{
		return sTypeID >> (sizeof(short) * 8 - 2);
	}
	short GetID()
	{
		return ~(0x0003 << (sizeof(short) * 8 - 2)) & sTypeID;
	}
};

class	CSceneObjFile
{
	
	struct SFileHead
	{
		_TCHAR	tcsTitle[16];	// "HF Object File!"
		int		lVersion;
		long	lFileSize;

		int		iSectionCntX;	// ��ͼ�ĺ���������
		int		iSectionCntY;	// ��ͼ������������
		int		iSectionWidth;	// ����Ŀ�ȣ���λ��Tile��
		int		iSectionHeight; // ����ĸ߶ȣ���λ��Tile��
		int		iSectionObjNum;	// �����������������
	};

	struct SSectionIndex
	{
		// obj
		long	lObjInfoPos;
		int		iObjNum;
	};
	friend class CObjInfluence;

public:
	CSceneObjFile();
	~CSceneObjFile();
	long	Init(_TCHAR *ptcsFileName, bool bSilence = true);
	void	Free(void);
	long	CreateFile(_TCHAR *ptcsFileName, int iSectionCntX = 512, int iSectionCntY = 512,
			int iSectionWidth = 8, int iSectionHeight = 8, int iSectionObjNum = MAX_MAP_SECTION_OBJ);
	long	ConvertObjFileVer(_TCHAR *ptcsFile, bool bBackUp = true); // �Ӱ汾300ת����400
	long	ReadSectionObjInfo(int nSectionNO, SSceneObjInfo *SSceneObj, long *lSectionObjNum);
	long	WriteSectionObjInfo(int nSectionNO, SSceneObjInfo *SSceneObj, long lSectionObjNum);
	long	TrimFile(_TCHAR *ptcsFileName, bool bBackUp);
	long	TrimDirectory(_TCHAR *ptcsDirectory, bool bBackUp);

	long	GetSectionObjNub(int nSectionNO, long *lSectionObjNum)
	{
		if (!m_bInitSuccess)
			return 0;

		if (nSectionNO >= m_SFileHead.iSectionCntX * m_SFileHead.iSectionCntY)
			return 0;

		*lSectionObjNum = m_SSectionIndex[nSectionNO].iObjNum;
		return 1;
	}

	SFileHead *	GetSFileHead()
	{
		if (m_bInitSuccess)
			return (&m_SFileHead);
		else
			return NULL;
	};

protected:
	bool			m_bInitSuccess;
	FILE			*m_fRdWr, *m_fAppend;
	SFileHead		m_SFileHead;
	SSectionIndex	*m_SSectionIndex;

private:
	// Added by CLP
	// RBO == Really Big Object
	std::string filename_RBO;
	std::set < struct ReallyBigObjectInfo > RBOinfoList;
	void _init_RBO( const std::string &filename );
	void _Serialize_RBO();
	void _Serialize_RBO_ToMap();
public:
	void end_RBO();
	std::set < struct ReallyBigObjectInfo > & GetRBOinfoList()
	{
		return RBOinfoList;
	}
};

extern	CSceneObjFile	g_ObjFile;

#endif //SCENEOBJFILE_H
