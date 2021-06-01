#pragma once

#include "Scene.h"
#include "uiguidata.h"
#include "NetProtocol.h"

#define MAX_SEL_CHA			4
#define MAX_USERNAME_LEN	64
#define MAX_PASSWORD_LEN	64

namespace GUI
{
	class CForm;
	class C3DCompent;
	class CGuiData;
	class CLabelEx;
	class CEdit;
	class CTextButton;
	class CList;
	class CMemo;
	class CCheckBox;
	class CListView;
	class CImage;
}

extern bool registerLogin;
extern char autoLoginName[32];
extern char autoLoginPassword[32] ;
extern bool useAutoLogin;

extern bool useModelMode;
extern bool modelMode;
extern char modelLook[8192];


class CCharacter;

	struct TOM_SERVER
	{
		TOM_SERVER()
			: szPassport("tom")
		{
			nPort = 0;
			bEnable=false;
		}

		string	szServerIP;
		int		nPort;
		string	szUser;
		string	szPassword;
		string	szPassport;
		bool	bEnable;
	};

//  �����̱��
//  By Jampe
#define COP_OURGAME                 1001        //  ����
#define COP_SINA                    1002        //  ����
#define COP_CGA                     1003        //  �Ʒ�

#define NUM_REGIN_LIST				2
#define NUM_SERVR_LIST				2


    typedef struct _Cooperate_
    {
        long code;
        string serv;
        string uid;
        string pwd;
    }Cooperate, *pCooperate;

	extern TOM_SERVER	g_TomServer;
    extern Cooperate    g_cooperate;
//#endif

class CLoginScene : public CGameScene
{
public:	
    CLoginScene(stSceneInitParam& param);

	~CLoginScene();

    void LoginFlow();
    void Select( int num, NetChaBehave chabehave[] );
    void NewCha();
    void DelCha();
	static bool IsValidCheckChaName(const char *name);
    void CloseNewChaFrm();
	void ReSetNewCha();

    static void BeginPlay();

	static int nSelectChaType;      // ����ѡ�н�ɫ����Ϣ
    static int nSelectChaPart[5];   // �����5����λ


	static void ShowPathLogo(int isShow);

    void Error( int error_no, const char* error_info );
    void ShowServerList();
	void ShowRegionList();
	void ShowChaList();
    void PlayWhalePose();

	void SetCurSelRegionIndex(int iIndex) { m_iCurSelRegionIndex = iIndex; }
	int	 GetCurSelRegionIndex() { return m_iCurSelRegionIndex; }
	void SetCurSelServerIndex(int iIndex) { m_iCurSelServerIndex = iIndex; }
	int	 GetCurSelServerIndex() { return m_iCurSelServerIndex; }
	void ShowLoginForm();

	bool IsPasswordError()		{ return m_bPasswordError;}
	void SetPasswordError(bool bFlag)	{ m_bPasswordError = bFlag; }
	void InitRegionList();
	void InitServerList(int nRegionNo);

	static CForm*			frmRegion;
	static CCharacter* modelCha;


private:

	void SaveUserName(CCheckBox& chkID, CEdit& edtID);

	int  GetServIconIndex(int iNum);

	static void ShowKeyboard(bool bShow);
	
	static void		__cha_render_event( C3DCompent *pSender, int x, int y);

protected:
	
	virtual bool _Init();
	virtual void _FrameMove(DWORD dwTimeParam);
    virtual void _Render();
	virtual void LoadingCall();

	BOOL            _InitUI();


    bool		    _Clear();
	BOOL			_bAutoInputAct;

    void			_Login();
	bool			_CheckAccount();
	bool			_Bill();
	void			_Connect();

	//CSceneObj*	pObj;
private:

	static CForm*			frmServer;
	static CForm*			frmAccount;
	static CForm*           frmLOGO;
	static CForm*           frmAnnounce;
	static CForm*           frmKeyboard;	// add by Philip.Wu  ����̽���  2006-06-05
	static CForm*           frmRegister;	// add by Philip.Wu  ����̽���  2006-06-05
	static CForm*			frmPathLogo;	// ���½�����Ƭ LOGO

	static CList*			lstRegion[NUM_REGIN_LIST];
	static CListView*		lstServer[NUM_SERVR_LIST];

	static CEdit*			edtID;
	static CEdit*           edtPassword;
	static CCheckBox*		chkID;
	static CEdit*			edtFocus;		// add by Philip.Wu  ��꼤��ı༭��  2006-06-07
	static CCheckBox*       chkShift;		// add by Philip.Wu  ������ϵ� Shift  2006-06-09

	static CImage*          imgLogo1;		// add by Philip.Wu  LOGOͼ1     2006-06-20
	static CImage*          imgLogo2;		// add by Philip.Wu  LOGOͼ2     2006-06-20

	static const int		ServIconNum = 4;
	CImage*					imgServerIcons;

	int						m_iCurSelRegionIndex;
	int						m_iCurSelServerIndex;
	bool					m_bSaveAccount;
	string					m_sSaveAccount;
	string					m_sPassport;
	string					m_sUsername;
	string					m_sPassword;

	const char *			m_szSelServIp;

	CSceneObj*	    pObj;
	bool			IsLoad;

private:
	static void _evtEnter(CGuiData *pSender);	
	static void _evtServerLDBDown(CGuiData *pSender, int x, int y, DWORD key);
	static void _evtRegionLDBDown(CGuiData *pSender, int x, int y, DWORD key);
	static void _evtServerFrmBeforeShow(CForm* pForm, bool& IsShow);
	static void _evtServerFrmOnClose(CForm* pForm, bool& IsClose);

	static void  _evtServerFrm(CCompent *pSender, int state, int x, int y, DWORD key);
	static void  _evtRegionFrm(CCompent *pSender, int state, int x, int y, DWORD key); 
	static void  _evtLoginFrm(CCompent *pSender, int state, int x, int y, DWORD key); 
	static void  _evtVerErrorFrm(CCompent *pSender, int state, int x, int y, DWORD key); 

	static void  CallbackUIEvent_LoginScene( CCompent *pSender, int state, int x, int y, DWORD key);

	// add by Philip.Wu  2006-06-05
	// ����̽��������¼�
	static void _evtKeyboardFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

	// �༭�򼤻��¼��������¼���ı༭��
	static void _evtAccountFocus(CGuiData* pSender);


	enum eLoginState { enumInit, enumConnect, enumAccount, enumLogin, enumSelect, enumPlay };

	eLoginState     _eState;
    BYTE            _loadtex_flag;
    BYTE            _loadmesh_flag;

	bool			m_bPasswordError;	//�ж������Ƿ����

	static const int IMAGE_INDEX = 0;
	static const int TEXT_INDEX = 1;

};
