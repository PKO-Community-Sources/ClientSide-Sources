
#if 1
#include <d3d8.h>
#include <d3dx8.h>
#define LPSURFACE LPDIRECT3DSURFACE8
#define LPTEXTURE LPDIRECT3DTEXTURE8
#define LPD3D	  IDirect3DX*
#define LPDEVICE  LPDIRECT3DDEVICE8
#endif

//#include "lwDirectX.h"
//#define LPSURFACE IDirect3DSurfaceX*
//#define LPTEXTURE IDirect3DTextureX*
//#define LPD3D	  IDirect3DX*
//#define LPDEVICE  IDirect3DDeviceX*



LPTEXTURE LoadTextureFromRawFile(char *strFileName);

extern LPDEVICE g_pd3dDevice;

