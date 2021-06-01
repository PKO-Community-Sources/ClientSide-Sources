#pragma once
#include "UIGlobalVar.h"

namespace GUI
{


#define ERROR_DATA -99999999

/**
 * The system properties class. It takes charge load or save the system properties.
 * @author Michael Chen 
 * @time 2005-4-19
 */
class CSystemProperties
{
public:
	struct SVideo
	{
		int nTexture;
		bool bAnimation;
		bool bCameraRotate;
		//bool bViewFar;        //取消视野远近(Michael Chen 2005-04-22
		bool bGroundMark;
		bool bDepth32;
		int  nQuality;
		bool bFullScreen;
		bool bPixel1024;

		SVideo();

	} m_videoProp;

	struct Audio
	{
		int nMusicSound;
		int nMusicEffect;

		Audio() : nMusicSound(0), nMusicEffect(0) {}
	} m_audioProp;

	struct SGameOption {
		bool bRunMode;
		bool bLockMode;
		bool bHelpMode;
		bool bAppMode;
		bool bEffMode;
		bool bStateMode;
	} m_gameOption;

	// Add by lark.li 20080826 begin
	struct StartOption {
		bool bFirst;

		StartOption() : bFirst(true) {}
	} m_startOption;
	// End

public:
	CSystemProperties() {}
	~CSystemProperties() {}
	/**
	 * Load the propties from the file(*.ini)
	 * @param: szIniFileName The name of ini file.
	 * @return: success Return 0.
	 */
	int Load(const char * szIniFileName)
	{
		return readFromFile(szIniFileName);
	}
	/**
	 * Save the propties to the file(*.ini)
	 * @param: szIniFileName The name of ini file.
	 * @return: success Return 0.
	 */
	int Save(const char * szIniFileName)
	{
		return writeToFile(szIniFileName);
	}

	/**
	* 将系统配置在游戏中生效.
	* @return: success Return 0.
	*          video failure Return -1.
	*          audio failure Return -2.
	*          gameOption failureboth Return -3.
	*		   other failure Return -4.
	*/
    int Apply();

    /**
    * 将系统的视频配置在游戏中生效.
    * @return: success Return 0.
    */
    int ApplyVideo();
    /**
    * 将系统音频配置在游戏中生效.
    * @return: success Return 0.
    */
    int ApplyAudio();
	/**
	* 将系统音频配置在游戏中生效.
	* @return: success Return 0.
	*/
	int ApplyGameOption();

private:
	/**
	 * The help function of reading the propties from the file(*.ini).
	 * @param: szIniFileName The name of ini file
	 * @return: success Return 0.
	 */
	int readFromFile(const char * szIniFileName);
	/**
	 * The help function of write the propties to the file(*.ini).
	 * @param: szIniFileName The name of ini file.
	 * @return: success Return 0.
	 */
	int writeToFile(const char * szIniFileName);
	
	bool int2bool(int n);

	int	bool2int(bool b);

	BOOL WriteInteger( const char* szSection, const char* szKey, int value, const char* szFileName )
	{
		char szbuf[50] = { 0 };
		itoa( value, szbuf, 10 );

		return WritePrivateProfileString( szSection, szKey, szbuf, szFileName );
	}


};

class CChaExitOnTime
{
public:
	CChaExitOnTime();

public:	// 玩家操作
	void	ChangeCha();	// 更换角色
	void	ExitApp();		// 退出程序
	void	Relogin();		// 重新登录

	void	Cancel();		// 取消以上操作

	void	FrameMove(DWORD dwTime);

	bool	TimeArrived();

	void	Reset();

public:	// 网络返回
	void	NetStartExit( DWORD dwExitTime );	// 开始计时
	void	NetCancelExit();					// 已被取消计时

private:
	bool	_IsTime();				// 正在计时中,玩家不能操作

private:
	enum eOptionType
	{
		enumInit,
		enumChangeCha,
		enumExitApp,
		enumRelogin,
	};

	eOptionType		_eOptionType;
	DWORD			_dwStartTime;
	DWORD			_dwEndTime;
	bool			_IsEnabled;

};

extern CChaExitOnTime g_ChaExitOnTime;

// 改变图象,声音,退出系统等
class CSystemMgr : public CUIInterface
{
public:
    CSystemMgr();
	CForm*			GetSystemForm()		{ return frmSystem;			}

    void LoadCustomProp();

public:
	virtual void CloseForm();

protected:
	virtual bool Init();
    virtual void End();
	virtual void FrameMove(DWORD dwTime);

private:
	static void		_evtVideoChangeChange(CGuiData *pSender);
	static void		_evtMainMusicMouseDown(CGuiData *pSender,int x,int y ,DWORD key);		//处理音效

	static void		_evtVideoFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void		_evtSystemFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void		_evtAudioFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

	static void		_evtAskReloginFormMouseDown(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void		_evtAskExitFormMouseDown(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void		_evtAskChangeFormMouseDown(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void		_evtGameOptionFormMouseDown(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void		_evtGameOptionFormBeforeShow(CForm* pForm, bool& IsShow);

private:
	CForm			 *frmSystem;

	CForm            *frmAudio;
	CProgressBar     *proAudioMusic;
	CProgressBar     *proAudioMidi;

	CForm            *frmVideo;
	CCheckGroup      *cbxTexture;
	CCheckGroup      *cbxMovie;
	CCheckGroup      *cbxCamera;
	//CCheckGroup      *cbxView;        //取消视野远近(Michael Chen 2005-04-22
	CCheckGroup      *cbxTrail;
	CCheckGroup      *cbxColor;
	CCheckGroup      *cbxSize;
	CCheckGroup      *cbxModel;
	CCheckGroup      *cbxQuality;

	CForm			*frmGameOption;
	CCheckGroup		*cbxRunMode;
	CCheckGroup		*cbxLockMode;
	CCheckGroup		*cbxHelpMode;
	CCheckGroup		*cbxAppMode;
	CCheckGroup		*cbxEffMode;
	CCheckGroup		*cbxStateMode;

	CForm			*frmAskRelogin;
	CForm			*frmAskExit;
	CForm			*frmAskChange;

    
public:
    CSystemProperties   m_sysProp;
    bool                m_isLoad;

};

}

