#ifndef _MPMAP_H_
#define _MPMAP_H_
#include "ZRBlock.h"
#include "MPTile.h"





#define UNDERWATER_HEIGHT		 -2.0f
#define UNDERWATER_TEXNO			22
#define MAX_OTHER_DATA              40
#define SKYDOOM_TEXTURE_MOVESPEED   10  
#define PI                          3.1415926535897932384626433832795f 

struct MPActiveMapSection
{
	MPTile*		pTileData;							// ָ��ʵ�ʵ�Tile����
	int			nX;									// MapSection���ڵ�λ��
	int			nY;
	DWORD		dwActiveTime;						// ���һ��ʹ�õ�ʱ��
	DWORD		dwDataOffset;						// �ļ�����ָ��λ�� = 0, ��ʾû������
	
	void	Init()
	{
		pTileData = NULL;
		nX = 0;
		nY = 0;
		dwActiveTime = 0;
		dwDataOffset = 0;
	}
};
class  MPMap;
typedef long (CALLBACK* MAP_PROC)(int nFlag, int nSectionX, 
								  int nSectionY, unsigned long dwParam,MPMap* pthis);

class ZRBlock;

class MINDPOWER_API MPMap
{
public:
	
	MPMap();
	~MPMap();
	
	BOOL		Load(const char *pszMapName, BOOL bEdit = FALSE);
	// void		Create(int nWidth, int nHeight, const char *pszMapName, int nSectionWidth = 16, int nSectionHeight = 16);
	
	void		SetSectionTileData(int nX, int nY, BYTE btTexNo);
	void		SetMapProcFN(MAP_PROC pfn)				{	_pfnProc = pfn;						  }
	
	MPTile*		GetTile(int nX, int nY);
	MPTile*		GetGroupTile(int nX, int nY, int nGroupNo);
	void		DynamicLoading(DWORD dwTimeParam);
    void        FullLoading();
    void        ClearSectionArray();
    std::list<MPActiveMapSection*> *GetActiveSectionList()    {   return &_ActiveSectionList; }
    
    MPActiveMapSection*	GetActiveSection(int nSectionX, int nSectionY);
    void                SetSectionBufferSize(int nSize)               {   _nSectionBufferSize = nSize;}
    void                ClearSectionData(int nSectionX, int nSectionY);
    MPActiveMapSection* LoadSectionData(int nSectionX, int nSectionY);

	void		FrameMove(DWORD dwTimeParam);
	void		Render();
	void		RenderSea();

	void		RenderSmMap();
	
	BOOL		GetPickPos(int nX, int nY, D3DXVECTOR3 &vPickPos);
    BOOL        GetPickPosEditor(int nX, int nY, D3DXVECTOR3 &vPickPos);
	
	int			GetWidth()								{ return _nWidth;						  }
	int			GetHeight()								{ return _nHeight;						  }
	int			GetSectionCntX()						{ return _nSectionCntX;					  }
	int			GetSectionCntY()						{ return _nSectionCntY;					  }
    int         GetSectionCnt()                         { return _nSectionCnt;                    }

	int			GetSectionWidth()
	{
		return _nSectionWidth;
	}
	int			GetSectionHeight()
	{
		return _nSectionHeight;
	}

    int         GetValidSectionCnt();
    
    void		SetShowSize(int nWidth, int nHeight)	
    { 
        if(nWidth!=_nShowWidth || nHeight!=_nShowHeight)
        {
            _nShowWidth  = nWidth;
            _nShowHeight = nHeight;
			_nGridShowWidth  = nWidth * 2;
			_nGridShowHeight = nHeight * 2;
        }
    }
	int			GetShowWidth()							{ return _nShowWidth;					  }
	int			GetShowHeight()							{ return _nShowHeight;					  }
	void		SetShowCenter(float fX, float fY)		
    { 
        _fShowCenterX = fX; 
        _fShowCenterY = fY;
    }
	float		GetShowCenterX()						{ return _fShowCenterX;					  }
	float		GetShowCenterY()						{ return _fShowCenterY;					  }
	BOOL		IsPointVisible(float fX, float fY);

	void		SetSeaVisible(BOOL bSeaVisible)			{ _bSeaVisible = bSeaVisible;			  }
	BOOL		IsSeaVisible()							{ return _bSeaVisible;					  }
	float		GetHeight(float fX, float fY);
	void		ShowCenterPoint(BOOL bShow)             { _bShowCenterPoint = bShow;	          }
	void        ClearAllSection(BOOL bSave); 
	
	void		EnableVB(BOOL bUseVB)					{ _bUseVB     = bUseVB;					  }
	void		EnableClip(BOOL bClip)					{ _bClip      = bClip;					  }
	void		EnableNormalLight(BOOL bEnable)			{ _bEnableNormalLight = bEnable;		  }	
	BOOL		IsEnableVB()							{ return _bUseVB;						  }
	BOOL		IsEnableClip()							{ return _bClip;						  }
	BOOL		IsEnableNormalLight()					{ return _bEnableNormalLight;			  }
	void		EnableWireFrame(BOOL bEnable)			{ _bWireFrame = bEnable;				  }
    BOOL        CanEdit()                               { return _bEdit;                          }
    void        EnableBatchRender(BOOL bEnable)         { _bBatchRender = bEnable;                }
    BOOL        IsBatchRender()                         { return _bBatchRender;                   }
    void        SetSeaDefaultColor(DWORD dwColor)       { _dwSeaDefaultColor = dwColor;           }
    void        UpdateRender(BOOL bReset = FALSE)       
    { 
        if(bReset) _nUpdateRender = 2; // ��Ҫ��������Group (�������, ��Ұ�仯)
        else       _nUpdateRender = 1; // ֻ��Ҫ���²���VB(����༭��, �ƹ���ɫ����)
    }

    float       GetGridHeight(int x, int y);    // x,yΪС��������
    BYTE        IsGridBlock(int x, int y);      // x,yΪС��������
	float		GetTileHeight(int x, int y);    // x,yΪ�����������
	short       GetTileRegionAttr(int x, int y);// x,yΪ�����������

public:

    DWORD       m_dwTerrainRenderTime;
    DWORD       m_dwSeaRenderTime;
    DWORD       m_dwLoadingTime[3]; // ���3�β�Ϊ0�Ķ���ʱ��
    DWORD       m_dwMaxLoadingTime;
    DWORD       m_dwActiveSectionCnt;
    DWORD       m_dwBatchTime;

public:
	bool		m_bCullTile;
protected:

	// MPMapSection*	_GetSection(int nSectionX, int nSectionY);
	void				_LoadSectionData(MPActiveMapSection *pSection);
	void				_SaveSection(MPActiveMapSection *pSection);
	void				_RenderSea(int nStartX, int nStartY);
	DWORD				_ReadSectionDataOffset(int nSectionX, int nSectionY);
	void				_WriteSectionDataOffset(int nSectionX, int nSectionY, DWORD dwDataOffset);
	void				_CalTileNormal(int nX, int nY);
	void				_RenderFocusRect();
    void                _GenerateTerrainGroup(int nTileStartX, int nTileStartY);
    BOOL                _AddRenderGroup(BYTE btLayer, int nTexNo, MPTile *pTile, short sTileX, short sTileY);
    void                _FillVB();
    void                _RenderVB(BOOL bWireframe = FALSE);
	

	// MPMapSection*			_pSectionData;
	
	MPTile*						_pDefaultTile;
	MPTile*                     _RenderTileList[4096];
    int                         _nRenderTileCnt;
    int                         _nUpdateRender;
    int                         _nLastTileStartX;
    int                         _nLastTileStartY;
    int                         _nLastGridStartX;
    int                         _nLastGridStartY;
	int							_nGridShowWidth;
	int							_nGridShowHeight;

	int							_nLastSectionX;
	int							_nLastSectionY;

    float                       _fHeightBuffer[200][200];
    BYTE                        _btBlockBuffer[200][200];
	float						_fTileHeightBuffer[100][100];
	short						_sTileRegionAttr[100][100];


#if(defined DRAW_SEA_USE_DYNAMIC_BUFFER)
    MPSeaTileVertex*            _pVertBuf;
#else
	LPDIRECT3DVERTEXBUFFER8		_pVB;
#endif

    LPDIRECT3DVERTEXBUFFER8		_pLandVB;
    DWORD                       _dwLandVBSize;
    
	
    int							_nWidth;
	int							_nHeight;
	int							_nSectionWidth;		// ÿһ��Section�Ŀ�Ⱥ͸߶�
	int							_nSectionHeight;
	int							_nSectionCntX;		// ��ͼˮƽ������Section�Ŀ���
	int							_nSectionCntY;		// ��ͼ��ֱ������Section�Ŀ���
	int							_nSectionCnt;		// ��ͼ��Section���ܿ���
	
	float						_fShowCenterX;	    // ��̬���������λ��
	float						_fShowCenterY;

	int							_nShowWidth;		// ���ӷ�Χ
	int							_nShowHeight;

	BOOL						_bRenderSea;
	
    std::list<MPActiveMapSection*>	_ActiveSectionList;
	MPActiveMapSection*			    _ActiveSectionArray[512][512];
	int							    _nSectionStartX;
	int							    _nSectionStartY;
    DWORD*                          _pOffsetIdx;

	MAP_PROC					_pfnProc;			// ��ͼ��̬����ص�֪ͨ����
	
	FILE*						_fp;				// ��ͼ�ļ����
	BOOL						_bEdit;				// �Ƿ�ɱ༭
	BOOL						_bSeaVisible;
	BOOL						_bShowCenterPoint;	// �Ƿ���ʾ����ͽ�������Section

	int							_nWaterLoopFrame;
	BOOL						_bUseVB;
    BOOL                        _bUseLandVB;
	BOOL						_bClip;
	BOOL						_bWireFrame;
    int                         _nSectionBufferSize;
    BOOL                        _bBatchRender;
	BOOL						_bEnableNormalLight;
    DWORD                       _dwLoadingCnt;

    DWORD                       _dwSeaDefaultColor;

	LPBYTE						m_pMapData;
	DWORD						m_dwMapDataSize;
	DWORD						m_dwMapPos;
//jze
public:
	void        CreateSkyDoom(D3DXVECTOR3 center, float radius, char* txPath, bool hemisphere = true); //add by jze 2008.6.27
	void		SetupPixelFog(DWORD Color, DWORD Mode,float Start, float End, float Density); //add by jze 2008.7.3
	void        CloseFog(){g_Render.GetDevice()->SetRenderState(D3DRS_FOGENABLE,FALSE);}
	BOOL        IsSkyDoom(){return m_bSkyDoom;}
	void        SetSkyDoom(bool bSkyDoom){m_bSkyDoom = bSkyDoom;}

	void		SetPathFindingRange(int val){ m_iRange = val;}
	ZRBlock*  	GetBlock(){return m_pBlock;}
	int			GetPathFindingRange(){ return m_iRange; }
private:
	BOOL        UseShader();

private:
	DWORD       m_SkyDoomVertexShaderHandle;
	DWORD       m_SkyDoomPixelShaderHandle;

	BOOL						m_bSkyDoom;                        
	float                       m_txMoveSpeed;

	MPSkyDoomVertex*            m_pSkyDoomVB;

	ZRBlock*					m_pBlock;
	int							m_iRange;
};

inline MPTile*	MPMap::GetTile(int nX, int nY)
{
	if(nX >= _nWidth || nY >= _nHeight || nX < 0 || nY < 0) 
	{
		return _pDefaultTile;
	}

	int nSectionX = nX / _nSectionWidth;
	int nSectionY = nY / _nSectionHeight;

	MPActiveMapSection *pSection = GetActiveSection(nSectionX, nSectionY);
	if(pSection && pSection->pTileData) 
	{
		int nOffX = nX % _nSectionWidth;
		int nOffY = nY % _nSectionHeight;
		return pSection->pTileData + nOffY * _nSectionWidth + nOffX;
	}
	
	return _pDefaultTile;
}


inline MPTile* MPMap::GetGroupTile(int nX, int nY, int nGroupNo)
{
	return GetTile(nX + MPTile::Offset[nGroupNo][0], nY + MPTile::Offset[nGroupNo][1]);
}

inline MPActiveMapSection*	MPMap::GetActiveSection(int nSectionX, int nSectionY)
{
	return _ActiveSectionArray[nSectionY][nSectionX];
}

inline BOOL MPMap::IsPointVisible(float fX, float fY)
{
	float fStartX = _fShowCenterX - (float)_nShowWidth  / 2;
	float fStartY = _fShowCenterY - (float)_nShowHeight / 2;
        
    float fEndX   = fStartX + (float)_nShowWidth;
	float fEndY   = fStartY + (float)_nShowHeight;
	
    // fStartX-=3;
    // fStartY-=3;
    // fEndX+=3;
    // fEndY+=3;
	if(fX >= fStartX && fY >= fStartY && fX <= fEndX && fY <= fEndY) return TRUE;
	return FALSE;
}

inline void CopyMapSection(MPActiveMapSection *pSource, MPActiveMapSection *pDest)
{
   memcpy(pDest, pSource, sizeof(MPActiveMapSection));
   memcpy(pDest->pTileData, pSource->pTileData, sizeof(MPTile) * 64);
}

inline float MPMap::GetTileHeight(int x, int y) // ���������
{
    int offx = x - _nLastTileStartX;
    int offy = y - _nLastTileStartY;

    if(offx < 0 || offx >= 100) return 0.0f;
    if(offy < 0 || offy >= 100) return 0.0f;

    return _fTileHeightBuffer[offy][offx];
}

inline short MPMap::GetTileRegionAttr(int x, int y) // ���������
{
    int offx = x - _nLastTileStartX;
    int offy = y - _nLastTileStartY;

    if(offx < 0 || offx >= 100) return 0;
    if(offy < 0 || offy >= 100) return 0;

    return _sTileRegionAttr[offy][offx];
}

inline float MPMap::GetGridHeight(int x, int y) // С��������
{
    int offx = x - _nLastGridStartX;
    int offy = y - _nLastGridStartY;

    if(offx < 0 || offx >= _nGridShowWidth) return 0.0f;
    if(offy < 0 || offy >= _nGridShowHeight) return 0.0f;

    return _fHeightBuffer[offy][offx];
}

inline BYTE MPMap::IsGridBlock(int x, int y) // С��������
{
    int offx = x - _nLastGridStartX;
    int offy = y - _nLastGridStartY;
	
	if(offx < 0 || offx >= _nGridShowWidth)  return 1;
    if(offy < 0 || offy >= _nGridShowHeight) return 1;

    return _btBlockBuffer[offy][offx];
}

#endif