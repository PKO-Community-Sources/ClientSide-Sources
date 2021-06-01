#pragma once


// �汾˵��
// ��ǰ�汾ת��Ϊ map 780625�� -> 780626��
//                obj 600��    -> 700��
// Ŀ�����ڽ�һ������map�ļ���obj�ļ��ĳߴ�
// �˰汾ʹ��short����ʾ�ر�����ɫ, byte����ʾ�ر���߶�
// ͨ�����;�������С�ļ��ߴ�, �°汾���ļ�����С1/3�ĳߴ�


#define MP_MAP_FLAG		 780624   // Map�ļ��汾����ʼ
#define LAST_VERSION_NO  780626	  // ��һ�ΰ汾��Ҫ֧��ת��	
#define CUR_VERSION_NO   780627	  // ��ǰʹ�õİ汾��

// ��ͼ�ļ��ļ�ͷ����
struct MPMapFileHeader
{
	int		nMapFlag;
	int		nWidth;
	int     nHeight;
	int		nSectionWidth;
	int		nSectionHeight;
};


#pragma pack(push)
#pragma pack(1)

struct SNewFileTile // �ܼ�15���ֽ�
{
	DWORD	dwTileInfo;	// ����3��layer��tex no��alpha no
	BYTE	btTileInfo; // ���²��tex no
	short	sColor;		// 32bit����ɫ����Ϊ565
	char	cHeight;	// ÿ10cmΪһ��������߶�
	short	sRegion;	
	BYTE	btIsland;
	BYTE	btBlock[4];
	
	SNewFileTile(): sRegion(0), btIsland(0)
    {
        btBlock[0] = 0;
        btBlock[1] = 0;
        btBlock[2] = 0;
        btBlock[3] = 0;
    }
};

struct SFileTile
{
    BYTE  t[8];
    short sHeight;    // ������Ϊ��λ�ĵر���߶�
    DWORD dwColor;
    short sRegion;
    BYTE  btIsland;
    BYTE  btBlock[4]; // 4 x 1���ϰ��͸߶ȼ�¼

    SFileTile(): sRegion(0), btIsland(0)
    {
        btBlock[0] = 0;
        btBlock[1] = 0;
        btBlock[2] = 0;
        btBlock[3] = 0;
    }
};

#pragma pack(pop)



// ԭ�е�8���ֽڵ�Tile��Ϣ, ת��Ϊ5���ֽڱ�ʾ
// ÿ����ͼ���ռ6λ, �� 1 ~ 63, ����ʾ63��, 0����
// ÿ��Alpha���ռ4λ, 0 ~ 15, ����ʾ16����ʽ
inline void TileInfo_8To5(BYTE *pbtTile, DWORD &dwNew, BYTE &btNew)
{
	DWORD dwTex1   = *(pbtTile + 2);
	DWORD dwTex2   = *(pbtTile + 4);
	DWORD dwTex3   = *(pbtTile + 6);
	DWORD dwAlpha1 = *(pbtTile + 3);
	DWORD dwAlpha2 = *(pbtTile + 5);
	DWORD dwAlpha3 = *(pbtTile + 7);

	dwNew = dwTex1<<26 | dwAlpha1<<22 | dwTex2<<16 | dwAlpha2<<12 | dwTex3<<6 | dwAlpha3<<2;
	btNew = *(pbtTile + 0);
}

// 5���ֽڵ�8���ֽڵķ���ת��
inline void TileInfo_5To8(DWORD dwNew, BYTE btNew, BYTE *pbtTile)
{
	*(pbtTile + 0) = btNew;
	*(pbtTile + 1) = 15;
	*(pbtTile + 2) = (BYTE)(dwNew>>26);
	*(pbtTile + 3) = (BYTE)(dwNew>>22) & 0x000f;
	*(pbtTile + 4) = (BYTE)(dwNew>>16) & 63;
	*(pbtTile + 5) = (BYTE)(dwNew>>12) & 0x000f;
	*(pbtTile + 6) = (BYTE)(dwNew>>6)  & 63;
	*(pbtTile + 7) = (BYTE)(dwNew>>2)  & 0x000f;
}

#define NEW_VERSION  // MindPower��ͼ�����Ƿ�ʹ���°汾

