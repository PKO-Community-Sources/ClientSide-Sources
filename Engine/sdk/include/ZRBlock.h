#ifndef _ZRBLOCK_H_
#define _ZRBLOCK_H_

#include "MPMap.h"
#include "assert.h" 
#include <sys/types.h>
#include <sys/stat.h>
#include "MPMapDef.h"

#define MAX_BLOCK_SECTION 512
#define MAX_BLOCK_RANGE 1024

//add by jze 2008.7.16
class ZRBlockData
{
public:
    short           sRegion;     // 区域属性
    BYTE            btBlock[4];  // 4个分格的障碍记录
public:
    ZRBlockData()
	{
		sRegion = 0;
		for(int i = 0; i < 4; i++)
		{
		    btBlock[i] = 0;
        }
    }

	~ZRBlockData(){}

    BYTE	IsBlock(BYTE no)
    {
        if(btBlock[no] & 128) return 1;
        return 0;
    }

    void	setBlock(BYTE no, BOOL bSet)
    {
        if(bSet)
        {
            btBlock[no]|=128;
        }
        else
        {
            btBlock[no]&=127;
        }
    }

    BOOL    IsRegion(int nRegionNo)
    {
        short s = 1;
        s<<=(nRegionNo - 1);
        return sRegion & s; 
    }

    short	GetRegionValue() { return sRegion; }
};

//add by jze 2008.7.16
class ZRBlockSection
{
public:
	ZRBlockData* pBlockData;
	int			 nX;									// MapSection所在的位置
	int			 nY;
	DWORD		 dwDataOffset;						// 文件数据指针位置 = 0, 表示没有数据
public:
	ZRBlockSection()
	{
		pBlockData = NULL;
		nX = 0;
		nY = 0;
		dwDataOffset = 0;
	}

	~ZRBlockSection(){}
};

class ZRBlock
{
public:
	ZRBlock();
	~ZRBlock();
	BOOL                Load(const char *pszMapName, BOOL bEdit);
	void				GetBlockByRange(int CenterX, int CenterY, int range); //动态读入一定范围的Block信息
	ZRBlockData*		GetBlock(int nX, int nY); //获取Block数据
    BYTE				IsGridBlock(int x, int y);      // x,y为小格子坐标
	short				GetTileRegionAttr(int x, int y);// x,y为大格子米坐标
	void                SetGrid(int GridX, int GridY);
private:
	ZRBlockSection*		GetBlockSection(int nSectionX, int nSectionY); //货物存放block的动态数组
	ZRBlockSection*		LoadBlockData(int nSectionX, int nSectionY); //读取block数据
	void				ClearSectionArray();

	void				_LoadBlockData(ZRBlockSection *pSection); //从文件中读block数据
	DWORD				_ReadSectionDataOffset(int nSectionX, int nSectionY); //读数据

public:
    BYTE                m_btBlockBuffer[MAX_BLOCK_RANGE][MAX_BLOCK_RANGE];
	short				m_sTileRegionAttr[MAX_BLOCK_SECTION][MAX_BLOCK_SECTION];
private:
	ZRBlockSection*		m_BlockSectionArray[MAX_BLOCK_SECTION][MAX_BLOCK_SECTION]; //存放block的动态数组

	ZRBlockData*        m_pDefaultBlock;     //默认的block数据
	FILE*				m_fp;				// 地图文件句柄

	int					m_fShowCenterX; // 动态读入的中心位置
	int					m_fShowCenterY;
	int					m_nSectionWidth;// 每一块Section的宽度和高度
	int					m_nSectionHeight;
	int					m_nSectionCntX;// 地图水平方向上Section的块数
	int					m_nSectionCntY;
	int                 m_nSectionCnt;
	int                 m_nLastGridStartX;
	int                 m_nLastGridStartY;
	int					m_nGridShowWidth;
	int					m_nGridShowHeight;
	BOOL				m_bEdit;
	DWORD				m_dwMapPos;
	DWORD				m_dwMapDataSize;
	LPBYTE				m_pMapData;
	int					m_nWidth;
	int					m_nHeight;
	DWORD*              m_pOffsetIdx;
};

inline BYTE ZRBlock::IsGridBlock(int x, int y) // 小格子坐标
{
    int offx = x - m_nLastGridStartX;
    int offy = y - m_nLastGridStartY;
	
	if(offx < 0 || offx >= m_nGridShowWidth)  return 1;
    if(offy < 0 || offy >= m_nGridShowHeight) return 1;

    return m_btBlockBuffer[offy][offx];
}

inline short ZRBlock::GetTileRegionAttr(int x, int y) // 大格子坐标
{
    int offx = x - m_nLastGridStartX/2;
    int offy = y - m_nLastGridStartY/2;

    if(offx < 0 || offx >= m_nGridShowWidth) return 0;
    if(offy < 0 || offy >= m_nGridShowHeight) return 0;

    return m_sTileRegionAttr[offy][offx];
}
#endif