
//
// TerrainAttrib.h
//
//  by claude at 2004-9-13
//

#pragma once

#include <stdio.h>
#include <memory.h>
#include <io.h>


#define ATTRIB_FILE_DEFAULT_WIDTH 4096
#define ATTRIB_FILE_DEFAULT_HEIGHT 4096

// ��������ֵ����
#define TILE_ATTRIB_MIN_BITS 1
#define TILE_ATTRIB_MAX_BITS 16
#define TILE_ATTRIB_DEFAULT_VALUE 0

// ����ֵ����
#define TILE_ISLAND_MIN_VALUE 0
#define TILE_ISLAND_MAX_VALUE 200
#define TILE_ISLAND_DEFAULT_VALUE TILE_ISLAND_MIN_VALUE

typedef unsigned char byte;


// ���Ÿ��ͻ���ʹ�ã�����filename������չ��
bool createAttribFile(char const* filename, int width = ATTRIB_FILE_DEFAULT_WIDTH,
					  int height = ATTRIB_FILE_DEFAULT_HEIGHT,
					  int option = 0);
void setTileAttrib(int nX, int nY, byte btAttrib, bool bAdd = true);
void setTileIsland(int nX, int nY, byte btIsland);
bool openAttribFile(char const* filename);
bool getAttribFileInfo(int& width, int& height);
bool getTileAttrib(int x, int y, unsigned short& attrib);
bool hasTileAttrib(int x, int y, byte attrib);
bool delTileAttrib(int x, int y, byte attrib);
bool getTileIsland(int x, int y, byte& index);


// �����ֻʹ�����½ӿڣ��ͻ�������ʹ�ã�
int s_openAttribFile(char const* filename); // ���� -1 �����
bool s_getAttribFileInfo(int id, unsigned int& width, unsigned int& height);
bool s_getTileAttrib(int id, unsigned int x, unsigned int y, unsigned short& attrib);
bool s_hasTileAttrib(int id, unsigned int x, unsigned int y, byte attrib_mask);
bool s_getTileIsland(int id, unsigned int x, unsigned int y, byte& island_index);

// ���ö��뷽ʽΪ1�ֽڣ���ΪҪд���ļ���
#pragma pack(push)
#pragma pack(1)

// ���������ļ��ļ�ͷ����
struct SAttribFileHeader
	{
	unsigned int width;
	unsigned int height;
	};
typedef SAttribFileHeader terrain_attr_hdr;

// ���������ļ���λ���ݶ���
typedef struct _Tile_Attrib
	{
	unsigned short attrib; // tile����
	byte island; // �������

	} STILE_ATTRIB;
typedef _Tile_Attrib terrain_attr_dat;

#pragma pack(pop)
