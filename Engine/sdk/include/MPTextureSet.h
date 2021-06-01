#pragma once

#include "TableData.h"
#include "MPRender.h"

#pragma warning(disable: 4275)

class MINDPOWER_API MPTexInfo : public CRawDataInfo
{
public:
	
	MPTexInfo()
	{
		bAlpha   = 0;
        btMipmap = 5;
        bExist   = TRUE;
	}

public:	

	BYTE    bAlpha;
    BYTE    btMipmap;
	short	sWidth;
	short	sHeight;
};

#pragma pack(push,__Data_Align)
#pragma pack(1)

struct STGAHeader
{
	unsigned char    id_len;
	unsigned char    cm_type;
	unsigned char    type;
	unsigned short   cm_start;
	unsigned short   cm_len;
	unsigned char    cm_bits;
	unsigned short   xorg;
	unsigned short   yorg;
	unsigned short   width;
	unsigned short   height;
	unsigned char    bpp;
	unsigned char    flags;
};

#pragma pack(pop,__Data_Align)


class MINDPOWER_API MPTexSet : public CRawDataSet
{

public:
	
	static MPTexSet* I() { return _Instance; }

    MPTexSet(int nIDStart, int nIDCnt)
	:CRawDataSet(nIDStart, nIDCnt)
	{
	    _Init();
        _Instance = this;
        _nTextureLevel = D3DX_DEFAULT;
		CRawDataSet::SetReleaseInterval(8000);
	}

    
	BOOL IsAlphaTGA(char* pFileBuf)
	{
		STGAHeader header;
		memcpy(&header, pFileBuf, sizeof(STGAHeader));
		return (header.bpp == 32);
	}

	BOOL IsAlphaTexture(int nID)
	{
		MPTexInfo* pInfo = (MPTexInfo*)GetRawDataInfo(nID);
		if(pInfo && pInfo->bAlpha== 1)
		{
			return TRUE;
		}
		return FALSE;
	}

	void    ReloadAllTexture()
    {
        for(int i = 0; i < _nIDCnt; i++)
	    {
		    CRawDataInfo *pInfo = GetRawDataInfo(_nIDStart + i);
		    if(pInfo->pData)
            {
                _DeleteRawData(pInfo);
		        pInfo->pData = NULL;
		        _nLoadedRawDataCnt--;
		        if(_nLoadedRawDataCnt < 0)
		        {
			        LG("error", "LoadedRawDataCnt = %d , < 0 ?\n", _nLoadedRawDataCnt);
		        }
                GetRawData(pInfo->nID);
            }
        }
    }
    
    void    SetTextureLevel(int nLevel, BOOL bUpdateAll = FALSE)
    {
        if(_nTextureLevel!=nLevel)
        {
            _nTextureLevel = nLevel;
            ReloadAllTexture();
        }
    }

    int     GetTextureLevel()  { return _nTextureLevel; }

protected:

	static MPTexSet* _Instance; // 相当于单键, 把自己记住
    
    virtual CRawDataInfo* _CreateRawDataArray(int nIDCnt)
	{
		return new MPTexInfo[nIDCnt];
	}
	
	virtual void _DeleteRawDataArray()
	{
		delete[] (MPTexInfo*)_RawDataArray;
	}
	
	virtual int _GetRawDataInfoSize()
	{
		return sizeof(MPTexInfo);
	}

	virtual BOOL _IsFull()
	{
		// 如果显存占用超过8M,则开始进行界面贴图和地表贴图的动态释放
		DWORD dwMem = g_Render.GetRegisteredDevMemSize();
		if(rand()%100 < 2)
		{
			LG("vmem", "vmem = %d k\n", dwMem / 1024);
		}
		if(dwMem >= 64 * 1024 * 1024)
		{
			return TRUE;
		}
		return FALSE;
	}

	virtual void* _CreateNewRawData(CRawDataInfo *pInfo)
	{
#define USE_MANAGED_TEXTURE

#ifdef USE_MANAGED_TEXTURE

        MPTexInfo *pTexInfo = (MPTexInfo*)pInfo;
        lwITex* tex;
        lwIResourceMgr* res_mgr = g_Render.GetInterfaceMgr()->res_mgr;
        res_mgr->CreateTex(&tex);

        lwTexInfo tex_info;
        lwTexInfo_Construct(&tex_info);
        _tcscpy(tex_info.file_name, pTexInfo->szDataName);
        _tcslwr(tex_info.file_name);
        tex_info.pool = D3DPOOL_MANAGED;
        tex_info.usage = 0;
        tex_info.level = D3DX_DEFAULT;
        tex_info.type = TEX_TYPE_FILE;


        D3DFORMAT tex_fmt[2];
        tex_fmt[0] = g_Render.GetTexSetFormat(0);
        tex_fmt[1] = g_Render.GetTexSetFormat(1);

        // 针对dds文件格式不需要设置，只判断tga格式
        /*
        BYTE* pbtBuf = LoadRawFileData(pTexInfo);
        if(pbtBuf)
        {
            tex_info.format = tex_fmt[IsAlphaTGA((char*)pbtBuf)];
            delete[] pbtBuf;
        }
        else
        {
            tex_info.format = tex_fmt[0];
        }
        */
        size_t l = _tcslen(tex_info.file_name);
        if(tex_info.file_name[l - 1] == 'a'
            && tex_info.file_name[l - 2] == 'g'
            && tex_info.file_name[l - 3] == 't')
        {
            tex_info.format = tex_fmt[1];
        }
        else
        {
            tex_info.format = tex_fmt[0];
        }

#if 1
        if(_tcsstr(tex_info.file_name, "ui"))
        {
            tex_info.level = D3DX_DEFAULT;

            size_t str_len = _tcslen(tex_info.file_name);

            if(_tcsicmp(&tex_info.file_name[str_len - 3], "bmp") == 0)
            {
                tex_info.colorkey_type = COLORKEY_TYPE_COLOR;
                tex_info.colorkey.color = 0xffff00ff;
                tex_info.format = tex_fmt[1];
            }
        }
#endif

        tex->LoadTexInfo(&tex_info, NULL);
        tex->LoadVideoMemory();

		tex->GetTexInfo(&tex_info);

        //return tex->GetTex();
		pTexInfo->sWidth  = (short)tex_info.width;
		pTexInfo->sHeight = (short)tex_info.height; 
		LG("tex_release", "Load Texture [%s] size = %d %d, id = %d\n",  pTexInfo->szDataName, pTexInfo->sWidth, pTexInfo->sHeight, pInfo->nID);
        return tex;


#else
        MPTexInfo *pTexInfo = (MPTexInfo*)pInfo;

        char pszSrcName[64] = "";
        
        BOOL bReplaceDataName = FALSE;
        if(_nTextureLevel > 0)
        {
            strcpy(pszSrcName, pTexInfo->szDataName);
            char *pszLevelTex = "tex___1";
            memcpy(pTexInfo->szDataName, pszLevelTex, 7);
            
            if(!Util_IsFileExist(pTexInfo->szDataName))
            {
               strcpy(pTexInfo->szDataName, pszSrcName);
            }
            else
            {
                bReplaceDataName = TRUE;
            }
        }
        
        
        BOOL bDDS = FALSE;
		DWORD  dwBufSize;
		
		// get 'dds' texture file name
		CRawDataInfo ddsInfo;
		int len = (int)(strlen(pInfo->szDataName));
		strcpy(ddsInfo.szDataName, pInfo->szDataName);
		ddsInfo.szDataName[len - 3] = 'd';
		ddsInfo.szDataName[len - 2] = 'd';
		ddsInfo.szDataName[len - 1] = 's';

		// try 'dds' texture at 1st time 
		LPBYTE pbtBuf = NULL;
		
		/*
		if(pTexInfo->nMipmap != 0xff && g_pDevice->m_bEnableDXT)	// UI texture don't use 'dds'
		//if (0)
		{
			pbtBuf = LoadRawFileData(&ddsInfo);
		}
		*/

		if (!pbtBuf) 
		{
			// try 'tga' texture at 2nd time 
			pbtBuf = LoadRawFileData(pTexInfo);
			if (!pbtBuf)
			{
		        if(bReplaceDataName)
                {
                    strcpy(pTexInfo->szDataName, pszSrcName);
                }
        		return NULL;
			}
			else
			{
				dwBufSize = pTexInfo->dwDataSize;
			}
		}
		else
		{
			dwBufSize = ddsInfo.dwDataSize;

			/*DDSURFACEDESC2 ddsc2;
			memcpy(&ddsc2, pbtBuf + 4, sizeof(DDSURFACEDESC2));
			if (ddsc2.dwAlphaBitDepth > 0)
			{
				pTexInfo->bAlpha = 1;
			}
			else
			{
				pTexInfo->bAlpha = 0;
			}*/
			bDDS = TRUE;
		}
		
        if(bReplaceDataName)
        {
           strcpy(pTexInfo->szDataName, pszSrcName);
        }
        		
		if (!bDDS)
		{
			if(IsAlphaTGA((char*)pbtBuf))
			{
				pTexInfo->bAlpha = 1;
			}
			else
			{
				pTexInfo->bAlpha = 0;
			}
		}

        DWORD   dwUsage  = 0;
        D3DPOOL Manage   = D3DPOOL_MANAGED;
        DWORD   dwFilter = D3DX_FILTER_NONE;
		DWORD   dwMipFilter = D3DX_FILTER_NONE;
		BYTE btMipMapLv = 1;
        if(pTexInfo->btMipmap != 0xff)
        {
            dwFilter    = D3DX_FILTER_LINEAR;
			dwMipFilter = D3DX_DEFAULT;
			btMipMapLv  = pTexInfo->btMipmap;
        }
    
        btMipMapLv  = D3DX_DEFAULT;
        
		// DWORD  dwBufSize = pInfo->dwDataSize;
		
		IDirect3DTextureX* pTexture = NULL;
	

		D3DFORMAT fmt = D3DFMT_R5G6B5;
		if(pTexInfo->bAlpha) fmt = D3DFMT_A4R4G4B4;

		if( FAILED( D3DXCreateTextureFromFileInMemoryEx(g_Render.GetDevice(),
			pbtBuf,
			dwBufSize,
			D3DX_DEFAULT,				// default width
			D3DX_DEFAULT,				// default height
		    btMipMapLv,					// mipmap level
			dwUsage,					// usage
			// D3DFMT_DXT3,
			fmt,			       		// Texture Format
			Manage, 	        		// Pool
			dwFilter,       			// texture filter
			dwMipFilter,				// MipMap filter
			0x00000000,					// Alpha 
			NULL,						// info
			NULL,						// palette
			&pTexture) ))				// texture
		{	
			// texture
			LG("error", "Create Texture From Data[%s] Failed!\n", pInfo->szDataName);
			pTexture = NULL;
		}
		else
		{
			LG("debug", "Load Texture Data [%s]\n", pInfo->szDataName);
			//D3DSURFACE_DESC desc;
			//pTexture->GetLevelDesc(0, &desc);
			//pTexInfo->nWidth  = desc.Width;
			//pTexInfo->nHeight = desc.Height;
		}
		delete[] pbtBuf;

		return pTexture;
#endif
	}

	virtual void  _DeleteRawData(CRawDataInfo *pInfo)
	{
		MPTexInfo *pTexInfo = (MPTexInfo*)pInfo;
        LG("tex_release", "Release Texture Data [%s], size = %d %d\n", pTexInfo->szDataName, pTexInfo->sWidth, pTexInfo->sHeight);
        // by lsh
        // __asm
        // {
        //    int 3;
        // }
#if(defined USE_MANAGED_TEXTURE)
        lwITex* pTex = (lwITex*)pInfo->pData;
#else
		IDirect3DTextureX* pTex = (IDirect3DTextureX*)(pInfo->pData);
#endif
		SAFE_RELEASE(pTex);
	}
	
	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
	{
        if(ParamList.size()==0) return FALSE;
		
        string strParam;
		for(vector<string>::iterator it = ParamList.begin(); it!=ParamList.end(); it++)
		{
			strParam+=(*it);
		}
		
		MPTexInfo *pTexInfo = (MPTexInfo*)pRawDataInfo;
        return TRUE;
    }

protected:

    int     _nTextureLevel;
};

//--------------
// 快捷全局函数:
//--------------

inline lwITex* GetTexByID(int nID, BOOL bRequest = FALSE)
{
    lwITex* tex = (lwITex*)MPTexSet::I()->GetRawData(nID, bRequest);
    return tex;
}

inline IDirect3DTextureX* GetTextureByID(int nID, BOOL bRequest = FALSE) // 通过ID取得Texture表面
{
#ifdef USE_MANAGED_TEXTURE
    lwITex* tex = (lwITex*)MPTexSet::I()->GetRawData(nID, bRequest);
    return tex ? tex->GetTex() : 0;
#else
    return (IDirect3DTextureX*)MPTexSet::I()->GetRawData(nID, bRequest);
#endif
}

inline SIZE GetTextureSizeByID(int nID)
{
    SIZE sz = { 0, 0};
#ifdef USE_MANAGED_TEXTURE
    lwITex* tex = (lwITex*)MPTexSet::I()->GetRawData(nID, 0);
    if(tex)
    {
        lwTexInfo info;
        tex->GetTexInfo(&info);
        sz.cx = info.width;
        sz.cy = info.height;
    }
#endif

    return sz;
}

inline int GetTextureID(const char *pszDataName)  // 通过名字取得Texture ID, 如果是动态ID方式又没有查找到ID, 就会自动分配一个, 返回-1表示失败
{
    if(MPTexSet::I()!=NULL)
	{
		return MPTexSet::I()->GetRawDataID(pszDataName);
	}
	return 0;
}

inline MPTexInfo* GetTextureInfo(int nID) // 通过ID取得Texture描述信息
{
	if(MPTexSet::I()!=NULL)
	{
		return (MPTexInfo*)MPTexSet::I()->GetRawDataInfo(nID);
	}
	return NULL;
}

inline BOOL IsAlphaTexture(int nID)
{
	MPTexInfo *pInfo = GetTextureInfo(nID);
	if(!pInfo) return FALSE;
	return pInfo->bAlpha==1;
}

inline BOOL IsTextureExist(const char *pszDataName)
{
    if(MPTexSet::I()->IsEnablePack()==FALSE) // 非打包方式下
    {
        if(Util_IsFileExist((char*)pszDataName))
        {
            return TRUE;
        }
        return FALSE;
    }
    else
    {
        MPTexInfo *pInfo = (MPTexInfo*)MPTexSet::I()->GetRawDataInfo(pszDataName);
        if(pInfo)
        {
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

#pragma warning(default: 4275)