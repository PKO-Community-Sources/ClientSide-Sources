#pragma once

#include <ViMD3DLib.h>

namespace Vision3D
{
	extern bool                     regular_game;
	extern VIMD3D					g_vimd3d;			// ViM
	extern LPDIRECT3DTEXTURE8*		g_pRenderTextures;	// ViM
	extern LPDIRECT3DSURFACE8*		g_pRenderTargets;	// ViM
	extern DWORD					g_dwNumViews;		// ViM
	extern DWORD					g_xpos, g_ypos, g_width, g_height, g_adapterIdx, g_refreshRate;	// ViM
	extern FLOAT					g_fCamNear;
	extern FLOAT					g_fCamFar;		// ViM  Camera Fixpoint/Focal tuning parameter
	extern FLOAT					g_fCamTarget;
	
	extern DWORD					g_descWidth, g_descHeight;

	class ViM_Helper
	{
	public :
		ViM_Helper();
		~ViM_Helper();

		void onDeviceLost();
		void _create();
		void _destroy();
		DWORD _init();
		void SetViewIndex ( int i )
		{
			my_viewIndex = i;
		}
		void GetViewIndex ( int &i )
		{
			i = my_viewIndex;
		}

		D3DXMATRIX * GetVimProjMatrix ( int viewIndex );

		void OutputMatrix ( std::ofstream &ofs, const D3DXMATRIX *m );

		bool Is3D()				{ return !regular_game; }

	private :
		
		void Load(char *pszFileName);
		
		int my_viewIndex;

	};

	extern ViM_Helper g_vimHelper;
}