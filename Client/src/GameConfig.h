#pragma once

#include <string>

struct SPlaceCha 
{
	int nTypeID;
	int nX;
	int nY;
};

class CGameConfig
{
public:	
	CGameConfig();
	
	void	Load(char *pszFileName); // �������ļ�
	void	SetDefault();  //Ĭ������
	void	SetMoveClient( bool v ); //�Ƿ���ͻ���ͬ��
	
public:

	// ��ע��˴���Ҫ�����κηǹ̶��ڴ�ߴ�ı���, ����ģ��, string֮���,
	// ���ཫ��ֱ��д�̺Ͷ���
	BOOL		m_bAutoLogin;
	BOOL		m_bFullScreen;
	SPlaceCha	m_ChaList[20];
	int			m_nChaCnt;
	float		m_LightDir[3];
	float		m_LightColor[3];
    BOOL        m_bNoObj;
    BOOL        m_bEditor;
    BYTE        m_btScreenMode;

	int			m_iFogR;
	int			m_iFogG;
	int			m_iFogB;
	float		m_fExp2;


    int         m_nMaxChaType;
    int         m_nMaxSceneObjType;
    int         m_nMaxEffectType;
	int			m_nMaxResourceNum;
    int         m_nMaxItemType;
    BOOL        m_bEnableMusic;
	BOOL		m_bCheckOvermax;

	// ����
    BOOL        m_nSendHeartbeat;       // ����ʱ�䷢��һ������,��λ:s,��Сֵ10s,�ر�0
	DWORD		m_nConnectTimeOut;		// �������ӳ�ʱ

    BOOL        m_bEnableLG;            // �Ƿ��������LG��Ϣ
    BOOL        m_bEnableLGMsg;         // �Ƿ�������LG-Box
	BOOL		m_bMThreadRes;			// ���߳���Դ����


	int		m_nCreateScene;

	//{lemon add@2004.9.16
	float	m_fCameraVel;
	float	m_fCameraAccl;

	//lemon add@2004.11.26, ��½ר��
	float	eyeX;
	float   eyeY;      
	float	eyeZ;      
	float   refX;      
	float   refY;      
	float   refZ;
	float   fov;
	//lemon add@2005.1.24
	float   fAspect;

	float	fnear;
	float	ffar;
	
    float   m_fLgtFactor;
    DWORD   m_dwLgtBkColor;

    // lh add@2004.12.8���ڵ�������������
    int     nLeftHand;
    int     nRightHand;


	BOOL	m_bRenderSceneObj;
	BOOL    m_bRenderCha;
	BOOL    m_bRenderEffect;
	BOOL    m_bRenderTerrain;
	BOOL	m_bRenderUI;
	BOOL    m_bRenderMinimap;
	BOOL	m_bRender;

    DWORD   m_dwFullScreenAntialias;

	// ���������л���ͼʱ�Ĳ���
	DWORD	m_dwMaxCha;
	DWORD	m_dwMaxEff;
	DWORD	m_dwMaxItem;
	DWORD	m_dwMaxObj;

	char	m_szMD5Pass[48];	// ��д��MD5����

	bool	m_IsShowConsole;	// �Ƿ���Բ����ؼ�̨
	bool	m_IsTomServer;		// ��½Tom������

	bool	m_IsMoveClient;		// �Ƿ�ͻ��˷�ͬ����·

	char	m_szVerErrorHTTP[256];	// �汾��ƥ��ʱ�����õ���ҳ

	bool	m_IsBill;

	bool	IsPower()		{ return m_IsShowConsole;	}		// ����Ǳ༭��ģʽ,�Զ����Բ����ؼ�̨

	bool	m_IsDoublePwd;	// �Ƿ��ж�������

	// Add by lark.li 20080429 for res
	char   m_szLocale[256];
	char   m_szResPath[256];

	char   m_szFontName1[50];
	char   m_szFontName2[50];
	// End

	// Add by lark.li 20080825 begin
	int		m_nMovieW;
	int		m_nMovieH;
	// End
};


extern CGameConfig g_Config;

