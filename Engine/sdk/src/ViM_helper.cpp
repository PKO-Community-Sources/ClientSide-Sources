#include "Stdafx.h"
#include "ViM_helper.h"
#include "MindPowerAPI.h"
#include "mprender.h"

namespace Vision3D
{
	bool                    regular_game = true;
	VIMD3D					g_vimd3d = NULL;	// ViM
	LPDIRECT3DTEXTURE8*		g_pRenderTextures;	// ViM
	LPDIRECT3DSURFACE8*		g_pRenderTargets;	// ViM
	DWORD					g_dwNumViews;		// ViM
	DWORD					g_xpos, g_ypos, g_width, g_height, g_adapterIdx, g_refreshRate;	// ViM
	FLOAT					g_fCamNear;
	FLOAT					g_fCamFar;
	FLOAT					g_fCamTarget;
	DWORD					g_descWidth, g_descHeight;

	ViM_Helper				g_vimHelper;

	ViM_Helper::ViM_Helper()
	{
		//_init();
	}

	ViM_Helper::~ViM_Helper()
	{
		//_destroy();
	}

	void ViM_Helper::onDeviceLost()
	{
	}

	DWORD ViM_Helper::_init()
	{
		g_pRenderTextures = NULL;	// ViM
		g_pRenderTargets = NULL;	// ViM
		g_dwNumViews = 0;			// ViM
		//g_fCamTranslation = 1.0;	//0.01
		//g_fCamFixpoint = 60.0;
		g_fCamNear = 0.75f;
		g_fCamFar = 1.5f;
		g_vimd3d = NULL;
	

		if (ViMD3D_InitLib(NULL))
		{

			if (ViMD3D_GetInfo(g_dwNumViews,					// ViM
				g_xpos, g_ypos, g_width, g_height,				// ViM
				g_adapterIdx, g_refreshRate))					// ViM
			{
				regular_game=false;
			}
		}

		//Load configuration file

		this->Load("scripts/kop.cfg");


		return 1;
	}

	void ViM_Helper::_destroy()
	{
		if (g_pRenderTargets)							// ViM
		{									// ViM
			for (DWORD i=0;i<g_dwNumViews;i++)					// ViM
			{									// ViM
				if (g_pRenderTargets[i])						// ViM
					g_pRenderTargets[i]->Release();					// ViM
			}									// ViM
			delete [] g_pRenderTargets; g_pRenderTargets = 0;		// ViM
		}									// ViM
		if (g_pRenderTextures)							// ViM
		{									// ViM
			for (DWORD i=0;i<g_dwNumViews;i++)					// ViM
			{									// ViM
				if (g_pRenderTextures[i])						// ViM
					g_pRenderTextures[i]->Release();					// ViM
			}									// ViM
			delete [] g_pRenderTextures; g_pRenderTextures = 0;	// ViM
		}									// ViM
		if(g_vimd3d) { ViMD3D_Release(g_vimd3d); g_vimd3d=0; }					// ViM
	}

	void ViM_Helper::_create()
	{
		//  Need to get g_pd3dDevice .......
		IDirect3DDevice8 *g_pd3dDevice= g_Render.GetDevice();

		if (FAILED(ViMD3D_Create(g_pd3dDevice, &g_vimd3d)))			// ViM
			return;							// ViM

		D3DSURFACE_DESC desc;						// ViM
		LPDIRECT3DSURFACE8 surf=0;		         		// ViM
		if (FAILED(g_pd3dDevice->GetRenderTarget(&surf)))		// ViM
			return;							// ViM
		if (FAILED(surf->GetDesc(&desc)))		        	// ViM
			return;							// ViM

		g_descWidth  = desc.Width;
		g_descHeight = desc.Height;

		g_pRenderTextures = new LPDIRECT3DTEXTURE8[g_dwNumViews];				// ViM
		ZeroMemory(g_pRenderTextures, g_dwNumViews*sizeof(LPDIRECT3DTEXTURE8));			// ViM
		g_pRenderTargets = new LPDIRECT3DSURFACE8[g_dwNumViews];				// ViM
		ZeroMemory(g_pRenderTargets, g_dwNumViews*sizeof(LPDIRECT3DSURFACE8));			// ViM

		for (DWORD i=0;i<g_dwNumViews;i++)							// ViM

		{								         		// ViM
			if (FAILED(g_pd3dDevice->CreateTexture(desc.Width/*/2*/, desc.Height/*/2*/, 1, 		// ViM
				D3DUSAGE_RENDERTARGET, desc.Format, D3DPOOL_DEFAULT, 				// ViM
				&g_pRenderTextures[i])))							// ViM
				return;																		// ViM
			if (FAILED(g_pRenderTextures[i]->GetSurfaceLevel(0, &g_pRenderTargets[i])))      	// ViM
				return;																		// ViM
		}               		// ViM
		surf->Release();	// ViM

		//std::ofstream file("file.txt");
		//file<< ", g_xpos :" <<g_xpos
		//	<< ", g_ypos :" <<g_ypos
		//	<< ", g_width :" <<g_width
		//	<< ", g_height :" <<g_height
		//	<< ", g_adapterIdx :" <<g_adapterIdx
		//	<< ", g_refreshRate :" <<g_refreshRate
		//	<< ", g_fCamTranslation :" <<g_fCamTranslation
		//	<< ", g_fCamFixpoint :" <<g_fCamFixpoint
		//	<< ", g_descWidth :" <<g_descWidth
		//	<< ", g_descHeight :" <<g_descHeight;
		//file.close();
	}

	void ViM_Helper::OutputMatrix ( std::ofstream &ofs, const D3DXMATRIX *m )
	{
		for ( int i = 0; i < 4; ++i )
		{
			for ( int j = 0; j < 4; ++j )
			{
				const float*f = ( ( const float* )m ) + i * 4 + j;
				ofs << *f << " ";
			}
			ofs << std::endl;
		}
		ofs << std::endl;
	}

	D3DXMATRIX * ViM_Helper::GetVimProjMatrix ( int viewIndex )
	{
		static D3DXMATRIXA16 mat;

		// get view from  the loop   Need set view and get view subroutine to be consistent 
		// get 3d device and set it to g_pd3dDevice
		//float fov_y    =  g_Render.GetWorldViewFOV();
		//float zn       =  g_Render.GetNearClipPlane();
		//float zf       =  g_Render.GetFarClipPlane();
		//int vp_Width   =  g_Render.GetWorldViewWidth()/2;  
		//int vp_Height  =  g_Render.GetWorldViewHeight()/2;

		//ViMD3Dutil_MatrixPerspectiveFovLH(&mat,D3DX_PI/8,(float)g_descWidth/g_descHeight, 0.1f, 1000.0f,	// ViM
		//	g_fCamTranslation,g_fCamFixpoint,viewIndex, g_dwNumViews); //ViM

		return &mat;
	}

	// Taken from GameConfig.cpp
	void ViM_Helper::Load(char *pszFileName)
	{
		ifstream in(pszFileName);
		if(in.is_open()==0)
		{
			return;
		}
		string strPair[2];
		string strComment;
		string strLine;
		char szLine[255];
		while(!in.eof())
		{
			in.getline(szLine, 255);
			strLine = szLine;
			int p = (int)(strLine.find("//"));
			if(p!=-1)
			{
				string strLeft = strLine.substr(0, p);
				strComment = strLine.substr(p + 2, strLine.size() - p - 2);
				strLine = strLeft;
			}
			else
			{
				strComment = "";
			}
			Util_TrimString(strLine);
			if(strLine.size()==0) continue;
			if(strLine[0]=='[') 
			{
				Log("\n%s\n", strLine.c_str());
				continue;
			}
			
			int n = Util_ResolveTextLine(strLine.c_str(), strPair, 2, '=');
			if(n < 2) continue;
			string strKey   = strPair[0];
			string strValue = strPair[1];
			
			/////////////////////////
			if(strKey=="near3d")
			{
				g_fCamNear = Str2Float(strValue);
				if(g_fCamNear < 0.1f ) g_fCamNear = 0.1f; 
			}else if(strKey=="far3d")
			{
				g_fCamFar = Str2Float(strValue);
				if(g_fCamFar < 0.5f ) g_fCamFar = 0.5f; 
			}
		
   		}
		in.close();
	}
}
