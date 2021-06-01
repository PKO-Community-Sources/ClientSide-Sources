#include "StdAfx.h"
#include "packfile.h"

//---------------------------------------------------------------------------
// class CPackFile
//---------------------------------------------------------------------------
CPackFile::CPackFile(void) // �ļ����
{
	_pPackFile = NULL;
}

CPackFile::~CPackFile(void) 
{
	Clear();
}

bool	CPackFile::SaveToPack(char* pszDirName, char* pszOutFile, char* pszFilter)
{
	FILE* ptempfile = fopen("temp.tmp","wb"); // �򿪿ɶ�д�ļ�
	if(!ptempfile)
		return false;

	strFilter = pszFilter;
	if(!PackDirectory(ptempfile, NULL, pszDirName))
	{
		goto __ret;
	}

	fseek( ptempfile, 0, SEEK_SET );

	long	dwOffset = GetDirectorySize(NULL);

	FILE* pf = fopen(pszOutFile,"wb");
	if(!pf)
	{
		goto __ret;
	}

	SaveDirData(pf, NULL, dwOffset);

	long lpos = ftell(pf);

	if(lpos != dwOffset)
		goto __ret;

	fclose(ptempfile);
	ptempfile = fopen("temp.tmp","rb");
	fseek( ptempfile, 0, SEEK_END );
	long size = ftell( ptempfile );
	fseek( ptempfile, 0, SEEK_SET );

	BYTE* pData = new BYTE[size];
	fread(pData,size,1,ptempfile);

	fwrite(pData,size,1,pf);
	delete []pData;

	fclose(pf);
__ret:
	if(pf)
		fclose(pf);
	if(ptempfile)
		fclose(ptempfile);
	::DeleteFile("temp.tmp");

	return true;
}

bool	CPackFile::LoadPackFile(char* pszFile)
{
	Clear();
	_pPackFile = fopen(pszFile, "rb");
	if(!_pPackFile)
		return false;

	if(!LoadDirData(NULL))
		return false;
	return true;
}

void	CPackFile::Clear() // ����ļ���Ϣ���
{
	if(_pPackFile)
		fclose(_pPackFile);

	_RootDir.vecFile.clear();
	_RootDir.vecSubDir.clear();
	_RootDir.dwDirNum = 0;
	_RootDir.dwFileNum = 0;
	_RootDir.strDirName = "";
}

bool	CPackFile::PackDirectory(FILE* ptf,DirectoryData* pParentDir, char* pszDir)
{
	DirectoryData* ptDir;
	if(!pParentDir)
	{
		ptDir = &_RootDir;
	}
	else 
		ptDir = pParentDir;

	ptDir->dwDirNum = 0;
	ptDir->dwFileNum = 0;
	ptDir->strDirName = pszDir;

	char t_Path[MAX_PATH];
	char t_FilePath[MAX_PATH];

	WIN32_FIND_DATA t_sfd;
	HANDLE  t_hFind = NULL;

	DirectoryData  tDirData;
	FileData		tFileData;
	tFileData.offset = 0;
	tFileData.length = 0;

	sprintf(t_Path,"%s\\%s",pszDir,strFilter.c_str());

	if((t_hFind=FindFirstFile(t_Path,&t_sfd))==INVALID_HANDLE_VALUE)
		return false;
	do{
		if(t_sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(strcmp(t_sfd.cFileName,".") == 0)
				continue;
			if(strcmp(t_sfd.cFileName,"..") == 0)
				continue;

			sprintf(t_FilePath,"%s\\%s",pszDir,t_sfd.cFileName);

			ptDir->vecSubDir.push_back(tDirData);
			PackDirectory( ptf, &ptDir->vecSubDir[ptDir->dwDirNum], t_FilePath);
			ptDir->dwDirNum++;
		}else
		{
			tFileData.strFileName = t_sfd.cFileName;
			tFileData.offset = ftell(ptf);

			sprintf(t_FilePath,"%s\\%s",pszDir,t_sfd.cFileName);

			tFileData.length = AddFileToTempPack(ptf,t_FilePath);

			ptDir->vecFile.push_back(tFileData);
			ptDir->dwFileNum++;
		}

	}while(FindNextFile(t_hFind,&t_sfd));
	FindClose(t_hFind);

	return true;
}

long	CPackFile::AddFileToTempPack(FILE* pf, char* pszFileName)
{
	FILE* pfile = fopen(pszFileName, "rb");
	if(!pfile)
		return 0;
	fseek( pfile, 0, SEEK_END );
	long size = ftell( pfile );

	fseek( pfile, 0, SEEK_SET );

	BYTE* pData = new BYTE[size];
	fread(pData,  size, 1, pfile);

	fwrite(pData, size, 1, pf);

	fclose(pfile);
	delete [] pData;
	return size;
}

long	CPackFile::GetFileDataSize(FileData* pFileData)
{
	long size = sizeof(DWORD) * 2 +  pFileData->strFileName.length() + 1;

	return size;
}
void	CPackFile::SaveFileData(FILE* pf, FileData* pFileData, long offset)
{
	BYTE bylen = (BYTE)pFileData->strFileName.length();
	fwrite(&bylen,sizeof(BYTE),1,pf);
	fwrite(pFileData->strFileName.c_str(),sizeof(char),bylen,pf);

	pFileData->offset += offset;
	fwrite(&pFileData->offset, sizeof(DWORD), 1, pf);
	fwrite(&pFileData->length, sizeof(DWORD), 1, pf);
}
long	CPackFile::GetDirectorySize(DirectoryData* pDir)
{
	DirectoryData* ptDir;
	if(!pDir)
	{
		ptDir = &_RootDir;
	}
	else 
		ptDir = pDir;

	long size = sizeof(DWORD) * 2  +  ptDir->strDirName.length() + 1;

	WORD n = 0;
	for (; n < ptDir->dwFileNum; ++n)
	{
		size += GetFileDataSize(&ptDir->vecFile[n]);
	}
	for (n = 0; n < ptDir->dwDirNum; ++n)
	{
		size += GetDirectorySize(&ptDir->vecSubDir[n]);
	}
	return size;
}
void	CPackFile::SaveDirData(FILE* pf, DirectoryData* pDir, long offset)
{
	DirectoryData* ptDir;
	if(!pDir)
	{
		ptDir = &_RootDir;
	}
	else 
		ptDir = pDir;

	BYTE bylen = (BYTE)ptDir->strDirName.length();
	fwrite(&bylen,sizeof(BYTE),1,pf);
	fwrite(ptDir->strDirName.c_str(),sizeof(char),bylen,pf);

	fwrite(&ptDir->dwFileNum, sizeof(DWORD), 1, pf);
	fwrite(&ptDir->dwDirNum, sizeof(DWORD), 1, pf);

	WORD n = 0;
	for (; n < ptDir->dwFileNum; ++n)
	{
		SaveFileData(pf, &ptDir->vecFile[n], offset);
	}
	for (n = 0; n < ptDir->dwDirNum; ++n)
	{
		SaveDirData(pf, &ptDir->vecSubDir[n], offset);
	}
}

bool	CPackFile::LoadFileData(FileData* pFileData)
{
	BYTE bylen;
	fread(&bylen,sizeof(BYTE),1,_pPackFile);
	char* pszName = new char[bylen + 1];
	fread(pszName,sizeof(char),bylen,_pPackFile);
	pszName[bylen] = 0;
	pFileData->strFileName = pszName;
	delete []pszName;

	fread(&pFileData->offset, sizeof(DWORD), 1, _pPackFile);
	fread(&pFileData->length, sizeof(DWORD), 1, _pPackFile);

	return true;
}
bool	CPackFile::LoadDirData(DirectoryData* pDir)
{
	DirectoryData* ptDir;
	if(!pDir)
		ptDir = &_RootDir;
	else 
		ptDir = pDir;


	BYTE bylen;
	fread(&bylen,sizeof(BYTE),1,_pPackFile);
	char* pszName = new char[bylen + 1];
	fread(pszName,sizeof(char),bylen,_pPackFile);
	pszName[bylen] = 0;
	ptDir->strDirName = pszName;
	delete []pszName;

	fread(&ptDir->dwFileNum, sizeof(DWORD), 1, _pPackFile);
	fread(&ptDir->dwDirNum, sizeof(DWORD), 1, _pPackFile);

	ptDir->vecFile.resize(ptDir->dwFileNum);
	ptDir->vecSubDir.resize(ptDir->dwDirNum);

	WORD n = 0;
	for (; n < ptDir->dwFileNum; ++n)
	{
		LoadFileData(&ptDir->vecFile[n]);
	}
	for (n = 0; n < ptDir->dwDirNum; ++n)
	{
		LoadDirData(&ptDir->vecSubDir[n]);
	}

	return true;
}


//---------------------------------------------------------------------------
// class CMiniPack
//---------------------------------------------------------------------------
CMiniPack::CMiniPack()
{
	_pIdx = NULL;
	_pCurDir = NULL;
	strMapName = "";
	_pTData = NULL;
	_lsize = 0;

	texInfo.type = TEX_TYPE_DATA;
	texInfo.usage = 0;
	texInfo.level = 1;
	texInfo.format = D3DFMT_A4R4G4B4;
	texInfo.pool = D3DPOOL_MANAGED;

	_imaxX = _imaxY = 0;
}

CMiniPack::~CMiniPack()
{
	SAFE_DELETE_ARRAY(_pTData);
	SAFE_DELETE_ARRAY(_pIdx);
}

bool 	CMiniPack::Init(char* pszMapName)
{
	char pszPath[MAX_PATH];
	sprintf(pszPath,"texture\\minimap\\%s\\%s.pk",pszMapName,pszMapName);
	if(!LoadPackFile(pszPath))
		return false;

	std::vector<sXY>	vecIdx;
	vecIdx.resize(_RootDir.dwFileNum);
	DWORD n;
	_imaxX = 0;
	_imaxY = 0;
	for(n = 0; n < _RootDir.dwFileNum; n++)
	{
		GetXY((char*)_RootDir.vecFile[n].strFileName.c_str(), vecIdx[n].x,vecIdx[n].y);
		_imaxX = vecIdx[n].x > _imaxX ? vecIdx[n].x : _imaxX;
		_imaxY = vecIdx[n].y > _imaxY ? vecIdx[n].y : _imaxY;
	}

	_imaxX++;
	_imaxY++;
	_pIdx = new FileData*[_imaxX * _imaxY];
	//ZeroMemory(_pIdx,sizeof(FileData*)*(_imaxX* _imaxY));
	memset(_pIdx,0,sizeof(FileData*)*(_imaxX* _imaxY));

	for(n = 0; n < _RootDir.dwFileNum; n++)
	{
		_pIdx[vecIdx[n].x + vecIdx[n].y * _imaxX] = &_RootDir.vecFile[n];
	}
	return true;
}

void	CMiniPack::GetXY( char*pszName, int& x,int& y)
{
	static char pnum[257] = { 0 };
	byte idx = 0;
	char* p = pszName;
	p += 3;
	while (*p!='_')
	{
		pnum[idx] = *p;
		p++;
		idx++;
	}
	pnum[idx] = 0;
	x = atoi(pnum);
	idx = 0;
	p++;
	while (*p!='.')
	{
		pnum[idx] = *p;
		p++;
		idx++;
	}
	pnum[idx] = 0;
	y = atoi(pnum);
}
