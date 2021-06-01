#pragma once
/************************************************************************/
/*����ļ���:���һ����ǰ�����µ�ĳ��Ŀ¼����Ŀ¼���ļ���
/*			�Լ��Ӵ���ļ��ж�ȡ�ļ����ݡ�
/*create by lemon@2005.6.3
/************************************************************************/
class CPackFile
{
public:
	CPackFile();
	
	~CPackFile();
	

	struct FileData  // �ļ�����
	{
		s_string strFileName; // �ļ���
		DWORD offset;   //
		DWORD length;  // �ļ�����
	};

	struct DirectoryData // �ļ�·������
	{
		s_string  strDirName; // �ļ�·��
		DWORD dwDirNum;  // �ļ�Ŀ¼��
		DWORD dwFileNum; // �ļ���
		std::vector<DirectoryData>	vecSubDir; // ����DirectoryData()�Զ�Ӧ����vecSubDir��
		std::vector<FileData>	vecFile; // ����FileData()�Զ�Ӧ����vecFile��
	};

	bool	SaveToPack(char* pszDirName, char* pszOutFile, char* pszFilter = "*.bmp"); // ���ļ�������

	bool	LoadPackFile(char* pszFile); // ������ļ�����

	void	Clear(); 

	DirectoryData*	GetDir(char* pszDirName) // ��ȡ����ļ���Ŀ¼
	{
		if(!_pPackFile)  
			return NULL;
		if(_RootDir.strDirName == pszDirName)
			return &_RootDir;
		return FindDir(pszDirName, &_RootDir);
	}
protected:
	bool	PackDirectory(FILE* ptf,DirectoryData* pParentDir, char* pszDir); // ������ļ�����Ŀ¼
	long	AddFileToTempPack(FILE* pf, char* pszFileName); // �����ļ�������ļ���
	long	GetFileDataSize(FileData* pFileData); // ��ȡ�ļ����ݴ�С
	void	SaveFileData(FILE* pf, FileData* pFileData, long offset); //�����ļ�����
	long	GetDirectorySize(DirectoryData* pDir); // ����ļ�Ŀ¼���ݴ�С
	void	SaveDirData(FILE* pf, DirectoryData* pDir, long offset); // ����Ŀ¼����


	bool	LoadFileData(FileData* pFileData); // ���ļ�����				
	bool	LoadDirData(DirectoryData* pDir);  // ��Ŀ¼����	

	DirectoryData* FindDir(char* pszDirName, DirectoryData* pDir)  // �����ļ����Ŀ¼
	{
		if(pDir->strDirName == pszDirName)
			return pDir;
		WORD n;
		DirectoryData* ptDir;
		for (n = 0; n < pDir->dwDirNum; ++n)
		{
			if(ptDir = FindDir(pszDirName, &pDir->vecSubDir[n]))
				return ptDir;
		}
		return NULL;
	}
public:
	//��ǰ��pack�ļ���
	s_string strName;
	s_string strFilter;

protected:

	FILE*			_pPackFile; // Ҫ������ļ�
	DirectoryData	_RootDir; // Ҫ�������ļ���Ŀ¼
}; 


class CMiniPack : public CPackFile // ������
{
public:
	CMiniPack();
	~CMiniPack();

	bool 	Init(char* pszMapName);
	void	GetXY( char*pszName, int& x,int& y);
	bool	IsExit(int x,int y)
	{
		if(x >= _imaxX || y >= _imaxY)
			return false;
		if(!_pIdx[x + y * _imaxX])
		{
			return false;
		}
		return true;
	}
	void	GetTex(MPITex**	pTex, int x,int y)
	{
		int idx = x + y * _imaxX;
		if( idx > _imaxX * _imaxY)
		{
			*pTex = NULL;
			return;
		}
		FileData*	   _pFile = _pIdx[idx];
		if(!_pFile)
		{
			*pTex = NULL;
			return;
		}
		if(!_pTData || _lsize != _pFile->length)
		{
			SAFE_DELETE_ARRAY(_pTData);
			_pTData = new BYTE[_pFile->length];
			_lsize = _pFile->length;
		}
		fseek(_pPackFile,_pFile->offset, SEEK_SET); 
		fread(_pTData, 1,_pFile->length,_pPackFile);

		texInfo.width = _pFile->length;
		texInfo.data = _pTData;

		lwLoadTex(pTex,g_Render.GetInterfaceMgr()->res_mgr, &texInfo);
	}
public:
	s_string	strMapName;
	lwTexInfo	texInfo;

protected:
	struct sXY
	{
		int x,y;
	};

	BYTE*		    _pTData;
	long			_lsize;
	FileData**	   _pIdx;
	DirectoryData* _pCurDir;
	int			  _imaxX, _imaxY;

};
