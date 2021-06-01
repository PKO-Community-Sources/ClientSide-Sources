#include	"Stdafx.h"
#include	<sys/stat.h>
#include	<sys/timeb.h>
#include	<time.h>
#include	"SceneObjFile.h"
#include	"GameConfig.h"
#include "scene.h"
#include "GameApp.h"
#include "SceneObj.h"
#include "SceneObjSet.h"

CSceneObjFile	g_ObjFile;


CSceneObjFile::CSceneObjFile()
{
	m_bInitSuccess = false;
	m_fRdWr = NULL;
	m_fAppend = NULL;
	m_SSectionIndex = NULL;
}

CSceneObjFile::~CSceneObjFile()
{
	Free();
	//end_RBO();
}

// Added by clp
void CSceneObjFile::_init_RBO( const std::string &filename )
{
	filename_RBO = filename + "rbo";
	RBOinfoList.clear();
	//std::ifstream file ( filename_RBO.c_str() );
	//struct ReallyBigObjectInfo info;
	//while ( file >> info )
	//{
	//	RBOinfoList.insert( info );
	//}
	//file.close();
}

void CSceneObjFile::end_RBO()
{
	_Serialize_RBO();
	//_Serialize_RBO_ToMap();
}

void CSceneObjFile::_Serialize_RBO()
{
	if( !filename_RBO.empty() )
	{
		std::ofstream file ( filename_RBO.c_str() );
		std::set < struct ReallyBigObjectInfo >::iterator itr = RBOinfoList.begin();
		std::set < struct ReallyBigObjectInfo >::iterator end = RBOinfoList.end();
		while( itr != end )
		{
			file << *(itr++);
		}
		file.close();

		//FILE *file = fopen ( filename_RBO.c_str(), "wt" );
		//if( !file )
		//{
		//	return;
		//}

		//std::set < struct ReallyBigObjectInfo >::iterator itr = RBOinfoList.begin();
		//std::set < struct ReallyBigObjectInfo >::iterator end = RBOinfoList.end();
		//while( itr != end )
		//{
		//	file << *(itr++);
		//}
		//fclose ( file );
	}
}

void CSceneObjFile::_Serialize_RBO_ToMap()
{
	SSceneObjInfo	infoex[MAX_MAP_SECTION_OBJ];
	CGameScene* scene = CGameApp::GetCurScene();
	MPTerrain* terrain = scene->GetTerrain();

	std::set < struct ReallyBigObjectInfo >::iterator itr = RBOinfoList.begin();
	std::set < struct ReallyBigObjectInfo >::iterator end = RBOinfoList.end();

	while( itr != end )
	{
		int sectionX = itr->position.x / ( 100.f * terrain->GetSectionWidth() );
		int sectionY = itr->position.y / ( 100.f * terrain->GetSectionHeight() );

		int sectionNumber = sectionY * terrain->GetSectionCntX() + sectionX;

		long sectionObjCount = 0;

		if( ReadSectionObjInfo( sectionNumber, infoex, &sectionObjCount ) )
		{
			sectionObjCount++;
			int RBOIndex = sectionObjCount - 1;
			if( RBOIndex <= MAX_MAP_SECTION_OBJ )
			{
				infoex[RBOIndex].sTypeID    = itr->typeID;
				infoex[RBOIndex].nX         = itr->position.x;
				infoex[RBOIndex].nY         = itr->position.y;
				infoex[RBOIndex].sYawAngle  = itr->orientation.w;
				infoex[RBOIndex].sHeightOff = itr->position.z;
			}
			g_ObjFile.WriteSectionObjInfo(sectionNumber, infoex, sectionObjCount);
        }
		++itr;
	}
}

long CSceneObjFile::Init(_TCHAR *ptcsFileName, bool bSilence)
{
	long	lRet = 1;
	long	lFileSize;
	_TCHAR	tcsPrint[256];
	FILE	*fFile = NULL;

	Free();

	_tchmod(ptcsFileName, _S_IWRITE );

	fFile = fopen(ptcsFileName, "rb");

	// Added by clp
	std::string filename ( ptcsFileName );
	filename.erase ( filename.length() - 3 );
	_init_RBO( filename.c_str() );

	if (fFile == NULL)
	{
		if (CreateFile(ptcsFileName) == 0)
		{
			if (!bSilence)
			{
				_stprintf(tcsPrint, _TEXT("%s %s %s"), _TEXT(g_oLangRec.GetString(355)), ptcsFileName, _TEXT(g_oLangRec.GetString(356)));
				MessageBox(NULL, tcsPrint, _TEXT(g_oLangRec.GetString(25)), 0);
			}
			lRet = 0;
			goto end;
		}
		fFile = _tfopen(ptcsFileName, _TEXT("rb"));
	}

	fseek(fFile, 0, SEEK_END);
	lFileSize = ftell(fFile);
	fseek(fFile, 0, SEEK_SET);
	fread((void *)&m_SFileHead, sizeof(SFileHead), 1, fFile);
	if (m_SFileHead.lFileSize != lFileSize)
	{
		if (!bSilence)
		{
			_stprintf(tcsPrint, _TEXT("%s %s"), ptcsFileName, _TEXT(g_oLangRec.GetString(357)));
			MessageBox(NULL, tcsPrint, _TEXT(g_oLangRec.GetString(25)), 0);
		}
		lRet = 0;
		goto end;
	}
	if (m_SFileHead.lVersion == OBJ_FILE_VER500) //��Ҫ�汾ת��
	{
		fclose(fFile);
		if (ConvertObjFileVer(ptcsFileName) <= 0) //�汾ת��ʧ��
		{
			lRet = 0;
			goto end;
		}

		fFile = _tfopen(ptcsFileName, _TEXT("rb"));
		if (fFile == NULL)
		{
			if (!bSilence)
			{
				_stprintf(tcsPrint, _TEXT("%s %s"), ptcsFileName, _TEXT(g_oLangRec.GetString(358)));
				MessageBox(NULL, tcsPrint, _TEXT(g_oLangRec.GetString(25)), 0);
			}
			lRet = 0;
			goto end;
		}

		fseek(fFile, 0, SEEK_END);
		lFileSize = ftell(fFile);
		fseek(fFile, 0, SEEK_SET);
		fread((void *)&m_SFileHead, sizeof(SFileHead), 1, fFile);
	}
	if (m_SFileHead.lVersion != OBJ_FILE_VER600)
	{
		if (!bSilence)
		{
			_stprintf(tcsPrint, _TEXT("%s %s"), ptcsFileName, _TEXT(g_oLangRec.GetString(340)));
			MessageBox(NULL, tcsPrint, _TEXT(g_oLangRec.GetString(25)), 0);
		}
		lRet = 0;
		goto end;
	}

	m_SSectionIndex = new (SSectionIndex[m_SFileHead.iSectionCntX * m_SFileHead.iSectionCntY]);
	if (m_SSectionIndex == NULL)
	{
		lRet = 0;
		goto end;
	}
	fread((void *)m_SSectionIndex, sizeof(SSectionIndex), m_SFileHead.iSectionCntX * m_SFileHead.iSectionCntY, fFile);

	if( g_Config.m_bEditor )
		m_fRdWr = _tfopen(ptcsFileName, _TEXT("r+b"));
	else
		m_fRdWr = _tfopen(ptcsFileName, _TEXT("rb"));

	if (m_fRdWr == NULL)
	{
		if (!bSilence)
		{
			_stprintf(tcsPrint, _TEXT("%s %s"), ptcsFileName, _TEXT(g_oLangRec.GetString(339)));
			MessageBox(NULL, tcsPrint, _TEXT(g_oLangRec.GetString(25)), 0);
		}
		lRet = 0;
		goto end;
	}

	if( g_Config.m_bEditor )
		m_fAppend = _tfopen(ptcsFileName, _TEXT("a+b"));
	else
		m_fAppend = _tfopen(ptcsFileName, _TEXT("rb"));

	if (m_fAppend == NULL)
	{
		if (!bSilence)
		{
			_stprintf(tcsPrint, _TEXT("%s %s"), ptcsFileName, _TEXT(g_oLangRec.GetString(359)));
			MessageBox(NULL, tcsPrint, _TEXT(g_oLangRec.GetString(25)), 0);
		}
		lRet = 0;
		goto end;
	}

	m_bInitSuccess = true;

end:
	if (lRet == 0)
	{
		if (m_fRdWr)
		{
			fclose(m_fRdWr);
			m_fRdWr = NULL;
		}
		if (m_fAppend)
		{
			fclose(m_fAppend);
			m_fAppend = NULL;
		}
		if (m_SSectionIndex)
		{
			delete [] m_SSectionIndex;
			m_SSectionIndex = NULL;
		}
	}
	if (fFile)
		fclose(fFile);

	return lRet;
}
void CSceneObjFile::Free(void)
{
	m_bInitSuccess = false;
	if (m_fRdWr)
	{
		fclose(m_fRdWr);
		m_fRdWr = NULL;
	}
	if (m_fAppend)
	{
		fclose(m_fAppend);
		m_fAppend = NULL;
	}
	if (m_SSectionIndex)
	{
		delete [] m_SSectionIndex;
		m_SSectionIndex = NULL;
	}
}

long CSceneObjFile::CreateFile(_TCHAR *ptcsFileName,
								 int iSectionCntX, int iSectionCntY, int iSectionWidth,
								 int iSectionHeight, int iMaxSectionObjNum)
{
	FILE				*fFile = NULL;
	SFileHead			SHead;
	SSectionIndex		*lFileSectionIndex = NULL;

	if( g_Config.m_bEditor )
		fFile = _tfopen(ptcsFileName, _TEXT("wb"));
	else
		fFile = _tfopen(ptcsFileName, _TEXT("rb"));
	if (fFile == NULL)
		return 0;

	_tcscpy(SHead.tcsTitle, _TEXT("HF Object File!"));
	SHead.lVersion = OBJ_FILE_VER600;
	SHead.lFileSize = sizeof (SFileHead) + sizeof (SSectionIndex) * iSectionCntX * iSectionCntY;
	SHead.iSectionCntX = iSectionCntX;
	SHead.iSectionCntY = iSectionCntY;
	SHead.iSectionHeight = iSectionHeight;
	SHead.iSectionWidth = iSectionWidth;
	SHead.iSectionObjNum = iMaxSectionObjNum;

	lFileSectionIndex = new (SSectionIndex[iSectionCntX * iSectionCntY]);
	if (lFileSectionIndex == NULL)
	{
		fclose(fFile);
		return 0;
	}
	memset(lFileSectionIndex, 0, sizeof(SSectionIndex) * iSectionCntX * iSectionCntY);

	fwrite((const void *)&SHead, sizeof(SFileHead), 1, fFile);
	fwrite((const void *)lFileSectionIndex, sizeof(SSectionIndex), iSectionCntX * iSectionCntY, fFile);

	if (fFile)
		fclose(fFile);
	if (lFileSectionIndex)
		delete [] lFileSectionIndex;
	return 1;
}

long CSceneObjFile::ConvertObjFileVer(_TCHAR *ptcsFileName, bool bBackUp) // �Ӱ汾500ת����600(����ľ�������ת�������section���������)
{
	long			lRet = 2;
	_TCHAR			tcsBackUpName[_MAX_FNAME] = _TEXT("");
	_TCHAR			tcsPrint[256];
	long			i, j;
	long			lMaxSectionNum;
	FILE			*fFileOld, *fFileNew;
	SSceneObjInfo	*pSObjInfo = NULL;
	SFileHead		SHead;
	SSectionIndex	*pSSectionIndex = NULL;
	unsigned long ulFileSize;

	_tcscpy(tcsBackUpName, ptcsFileName);
	for (i = 0; i < _MAX_FNAME - (long)_tcslen(ptcsFileName); i += 4)
	{
		_tcscat(tcsBackUpName, _TEXT(".bak"));
		fFileNew = _tfopen(tcsBackUpName, _TEXT("rb"));
		if (fFileNew == NULL)
			break;
		else
		{
			fclose(fFileNew);
			fFileNew = NULL;
		}
	}
	if (i >= _MAX_FNAME - (long)_tcslen(ptcsFileName))
	{
		lRet = -1; // ������ʱ�ļ�ʧ��
		goto end;
	}
	if (_trename(ptcsFileName, tcsBackUpName) != 0) // 
	{
		lRet = -2; // �ļ�������ʧ��
		goto end;
	}

	fFileOld = _tfopen(tcsBackUpName, _TEXT("rb"));
	if (fFileOld == NULL)
	{
		lRet = -3; // ��Դ�ļ�ʧ��
		goto end;
	}

	if( g_Config.m_bEditor )
		fFileNew = _tfopen(ptcsFileName, _TEXT("wb"));
	else
		fFileNew = _tfopen(ptcsFileName, _TEXT("rb"));
	if (fFileNew == NULL)
	{
		lRet = -1; // �������ļ�ʧ��
		goto end;
	}

	fseek(fFileOld, 0, SEEK_END);
	ulFileSize = ftell(fFileOld);
	fseek(fFileOld, 0, SEEK_SET);
	fread(&SHead, sizeof(SFileHead), 1, fFileOld);
	if (SHead.lVersion != OBJ_FILE_VER500) // ����Ҫת��
	{
		lRet = 1;
		goto end;
	}

	_stprintf(tcsPrint, _TEXT(g_oLangRec.GetString(360)), ptcsFileName);
	MessageBox(NULL, tcsPrint, _TEXT(g_oLangRec.GetString(361)), 0);

	lMaxSectionNum = SHead.iSectionCntX * SHead.iSectionCntY;
	pSSectionIndex = new (SSectionIndex[lMaxSectionNum]);
	if (pSSectionIndex == NULL)
	{
		lRet = -4; // �ڴ�������
		goto end;
	}

	pSObjInfo = new (SSceneObjInfo[SHead.iSectionObjNum]);
	if (pSObjInfo == NULL)
	{
		lRet = -4;
		goto end;
	}

	fread((void *)pSSectionIndex, sizeof(SSectionIndex), lMaxSectionNum, fFileOld);
	fseek(fFileNew, sizeof(SFileHead) + sizeof(SSectionIndex) * lMaxSectionNum, SEEK_SET);

	int nSectionX, nSectionY;
	for (i = 0; i < lMaxSectionNum && ulFileSize >= (unsigned long)ftell(fFileOld); i ++)
	{
	    if (pSSectionIndex[i].iObjNum > 0) //������Ч����
		{
			fseek(fFileOld, pSSectionIndex[i].lObjInfoPos, SEEK_SET);
			fread(pSObjInfo, sizeof(SSceneObjInfo) * SHead.iSectionObjNum, 1, fFileOld);
			for (j = 0; j < pSSectionIndex[i].iObjNum; j++)
			{
				nSectionX = i % SHead.iSectionCntX * SHead.iSectionWidth * 100;
				nSectionY = i / SHead.iSectionCntX * SHead.iSectionHeight * 100;
				pSObjInfo[j].nX -= nSectionX;
				pSObjInfo[j].nY -= nSectionY;
                if(pSObjInfo[j].nY < 0)
                {
                    int nnn = 0;
                }
			}
			pSSectionIndex[i].lObjInfoPos = ftell(fFileNew);
			fwrite(pSObjInfo, sizeof(SSceneObjInfo) * SHead.iSectionObjNum, 1, fFileNew);
		}
		else
		{
			pSSectionIndex[i].lObjInfoPos = 0;
		}
	}

	SHead.lVersion = OBJ_FILE_VER600;
	SHead.lFileSize = ftell(fFileNew);
	fseek(fFileNew, 0, SEEK_SET);
	fwrite((const void *)&SHead, sizeof(SFileHead), 1, fFileNew);
	fwrite((const void *)pSSectionIndex, sizeof(SSectionIndex), lMaxSectionNum, fFileNew);

	if (!bBackUp)
	{
		fclose(fFileOld);
		fFileOld = NULL;
		_tremove(tcsBackUpName);
		_tcscpy(tcsBackUpName, ptcsFileName);
	}

end:
	if (lRet == 2)
	{
		_stprintf(tcsPrint, _TEXT(g_oLangRec.GetString(362)), ptcsFileName, tcsBackUpName);
		MessageBox(NULL, tcsPrint, _TEXT(g_oLangRec.GetString(363)), 0);
	}
	if (fFileOld)
		fclose(fFileOld);
	if (fFileNew)
		fclose(fFileNew);
	if (pSSectionIndex)
		delete [] pSSectionIndex;
	if (pSObjInfo)
		delete [] pSObjInfo;
	return lRet;
}

long CSceneObjFile::ReadSectionObjInfo(int nSectionNO, SSceneObjInfo *SSceneObj, long *lSectionObjNum)
{
	if (!m_bInitSuccess)
		return 0;

	if (nSectionNO >= m_SFileHead.iSectionCntX * m_SFileHead.iSectionCntY)
		return 0;

	if ((*lSectionObjNum = m_SSectionIndex[nSectionNO].iObjNum) > 0)
	{
		fseek(m_fRdWr, m_SSectionIndex[nSectionNO].lObjInfoPos, SEEK_SET);
        LG("readmap", "Seek Offset [%d %d] = %d\n", nSectionNO % m_SFileHead.iSectionCntX, nSectionNO / m_SFileHead.iSectionCntX, m_SSectionIndex[nSectionNO].lObjInfoPos);
        fread(SSceneObj, sizeof(SSceneObjInfo), m_SSectionIndex[nSectionNO].iObjNum, m_fRdWr);
		// ת���ɾ�������
		int nSectionX, nSectionY;
		for (int i = 0; i < m_SSectionIndex[nSectionNO].iObjNum; i++)
		{
			nSectionX = nSectionNO % m_SFileHead.iSectionCntX * m_SFileHead.iSectionWidth * 100;
			nSectionY = nSectionNO / m_SFileHead.iSectionCntX * m_SFileHead.iSectionHeight * 100;
            if(SSceneObj[i].nX < 0)
            {
                int jjj = 0;
            }
            SSceneObj[i].nX += nSectionX;
			SSceneObj[i].nY += nSectionY;
			SSceneObjInfo *pObj = (SSceneObj + i);
			if(pObj->GetID()==0)
			{
				LG("error", g_oLangRec.GetString(364));
			}
			
		}
		//
	}

	return 1;
}

long CSceneObjFile::WriteSectionObjInfo(int nSectionNO, SSceneObjInfo *SSceneObj, long lSectionObjNum)
{
	FILE	*fFile;

	if (!m_bInitSuccess)
		return 0;

	if (nSectionNO >= m_SFileHead.iSectionCntX * m_SFileHead.iSectionCntY)
		return 0;

	m_SSectionIndex[nSectionNO].iObjNum = lSectionObjNum;
	if (lSectionObjNum > 0)
	{
		if (m_SSectionIndex[nSectionNO].lObjInfoPos > 0)
		{
			fFile = m_fRdWr;
			fseek(fFile, m_SSectionIndex[nSectionNO].lObjInfoPos, SEEK_SET);
		}
		else
		{
			m_SSectionIndex[nSectionNO].lObjInfoPos = m_SFileHead.lFileSize;
			m_SFileHead.lFileSize += sizeof(SSceneObjInfo) * m_SFileHead.iSectionObjNum;
			fseek(m_fRdWr, 0, SEEK_SET);
			fwrite((const void *)&m_SFileHead, sizeof(SFileHead), 1, m_fRdWr);
			fflush(m_fRdWr);

			fFile = m_fAppend;
		}
		// ת�����������
		int nSectionX, nSectionY;
		for (int j = 0; j < m_SSectionIndex[nSectionNO].iObjNum; j++)
		{
			nSectionX = nSectionNO % m_SFileHead.iSectionCntX * m_SFileHead.iSectionWidth * 100;
			nSectionY = nSectionNO / m_SFileHead.iSectionCntX * m_SFileHead.iSectionHeight * 100;
			SSceneObj[j].nX -= nSectionX;
			SSceneObj[j].nY -= nSectionY;
		}
		//
		// д��obj��Ϣ
		fwrite((const void *)SSceneObj, sizeof(SSceneObjInfo), m_SFileHead.iSectionObjNum, fFile);
		fflush(fFile);
	}
	else
	{
		m_SSectionIndex[nSectionNO].lObjInfoPos = 0;
	}

	fseek(m_fRdWr, sizeof(SFileHead) + sizeof(SSectionIndex) * nSectionNO, SEEK_SET);
	fwrite((const void *)(m_SSectionIndex + nSectionNO), sizeof(SSectionIndex), 1, m_fRdWr);
	fflush(m_fRdWr);

	// ��ʱ���롣����
	// 
	long	lFileSize;
	fseek(m_fRdWr, 0, SEEK_END);
	lFileSize = ftell(m_fRdWr);
	if (lFileSize != m_SFileHead.lFileSize)
	{
		MessageBox(NULL, _TEXT(g_oLangRec.GetString(365)), _TEXT(g_oLangRec.GetString(25)), 0);
	}
	//

	return 1;
}

long CSceneObjFile::TrimFile(_TCHAR *ptcsFileName, bool bBackUp)
{
	long		lRet = 1;
	_TCHAR		tcsBackUpName[_MAX_FNAME] = _TEXT("");
	long		i;
	long		lPos = 0;
	SFileHead	SHead;
	long		lMaxSectionNum;
	FILE		*fFileOld = NULL, *fFileNew = NULL; 
	char		*pszSectionInfo = NULL;
	SSectionIndex		*pSSectionIndex = NULL;
	unsigned long ulFileSize;

	_tcscpy(tcsBackUpName, ptcsFileName);
	for (i = 0; i < _MAX_FNAME - (long)_tcslen(ptcsFileName); i += 4)
	{
		_tcscat(tcsBackUpName, _TEXT(".bak"));
		fFileNew = _tfopen(tcsBackUpName, _TEXT("rb"));
		if (fFileNew == NULL)
			break;
		else
		{
			fclose(fFileNew);
			fFileNew = NULL;
		}
	}
	if (i >= _MAX_FNAME - (long)_tcslen(ptcsFileName))
	{
		lRet = -1; // ������ʱ�ļ�ʧ��
		goto end;
	}
	if (_trename(ptcsFileName, tcsBackUpName) != 0) // 
	{
		lRet = -2; // �ļ�������ʧ��
		goto end;
	}

	fFileOld = _tfopen(tcsBackUpName, _TEXT("rb"));
	if (fFileOld == NULL)
	{
		lRet = -3; // ���ļ�ʧ��
		goto end;
	}

	if( g_Config.m_bEditor )
		fFileNew = _tfopen(ptcsFileName, _TEXT("wb"));
	else
		fFileNew = _tfopen(ptcsFileName, _TEXT("rb"));
	if (fFileNew == NULL)
	{
		lRet = -3; // ���ļ�ʧ��
		goto end;
	}

	fseek(fFileOld, 0, SEEK_END);
	ulFileSize = ftell(fFileOld);
	fseek(fFileOld, 0, SEEK_SET);
	fread((void *)&SHead, sizeof(SFileHead), 1, fFileOld);
	if (_tcscmp(SHead.tcsTitle, _TEXT("HF Object File!")) != 0
		|| SHead.lVersion != OBJ_FILE_VER600)// || SHead.lFileSize != i)
	{
		lRet = -4; // Դ�ļ��ѱ��ƻ���汾����
		goto end;
	}

    lMaxSectionNum = SHead.iSectionCntX * SHead.iSectionCntY;
	pSSectionIndex = new (SSectionIndex[lMaxSectionNum]);
	if (pSSectionIndex == NULL)
	{
		lRet = -5; // �ڴ�������
		goto end;
	}
	fread((void *)pSSectionIndex, sizeof(SSectionIndex), lMaxSectionNum, fFileOld);
	pszSectionInfo = new (char[sizeof(SSceneObjInfo) * lMaxSectionNum]);
	if (pszSectionInfo == NULL)
	{
		lRet = -5;
		goto end;
	}

	fseek(fFileNew, sizeof(SFileHead) + sizeof(SSectionIndex) * lMaxSectionNum, SEEK_SET);

	for (i = 0; i < lMaxSectionNum && ulFileSize >= (unsigned long)ftell(fFileOld); i ++)
	{
	    if (pSSectionIndex[i].iObjNum > 0) //������Ч����
		{
			fseek(fFileOld, pSSectionIndex[i].lObjInfoPos, SEEK_SET);
			fread(pszSectionInfo, sizeof(SSceneObjInfo) * SHead.iSectionObjNum, 1, fFileOld);
			pSSectionIndex[i].lObjInfoPos = ftell(fFileNew);
			fwrite(pszSectionInfo, sizeof(SSceneObjInfo) * SHead.iSectionObjNum, 1, fFileNew);
		}
		else
		{
			pSSectionIndex[i].lObjInfoPos = 0;
		}
	}

	SHead.lFileSize = ftell(fFileNew);
	fseek(fFileNew, 0, SEEK_SET);
	fwrite(&SHead, sizeof(SFileHead), 1, fFileNew);
	fwrite(pSSectionIndex, sizeof(SSectionIndex), lMaxSectionNum, fFileNew);

	if (!bBackUp)
	{
		fclose(fFileOld);
		fFileOld = NULL;
		_tremove(tcsBackUpName);
	}

end:
	if (fFileOld)
		fclose(fFileOld);
	if (fFileNew)
		fclose(fFileNew);
	if (pSSectionIndex)
		delete [] pSSectionIndex;
	if (pszSectionInfo)
		delete [] pszSectionInfo;
	return lRet;
}

long CSceneObjFile::TrimDirectory(_TCHAR *ptcsDirectory, bool bBackUp)
{
	_TCHAR		tcsFileName[_MAX_FNAME], tcsPath[_MAX_PATH];
	size_t		lLen;
	_finddata_t	c_file;
	long		hFile;
	_TCHAR		*ptcsRecordFile = _TEXT("TrimRecord.txt");
	FILE		*fRecord = NULL;
	_timeb		tTimeBuffer;
	_TCHAR			tcsPrint[256];

	if (bBackUp)
		_stprintf(tcsPrint, _TEXT(g_oLangRec.GetString(366)), ptcsDirectory);
	else
		_stprintf(tcsPrint, _TEXT(g_oLangRec.GetString(367)), ptcsDirectory);
	if (IDYES != MessageBox(NULL, tcsPrint, _TEXT(g_oLangRec.GetString(361)), MB_YESNO))
		return 1;

	lLen = _tcslen(ptcsDirectory);
	_tcscpy(tcsPath, ptcsDirectory);
	if (tcsPath[lLen - 1] != _TEXT('\\') || tcsPath[lLen - 1] != _TEXT('/'))
	{
		tcsPath[lLen] = _TEXT('/');
		tcsPath[lLen + 1] = 0;
	}

	_stprintf(tcsFileName, _TEXT("%s%s"), tcsPath, ptcsRecordFile);
	if( g_Config.m_bEditor )
		fRecord = _tfopen(tcsFileName, _TEXT("w"));
	else
		fRecord = _tfopen(tcsFileName, _TEXT("rb"));
	if (fRecord == NULL)
		return 0;
	_ftime(&tTimeBuffer);
	_ftprintf(fRecord, _TEXT(g_oLangRec.GetString(368)), _tctime(&tTimeBuffer.time));

	_stprintf(tcsFileName, _TEXT("%s%s"), tcsPath, _TEXT("*.obj"));
	if( (hFile = (long)_tfindfirst(tcsFileName, &c_file)) == -1L )
	{
		fclose(fRecord);
		return 1;
	}
	else
	{
		if (!(c_file.attrib & _A_SUBDIR))
		{
			_stprintf(tcsFileName, _TEXT("%s%s"), tcsPath, _TEXT(c_file.name));
			if (c_file.attrib & _A_RDONLY)
				_tchmod(tcsFileName, _S_IREAD | _S_IWRITE);
			_ftprintf(fRecord, _TEXT("%s:\n"), tcsFileName);
			switch (TrimFile(tcsFileName, bBackUp))
			{
			case	1:
				_ftprintf(fRecord, _TEXT(g_oLangRec.GetString(369)));
				break;
			case	-1:
				_ftprintf(fRecord, _TEXT(g_oLangRec.GetString(370)));
				break;
			case	-2:
				_ftprintf(fRecord, _TEXT(g_oLangRec.GetString(371)));
				break;
			case	-3:
				_ftprintf(fRecord, _TEXT(g_oLangRec.GetString(372)));
				break;
			case	-4:
				_ftprintf(fRecord, _TEXT(g_oLangRec.GetString(373)));
				break;
			case	-5:
				_ftprintf(fRecord, _TEXT(g_oLangRec.GetString(374)));
				break;
			default:
				_ftprintf(fRecord, _TEXT(g_oLangRec.GetString(375)));
			}
		}
		while( _findnext( hFile, &c_file ) == 0 )
		{
			if (!(c_file.attrib & _A_SUBDIR))
			{
				_stprintf(tcsFileName, _TEXT("%s%s"), tcsPath, _TEXT(c_file.name));
				if (c_file.attrib & _A_RDONLY)
					_tchmod(tcsFileName, _S_IREAD | _S_IWRITE);
				_ftprintf(fRecord, _TEXT("%s:\n"), tcsFileName);
				switch (TrimFile(tcsFileName, bBackUp))
				{
				case	1:
					_ftprintf(fRecord, _TEXT(g_oLangRec.GetString(369)));
					break;
				case	-1:
					_ftprintf(fRecord, _TEXT(g_oLangRec.GetString(370)));
					break;
				case	-2:
					_ftprintf(fRecord, _TEXT(g_oLangRec.GetString(371)));
					break;
				case	-3:
					_ftprintf(fRecord, _TEXT(g_oLangRec.GetString(372)));
					break;
				case	-4:
					_ftprintf(fRecord, _TEXT(g_oLangRec.GetString(373)));
					break;
				case	-5:
					_ftprintf(fRecord, _TEXT(g_oLangRec.GetString(374)));
					break;
				default:
					_ftprintf(fRecord, _TEXT(g_oLangRec.GetString(375)));
				}
			}
		}

		_findclose( hFile );
	}

	if (fRecord)
		fclose(fRecord);
	return 1;
}
