#pragma once
#include "MPRender.h"

#define UI_FVF (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define FONTLEVEN	3

struct UI_VERTEX
{
	float x, y, z,w;
	DWORD diffuse;
	float tu, tv;
};



class CUIPanel
{
public:
	CUIPanel();
	~CUIPanel();
	bool			Create(LPDIRECT3DDEVICE8  pDev);

	void			Draw( LPDIRECT3DTEXTURE8  pSrcTexture, 
		CONST RECT* pSrcRect, CONST D3DXVECTOR2* pScaling, 
		CONST D3DXVECTOR2* pRotationCenter, FLOAT Rotation, 
		CONST D3DXVECTOR2* pTranslation, D3DCOLOR Color);
	void			Begin(){}
	void			End();
public:
	LPDIRECT3DDEVICE8		m_pDev;
protected:
	LPDIRECT3DVERTEXBUFFER8 _lpVB;

	int						_w,_h;

	int						_x,_y;
};

class UIRender
{
public:
	UIRender();
	~UIRender()	    { SAFE_RELEASE(_p2DSprite);  }

    bool Init();

	void RenderSprite(LPTEXTURE tex, RECT* rc,VECTOR2* vscale,VECTOR2* vdest, DWORD dwColor);
	void RenderTextureRect(int nX, int nY, MPTexRect *pRect);

	void RenderTextureAbsRect(int nX, int nY, MPTexRect *pRect);

	void RenderTextureRect(int nX, int nY, MPTexRect *pRect, DWORD dwColor);

	void LineFrame(int left, int top, int right, int bottom, DWORD color=0x90000000 );	// ��һ���߿�
	void SelectChaLineFrame(int left, int top, int right, int bottom, DWORD color=0x90000000 );	// ��һ���߿�
	
	void LineThinFrame(int left, int top, int right, int bottom, DWORD color=0x90000000 );	// ��һ��ϸ��
	void FillFrame(int left, int top, int right, int bottom, DWORD color=0x90000000 );	// ���߿�
	void SetIsConvert( bool v );
	void SetIsChangeResolution(bool v )  {      _bChangeResolution = v ;    }
	bool GetIsChangeResolution()         {      return _bChangeResolution;  }  
		
	void SetClipRect( int x, int y, int w, int h );
	void Reset();

public:	// �������꣺��Ļ���꣬��ͼ���꣬GUI����
	void SetScreen( int w, int h, bool isFull );			// ������Ϸ������ʾ�Ŀ�ߣ�����ʼ����ı���Ļ�ֱ���ʱ����

	// ����Ļ����ת��ΪGUI����
	void ScreenConvert( int &x, int &y ) {
		x = (int)((float)(x) / _fScreenScaleX);
		y = (int)((float)(y) / _fScreenScaleY);
	}
    float ScreenConvertX( int x ) {     
        return (float)x / _fScreenScaleX; 
    }
    float ScreenConvertY( int y ) {    
        return (float)y / _fScreenScaleY; 
    }

	// ����ͼ����ת��ΪGUI����
	void DrawConvert( int &x, int &y ) {
		x = (int)((float)(x) * _fDrawScaleX);
		y = (int)((float)(y) * _fDrawScaleY);
	}
	float DrawConvertX( float x ) {	
        return x * _fDrawScaleX; 
    }
	float DrawConvertY( float y ) { 
        return y * _fDrawScaleY; 
    }

	float DrawConvertX2( float x ) { //added by billy 
        return x / _fDrawScaleX; 
    } 
	float DrawConvertY2( float y ) { 
        return y / _fDrawScaleY; 
    }
	
	int	  GetGuiWidth()		{ return _nGuiWidth;    }
	int	  GetGuiHeight()	{ return _nGuiHeight;   }

    int   GetScreenWidth()  { return _nScrWidth;    }
    int   GetScreenHeight() { return _nScrHeight;   }

	void			OnLostDevice();
	void			OnResetDevice();

	void			RegisterFunc();

	bool			IsFullScreen()	{ return _IsFullScreen;		}

    static UIRender s_Render;

private:
	LPD3DXSPRITE	_p2DSprite;
	//CUIPanel		_p2DSprite;


	float			_fScreenScaleX, _fScreenScaleY;
	float			_fDrawScaleX, _fDrawScaleY;

	float			_fOldScreenScaleX, _fOldScreenScaleY;
	float			_fOldDrawScaleX, _fOldDrawScaleY;

	int				_nGuiWidth, _nGuiHeight;		// GUIϵͳ���,����Ļ��С
	bool			_IsFullScreen;

private:
	//LPTEXTURE		_pTex;				// ����
    int             _nTex;				// ����
	int             _nOutLine ;         // ˽�˿�
	RECT			_rtFrame;			// �����С
	VECTOR2			_vesLevel;
	VECTOR2			_vesVertical;

	VECTOR2			_vesLevelThin;
	VECTOR2			_vesVerticalThin;

    int             _nScrWidth;
    int             _nScrHeight;
	bool            _bChangeResolution;

};

inline UIRender& GetRender() { return UIRender::s_Render;   }

extern bool UIGetChooseColor( DWORD &c );					// ��ɫ�Ի���,cΪĬ����ɫ,ͬʱҲ��Ϊִ�гɹ�ʱ�ķ���ֵ
extern char* UIGetOpenFileName( char* strInitDir=NULL );	// ���ļ��Ի���,�ɹ������ļ�����,���򷵻�NULL

inline void UIRender::RenderSprite(LPTEXTURE tex, RECT* rc,VECTOR2* vscale,VECTOR2* vdest, DWORD dwColor)
{
	ui::UIClip* pCliper = ui::UIClip::Instance();
	if(pCliper->GetClipState())
	{
		int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
		int w = rc->right - rc->left;
		int h = rc->bottom - rc->top;

		RECT* prc = &pCliper->GetClipRect();
		
		// ͼ���ڲü����⣬����ʾ
		if( vdest->x > prc->right || vdest->x < prc->left - w )
		{
			return;
		}

		// ���вü�
		if( vdest->x < prc->left )
		{
			x1 = (int)vdest->x - prc->left;
			vdest->x = (float)prc->left;
		}
		if( vdest->x + w > prc->right )
		{
			x2 = prc->right - (int)(vdest->x + w);
		}
		if( vdest->y < prc->top )
		{
			y1 = (int)vdest->y - prc->top;
			vdest->y = (float)prc->top;
		}
		if( vdest->y + h > prc->bottom )
		{
			y2 = prc->bottom - ((int)vdest->y + h);
		}
		rc->left -= x1;
		rc->right += x2;
		rc->top -= y1;
		rc->bottom += y2;
	}
     if(_p2DSprite) _p2DSprite->Draw(tex, rc, vscale, NULL, 0, vdest, dwColor);
}
