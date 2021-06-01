#include "ZRBlock.h"

ZRBlock::ZRBlock()
{
	memset(m_BlockSectionArray, 0, MAX_BLOCK_SECTION*MAX_BLOCK_SECTION*4);
	m_pDefaultBlock = new ZRBlockData;
	m_fp = NULL;
	m_nSectionWidth = 8;
	m_nSectionHeight = 8;
	m_bEdit = true;
	m_pOffsetIdx = NULL;
	m_pMapData = NULL;
	m_dwMapPos = 0;
}

ZRBlock::~ZRBlock()
{
	delete m_pDefaultBlock;
	delete m_pOffsetIdx;
	delete m_pMapData;
}

BOOL ZRBlock::Load(const char *pszMapName, BOOL bEdit)
{
	if(m_fp!=NULL) 
	{
		fclose(m_fp); m_fp = NULL;
	}
	
	FILE *fp = NULL;
	if(bEdit)
	{
		_chmod(pszMapName, _S_IWRITE );

        fp = fopen(pszMapName, "r+b");
	}
	else
	{
		fp = fopen(pszMapName, "rb");
	}
	if(fp==NULL) 
	{
		LG("map", "msgLoad Map [%s] Error!\n", pszMapName);
		return FALSE;
	}
	
	MPMapFileHeader header;

	DWORD dwReadSize = 0;
	
	fread(&header, sizeof(MPMapFileHeader), 1, fp);
	dwReadSize += sizeof(MPMapFileHeader);

    if(header.nMapFlag==MP_MAP_FLAG+1)
    {
	    fclose(fp);
		LG("map", "msg该地图文件[%s]版本过期, 请使用MapTool打开它来升级版本!", pszMapName);
		return FALSE;
	}
    
#ifdef NEW_VERSION
    if(header.nMapFlag!=MP_MAP_FLAG + 3) 
#else
    if(header.nMapFlag!=MP_MAP_FLAG + 2)
#endif
	{
		fclose(fp);
		LG("map", "msg[%s]不是有效的 MindPower Map File!\n", pszMapName);
		return FALSE;
	}

	m_nWidth		 = header.nWidth;
	m_nHeight		 = header.nHeight;
	m_nSectionWidth  = header.nSectionWidth;
	m_nSectionHeight = header.nSectionHeight;

	m_nSectionCntX   = m_nWidth  / m_nSectionWidth;
	m_nSectionCntY   = m_nHeight / m_nSectionHeight;
	m_nSectionCnt    = m_nSectionCntX * m_nSectionCntY;

	m_bEdit = bEdit;

	m_fp = fp;

    // 读取全部索引
    m_pOffsetIdx = new DWORD[m_nSectionCnt];
    fread(m_pOffsetIdx, m_nSectionCnt * 4, 1, m_fp);
	dwReadSize += m_nSectionCnt * 4;

	if( !m_bEdit )
	{
		m_dwMapPos = ftell( m_fp );

		fseek( m_fp, 0, SEEK_END );
		DWORD dwPos = ftell( m_fp );

		DWORD dwMapDataSize = dwPos - dwReadSize;

		if( dwMapDataSize > m_dwMapDataSize )
		{
			SAFE_DELETE_ARRAY(m_pMapData);
			m_pMapData = new BYTE[dwMapDataSize];
			m_dwMapDataSize = dwMapDataSize;
		}
		fseek( m_fp, dwReadSize, SEEK_SET );
		fread( m_pMapData, dwMapDataSize, 1, m_fp );
	}
    ClearSectionArray();

	return TRUE;
}

void ZRBlock::GetBlockByRange(int CenterX, int CenterY, int range)
{
    if(m_fp==NULL) return;
	
	m_nGridShowWidth  = range * 2;
	m_nGridShowHeight = range * 2;

	m_fShowCenterX = CenterX;
	m_fShowCenterY = CenterY;

    MPTimer t;
    t.Begin();
    int nCurSectionX = (int)(m_fShowCenterX - (float)range / 2.0f)  / m_nSectionWidth;
	int nCurSectionY = (int)(m_fShowCenterY - (float)range / 2.0f)  / m_nSectionHeight;

    int nEndSectionX = (int)(m_fShowCenterX + (float)range / 2.0f)  / m_nSectionWidth;
	int nEndSectionY = (int)(m_fShowCenterY + (float)range / 2.0f)  / m_nSectionHeight;

	int nShowSectionCntX = nEndSectionX - nCurSectionX;
	int nShowSectionCntY = nEndSectionY - nCurSectionY;
	
	if(range  % m_nSectionWidth!=0)   nShowSectionCntX++;
	if(range  % m_nSectionHeight!=0)  nShowSectionCntY++;
	
    for(int y = 0; y < nShowSectionCntY; y++)
	{
		int nSectionY = nCurSectionY + y;

		if(nSectionY < 0 || nSectionY >= m_nSectionCntY) continue;
		for(int x = 0; x < nShowSectionCntX; x++)
		{
			int nSectionX = nCurSectionX + x;

			if(nSectionX < 0 || nSectionX >= m_nSectionCntX) continue;
		
			ZRBlockSection* pBlock = GetBlockSection(nSectionX, nSectionY);
			if(!pBlock)
			{
				pBlock = LoadBlockData(nSectionX, nSectionY);
           	}
		}
	}
}


ZRBlockSection* ZRBlock::GetBlockSection(int nSectionX, int nSectionY)
{
	return m_BlockSectionArray[nSectionX][nSectionY];
}

ZRBlockSection *ZRBlock::LoadBlockData(int nSectionX, int nSectionY)
{
    ZRBlockSection *pBlock = new ZRBlockSection;
	pBlock->nX = nSectionX;
	pBlock->nY = nSectionY;
	
    _LoadBlockData(pBlock);
    m_BlockSectionArray[nSectionX][nSectionY] = pBlock;

    return pBlock;
}


void ZRBlock::_LoadBlockData(ZRBlockSection *pBlock)
{
	int nSectionX = pBlock->nX;
	int nSectionY = pBlock->nY;

	pBlock->dwDataOffset = _ReadSectionDataOffset(nSectionX, nSectionY);
    
    if(pBlock->dwDataOffset==0) return;

	DWORD dwPos = 0;
	if( m_bEdit )
	{
		fseek(m_fp, pBlock->dwDataOffset, SEEK_SET);
	}
	else
	{
		dwPos = pBlock->dwDataOffset - m_dwMapPos;
	}
	
	pBlock->pBlockData = new ZRBlockData[m_nSectionWidth * m_nSectionHeight];

#ifdef NEW_VERSION
	SNewFileTile tile;
#else
	SFileTile tile;
#endif
	
	for(int y = 0; y < m_nSectionHeight; y++)
	{
		for(int x = 0; x < m_nSectionWidth; x++)
		{
			ZRBlockData *pB = pBlock->pBlockData + m_nSectionWidth * y + x;
			if( m_bEdit )
			{
				fread(&tile, sizeof(tile), 1, m_fp);
			}
			else
			{
				memcpy( &tile, m_pMapData + dwPos, sizeof(tile) );
				dwPos += sizeof(tile);
			}
			pB->sRegion  = tile.sRegion;
            memcpy(&pB->btBlock[0], &tile.btBlock[0], 4); 
        }
	}
}


ZRBlockData*	ZRBlock::GetBlock(int nX, int nY)
{
	if(nX >= m_nWidth || nY >= m_nHeight || nX < 0 || nY < 0) 
	{
		return m_pDefaultBlock;
	}

	int nSectionX = nX / m_nSectionWidth;
	int nSectionY = nY / m_nSectionHeight;

	ZRBlockSection *pBlock = GetBlockSection(nSectionX, nSectionY);
	 
	if(pBlock && pBlock->pBlockData)
	{
		int nOffX = nX % m_nSectionWidth;
		int nOffY = nY % m_nSectionHeight;
		return pBlock->pBlockData + nOffY * m_nSectionWidth + nOffX;
	}

	return m_pDefaultBlock;	
}

DWORD ZRBlock::_ReadSectionDataOffset(int nSectionX, int nSectionY)
{
	DWORD dwLoc = (nSectionY * m_nSectionCntX + nSectionX);
    return *(m_pOffsetIdx + dwLoc);
    
    fseek(m_fp, sizeof(MPMapFileHeader) + 4 * dwLoc, SEEK_SET);
	DWORD dwDataOffset; fread(&dwDataOffset, 4, 1, m_fp);
	return dwDataOffset;
}

void ZRBlock::SetGrid(int GridX, int GridY)
{
	m_nLastGridStartX = GridX;
	m_nLastGridStartY = GridY;
}

void ZRBlock::ClearSectionArray()
{
	memset(&m_BlockSectionArray, 0, MAX_BLOCK_SECTION * MAX_BLOCK_SECTION * 4);
}