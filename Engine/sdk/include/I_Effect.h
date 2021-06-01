#pragma once
#define		USE_MGR
#define		USE_RENDER

//#define		USE_GAME


//#define		USE_DDS_FILE USE_DDS_FILE_EFFECT

//#ifdef USE_DDS_FILE
//#undef USE_DDS_FILE 

//#endif

#include <Util.h>
//stl
# include <string>
# include <list>
# include <map>
# include <vector>
#include <algorithm>

class   MPRender;
class   CMPResManger;

//--------------------------------------------------------------------------------------
//常用的struct or type.
//--------------------------------------------------------------------------------------
#define    s_string								string          
#define    LIST_string							list<s_string>  
#define    VEC_string							vector<s_string>
#define    LIST_int								list<int>       
#define    LIST_dword							list<DWORD>     
#define    VEC_dword							vector<DWORD>   
#define    VEC_word								vector<WORD>    
#define    VEC_int								vector<int>     
#define    VEC_bool								vector<bool>    
#define    VEC_ptr								vector<DWORD>   
#define    VEC_BYTE								vector<BYTE>    
#define    VEC_float							vector<float>   

/////////////////////////////////////////////////////////////////////////
template<class _Ty>	class S_BVECTOR
{
	std::vector<_Ty>	m_VECPath;
	int m_nCount;
	int m_nPos;
public:
	S_BVECTOR()
	{
		m_VECPath.clear();
		m_nCount =0;
		m_nPos = 0;
	}
	void resize(WORD _nSize = 100)
	{
		m_VECPath.clear();
		m_VECPath.resize(_nSize);
		clear();
	}
	void addsize(WORD _nSize = 100)
	{
		WORD size = (WORD)m_VECPath.size();
		m_VECPath.resize(size + _nSize);
	}
	void setsize(WORD _nSize = 1)
	{		
		if( m_VECPath.size() < _nSize )
		{
			m_VECPath.resize( _nSize );
			//setsizeNew( _nSize );
		}
		clear();
		m_nCount = _nSize;
	}
	void setsizeNew(WORD _nSize = 1)
	{
		std::vector<_Ty> temp;
		temp.resize( m_nCount );
		for( WORD i = 0; i < m_nCount; i++ )
		{
			temp[i] = m_VECPath[i];
		}
		
		m_VECPath.resize( _nSize );
		for( WORD i = 0; i < m_nCount; i++ )
		{
			m_VECPath[i] = temp[i];
		}
	}
	void clear()
	{
		m_nCount = 0;
		m_nPos = 0;
	}
	void push_back(_Ty &_Base)
	{
		m_VECPath[m_nCount] = _Base;
		++m_nCount;
	}
	void pop_front()
	{
		if(m_nCount > m_nPos)
		{
			++m_nPos;
			if(m_nPos == m_nCount)
			{
				clear();
			}
		}
	}
	void pop_back()
	{
		if(m_nCount > m_nPos)
		{
			m_nCount--;
			if(m_nPos == m_nCount)
			{
				clear();
			}
		}
	}
	int		size()
	{
		return m_nCount - m_nPos;
	}
	bool	empty()
	{
		return m_nPos == m_nCount ? true : false;
	}
	_Ty* front()
	{
		if(!empty())
			return &m_VECPath[m_nPos];
		return NULL;
	}
	_Ty* next()
	{
		if(!empty())
		{
			if( (m_nPos + 1) != m_nCount)
				return &m_VECPath[m_nPos + 1];
		}
		return NULL;
	}
	_Ty* end()
	{
		if(!empty())
		{
			return &m_VECPath[m_nCount - 1];
		}
		return NULL;
	}
	_Ty* operator[] ( int i )
	{
		if(!empty() && i >= 0 && i < size())
		{
			return &m_VECPath[m_nPos + i];
		}
		return NULL;
	}
	void remove(int _iIndex)
	{
		if(empty())
			return;
		int ipos = m_nPos + _iIndex;

			for(int n = ipos; n < m_nCount-1; ++n)
			{
				m_VECPath[n] = m_VECPath[n+1];
			}
			m_nCount--;
			if(m_nPos == m_nCount)
			{
				clear();
			}
			return;
	}
};

class CMemoryBuf
{
public:
	CMemoryBuf()
	{
		_pData = NULL;
		_lpos = 0;
		_size = 0;
	}
	~CMemoryBuf()
	{
		SAFE_DELETE_ARRAY(_pData);
	}
	bool	LoadFile(char* pszName)
	{
		FILE* t_pFile;
		t_pFile = fopen(pszName, "rb");
		if(!t_pFile)
			return false;
		fseek(t_pFile,0,SEEK_END);
		_size = ftell(t_pFile);
		SAFE_DELETE_ARRAY(_pData);
		_pData = new BYTE[_size];
		fseek(t_pFile,0,SEEK_SET);
		fread(_pData,sizeof(BYTE),_size,t_pFile);
		fclose(t_pFile);
		_lpos = 0;
		return true;
	}
	void	mseek(long ioffset, int ipos)
	{
		if(ipos == SEEK_CUR)
			_lpos += ioffset;
		if(ipos == SEEK_SET)
			_lpos = ioffset;
		if(ipos == SEEK_END)
			_lpos = _size;
	}
	long	mtell()
	{
		return _lpos;
	}
	void	mread(void* pmem,size_t psize, size_t pcount)
	{
		memcpy(pmem,&_pData[_lpos],pcount * psize);
		_lpos += (long)pcount * (long)psize;
	}

protected:
	long    _size;
	BYTE*	_pData;
	long	_lpos;
};
//#include <mindpower.h>

class   MPRender;
////////////////////////////////////////////////////////////////////
class	CMPResManger;
class	CEffectModel;
class	CTexCoordList;
class	CTexList;
class	I_Effect;
class   CEffPath;
class	EffParameter;

//class lwSceneItem ;
//!将最大顶点数量设为40，是为了使用VS来改变顶点,不能使数量大于95 - 15
//每次计算产生的影子顶点数量不能大于这个值
#define  MAX_SHADER_VERNUM		300
#define  MAX_SHADER_IDXNUM		MAX_SHADER_VERNUM  * 3 * 3

/************************************************************************/
/* 效果模型 */
/************************************************************************/
struct SEFFECT_VERTEX
{
	D3DXVECTOR3		m_SPos;
	FLOAT			m_fIdx;//只用于面片的MESH，做为UV间接寻址的索引
	DWORD			m_dwDiffuse;
	D3DXVECTOR2		m_SUV;
};
#define		EFFECT_VER_FVF	(D3DFVF_XYZB1 | D3DFVF_DIFFUSE | D3DFVF_TEX1)


struct SEFFECT_SHADE_VERTEX
{
	D3DXVECTOR3		m_SPos;
	//float			m_fIdx;//只用于阴影的MESH，做为间接寻址的索引
							  //m_fIdx[0]为顶点位置的索引，m_fIdx[1]为uv坐标的索引
	DWORD			m_dwDiffuse;
	D3DXVECTOR2		m_SUV;
	D3DXVECTOR2		m_SUV2;
};
#define		EFFECT_SHADE_FVF	(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2)


#define		MESH_TRI		"Triangle"
#define		MESH_PLANETRI	"TrianglePlane"
#define		MESH_RECT		"Rect"
#define		MESH_RECTZ		"RectZ"
#define		MESH_PLANERECT	"RectPlane"

#define		MESH_CYLINDER	"Cylinder"
#define		MESH_CONE		"Cone"
#define		MESH_SPHERE		"Sphere"


inline	bool IsTobMesh(const s_string& strName)
{
	return ((strName==MESH_CYLINDER)||(strName==MESH_CONE)||(strName==MESH_SPHERE));
}
inline	bool IsCylinderMesh(const s_string& strName)
{
	return (strName==MESH_CYLINDER);
}
inline	bool IsDefaultMesh(const s_string& strName)
{
	static s_string str[] = 
	{
		MESH_TRI		,
		MESH_PLANETRI	,
		MESH_RECT		,
		MESH_RECTZ		,
		MESH_PLANERECT	,
		MESH_CYLINDER	,
		MESH_CONE		,
		MESH_SPHERE		,
	};
	for (int n = 0; n < 8; ++n)
	{
		if(strName == str[n])
			return true;
	}
	return false;
}

#include "MPSceneItem.h"
#include "MPCharacter.h"

#define _MAX_STRING					256
struct ActionInfo
{
	short		m_sActionNO;	// 动作编号
	short		m_sStartFrame;	// 起始帧
	short		m_sEndFrame;	// 结束帧

	short		m_sKeyFrameNum;	// 关键帧个数
	short		*m_sKeyFrame;	// 关键帧集
};
struct SChaAction
{
	short		m_iCharacterType;	// 角色类型
	short		m_iMaxActionNum;	// 角色的最大动作数(从文件中读到的角色的最大动作号)
	short		m_iActualActionNum;	// 角色的有效动作数(从文件中读到的角色的动作个数)
	ActionInfo*  m_SActionInfo;
};
class  MINDPOWER_API CScriptFile
{
private:
	FILE *m_fp;
	bool m_bRead;


	BYTE* m_pData;

	int   m_iSize;

	int   m_iP;

	bool  m_bReadMem;

	bool  m_bOpen;

public:

	CScriptFile();
	~CScriptFile();

	bool OpenFileRead( const char *filename );

	void CloseFile();

	bool ReadSection(  char *section );

	bool ReadLine( const char *name, char *value, int nSize );
	bool ReadLine( const char *name, int *value );
	bool ReadLine( const char *name, float *value );
	bool ReadLine( const char *name, double *value );


	bool ReadLine( const char *name, char* value );

	//////////////////////////////////////////////////////////////////////////
	bool		InitAction(const char *ptcsFileName);
	void		Free(void);

	void		GetMaxCharType(int *iMaxType) {*iMaxType = m_iMaxCharacterType;};
	bool		GetCharAction(int iCharType, SChaAction *SCharAct);

	static		CScriptFile		m_ctScript;
protected:
	short		m_iMaxCharacterType;		// 最大角色类型数(从文件中读到的最大的角色类型号)
	short		m_iActualCharacterType;		// 有效角色类型数(从文件中读到的角色个数)
	SChaAction	*m_SCharacterAction;

};
long StringGetT(_TCHAR *out, long out_max, _TCHAR *in, long *in_from, _TCHAR *end_list, long end_len);
void StringSkipCompartmentT(_TCHAR *in, long *in_from, _TCHAR *skip_list, long skip_len);

//class  lwSceneItem;
LW_USING



struct ModelParam
{
	int   iSegments;
	float fHei;
	float fTopRadius;
	float fBottomRadius;
	std::vector<D3DXVECTOR3> vecVer;
	void	Create();
};

class  MINDPOWER_API CEffectModel : public MPSceneItem
{
public:
	CEffectModel();

	bool Copy(const CEffectModel& rhs);

#ifdef		USE_RENDER
	CEffectModel(MPRender*		  pDev,lwIResourceMgr*	pRes = NULL);
#else
	CEffectModel(LPDIRECT3DDEVICE8  pDev,lwIResourceMgr*	pRes = NULL);
#endif
	virtual ~CEffectModel();

public:
	void	ReleaseModel();

#ifdef USE_RENDER
	void	InitDevice(MPRender*	  pDev,lwIResourceMgr*	pRes = NULL);
#else
	void	InitDevice(LPDIRECT3DDEVICE8  pDev,lwIResourceMgr*	pRes = NULL);
#endif

	bool	CreateTriangle();
	bool	CreatePlaneTriangle();
	bool	CreateRect();
	bool	CreatePlaneRect();
	bool	CreateRectZ();

	//创建阴影模型
	bool	CreateShadeModel(WORD wVerNum = 6, WORD wFaceNum = 2,int iGridCrossNum = 1,bool usesoft = false);

	//创建可变形的模型
	bool	CreateTob(const s_string& str, int nSeg,float fHei,float fTopRadius,float fBottomRadius)
	{
		if(str==MESH_CYLINDER)
			return CreateCylinder(nSeg,fHei,fTopRadius,fBottomRadius);
		if(str==MESH_CONE)
			return CreateCone(nSeg,fHei,fBottomRadius);
		return false;
	}
	bool	CreateCylinder(int nSeg,float fHei,float fTopRadius,float fBottomRadius);
	bool	CreateCone(int nSeg,float fHei,float fRadius);
	//bool	CreateSphere(int nSeg,float fHei,float fRadius);


	bool	IsBoard(){return (!IsChangeably() && !IsItem());}
	bool	IsChangeably()			{return m_bChangeably;}

	bool	IsItem()
	{	
#ifdef USE_MGR
		return _lwMesh ? false : true; 
#else
		return _lpVB ? false : true; 
#endif

	}
	bool	LoadModel(const char* pszName);

	void	Begin();

	void	FrameMove(DWORD dwDailTime);
	void	RenderModel();

	void	RenderTob(ModelParam* last, ModelParam* next, float lerp);

	void	End();

	DWORD	GetVerCount()				{ return _dwVerCount;}
	DWORD	GetFaceCount()				{ return _dwFaceCount;}


#ifdef USE_MGR
	lwILockableStreamIB*		GetIndexBuffer()	
	{
		return _lpSIB;
	}
#else
	LPDIRECT3DINDEXBUFFER8		GetIndexBuffer()	
	{
		return _lpIB;
	}
#endif

#ifdef USE_MGR
	lwILockableStreamVB*		GetVertexBuffer()	
	{
		return _lpSVB;
	}
#else
	LPDIRECT3DVERTEXBUFFER8		GetVertexBuffer()	
	{
		return _lpVB;
	}
#endif

	void						Lock(BYTE** pvEffVer)
	{
#ifdef USE_MGR
		if (_lpSVB == 0)
		{
			*pvEffVer = 0;
			return;
		}

		if(LW_FAILED(_lpSVB->Lock(0, 0, (void**)pvEffVer, 0)))
		{
			MessageBox(NULL, "lock error msglock error", "error", 0);
			*pvEffVer = 0;
			assert(false);
		}
#else
		_lpVB->Lock(0, 0, pvEffVer, 0 );
#endif
	}
	void						Unlock()
	{ 
#ifdef USE_MGR
		_lpSVB->Unlock();
#else
		_lpVB->Unlock();
#endif
	}

	void						LockIB(BYTE** pIdx)
	{
#ifdef USE_MGR
		if(LW_FAILED(_lpSIB->Lock(0, 0, (void**)pIdx, 0)))
		{
			MessageBox(NULL, "lock error msglock error", "error", 0);
			assert(false);
		}
#else
		_lpIB->Lock(0, 0, pIdx, 0 );
#endif
	}
	void						UnlockIB()
	{ 
#ifdef USE_MGR
		_lpSIB->Unlock();
#else
		_lpIB->Unlock();
#endif
	}
	void						SetRenderNum(WORD wVer,WORD wFace);

#ifdef USE_RENDER
	MPRender*	GetDev();
#else
	LPDIRECT3DDEVICE8	GetDev();
#endif
public:
	//!3D设备
#ifdef USE_RENDER
	MPRender*					m_pDev;
#else
	LPDIRECT3DDEVICE8			m_pDev;
#endif

	lwIResourceMgr*				m_pRes;

	s_string					m_strName;

	bool						m_bChangeably;

	int							m_nSegments;
	float						m_rHeight;
	float						m_rRadius;
	float						m_rBotRadius;

	SEFFECT_VERTEX*				m_vEffVer;

	//MPSceneItem*				m_pModel;
	lwITex*						m_oldtex;
	lwITex*						m_oldtex2;
	bool						m_bItem;

	int							m_iID;

	bool						m_bUsing;

protected:
#ifdef USE_MGR
	lwIMesh*					_lwMesh;
	lwILockableStreamVB*		_lpSVB;
	lwILockableStreamIB*		_lpSIB;

#else
	LPDIRECT3DINDEXBUFFER8		_lpIB;
	LPDIRECT3DVERTEXBUFFER8		_lpVB;
#endif

	DWORD						_dwVerCount;
	DWORD						_dwFaceCount;

	//CChaModel*					_pChaModel;

public:
	// Getters and Setters
	bool IsUsing() { return m_bUsing; }
	void SetUsing(bool bUsing) { m_bUsing = bUsing; }
};

/************************************************************************/
/* 纹理坐标变换*/
/************************************************************************/
typedef     std::vector<D3DXVECTOR2>  TEXCOORD;

class CTexCoordList
{
public:
	CTexCoordList();
	~CTexCoordList();

public:
	void		GetCoordFromModel(CEffectModel *pCModel);
	void		CreateTranslateCoord();

	void		GetCurCoord(S_BVECTOR<D3DXVECTOR2>& vecOutCoord, WORD& wCurIndex,float &fCurTime, float fDailTime);

	void		Reset();

	void		Clear();

	void		Copy(CTexCoordList* pList)
	{
		m_wVerCount = pList->m_wVerCount;
		//纹理坐标数.
		m_wCoordCount = pList->m_wCoordCount;
		//纹理坐标变换时间
		m_fFrameTime = pList->m_fFrameTime;

		m_vecCoordList.resize(m_wCoordCount);

		//纹理坐标变换序列
		int n;
		for(n = 0; n < m_wCoordCount; ++n)
		{
			m_vecCoordList[n].resize(m_wVerCount);
			m_vecCoordList[n] =  pList->m_vecCoordList[n];
		}
	}
public:
	//!顶点数量
	WORD		m_wVerCount;
	//纹理坐标数.
	WORD		m_wCoordCount;
	//纹理坐标变换时间
	float		m_fFrameTime;
	//纹理坐标变换序列
	std::vector<TEXCOORD>	m_vecCoordList;

	////!当前坐标索引
	//WORD					m_wCurIndex;
	////!当前时间
	//float					m_fCurTime;
	//!当前的坐标
	//TEXCOORD				m_vecCurCoord;
};

/************************************************************************/
/* 纹理变换*/
/************************************************************************/
class CTexList
{
public:
	CTexList();
	~CTexList();

public:
	void					SetTextureName(const s_string&  pszName);

	void					GetTextureFromModel(CEffectModel *pCModel);

	void					CreateSpliteTexture(int iRow, int iColnum);
	
	void					GetCurTexture(S_BVECTOR<D3DXVECTOR2>& coord, WORD&  wCurIndex,float& fCurTime, float fDailTime);
	void					Reset();

	void					Clear();

	void					Remove();

	void					Copy(CTexList* pList)
	{
		m_wTexCount = pList->m_wTexCount;
		//纹理变换时间
		m_fFrameTime = pList->m_fFrameTime;
		//纹理变换序列
		m_vecTexList.resize(m_wTexCount);
		int n;
		for(n = 0; n < m_wTexCount; ++n)
		{
			m_vecTexList[n].resize(4);
			m_vecTexList[n] =  pList->m_vecTexList[n];
		}

		//!纹理名
		m_vecTexName = pList->m_vecTexName;
		//!当前的纹理指针
		m_lpCurTex = NULL;

		m_pTex = NULL;
	}
public:
	//纹理数
	WORD		m_wTexCount;
	//纹理变换时间
	float		m_fFrameTime;
	//纹理变换序列
	std::vector<TEXCOORD>	m_vecTexList;

	//!纹理名
	s_string	m_vecTexName;
	//!当前的纹理指针
	LPDIRECT3DTEXTURE8		m_lpCurTex;

	lwITex*					m_pTex;

};
/************************************************************************/
/* 纹理变换*/
/************************************************************************/
class CTexFrame
{
public:
	CTexFrame();
	~CTexFrame();

public:
	void					GetCoordFromModel(CEffectModel *pCModel);

	void					AddTexture(const s_string&  pszName);

	lwITex*					GetCurTexture(WORD&  wCurIndex,float& fCurTime, float fDailTime);

	void					Remove();

	void					Copy(CTexFrame* pList)
	{
		m_wTexCount = pList->m_wTexCount;
		m_fFrameTime = pList->m_fFrameTime;
		m_fFrameTime = pList->m_fFrameTime;

		m_vecTexName.resize(m_wTexCount);
		m_vecTexs.resize(m_wTexCount);
		int n;
		for (n = 0; n < m_wTexCount; ++n)
		{
			m_vecTexName[n] = pList->m_vecTexName[n];
		}
		m_vecCoord.resize(pList->m_vecCoord.size());
		m_vecCoord = pList->m_vecCoord;
	}
public:
	//纹理数
	WORD		m_wTexCount;
	//纹理变换时间
	float		m_fFrameTime;
	//!纹理名
	std::vector<s_string>	m_vecTexName;
	//!当前的纹理指针
	lwITex*					m_lpCurTex;

	std::vector<lwITex*>	m_vecTexs;

	TEXCOORD				m_vecCoord;
};

class CEffectFont : public CTexList, CEffectModel
{
public:
	CEffectFont();
	~CEffectFont();

public:
#ifdef USE_RENDER
	bool	CreateEffectFont(MPRender*  pDev,
		CMPResManger	*pCResMagr,int iTexID,D3DXCOLOR dwColor, bool bUseBack = false,bool bmain =false);
#else
	bool	CreateEffectFont(LPDIRECT3DDEVICE8  pDev,
		CMPResManger	*pCResMagr,int iTexID,D3DXCOLOR dwColor, bool bUseBack = false,bool bmain = false);
#endif
	
	void	SetRenderText(char* pszText);

	void	RenderEffectFont(D3DXMATRIX* pmat);
	void	RenderEffectFontBack(D3DXMATRIX* pmat);

protected:
	s_string	_strText;
	int			_iTextNum;
	std::vector<int>	_vecCurText;
	int			_iTextureID;

	//背景图
	bool		_bUseBack;
	s_string	_strBackBmp;
#ifdef USE_RENDER
	lwITex*					_lpBackTex;
#else
	LPDIRECT3DTEXTURE8		_lpBackTex;
#endif

//#ifdef USE_MGR
//	lwIMesh*					_lwBackMesh;
//#else
//	LPDIRECT3DVERTEXBUFFER8	_lpBackVB;
//#endif
	SEFFECT_VERTEX			t_SEffVer[4];


	D3DXCOLOR				_dwColor;

};

enum  EFFECT_TYPE
{
	EFFECT_NONE			= 0,
	EFFECT_FRAMETEX		= 1,
	EFFECT_MODELUV		= 2,
	EFFECT_MODELTEXTURE	= 3,
	EFFECT_MODEL		= 4,
};
#define ENUMTOSTR(s)  #s

/************************************************************************/
/* Effect base class */
/************************************************************************/
class MINDPOWER_API I_Effect
{
public:
	I_Effect(void);
	~I_Effect(void);
public:

	void DestroyTobMesh(CMPResManger* resMgr);

	//!	释放全部资源
	virtual void ReleaseAll();
	//!
	virtual void Reset();
	//！初始化
#ifdef USE_RENDER
	virtual	void Init(MPRender*	 pDev,EFFECT_TYPE  eType,
		WPARAM wParam, LPARAM lParam);
#else
	virtual	void Init(LPDIRECT3DDEVICE8	 pDev,EFFECT_TYPE  eType,
		WPARAM wParam, LPARAM lParam);
#endif

	void		 SetTexture();			
	//{ 
	//	if(m_pCModel->IsItem())
	//	{
	//		lwITex* tex;
	//		m_pCModel->ResetItemTexture(m_CTextruelist.m_pTex,&tex);
	//	}else
	//		m_pDev->SetTexture(0, m_CTextruelist.m_lpCurTex);
	//}
	void		 SetVertexShader();//		{ m_pDev->SetVertexShader(*_pdwVShader);}
	//!渲染
	virtual void Begin()
	{
		if (m_pCModel)
			m_pCModel->Begin();
	}
	virtual void Render();				
	//{
	//	m_pDev->SetRenderState( D3DRS_SRCBLEND,_eSrcBlend );
	//	m_pDev->SetRenderState( D3DRS_DESTBLEND,_eDestBlend);
	//	m_pCModel->RenderModel();
	//}
	virtual void End()								
	{
		if (m_pCModel)
			m_pCModel->End();
	}

//!成员函数
public:
	//! 得到设置桢数量
	WORD			getFrameCount(){ return _wFrameCount;}
	WORD			setFrameCount(WORD  wFrameCount){ return _wFrameCount = wFrameCount;}

	//!得到类型
	EFFECT_TYPE		getType()						{ return _eEffectType;}
	void			setType(EFFECT_TYPE eType)		{ _eEffectType =  eType;}

	//!得到长度
	float			getLength()						{ return _fLength;}
	void			setLength(float fLength)		{ _fLength = fLength;}

	//!得到帧时间
	float			getFrameTime(WORD wIndex)		{ return _vecFrameTime[wIndex];}
	void			setFrameTime(WORD wIndex,float fTime){ _vecFrameTime[wIndex] = fTime;}

	//!得到帧大小
	D3DXVECTOR3		getFrameSize(WORD wIndex)		{ return _vecFrameSize[wIndex];}
	void			setFrameSize(WORD wIndex,D3DXVECTOR3& SVerSize){ _vecFrameSize[wIndex] = SVerSize;}

	//!得到帧角度
	D3DXVECTOR3&	getFrameAngle(WORD wIndex)		{ return _vecFrameAngle[wIndex];}
	void			setFrameAngle(WORD wIndex,D3DXVECTOR3& SVerAngle){_vecFrameAngle[wIndex]=SVerAngle;}

	//!得到帧位置
	D3DXVECTOR3&	getFramePos(WORD wIndex)		{ return _vecFramePos[wIndex];}
	void			setFramePos(WORD wIndex,D3DXVECTOR3& SVerPos){_vecFramePos[wIndex]=SVerPos;}

	//!得到帧颜色
	D3DXCOLOR&		getFrameColor(WORD wIndex)		{ return _vecFrameColor[wIndex];}
	void			setFrameColor(WORD wIndex,D3DXCOLOR& SVerColor){_vecFrameColor[wIndex]=SVerColor;}

	//!得到帧坐标时间
	float	getFrameCoordTime()						{ return m_CTexCoordlist.m_fFrameTime; }
	void    setFrameCoordTime(float fTime)			{ m_CTexCoordlist.m_fFrameTime = fTime;}

	int		getFrameCoordCount()					{ return (int)m_CTexCoordlist.m_vecCoordList.size(); }
	void	setFrameCoordCount(int iNum)					
	{ 
		m_CTexCoordlist.m_wCoordCount = iNum;
		m_CTexCoordlist.m_vecCoordList.resize(m_CTexCoordlist.m_wCoordCount);
	}

	//!得到帧坐标
	void	getFrameCoord(TEXCOORD& vecOutCoord, WORD wIndex)
	{
		vecOutCoord.clear();
		vecOutCoord.resize(m_CTexCoordlist.m_wVerCount);
		for(WORD n = 0; n < m_CTexCoordlist.m_wVerCount; ++n)
		{
			vecOutCoord[n] = m_CTexCoordlist.m_vecCoordList[wIndex][n];
		}
	}
	void	setFrameCoord(TEXCOORD& vecInCoord, WORD wIndex)
	{
		//for(WORD n = 0; n < m_CTexCoordlist.m_wVerCount; ++n)
		//{
		//	m_CTexCoordlist.m_vecCoordList[wIndex][n] = vecInCoord[n];
		//}
		m_CTexCoordlist.m_vecCoordList[wIndex] = vecInCoord;
	}

	//!得到帧纹理
	TEXCOORD& getFrameTexture(WORD  wIndex)
	{
		return m_CTextruelist.m_vecTexList[wIndex];
	}
	void	setFrameTexture(WORD  wIndex,TEXCOORD& lptex)
	{
		m_CTextruelist.m_vecTexList[wIndex] = lptex;
	}
	void    SpliteTexture(int iRow, int iCol);
	void	SetTextureTime(float ftime);

	///////////////////////////////////////////////////////////
	bool	 IsModelRect()		{ return m_strModelName == MESH_RECT;}
	bool	 IsModelPlaneRect()	{ return m_strModelName == MESH_PLANERECT;}
	bool	 IsModelTri()		{ return m_strModelName == MESH_TRI;}

	bool	IsItem()
	{
		if(m_pCModel)
			return m_pCModel->IsItem();
		if(strstr( m_strModelName.c_str(),".lgo"))
			return true;
		return false;
	}
		//{ return m_pCModel->IsItem();}

	bool	IsChangeably();

	//////////////////////////////////////////////////////////////////////////
	//!得到插值大小
	void	GetLerpSize(D3DXVECTOR3 *pSOut, WORD wIdx1, WORD wIdx2, float fLerp)
	{
		if(_wFrameCount == 1 || _bSizeSame)
		{	*pSOut = _vecFrameSize[0];return;}
		D3DXVec3Lerp(pSOut, &_vecFrameSize[wIdx1], &_vecFrameSize[wIdx2], fLerp);
	}
	//!得到插值角度
	void	GetLerpAngle(D3DXVECTOR3 *pSOut, WORD wIdx1, WORD wIdx2, float fLerp)
	{
		if(_wFrameCount == 1 || _bAngleSame)
		{	*pSOut = _vecFrameAngle[0];return;}
		D3DXVec3Lerp(pSOut, &_vecFrameAngle[wIdx1], &_vecFrameAngle[wIdx2], fLerp);
	}
	//!得到插值位置
	void	GetLerpPos(D3DXVECTOR3 *pSOut, WORD wIdx1, WORD wIdx2, float fLerp)
	{
		if(_wFrameCount == 1 || _bPosSame)
		{	*pSOut =   _vecFramePos[0];return; }
		D3DXVec3Lerp(pSOut, &_vecFramePos[wIdx1], &_vecFramePos[wIdx2], fLerp);
	}
	//!得到插值颜色
	void	GetLerpColor(D3DXCOLOR *pSOut, WORD wIdx1, WORD wIdx2, float fLerp)
	{
		if(_wFrameCount == 1 || _bColorSame)
		{	*pSOut =  _vecFrameColor[0];return; }
		D3DXColorLerp( pSOut, &_vecFrameColor[wIdx1], &_vecFrameColor[wIdx2], fLerp );
	}
	//!得到插值坐标
	void	GetLerpCoord(S_BVECTOR<D3DXVECTOR2>& vecOutCoord, WORD& wCurIndex,float &fCurTime, float fDailTime)
	{
		m_CTexCoordlist.GetCurCoord(vecOutCoord,wCurIndex,fCurTime,fDailTime);
	}
	void GetLerpTexture(S_BVECTOR<D3DXVECTOR2>& vecOutCoord, WORD&  wCurIndex,float& fCurTime, float fDailTime)
	{
		m_CTextruelist.GetCurTexture(vecOutCoord, wCurIndex,fCurTime,fDailTime);
	}
	void GetLerpFrame(WORD&  wCurIndex,float& fCurTime, float fDailTime)
	{
		m_CTexFrame.GetCurTexture(wCurIndex,fCurTime,fDailTime);
	}

	void GetLerpVertex(WORD wIdx1, WORD wIdx2, float fLerp)
	{
		m_ilast = wIdx1;
		m_inext = wIdx2;
		m_flerp = fLerp;
	}

	void GetRotaLoopMatrix(D3DXMATRIX* pmat, float& pCurRota, float fTime)
	{
		pCurRota += _vRotaLoop.w * fTime;
		if(pCurRota >= 6.283185f)
			pCurRota = pCurRota - 6.283185f;
		D3DXMatrixRotationAxis(pmat,
			&D3DXVECTOR3(_vRotaLoop.x,_vRotaLoop.y,_vRotaLoop.z), pCurRota);
	}

	//////////////////////////////////////////////////////////////////////////
	//!关联到资源
	void		 BindingResInit(CMPResManger	*m_CResMagr);
	//!返回0表示正常装入，1表示缺少帖图，2表示缺少模型。3表示少shade
	int 		 BoundingRes(CMPResManger	*m_CResMagr, char* pszParentName = "temp");

	s_string	 GetTextureName()		{ return m_CTextruelist.m_vecTexName;}

	void		 SetTextureName(const s_string&  pszName)		{ m_CTextruelist.SetTextureName(pszName);}

	void		 SetModel(CEffectModel*	pCModel);
	//！替换纹理
	void		 ChangeTexture(const s_string&  pszName)
	{
		if(_eEffectType == EFFECT_FRAMETEX)
		{

		}else
		{
			SetTextureName(pszName);	
			m_CTextruelist.GetTextureFromModel(m_pCModel);
		}
	}
	//!替换模型
	void		 ChangeModel(CEffectModel*	pCModel,CMPResManger	*pCResMagr);

	///////////////////////////////////////////////////////////////////////////
	//!得到效果名称
	s_string&	 getEffectName()							{ return m_strEffectName;}
	void		 setEffectName(const s_string& strName)		{ m_strEffectName = strName;}

	//!得到模型名称
	s_string&	 getEffectModelName();
	void		 setEffectModelName(const s_string& strModelName)	{ m_strModelName = strModelName;}

	//!设置BILLBOARD矩阵
	void		 setBillBoardMatrix(D3DXMATRIX* pMatBBoard)	{ _SpmatBBoard = pMatBBoard;}
	D3DXMATRIX*	 getBillBoardMatrix()						{ return _SpmatBBoard;}

	bool		 IsBillBoard()								{ return _bBillBoard;}

	void		 SetLoop(bool	bloop)						{ _bRotaLoop = bloop; }
	bool		 IsRotaLoop()								{ return _bRotaLoop;}
	D3DXVECTOR4* GetRotaLoop()								{ return &_vRotaLoop;}

	bool		 IsAlpah()									{ return _bAlpha;}
	void		 EnableAlpha(bool balpha)					{ _bAlpha = balpha;}


	D3DBLEND	GetAlphaTypeSrc()	{ return _eSrcBlend;}
	D3DBLEND	GetAlphaTypeDest()	{ return _eDestBlend;}

	void		SetAlphaType(D3DBLEND eSrcBlend, D3DBLEND eDestBlend)
									{ _eSrcBlend = eSrcBlend; _eDestBlend = eDestBlend;}

	//!保存效果到文件	
	bool		 SaveToFile(FILE* pFile);
	//!装入效果从文件
	bool		 LoadFromFile(FILE* pFile,DWORD dwVersion);

	void		 IsSame();

	void		 RemoveTexBack()
	{
		m_CTextruelist.Remove();
	}
	void		 AddFrameTex(const s_string& str)
	{
		if(_eEffectType == EFFECT_FRAMETEX)
		{
			m_CTexFrame.AddTexture(str);
		}
	}
	VEC_string&		 GetFrameTex()
	{
		//if(_eEffectType == EFFECT_FRAMETEX)
		{
			return m_CTexFrame.m_vecTexName;
		}
	}
	void			RemoveFrameTex()
	{
		m_CTexFrame.Remove();
	}
	
	void	InitTopParam()
	{
		if(_iUseParam > 0)
			return;
		_iUseParam = 1;
	}
	void	ClearTopParam(){ _iUseParam = 0; }
	void	SetTobParam(int nFrame, int	nSegments,float	rHeight,float rRadius,float	rBotRadius)
	{
		_iUseParam = 0;
		_CylinderParam[nFrame].iSegments = nSegments;
		_CylinderParam[nFrame].fTopRadius = rRadius;
		_CylinderParam[nFrame].fBottomRadius = rBotRadius;
		_CylinderParam[nFrame].fHei = rHeight;
		_CylinderParam[nFrame].Create();

		_iUseParam = 1;
	}
	void	GetTobParam(int nFrame, int&	nSegments,float&	rHeight,float& rRadius,float&	rBotRadius)
	{
		 nSegments = _CylinderParam[nFrame].iSegments;
		rRadius = _CylinderParam[nFrame].fTopRadius;
		rBotRadius = _CylinderParam[nFrame].fBottomRadius ;
		rHeight = _CylinderParam[nFrame].fHei;
	}
	int		IsUseParam(){ return _iUseParam;}

	bool	IsRotaBoard(){ return _bRotaBoard;}
	void	SetRoatBoard(bool bRota){ _bRotaBoard = bRota;}


	void	GetRes(CMPResManger	*pCResMagr,std::vector<INT>& vecTex,std::vector<INT>& vecModel);

	void	PlayModel()
	{
		if (!m_pCModel) return;
		if(m_pCModel->IsItem())
			m_pCModel->PlayDefaultAnimation();
	}
	void	ResetModel();
	void	CopyEffect(I_Effect* pEff);

	void	DeleteItem(CMPResManger* pResMgr);

//！成员变量
public:
	//!3D设备
#ifdef		USE_RENDER
	MPRender*					m_pDev;
#else
	LPDIRECT3DDEVICE8			m_pDev;
#endif
	//!纹理坐标列表
	CTexCoordList				m_CTexCoordlist;
	//!纹理列表
	CTexList					m_CTextruelist;
	//!纹理帧
	CTexFrame					m_CTexFrame;

	CEffectModel*				m_pCModel;
	s_string					m_strModelName;

	int							m_nSegments;
	float						m_rHeight;
	float						m_rRadius;
	float						m_rBotRadius;

	s_string					m_strEffectName;

	int							m_ilast,m_inext;
	float						m_flerp;
protected:
	//!	动画类型
	EFFECT_TYPE			_eEffectType;
	//！动画的时间长度(秒)
	float				_fLength; 
	//！动画桢数量
	WORD				_wFrameCount;
	//! 每一桢的时间
	VEC_float			_vecFrameTime;
	//!	每一桢的大小(缩放的倍数，默认为1.0f)
	std::vector<D3DXVECTOR3>			_vecFrameSize;
	//!	每一桢的角度
	std::vector<D3DXVECTOR3>			_vecFrameAngle;
	//!	每一桢的位置
	std::vector<D3DXVECTOR3>			_vecFramePos;

	//!	每一桢的顶点颜色(默认为0xffffffff)
	std::vector<D3DXCOLOR>				_vecFrameColor;

	//!	每一桢的顶点变换
	INT									_iUseParam;
	std::vector<ModelParam>				_CylinderParam;

	bool				_bBillBoard;
	D3DXMATRIX*			_SpmatBBoard;

	bool				_bRotaLoop;
	D3DXVECTOR4			_vRotaLoop;

	bool				_bRotaBoard;


	bool				_bAlpha;

	int					_iVSIndex;
	DWORD*				_pdwVShader;

	bool				_bSizeSame;
	bool				_bAngleSame;
	bool				_bPosSame;
	bool				_bColorSame;

	D3DBLEND			_eSrcBlend;
	D3DBLEND			_eDestBlend;
};

//class MINDPOWER_API CEffectBase
//{
//public:
//	CEffectBase(){}
//	~CEffectBase(){}
//public:
//	virtual void FrameMove(DWORD	dwDailTime)	{}
//	//!渲染
//	virtual void Render()						{}
//
//	virtual void RenderVS()						{}
//
//	virtual void RenderSoft()					{}
//
//
//protected:
//private:
//};

inline void Transpose(D3DMATRIX &result, D3DMATRIX &m)
{

	result.m[0][0] = m.m[0][0];  result.m[0][1] = m.m[1][0];  result.m[0][2] = m.m[2][0]; result.m[0][3] = m.m[3][0];
	result.m[1][0] = m.m[0][1];  result.m[1][1] = m.m[1][1];  result.m[1][2] = m.m[2][1]; result.m[1][3] = m.m[3][1];
	result.m[2][0] = m.m[0][2];  result.m[2][1] = m.m[1][2];  result.m[2][2] = m.m[2][2]; result.m[2][3] = m.m[3][2];
	result.m[3][0] = m.m[0][3];  result.m[3][1] = m.m[1][3];  result.m[3][2] = m.m[2][3]; result.m[3][3] = m.m[3][3];
}