#pragma once
#include "UIGlobalVar.h"
#include "UILabel.h"
#include "FindPath.h"
#include "UICheckBox.h"
namespace GUI
{
// 小地图
class CMiniMapMgr : public CUIInterface
{
public:
	CMiniMapMgr() : 
	  frmMinimap(0), MinimatRect(0), labMapPos(0), labMapName(0), frmBigmap(0){}


	// 用于小地图更新时得到Form大小
	CCompent*	GetMinimapRect()			{ return MinimatRect;	}

	void		RefreshChaPos( int x, int y );
	void        RefreshMapName (const char * name );

	C3DCompent*	GetBigmapRect()				{ return d3dBigmap;		}

	bool		IsShowBigmap();

	CForm*		GetBigmapForm()				{ return frmBigmap;		}

	void		ClearRadar();

	void        CloseRadar();

	const char* GetMapName()				{ return labMapName->GetCaption(); }

	bool		IsPKSilver();
	bool		IsGuildWar();


	CLabelEx*		GetLabMapPos(){return labMapPos;}
protected:
	virtual bool Init();
    virtual void End();
	virtual void SwitchMap();

private:
	static void _MiniFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _RadarFormEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

	static void _RenderBigmapEvent(C3DCompent *pSender, int x, int y);
	static void _evtShowbigmap(CGuiData *pSender);
	static void _evtHidebigmap(CGuiData *pSender);

	//雷达界面的enter事件
	static void _evtRadarEnter(CGuiData *pSender);
	static void _RadarFormBeforeShowEvent(CForm* pForm, bool& IsShow);

	static void ShowRadar();

	// 检查坐标输入框是否有非法字符(只支持整数) 正确返回0,其他的暂定为-1
	static int CheckCoordinateEdit(const char* input);

	// 绘制大地图提示
    void _RenderBigMapHint(void);//Add by sunny.sun 20080903

	//小地图表单
	CForm*		frmMinimap;
	CCompent*	MinimatRect;

	enum 
	{
		MAP_POS_MAX = 10,
	};
    //小地图控件
	CLabelEx	*labMapPosRand[MAP_POS_MAX];
	CLabelEx	*labMapPos;
	CLabelEx    *labMapName;
	
	CForm*		frmBigmap;
	C3DCompent*	d3dBigmap;

	static CForm*		frmRadar;
	static CEdit*		edtX;
	static CEdit*		edtY;
	static CCombo*		cboAddr;
	//CMPFont				g_CFont;
};

}

