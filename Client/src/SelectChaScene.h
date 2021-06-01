#pragma once

#include "Scene.h"
#include "uiguidata.h"
#include "NetProtocol.h"

class CSceneObj;
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
}

/* extern char autoLoginChar[32] ;
extern bool canAutoLoginChar; */
/**
 * 摘要：本类主要处理登陆流程中的选择角色场景,包括场景动画和GUI表单.
 * 　　　本类继承自CGameScene类.
 *
 * @author: Michael Chen
 * @date: 2005-04-26
 */
class CSelectChaScene : public CGameScene
{
public:
    CSelectChaScene(stSceneInitParam& param);
    ~CSelectChaScene();

    virtual void        LoadingCall();          // 在装载loading后,刷新
    virtual void		SetMainCha(int nChaID);

    static CSelectChaScene& GetCurrScene();

    //删除当前选中的角色
    void                DelCurrentSelCha();
    //显示现有角色
    bool                SelectCharacters(NetChaBehave* chabehave, int num);
    //新增角色
    bool                CreateCha(const string& sName, int nChaIndex, stNetChangeChaPart* part);

    void                SelectChaError( int error_no, const char* error_info );

    //向服务器发送删除角色消息
    void SendDelChaToServer(const char szPassword2[]);

	// 更新按钮状态
    void UpdateButton();

	// 获得角色个数
	int GetChaCount();

	void ShowWelcomeNotice(bool bShow = true);

protected:
    //场景相关函数
    virtual void		_FrameMove(DWORD dwTimeParam);
    virtual void		_Render();

    virtual bool		_MouseButtonDown(int nButton);
    virtual bool		_MouseButtonDB(int nButton);
	virtual void		_KeyDownEvent( int key );


    virtual bool		_Init();	
    virtual bool		_Clear();
    virtual void        _RenderUI();

    //UI相关函数
    bool                _InitUI();
	static void			_OnClickUpdatePass(CGuiData *pSender, int x, int y, DWORD key);
	static void			_OnClickGMLogin(CGuiData *pSender, int x, int y, DWORD key);
    static void         _SelChaFrmMouseEvent(CCompent *pSender, int nMsgType, 
                                             int x, int y, DWORD dwKey);

//	static void         _CheckFrmMouseEvent(CCompent *pSender, int nMsgType, 
//                                           int x, int y, DWORD dwKey);

    static void         _evtCreateDoublePwdEvent(CCompent *pSender, int nMsgType, 
                                            int x, int y, DWORD dwKey);		// 询问是否要创建二次密码

    static void         _evtWelcomeNoticeEvent(CCompent *pSender, int nMsgType, 
                                            int x, int y, DWORD dwKey);		// 欢迎界面 事件处理

    static void         _evtCreateOKNoticeEvent(CCompent *pSender, int nMsgType, 
                                            int x, int y, DWORD dwKey);		// 首次创建角色成功提示界面 事件处理

    static void         _evtChaNameAlterMouseEvent(CCompent *pSender, int nMsgType, 
                                            int x, int y, DWORD dwKey);		// 首次创建角色成功提示界面 事件处理

	//~ 逻辑函数: ============================================================


    //向服务器发送开始游戏消息
    void SendBeginPlayToServer();

    void SetChaDark(CCharacter* pCha);

private:
    struct ChaFont
    {
		ChaFont() : pCha(NULL), iLevel(-1), sProfession(""), iPos(-1), iFontX(-1), iFontY(-1) {}
        CCharacter * pCha;
        int          iLevel;
        string       sProfession;
        int          iPos;
		int			 iFontX;
		int			 iFontY;
    };
    typedef vector<ChaFont*> ChaFontPtrContainer;
    typedef ChaFontPtrContainer::iterator ChaFontPtrContainerIter;

    ChaFontPtrContainer m_CharactorPtrs;        //Characters in the scene (including fonts)
    vector<int>         m_XPositions;           //Scene X coordinates of the three characters in the scene
    vector<int>         m_YPositions;           //Y coordinates of the three characters in the scene
    vector<int>         m_ZPositions;
    vector<int>         m_Yaws;                 //The rotation of the characters in the scene
    vector<int>         m_FreePositions;        //The index of the empty position of the scene
    BYTE                m_ChaColors[3][3];      //人物的颜色
	int					m_nChaRotates[3];       //人物的偏移角度(-180~+180)


    //场景渲染和动画相关的
    BYTE            _loadtex_flag;          //保存场景渲染和动画的设置
    BYTE            _loadmesh_flag;         //保存场景渲染和动画的设置

    CSceneObj*	    pObj;                   //场景对象
    lwIPrimitive*   pAure[3];


    //UI
    CForm*			frmSelectCha; 
    CTextButton*    btnDel;
    CTextButton*    btnYes;
    CTextButton*    btnCreate;
    CTextButton*    btnExit;
	CTextButton*    btnAlter;
	CTextButton*    btnChangePassConf;

    CForm*          frmCheck;

    //逻辑
    int             m_nCurChaIndex;

    //
    bool            m_isInit;

	bool			m_isCreateCha;

	CForm*			frmWelcomeNotice;	// 定义欢迎界面  该界面仅在当前帐号内无角色时出现
	CForm*			frmCreateOKNotice;	// 定义首次创建角色成功提示界面  该界面仅在该帐号走完第一个角色的创建流程后显示
	CForm*			frmChaNameAlter;	// 定义改名界面  该界面仅对台湾版本有效，请做好备份工作
};
